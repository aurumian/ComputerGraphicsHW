struct VS_IN
{
	float4 pos : POSITION0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
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
	output.pos = mul(input.pos, objectToClip);

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = Color;

	return col;
}