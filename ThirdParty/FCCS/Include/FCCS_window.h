#pragma once
#include "FCCS_core.h"
#include "FCCS_resource.h"
namespace FCCS {
	struct FCCS_NOVTABLE IWindow : public IResource {
	public:
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual void ShowWindow() = 0;
		virtual void HideWindow() = 0;
		virtual bool IsActive() const = 0;
	};

	typedef RefCountPtr<IWindow> WindowHandle;
	FCCS_API IWindow* CreateWindowExW(const wchar_t* title, uint32 width, uint32 height);
}