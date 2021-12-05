#pragma once
#include "FCCS_core.h"
namespace FCCS {

	struct WindowContext {
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Release() = 0;
	};

	struct Window
	{
		virtual void Release() = 0;
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual HWND GetHWND() const = 0;
		virtual int32 Run(WindowContext*) = 0;
	};

	FCCS_API Window* CreateWindowExW(const wchar_t* title, uint32 width, uint32 height);
}