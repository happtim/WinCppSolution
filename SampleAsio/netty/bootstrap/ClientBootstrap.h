/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

//#include <folly/io/async/AsyncSSLSocket.h>
//#include <folly/io/async/AsyncSocket.h>
//#include <folly/io/async/DestructorCheck.h>
//#include <folly/io/async/EventBaseManager.h>
#include <netty/bootstrap/BaseClientBootstrap.h>
#include <netty/channel/Pipeline.h>
//#include <folly/executors/IOThreadPoolExecutor.h>
#include <asio.hpp>

//using folly::AsyncSSLSocket;

namespace netty {

/*
 * A thin wrapper around Pipeline and AsyncSocket to match
 * ServerBootstrap.  On connect() a new pipeline is created.
 */
template <typename Pipeline>
class ClientBootstrap : public BaseClientBootstrap<Pipeline>//,
                        //public folly::DestructorCheck 
{

  //class ConnectCallback : public folly::AsyncSocket::ConnectCallback {
  // public:
  //  ConnectCallback(
  //      folly::Promise<Pipeline*> promise,
  //      ClientBootstrap* bootstrap,
  //      std::shared_ptr<folly::AsyncSocket> socket,
  //      SSLSessionEstablishedCallbackUniquePtr sslSessionEstablishedCallback)
  //      : promise_(std::move(promise)),
  //        bootstrap_(bootstrap),
  //        socket_(socket),
  //        safety_(*bootstrap),
  //        sslSessionEstablishedCallback_(
  //            std::move(sslSessionEstablishedCallback)) {}

    //void connectSuccess() noexcept override {
    //  if (!safety_.destroyed()) {

    //    //if (sslSessionEstablishedCallback_) {
    //    //  AsyncSSLSocket* sslSocket =
    //    //    dynamic_cast<AsyncSSLSocket*>(socket_.get());
    //    //  if (sslSocket && !sslSocket->getSSLSessionReused()) {
    //    //    sslSessionEstablishedCallback_->onEstablished(
    //    //      sslSocket->getSSLSession());
    //    //  }
    //    //}
    //    bootstrap_->makePipeline(std::move(socket_));
    //    if (bootstrap_->getPipeline()) {
    //      bootstrap_->getPipeline()->transportActive();
    //    }
    //    promise_.setValue(bootstrap_->getPipeline());
    //  }
    //  delete this;
    //}

    //void connectErr(const std::exception & ex) noexcept override {
    //  promise_.setException(
    //    folly::make_exception_wrapper<folly::AsyncSocketException>(ex));
    //  delete this;
    //}
   //private:
    //std::promise<Pipeline*> promise_;
    //ClientBootstrap* bootstrap_;
    //std::shared_ptr<folly::AsyncSocket> socket_;
    //folly::DestructorCheck::Safety safety_;
    //SSLSessionEstablishedCallbackUniquePtr sslSessionEstablishedCallback_;
  //};

 public:
	ClientBootstrap() {}

	ClientBootstrap* group(
		//std::shared_ptr<folly::IOThreadPoolExecutor> group) {
		std::shared_ptr<asio::io_context> group) {
		//asio::io_context group) {
			group_ = group;
			return this;
	}

  ClientBootstrap* bind(int port) {
    port_ = port;
    return this;
  }

	std::future<Pipeline*> connect(
	  //const folly::SocketAddress& address,
		const std::string host,
		const std::string port,
		std::chrono::milliseconds timeout = std::chrono::milliseconds(0)) override {

    //auto base = (group_) ? group_->getEventBase()  : folly::EventBaseManager::get()->getEventBase();
    //std::future<Pipeline*> retval((Pipeline*)nullptr);

    //base->runImmediatelyOrRunInEventBaseThreadAndWait([&](){
      //std::shared_ptr<folly::AsyncSocket> socket;
      //if (this->sslContext_) {
      //  auto sslSocket = folly::AsyncSSLSocket::newSocket(
      //      this->sslContext_,
      //      base,
      //      this->deferSecurityNegotiation_);
      //  if (!this->sni_.empty()) {
      //    sslSocket->setServerName(this->sni_);
      //  }
      //  if (this->sslSession_) {
      //    sslSocket->setSSLSession(this->sslSession_, true);
      //  }
      //  socket = sslSocket;
      //} else {
      //  socket = folly::AsyncSocket::newSocket(base);
      /*}*/

	asio::ip::tcp::resolver resolver(*group_);
	auto endpoints = resolver.resolve(host, port);

	//����std::move��pipeline��
	std::shared_ptr<asio::ip::tcp::socket> socket_ = std::make_shared<asio::ip::tcp::socket>(*group_);

	std::promise<Pipeline*> promise;
	std::future<Pipeline*> retval = promise.get_future();

	//void do_connect(const tcp::resolver::results_type& endpoints)
	//{
	asio::async_connect(*socket_, endpoints,
			[&](std::error_code ec, asio::ip::tcp::endpoint)
		{
			if (!ec)
			{
				this->makePipeline(std::move(socket_));
				if (this->getPipeline()) {
					this->getPipeline()->transportActive();
				}
				promise.set_value(this->getPipeline());
				//promise_.setValue(this->getPipeline());

				//do_read_header();
			}
			else {
				//promise_.setException(
				//folly::make_exception_wrapper<folly::AsyncSocketException>(ex));

				//promise.set_exception(
				//	std::exception()
				//	folly::make_exception_wrapper<folly::AsyncSocketException>(ex));
			}
		});
	//}

      DCHECK_LE(timeout.count(), std::numeric_limits<int>::max());
      
	  //socket->connect(
   //       new ConnectCallback(
   //           std::move(promise),
   //           this,
   //           socket,
   //           std::move(this->sslSessionEstablishedCallback_)),
   //       address,
   //       folly::to_narrow(timeout.count()),
   //       BaseClientBootstrap<Pipeline>::getSocketOptions(address.getFamily()));
    //});
    return retval;
  }

  ~ClientBootstrap() override = default;

 protected:
  int port_;
  //std::shared_ptr<folly::IOThreadPoolExecutor> group_;
	std::shared_ptr<asio::io_context> group_;
	//asio::io_context group_;
};

class ClientBootstrapFactory : public BaseClientBootstrapFactory<BaseClientBootstrap<>> {
 public:
  ClientBootstrapFactory() {}

	BaseClientBootstrap<>::Ptr newClient() override {
		return std::make_unique<ClientBootstrap<DefaultPipeline>>();
	}
};

} // namespace wangle
