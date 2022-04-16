struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float3 worldPos : POSITION0;
};

//////////////////////////////////////////////////////
// Constant buffers
//////////////////////////////////////////////////////

struct DirLight
{
	float3 direction;
	float intensity;
	float4 color;
	matrix WorldToLightClip;
};

cbuffer CBPerDraw : register(b0)
{
	matrix WorldToClip;
	DirLight dirLight;
	float3 cameraPos;
};

struct LitMaterial
{
	float ambientCoef;
	float specularCoef;
	float specularExponent;
	float diffuseCoef;
};

cbuffer CBPerObject : register(b1)
{
	matrix ObjectToWorld;
	matrix NormalO2W;
	float4 Color;
	LitMaterial Mat;
};

//////////////////////////////////////////////////////
// Shader code
//////////////////////////////////////////////////////

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	output.pos = mul(float4(input.pos, 1.0f), objectToClip);
	output.uv = input.uv;
	output.normal = mul(input.normal, NormalO2W);
	output.worldPos = mul(float4(input.pos, 1.0f), ObjectToWorld).xyz;

	return output;
}

Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s1);

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = shaderTexture.Sample(samplerState, input.uv);

	float3 view = normalize(cameraPos - input.worldPos);
	float3 normal = normalize(input.normal);

	float lightAmbient = dirLight.intensity * Mat.ambientCoef;
	float lightDiffuse = dirLight.intensity * Mat.diffuseCoef * saturate(-dot(dirLight.direction, normal));
	float lightSpecular = dirLight.intensity * Mat.specularCoef * pow(saturate(dot(reflect(dirLight.direction, normal), view)), Mat.specularExponent);

	float4 lightSpacePos = mul(float4(input.worldPos, 1.0f), dirLight.WorldToLightClip);
	lightSpacePos = lightSpacePos / lightSpacePos.w;
	float2 smUV = lightSpacePos.xy;
	smUV = smUV * 0.5f + 0.5f;
	smUV.y = 1.0f - smUV.y;

	float shadow = shadowMap.SampleCmp(shadowSampler, smUV, lightSpacePos.z - 0.003f);

	col.xyz = col.xyz * (lightAmbient + (lightDiffuse + lightSpecular) * shadow) * dirLight.color;
	//col.xyz = shadow.xxx;
	return col;
}