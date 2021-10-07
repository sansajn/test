// Napi::AsyncWorker sample
#include <chrono>
#include <thread>
#include <string>
#include <napi.h>

using std::string;
using namespace std::chrono_literals;
using namespace Napi;

class EchoWorker : public AsyncWorker {
public:
	EchoWorker(Function & callback, string const & echo)
		: AsyncWorker{callback}
		, _echo{echo} 
	{}

	~EchoWorker() = default;

	// This code will be executed on the worker thread
	void Execute() override {
		std::this_thread::sleep_for(1s);  // Need to simulate cpu heavy task
	}

	void OnOK() override {
		HandleScope scope(Env());
		Callback().Call({Env().Null(), String::New(Env(), _echo)});
	}

private:
	string _echo;
};

Value Echo(CallbackInfo const & info) {
	string in = info[0].As<String>();
	Function cb = info[1].As<Function>();
	EchoWorker * w = new EchoWorker{cb, in};
	w->Queue();
	return info.Env().Undefined();
}

Object Init(Env env, Object exports) {
	exports.Set(String::New(env, "Echo"),
		Function::New(env, Echo));
	return exports;
}

NODE_API_MODULE(addon, Init)
