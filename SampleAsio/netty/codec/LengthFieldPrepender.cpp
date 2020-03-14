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

//#include <wangle/codec/LengthFieldPrepender.h>

#include "LengthFieldPrepender.h"
#include <asio/detail/socket_ops.hpp>
#include <asio/buffer.hpp>
#include <glog/logging.h>


//using folly::Future;
//using folly::Unit;
//using folly::IOBuf;

namespace netty {

LengthFieldPrepender::LengthFieldPrepender(
    int lengthFieldLength,
    int lengthAdjustment,
    bool lengthIncludesLengthField,
    bool networkByteOrder)
    : lengthFieldLength_(lengthFieldLength)
    , lengthAdjustment_(lengthAdjustment)
    , lengthIncludesLengthField_(lengthIncludesLengthField)
    , networkByteOrder_(networkByteOrder) {
	
    CHECK(lengthFieldLength == 1 ||
          lengthFieldLength == 2 ||
          lengthFieldLength == 4 ||
          lengthFieldLength == 8 );
  }

//Future<Unit> LengthFieldPrepender::write( Context* ctx, std::unique_ptr<IOBuf> buf) {
std::future<folly::Unit> LengthFieldPrepender::write(Context* ctx, std::unique_ptr<asio::mutable_buffer> buf)
{
	size_t length = lengthAdjustment_ + buf->size();
	if (lengthIncludesLengthField_) {
		length += lengthFieldLength_;
	}

	if (length < 0) {
		throw std::runtime_error("Length field < 0");
	}

  //auto len = IOBuf::create(lengthFieldLength_);
  //len->append(lengthFieldLength_);
  //folly::io::RWPrivateCursor c(len.get());

	std::unique_ptr<asio::mutable_buffer> len;

	switch (lengthFieldLength_) {
		case 1: {
			if (length >= 256) {
				throw std::runtime_error("length does not fit byte");
			}
			unsigned char c1[1];

			if (networkByteOrder_) {
				c1[0] = asio::detail::socket_ops::host_to_network_short((uint8_t)length);
				//len = asio::buffer(c1, 1) ;

			} else {
				c1[0] = (uint8_t)length;
				//len = asio::buffer(c1, 1);
			}
			break;
		}
		case 2: {
			if (length >= 65536) {
				throw std::runtime_error("length does not fit byte");
			}
			unsigned char c2[2];

			  if (networkByteOrder_) {
				  c2[0] = asio::detail::socket_ops::host_to_network_short((uint16_t)length);
				  //len = asio::buffer(c2, 2);
			  } else {
				  c2[0] = (uint16_t)length;
				  //len = asio::buffer(c2, 2);
			  }
		break;
    }
    case 4: {
		unsigned char c4[4];
      if (networkByteOrder_) {
		  c4[0] = asio::detail::socket_ops::host_to_network_long((uint32_t)length);
		  //len = asio::buffer(c4, 4);

      } else {
		  c4[0] = (uint32_t)length;
		  //len = asio::buffer(c4, 4);
      }
      break;
    }
    case 8: {
		unsigned char c8[8];
      if (networkByteOrder_) {
		  c8[0] = asio::detail::socket_ops::host_to_network_long((uint64_t)length);
		  //len = asio::buffer(c8, 8);
      } else {
		  c8[0] = (uint64_t)length;
		  //len = asio::buffer(c8, 8);
      }
      break;
    }
    default: {
      throw std::runtime_error("Invalid lengthFieldLength");
    }
  }
	//len->prependChain(std::move(buf));
	//return ctx->fireWrite(std::move(len));

	return ctx->fireWrite( std::move(len) );
}


} // namespace wangle
