#include "Game.h"
#include "kyoutai.h"
#include "reel.h"

CKyoutai* g_pkyoutai;
CReel* g_preel;

bool InitGame()
{
	g_pkyoutai = new CKyoutai();



	return true;
}
void UninitGame()
{
}

void UpdateGame()
{
	g_pkyoutai->Update();
}

void DrawGame()
{
	g_pkyoutai->Draw();
}