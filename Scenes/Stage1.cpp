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
	world.vaus = new Vaus(&world);
}

Stage1::~Stage1()
{
	//=========================================================================
	// Destruction
	//=========================================================================
	SAFE_DELETE(world.vaus);
}

void Stage1::Update()
{
	//=========================================================================
	// Physics Update
	//=========================================================================
	((Vaus*)world.vaus)->PhysicsUpdate();

	//=========================================================================
	// Update
	//=========================================================================
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	background.Update(V, P);
	((Vaus*)world.vaus)->Update(V, P);
}

void Stage1::Render()
{
	//=========================================================================
	// Render
	//=========================================================================
	background.Render();
	((Vaus*)world.vaus)->Render();
}