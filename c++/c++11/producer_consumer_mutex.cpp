// producer, consumer priklad v ktorom konzument meni hodnotu zdielaneho droja
#include <chrono>
#include <mutex>
#include <thread>
#include <iostream>

using std::cout;

static std::mutex the_lock;

void produce(int * data)
{
	for (int i = 1; i <= 5; ++i)
	{
		std::lock_guard<std::mutex> lock{the_lock};
		*data = i;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds{500});
}

int main(int argc, char * argv[])
{
	int data = 0, val = 0;

	std::thread t{produce, &data};

	// consumer
	do 
	{
		{
			std::lock_guard<std::mutex> lock{the_lock};
			if (data != 5)
				data += 12;
			val = data;
		}

		cout << "current value " << val << std::endl;
	} 
	while (val != 5);

	t.join();

	return 0;
}
