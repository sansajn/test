// serve page to a browser sample
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>
#include <libsoup/soup.h>

using std::string, std::cout, std::endl;
using namespace std::chrono_literals;

constexpr int SERVER_PORT = 40001;

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data);

int main(int argc, char * argv[])
{
	SoupServer * server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "test soap server", nullptr);
	assert(server);

	soup_server_add_handler(server, "/", server_http_handler, nullptr, nullptr);
	soup_server_listen_all(server, SERVER_PORT, (SoupServerListenOptions)0, nullptr);

	cout << "page link: http://127.0.0.1:" << SERVER_PORT << "\n";

	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	g_main_loop_run(loop);

	// clean up
	g_object_unref(G_OBJECT(server));
	g_main_loop_unref(loop);

	cout << "done!\n";

	return 0;
}

void server_http_handler(SoupServer * server, SoupMessage * message,
	char const * path, GHashTable * query, SoupClientContext * client, gpointer user_data)
{
	static string page_source = "<html><body>Hello!</body></html>";

	SoupBuffer * page_buf = soup_buffer_new(SOUP_MEMORY_STATIC, page_source.c_str(), size(page_source));
	assert(page_buf);

	soup_message_headers_set_content_type(message->response_headers, "text/html", nullptr);
	soup_message_body_append_buffer(message->response_body, page_buf);

	soup_buffer_free(page_buf);

	soup_message_set_status(message, SOUP_STATUS_OK);
}
