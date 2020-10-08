#pragma once
#include "stdafx.h"

struct World {
	/*vector<IArkanoidPhysics*> blocks;
	vector<IArkanoidPhysics*> balls;
	vector<IArkanoidPhysics*> powerups;*/
	void* vaus;
};

class IArkanoidPhysics {
public:
	IArkanoidPhysics(World* world) : world(world) {};
	virtual ~IArkanoidPhysics() {};

	virtual void PhysicsUpdate() = 0;
	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) { animation->Update(V, P); }
	virtual void Render() { animation->Render(); }

	virtual bool CheckCollision(IArkanoidPhysics* b) { return GetSprite()->AABB(b->GetSprite()); }
	virtual Sprite* GetSprite() { return animation->GetSprite(); }

	virtual D3DXVECTOR2 Position() const { return animation->Position(); }
	virtual void Position(float x, float y) { Position(D3DXVECTOR2(x, y)); }
	virtual void Position(D3DXVECTOR2 vec) { animation->Position(vec); }

	virtual D3DXVECTOR2 Scale() const { return animation->Scale(); }
	virtual void Scale(float x, float y) { Scale(D3DXVECTOR2(x, y)); }
	virtual void Scale(D3DXVECTOR2 vec) { animation->Scale(vec); }

	//Arkanoid Objects never rotates.
protected:
	World* world;
	Animation* animation;
};