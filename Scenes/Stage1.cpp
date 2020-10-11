#include "stdafx.h"
#include "Stage1.h"

Stage1::Stage1(SceneValues * values) :
	Scene(values), 
	background(Textures+L"Arkanoid/Field.png", Shaders + L"009_Sprite.fx",
		0,0,224, 240)
{
	//=========================================================================
	// Initialization
	//=========================================================================
	background.Position(112, 120);
	Ball* ball = new Ball(&world);
	world.balls.push_back(ball);
	world.vaus = new Vaus(&world, ball);

	Blocks::BlockType type[6];
	type[0] = Blocks::BlockType::Green;
	type[1] = Blocks::BlockType::Magenta;
	type[2] = Blocks::BlockType::Blue;
	type[3] = Blocks::BlockType::Yellow;
	type[4] = Blocks::BlockType::Red;
	type[5] = Blocks::BlockType::Iron;

	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 13; i++) {
			Blocks* b = new Blocks(&world, type[j], 0);
			b->Position(16 * (i + 1), j * 8 + 156);
			world.blocks.push_back(b);
		}
	}
}

Stage1::~Stage1()
{
	//=========================================================================
	// Destruction
	//=========================================================================
	SAFE_DELETE(world.vaus);
	for (auto ball : world.balls)
		SAFE_DELETE(ball);
	for (auto block : world.blocks)
		SAFE_DELETE(block);
}

void Stage1::Update()
{
	//=========================================================================
	// Physics Update
	//=========================================================================
	((Vaus*)world.vaus)->PhysicsUpdate();
	for (auto ball : world.balls)
		((Ball*)ball)->PhysicsUpdate();
	for (auto block : world.blocks)
		((Blocks*)block)->PhysicsUpdate();

	// Trash check blocks and generate powerup
	for (auto iter = world.blocks.begin(); iter != world.blocks.end();) {
		if (((Blocks*)(*iter))->Health() <= 0) {
			//TODO: Try to make random powerup here
			delete ((Blocks*)(*iter));
			iter = world.blocks.erase(iter);
		}else{
			++iter;
		}
	}
	
	//=========================================================================
	// Update
	//=========================================================================
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	background.Update(V, P);
	for (auto ball : world.balls)
		((Ball*)ball)->Update(V, P);
	for (auto block : world.blocks)
		((Blocks*)block)->Update(V, P);
	((Vaus*)world.vaus)->Update(V, P);
}

void Stage1::Render()
{
	//=========================================================================
	// Render
	//=========================================================================
	background.Render();
	for (auto ball : world.balls)
		((Ball*)ball)->Render();
	for (auto block : world.blocks)
		((Blocks*)block)->Render();
	((Vaus*)world.vaus)->Render();
}