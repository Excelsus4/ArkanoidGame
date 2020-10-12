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

	for (int i = 0; i < 14; i++) {
		pool.push_back(new PowerUps(&world, (PowerUps::Type)(i / 2 + 1)));
	}
}

Stage1::~Stage1()
{
	//=========================================================================
	// Destruction
	//=========================================================================
	SAFE_DELETE(world.vaus);
	for (auto ball : world.balls)
		delete (Ball*)ball;
	for (auto block : world.blocks)
		delete (Blocks*)block;
	for (auto pu : world.powerups)
		delete (PowerUps*)pu;
	for (auto pu : pool)
		delete pu;
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
	for (auto pu : world.powerups)
		((PowerUps*)pu)->PhysicsUpdate();

	// Trash check blocks and generate powerup
	for (auto iter = world.blocks.begin(); iter != world.blocks.end();) {
		if (((Blocks*)(*iter))->Health() <= 0) {
			// Try to make random powerup here
			if (world.balls.size() <= 1) {
				if (Math::Random(0, 1) < 0.1f) {
					int selection = Math::Random(0, pool.size() - 1);
					PowerUps* drops = pool[selection];
					pool.erase(pool.begin() + selection);
					drops->recycleFlag = false;
					drops->Position(((Blocks*)(*iter))->Position());
					world.powerups.push_back(drops);
				}
			}

			delete ((Blocks*)(*iter));
			iter = world.blocks.erase(iter);
		}
		else {
			++iter;
		}
	}

	// Trash check Powerup and recycle
	for (auto iter = world.powerups.begin(); iter != world.powerups.end();) {
		if (((PowerUps*)(*iter))->recycleFlag) {
			pool.push_back(((PowerUps*)(*iter)));
			iter = world.powerups.erase(iter);
		}
		else {
			++iter;
		}
	}

	// Find Ball and trash
	for (auto iter = world.balls.begin(); iter != world.balls.end();) {
		if (((Ball*)(*iter))->Position().y + ((Ball*)(*iter))->HalfSize().y < 0) {
			delete ((Ball*)(*iter));
			iter = world.balls.erase(iter);
		}
		else {
			++iter;
		}
	}

	//TODO: Check winning and losing condition...
	// Winning: if blocks are all gone -> End the game
	// Losing : if balls are all gone -> Reduce the life by one, and recreate a bound ball.if no life is left, end the game.
	if (world.balls.size() <= 0) {
		if (!((Vaus*)world.vaus)->CreateBall()) {
			//TODO: Lose the game...
		}
	}

	if (world.blocks.size() <= 0) {
		//TODO: win the game...
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
	for (auto pu : world.powerups)
		((PowerUps*)pu)->Update(V, P);
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
	for (auto pu : world.powerups)
		((PowerUps*)pu)->Render();

	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect;
		rect.left = 20;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

		rect.top += 20;
		rect.bottom += 20;

		text = L"Life : ";
		text += to_wstring(*world.life);
		DirectWrite::RenderText(text, rect);
	}
	DirectWrite::GetDC()->EndDraw();

}