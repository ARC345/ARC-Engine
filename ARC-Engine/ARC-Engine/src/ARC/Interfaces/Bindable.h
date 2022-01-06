#pragma once

namespace ARC {
	class IBindable {
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	};
}