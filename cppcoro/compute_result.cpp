// sample from [structured concurrency](https://ericniebler.com/2020/11/08/structured-concurrency/?utm_source=rss&utm_medium=rss&utm_campaign=structured-concurrency) article

cppcoro::task<> computeResult(State & s);

cppcoro::task<int> doThing() 
{
	State s;
	co_await computeResult(s);
	co_return s.result;  // computeResult() call ends before return
}
