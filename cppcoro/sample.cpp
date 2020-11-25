// from structured concurrency article

cppcoro::task<> Manager::allocate()
{
	// retry the allocation up to kMaxRetries times
	for (int retiesCount = 1; retriesCount <= kMaxRetries; ++retriesCount)
	{
		try {
			co_await allocator_.doAllocate();
			co_return;  // success!
		}
		catch (...) {}
		
		// Oops, it failed. Yield the thread for a bit and then retry
		co_await scheduler_.schedule_after(10ms * (1 << retriesCount));
	}
	
	// error, too many retries
	throw std::runtime_error{"Resource allocation retry count exceeded."};
}
