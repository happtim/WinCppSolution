//
// logger.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_COMMANDER_HPP
#define SERVICES_COMMANDER_HPP

#include "basic_commander.hpp"
#include "commander_service.hpp"

namespace services {

	/// Typedef for typical logger usage.
	typedef basic_commander<commander_service> commander;

} // namespace services

#endif // SERVICES_LOGGER_HPP
