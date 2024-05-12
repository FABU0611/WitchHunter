/*==============================================================================

	エネミー管理 [boss.cpp]
	Author :20106_田中蓮
	Date   :23_09_08
--------------------------------------------------------------------------------

==============================================================================*/
#include "boss.h"
#include "bossbullet.h"
#include "bossexplosion.h"
#include "sprite.h"
#include "texture.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "bg.h"

#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SP			(5.0f)

#define BOSS_WIDTH		(120.0f)	//テクスチャの横幅
#define BOSS_HEIGHT		(120.0f)	//テクスチャの縦幅
#define BOSS_CHECK_BLOCK	(10.0f)

#define BOSS_LIFE			(30)
#define BOSS_SHOT_RATE		(20)
#define BOSS_SHOT_NUM		(2)

#define ANIME_SP		(50)		//アニメーションの速度 値が大きいと遅くなる
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
enum BOSS_STATE {
	ST_SHOT,
	ST_EXPLOSION,
	ST_WAIT_SHOT,
	ST_WAIT_EXPLOSION,
	ST_WEAK,
	ST_DEAD,

	ST_NUM,
};

void HitBlockforBoss(void);
void BossAnim(void);
void BossStShot(void);
void BossStWaitShot(void);
void BossStExplosion(void);
void BossStWaitExplosion(void);
void BossStWeak(void);
void SetDie(void);

