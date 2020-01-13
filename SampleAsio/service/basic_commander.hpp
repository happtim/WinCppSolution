//
// basic_logger.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_BASIC_COMMANDER_HPP
#define SERVICES_BASIC_COMMANDER_HPP

#include "../stdafx.h"

namespace services {

	/// Class to provide simple logging functionality. Use the services::logger
	/// typedef.
	template <typename Service>
	class basic_commander
		: private asio::detail::noncopyable
	{
	public:
		/// The type of the service that will be used to provide timer operations.
		typedef Service service_type;

		/// The native implementation type of the timer.
		typedef typename service_type::impl_type impl_type;

		/// Constructor.
		/**
		 * This constructor creates a logger.
		 *
		 * @param context The execution context used to locate the logger service.
		 *
		 * @param identifier An identifier for this logger.
		 */
		explicit basic_commander(asio::execution_context& context)
			: service_(asio::use_service<Service>(context)),
			impl_(service_.null())
		{
			service_.create(impl_);
		}

		/// Destructor.
		~basic_commander()
		{
			service_.destroy(impl_);
		}

		/// Get the io_context associated with the object.
		asio::io_context& get_io_context()
		{
			return service_.get_io_context();
		}

		/// Log a message.
		void execute_command(const std::string& message)
		{
			service_.execute_command(impl_, message);
		}

	private:
		/// The backend service implementation.
		service_type& service_;

		/// The underlying native implementation.
		impl_type impl_;
	};

} // namespace services

#endif // SERVICES_BASIC_LOGGER_HPP