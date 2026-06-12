#pragma once

namespace arc {
	class graphics_context
	{
	public:
		virtual ~graphics_context() = default;
		virtual void init()=0;
		virtual void swap_buffers()=0;
	};
}