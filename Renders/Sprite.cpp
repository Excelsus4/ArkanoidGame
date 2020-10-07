#include "stdafx.h"
#include "Sprite.h"

//-----------------------------------------------------------------------------
// Sprite
//-----------------------------------------------------------------------------

Sprite::Sprite(wstring textureFile, wstring shaderFile)
{
	Initialize(textureFile, shaderFile, 0, 0, 0, 0);
}

Sprite::Sprite(wstring textureFile, wstring shaderFile, float endX, float endY)
{
	Initialize(textureFile, shaderFile, 0, 0, endX, endY);
}

Sprite::Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY)
{
	Initialize(textureFile, shaderFile, startX, startY, endX, endY);
}

Sprite::~Sprite()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);

	Sprites::Remove(textureFile);
}

void Sprite::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	boundShader->AsMatrix("View")->SetMatrix(V);
	boundShader->AsMatrix("Projection")->SetMatrix(P);

	D3DXMATRIX W, S, R, T;
	D3DXMatrixScaling(&S, scale.x * textureSize.x, scale.y * textureSize.y, 1.0f);
	D3DXMatrixRotationY(&R, rotation.y);
	D3DXMatrixTranslation(&T, position.x + scale.x * 0.5f, position.y + scale.y*0.5f, 0.0f);
	W = S * R * T;

	shader->AsMatrix("World")->SetMatrix(W);
	boundShader->AsMatrix("World")->SetMatrix(W);
}

void Sprite::Render()
{
	//ImGui::LabelText("Position", "%f, %f", position.x, position.y);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);

	if (bDrawBound == true) {
		stride = sizeof(BoundVertex);
		offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &boundVertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		boundShader->Draw(0, 0, 5);
	}
}

bool Sprite::AABB(D3DXVECTOR2 & position)
{
	return AABB(this, position);
}

bool Sprite::AABB(Sprite * b)
{
	return AABB(this, b);
}

bool Sprite::AABB(Sprite * a, D3DXVECTOR2 & position)
{
	float xScale = a->scale.x * a->TextureSize().x * 0.5f;
	float yScale = a->scale.y * a->TextureSize().y * 0.5f;

	float left = a->position.x - xScale;
	float right = a->position.x + xScale;
	float bottom = a->position.y - yScale;
	float top = a->position.y + yScale;

	bool b = true;
	b &= position.x > left;
	b &= position.x <= right;
	b &= position.y > bottom;
	b &= position.y <= top;

	return b;
}

bool Sprite::AABB(Sprite * a, Sprite * b)
{
	float xScale = a->scale.x * a->TextureSize().x * 0.5f;
	float yScale = a->scale.y * a->TextureSize().y * 0.5f;

	float leftA = a->position.x - xScale;
	float rightA = a->position.x + xScale;
	float bottomA = a->position.y - yScale;
	float topA = a->position.y + yScale;

	xScale = b->scale.x * b->TextureSize().x * 0.5f;
	yScale = b->scale.y * b->TextureSize().y * 0.5f;

	float leftB = b->position.x - xScale;
	float rightB = b->position.x + xScale;
	float bottomB = b->position.y - yScale;
	float topB = b->position.y + yScale;

	bool bCheck = true;
	bCheck &= leftA < rightB;
	bCheck &= topA > bottomB;
	bCheck &= rightA > leftB;
	bCheck &= bottomA < topB;

	return bCheck;
}

void Sprite::CreateBound()
{
	BoundVertex vertices[5];

	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoundVertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &boundVertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Sprite::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Sprite::Position(D3DXVECTOR2 & vec)
{
	position = vec;
}

void Sprite::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Sprite::Scale(D3DXVECTOR2 & vec)
{
	scale = vec;
}

void Sprite::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Sprite::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;
}

void Sprite::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Sprite::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

D3DXVECTOR3 Sprite::RotationDegree() const
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
}

void Sprite::Initialize(wstring spriteFile, wstring shaderFile, float startX, float startY, float endX, float endY)
{
	bDrawBound = false;
	textureFile = spriteFile;

	shader = new Shader(shaderFile);
	boundShader = new Shader(Shaders + L"015_Bounding.fx");
	
	srv = Sprites::Load(spriteFile);
	
	shader->AsShaderResource("Map")->SetResource(srv);

	Position(0, 0);
	Scale(1, 1);
	Rotation(0, 0, 0);

	HRESULT hr;
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile(spriteFile.c_str(), NULL, &info, NULL);
	assert(SUCCEEDED(hr));

	startX = (startX > 0) ? startX / (float)info.Width : 0.0f;
	startY = (startY > 0) ? startY / (float)info.Height : 0.0f;

	endX = (endX > 0) ? endX / (float)info.Width : 1.0f;
	endY = (endY > 0) ? endY / (float)info.Height : 1.0f;

	Vertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(startX, endY);
	vertices[1].Uv = D3DXVECTOR2(startX, startY);
	vertices[2].Uv = D3DXVECTOR2(endX, endY);
	vertices[3].Uv = D3DXVECTOR2(endX, endY);
	vertices[4].Uv = D3DXVECTOR2(startX, startY);
	vertices[5].Uv = D3DXVECTOR2(endX, startY);

	float sizeX = (endX > 0) ? endX * (float)info.Width : (float)info.Width;
	sizeX -= startX * (float)info.Width;

	float sizeY = (endY > 0) ? endY * (float)info.Height : (float)info.Height;
	sizeY -= startY * (float)info.Height;

	textureSize = D3DXVECTOR2(sizeX, sizeY);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
	
	CreateBound();
}

//-----------------------------------------------------------------------------
// Sprites
//-----------------------------------------------------------------------------

map<wstring, Sprites::SpriteDesc> Sprites::spriteMap;

ID3D11ShaderResourceView * Sprites::Load(wstring file)
{
	if (spriteMap.count(file) > 0) {
		spriteMap[file].RefCount++;

		return spriteMap[file].SRV;
	}
	HRESULT hr;
	ID3D11ShaderResourceView* srv;
	hr = D3DX11CreateShaderResourceViewFromFile(Device, file.c_str(), NULL, NULL, &srv, NULL);
	assert(SUCCEEDED(hr));

	SpriteDesc desc;
	desc.RefCount++;
	desc.SRV = srv;
	spriteMap[file] = desc;

	return desc.SRV;
}

void Sprites::Remove(wstring file)
{
	UINT count = spriteMap.count(file);
	assert(count > 0);
	spriteMap[file].RefCount--;
	if (spriteMap[file].RefCount < 1) {
		SAFE_RELEASE(spriteMap[file].SRV);

		spriteMap.erase(file);
	}
}
