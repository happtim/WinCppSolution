
#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include <netty/bootstrap/ClientBootstrap.h>
#include <netty/codec/StringCodec.h>

typedef netty::Pipeline<std::vector<asio::mutable_buffer>&, std::string> EchoPipeline;

// the handler for receiving messages back from the server

class EchoHandler : public netty::HandlerAdapter<std::string> {
public:
	void read(Context*, std::string msg) override {
		std::cout << "received back: " << msg;
	}
	void readException(Context* ctx, std::exception e) override {
		std::cout << e.what() << std::endl;
		close(ctx);
	}
	void readEOF(Context* ctx) override {
		std::cout << "EOF received :(" << std::endl;
		close(ctx);
	}
};

// chains the handlers together to define the response pipeline
class EchoPipelineFactory : public netty::PipelineFactory<EchoPipeline> {
public:
	//EchoPipeline::Ptr newPipeline( std::shared_ptr<AsyncTransportWrapper> sock) override {
	EchoPipeline::Ptr newPipeline(std::shared_ptr<asio::ip::tcp::socket> sock) override {

		auto pipeline = EchoPipeline::create();
		//pipeline->addBack(AsyncSocketHandler(sock));
		//pipeline->addBack(EventBaseHandler()); // ensure we can write from any thread
		//pipeline->addBack(LineBasedFrameDecoder(8192, false));
		pipeline->addBack(netty::StringCodec());
		pipeline->addBack(EchoHandler());
		pipeline->finalize();
		return pipeline;
	}
};
/*
发什么回什么的客户端
*/
void test_netty_client() {

	netty::ClientBootstrap<EchoPipeline> client;
	client.group(std::make_shared<asio::io_context>());
	client.pipelineFactory(std::make_shared<EchoPipelineFactory>());
	auto pipeline = client.connect("127.0.0.1", "9601").get();

}

TEST(TestASIO, test_netty_client) {
	test_netty_client();
}