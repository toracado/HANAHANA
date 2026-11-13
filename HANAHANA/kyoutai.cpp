#include "kyoutai.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Defines.h"

CKyoutai::CKyoutai()
	: m_pBuf(nullptr)
	, m_pTex(nullptr)
{
	//頂点バッファの作成
	float w = 550 * 0.5f;
	float h = 700 * 0.5f;
	Vertex vtx[] =
	{
		{{-w,-h,0.0f},{0.0f,0.0f}},
		{{-w, h,0.0f},{0.0f,1.0f}},
		{{ w,-h,0.0f},{1.0f,0.0f}},
		{{ w, h,0.0f},{1.0f,1.0f}},
	};
	m_pBuf = CreateVertexBuffer(GetDevice(), vtx, _countof(vtx));

	if (m_pBuf == nullptr)
	{
		MessageBox(NULL, "kyoutai", "Error", MB_OK);
	}

	HRESULT hr = LoadTextureFromFile(GetDevice(),"rom/kyotai.png", &m_pTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, "kyoutai", "Error", MB_OK);
	}
}

CKyoutai::~CKyoutai()
{
	if (m_pTex != nullptr)
	{
		m_pTex->Release();
		m_pTex = nullptr;
	}

	if (m_pBuf != nullptr)
	{
		m_pBuf->Release();
		m_pBuf = nullptr;
	}
}

void CKyoutai::Update()
{
}

void CKyoutai::Draw()
{
	SetSpritePos(0.0f, 100.0f);
	SetSpriteTexture(m_pTex);
	DrawSprite(m_pBuf);
	SetSpritePos(0.0f, 0.0f);

}
