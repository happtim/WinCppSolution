// SampleWangle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <folly/portability/GFlags.h>

#include <folly/init/Init.h>
#include <wangle/bootstrap/ClientBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/channel/EventBaseHandler.h>
#include <wangle/codec/LineBasedFrameDecoder.h>
#include <wangle/codec/StringCodec.h>

using namespace folly;
using namespace wangle;

DEFINE_int32(port, 8080, "echo server port");
DEFINE_string(host, "::1", "echo server address");

typedef Pipeline<folly::IOBufQueue&, std::string> EchoPipeline;

// the handler for receiving messages back from the server
class EchoHandler : public HandlerAdapter<std::string> {
public:
	void read(Context*, std::string msg) override {
		std::cout << "received back: " << msg;
	}
	void readException(Context* ctx, exception_wrapper e) override {
		std::cout << exceptionStr(e) << std::endl;
		close(ctx);
	}
	void readEOF(Context* ctx) override {
		std::cout << "EOF received :(" << std::endl;
		close(ctx);
	}
};

// chains the handlers together to define the response pipeline
class EchoPipelineFactory : public PipelineFactory<EchoPipeline> {
public:
	EchoPipeline::Ptr newPipeline(
		std::shared_ptr<AsyncTransportWrapper> sock) override {
		auto pipeline = EchoPipeline::create();
		pipeline->addBack(AsyncSocketHandler(sock));
		pipeline->addBack(
			EventBaseHandler()); // ensure we can write from any thread
		pipeline->addBack(LineBasedFrameDecoder(8192, false));
		pipeline->addBack(StringCodec());
		pipeline->addBack(EchoHandler());
		pipeline->finalize();
		return pipeline;
	}
};

int main(int argc, char** argv) {
	folly::Init init(&argc, &argv);

	ClientBootstrap<EchoPipeline> client;
	client.group(std::make_shared<folly::IOThreadPoolExecutor>(1));
	client.pipelineFactory(std::make_shared<EchoPipelineFactory>());
	auto pipeline = client.connect(SocketAddress("127.0.0.1", 8080)).get();

	try {
		while (true) {
			std::string line;
			std::getline(std::cin, line);
			if (line == "") {
				break;
			}

			pipeline->write(line + "\r\n").get();
			if (line == "bye") {
				pipeline->close();
				break;
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << exceptionStr(e) << std::endl;
	}

	return 0;
}