// http get sample
#include <string_view>
#include <iostream>
#include <libsoup/soup.h>

using std::string_view, std::cout, std::endl;

int main(int argc, char * argv[])
{
	// note: GMainLoop is not required to get sample working

	// initialize
	SoupSession * session = soup_session_new();

	// connect
	SoupMessage * request = soup_message_new("GET", "http://localhost:8000");
	soup_session_send_message(session, request);

	string_view content{request->response_body->data, request->response_body->length};
	cout << content << endl;

	cout << "done!\n";
	return 0;
}
