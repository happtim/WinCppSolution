#pragma once

namespace folly {

	struct Unit {
		constexpr bool operator==(const Unit& /*other*/) const {
			return true;
		}
		constexpr bool operator!=(const Unit& /*other*/) const {
			return false;
		}
	};

}

