#pragma once
#include "FCCS_core.h"
namespace FCCS {
	struct Window : public FObject
	{
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual HWND GetHWND() const = 0;
		virtual bool IsRun() = 0;
	};
	FCCS_API Window* CreateWindowExW(const wchar_t* pTitle, uint32 width, uint32 height);
}