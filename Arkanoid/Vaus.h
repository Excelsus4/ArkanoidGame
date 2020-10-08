#pragma once
#include "IArkanoidPhysics.h"
#include "PowerUps.h"

class Vaus : public IArkanoidPhysics {
public:
	Vaus(World* world);
	virtual ~Vaus();

	virtual void PhysicsUpdate() override;
	void PowerUp(PowerUps* capsule);

	void Translate(D3DXVECTOR2 Translation);
private:
	PowerUps::Type currentState;
	float speed;
};