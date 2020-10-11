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
			clip->AddFrame(GenerateBlockSprite(0, 2), 0.1f);
			clip->AddFrame(GenerateBlockSprite(1, 2), 0.1f);
			clip->AddFrame(GenerateBlockSprite(2, 2), 0.1f);
			clip->AddFrame(GenerateBlockSprite(3, 2), 0.1f);
			clip->AddFrame(GenerateBlockSprite(4, 2), 0.1f);
			clip->AddFrame(GenerateBlockSprite(5, 2), 0.1f);
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

void Blocks::TryAttaching(IArkanoidPhysics * ball)
{
	animation->GetClip()->Play(0);
	--(this->health);

	//TODO: make ball bounce back by giving it a normal vector
	D3DXVECTOR4 blockR;
	blockR.w = Position().x - HalfSize().x;
	blockR.x = Position().x + HalfSize().x;
	blockR.y = Position().y + HalfSize().y;
	blockR.z = Position().y - HalfSize().y;

	Relation rl = GetRelation(blockR, ball->Position());
	D3DXVECTOR2 normal;
	D3DXVECTOR2 point;
	if (rl == Blocks::Relation::W || rl == Blocks::Relation::E) {
		int a = 0;
	}
	switch (rl)
	{
	case Blocks::Relation::N:
	case Blocks::Relation::W:
	case Blocks::Relation::E:
	case Blocks::Relation::S:
		normal = D3DXVECTOR2((float)(((int)rl) % 3 - 1), -(float)(((int)rl) / 3 - 1));
		break;
	case Blocks::Relation::NW:
		point = D3DXVECTOR2(blockR.w, blockR.y);
		goto DIAGONAL_COMMON;
	case Blocks::Relation::NE:
		point = D3DXVECTOR2(blockR.x, blockR.y);
		goto DIAGONAL_COMMON;
	case Blocks::Relation::SW:
		point = D3DXVECTOR2(blockR.w, blockR.z);
		goto DIAGONAL_COMMON;
	case Blocks::Relation::SE:
		point = D3DXVECTOR2(blockR.x, blockR.z);
		goto DIAGONAL_COMMON;
	DIAGONAL_COMMON:
		normal = ball->Position() - point;
		D3DXVec2Normalize(&normal, &normal);
		break;
	case Blocks::Relation::C:
	default:
		assert(false);
		break;
	}
	ball->Reflect(normal);
}

Blocks::Relation Blocks::GetRelation(D3DXVECTOR4& block, D3DXVECTOR2& bCenter)
{
	int answer = 0;

	if (bCenter.x >= block.x)
		answer += 2;
	else if (bCenter.x >= block.w)
		answer += 1;

	if (bCenter.y <= block.z)
		answer += 6;
	else if (bCenter.y <= block.y)
		answer += 3;

	assert(answer != 4);
	return (Relation)answer;
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
