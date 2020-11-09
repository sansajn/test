/*! mqtt mosquitto subscriber implementation for unit testing purpose
*/
#include <map>
#include <string>
#include <queue>
#include <chrono>
#include <iostream>
#include <boost/format.hpp>
#include <mosquitto.h>

using std::string;
using std::map;
using std::queue;
using std::cout;
using std::endl;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;
using namespace std::chrono_literals;
using boost::format;


using queue_map = std::map<std::string, std::queue<std::string>>;

/*! MQTT subscriber for testing purpose
\code
mqtt_subscriber test;
test.subscribe("test/topic");

string result;
if (test.recv_for(result, seconds{5}))
	cout << result << "\n";  // something received
\endcode */
class mqtt_subscriber
{
public:
	//! \param[in] timeout connect timeout
	mqtt_subscriber(std::chrono::seconds timeout = 5s);
	~mqtt_subscriber();
	bool subscribe(std::string const & topic);
	bool recv_for(std::string & result, std::chrono::milliseconds timeout);

private:
	typename queue_map::iterator _queue_it;
};


int main(int argc, char * argv[])
{
	string const topic = "test/topic";
	seconds const timeout = 20s;

	mqtt_subscriber test;
	test.subscribe(topic);
	cout << "subscribed to '" << topic << "' topic with " << timeout.count() << "s timeout" << endl;

	string msg;
	bool alive = true;
	while (alive)
	{
		alive = test.recv_for(msg, timeout);
		if (alive)
			cout << msg << endl;
	}

	cout << "done, no content for " << timeout.count() << "s.\n";

	return 0;
}


// subscriber implementation

void mqtt_message_callback(mosquitto * mosq, void * userdata, mosquitto_message const * message);
void mqtt_connect_callback(mosquitto * mosq, void * userdata, int result);


mosquitto * __mosq = nullptr;
queue_map _topic_mapping;

void mqtt_message_callback(mosquitto * mosq, void * userdata, mosquitto_message const * message)
{
	auto it = _topic_mapping.find(message->topic);
	if (it != _topic_mapping.end())
		it->second.push((char const *)message->payload);
}

void mqtt_connect_callback(mosquitto * mosq, void * userdata, int result)
{
	if (!result)
		*((bool *)userdata) = true;  // connected
}


mqtt_subscriber::mqtt_subscriber(seconds timeout)
{
	char const * host = "localhost";
	int port = 1883;
	int keepalive = 60;

	if (!__mosq)
	{
		mosquitto_lib_init();
		bool connected = false;
		__mosq = mosquitto_new(nullptr, true, (void *)&connected);
		if (!__mosq)
			throw std::runtime_error{"mosquitto: API function failed"};

		mosquitto_connect_callback_set(__mosq, mqtt_connect_callback);
		mosquitto_message_callback_set(__mosq, mqtt_message_callback);

		if (mosquitto_connect(__mosq, host, port, keepalive) != MOSQ_ERR_SUCCESS)
			throw std::runtime_error{
				boost::str(format("mosquitto: unable connect to %1:%2") % host % port)};

		// wait for connection done ...
		while (!connected)
			mosquitto_loop(__mosq, 10, 1);
	}
}

mqtt_subscriber::~mqtt_subscriber()
{
	_topic_mapping.erase(_queue_it);

	if (_topic_mapping.empty())
	{
		mosquitto_destroy(__mosq);
		mosquitto_lib_cleanup();
	}
}

bool mqtt_subscriber::subscribe(string const & topic)
{
	auto it = _topic_mapping.find(topic);
	if (it != _topic_mapping.end())
		return true;  // known topic

	if (mosquitto_subscribe(__mosq, nullptr, topic.c_str(), 2) != MOSQ_ERR_SUCCESS)
		return false;

	auto result = _topic_mapping.insert(make_pair(topic, queue<string>{}));
	assert(result.second);

	_queue_it = result.first;

	return true;
}

bool mqtt_subscriber::recv_for(string & result, milliseconds timeout)
{
	steady_clock::time_point now = steady_clock::now();
	steady_clock::time_point until = now + timeout;
	while (now < until)
	{
		if (mosquitto_loop(__mosq, 10, 1) != MOSQ_ERR_SUCCESS)
			return false;

		queue<string> & q = _queue_it->second;

		if (!q.empty())
		{
			result = q.front();
			q.pop();
			return true;
		}

		now = steady_clock::now();
	}

	return false;  // timeout
}
