#pragma  once

namespace arc {
	struct gui_helper {
		static void draw_gui_control(vec1& pVec, const char* pID, float pColumnWidth, const vec1& pDefaults = vec1::ZeroVector());
		static void draw_gui_control(vec2& pVec, const char* pID, float pColumnWidth, const vec2& pDefaults = vec2::ZeroVector());
		static void draw_gui_control(vec3& pVec, const char* pID, float pColumnWidth, const vec3& pDefaults = vec3::ZeroVector());
		static void draw_gui_control(vec4& pVec, const char* pID, float pColumnWidth, const vec4& pDefaults = vec4::ZeroVector());
		static void draw_gui_control(color4& pVec, const char* pID, float pColumnWidth, const color4& pDefaults = color4::White());
	};
}