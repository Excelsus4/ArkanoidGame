#include "stdafx.h"
#include "Ball.h"

Ball::Ball(World * world):
	IArkanoidPhysics(world), isCaught(false), multiplier(1)
{
	animation = new Animation;
	wstring spriteFile = Textures + L"Arkanoid/Vaus.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	{
		Clip* clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 40, 5, 44), 0.2f);
		animation->AddClip(clip);
	}

	shadowOffset = D3DXVECTOR2(4, -4);
	shadow = new Sprite(spriteFile, shaderFile, 4, 44, 9, 48);

	Position(120, 26);
	Scale(1, 1);
	animation->Play(0);
	animation->DrawBound(true);
}

Ball::~Ball()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(shadow);
}

void Ball::PhysicsUpdate()
{
	if (!isCaught) {
		//=========================================================================
		// Translation
		//=========================================================================
		D3DXVECTOR2 pos = Position();
		pos += velocity * Timer->Elapsed() * multiplier;

		//=========================================================================
		// Wall
		//=========================================================================
		if (pos.y > 232 - HalfSize().y) {
			Reflect(D3DXVECTOR2(0, -1));
			pos.y = 232 - HalfSize().y;
		}
		else if (pos.y < 0 + HalfSize().y) {
			Reflect(D3DXVECTOR2(0, 1));
			pos.y = 0 + HalfSize().y;
		}

		if (pos.x < 8 + HalfSize().x) {
			Reflect(D3DXVECTOR2(1, 0));
			pos.x = 8 + HalfSize().x;
		}
		else if (pos.x > 216 - HalfSize().x) {
			Reflect(D3DXVECTOR2(-1, 0));
			pos.x = 216 - HalfSize().x;
		}

		//=========================================================================
		// Vaus
		//=========================================================================
		if (velocity.y < 0) {
			if (CheckCollision((IArkanoidPhysics*)world->vaus)) {
				// Try to bind the ball onto the vaus
				((IArkanoidPhysics*)world->vaus)->TryAttaching(this);
				//SetVelocity(D3DXVECTOR2(0, 0));
			}
		}

		//=========================================================================
		// Finalize
		//=========================================================================
		Position(pos);
	}
}

void Ball::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	shadow->Update(V, P);
	__super::Update(V, P);
}

void Ball::Render()
{
	shadow->Render();
	__super::Render();
}

void Ball::Reflect(D3DXVECTOR2 normal)
{
	velocity = velocity - 2 * D3DXVec2Dot(&velocity, &normal) * normal;
	multiplier += 0.02f;
	if (multiplier > 3)
		multiplier = 3;
}

void Ball::SetVelocity(D3DXVECTOR2 vec)
{
	velocity = vec;
	if (velocity.x == 0 && velocity.y == 0)
		isCaught = true;
	else
		isCaught = false;
}
