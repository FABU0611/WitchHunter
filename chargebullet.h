/*==============================================================================

	バレット管理 [chargebullet.h]
	Author :20106_田中蓮
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHARGEBULLET_MAX	(3)
#define CHARGEBULLET_WIDTH	(80.0f)
#define CHARGEBULLET_RAD	(CHARGEBULLET_WIDTH * 0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitChargeBullet(void);
void UninitChargeBullet(void);
void UpdateChargeBullet(void);
void DrawChargeBullet(void);

void SetChargeBullet(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetChargeBulletPos(int index);
bool GetChargeBulletUse(int index);
void EraseChargeBullet(int index);