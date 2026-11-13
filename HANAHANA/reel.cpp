#include "reel.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "DirectX.h"
#include "Defines.h"
#include "keyboard.h"

float maxreelheight = REEL_HEIGHT * 7.0f;//リールの最大の高さ
float oneminmove = maxreelheight / 0.78f;//一秒間に進む量
float oneframemove = oneminmove / FPS;//一フレームで進む量
float rate = oneframemove / maxreelheight;//一フレームで進む割合

CReel::CReel()
	: m_pBuf{ nullptr }
	, m_pTex{ nullptr }
	, m_pos{ 0.0f,0.0f }
	, m_UVpos{ 0.0f,0.0f }
	, m_reelOffsetU{ 0.0f ,0.0f ,0.0f }
	, frame(0)
	, m_state{ CReel::State::ROTATE,CReel::State::ROTATE,CReel::State::ROTATE }
	, reelwait(0)
{
	//頂点座標の作成
	float w = REEL_WIDTH * 0.5f;
	float h = REEL_HEIGHT * 0.5f;
	Vertex vtx[] =
	{
		{{-w,-h,0.0f},{0.0f,0.0f}},
		{{-w, h,0.0f},{0.0f,1.0f}},
		{{ w,-h,0.0f},{1.0f,0.0f}},
		{{ w, h,0.0f},{1.0f,1.0f}},
	};
	for (int i = 0; i < 3;i++)
	{
		m_pBuf[i] = CreateVertexBuffer(GetDevice(), vtx, _countof(vtx));
	}

	//テクスチャの読み込み
	const char* texture[] =
	{
		"rom/leftreel.png",
		"rom/centerreel.png",
		"rom/rightreel.png",
	};

	HRESULT hr;

	for (int i = 0; i < 3;i++)
	{
		hr = LoadTextureFromFile(GetDevice(), texture[i], &m_pTex[i]);
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "reel", "Load Error", MB_OK);
	}

	m_pos[0].x = -140.0f;
	m_pos[0].y = 8.0f;

	


	m_pos[1].x = -3.0f;
	m_pos[1].y = 8.0f;


	m_pos[2].x = 137.0f;
	m_pos[2].y = 8.0f;
}

CReel::~CReel()
{
	for (int i = 0; i < 3;i++)
	{
		if (m_pTex[i] != nullptr)
		{
			m_pTex[i]->Release();
			m_pTex[i] = nullptr;
		}

		if (m_pBuf[i] != nullptr)
		{
			m_pBuf[i]->Release();
			m_pBuf[i] = nullptr;
		}
	}
}

void CReel::Update()
{
	for (int i = 0;i < 3;i++)
	{
		switch (m_state[i])//ステートごとにアップデートを変更
		{
		case CReel::State::IDLE:   UpdateIdle();   break;
		case CReel::State::ROTATE: UpdateRotate(i); break;
		case CReel::State::SLIDE:  UpdateSlide(i);  break;
		case CReel::State::STOP:   UpdateStop();   break;
		case CReel::State::WAIT:   UpdateWait();   break;
		}
	}
	reelwait++;
}

void CReel::Draw()
{
	for (int i = 0; i < 3;i++)
	{
		SetSpriteUVScale(1.0f, REEL_UV_WIDTH);
		SetSpriteUVPos(0.0f, m_reelOffsetU[i]);
		SetSpritePos(m_pos[i].x, m_pos[i].y);
		SetSpriteTexture(m_pTex[i]);
		DrawSprite(m_pBuf[i]);
	}
	SetSpriteUVScale(1.0f, 1.0f);
	SetSpriteUVPos(0.0f, 0.0f);

}

void CReel::UpdateIdle()
{
	static bool enter = false;
	if (IsKeyTrigger(VK_RETURN))
		enter = true;
	if (enter && IsKeyTrigger(VK_UP))
	{
		for (int i = 0; i < 3;i++)
		{
			m_state[i] = CReel::WAIT;
		}
	}
}

