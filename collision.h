/*==============================================================================

	当たり判定管理 [collision.h]
	Author :20106_田中蓮
	Date   :23_06_13
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
void InitCollision(void);
void UninitCollision(void);
void UpdateCollision(void);
void DrawCollision(void);

bool CheckHitBCSq(float ccla_x, float ccla_y, float ccla_r,
	float cclb_x, float cclb_y, float cclb_r);
bool CheckDisp(float objx, float objy, float objw, float objh);
bool CheckLoadRight(float objx, float objy, float objw, float objh);

void ToPlayer(void);