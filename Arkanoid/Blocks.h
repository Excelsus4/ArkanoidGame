#pragma once
#include "IArkanoidPhysics.h"

class Blocks : public IArkanoidPhysics{
public:
	enum class BlockType {
		White=0, Orange, Cyan, Green, Red, Blue, Magenta, Yellow, Iron, Gold
	};
public:
	Blocks(World* world, BlockType blocktype, int stage);
	virtual ~Blocks();

	virtual void PhysicsUpdate() override;
	virtual void TryAttaching(IArkanoidPhysics* ball) override;

	int Health() const { return health; }
private:
	enum class Relation {
		NW=0, N, NE,
		W, C, E,
		SW, S, SE
	};
	Relation GetRelation(D3DXVECTOR4& block, D3DXVECTOR2& bCenter);
private:
	Sprite* GenerateBlockSprite(int x, int y);
private:
	int health;
};