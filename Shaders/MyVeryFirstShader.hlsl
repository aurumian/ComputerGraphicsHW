struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
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

	//matrix objectToClip = mul(WorldToClip, ObjectToWorld);
	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	//output.pos = mul(mul(input.pos, ObjectToWorld), WorldToClip);
	//output.pos = mul(mul(input.pos, WorldToClip),ObjectToWorld);
	output.pos = mul(input.pos, objectToClip);

	//output.pos = input.pos;
	output.col = input.col;

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = input.col;

	return col;
}

float4 PSPlainColor(PS_IN input) : SV_Target
{
	float4 col = Color;

	return col;
}