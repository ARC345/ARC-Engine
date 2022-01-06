#pragma once

namespace ARC {
	class CGraphicsContext
	{
	public:
		virtual void Init()=0;
		virtual void SwapBuffers()=0;
	};
}