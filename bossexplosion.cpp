/*==============================================================================

	バレット管理 [bossexplosion.cpp]
	Author :20106_田中蓮
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "bossexplosion.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSSEXPLOSION_SIZE	(700.0f)
#define BOSSEXPLOSION_SPEED	(40.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct BOSSEXPLOSION {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXCOLOR		color;	//頂点カラー
	float			rad;	//半径
	bool			use;

	float			U;		//U値の横幅
	float			V;		//V値の高さ
};

static BOSSEXPLOSION g_Bossexplosion;

static int g_TexNo = 0;
static int g_SoundNo = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitBossexplosion(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/explosion.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/b_explosion.wav");
	SetVolume(g_SoundNo, 0.2f);

	//初期化
	g_Bossexplosion.pos.x = 0.0f;
	g_Bossexplosion.pos.y = 0.0f;
	g_Bossexplosion.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Bossexplosion.rad = 0.0f;
	g_Bossexplosion.use = false;
	g_Bossexplosion.U = 0.0f;
	g_Bossexplosion.V = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossexplosion(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossexplosion(void)
{
	if (!g_Bossexplosion.use) {
		return;
	}

	//大きさ更新
	if (g_Bossexplosion.rad < BOSSEXPLOSION_SIZE) {
		g_Bossexplosion.rad += BOSSEXPLOSION_SPEED;
	}

	//画面外に出たら消す
	if (g_Bossexplosion.rad >= BOSSEXPLOSION_SIZE) {
		EraseBossexplosion();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossexplosion(void)
{
	if (!g_Bossexplosion.use) {
		return;
	}
	//バレットの描画
	DrawSpriteColorBase(g_TexNo,
		g_Bossexplosion.pos.x, g_Bossexplosion.pos.y,
		g_Bossexplosion.rad, g_Bossexplosion.rad,
		g_Bossexplosion.U, g_Bossexplosion.V,
		1.0f, 1.0f,
		g_Bossexplosion.color.r, g_Bossexplosion.color.g, g_Bossexplosion.color.b, g_Bossexplosion.color.a);
}

//弾を打つ---------------------------------------------------------------------
void SetBossexplosion(float posx, float posy){
	if (g_Bossexplosion.use) {
		return;
	}
	g_Bossexplosion.pos.x = posx;
	g_Bossexplosion.pos.y = posy;
	g_Bossexplosion.rad = 50.0f;

	g_Bossexplosion.use = true;

	PlaySound(g_SoundNo, 0);
}

//Bossexplosionの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetBossexplosionPos(void){
	return g_Bossexplosion.pos;
}

//Bossexplosionが表示されているかを渡す-----------------------------------------------
bool GetBossexplosionUse(void){
	return g_Bossexplosion.use;
}

//Bossexplosionを消す-----------------------------------------------------------------
void EraseBossexplosion(void){
	g_Bossexplosion.use = false;
}

float GetBossexplosionRad(void) {
	return g_Bossexplosion.rad * 0.5f;
}