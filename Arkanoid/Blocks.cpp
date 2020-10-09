#include "stdafx.h"
#include "Blocks.h"

Blocks::Blocks(World * world, BlockType blocktype, int stage) :
	IArkanoidPhysics(world)
{
	animation = new Animation;

	switch (blocktype)
	{
	case Blocks::BlockType::White:
	case Blocks::BlockType::Orange:
	case Blocks::BlockType::Cyan:
	case Blocks::BlockType::Green:
	case Blocks::BlockType::Red:
	case Blocks::BlockType::Blue:
	case Blocks::BlockType::Magenta:
	case Blocks::BlockType::Yellow:
		health = 1;
		{
			Clip* clip = new Clip(PlayMode::End);
			clip->AddFrame(GenerateBlockSprite((int)blocktype%4, (int)blocktype/4), 0.2f);
			animation->AddClip(clip);
		}
		break;
	case Blocks::BlockType::Iron:
		health = stage / 4 + 2;
		{
			Clip* clip = new Clip(PlayMode::End);
			clip->AddFrame(GenerateBlockSprite(0, 2), 0.2f);
			clip->AddFrame(GenerateBlockSprite(1, 2), 0.2f);
			clip->AddFrame(GenerateBlockSprite(2, 2), 0.2f);
			clip->AddFrame(GenerateBlockSprite(3, 2), 0.2f);
			clip->AddFrame(GenerateBlockSprite(4, 2), 0.2f);
			clip->AddFrame(GenerateBlockSprite(5, 2), 0.2f);
			animation->AddClip(clip);
		}
		break;
	case Blocks::BlockType::Gold:
		health = INT_MAX;
		break;
	}

	Scale(1, 1);
	animation->Play(0);
	animation->DrawBound(true);
}

Blocks::~Blocks()
{
	SAFE_DELETE(animation);
}

void Blocks::PhysicsUpdate()
{
}

Sprite * Blocks::GenerateBlockSprite(int x, int y)
{
	wstring spriteFile = Textures + L"Arkanoid/Blocks.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";
	const int width = 16;
	const int height = 8;

	return new Sprite(spriteFile, shaderFile,
		x*width, y*height,
		(x + 1)*width, (y + 1)*height);
}