void RemoveBossColor(void);
void SetWaitExColor(void);
void SetWeakColor(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
struct BOSS{
	D3DXVECTOR2		pos;	//位置ベクトル
	D3DXVECTOR2		GHitpos;	//着地判定ようの位置
	D3DXVECTOR2		dir;	//方向ベクトル
	float			speed;	//速度
	D3DXCOLOR		color;	//頂点カラー
	float			rot;
	float			U = 0.0f;		//U値の横幅
	float			V = 0.0f;		//V値の高さ

	float			shot_angle;

	int				Life;

	float			Fall;		//重力用
	bool			isGround;	//着地判定
	bool			isJump;		//ジャンプ中判定
	bool			isPressJump;	//ジャンプボタン押した判定
	bool			isHead;

	int				state;
	int				count_state;
	int				count_shot;
	int				count_color;
	int				shot_rate;

	bool			onScreen;	//画面内判定
	bool			isLive;		//見えるかフラグ
	bool			isFight;	//戦っている状態


	float			JumpPos;	//ジャンプ開始地点
	float			JumpHeight;	//最大ジャンプ地点

	int				AnimePattern;
	int				Anime;
	int				AnimeSkipFrame;

	DIRECTION		direction;	//向き
};

static BOSS g_Boss;

static int	g_TexNo = 0;
static int	g_SoundNo = 0;

static int	g_AnimeWalk[4] = { 0,1,2,1 };

int g_Dead_frame = 0;
static float	Gravity = 0.0f;		//重力
//=============================================================================
// 初期化処理
//=============================================================================
void InitBoss(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/boss.png");

	g_SoundNo = LoadSound((char*)"data/sound/b_dead.wav");
	SetVolume(g_SoundNo, 0.2f);

	//初期化
	g_Boss.pos.x = 0.0f;
	g_Boss.pos.y = 0.0f;
	g_Boss.GHitpos.x = g_Boss.pos.x;
	g_Boss.GHitpos.y = g_Boss.pos.y + (BOSS_HEIGHT * 0.5f);
	g_Boss.dir.x = 1.0f;
	g_Boss.dir.y = 0.0f;
	g_Boss.speed = NUM_SP;
	g_Boss.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Boss.rot = 0.0f;

	g_Boss.U = PTN_WIDTH * 1;
	g_Boss.V = PTN_HEIGHT * 0;

	g_Boss.shot_angle = 0.0f;

	g_Boss.Life = BOSS_LIFE;

	g_Boss.Fall = 0.0f;
	g_Boss.isGround = false;
	g_Boss.isJump = false;
	g_Boss.isPressJump = false;
	g_Boss.isHead = false;

	g_Boss.state = ST_SHOT;
	g_Boss.count_state = 0;
	g_Boss.count_shot = 0;
	g_Boss.count_color = 0;
	g_Boss.shot_rate = 0;

	g_Boss.onScreen = true;
	g_Boss.isLive = false;
	g_Boss.isFight = false;

	g_Boss.JumpPos = 0.0f;
	g_Boss.JumpHeight = BOSS_HEIGHT * 3.5f;

	g_Boss.AnimePattern = 0;
	g_Boss.Anime = 0;
	g_Boss.AnimeSkipFrame = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	Gravity = GRAVITY;

	//画面に近づいたらロード
	if (CheckLoadRight(g_Boss.pos.x, g_Boss.pos.y, BOSS_WIDTH, BOSS_HEIGHT)) {
		g_Boss.isLive = true;
	}

	//見えてるなら
	if (!g_Boss.isLive) {
		return;
	}

	//画面内判定
	if (!CheckDisp(g_Boss.pos.x, g_Boss.pos.y, BOSS_WIDTH, BOSS_HEIGHT)) {
		g_Boss.onScreen = false;
	}
	else {
		g_Boss.onScreen = true;
	}


	//地面の上にいるか
	g_Boss.GHitpos.x = g_Boss.pos.x;
	g_Boss.GHitpos.y = g_Boss.pos.y + (BOSS_HEIGHT * 0.5f);


	//画面内にいる間
	if (g_Boss.onScreen) {
		//アニメーション
		BossAnim();
	}

	//マップとの当たり判定
	HitBlockforBoss();



	//攻撃処理----------------------------------------------------------------------
	if (g_Boss.isFight) {
		switch (g_Boss.state)		{
		case ST_SHOT:
			BossStShot();
			break;
		case ST_WAIT_SHOT:
			BossStWaitShot();
			break;
		case ST_EXPLOSION:
			BossStExplosion();
			break;
		case ST_WAIT_EXPLOSION:
			BossStWaitExplosion();
			break;
		case ST_WEAK:
			BossStWeak();
			break;
		}
	}
	if (g_Boss.state == ST_DEAD) {
		DieBoss();
	}

	//着地しているときは落ちない
	if (g_Boss.isGround) {
		g_Boss.Fall = 0.0f;
		Gravity = 0.0f;
	}
	//空中にいるとき落ちていく
	else {
		g_Boss.pos.y -= g_Boss.Fall;
		g_Boss.Fall -= Gravity;
		//空中で立ちっぱなしの絵にしない
		if (g_Boss.direction == left) {
			g_Boss.U = PTN_WIDTH * 2;
		}
		if (g_Boss.direction == right) {
			g_Boss.U = PTN_WIDTH * 0;
		}
	}


}


//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	//エネミーの描画
		//倒されたか画面外なら表示しない
	if (!g_Boss.isLive || !g_Boss.onScreen)	{
		return;
	}
	DrawSpriteColorRotateBase(g_TexNo,
		g_Boss.pos.x, g_Boss.pos.y,
		BOSS_WIDTH, BOSS_HEIGHT,
		g_Boss.U, g_Boss.V,
		PTN_WIDTH, PTN_HEIGHT,
		g_Boss.color.r, g_Boss.color.g, g_Boss.color.b, g_Boss.color.a,
		g_Boss.rot);
}

