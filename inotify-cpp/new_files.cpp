// notify on new, removed files in a directory
#include <vector>
#include <iostream>
#include <inotify-cpp/NotifierBuilder.h>

using std::vector;
using std::cout;
namespace fs = boost::filesystem;


int main(int argc, char * argv[])
{
	auto notifycation_handler = [](inotify::Notification notification) {
		std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(notification.time.time_since_epoch());
		std::chrono::milliseconds mills = std::chrono::duration_cast<std::chrono::milliseconds>(notification.time.time_since_epoch() - sec);
		cout << "event " << notification.event << " on " << notification.path
			<< " at " << sec.count() << "s," << mills.count() << "ms was triggered" << std::endl;
	};

	vector<inotify::Event> events = {
		inotify::Event::close_write, inotify::Event::create, inotify::Event::remove};

	fs::path path = (argc > 1) ? argv[1] : ".";

	cout << "directory " << path << " watched ..." << std::endl;

	inotify::NotifierBuilder notifier = inotify::BuildNotifier()
		.watchPathRecursively(path)
		.onEvents(events, notifycation_handler);

	std::thread t{[&]{notifier.run();}};

	std::this_thread::sleep_for(std::chrono::seconds{600});
	notifier.stop();
	t.join();

	return 0;
}
