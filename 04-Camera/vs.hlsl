
cbuffer cameraMat : register(b0)
{
	float4x4 v;
	float4x4 p;
};

struct PSInput {
	float4 position : SV_POSITION;
};
PSInput main(
	float3 position : POSITION
) {

	float4 pos = float4(position.x, position.y, position.z, 1);
	pos = mul(v, pos);
	pos = mul(pos, p);
	PSInput result;
	result.position = pos;
	return result;
}