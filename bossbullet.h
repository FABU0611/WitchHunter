/*==============================================================================

	バレット管理 [bossbullet.h]
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
#define BOSSBULLET_MAX	(3)
#define BOSSBULLET_RAD	(BOSSBULLET_WIDTH * 0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBossbullet(void);
void UninitBossbullet(void);
void UpdateBossbullet(void);
void DrawBossbullet(void);

void SetBossbullet(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetBossbulletPos(int index);
bool GetBossbulletUse(int index);
void EraseBossbullet(int index);