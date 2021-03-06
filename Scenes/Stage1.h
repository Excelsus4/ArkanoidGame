#pragma once
#include "Scene.h"
#include "Arkanoid/IArkanoidPhysics.h"
#include "Arkanoid/Vaus.h"
#include "Arkanoid/Ball.h"
#include "Arkanoid/Blocks.h"

class Stage1 : public Scene {
public:
	Stage1(SceneValues* values);
	virtual ~Stage1();

	virtual void Update() override;
	virtual void Render() override;

private:
	World world;
	vector<PowerUps*> pool;
	Sprite background;
};