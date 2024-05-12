/*==============================================================================

	バレット管理 [bossexplosion.h]
	Author :20106_田中蓮
	Date   :23_09_14
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBossexplosion(void);
void UninitBossexplosion(void);
void UpdateBossexplosion(void);
void DrawBossexplosion(void);

void SetBossexplosion(float posx, float posy);
D3DXVECTOR2 GetBossexplosionPos(void);
bool GetBossexplosionUse(void);
void EraseBossexplosion(void);
float GetBossexplosionRad(void);