//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

struct PSInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
cbuffer _Global : register(b0){
	float4x4 _CameraWorldToViewMatrix;
};

PSInput VSMain(float3 position
: POSITION, float4 color
: COLOR) {
	PSInput result;

	result.position = mul(_CameraWorldToViewMatrix, float4(position, 1));
	result.color = color;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET {
	return input.color;
}
