#pragma once
#include "IArkanoidPhysics.h"

class PowerUps : public IArkanoidPhysics {
public:
	enum Type{
		Paddle = 1,
		Lasers,
		Enlarge,
		Catch,
		Slow,
		Break,
		Disruption
	};
public:
	PowerUps(World* world, Type type);
	virtual ~PowerUps();

	virtual void PhysicsUpdate() override;
public:
	bool recycleFlag;
private:
	Type type;
	D3DXVECTOR2 velocity;
};