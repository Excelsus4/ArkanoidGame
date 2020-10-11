#include "stdafx.h"
#include "Vaus.h"

Vaus::Vaus(World * world, IArkanoidPhysics* startingBall):
	IArkanoidPhysics(world), speed(200)
{
	animation = new Animation;
	wstring spriteFile = Textures + L"Arkanoid/Vaus.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 0; i < 6; ++i) {
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 32, i * 8, 64, (i + 1) * 8), 0.2f);
		}
		animation->AddClip(clip);
	}
	
	Position(112, 20);
	Scale(1, 1);
	animation->Play(0);
	animation->DrawBound(true);

	startingBall->SetVelocity(D3DXVECTOR2(0, 0));
	boundBall* bb = new boundBall;
	bb->ball = startingBall;
	bb->offset = startingBall->Position() - Position();
	boundBalls.push_back(bb);
}

Vaus::~Vaus()
{
	SAFE_DELETE(animation);
	for (auto bb : boundBalls)
		SAFE_DELETE(bb);
}

void Vaus::PhysicsUpdate()
{
	if (Key->Press(VK_LEFT))
		Translate(D3DXVECTOR2(-1, 0)*speed*Timer->Elapsed());
	else if (Key->Press(VK_RIGHT))
		Translate(D3DXVECTOR2(1, 0)*speed*Timer->Elapsed());

	if (Key->Down(VK_SPACE))
		Detach();
}

void Vaus::Detach()
{
	for (auto bb : boundBalls) {
		// assign directions according to the offset
		// Normalize offset, multiply it with speed and release
		D3DXVECTOR2 direction = bb->offset + D3DXVECTOR2(0, 12);
		D3DXVec2Normalize(&direction, &direction);
		bb->ball->SetVelocity(direction*100);

		SAFE_DELETE(bb);
	}
	boundBalls.clear();
}

void Vaus::Translate(D3DXVECTOR2 Translation)
{
	D3DXVECTOR2 pos = Position() + Translation;
	pos.x = Math::Clamp(
		pos.x, 8 + HalfSize().x, 
		216 - HalfSize().x);

	for (auto bb : boundBalls) {
		bb->ball->Position(pos + bb->offset);
	}
	Position(pos);
}

void Vaus::TryAttaching(IArkanoidPhysics * ball)
{
	ball->SetVelocity(D3DXVECTOR2(0, 0));
	boundBall* bb = new boundBall;
	bb->ball = ball;
	bb->offset = ball->Position() - Position();
	boundBalls.push_back(bb);

	//TODO: if catch is false, then instance release
	if (true) {
		Detach();
	}
}

void Vaus::Feed(int type)
{
}
