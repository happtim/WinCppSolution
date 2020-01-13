//
// logger_service.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_COMMANDER_SERVICE_HPP
#define SERVICES_COMMANDER_SERVICE_HPP

#include "../stdafx.h"

namespace services {

	/// Service implementation for the logger.
	class commander_service
		: public asio::execution_context::service
	{
	public:
		/// The type used to identify this service in the execution context.
		typedef commander_service key_type;

		/// The backend implementation of a commander.
		struct commander_impl
		{
			explicit commander_impl() {}
		};

		/// The type for an implementation of the logger.
		typedef commander_impl* impl_type;

		/// Constructor creates a thread to run a private io_context.
		commander_service(asio::execution_context& context)
			: asio::execution_context::service(context),
			work_io_context_(),
			work_(asio::make_work_guard(work_io_context_)),
			work_thread_(new std::thread(
				std::bind((asio::io_context::count_type(asio::io_context::*)()) &asio::io_context::run, &work_io_context_)))
		{
		}

		/// Destructor shuts down the private io_context.
		~commander_service()
		{
			/// Indicate that we have finished with the private io_context. Its
			/// io_context::run() function will exit once all other work has completed.
			work_.reset();
			if (work_thread_)
				work_thread_->join();
		}

		/// Destroy all user-defined handler objects owned by the service.
		void shutdown()
		{
		}

		/// Return a null logger implementation.
		impl_type null() const
		{
			return 0;
		}

		/// Create a new logger implementation.
		void create(impl_type& impl)
		{
			impl = new commander_impl();
		}

		/// Destroy a logger implementation.
		void destroy(impl_type& impl)
		{
			delete impl;
			impl = null();
		}

		/// Log a message.
		void execute_command(impl_type& impl, const std::string& message)
		{
			// Format the text to be logged.
			std::ostringstream os;
			os  << " execuate command and do heavy 5s work : " << message;

			// Pass the work of writing to the file to the background thread.
			asio::post(work_io_context_, std::bind(
				&commander_service::command_impl, this, os.str()));
		}

	private:

		/// Helper function used to log a message from within the private io_context's
		/// thread.
		void command_impl(const std::string& text)
		{
			//每次执行command 的时候睡5s的时间
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::cout << "current thread "<< std::this_thread::get_id() << ". " << text << std::endl;
		}

		/// Private io_context used for performing logging operations.
		asio::io_context work_io_context_;

		/// Work for the private io_context to perform. If we do not give the
		/// io_context some work to do then the io_context::run() function will exit
		/// immediately.
		asio::executor_work_guard<
			asio::io_context::executor_type> work_;

		/// Thread used for running the work io_context's run loop.
		std::unique_ptr<std::thread> work_thread_;

	};

} // namespace services

#endif // SERVICES_LOGGER_SERVICE_HPP