#pragma once

namespace ARC {
	using TUUID = TUInt64;

	struct SUUID {
		static TUUID Generate();
	};
}