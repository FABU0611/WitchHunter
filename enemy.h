/*==============================================================================

	エネミー管理 [enemy.h]
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
#define ENEMY_MAX	(5)
#define ENEMY_WIDTH		(100.0f)	//テクスチャの横幅
#define ENEMY_HEIGHT	(100.0f)	//テクスチャの縦幅
#define ENEMY_RAD		(ENEMY_WIDTH * 0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetEnemyPos(int index);
bool GetEnemyState(int index);
void DieEnemy(int index);

void HitBulletforEnemy(int idx);
void HitChrageBulletforEnemy(int idx);
int GetEnemyLife(int index);

//*****************************************************************************
// グローバル変数
//*****************************************************************************