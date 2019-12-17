#include "ThreadPool.h"



ThreadPool::ThreadPool(std::size_t numThreads)
{
	start(numThreads);
}


ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::start(std::size_t numThreads)
{

}

void ThreadPool::stop() noexcept
{

}