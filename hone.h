/*==============================================================================

	エネミー管理 [hone.h]
	Author :20106_田中蓮
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HONE_MAX	(5)
#define HONE_WIDTH		(110.0f)	//テクスチャの横幅
#define HONE_HEIGHT		(110.0f)	//テクスチャの縦幅
#define HONE_RAD		(HONE_WIDTH * 0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitHone(void);
void UninitHone(void);
void UpdateHone(void);
void DrawHone(void);

void SetHone(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetHonePos(int index);
bool GetHoneLive(int index);
void DieHone(int index);
int GetHoneLife(int index);

void HitBulletforHone(int idx);
void HitChrageBulletforHone(int idx);

//*****************************************************************************
// グローバル変数
//*****************************************************************************