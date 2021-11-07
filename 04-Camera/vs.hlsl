struct PSInput {
	float4 position : SV_POSITION;
};
PSInput main(
	float3 position : POSITION
) {
	PSInput result;
	result.position = float4(position.x, position.y, position.z, 1);
	return result;
}