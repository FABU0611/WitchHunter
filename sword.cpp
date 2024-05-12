/*==============================================================================

	剣管理 [sword.cpp]
	Author :20106_田中蓮
	Date   :23_06_29
--------------------------------------------------------------------------------

==============================================================================*/
#include "sword.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define SWORD_FRAME		(15)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct SWORD {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		inhitpos;	//位置ベクトル
	D3DXVECTOR2		player_pos;	//位置ベクトル
	D3DXVECTOR2		dir;	//位置ベクトル
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	int				Frame;
	bool			use;
};

static SWORD	g_Sword;

static int		g_TexNo = 0;
static int		g_SoundNo = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitSword(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/sword.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/p_sword.wav");
	SetVolume(g_SoundNo, 0.2f);

	//初期化
	g_Sword.pos.x = 0.0f;
	g_Sword.pos.y = 0.0f;
	g_Sword.inhitpos.x = 0.0f;
	g_Sword.inhitpos.y = 0.0f;
	g_Sword.player_pos.x = 0.0f;
	g_Sword.player_pos.y = 0.0f;
	g_Sword.dir.x = 0.0f;
	g_Sword.dir.y = 0.0f;
	g_Sword.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Sword.rot = 0.0f;
	g_Sword.Frame = 0;
	g_Sword.use = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSword(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSword(void)
{
	if (!g_Sword.use) {
		return;
	}
	g_Sword.player_pos = GetPlayerPos();

	g_Sword.pos.x = g_Sword.player_pos.x + cosf(g_Sword.rot) * 130.0f;
	g_Sword.pos.y = g_Sword.player_pos.y + sinf(g_Sword.rot) * 130.0f;

	g_Sword.inhitpos.x = g_Sword.player_pos.x + cosf(g_Sword.rot) * 70.0f;
	g_Sword.inhitpos.y = g_Sword.player_pos.y + sinf(g_Sword.rot) * 70.0f;

	if (g_Sword.dir.x == 1.0f) {
		g_Sword.rot += (D3DX_PI * 0.5f) / SWORD_FRAME;
	}
	if (g_Sword.dir.x == -1.0f) {
		g_Sword.rot -= (D3DX_PI * 0.5f) / SWORD_FRAME;
	}

	g_Sword.Frame++;
	if (g_Sword.Frame >= SWORD_FRAME) {
		g_Sword.use = false;
		g_Sword.Frame = 0;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSword(void)
{
	if (!g_Sword.use){
		return;
	}
	//バレットの描画
	DrawSpriteColorRotateBase(g_TexNo,
		g_Sword.pos.x, g_Sword.pos.y,
		SWORD_WIDTH, SWORD_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,
		g_Sword.color.r, g_Sword.color.g, g_Sword.color.b, g_Sword.color.a,
		g_Sword.rot + (D3DX_PI * 0.5f));
}

//剣を振る---------------------------------------------------------------------
void SetSword(float posx, float posy, float dirx, float diry)
{
	//バレットの配列を走査して働いていない物を見つける
	if (g_Sword.use) {
		return;
	}
	g_Sword.player_pos.x = posx;
	g_Sword.player_pos.y = posy;

	g_Sword.pos.x = posx;
	g_Sword.pos.y = posy;

	g_Sword.inhitpos.x = posx;
	g_Sword.inhitpos.y = posy;

	g_Sword.dir.x = dirx;
	g_Sword.dir.y = diry;
	if (g_Sword.dir.x == 1.0f) {
		g_Sword.rot = D3DX_PI * -0.4f;
	}
	if (g_Sword.dir.x == -1.0f) {
		g_Sword.rot = D3DX_PI * 1.4f;
	}
	g_Sword.use = true;

	PlaySound(g_SoundNo, 0);
}


//Swordの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetSwordPos(void)
{
	return g_Sword.pos;
}
//Sword内側の位置を渡す-------------------------------------------------------
D3DXVECTOR2 GetInhitPos(void)
{
	return g_Sword.inhitpos;
}

//Swordが表示されているかを渡す-----------------------------------------------
bool GetSwordUse(void)
{
	return g_Sword.use;
}