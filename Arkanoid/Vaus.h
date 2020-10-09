#pragma once
#include "IArkanoidPhysics.h"
#include "PowerUps.h"

class Vaus : public IArkanoidPhysics {
	struct boundBall {
		IArkanoidPhysics* ball;
		D3DXVECTOR2 offset;
	};
public:
	Vaus(World* world, IArkanoidPhysics* startingBall);
	virtual ~Vaus();

	virtual void PhysicsUpdate() override;
	virtual void TryAttaching(IArkanoidPhysics* ball) override;
private:
	void Translate(D3DXVECTOR2 Translation);
	void PowerUp(PowerUps* capsule);
	void Detach();
private:
	PowerUps::Type currentState;
	vector<boundBall*> boundBalls;
	float speed;
};