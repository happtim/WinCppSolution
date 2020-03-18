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

#include <netty/channel/Handler.h>

namespace netty {

/*
 * StringCodec converts a pipeline from IOBufs to std::strings.
 */
class StringCodec : public Handler<std::unique_ptr<asio::mutable_buffer>, std::string,
                                   std::string, std::unique_ptr<asio::mutable_buffer>> {
 public:
  //typedef typename Handler<
  // std::unique_ptr<folly::IOBuf>, std::string,
  // std::string, std::unique_ptr<folly::IOBuf>>::Context Context;

	 typedef typename Handler<
		 std::unique_ptr<asio::mutable_buffer>, std::string,
		 std::string, std::unique_ptr<asio::mutable_buffer>>::Context Context;

  void read(Context* ctx, std::unique_ptr<asio::mutable_buffer> buf) override {
    if (buf) {
      //buf->coalesce();
      std::string data((const char*)buf->data(), buf->size());
      ctx->fireRead(data);
    }
  }

  std::future<folly::Unit> write(Context* ctx, std::string msg) override {
    //auto buf = folly::IOBuf::copyBuffer(msg.data(), msg.length());
	//TODO ”–Œ Ã‚
	auto buf = std::unique_ptr<asio::mutable_buffer> ( &asio::buffer(msg) );
    return ctx->fireWrite(std::move(buf));
  }
};

} // namespace wangle
