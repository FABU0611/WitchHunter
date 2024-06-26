/*==============================================================================

	剣管理 [sword.h]
	Author :20106_田中蓮
	Date   :23_06_29
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SWORD_WIDTH		(62.0f)
#define SWORD_HEIGHT	(120.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSword(void);
void UninitSword(void);
void UpdateSword(void);
void DrawSword(void);

D3DXVECTOR2 GetSwordPos(void);
D3DXVECTOR2 GetInhitPos(void);
bool GetSwordUse(void);
void SetSword(float posx, float posy, float dirx, float diry);