//アニメーション-------------------------------------------------------------------------
void BossAnim(void) {
	g_Boss.AnimeSkipFrame++;					//フレームを数える
	if (g_Boss.AnimeSkipFrame >= ANIME_SP) {
		g_Boss.AnimeSkipFrame = 0;			//0に戻す

		g_Boss.AnimePattern++;				//20フレームに一回数える
		g_Boss.Anime = g_AnimeWalk[g_Boss.AnimePattern % 4];	//0,1,2,1を順番に見る
		if (g_Boss.AnimePattern > ANIME_PTN_YOKO) {
			g_Boss.AnimePattern = 0;			//0に戻す
		}
	}
	//UV値更新
	if (g_Boss.state == ST_WEAK) {
		g_Boss.U = PTN_WIDTH * 1;
	}
	else {
		g_Boss.U = PTN_WIDTH * g_Boss.Anime;
	}
	if (!g_Boss.isFight) {
		g_Boss.U = PTN_WIDTH * 1;
		g_Boss.V = PTN_HEIGHT * 0;
	}
	else {
		//左
		if (g_Boss.dir.x < 0) {
			g_Boss.V = PTN_HEIGHT * 1;
		}
		//右
		else if (g_Boss.dir.x > 0) {
			g_Boss.V = PTN_HEIGHT * 2;
		}
	}
}

//位置を入れて使っている状態にする
void SetBoss(float posx, float posy, float dirx, float diry)
{
	if (g_Boss.isLive) {
		return;
	}

	g_Boss.pos.x = posx;
	g_Boss.pos.y = posy;
	g_Boss.dir.x = dirx;
	g_Boss.dir.y = diry;
}

//Bossの位置を渡す
D3DXVECTOR2 GetBossPos(void){
	return g_Boss.pos;
}

//Bossが生きているかを渡す
bool GetBossLive(void){
	return g_Boss.isLive;
}

//Bossが戦っている状態かを渡す
bool GetFightState(void) {
	return g_Boss.isFight;
}
void SetFightState(bool fight) {
	g_Boss.isFight = fight;
}

//死亡処理
void DieBoss(void){
	if (g_Dead_frame == 0) {
		PlaySound(g_SoundNo, 0);
	}
	g_Dead_frame++;
	if (g_Dead_frame > 0) {
		//消えていく
		g_Boss.color.a -= 0.005f;
		//消えたらシーンを切り替える
		if (g_Boss.color.a <= 0.0f) {
			g_Boss.isLive = false;
			if (GetFadeState() == SCENE_NONE) {
				SetFadeColor(0.0f, 0.0f, 0.0f);
				//シーンの切り替えを行う
				//ゲームシーン遷移
				SceneTransition(SCENE_RESULT);
			}
		}
	}
}

void SetDie(void) {
	//ライフが０以下になったら死ぬ
	if (g_Boss.Life < 1) {
		g_Boss.state = ST_DEAD;
		g_Boss.isFight = false;
		EraseBossexplosion();
		for (int i = 0; i < BOSSBULLET_MAX; i++)		{
			EraseBossbullet(i);
		}
	}
}

//ブロックとの当たり判定
void HitBlockforBoss(void) {
	//横
	if (CheckHitBG(g_Boss.pos.x + g_Boss.dir.x * g_Boss.speed, g_Boss.pos.y,
		BOSS_CHECK_SIZE, BOSS_CHECK_SIZE)) {
		g_Boss.dir.x *= -1.0f;
	}
	//ブロックに着地しているか
	if (CheckHitBG(g_Boss.GHitpos.x, g_Boss.GHitpos.y - g_Boss.Fall,
		BOSS_CHECK_SIZE, BOSS_CHECK_BLOCK)) {
		g_Boss.isGround = true;
	}
	//ブロックに着地していないならfalse
	else {
		g_Boss.isGround = false;
	}
}

//ダメージ処理---------------------------------------------------------------------------
//プレイヤーの弾に当たったら
void HitBulletforBoss(void){
	//弱点状態なら二倍
	if (g_Boss.state == ST_WEAK) {
		g_Boss.Life -= (BULLET_POWER * 2);
	}
	else {
		g_Boss.Life -= BULLET_POWER;
	}
}
//プレイヤーのチャージ弾に当たったら
void HitChrageBulletforBoss(void){
	//弱点状態なら二倍
	if (g_Boss.state == ST_WEAK) {
		g_Boss.Life -= (CHARGEBULLET_POWER * 2);
	}
	else {
		g_Boss.Life -= CHARGEBULLET_POWER;
	}
}
//プレイヤーの剣に当たったら
void HitSwordforBoss(void) {
	//爆発準備状態なら弱点状態にする
	if (g_Boss.state == ST_WAIT_EXPLOSION) {
		g_Boss.state = ST_WEAK;
	}
}



