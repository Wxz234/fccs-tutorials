#pragma once
#include <cstdint>
namespace FCCS {
	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;
}

#ifdef FCCS_LIBRARY
#define FCCS_API __declspec(dllexport)
#else
#define FCCS_API __declspec(dllimport)
#endif // FCCS_LIBRARY

#define FCCS_NOVTABLE __declspec(novtable)

namespace FCCS {
	struct FObject {
		virtual void Release() = 0;
	};

	FCCS_API void DestroyObject(FObject* object);
}
