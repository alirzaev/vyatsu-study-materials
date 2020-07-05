#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>

const unsigned int MAX_QUEUE_SIZE = 5,
                   MAX_CLIENTS = 100;
unsigned int q_size = 0, count = 0;
std::mutex q_mutex, io_mutex, c_mutex;

bool test_count() {
	std::lock_guard<std::mutex> c_lock(c_mutex);
	return count < MAX_CLIENTS;
}

void work(int num) {
	while (test_count()) {
		bool ok = false;
		{
			std::lock_guard<std::mutex> q_lock(q_mutex);
			if (q_size > 0) {
				std::lock_guard<std::mutex> c_lock(c_mutex);
				count++;
				q_size--;
				ok = true;

			}
		}
		if (ok) {
			{
				std::lock_guard<std::mutex> io_lock(io_mutex);
				std::cout << "Server #" << num << " got client" << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(800));

			{
				std::lock_guard<std::mutex> io_lock(io_mutex);
				std::cout << "Server #" << num << " has served client" << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
}

void add_client() {
	while (test_count())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::lock_guard<std::mutex> q_lock(q_mutex);

		if (q_size < MAX_QUEUE_SIZE) {
			q_size++;

			std::lock_guard<std::mutex> io_lock(io_mutex);
			std::cout << "New client arrived. " << "In queue: " << q_size << std::endl;
		}
	}
}

int main() {
	auto start = std::chrono::system_clock::now();
	auto w1 = std::thread(work, 1);
	auto w2 = std::thread(work, 2);
	auto w3 = std::thread(work, 3);
	auto w4 = std::thread(work, 4);
	auto a = std::thread(add_client);
	w1.join();
	w2.join();
	w3.join();
	w4.join();
	a.join();
	auto stop = std::chrono::system_clock::now();
	std::cout
		<< MAX_CLIENTS << " clients was served, "
		<< std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds was spent"
		<< std::endl;
	std::cin.get();
	return 0;
}
