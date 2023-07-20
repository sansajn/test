// Single client WebRTC (h264) stream server sample.
#include <string>
#include <thread>
#include <chrono>
#include <string_view>
#include <iostream>
#include <filesystem>
#include <cassert>
#include <csignal>
#include <boost/filesystem/string_file.hpp>
#include <boost/stacktrace.hpp>
#include <spdlog/spdlog.h>
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

// GStreamer
#include <gst/gst.h>
#include <gst/sdp/sdp.h>
#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>

using std::string_view, std::string, std::cout, std::endl, std::signal;
using namespace std::chrono_literals;
using namespace std::string_literals;
using boost::filesystem::load_string_file;

namespace fs = std::filesystem;

#define RTP_PAYLOAD_TYPE "96"
#define SOUP_HTTP_PORT 57778
#define STUN_SERVER "stun.l.google.com:19302"

constexpr int SERVER_PORT = 40001;
constexpr char const * default_page_file = "client.html";

string page_source;  // content of page-file

void new_client_request();

// Soap server handlers
void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data);

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data);

void websocket_closed_handler(SoupWebsocketConnection * connection, gpointer user_data);

struct ReceiverEntry {
	SoupWebsocketConnection * connection;
	GstElement *pipeline;
	GstElement *webrtcbin;
};

void soup_websocket_message_cb(G_GNUC_UNUSED SoupWebsocketConnection * connection,
	SoupWebsocketDataType data_type, GBytes * message, gpointer user_data) {

	gsize size;
	gchar *data;
	gchar *data_string;
	const gchar *type_string;
	JsonNode *root_json;
	JsonObject *root_json_object;
	JsonObject *data_json_object;
	JsonParser *json_parser = NULL;
	ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

	switch (data_type) {
		case SOUP_WEBSOCKET_DATA_BINARY:
			g_error ("Received unknown binary message, ignoring\n");
			return;

		case SOUP_WEBSOCKET_DATA_TEXT:
			data = (gchar *)g_bytes_get_data(message, &size);
			/* Convert to NULL-terminated string */
			data_string = g_strndup(data, size);
			break;

		default:
			g_assert_not_reached ();
	}

	json_parser = json_parser_new ();
	if (!json_parser_load_from_data (json_parser, data_string, -1, NULL))
		goto unknown_message;

	root_json = json_parser_get_root (json_parser);
	if (!JSON_NODE_HOLDS_OBJECT (root_json))
		goto unknown_message;

	root_json_object = json_node_get_object (root_json);

	if (!json_object_has_member (root_json_object, "type")) {
		g_error ("Received message without type field\n");
		goto cleanup;
	}
	type_string = json_object_get_string_member (root_json_object, "type");

	if (!json_object_has_member (root_json_object, "data")) {
		g_error ("Received message without data field\n");
		goto cleanup;
	}
	data_json_object = json_object_get_object_member (root_json_object, "data");

	if (g_strcmp0 (type_string, "sdp") == 0) {
		const gchar *sdp_type_string;
		const gchar *sdp_string;
		GstPromise *promise;
		GstSDPMessage *sdp;
		GstWebRTCSessionDescription *answer;
		int ret;

		if (!json_object_has_member (data_json_object, "type")) {
			g_error ("Received SDP message without type field\n");
			goto cleanup;
		}
		sdp_type_string = json_object_get_string_member (data_json_object, "type");

		if (g_strcmp0 (sdp_type_string, "answer") != 0) {
			g_error ("Expected SDP message type \"answer\", got \"%s\"\n",
						sdp_type_string);
			goto cleanup;
		}

		if (!json_object_has_member (data_json_object, "sdp")) {
			g_error ("Received SDP message without SDP string\n");
			goto cleanup;
		}
		sdp_string = json_object_get_string_member (data_json_object, "sdp");

		// g_print ("Received SDP:\n%s\n", sdp_string);
		spdlog::debug("Received SDP answer");

		ret = gst_sdp_message_new (&sdp);
		g_assert_cmphex (ret, ==, GST_SDP_OK);

		ret =
				gst_sdp_message_parse_buffer ((guint8 *) sdp_string,
														strlen (sdp_string), sdp);
		if (ret != GST_SDP_OK) {
			g_error ("Could not parse SDP string\n");
			goto cleanup;
		}

		answer = gst_webrtc_session_description_new (GST_WEBRTC_SDP_TYPE_ANSWER,
																	sdp);
		g_assert_nonnull (answer);

		promise = gst_promise_new ();
		g_signal_emit_by_name (receiver_entry->webrtcbin, "set-remote-description",
									  answer, promise);
		gst_promise_interrupt (promise);
		gst_promise_unref (promise);
		gst_webrtc_session_description_free (answer);
	} else if (g_strcmp0 (type_string, "ice") == 0) {
		guint mline_index;
		const gchar *candidate_string;

		if (!json_object_has_member (data_json_object, "sdpMLineIndex")) {
			g_error ("Received ICE message without mline index\n");
			goto cleanup;
		}
		mline_index =
				json_object_get_int_member (data_json_object, "sdpMLineIndex");

		if (!json_object_has_member (data_json_object, "candidate")) {
			g_error ("Received ICE message without ICE candidate string\n");
			goto cleanup;
		}
		candidate_string = json_object_get_string_member (data_json_object,
																		  "candidate");

		// g_print ("Received ICE candidate with mline index %u; candidate: %s\n",
		// 			mline_index, candidate_string);
		spdlog::debug("Received ICE candidate with mline index:{}; candidate: {}", 
			mline_index, candidate_string);

		g_signal_emit_by_name (receiver_entry->webrtcbin, "add-ice-candidate",
									  mline_index, candidate_string);
	} else
		goto unknown_message;

cleanup:
	if (json_parser != NULL)
		g_object_unref (G_OBJECT (json_parser));
	g_free (data_string);
	return;

unknown_message:
	g_error ("Unknown message \"%s\", ignoring", data_string);
	goto cleanup;
}

