/*==============================================================================

	バレット管理 [bullet.cpp]
	Author :20106_田中蓮
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#include "bullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_WIDTH	(30.0f)
#define BULLET_HEIGHT	(30.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct BULLET {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		dir;	//移動方向
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	bool			use;
	float			U;		//U値の横幅
	float			V;		//V値の高さ
};

static BULLET g_Bullet[BULLET_MAX];

static int		g_ShotNo = 0;

static int g_TexNo = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet00.png");

	//サウンドの読み込み
	g_ShotNo = LoadSound((char*)"data/sound/p_shot.wav");
	SetVolume(g_ShotNo, 0.3f);

	//初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].pos.x = 0.0f;
		g_Bullet[i].pos.y = 0.0f;
		g_Bullet[i].dir.x = 0.0f;
		g_Bullet[i].dir.y = 0.0f;
		g_Bullet[i].speed = 30.0f;
		g_Bullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Bullet[i].rot = 0.0f;
		g_Bullet[i].use = false;
		g_Bullet[i].U = 0.0f;
		g_Bullet[i].V = 0.0f;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	StopSound(g_ShotNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_Bullet[i].use)
		{
			continue;
		}

		//座標更新
		g_Bullet[i].pos += g_Bullet[i].dir * g_Bullet[i].speed;
		
		//画面外に出たら消す
		if (!CheckDisp(g_Bullet[i].pos.x, g_Bullet[i].pos.y, BULLET_WIDTH, BULLET_HEIGHT) ||
			CheckHitBG(g_Bullet[i].pos.x, g_Bullet[i].pos.y, BULLET_WIDTH, BULLET_HEIGHT))
		{
			EraseBullet(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_Bullet[i].use)
		{
			continue;
		}
		//バレットの描画
		DrawSpriteColorRotateBase(g_TexNo,
			g_Bullet[i].pos.x, g_Bullet[i].pos.y,
			BULLET_WIDTH, BULLET_HEIGHT,
			g_Bullet[i].U, g_Bullet[i].V,
			1.0f, 1.0f,
			g_Bullet[i].color.r, g_Bullet[i].color.g, g_Bullet[i].color.b, g_Bullet[i].color.a,
			g_Bullet[i].rot);
	}
}

//弾を打つ---------------------------------------------------------------------
void SetBullet(float posx, float posy, float dirx, float diry)
{
	//バレットの配列を走査して働いていない物を見つける
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use)
		{
			continue;
		}
		g_Bullet[i].pos.x = posx;
		g_Bullet[i].pos.y = posy;
		g_Bullet[i].dir.x = dirx;
		g_Bullet[i].dir.y = diry;
		g_Bullet[i].rot = atan2f(g_Bullet[i].dir.y, g_Bullet[i].dir.x) + (D3DX_PI / 2);

		g_Bullet[i].use = true;

		//音再生
		PlaySound(g_ShotNo, 0);

		break;
	}
}

//Bulletの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetBulletPos(int index)
{
	return g_Bullet[index].pos;
}

//Bulletが表示されているかを渡す-----------------------------------------------
bool GetBulletUse(int index)
{
	return g_Bullet[index].use;
}

//Bulletを消す-----------------------------------------------------------------
void EraseBullet(int index)
{
	g_Bullet[index].use = false;
}