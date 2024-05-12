/*==============================================================================

	プレイヤー管理 [player.h]
	Author :20106_田中蓮
	Date   :23_06_09
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_POWER	(1)
#define CHARGEBULLET_POWER	(3)
#define PLAYER_CHECK_SIZE	(80.0f)	//テクスチャの横幅
#define PLAYER_RAD	(PLAYER_CHECK_SIZE * 0.5f)


#define PLAYER_LIFE		(10)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

D3DXVECTOR2 GetPlayerPos(void);
bool GetFightBoss(void);

bool GetPlayerDead(void);
void HitPlayer(void);
void HitPlayerforBoss(void);
void SetPlayerisHit(void);
int GetLife(void);


enum DIRECTION {				//向き
	up,
	down,
	left,
	right,
};