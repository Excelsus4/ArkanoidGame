#include "stdafx.h"
#include "Fire.h"

Fire::Fire(wstring shaderFile, D3DXVECTOR2 start)
{
	clip = new Clip(PlayMode::Loop);

	for (UINT y = 0; y < 3; y++) {
		for (UINT x = 0; x < 5; x++) {
			float startX = (float)x * 43;
			float startY = (float)y * 67;

			float endX = (float)(x + 1) * 43;
			float endY = (float)(y + 1) * 67;

			clip->AddFrame(new Sprite(Textures + L"Fire.png", shaderFile, startX, startY, endX, endY), 0.1f);
		}
	}

	position = start;

	clip->Position(position);
	clip->Scale(1.5f, 1.5f);
	clip->Play();
	clip->DrawBound(true);
}

Fire::~Fire()
{
	SAFE_DELETE(clip);
}

void Fire::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	clip->Update(V, P);
}

void Fire::Render()
{
	clip->Render();
}
