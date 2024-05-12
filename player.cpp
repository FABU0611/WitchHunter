/*==============================================================================

	プレイヤー管理 [player.cpp]
	Author :20106_田中蓮
	Date   :23_06_09
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "boss.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "bullet.h"
#include "chargebullet.h"
#include "sword.h"
#include "camera.h"
#include "bg.h"
#include "life.h"
#include "collision.h"

#include "scene.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SP		(0.7f)
#define PLAYER_WIDTH	(90.0f)	//テクスチャの横幅
#define PLAYER_HEIGHT	(90.0f)	//テクスチャの縦幅
#define PLAYER_CHECK_BLOCK	(20.0f)	//テクスチャの縦幅

#define PLAYER_X_MAX	(MAP_X_MAX + (MAP_SCREAN * SIZE) - (PLAYER_WIDTH * 0.5f))
#define PLAYER_Y_MAX	(0.0f + (PLAYER_WIDTH * 0.5f))

#define RIGHT_MAX		(1000.0f)	//行動できる右端
#define LEFT_MAX		(400.0f)	//行動できる左端

#define ANIME_SP	(10)			//アニメーションの速度 値が大きいと遅くなる
#define ANIME_PTN_YOKO (3)			//横のアニメパターン
#define ANIME_PTN_TATE (4)			//縦のアニメパターン
#define ANIME_PTN (ANIME_PTN_YOKO * ANIME_PTN_TATE)	//すべてのアニメパターン

#define PTN_WIDTH	(1.0f / ANIME_PTN_YOKO)	//一人分の横幅
#define PTN_HEIGHT	(1.0f / ANIME_PTN_TATE)	//一人分の縦幅

#define JUMP_P (25.0f)
#define GRAVITY	(5.0f)
#define SLOW_DOWN	(-1.0f)

#define CHARGE_TIME		(50)

#define PLAYER_CAMERABASE_X	(SCREEN_WIDTH * 0.3f)
#define PLAYER_CAMERABASE_Y	(SCREEN_HEIGHT * 0.5f)

#define NOHITFRAME	(90)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetChargeColor(void);
void RemoveColor(void);
void ErasePlayer(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//サウンド用
enum SOUND_NUM {
	maxcharge,
	jump,
	hit,
	dead,

	sound_max,
};
enum BGM_NUM {
	fight,
	boss,

	bgm_max,
};

struct PLAYER{
	D3DXVECTOR2		pos;		//位置ベクトル
	D3DXVECTOR2		GHitpos;	//着地判定ようの位置
	D3DXVECTOR2		HHitpos;	//頭当たっている判定ようの位置
	D3DXVECTOR2		dir;		//速度ベクトル
	float			speed;
	D3DXCOLOR		color;		//頂点カラー
	float			rot;

	float			U = 0.0f;		//U値の横幅
	float			V = 0.0f;		//V値の高さ

	bool			isUse;
	bool			isDead;
	int				NoHitFrame;
	bool			isHit;
	int				FrashFrame;

	bool			isCharge;		//チャージ状態か
	bool			ChargeSound;

	float			JumpPos;		//ジャンプ開始地点
	float			JumpHeight;		//最大ジャンプ地点
	float			Fall;			//ジャンプ用
	bool			isGround;		//着地判定
	bool			isJump;			//ジャンプ中判定
	bool			isPressJump;	//ジャンプボタン押した判定
	bool			isHead;			//頭ぶつけた判定
	bool			isSlowDown;
	int				SlowCount;

	bool			FightBoss;


	DIRECTION		direction;	//向き

	int				Life;
};

static PLAYER g_Player;

static int		g_TexNo = 0;

static int		g_SoundNo[sound_max];		//サウンド
static int		g_BgmNo[bgm_max];		//サウンド

static float	Gravity = 0.0f;		//重力

static int g_AnimeSkipFrame = 0;	//フレーム数える用
static int g_AnimePattern = 0;		//数フレームに一回数える
static int g_Anime = 0;				//アニメーション用
static int g_AnimeWalk[4] = { 0,1,2,1 };	//綺麗に歩けるように

static int g_ChargeFrame = 0;

static int				g_Dead_frame;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/majo.png");

	//サウンドの読み込み
	g_SoundNo[maxcharge] = LoadSound((char*)"data/sound/p_chargemax.wav");
	SetVolume(g_SoundNo[maxcharge], 0.2f);
	g_SoundNo[jump] = LoadSound((char*)"data/sound/p_jump.wav");
	SetVolume(g_SoundNo[jump], 0.2f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/p_hit.wav");
	SetVolume(g_SoundNo[hit], 0.5f);
	g_SoundNo[dead] = LoadSound((char*)"data/sound/p_dead.wav");
	SetVolume(g_SoundNo[dead], 0.2f);

	g_BgmNo[fight] = LoadSound((char*)"data/sound/bgm_fight.wav");
	SetVolume(g_BgmNo[fight], 0.2f);
	PlaySound(g_BgmNo[fight], 1);
	g_BgmNo[boss] = LoadSound((char*)"data/sound/bgm_boss.wav");
	SetVolume(g_BgmNo[boss], 0.2f);

	//初期化
	g_Player.pos.x = LEFT_MAX + PLAYER_WIDTH;
	g_Player.pos.y = SCREEN_HEIGHT / 2;
	g_Player.GHitpos.x = g_Player.pos.x;
	g_Player.GHitpos.y = g_Player.pos.y + (PLAYER_HEIGHT * 0.5f);
	g_Player.HHitpos.x = g_Player.pos.x;
	g_Player.HHitpos.y = g_Player.pos.y - (PLAYER_HEIGHT);
	g_Player.dir.x = 0.0f;
	g_Player.dir.y = 0.0f;
	g_Player.speed = NUM_SP;
	g_Player.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Player.rot = 0.0f;

	g_Player.isUse = true;
	g_Player.isDead = false;
	g_Player.NoHitFrame = 0;
	g_Player.isHit = false;
	g_Player.FrashFrame = 0;

	g_Player.isCharge = false;
	g_Player.ChargeSound = false;

	g_Player.Fall = 0.0f;
	g_Player.isGround = false;
	g_Player.isJump = false;
	g_Player.isPressJump = false;
	g_Player.isHead = false;
	g_Player.isSlowDown = false;
	g_Player.SlowCount = 0;

	g_Player.JumpPos = 0.0f;
	g_Player.JumpHeight = PLAYER_HEIGHT * 3;

	g_Player.direction = right;

	g_Player.U = PTN_WIDTH * 1;
	g_Player.V = PTN_HEIGHT * 0;

	g_Player.FightBoss = false;

	g_Player.Life = PLAYER_LIFE;

	Gravity = 0.0f;

	g_AnimeSkipFrame = 0;
	g_AnimePattern = 0;
	g_Anime = 0;

	g_ChargeFrame = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < sound_max; i++) {
		StopSound(g_SoundNo[i]);
	}
	for (int i = 0; i < bgm_max; i++) {
		StopSound(g_BgmNo[i]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (!g_Player.isUse) {
		return;
	}
	//生きている間だけ処理
	if (!g_Player.isDead) {
		//当たってないなら当たり判定処理
		if (!g_Player.isHit) {
			ToPlayer();
		}

		//地面の上にいるか
		g_Player.GHitpos.x = g_Player.pos.x;
		g_Player.GHitpos.y = g_Player.pos.y + (PLAYER_HEIGHT * 0.5f);
		//頭が当たっているか
		g_Player.HHitpos.x = g_Player.pos.x;
		g_Player.HHitpos.y = g_Player.pos.y - (PLAYER_HEIGHT);

		Gravity = GRAVITY;

		//アニメーション---------------------------------------------------------------------
		g_AnimeSkipFrame++;					//フレームを数える
		if (g_AnimeSkipFrame >= ANIME_SP) {
			g_AnimeSkipFrame = 0;			//0に戻す

			g_AnimePattern++;				//20フレームに一回数える
			g_Anime = g_AnimeWalk[g_AnimePattern % 4];
			if (g_AnimePattern > ANIME_PTN_YOKO) {
				g_AnimePattern = 0;
			}
		}


		//プレイヤー移動（キーボード、パッド）----------------------------------------------------------
		if (GetKeyboardPress(DIK_A) ||
			IsButtonPressed(0, BUTTON_LEFT)) {	//左	
			g_Player.direction = left;
			g_Player.speed = NUM_SP * 10;
			g_Player.U = PTN_WIDTH * g_Anime;
		}
		else if (GetKeyboardPress(DIK_D) ||
			IsButtonPressed(0, BUTTON_RIGHT)) {	//右
			g_Player.direction = right;
			g_Player.speed = NUM_SP * 10;
			g_Player.U = PTN_WIDTH * g_Anime;
		}
		else {									//移動キーを押していないとき立ち止まっている絵
			g_Player.U = PTN_WIDTH * 1;
			g_Player.speed = 0.0f;
		}


		//向いている方向で絵を変える
		if (g_Player.direction == left) {
			g_Player.V = PTN_HEIGHT * 1;
			g_Player.dir.x = -1.0f;
			g_Player.dir.y = 0.0f;
		}
		if (g_Player.direction == right) {
			g_Player.V = PTN_HEIGHT * 2;
			g_Player.dir.x = 1.0f;
			g_Player.dir.y = 0.0f;
		}


		//弾を打つ------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_J) ||
			IsButtonPressed(0, BUTTON_Y)) {
			//チャージ開始
			g_ChargeFrame++;
			if (g_ChargeFrame >= CHARGE_TIME) {
				g_Player.isCharge = true;
			}

			//チャージしてないなら一発だけ撃つ
			if (GetKeyboardTrigger(DIK_J) ||
				IsButtonTriggered(0, BUTTON_Y)) {
				//上を向いている
				if (g_Player.direction == up) {
					SetBullet(g_Player.pos.x, g_Player.pos.y - (PLAYER_HEIGHT * 0.5f), g_Player.dir.x, g_Player.dir.y);
				}
				//下を向いている
				if (g_Player.direction == down) {
					SetBullet(g_Player.pos.x, g_Player.pos.y + (PLAYER_HEIGHT * 0.5f), g_Player.dir.x, g_Player.dir.y);
				}
				//左を向いている
				if (g_Player.direction == left) {
					SetBullet(g_Player.pos.x - (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
				}
				//右を向いている
				if (g_Player.direction == right) {
					SetBullet(g_Player.pos.x + (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
				}
				//撃ったら0に戻す
				g_ChargeFrame = 0;
			}
		}
		//チャージショット--------------------------------------------------------------------
		if (GetKeyboardRelease(DIK_J) &&
			g_Player.isCharge ||
			IsButtonPressed(0, BUTTON_Y) &&
			g_Player.isCharge) {
			//左を向いている
			if (g_Player.direction == left) {
				SetChargeBullet(g_Player.pos.x - (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//右を向いている
			if (g_Player.direction == right) {
				SetChargeBullet(g_Player.pos.x + (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//撃ったら0に戻す
			g_ChargeFrame = 0;
			g_Player.isCharge = false;
		}

		//チャージ完了時の色変更処理
		if (g_Player.isCharge) {
			SetChargeColor();
			if (!g_Player.ChargeSound) {
				//音再生
				PlaySound(g_SoundNo[maxcharge], 0);
				g_Player.ChargeSound = true;
			}
		}
		else {
			RemoveColor();
			g_Player.ChargeSound = false;
		}


		//剣を振る------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, BUTTON_X)) {
			//左を向いている
			if (g_Player.direction == left) {
				//剣を振ったとき少しダッシュしたい
				SetSword(g_Player.pos.x, g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//右を向いている
			if (g_Player.direction == right) {
				SetSword(g_Player.pos.x, g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
		}


		//プレイヤーの行動範囲------------------------------------------------------------
		//下	画面外に行かないように
		if (g_Player.pos.y > SCREEN_HEIGHT - (PLAYER_WIDTH / 2)) {
			g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_WIDTH / 2);
			g_Player.isGround = true;
		}
		//上
		else if (g_Player.pos.y < (PLAYER_WIDTH / 2)) {
			g_Player.pos.y = (PLAYER_WIDTH / 2);
		}
		//左
		if (g_Player.pos.x + g_Player.dir.x * g_Player.speed < PLAYER_Y_MAX) {
			g_Player.dir.x = 0.0f;
		}
		//端にいないなら戻す
		else if (g_Player.pos.x + g_Player.dir.x * g_Player.speed > PLAYER_X_MAX) {
			g_Player.dir.x = 0.0f;
		}

		//マップとの当たり判定
		//横
		if (CheckHitBG(g_Player.pos.x + g_Player.dir.x * g_Player.speed, g_Player.pos.y,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_SIZE)) {
			g_Player.dir.x = 0.0f;
		}
		//ブロックに着地しているか
		if (CheckHitBG(g_Player.GHitpos.x, g_Player.GHitpos.y - g_Player.Fall,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_BLOCK)) {
			g_Player.isGround = true;
		}
		else {
			g_Player.isGround = false;
		}
		//ブロックと頭が当たっているか判定
		if (CheckHitBG(g_Player.HHitpos.x, g_Player.HHitpos.y - g_Player.Fall,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_BLOCK)) {
			g_Player.isHead = true;
		}
		else {
			g_Player.isHead = false;
		}

		//座標更新
		g_Player.pos.x += g_Player.dir.x * g_Player.speed;


		//ジャンプ処理----------------------------------------------------------------------
		if (g_Player.isGround) {
			if (GetKeyboardTrigger(DIK_K) ||
				IsButtonTriggered(0, BUTTON_A)) {
				//ジャンプ開始地点を取得
				g_Player.JumpPos = g_Player.pos.y;
				//ジャンプキーが押されたらジャンプ中にする
				g_Player.isJump = true;
				//長押しでも一回しかジャンプしないように
				g_Player.isPressJump = true;
				//サウンド再生
				PlaySound(g_SoundNo[jump], 0);
			}
			else {
				//離したらジャンプ中ではない
				g_Player.isJump = false;
			}
		}
		if (g_Player.isJump &&
			g_Player.isPressJump) {
			//ジャンプ最高到達点より下にいる間上昇する
			if (GetKeyboardPress(DIK_K) &&
				(g_Player.JumpPos - g_Player.JumpHeight) < g_Player.pos.y &&
				!g_Player.isHead ||
				IsButtonPressed(0, BUTTON_A) &&
				(g_Player.JumpPos - g_Player.JumpHeight) < g_Player.pos.y &&
				!g_Player.isHead) {
				//ジャンプ中にキーが押されたら処理開始
				g_Player.Fall = JUMP_P;
				//空中に行くので着地はfalse
				g_Player.isGround = false;
				g_Player.SlowCount = 0;
			}
			else {
				//キーが離されたらジャンプ中ではない
				g_Player.isJump = false;
			}
		}
		else {
			//ジャンプ中にジャンプボタンでゆっくり落ちる
			if (GetKeyboardTrigger(DIK_K) ||
				IsButtonTriggered(0, BUTTON_A)) {
				g_Player.isSlowDown = true;
			}
			if (GetKeyboardPress(DIK_K) &&
				g_Player.isSlowDown ||
				IsButtonPressed(0, BUTTON_A) &&
				g_Player.isSlowDown) {
				g_Player.SlowCount++;
				g_Player.Fall = SLOW_DOWN;
				if (g_Player.SlowCount > 30) {
					g_Player.isSlowDown = false;
					g_Player.SlowCount = 0;
				}
			}
			//ジャンプ中でないなら落ちる
			g_Player.Fall -= Gravity;
		}

		//着地しているときは落ちない
		if (g_Player.isGround) {
			g_Player.Fall = 0.0f;
			Gravity = 0.0f;
			g_Player.isPressJump = false;
		}
		//空中にいるとき落ちていく
		else {
			g_Player.pos.y -= g_Player.Fall;
			//空中で立ちっぱなしの絵にしない
			if (g_Player.direction == left) {
				g_Player.U = PTN_WIDTH * 2;
			}
			if (g_Player.direction == right) {
				g_Player.U = PTN_WIDTH * 0;
			}
		}


		//カメラの座標更新
		D3DXVECTOR2 cameraPos;
		//画面左端
		cameraPos.x = g_Player.pos.x - PLAYER_CAMERABASE_X;
		if (cameraPos.x < 0.0f) {
			cameraPos.x = 0.0f;
		}
		//ボス部屋でカメラ固定
		if (g_Player.FightBoss) {
			cameraPos.x = MAP_X_MAX;
		}
		cameraPos.y = g_Player.pos.y - PLAYER_CAMERABASE_Y;
		//画面右端
		if (cameraPos.x > MAP_X_MAX &&
			!g_Player.FightBoss) {
			g_Player.FightBoss = true;
			//ボスを戦う状態にする
			SetFightState(true);
			StopSound(g_BgmNo[fight]);
			PlaySound(g_BgmNo[boss], 1);
		}
		//上
		if (cameraPos.y < 60.0f) {
			cameraPos.y = 60.0f;
		}
		//下
		if (cameraPos.y > 60.0f) {
			cameraPos.y = 60.0f;
		}
		SetCameraPos(cameraPos);
	}


	//無敵時間---------------------------------------------------------------------------
	if (g_Player.isHit && !g_Player.isDead) {
		EraseLife(g_Player.Life - 1);
		//無敵時間を数える
		g_Player.NoHitFrame++;
		//攻撃に当たった時の点滅処理
		g_Player.FrashFrame++;
		g_Player.color.a = 1.0f;
		if (g_Player.FrashFrame == 20) {
			g_Player.color.a = 0.0f;
			g_Player.FrashFrame = 0;
		}
	}
	//無敵時間終了
	if (g_Player.NoHitFrame >= NOHITFRAME) {
		g_Player.color.a = 1.0f;
		g_Player.isHit = false;
		g_Player.NoHitFrame = 0;
	}

	//死亡処理---------------------------------------------------------------------------
	if (g_Player.Life <= 0) {
		g_Player.isDead = true;
	}
	if (g_Player.isDead) {
		//こっちを向いている絵
		g_Player.V = (1.0f / 4) * 0;
		if (g_Dead_frame == 0) {
			//SE再生
			PlaySound(g_SoundNo[dead], 0);
			g_Dead_frame++;
		}
		//回転しながら消えていく
		else if (g_Dead_frame > 0) {
			g_Player.rot += 0.1f;
			g_Player.color.a -= 0.008f;
			//透明になったら
		}
		if (g_Player.color.a <= 0.0f) {
			ErasePlayer();
			SetScene(SCENE_TITLE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (!g_Player.isUse) {
		return;
	}
	//プレイヤーの描画
	DrawSpriteColorRotateBase(g_TexNo,
		g_Player.pos.x, g_Player.pos.y,
		PLAYER_WIDTH, PLAYER_HEIGHT,
		g_Player.U, g_Player.V,
		PTN_WIDTH, PTN_HEIGHT,
		g_Player.color.r, g_Player.color.g, g_Player.color.b, g_Player.color.a,
		g_Player.rot);
}

//プレイヤーの位置を渡す-------------------------------------------------------
D3DXVECTOR2 GetPlayerPos(void){
	return g_Player.pos;
}

//チャージ完了時の色
void SetChargeColor(void){
	g_Player.color = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
}

//通常の色
void RemoveColor(void){
	g_Player.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

bool GetFightBoss(void) {
	return g_Player.FightBoss;
}

bool GetPlayerDead(void) {
	return g_Player.isDead;
}

void HitPlayer(void) {
	g_Player.Life--;
}
void HitPlayerforBoss(void) {
	g_Player.Life = g_Player.Life - 2;
}

void SetPlayerisHit(void) {
	g_Player.isHit = true;
	PlaySound(g_SoundNo[hit], 0);
}
//ライフを取得
int GetLife(void) {
	return g_Player.Life;
}

//プレイヤーを使わない状態に
void ErasePlayer(void) {
	g_Player.isUse = false;
}