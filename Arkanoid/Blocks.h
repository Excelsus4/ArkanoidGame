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
private:
	Sprite* GenerateBlockSprite(int x, int y);
private:
	int health;
};