// WebRTC negotiation SDP/ICE handlers
void on_offer_created_cb(GstPromise * promise, gpointer user_data);
void on_negotiation_needed_cb(GstElement * webrtcbin, gpointer user_data);
void on_ice_candidate_cb([[maybe_unused]] GstElement * webrtcbin, guint mline_index,
	gchar * candidate, gpointer user_data);

// WebRTC pipeline functions
ReceiverEntry * create_receiver_entry(SoupWebsocketConnection * connection);
void destroy_receiver_entry(gpointer receiver_entry_ptr);

// helpers
void crash_handler(int signal);
void quit_handler(int signal);  // ctrl+c handler
gchar * get_string_from_json_object(JsonObject * object);
void quit_loop();
void print_gstreamer_version();

GMainLoop * loop = nullptr;
ReceiverEntry * pipeline = nullptr;  // pipeline


int main(int argc, char * argv[]) {
	setenv("OPENSSL_CONF", "", 1);   // set OPENSSL_CONF="" to get WebRTC working against GStreamer 1.14 on Debian 10

	string const page_file = argc > 1 ? argv[1] : default_page_file;
	spdlog::info("page: {}", page_file);

	if (!fs::exists(page_file)) {
		spdlog::critical("page file '{}' can't be opened", page_file);
		return 1;
	}

	gst_init(&argc, &argv);
	print_gstreamer_version();

	load_string_file(page_file, page_source);

	SoupServer * server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "test soap server", nullptr);
	assert(server);

	soup_server_add_handler(server, "/", server_http_handler, nullptr, nullptr);
	soup_server_add_websocket_handler(server, "/ws", nullptr, nullptr, server_websocket_handler, nullptr, nullptr);
	soup_server_listen_all(server, SERVER_PORT, (SoupServerListenOptions)0, nullptr);

	spdlog::info("page link: http://127.0.0.1:{}", SERVER_PORT);

	loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	signal(SIGSEGV|SIGFPE, crash_handler);  // install signal handler
	signal(SIGINT, quit_handler);  // install ctrl+c signal handler

	g_main_loop_run(loop);  // blocking

	spdlog::info("main loop stopped");

	// clean up
	if (pipeline) {
		destroy_receiver_entry(pipeline);
		pipeline = nullptr;
	}

	g_object_unref(G_OBJECT(server));
	g_main_loop_unref(loop);

	gst_deinit();

	cout << "done!\n";

	return 0;
}

void new_client_request(SoupWebsocketConnection * connection) {
	spdlog::info("client connected");

	assert(!pipeline && "we expect receiver_entry not yet initialized");
	pipeline = create_receiver_entry(connection);
	assert(pipeline);
}

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data) {
	
	spdlog::info("http request received, path:{}", path);

	if (string_view{path} == "/") {
		SoupBuffer * page_buf = soup_buffer_new(SOUP_MEMORY_STATIC, page_source.c_str(), size(page_source));
		assert(page_buf);

		soup_message_headers_set_content_type(message->response_headers, "text/html", nullptr);
		soup_message_body_append_buffer(message->response_body, page_buf);

		soup_buffer_free(page_buf);

		soup_message_set_status(message, SOUP_STATUS_OK);
	}
	else
		soup_message_set_status(message, SOUP_STATUS_NOT_FOUND);
}

