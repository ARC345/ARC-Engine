#pragma  once

namespace ARC {
	struct SGuiHelper {
		static void DrawGuiControl(FVec1& pVec, const char* pID, float pColumnWidth, const FVec1& pDefaults = FVec1::ZeroVector());
		static void DrawGuiControl(FVec2& pVec, const char* pID, float pColumnWidth, const FVec2& pDefaults = FVec2::ZeroVector());
		static void DrawGuiControl(FVec3& pVec, const char* pID, float pColumnWidth, const FVec3& pDefaults = FVec3::ZeroVector());
		static void DrawGuiControl(FVec4& pVec, const char* pID, float pColumnWidth, const FVec4& pDefaults = FVec4::ZeroVector());
		static void DrawGuiControl(FColor4& pVec, const char* pID, float pColumnWidth, const FColor4& pDefaults = FColor4::White());
	};
}