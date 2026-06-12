#pragma once

namespace arc {
	using uuid = u64;

	struct uuid {
		static uuid generate();
	};
}