void server_websocket_handler(SoupServer * server, SoupWebsocketConnection * connection,
	char const * path, SoupClientContext * client, gpointer user_data) {

	g_signal_connect(G_OBJECT(connection), "closed", G_CALLBACK(websocket_closed_handler), nullptr);

	new_client_request(connection);
}

void websocket_closed_handler(SoupWebsocketConnection * connection, gpointer user_data) {
	gushort close_code = soup_websocket_connection_get_close_code(connection);
	spdlog::info("client disconnected, what: {}", close_code);
	destroy_receiver_entry(pipeline);
	pipeline = nullptr;

	// quit_loop();  // TODO; just for a valgrind output
}

gchar * get_string_from_json_object(JsonObject * object) {
	JsonNode *root;
	JsonGenerator *generator;
	gchar *text;

	/* Make it the root node */
	root = json_node_init_object (json_node_alloc (), object);
	generator = json_generator_new ();
	json_generator_set_root (generator, root);
	text = json_generator_to_data (generator, NULL);

	/* Release everything */
	g_object_unref (generator);
	json_node_free (root);
	return text;
}


ReceiverEntry * create_receiver_entry(SoupWebsocketConnection * connection) {
  GError *error;
  ReceiverEntry *receiver_entry;
  GstWebRTCRTPTransceiver *trans;
  GArray *transceivers;

  receiver_entry = (ReceiverEntry *)g_slice_alloc0 (sizeof (ReceiverEntry));
  receiver_entry->connection = connection;

  g_object_ref (G_OBJECT (connection));

  g_signal_connect (G_OBJECT (connection), "message",
      G_CALLBACK (soup_websocket_message_cb), (gpointer) receiver_entry);

  string const source = "videotestsrc is-live=true pattern=ball ! ";

  string const format = "webrtcbin name=webrtcbin stun-server=stun://"s +
    STUN_SERVER " " +
    source +
    "videoconvert ! queue max-size-buffers=1 ! x264enc bitrate=600 speed-preset=ultrafast tune=zerolatency key-int-max=15 ! video/x-h264,profile=constrained-baseline ! queue max-size-time=100000000 ! h264parse ! " +
    "rtph264pay config-interval=-1 name=payloader ! " +
    "application/x-rtp,media=video,encoding-name=H264,payload=" RTP_PAYLOAD_TYPE " ! "
    "webrtcbin. ";
  
  char pipe_desc[4096] = {0};
  sprintf(pipe_desc, format.c_str());

//   g_print("pipeline: %s\n", pipe_desc);
  spdlog::info("pipeline created");

  error = NULL;
  receiver_entry->pipeline =
      gst_parse_launch (pipe_desc, &error);
  if (error != NULL) {
    g_error ("Could not create WebRTC pipeline: %s\n", error->message);
    g_error_free (error);
    goto cleanup;
  }

  receiver_entry->webrtcbin =
      gst_bin_get_by_name (GST_BIN (receiver_entry->pipeline), "webrtcbin");
  g_assert (receiver_entry->webrtcbin != NULL);

  g_signal_emit_by_name (receiver_entry->webrtcbin, "get-transceivers",
      &transceivers);
  g_assert (transceivers != NULL && transceivers->len > 0);
  trans = g_array_index (transceivers, GstWebRTCRTPTransceiver *, 0);
  trans->direction = GST_WEBRTC_RTP_TRANSCEIVER_DIRECTION_SENDONLY;
  g_array_unref (transceivers);

  g_signal_connect (receiver_entry->webrtcbin, "on-negotiation-needed",
      G_CALLBACK (on_negotiation_needed_cb), (gpointer) receiver_entry);

  g_signal_connect (receiver_entry->webrtcbin, "on-ice-candidate",
      G_CALLBACK (on_ice_candidate_cb), (gpointer) receiver_entry);

  gst_element_set_state (receiver_entry->pipeline, GST_STATE_PLAYING);

  return receiver_entry;

cleanup:
  destroy_receiver_entry ((gpointer) receiver_entry);
  return NULL;
}

