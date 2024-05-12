/*==============================================================================

	ゲームシーン管理 [game.cpp]
	Author :20106_田中蓮
	Date   :23_06_30
--------------------------------------------------------------------------------

==============================================================================*/
#include "game.h"

#include "back.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "hone.h"
#include "boss.h"
#include "bossbullet.h"
#include "bossexplosion.h"
#include "collision.h"
#include "bullet.h"
#include "chargebullet.h"
#include "honebullet.h"
#include "sword.h"
#include "info.h"
#include "second.h"
#include "life.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	InitSecond();
	InitInfo();
	InitBack();
	InitBg();
	InitPlayer();
	InitLife();
	InitBullet();
	InitChargeBullet();
	InitSword();
	InitEnemy();
	InitHone();
	InitBoss();
	InitBossbullet();
	InitBossexplosion();
	BgSetEnemy();
	BgSetHone();
	BgSetBoss();
	InitHonebullet();

	InitCollision();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	UninitCollision();

	UninitHonebullet();
	UninitBossexplosion();
	UninitBossbullet();
	UninitBoss();
	UninitHone();
	UninitEnemy();
	UninitSword();
	UninitChargeBullet();
	UninitBullet();
	UninitLife();
	UninitPlayer();
	UninitBg();
	UninitBack();
	UninitInfo();
	UninitSecond();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	UpdatePlayer();
	UpdateLife();
	UpdateBullet();
	UpdateChargeBullet();
	UpdateSword();
	UpdateEnemy();
	UpdateHone();
	UpdateBoss();
	UpdateBossbullet();
	UpdateBossexplosion();
	UpdateHonebullet();
	UpdateBack();
	UpdateBg();
	UpdateInfo();
	UpdateSecond();

	UpdateCollision();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	DrawBack();
	DrawBg();
	DrawBgObj();
	DrawEnemy();
	DrawHone();
	DrawBoss();
	DrawBossbullet();
	DrawBossexplosion();
	DrawHonebullet();
	DrawPlayer();
	DrawBullet();
	DrawChargeBullet();
	DrawSword();
	DrawInfo();
	DrawLife();
	DrawSecond();

	DrawCollision();
}
