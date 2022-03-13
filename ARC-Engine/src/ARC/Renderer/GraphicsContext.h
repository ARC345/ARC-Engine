#pragma once

namespace ARC {
	class CGraphicsContext
	{
	public:
		virtual ~CGraphicsContext() = default;
		virtual void Init()=0;
		virtual void SwapBuffers()=0;
	};
}