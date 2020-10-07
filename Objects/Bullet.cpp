#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle, float speed)
{
	clip = new Clip(PlayMode::Loop);
	for (UINT i = 0; i < 7; i++) {
		float x = (float)i * 15 + 173;
		clip->AddFrame(new Sprite(Textures + L"Bullets.png", shaderFile, x, 155, x + 11, 167), 0.3f);
	}

	position = start;
	clip->Position(position);
	clip->Play();
	clip->DrawBound(true);
}

Bullet::~Bullet()
{
	SAFE_DELETE(clip);
}

void Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (Key->Press('I'))
		position.y += 200.0f * Timer->Elapsed();
	else if (Key->Press('K'))
		position.y -= 200.0f * Timer->Elapsed();

	if (Key->Press('J'))
		position.x -= 200.0f*Timer->Elapsed();
	else if (Key->Press('L'))
		position.x += 200.0f * Timer->Elapsed();

	clip->Position(position);
	clip->Update(V, P);
}

void Bullet::Render()
{
	clip->Render();
}

