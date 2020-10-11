#pragma once
#include "IArkanoidPhysics.h"

class Ball : public IArkanoidPhysics{
public:
	Ball(World* world);
	virtual ~Ball();

	virtual void PhysicsUpdate() override;
	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

	virtual D3DXVECTOR2 Position() const { return animation->Position(); }
	virtual void Position(float x, float y) { Position(D3DXVECTOR2(x, y)); }
	virtual void Position(D3DXVECTOR2 vec) { shadow->Position(vec + shadowOffset); __super::Position(vec); }

	virtual D3DXVECTOR2 Scale() const { return animation->Scale(); }
	virtual void Scale(float x, float y) { Scale(D3DXVECTOR2(x, y)); }
	virtual void Scale(D3DXVECTOR2 vec) { shadow->Scale(vec); __super::Scale(vec); }

	virtual void Reflect(D3DXVECTOR2 normal) override;
	virtual void SetVelocity(D3DXVECTOR2 vec) override;
	virtual void Feed(int type) override;
private:
	Sprite* shadow;
	D3DXVECTOR2 shadowOffset;
	D3DXVECTOR2 velocity;
	float multiplier;
	bool isCaught;
};