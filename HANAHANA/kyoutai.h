#pragma once

#include "SpriteDrawer.h"

class CKyoutai
{
private:
	ID3D11Buffer* m_pBuf;
	ID3D11ShaderResourceView* m_pTex;

public:
	CKyoutai();
	~CKyoutai();
	void Update();
	void Draw();
};