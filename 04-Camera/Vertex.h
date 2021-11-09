#pragma once
#include <DirectXMath.h>
struct Vertex
{
	float position[3];
};

struct CameraMatrix {
	DirectX::XMMATRIX v;
	DirectX::XMMATRIX p;
};
