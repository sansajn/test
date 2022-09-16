// unifex execute() sample
#include <iostream>
#include <unifex/single_thread_context.hpp>
#include <unifex/execute.hpp>
#include <unifex/scheduler_concepts.hpp>

using std::cout;
using unifex::single_thread_context, unifex::execute;

int main(int argc, char * argv[]) {
	single_thread_context ctx;

	for (int i = 0; i < 5; ++i) {
		execute(ctx.get_scheduler(), [i](){
			cout << "hello execute() " << i << '\n';
		});
	}

	return 0;
}