//ボスのstate----------------------------------------------------------------------------
//射撃状態
void BossStShot(void) {
	g_Boss.shot_rate++;
	RemoveBossColor();
	//プレイヤー位置取得
	D3DXVECTOR2 player_pos = GetPlayerPos();
	//角度を出す
	g_Boss.shot_angle = atan2(g_Boss.pos.y - player_pos.y, g_Boss.pos.x - player_pos.x);
	//角度を入れる
	g_Boss.dir.x = -cos(g_Boss.shot_angle);
	g_Boss.dir.y = -sin(g_Boss.shot_angle);
	if (g_Boss.shot_rate > BOSS_SHOT_RATE) {
		SetBossbullet(g_Boss.pos.x, g_Boss.pos.y, g_Boss.dir.x, g_Boss.dir.y);
		g_Boss.shot_rate = 0;
		g_Boss.count_shot++;
	}
	//待機状態に遷移
	if (g_Boss.count_shot > BOSS_SHOT_NUM) {
		g_Boss.count_shot = 0;
		g_Boss.state = ST_WAIT_EXPLOSION;
	}
	SetDie();
}
//爆発状態
void BossStExplosion(void) {
	RemoveBossColor();
	SetBossexplosion(g_Boss.pos.x, g_Boss.pos.y);
	g_Boss.state = ST_WAIT_SHOT;
	SetDie();
}
//射撃準備状態
void BossStWaitShot(void) {
	g_Boss.count_state++;
	RemoveBossColor();
	if (g_Boss.count_state > 90) {
		g_Boss.count_state = 0;
		g_Boss.state = ST_SHOT;
	}
	SetDie();
}
//爆発準備状態
void BossStWaitExplosion(void) {
	g_Boss.count_state++;
	SetWaitExColor();
	//プレイヤー位置取得
	D3DXVECTOR2 player_pos = GetPlayerPos();
	//角度を出す
	g_Boss.shot_angle = atan2(g_Boss.pos.y - player_pos.y, g_Boss.pos.x - player_pos.x);
	//角度を入れる
	g_Boss.dir.x = -cos(g_Boss.shot_angle);
	//近づく
	if (!CheckHitBCSq(g_Boss.pos.x, g_Boss.pos.y, BOSS_CHECK_SIZE,
		player_pos.x, player_pos.y, PLAYER_RAD)) {
		g_Boss.pos.x += g_Boss.dir.x * g_Boss.speed;
	}
	if (CheckHitBCSq(g_Boss.pos.x, g_Boss.pos.y, BOSS_CHECK_SIZE * 2,
		player_pos.x, player_pos.y, PLAYER_RAD)) {
		g_Boss.speed = 0.0f;
	}
	else {
		g_Boss.speed = NUM_SP;
	}
	if (g_Boss.count_state > 200) {
		g_Boss.count_state = 0;
		g_Boss.state = ST_EXPLOSION;
	}
	SetDie();
}
//弱点状態
void BossStWeak(void) {
	g_Boss.count_state++;
	SetWeakColor();
	if (g_Boss.count_state > 200) {
		g_Boss.state = ST_WAIT_SHOT;
	}
	SetDie();
}

//bossの色-------------------------------------------------------------------------------
void RemoveBossColor(void) {
	g_Boss.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void SetWaitExColor(void) {
	g_Boss.count_color++;
	if (g_Boss.count_color == 10) {
		g_Boss.color = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
	}
	else if (g_Boss.count_color == 20) {
		RemoveBossColor();
		g_Boss.count_color = 0;
	}
}

void SetWeakColor(void) {
	g_Boss.color = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
}