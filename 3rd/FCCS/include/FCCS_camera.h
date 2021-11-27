#pragma once
#include "FCCS_core.h"
#include <DirectXMath.h>
namespace FCCS {
	class Camera {
	public:
		Camera() {
			SetViewMatrix({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
			SetPerspectiveMatrix(DirectX::XM_PIDIV2, 1, 1, 100);
		}
		void SetViewMatrix(
			DirectX::FXMVECTOR EyePosition,
			DirectX::FXMVECTOR FocusPosition,
			DirectX::FXMVECTOR UpDirection
		) {
			_eye = EyePosition;
			_at = FocusPosition;
			_up = UpDirection;
			_view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);
		}
		void SetPerspectiveMatrix(float FovAngleY, float AspectRatio, float NearZ, float FarZ) {
			_projection = DirectX::XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
		}

		DirectX::XMMATRIX GetViewMatrix() const {
			return _view;
		}

		DirectX::XMMATRIX GetPerspectiveMatrix() const {
			return _projection;
		}

		DirectX::XMMATRIX GetVP() const { return _view * _projection; }
	private:
		DirectX::XMVECTOR _eye;
		DirectX::XMVECTOR _at;
		DirectX::XMVECTOR _up;
		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _projection;
	};
}