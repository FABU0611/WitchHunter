/*==============================================================================

	シーン画面管理 [scene.cpp]
	Author :20106_田中蓮
	Date   :23_07_06
--------------------------------------------------------------------------------

==============================================================================*/
#include "scene.h"

#include "title.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SCENE g_Scene = SCENE_NONE;
static SCENE g_NextScene = SCENE_NONE;

//=============================================================================
// 初期化処理
//=============================================================================
void InitScene(void)
{
	//シーンを更新
	g_Scene = g_NextScene;

	switch (g_Scene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	default:
		break;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_RESULT:
		UninitResult();
		break;
	default:
		break;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	default:
		break;
	}
}


void SetScene(SCENE next_scene)
{
	//移行先シーンの設定を行う
	g_NextScene = next_scene;
}

void CheckScene(void)
{
	if (g_Scene != g_NextScene)
	{
		//現在実行中のシーンを終了させる
		UninitScene();

		//移行先のシーンを初期化
		InitScene();
	}
}