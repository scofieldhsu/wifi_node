// hello.cc
#include <node.h>
#include <v8.h>//scofield

#include "wlan.h"//scofield
#include <thread>//scofield
#include <windows.h>//scofield
#include <string>//scofield
#include <iostream>//scofield


namespace demo {

/*using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;*/
using namespace v8;//scofield
using namespace std;//scofield


/*
scofield note
Local<String> NewFromUtf8(Isolate* isolate, const char* data, 
	NewStringType type = kNormalString, int length = -1));
*/

void Method(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void method__wlan_sync_enumInterfaces(const FunctionCallbackInfo<Value>& args) {
	char output[256] = {0};
	wstring woutput = L"";

	enumInterfaces(woutput);
	wcstombs(output, woutput.c_str(), 256);

	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, output));
}

void method__wlan_sync_getAvailableNetworkList(const FunctionCallbackInfo<Value>& args) {
	char output[4096] = {0};
	wstring woutput = L"";

	getAvailableNetworkList(woutput);
	wcstombs(output, woutput.c_str(), 4096);	

	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, output));
}

void method__wlan_sync_getNotifications(const FunctionCallbackInfo<Value>& args) {
	char output[8192] = {0};
	wstring woutput = L"";

	woutput = getNotifications();
	wcstombs(output, woutput.c_str(), 8192);	

	//printf("method__wlan_sync_getNotifications(): %s\n", output);

	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, output));
}



void thread_task() {
printf("a thread is running ... \n");
Sleep(9000);

char output[256] = {0};
//getWlanInterfaces(output, 0);

printf("a thread ends \n");
}

void method__wlan_async_enumInterfaces(const FunctionCallbackInfo<Value>& args) {
	char output[512] = {0};
	wstring woutput = L"";

/*printf("create a thread \n");
	thread a_thread( thread_task );
	a_thread.join();*/

	enumInterfaces(woutput);
	wcstombs(output, woutput.c_str(), 512);	

	Isolate* isolate = args.GetIsolate();
	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Local<Value> argv[argc] = { String::NewFromUtf8(isolate, output) };
	cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void method__wlan_async_getAvailableNetworkList(const FunctionCallbackInfo<Value>& args) {
	char output[4096] = {0};
	wstring woutput = L"";

	getAvailableNetworkList(woutput);
	wcstombs(output, woutput.c_str(), 4096);	

	Isolate* isolate = args.GetIsolate();
	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Local<Value> argv[argc] = { String::NewFromUtf8(isolate, output) };
	cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}



void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "hello", Method);

	init_wlan();
	NODE_SET_METHOD(exports, "wlanapi_sync_enumInterfaces", method__wlan_sync_enumInterfaces);//scofield
	NODE_SET_METHOD(exports, "wlanapi_async_enumInterfaces", method__wlan_async_enumInterfaces);//scofield
	NODE_SET_METHOD(exports, "wlanapi_sync_getAvailableNetworkList", method__wlan_sync_getAvailableNetworkList);//scofield
	NODE_SET_METHOD(exports, "wlanapi_async_getAvailableNetworkList", method__wlan_async_getAvailableNetworkList);//scofield
	NODE_SET_METHOD(exports, "wlanapi_sync_getNotifications", method__wlan_sync_getNotifications);//scofield
}

/*
void Method(const v8::FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "scofield"));
	  }

	  void init(Handle<Object> exports) {
		Isolate* isolate = Isolate::GetCurrent();
	  exports->Set(String::NewFromUtf8(isolate, "hello"),
			FunctionTemplate::New(isolate, Method)->GetFunction());
		}
*/

NODE_MODULE(addon, init)

}  // namespace demo

