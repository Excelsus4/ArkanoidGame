#include "stdafx.h"
#include "Vaus.h"

Vaus::Vaus(World * world):
	IArkanoidPhysics(world), speed(100.0f)
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
}

Vaus::~Vaus()
{
	SAFE_DELETE(animation);
}

void Vaus::PhysicsUpdate()
{
	if (Key->Press(VK_LEFT))
		Translate(D3DXVECTOR2(-1, 0)*speed*Timer->Elapsed());
	else if (Key->Press(VK_RIGHT))
		Translate(D3DXVECTOR2(1, 0)*speed*Timer->Elapsed());
}

void Vaus::PowerUp(PowerUps * capsule)
{

}

void Vaus::Translate(D3DXVECTOR2 Translation)
{
	D3DXVECTOR2 pos = Position() + Translation;
	pos.x = Math::Clamp(
		pos.x, 8 + HalfSize().x, 
		216 - HalfSize().x);
	Position(pos);
}
