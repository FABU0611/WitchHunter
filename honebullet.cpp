/*==============================================================================

	バレット管理 [honebullet.cpp]
	Author :20106_田中蓮
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "honebullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HONEBULLET_HEIGHT	(30.0f)
#define HONEBULLET_SPEED	(15.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct HONEBULLET {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		dir;	//移動方向
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	bool			use;
};

static HONEBULLET g_Honebullet[HONEBULLET_MAX];

static int g_TexNo = 0;
static int g_SoundNo = 0;

static float g_U = 0.0f;		//U値の横幅
static float g_V = 0.0f;		//V値の高さ

//=============================================================================
// 初期化処理
//=============================================================================
void InitHonebullet(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet01.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/e_bullet.wav");
	SetVolume(g_SoundNo, 0.1f);

	//初期化
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		g_Honebullet[i].pos.x = 0.0f;
		g_Honebullet[i].pos.y = 0.0f;
		g_Honebullet[i].dir.x = 0.0f;
		g_Honebullet[i].dir.y = 0.0f;
		g_Honebullet[i].speed = HONEBULLET_SPEED;
		g_Honebullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Honebullet[i].rot = 0.0f;
		g_Honebullet[i].use = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHonebullet(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHonebullet(void)
{
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (!g_Honebullet[i].use)
		{
			continue;
		}

		//座標更新
		g_Honebullet[i].pos += g_Honebullet[i].dir * g_Honebullet[i].speed;
		g_Honebullet[i].rot -= (D3DX_PI * 0.05f);
		
		//画面外に出たら消す
		if (!CheckDisp(g_Honebullet[i].pos.x, g_Honebullet[i].pos.y, HONEBULLET_WIDTH, HONEBULLET_HEIGHT) ||
			CheckHitBG(g_Honebullet[i].pos.x, g_Honebullet[i].pos.y, HONEBULLET_WIDTH, HONEBULLET_HEIGHT))
		{
			EraseHonebullet(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHonebullet(void)
{
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (!g_Honebullet[i].use)
		{
			continue;
		}
		//バレットの描画
		DrawSpriteColorRotateBase(g_TexNo,
			g_Honebullet[i].pos.x, g_Honebullet[i].pos.y,
			HONEBULLET_WIDTH, HONEBULLET_HEIGHT,
			g_U, g_V,
			1.0f, 1.0f,
			g_Honebullet[i].color.r, g_Honebullet[i].color.g, g_Honebullet[i].color.b, g_Honebullet[i].color.a,
			g_Honebullet[i].rot);
	}
}

//弾を打つ---------------------------------------------------------------------
void SetHonebullet(float posx, float posy, float dirx, float diry)
{
	//バレットの配列を走査して働いていない物を見つける
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (g_Honebullet[i].use)
		{
			continue;
		}
		g_Honebullet[i].pos.x = posx;
		g_Honebullet[i].pos.y = posy;
		g_Honebullet[i].dir.x = dirx;
		g_Honebullet[i].dir.y = diry;
		g_Honebullet[i].rot = atan2f(g_Honebullet[i].dir.y, g_Honebullet[i].dir.x) + (D3DX_PI / 2);

		g_Honebullet[i].use = true;

		PlaySound(g_SoundNo, 0);
		break;
	}
}

//Honebulletの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetHonebulletPos(int index)
{
	return g_Honebullet[index].pos;
}

//Honebulletが表示されているかを渡す-----------------------------------------------
bool GetHonebulletUse(int index)
{
	return g_Honebullet[index].use;
}

//Honebulletを消す-----------------------------------------------------------------
void EraseHonebullet(int index)
{
	g_Honebullet[index].use = false;
}