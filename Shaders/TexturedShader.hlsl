struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer CBPerDraw : register(b0)
{
	matrix WorldToClip;
};

cbuffer CBPerObject : register(b1)
{
	matrix ObjectToWorld;
	float4 Color;
};

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	output.pos = mul(float4(input.pos, 1.0f), objectToClip);
	output.uv = input.uv;

	return output;
}

Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = shaderTexture.Sample(samplerState, input.uv);

	return col;
}