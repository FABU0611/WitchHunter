/*==============================================================================

	エネミー管理 [hone.cpp]
	Author :20106_田中蓮
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "hone.h"
#include "sprite.h"
#include "texture.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "bg.h"
#include "honebullet.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SP				(4.0f)

#define HONE_CHECK_WIDTH	(100.0f)
#define HONE_CHECK_HEIGHT	(100.0f)
#define HONE_CHECK_BLOCK	(10.0f)

#define HONE_LIFE			(10)

#define ANIME_SP			(20)		//アニメーションの速度 値が大きいと遅くなる
#define ANIME_PTN_YOKO		(3)		//横のアニメパターン
#define ANIME_PTN_TATE		(4)		//縦のアニメパターン
#define ANIME_PTN			(ANIME_PTN_YOKO * ANIME_PTN_TATE)	//すべてのアニメパターン

#define PTN_WIDTH			(1.0f / ANIME_PTN_YOKO)	//一人分の横幅
#define PTN_HEIGHT			(1.0f / ANIME_PTN_TATE)	//一人分の縦幅

#define GRAVITY				(10.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetNohitColor(int index);
void SetNomalColor(int index);

void HoneStateShot(int idx);
void HoneStateShield(int idx);
void HoneStateWaitShot(int idx);
void HoneStateWaitShield(int idx);

enum HOHE_STATE {
	ST_SHOT,
	ST_SHIELD,
	ST_WAIT_SHOT,
	ST_WAIT_SHIELD,
	
	ST_NUM,
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct HONE{
	D3DXVECTOR2		pos;		//位置ベクトル
	D3DXVECTOR2		GHitpos;	//着地判定ようの位置
	D3DXVECTOR2		dir;		//方向ベクトル
	float			speed;		//速度
	D3DXCOLOR		color;		//頂点カラー
	float			rot;
	float			U = 0.0f;	//U値の横幅
	float			V = 0.0f;	//V値の高さ

	int				Life;		//ライフ

	float			Fall;		//重力用
	bool			isGround;	//着地判定
	bool			onScreen;	//画面内判定

	int				state;
	int				frame;
	bool			isNohit;	//無敵状態

	bool			isLive;		//生きてるかフラグ
	bool			isLoad;
};

static HONE g_Hone[HONE_MAX];

static int	g_TexNo = 0;
static int	g_SoundNo = 0;

static float	Gravity = 0.0f;		//重力
//=============================================================================
// 初期化処理
//=============================================================================
void InitHone(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/hone.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/e_guard.wav");
	SetVolume(g_SoundNo, 0.1f);

	//初期化
	for (int i = 0; i < HONE_MAX; i++)
	{
		g_Hone[i].pos.x = 0.0f;
		g_Hone[i].pos.y = 0.0f;
		g_Hone[i].GHitpos.x = g_Hone[i].pos.x;
		g_Hone[i].GHitpos.y = g_Hone[i].pos.y + (HONE_HEIGHT * 0.5f);
		g_Hone[i].dir.x = -1.0f;
		g_Hone[i].dir.y = 0.0f;
		g_Hone[i].speed = NUM_SP;
		g_Hone[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Hone[i].rot = 0.0f;

		g_Hone[i].U = PTN_WIDTH * 1;
		g_Hone[i].V = PTN_HEIGHT * 0;

		g_Hone[i].Life = HONE_LIFE;

		g_Hone[i].Fall = 0.0f;
		g_Hone[i].isGround = false;
		g_Hone[i].onScreen = false;

		g_Hone[i].state = ST_WAIT_SHOT;
		g_Hone[i].frame = 0;
		g_Hone[i].isNohit = false;
		
		g_Hone[i].isLive = false;
		g_Hone[i].isLoad = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHone(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHone(void)
{
	Gravity = GRAVITY;

	for (int i = 0; i < HONE_MAX; i++) {
		if (g_Hone[i].Life < 1) {
			DieHone(i);
		}

		//画面に近づいたらロード
		if (!g_Hone[i].isLive &&
			g_Hone[i].isLoad &&
			CheckLoadRight(g_Hone[i].pos.x, g_Hone[i].pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			g_Hone[i].isLive = true;
			break;
		}
	}

	for (int i = 0; i < HONE_MAX; i++) {
		//見えてるなら
		if (!g_Hone[i].isLive)		{
			continue;
		}

		//画面内判定
		if (!CheckDisp(g_Hone[i].pos.x, g_Hone[i].pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			g_Hone[i].onScreen = false;
		}
		else {
			g_Hone[i].onScreen = true;
		}


		//地面の上にいるか
		g_Hone[i].GHitpos.x = g_Hone[i].pos.x;
		g_Hone[i].GHitpos.y = g_Hone[i].pos.y + (HONE_HEIGHT * 0.5f);


		//画面内にいる間
		if (g_Hone[i].onScreen) {
			//UV値更新
			g_Hone[i].U = PTN_WIDTH * 0;

			//左
			if (g_Hone[i].dir.x < 0)			{
				g_Hone[i].V = PTN_HEIGHT * 1;
			}
			//右
			else if (g_Hone[i].dir.x > 0)			{
				g_Hone[i].V = PTN_HEIGHT * 2;
			}

			//状態遷移
			switch (g_Hone[i].state){
			case ST_SHOT:
				HoneStateShot(i);
				break;
			case ST_SHIELD:
				HoneStateShield(i);
				break;
			case ST_WAIT_SHOT:
				HoneStateWaitShot(i);
				break;
			case ST_WAIT_SHIELD:
				HoneStateWaitShield(i);
				break;
			}
		}

		//マップとの当たり判定-----------------------------------------------------------
		//横
		if (CheckHitBG(g_Hone[i].pos.x + g_Hone[i].dir.x * g_Hone[i].speed, g_Hone[i].pos.y,
			HONE_CHECK_WIDTH, HONE_CHECK_HEIGHT)) {
			g_Hone[i].dir.x *= -1.0f;
		}
		//ブロックに着地しているか
		if (CheckHitBG(g_Hone[i].GHitpos.x, g_Hone[i].GHitpos.y - g_Hone[i].Fall,
			HONE_CHECK_WIDTH, HONE_CHECK_BLOCK)) {
			g_Hone[i].isGround = true;
		}
		else {
			g_Hone[i].isGround = false;
		}



		//重力処理----------------------------------------------------------------------
		g_Hone[i].Fall -= Gravity;

		//着地しているときは落ちない
		if (g_Hone[i].isGround)		{
			g_Hone[i].Fall = 0.0f;
		}
		//空中にいるとき落ちていく
		else		{
			g_Hone[i].pos.y -= g_Hone[i].Fall;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHone(void)
{
	//エネミーの描画
	for (int i = 0; i < HONE_MAX; i++)	{
		//倒されたか画面外なら表示しない
		if (!g_Hone[i].isLive || !g_Hone[i].onScreen)		{
			continue;
		}
		DrawSpriteColorRotateBase(g_TexNo,
			g_Hone[i].pos.x, g_Hone[i].pos.y,
			HONE_WIDTH, HONE_HEIGHT,
			g_Hone[i].U, g_Hone[i].V,
			PTN_WIDTH, PTN_HEIGHT,
			g_Hone[i].color.r, g_Hone[i].color.g, g_Hone[i].color.b, g_Hone[i].color.a,
			g_Hone[i].rot);
	}
}

//セッター-------------------------------------------------------------------------------
//位置を入れて使っている状態にする
void SetHone(float posx, float posy, float dirx, float diry) 
{
	for (int i = 0; i < HONE_MAX; i++) {
		if (g_Hone[i].isLoad) {
			continue;
		}

		g_Hone[i].pos.x = posx;
		g_Hone[i].pos.y = posy;
		g_Hone[i].dir.x = dirx;
		g_Hone[i].dir.y = diry;

		g_Hone[i].isLoad = true;
		break;
	}
}
//無敵状態の色
void SetNohitColor(int index)
{
	g_Hone[index].color = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
	g_Hone[index].U = PTN_WIDTH * 1;
}
//通常状態の色
void SetNomalColor(int index)
{
	g_Hone[index].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//ゲッター-------------------------------------------------------------------------------
//ライフを渡す
int GetHoneLife(int index) {
	return g_Hone[index].Life;
}
//Honeの位置を渡す
D3DXVECTOR2 GetHonePos(int index)
{
	return g_Hone[index].pos;
}
//Honeが表示されているかを渡す
bool GetHoneLive(int index)
{
	return g_Hone[index].isLive;
}

//Honeを消す
void DieHone(int index)
{
	g_Hone[index].isLive = false;
}

//プレイヤーの弾に当たったら
void HitBulletforHone(int idx)
{
	//無敵状態なら何もしない
	if (g_Hone[idx].isNohit) {
		return;
	}
	g_Hone[idx].Life -= BULLET_POWER;
}

//プレイヤーのチャージ弾に当たったら
void HitChrageBulletforHone(int idx)
{
	//無敵状態なら何もしない
	if (g_Hone[idx].isNohit) {
		return;
	}
	g_Hone[idx].Life -= CHARGEBULLET_POWER;
}

//状態-----------------------------------------------------------------------------------
//射撃状態
void HoneStateShot(int idx) 
{
	g_Hone[idx].frame++;
	g_Hone[idx].U = PTN_WIDTH * 2;
	if (g_Hone[idx].frame == 40) {
		SetHonebullet(g_Hone[idx].pos.x - (HONE_WIDTH * 0.7f), g_Hone[idx].pos.y, g_Hone[idx].dir.x, g_Hone[idx].dir.y);
	}
	if (g_Hone[idx].frame == 60) {
		SetHonebullet(g_Hone[idx].pos.x - (HONE_WIDTH * 0.7f), g_Hone[idx].pos.y, g_Hone[idx].dir.x, g_Hone[idx].dir.y);
	}
	if (g_Hone[idx].frame == 80) {
		SetHonebullet(g_Hone[idx].pos.x - (HONE_WIDTH * 0.7f), g_Hone[idx].pos.y, g_Hone[idx].dir.x, g_Hone[idx].dir.y);

		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_WAIT_SHIELD;
	}
}

//無敵状態
void HoneStateShield(int idx)
{
	g_Hone[idx].frame++;
	g_Hone[idx].isNohit = true;
	SetNohitColor(idx);
	if (g_Hone[idx].frame > 80) {
		g_Hone[idx].isNohit = false;
		SetNomalColor(idx);
		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_WAIT_SHOT;
	}
}

//射撃状態待機
void HoneStateWaitShot(int idx)
{
	g_Hone[idx].frame++;
	if (g_Hone[idx].frame > 50) {
		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_SHOT;
	}
}

//無敵状態待機
void HoneStateWaitShield(int idx)
{
	g_Hone[idx].frame++;
	if (g_Hone[idx].frame > 30) {
		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_SHIELD;
		//無敵状態移行時の音再生
		PlaySound(g_SoundNo, 0);
	}
}