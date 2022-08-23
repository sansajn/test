#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "concurrent_queue.hpp"

TEST_CASE("we can push and pop elements to the queue", 
	"[concurrent_queue]") {
	concurrent_queue<int> q;
	REQUIRE(q.empty());
	
	q.push(2);
	q.push(5);
	REQUIRE(!q.empty());

	int task;
	REQUIRE(q.try_pop(task));
	REQUIRE(q.try_pop(task));
	REQUIRE(q.empty());
}

TEST_CASE("should trigger cppcheck complain about bad index access", 
	"[cppcehck]") {
	
	int arr[10];
	int a10 = arr[10];
}
