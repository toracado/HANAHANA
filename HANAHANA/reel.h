#pragma once

#include "SpriteDrawer.h"

class CReel
{
public:
	enum State
	{
		IDLE,//リールが動いていない状態
		ROTATE,//リールが回っている状態
		SLIDE,//リールが滑る状態
		STOP,//リールが止まる状態
		WAIT,//リールが動き始めるまでの状態
	};
private:
	ID3D11Buffer* m_pBuf[3];
	ID3D11ShaderResourceView* m_pTex[3];

	State m_state[3];
	float2 m_pos[3];
	float2 m_UVpos[3];
	float m_reelOffsetU[3];
	int frame;
	int reelwait;


public:
	CReel();
	~CReel();
	void Update();
	void Draw();

private:
	void UpdateIdle();
	void UpdateRotate(int reel);
	void UpdateSlide(int reel);
	void UpdateStop();
	void UpdateWait();

};