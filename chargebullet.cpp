/*==============================================================================

	チャージバレット管理 [chargebullet.cpp]
	Author :20106_田中蓮
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#include "chargebullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHARGEBULLET_WIDTH	(80.0f)
#define CHARGEBULLET_HEIGHT	(80.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct CHARGEBULLET {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		dir;	//移動方向
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	bool			use;
	float			U;		//U値の横幅
	float			V;		//V値の高さ
};

static CHARGEBULLET g_ChargeBullet[CHARGEBULLET_MAX];

static int g_TexNo = 0;

static int g_ChargeNo = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitChargeBullet(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/enemy00.png");

	//サウンドの読み込み
	g_ChargeNo = LoadSound((char*)"data/sound/p_charge.wav");
	SetVolume(g_ChargeNo, 0.3f);

	//初期化
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		g_ChargeBullet[i].pos.x = 0.0f;
		g_ChargeBullet[i].pos.y = 0.0f;
		g_ChargeBullet[i].dir.x = 0.0f;
		g_ChargeBullet[i].dir.y = 0.0f;
		g_ChargeBullet[i].speed = 30.0f;
		g_ChargeBullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_ChargeBullet[i].rot = 0.0f;
		g_ChargeBullet[i].use = false;
		g_ChargeBullet[i].U = 0.0f;
		g_ChargeBullet[i].V = 0.0f;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitChargeBullet(void)
{
	StopSound(g_ChargeNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateChargeBullet(void)
{
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (!g_ChargeBullet[i].use)
		{
			continue;
		}

		//座標更新
		g_ChargeBullet[i].pos += g_ChargeBullet[i].dir * g_ChargeBullet[i].speed * 1.2f;
		
		//画面外に出たら消す
		if (!CheckDisp(g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y, CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT) ||
			CheckHitBG(g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y, CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT))
		{
			EraseChargeBullet(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawChargeBullet(void)
{
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (!g_ChargeBullet[i].use)
		{
			continue;
		}
		//バレットの描画
		DrawSpriteColorRotateBase(g_TexNo,
			g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y,
			CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT,
			g_ChargeBullet[i].U, g_ChargeBullet[i].V,
			1.0f, 1.0f,
			g_ChargeBullet[i].color.r, g_ChargeBullet[i].color.g, g_ChargeBullet[i].color.b, g_ChargeBullet[i].color.a,
			g_ChargeBullet[i].rot);
	}
}

//弾を打つ---------------------------------------------------------------------
void SetChargeBullet(float posx, float posy, float dirx, float diry)
{
	//バレットの配列を走査して働いていない物を見つける
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (g_ChargeBullet[i].use)
		{
			continue;
		}
		g_ChargeBullet[i].pos.x = posx;
		g_ChargeBullet[i].pos.y = posy;
		g_ChargeBullet[i].dir.x = dirx;
		g_ChargeBullet[i].dir.y = diry;
		g_ChargeBullet[i].rot = atan2f(g_ChargeBullet[i].dir.y, g_ChargeBullet[i].dir.x) + (D3DX_PI / 2);

		g_ChargeBullet[i].use = true;

		//音再生
		PlaySound(g_ChargeNo, 0);

		break;
	}
}

//ChargeBulletの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetChargeBulletPos(int index)
{
	return g_ChargeBullet[index].pos;
}

//ChargeBulletが表示されているかを渡す-----------------------------------------------
bool GetChargeBulletUse(int index)
{
	return g_ChargeBullet[index].use;
}

//ChargeBulletを消す-----------------------------------------------------------------
void EraseChargeBullet(int index)
{
	g_ChargeBullet[index].use = false;
}