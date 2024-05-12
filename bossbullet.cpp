/*==============================================================================

	バレット管理 [bossbullet.cpp]
	Author :20106_田中蓮
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "bossbullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSSBULLET_WIDTH	(50.0f)
#define BOSSBULLET_HEIGHT	(50.0f)
#define BOSSBULLET_SPEED	(15.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct BOSSBULLET {
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		dir;	//移動方向
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	bool			use;
};

static BOSSBULLET g_Bossbullet[BOSSBULLET_MAX];

static int g_TexNo = 0;
static int g_SoundNo = 0;

static float g_U = 0.0f;		//U値の横幅
static float g_V = 0.0f;		//V値の高さ

//=============================================================================
// 初期化処理
//=============================================================================
void InitBossbullet(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet00.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/b_bullet.wav");
	SetVolume(g_SoundNo, 0.2f);

	//初期化
	for (int i = 0; i < BOSSBULLET_MAX; i++)	{
		g_Bossbullet[i].pos.x = 0.0f;
		g_Bossbullet[i].pos.y = 0.0f;
		g_Bossbullet[i].dir.x = 0.0f;
		g_Bossbullet[i].dir.y = 0.0f;
		g_Bossbullet[i].speed = BOSSBULLET_SPEED;
		g_Bossbullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Bossbullet[i].rot = 0.0f;
		g_Bossbullet[i].use = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossbullet(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossbullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++)
	{
		if (!g_Bossbullet[i].use)		{
			continue;
		}

		//座標更新
		g_Bossbullet[i].pos += g_Bossbullet[i].dir * g_Bossbullet[i].speed;
		
		//画面外に出たら消す
		if (!CheckDisp(g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y, BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT) ||
			CheckHitBG(g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y, BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT))		{
			EraseBossbullet(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossbullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++)
	{
		if (!g_Bossbullet[i].use)
		{
			continue;
		}
		//バレットの描画
		DrawSpriteColorRotateBase(g_TexNo,
			g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y,
			BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT,
			g_U, g_V,
			1.0f, 1.0f,
			g_Bossbullet[i].color.r, g_Bossbullet[i].color.g, g_Bossbullet[i].color.b, g_Bossbullet[i].color.a,
			g_Bossbullet[i].rot);
	}
}

//弾を打つ---------------------------------------------------------------------
void SetBossbullet(float posx, float posy, float dirx, float diry)
{
	//バレットの配列を走査して働いていない物を見つける
	for (int i = 0; i < BOSSBULLET_MAX; i++)	{
		if (g_Bossbullet[i].use)		{
			continue;
		}
		g_Bossbullet[i].pos.x = posx;
		g_Bossbullet[i].pos.y = posy;
		g_Bossbullet[i].dir.x = dirx;
		g_Bossbullet[i].dir.y = diry;
		g_Bossbullet[i].rot = atan2f(g_Bossbullet[i].dir.y, g_Bossbullet[i].dir.x) + (D3DX_PI / 2);

		g_Bossbullet[i].use = true;

		PlaySound(g_SoundNo, 0);
		break;
	}
}

//Bossbulletの位置を渡す-----------------------------------------------------------
D3DXVECTOR2 GetBossbulletPos(int index){
	return g_Bossbullet[index].pos;
}

//Bossbulletが表示されているかを渡す-----------------------------------------------
bool GetBossbulletUse(int index){
	return g_Bossbullet[index].use;
}

//Bossbulletを消す-----------------------------------------------------------------
void EraseBossbullet(int index){
	g_Bossbullet[index].use = false;
}