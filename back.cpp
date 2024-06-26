/*==============================================================================

	頂点管理 [back.cpp]
	Author :20106_田中蓮
	Date   :23_06_22
--------------------------------------------------------------------------------

==============================================================================*/
#include "back.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_WIDTH	(SCREEN_WIDTH)	//テクスチャの横幅
#define TEX_HEIGHT	(SCREEN_HEIGHT)	//テクスチャの縦幅

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int g_TexNo;
static float g_U;		//U値の横幅
static float g_V;		//V値の高さ

//=============================================================================
// 初期化処理
//=============================================================================
void InitBack(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/haikei00.png");

	g_U = 0.0f;
	g_V = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBack(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBack(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBack(void)
{
	//線をなくす
	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_WRAP);

	D3DXVECTOR2 base = GetBasePos();
	
	//背景の描画
	DrawSpriteLeftTop(g_TexNo,
		0.0f, 0.0f,
		TEX_WIDTH, TEX_HEIGHT,
		g_U - (base.x * 0.0001f), g_V,
		1.0f, 1.0f);
}