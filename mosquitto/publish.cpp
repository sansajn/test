// MQTT mosquitto subscriber sample
#include <string>
#include <string_view>
#include <exception>
#include <chrono>
#include <iostream>
#include <boost/format.hpp>
#include <mosquitto.h>
#include <cstring>

using std::runtime_error;
using std::cout,
	std::endl;
using std::string;
using std::string_view;
using std::chrono::steady_clock;
using namespace std::chrono_literals;
using namespace std::string_view_literals;
using boost::format;

void mqtt_connect_callback(mosquitto * mosq, void * userdata, int result);

int main(int argc, char * argv[])
{
	// initialization
	constexpr string_view host = "localhost";
	constexpr int port = 1883;
	constexpr int keepalive = 60;
	constexpr string_view topic = "test/topic"sv;

	mosquitto_lib_init();

	bool connected = false;
	mosquitto *  mosq = mosquitto_new(nullptr, true, (void *)&connected);
	if (!mosq)
		throw runtime_error{"mosquitto: API function failed"};

	mosquitto_connect_callback_set(mosq, mqtt_connect_callback);

	if (mosquitto_connect(mosq, host.data(), port, keepalive) != MOSQ_ERR_SUCCESS)
		throw runtime_error{
			boost::str(format("mosquitto: unable connect to %1%:%2%") % host % port)};

	// wait for connection done ...
	while (!connected)
		mosquitto_loop(mosq, 10, 1);

	cout << "connected to broaker" << endl;

	size_t counter = 1;

	auto t0 = steady_clock::now();
	while (true)
	{
		// pulish
		if (steady_clock::now() > t0 + 1s)
		{
			string payload = boost::str(format("hello %1%") % counter);
			if (mosquitto_publish(mosq, nullptr, topic.data(), payload.size(), payload.c_str(), 2, false) != MOSQ_ERR_SUCCESS)
				throw runtime_error{boost::str(format("publishing '%1%' failed") % payload)};
			counter += 1;
			t0 = steady_clock::now();
		}

		mosquitto_loop(mosq, 10, 1);
	}

	cout << "done!\n";

	return 0;
}

void mqtt_connect_callback(mosquitto * mosq, void * userdata, int result)
{
	if (!result)
		*((bool *)userdata) = true;  // connected
}
