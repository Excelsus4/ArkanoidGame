#include "stdafx.h"
#include "PowerUps.h"

PowerUps::PowerUps(World* world, Type type) :
	IArkanoidPhysics(world), velocity(0, -60), type(type)
{
	animation = new Animation;
	wstring spriteFile = Textures + L"Arkanoid/PowerUps.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	int row;
	{
		Clip* clip = new Clip(PlayMode::Loop);
		switch (type)
		{
		case PowerUps::Paddle:
			row = 6;
			break;
		case PowerUps::Lasers:
			row = 2;
			break;
		case PowerUps::Enlarge:
			row = 3;
			break;
		case PowerUps::Catch:
			row = 1;
			break;
		case PowerUps::Slow:
			row = 0;
			break;
		case PowerUps::Break:
			row = 5;
			break;
		case PowerUps::Disruption:
			row = 4;
			break;
		}
		for (int i = 0; i < 8; i++) {
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 16 * i, row * 8, 16 * (i + 1), row * 8 + 7), 0.1f);
		}
		animation->AddClip(clip);
	}
	Scale(1, 1);
	animation->Play(0);
	animation->DrawBound(true);
}

PowerUps::~PowerUps()
{
	SAFE_DELETE(animation);
}

void PowerUps::PhysicsUpdate()
{
	//=========================================================================
	// Translation
	//=========================================================================
	D3DXVECTOR2 pos = Position();
	pos += velocity * Timer->Elapsed();
	Position(pos);

	if (pos.y + HalfSize().y < 0)
		recycleFlag = true;
	else if (CheckCollision((IArkanoidPhysics*)world->vaus)) {
		((IArkanoidPhysics*)world->vaus)->Feed(type);
		recycleFlag = true;
	}
}

