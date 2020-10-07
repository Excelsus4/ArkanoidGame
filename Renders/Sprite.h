#pragma once

class Sprite {
public:
	Sprite(wstring textureFile, wstring shaderFile);
	Sprite(wstring textureFile, wstring shaderFile, float endX, float endY);
	Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY);
	virtual ~Sprite();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void DrawBound(bool val) { bDrawBound = val; }

public:
	bool AABB(D3DXVECTOR2& position);
	bool AABB(Sprite* b);

	static bool AABB(Sprite* a, D3DXVECTOR2& position);
	static bool AABB(Sprite* a, Sprite* b);

private:
	void CreateBound();

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() const { return position; }

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale() const { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	D3DXVECTOR3 Rotation() const { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	D3DXVECTOR3 RotationDegree() const;

	D3DXVECTOR2 TextureSize() const { return textureSize; }

private:
	void Initialize(wstring spriteFile, wstring shaderFile, float startX, float startY, float endX, float endY);

private:
	bool bDrawBound;
	wstring textureFile;

	Shader* shader;
	ID3D11Buffer* vertexBuffer;

	Shader* boundShader;
	ID3D11Buffer* boundVertexBuffer;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

	D3DXVECTOR2 textureSize;

	ID3D11ShaderResourceView* srv;

private:
	struct Vertex {
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
	};

	struct BoundVertex {
		D3DXVECTOR3 Position;
	};
};

//=============================================================================

class Sprites {
private:
	friend class Sprite;

private:
	static ID3D11ShaderResourceView* Load(wstring file);
	static void Remove(wstring file);

private:
	struct SpriteDesc {
		UINT RefCount = 0;
		ID3D11ShaderResourceView* SRV = NULL;
	};

	static map<wstring, SpriteDesc> spriteMap;
};