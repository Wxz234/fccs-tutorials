#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
#include "FCCS_window.h"
namespace FCCS {
	struct Renderer: public FObject {
		virtual void Begin() = 0;
		virtual void End() = 0;
	};

	FCCS_API Renderer* CreateRenderer(Window *pWindow);
}