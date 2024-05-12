/*==============================================================================

	タイトル画面管理 [result.cpp]
	Author :20106_田中蓮
	Date   :23_07_06
--------------------------------------------------------------------------------

==============================================================================*/
#include "result.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "second.h"
#include "scene.h"
#include "fade.h"
#include "clearrunk.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON 100	//表示するポリゴンの数

#define ANIME_PTN_YOKO 10
#define ANIME_PTN_TATE 1
#define ANIME_PTN_WAIT 5

#define ANIME_PTN (ANIME_PTN_YOKO * ANIME_PTN_TATE)
#define ANIME_PTN_U (1.0f / ANIME_PTN_YOKO)
#define ANIME_PTN_V (1.0f / ANIME_PTN_TATE)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_TexNo = 0;
static int			g_BackNo = 0;
static int			g_SecondTexNo = 0;

static unsigned int g_ClearTime = 0;

static float		g_U;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/result01.png");
	g_BackNo = LoadTexture((char*)"data/TEXTURE/resultback.png");
	g_SecondTexNo = LoadTexture((char*)"data/TEXTURE/number000.png");

	g_ClearTime = GetClearTime();
	InitClearrunk();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	UninitClearrunk();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	//キーボード
	if (GetKeyboardTrigger(DIK_SPACE) ||
		IsButtonTriggered(0, BUTTON_Y))
	{
		//現在フェード中でないなら
		if (GetFadeState() == SCENE_NONE) {
			//ゲームシーン遷移
			SetScene(SCENE_TITLE);
		}
	}
	UpdateClearrunk();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	DrawSpriteLeftTop(g_BackNo,
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,//UV値の始点
		1.0f, 1.0f);

	DrawSpriteLeftTop(g_TexNo,
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,//UV値の始点
		1.0f, 1.0f);

	int second = g_ClearTime;
	//時間表示
	for (int i = 0; i < 3; i++)
	{
		//スコアの描画
		DrawSpriteColor(g_SecondTexNo,
			1280.0f - (150.0f * i), 465.0f,
			150.0f, 200.0f,
			ANIME_PTN_U * (second % 10), 0.0f,//UV値の始点
			ANIME_PTN_U, ANIME_PTN_V,
			1.0f, 1.0f, 1.0f, 1.0f);

		second = second / 10;
	}
	DrawClearrunk();
}
