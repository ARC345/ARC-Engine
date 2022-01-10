#pragma once

namespace ARC {
	template<typename T>
	class IArea2D {
	public:
		IArea2D() {}
		IArea2D(T _X, T _Y) : 
			m_Height(_Y),
			m_Width(_X) {}

		virtual T GetHeight() const { return m_Height; };
		virtual T GetWidth() const { return m_Width; };
		
	private:
		T m_Height;
		T m_Width;
	};
}