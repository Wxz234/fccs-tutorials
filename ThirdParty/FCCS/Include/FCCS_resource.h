#pragma once
#include "FCCS_core.h"
#include <wrl/client.h>
namespace FCCS {
	template<typename T>
	using RefCountPtr = Microsoft::WRL::ComPtr<T>;

	struct FCCS_NOVTABLE IResource
	{
	public:
		virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) = 0;
		virtual ULONG AddRef() = 0;
		virtual ULONG Release() = 0;
		virtual void* GetNativeObject() const noexcept = 0;
	};
}