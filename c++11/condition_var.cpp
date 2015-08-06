/* clang++ condition_var.cpp -o condition_var -std=x++11 -pthread */
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>


int main(int argc, char * argv[])
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false;
    bool notified = false;

	 std::thread producer([&]() {
		  for (int i = 0; i < 5; ++i) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::unique_lock<std::mutex> lock(m);
				std::cout << "producing " << i << '\n';
				produced_nums.push(i);
				notified = true;
				cond_var.notify_one();
		  }   

		  notified = true;
		  done = true;
		  cond_var.notify_one();
	 }); 

    std::thread consumer([&]() {
        std::unique_lock<std::mutex> lock(m);
        while (!done) {
            while (!notified) {  // loop to avoid spurious wakeups
                cond_var.wait(lock);
            }   
            while (!produced_nums.empty()) {
                std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
            }   
            notified = false;
        }   
    }); 

    producer.join();
    consumer.join();

    return 0;
}
