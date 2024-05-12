/*==============================================================================

	エネミー管理 [enemy.cpp]
	Author :20106_田中蓮
	Date   :23_06_09
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemy.h"
#include "sprite.h"
#include "texture.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SP			(4.0f)

#define ENEMY_CHECK_WIDTH	(90.0f)
#define ENEMY_CHECK_HEIGHT	(90.0f)
#define ENEMY_CHECK_BLOCK	(10.0f)

#define ENEMY_LIFE			(5)

#define ANIME_SP		(20)		//アニメーションの速度 値が大きいと遅くなる
#define ANIME_PTN_YOKO	(3)		//横のアニメパターン
#define ANIME_PTN_TATE	(4)		//縦のアニメパターン
#define ANIME_PTN		(ANIME_PTN_YOKO * ANIME_PTN_TATE)	//すべてのアニメパターン

#define PTN_WIDTH		(1.0f / ANIME_PTN_YOKO)	//一人分の横幅
#define PTN_HEIGHT		(1.0f / ANIME_PTN_TATE)	//一人分の縦幅

#define GRAVITY	(10.0f)

#define SCR_SP		(5.0f)	//スクロール速度
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct ENEMY{
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		GHitpos;	//着地判定ようの位置
	D3DXVECTOR2		dir;	//方向ベクトル
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	float			U = 0.0f;		//U値の横幅
	float			V = 0.0f;		//V値の高さ

	int				Life;

	float			Fall;		//重力用
	bool			isGround;	//着地判定
	bool			onScreen;	//画面内判定

	bool			isLive;	//見えるかフラグ
	bool			isLoad;

	int				AnimePattern;
	int				Anime;
	int				AnimeSkipFrame;
};

static ENEMY g_Enemy[ENEMY_MAX];

static int	g_TexNo = 0;
static int	g_AnimeWalk[4] = { 0,1,2,1 };

static float	Gravity = 0.0f;		//重力
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/hatghost_11.png");

	//初期化
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].pos.x = 0.0f;
		g_Enemy[i].pos.y = 0.0f;
		g_Enemy[i].GHitpos.x = g_Enemy[i].pos.x;
		g_Enemy[i].GHitpos.y = g_Enemy[i].pos.y + (ENEMY_HEIGHT * 0.5f);
		g_Enemy[i].dir.x = 1.0f;
		g_Enemy[i].dir.y = 0.0f;
		g_Enemy[i].speed = NUM_SP;
		g_Enemy[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Enemy[i].rot = 0.0f;

		g_Enemy[i].U = PTN_WIDTH * 1;
		g_Enemy[i].V = PTN_HEIGHT * 0;

		g_Enemy[i].Life = ENEMY_LIFE;

		g_Enemy[i].Fall = 0.0f;
		g_Enemy[i].isGround = false;
		g_Enemy[i].onScreen = false;
		g_Enemy[i].isLive = false;
		g_Enemy[i].isLoad = false;

		g_Enemy[i].AnimePattern = 0;
		g_Enemy[i].Anime = 0;
		g_Enemy[i].AnimeSkipFrame = 0;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	Gravity = GRAVITY;

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_Enemy[i].Life < 1) {
			DieEnemy(i);
		}

		if (!g_Enemy[i].isLive &&
			g_Enemy[i].isLoad &&
			CheckLoadRight(g_Enemy[i].pos.x, g_Enemy[i].pos.y, ENEMY_WIDTH, ENEMY_HEIGHT)) {
			g_Enemy[i].isLive = true;
			break;
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++) {
		//見えてるなら
		if (!g_Enemy[i].isLive)		{
			continue;
		}

		//画面内判定
		g_Enemy[i].onScreen = CheckDisp(g_Enemy[i].pos.x, g_Enemy[i].pos.y, ENEMY_WIDTH, ENEMY_HEIGHT);


		//地面の上にいるか
		g_Enemy[i].GHitpos.x = g_Enemy[i].pos.x;
		g_Enemy[i].GHitpos.y = g_Enemy[i].pos.y + (ENEMY_HEIGHT * 0.5f);


		//画面内にいる間
		if (g_Enemy[i].onScreen) {
			//アニメーション-------------------------------------------------------------------
			g_Enemy[i].AnimeSkipFrame++;					//フレームを数える
			if (g_Enemy[i].AnimeSkipFrame >= ANIME_SP)
			{
				g_Enemy[i].AnimeSkipFrame = 0;			//0に戻す

				g_Enemy[i].AnimePattern++;				//20フレームに一回数える
				g_Enemy[i].Anime = g_AnimeWalk[g_Enemy[i].AnimePattern % 4];	//0,1,2,1を順番に見る
				if (g_Enemy[i].AnimePattern > ANIME_PTN_YOKO)
				{
					g_Enemy[i].AnimePattern = 0;			//0に戻す
				}
			}
			//UV値更新
			g_Enemy[i].U = PTN_WIDTH * g_Enemy[i].Anime;

			//左
			if (g_Enemy[i].dir.x < 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 1;
			}
			//右
			else if (g_Enemy[i].dir.x > 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 2;
			}
			//上
			if (g_Enemy[i].dir.y < 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 3;
			}
			//下
			else if (g_Enemy[i].dir.y > 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 0;
			}
		}

		//マップとの当たり判定
		//横
		if (CheckHitBG(g_Enemy[i].pos.x + g_Enemy[i].dir.x * g_Enemy[i].speed, g_Enemy[i].pos.y,
			ENEMY_CHECK_WIDTH, ENEMY_CHECK_HEIGHT)) {
			g_Enemy[i].dir.x *= -1.0f;
		}
		//ブロックに着地しているか
		if (CheckHitBG(g_Enemy[i].GHitpos.x, g_Enemy[i].GHitpos.y - g_Enemy[i].Fall,
			ENEMY_CHECK_WIDTH, ENEMY_CHECK_BLOCK)) {
			g_Enemy[i].isGround = true;
		}
		else {
			g_Enemy[i].isGround = false;
		}


		//座標更新
		g_Enemy[i].pos += g_Enemy[i].dir * g_Enemy[i].speed;


		//重力処理----------------------------------------------------------------------
		g_Enemy[i].Fall -= Gravity;

		//着地しているときは落ちない
		if (g_Enemy[i].isGround)
		{
			g_Enemy[i].Fall = 0.0f;
		}
		//空中にいるとき落ちていく
		else
		{
			g_Enemy[i].pos.y -= g_Enemy[i].Fall;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	//エネミーの描画
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//倒されたか画面外なら表示しない
		if (!g_Enemy[i].isLive || !g_Enemy[i].onScreen)		{
			continue;
		}
		DrawSpriteColorRotateBase(g_TexNo,
			g_Enemy[i].pos.x, g_Enemy[i].pos.y,
			ENEMY_WIDTH, ENEMY_HEIGHT,
			g_Enemy[i].U, g_Enemy[i].V,
			PTN_WIDTH, PTN_HEIGHT,
			g_Enemy[i].color.r, g_Enemy[i].color.g, g_Enemy[i].color.b, g_Enemy[i].color.a,
			g_Enemy[i].rot);
	}
}

//位置を入れて使っている状態にする
void SetEnemy(float posx, float posy, float dirx, float diry) 
{
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (g_Enemy[i].isLoad) {
			continue;
		}

		g_Enemy[i].pos.x = posx;
		g_Enemy[i].pos.y = posy;
		g_Enemy[i].dir.x = dirx;
		g_Enemy[i].dir.y = diry;

		g_Enemy[i].isLoad = true;
		break;
	}
}

//Enemyの位置を渡す
D3DXVECTOR2 GetEnemyPos(int index)
{
	return g_Enemy[index].pos;
}

//Enemyが表示されているかを渡す
bool GetEnemyState(int index)
{
	return g_Enemy[index].isLive;
}

//Enemyを消す
void DieEnemy(int index)
{
	g_Enemy[index].isLive = false;
}

//プレイヤーの弾に当たったら
void HitBulletforEnemy(int idx)
{
	if (!g_Enemy[idx].isLive) {
		return;
	}
	g_Enemy[idx].Life -= BULLET_POWER;
}

//プレイヤーのチャージ弾に当たったら
void HitChrageBulletforEnemy(int idx)
{
	if (!g_Enemy[idx].isLive) {
		return;
	}
	g_Enemy[idx].Life -= CHARGEBULLET_POWER;
}

int GetEnemyLife(int index) {
	return g_Enemy[index].Life;
}