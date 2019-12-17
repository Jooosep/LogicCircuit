#pragma once
#include <vector>
#include <thread>
#include <functional>

class ThreadPool
{
public:
	explicit ThreadPool(std::size_t numThreads);
	~ThreadPool();
private:
	std::vector<std::thread> mThreads;
	void start(std::size_t numThreads);
	void stop() noexcept;
};

