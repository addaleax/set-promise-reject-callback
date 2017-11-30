#include <v8.h>
#include <node.h>

using namespace v8;

static Persistent<Function> fn;

static void OnPromiseReject(PromiseRejectMessage message) {
  Local<Promise> promise = message.GetPromise();
  Isolate* isolate = promise->GetIsolate();

  Local<Value> value = message.GetValue();
  Local<Integer> event = Integer::New(isolate, message.GetEvent());

  if (value.IsEmpty())
    value = Undefined(isolate);

  Local<Value> args[] = { event, promise, value };

  fn.Get(isolate)->Call(Undefined(isolate), 3, args);
}

void SetCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (!args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,
                            "Callback must be function",
                            NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  isolate->SetPromiseRejectCallback(OnPromiseReject);
  fn.Reset(isolate, args[0].As<Function>());
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "SetCallback", SetCallback);
  NODE_DEFINE_CONSTANT(exports, kPromiseRejectWithNoHandler);
  NODE_DEFINE_CONSTANT(exports, kPromiseHandlerAddedAfterReject);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