void destroy_receiver_entry(gpointer receiver_entry_ptr) {
  ReceiverEntry *receiver_entry = (ReceiverEntry *) receiver_entry_ptr;

  g_assert (receiver_entry != NULL);

  if (receiver_entry->pipeline != NULL) {
    gst_element_set_state (GST_ELEMENT (receiver_entry->pipeline),
        GST_STATE_NULL);

    gst_object_unref (GST_OBJECT (receiver_entry->webrtcbin));
    gst_object_unref (GST_OBJECT (receiver_entry->pipeline));
  }

  if (receiver_entry->connection != NULL)
    g_object_unref (G_OBJECT (receiver_entry->connection));

  g_slice_free1 (sizeof (ReceiverEntry), receiver_entry);
}

void on_offer_created_cb(GstPromise * promise, gpointer user_data) {
	gchar *sdp_string;
	gchar *json_string;
	JsonObject *sdp_json;
	JsonObject *sdp_data_json;
	GstStructure const *reply;
	GstPromise *local_desc_promise;
	GstWebRTCSessionDescription *offer = NULL;
	ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

	reply = gst_promise_get_reply (promise);
	gst_structure_get (reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION,
							 &offer, NULL);
	gst_promise_unref (promise);

	local_desc_promise = gst_promise_new ();
	g_signal_emit_by_name (receiver_entry->webrtcbin, "set-local-description",
								  offer, local_desc_promise);
	gst_promise_interrupt (local_desc_promise);
	gst_promise_unref (local_desc_promise);

	sdp_string = gst_sdp_message_as_text (offer->sdp);
	// g_print ("Negotiation offer SDP created:\n%s\n", sdp_string);
	spdlog::debug("Negotiation offer SDP created");

	sdp_json = json_object_new ();
	json_object_set_string_member (sdp_json, "type", "sdp");

	sdp_data_json = json_object_new ();
	json_object_set_string_member (sdp_data_json, "type", "offer");
	json_object_set_string_member (sdp_data_json, "sdp", sdp_string);
	json_object_set_object_member (sdp_json, "data", sdp_data_json);

	json_string = get_string_from_json_object (sdp_json);
	json_object_unref (sdp_json);

	soup_websocket_connection_send_text (receiver_entry->connection, json_string);
	g_free (json_string);
	g_free (sdp_string);

	gst_webrtc_session_description_free (offer);
}

void on_negotiation_needed_cb(GstElement * webrtcbin, gpointer user_data) {
	GstPromise *promise;
	ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

	// g_print ("Creating negotiation offer\n");
	spdlog::debug("Creating negotiation offer SDP");

	promise = gst_promise_new_with_change_func (on_offer_created_cb,
															  (gpointer) receiver_entry, NULL);
	g_signal_emit_by_name (G_OBJECT (webrtcbin), "create-offer", NULL, promise);
}

void on_ice_candidate_cb(G_GNUC_UNUSED GstElement * webrtcbin, guint mline_index,
	gchar * candidate, gpointer user_data) {
	JsonObject *ice_json;
	JsonObject *ice_data_json;
	gchar *json_string;
	ReceiverEntry *receiver_entry = (ReceiverEntry *) user_data;

	ice_json = json_object_new ();
	json_object_set_string_member (ice_json, "type", "ice");

	ice_data_json = json_object_new ();
	json_object_set_int_member (ice_data_json, "sdpMLineIndex", mline_index);
	json_object_set_string_member (ice_data_json, "candidate", candidate);
	json_object_set_object_member (ice_json, "data", ice_data_json);

	json_string = get_string_from_json_object (ice_json);
	json_object_unref (ice_json);

	soup_websocket_connection_send_text (receiver_entry->connection, json_string);
	g_free (json_string);
}

void crash_handler(int signal) {
	cout << strsignal(signal) << " signal caught\n"
		<< "Stack trace:\n"
		<< boost::stacktrace::stacktrace{}
		<< endl;

	exit(signal);
}

void quit_handler(int signal) {
	spdlog::warn("ctrl+c ({}) signal catched", strsignal(signal));
	g_main_loop_quit(loop);
}

void quit_loop() {
	g_main_loop_quit(loop);
}

void print_gstreamer_version() {
	guint major, minor, micro, nano;
	gst_version(&major, &minor, &micro, &nano);
	
	gchar const * nano_str = nullptr;
	if (nano == 1)
		nano_str = "(CVS)";
	else if (nano == 2)
		nano_str = "(Prerelease)";
	else
		nano_str = "";
	
	spdlog::info("linked against GStreamer {}.{}.{} {}", major, minor, micro, nano_str);
}
