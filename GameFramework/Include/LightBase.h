#pragma once

//#include "GameComponent.h"
#include "RenderingSystem.h"

class Renderer;

enum class LightType
{
	Ambient = 0,
	Directional, 
	Point,
	Spot
};

//@TODO: update to use different data for different lights?


// @TODO: uncomment and remove cyclic include
class LightBase : public GameComponent
{
public:

	// @TODO: should this be virtual? Should I return a reference?
	virtual LightData GetLightData() { return LightData(); };

	virtual LightType GetLightType() = 0;

	virtual Renderer* GetLightRenderer() { return renderer; }

	Renderer* renderer;

	void SetRenderer(Renderer* InRenderer) { renderer = InRenderer; }

};

//@TODO: move this to a separate file
class AmbientLight : public LightBase
{
public:

	virtual LightType GetLightType() override { return LightType::Ambient; }

	virtual LightData GetLightData() 
	{ 
		LightData light;
		light.Intensity = 0.15f;
		return light; 
	};


};

//@TODO: move this to a separate file
class DirectionalLight : public LightBase
{
public:

	virtual LightType GetLightType() override { return LightType::Directional; }

	virtual LightData GetLightData() { return LightData(); };


};


class PointLight : public LightBase
{
public:

	virtual LightType GetLightType() override { return LightType::Point; }

	virtual LightData GetLightData()
	{ 
		LightData light;
		light.Position = Vector4(mTransform.Position.x, mTransform.Position.y, mTransform.Position.z, 1.0f);
		light.Params = Vector3(0.3f, 0.3f, 0.3f);
		light.Intensity = 1.0f;
		light.Color = color;
		return light;
	};

	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
};