void CReel::UpdateRotate(int reel)
{
	if (reel == 0)
	{
		m_reelOffsetU[0] -= rate;

		if (IsKeyTrigger(VK_LEFT))
		{
			m_state[0] = CReel::SLIDE;
		}

		if (m_reelOffsetU[0] <= -1.0f)
			m_reelOffsetU[0] = 0.0f;
	}
	else if (reel == 1)
	{
		m_reelOffsetU[1] -= rate;

		if (IsKeyTrigger(VK_DOWN))
		{
			m_state[1] = CReel::SLIDE;
		}

		if (m_reelOffsetU[1] <= -1.0f)
			m_reelOffsetU[1] = 0.0f;
	}
	else if (reel == 2)
	{
		m_reelOffsetU[2] -= rate;

		if (IsKeyTrigger(VK_RIGHT))
		{
			m_state[2] = CReel::SLIDE;
		}

		if (m_reelOffsetU[2] <= -1.0f)
			m_reelOffsetU[2] = 0.0f;
	}

	frame++;

	if (frame >= FPS)
		frame = 0;
}

void CReel::UpdateSlide(int reel)
{
	const float reelrate = 0.04761904f;
	float index[3];
	index[reel] = -m_reelOffsetU[reel] / reelrate;
	if (-m_reelOffsetU[reel] >= 0.9523808f)
		index[reel] = 0.0f;
	float nextUV[3];
	nextUV[reel] = ceilf(index[reel]) * reelrate;

	if (reel == 0)
	{
		if (nextUV[0] != 0.0f)
		{
			if (nextUV[0] - (-m_reelOffsetU[0]) > rate)
			{
				m_reelOffsetU[0] -= rate;

				if (m_reelOffsetU[0] <= -1.0f)
					m_reelOffsetU[0] = 0.0f;

				return;
			}
		}
		else
		{
			if (1.0f - m_reelOffsetU[0] >= rate)
			{
				m_reelOffsetU[0] -= rate;

				if (m_reelOffsetU[0] <= -1.0f)
					m_reelOffsetU[0] = 0.0f;

				return;
			}
		}

		m_state[0] = CReel::IDLE;

	}
	else if (reel == 1)
	{
		if (nextUV[1] != 0.0f)
		{
			if (nextUV[1] - (-m_reelOffsetU[1]) > rate)
			{
				m_reelOffsetU[1] -= rate;

				if (m_reelOffsetU[1] <= -1.0f)
					m_reelOffsetU[1] = 0.0f;

				return;
			}
		}
		else
		{
			if (1.0f - m_reelOffsetU[1] >= rate)
			{
				m_reelOffsetU[1] -= rate;

				if (m_reelOffsetU[1] <= -1.0f)
					m_reelOffsetU[1] = 0.0f;

				return;
			}
		}
		
		m_state[1] = CReel::IDLE;

	}
	else if (reel == 2)
	{
		if (nextUV[2] != 0.0f)
		{
			if (nextUV[2] - (-m_reelOffsetU[2]) > rate)
			{
				m_reelOffsetU[2] -= rate;

				if (m_reelOffsetU[2] <= -1.0f)
					m_reelOffsetU[2] = 0.0f;

				return;
			}
		}
		else
		{
			if (1.0f - m_reelOffsetU[2] >= rate)
			{
				m_reelOffsetU[2] -= rate;

				if (m_reelOffsetU[2] <= -1.0f)
					m_reelOffsetU[2] = 0.0f;

				return;
			}
		}
		
		m_state[2] = CReel::IDLE;

	}

}

void CReel::UpdateStop()
{
}

void CReel::UpdateWait()
{
	if (reelwait >= 492)
	{
		for (int i = 0; i < 3;i++)
		{
			m_state[i] = CReel::ROTATE;
		}
		reelwait = 0;
	}
}
