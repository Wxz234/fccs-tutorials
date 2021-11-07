#pragma once
#include "FCCS_core.h"
#include <DirectXMath.h>
namespace FCCS {
	class Camera {
	public:
		Camera() {
			LookAt({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
			Perspective(XM_PIDIV2, 1, 1, 100);
		}
		void LookAt(
			const DirectX::XMVECTOR &EyePosition,
			const DirectX::XMVECTOR &FocusPosition,
			const DirectX::XMVECTOR &UpDirection) {
			_eye = EyePosition;
			_at = FocusPosition;
			_up = UpDirection;
			_view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);
		}
		void Perspective(float FovAngleY,float AspectRatio,float NearZ,float FarZ) {
			_projection = DirectX::XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
		}
	private:
		DirectX::XMVECTOR _eye;
		DirectX::XMVECTOR _at;
		DirectX::XMVECTOR _up;
		DirectX::XMMATRIX _view
		DirectX::XMMATRIX _projection;
	};
}