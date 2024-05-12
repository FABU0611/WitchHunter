/*==============================================================================

	当たり判定管理 [collision.cpp]
	Author :20106_田中蓮
	Date   :23_06_13
--------------------------------------------------------------------------------

==============================================================================*/
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "hone.h"
#include "honebullet.h"
#include "bullet.h"
#include "chargebullet.h"
#include "sword.h"
#include "boss.h"
#include "bossbullet.h"
#include "bossexplosion.h"
#include "camera.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CheckHitBB(float boxa_x, float boxa_y, float boxa_w, float boxa_h,
	float boxb_x, float boxb_y, float boxb_w, float boxb_h);

bool CheckHitBC(float ccla_x, float ccla_y, float ccla_r,
	float cclb_x, float cclb_y, float cclb_r);



void ToGhost(void);
void ToHone(void);
void ToBoss(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR2 player_pos;
D3DXVECTOR2 boss_pos;
D3DXVECTOR2 bossbullet_pos;
D3DXVECTOR2 bossexplosion_pos;
D3DXVECTOR2 enemy_pos;
D3DXVECTOR2 hone_pos;
D3DXVECTOR2 honebullet_pos;
D3DXVECTOR2 bullet_pos;
D3DXVECTOR2 chargebullet_pos;
D3DXVECTOR2 sword_pos;
D3DXVECTOR2 swordin_pos;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCollision(void)
{
	player_pos.y = 0.0f;
	player_pos.x = 0.0f;
	enemy_pos.x = 0.0f;
	enemy_pos.y = 0.0f;
	hone_pos.x = 0.0f;
	hone_pos.y = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCollision(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCollision(void)
{
	//当たり判定の処理
	if (GetPlayerDead()) {
		return;
	}
	ToGhost();
	ToHone();
	ToBoss();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCollision(void)
{

}

//四角の当たり判定
bool CheckHitBB(float boxa_x, float boxa_y, float boxa_w, float boxa_h,
				float boxb_x, float boxb_y, float boxb_w, float boxb_h){
	float boxa_left	  = boxa_x - (boxa_w * 0.5f);
	float boxa_right  = boxa_x + (boxa_w * 0.5f);
	float boxa_top	  = boxa_y - (boxa_h * 0.5f);
	float boxa_bottom = boxa_y + (boxa_h * 0.5f);

	float boxb_left	  = boxb_x - (boxb_w * 0.5f);
	float boxb_right  = boxb_x + (boxb_w * 0.5f);
	float boxb_top	  = boxb_y - (boxb_h * 0.5f);
	float boxb_bottom = boxb_y + (boxb_h * 0.5f);

	if (boxa_left < boxb_right &&
		boxa_right > boxb_left &&
		boxa_top < boxb_bottom &&
		boxa_bottom > boxb_top) {
		// 当たっていたらtrue
		return true;
	}
	//当たっていなかったらfalse
	return false;
}

//境界円同氏の当たり判定
//二つの円の距離とそれぞれの半径を足した数を比較
//こっちのほうが少し早い
bool CheckHitBCSq(float ccla_x, float ccla_y, float ccla_r,
				float cclb_x, float cclb_y, float cclb_r){
	D3DXVECTOR2 cirApos = D3DXVECTOR2(ccla_x, ccla_y);
	D3DXVECTOR2 cirBpos = D3DXVECTOR2(cclb_x, cclb_y);


	// A 100, 80
	// B 330, 200

	// dist 230, 120


	//AからBのほうに向かってくるベクトル
	D3DXVECTOR2 dist = cirBpos - cirApos;

	//ベクトルの大きさの二乗を計算（二つの円の距離のようなもの）
	float length = D3DXVec2LengthSq(&dist);

//	pow(dist.x, 2); 後ろに何条したいかいれる
//	float length = dist.x * diat.x + dist.y * dist.y;

	//二つの円の半径足した長さの二乗
	float r = (ccla_r + cclb_r) * (ccla_r + cclb_r);

	//lengthが小さければ当たっている
	if (length < r)	{
		return true;
	}

	return false;
}

//境界円同氏の当たり判定
//二つの円の距離とそれぞれの半径を足した数を比較
bool CheckHitBC(float ccla_x, float ccla_y, float ccla_r,
				float cclb_x, float cclb_y, float cclb_r){
	D3DXVECTOR2 cirApos = D3DXVECTOR2(ccla_x, ccla_y);
	D3DXVECTOR2 cirBpos = D3DXVECTOR2(cclb_x, cclb_y);


	// A 100, 80
	// B 330, 200

	// dist 230, 120


	//AからBのほうに向かってくるベクトル
	D3DXVECTOR2 dist = cirBpos - cirApos;

	//ベクトルの大きさを計算（二つの円の距離のようなもの）
	float length = D3DXVec2Length(&dist);

//	pow(dist.x, 2); 後ろに何条したいかいれる
//	float length = sqrt(dist.x * diat.x + dist.y * dist.y);

	//二つの円の半径足した長さ
	float r = ccla_r + cclb_r;

	//lengthが小さければ当たっている
	if (length < r)	{
		return true;
	}

	return false;
}

//画面内にいるか
bool CheckDisp(float objx, float objy, float objw, float objh){
	D3DXVECTOR2 base = GetBasePos();
	if (CheckHitBB(SCREEN_WIDTH * 0.5f - base.x, SCREEN_HEIGHT * 0.5f + base.y, SCREEN_WIDTH, SCREEN_HEIGHT,
		objx, objy, objw, objh))	{
		return true;
	}
	return false;
}
//画面に近づいたか
bool CheckLoadRight(float objx, float objy, float objw, float objh){
	D3DXVECTOR2 base = GetBasePos();
	if (CheckHitBB((SCREEN_WIDTH + 200.0f) - base.x, SCREEN_HEIGHT * 0.5f + base.y, 200.0f, SCREEN_HEIGHT,
		objx, objy, objw, objh))	{
		return true;
	}
	return false;
}

void ToPlayer(void) {
	//プレイヤーとゴーストの当たり判定
	for (int i = 0; i < ENEMY_MAX; i++) {
		//生きている敵だけ処理
		if (!GetEnemyState(i)) {
			continue;
		}
		//プレイヤーの位置取得
		player_pos = GetPlayerPos();
		//ゴーストの位置を取得
		enemy_pos = GetEnemyPos(i);

		//円で判定
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}

	//プレイヤーと骨の当たり判定
	for (int i = 0; i < HONE_MAX; i++) {
		//生きている敵だけ処理
		if (!GetHoneLive(i)) {
			continue;
		}
		//プレイヤーの位置取得
		player_pos = GetPlayerPos();
		//骨の位置を取得
		hone_pos = GetHonePos(i);

		//円で判定
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			hone_pos.x, hone_pos.y, HONE_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}

	//骨の弾との当たり判定
	for (int i = 0; i < HONEBULLET_MAX; i++) {
		if (!GetHonebulletUse(i)) {
			continue;
		}
		//プレイヤーの位置取得
		player_pos = GetPlayerPos();
		//骨の弾の位置を取得
		honebullet_pos = GetHonebulletPos(i);

		//円で判定
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			honebullet_pos.x, honebullet_pos.y, HONEBULLET_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}


	//プレイヤーとボスの当たり判定
	if (!GetFightState()) {
		return;
	}
	//ボスの位置を取得
	boss_pos = GetBossPos();
	//プレイヤーの位置取得
	player_pos = GetPlayerPos();

	//円で判定
	if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
		boss_pos.x, boss_pos.y, BOSS_RAD)) {
		HitPlayerforBoss();
		SetPlayerisHit();
	}

	//ボスの弾との当たり判定
	for (int i = 0; i < BOSSBULLET_MAX; i++) {
		if (!GetBossbulletUse(i)) {
			continue;
		}
		//プレイヤーの位置取得
		player_pos = GetPlayerPos();
		//骨の弾の位置を取得
		bossbullet_pos = GetBossbulletPos(i);

		//円で判定
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			bossbullet_pos.x, bossbullet_pos.y, HONEBULLET_RAD)) {
			HitPlayerforBoss();
			SetPlayerisHit();
			break;
		}
	}

	//ボスの爆発との当たり判定
	if (!GetBossexplosionUse()) {
		return;
	}
	//プレイヤーの位置取得
	player_pos = GetPlayerPos();
	//ボスの位置を取得
	bossexplosion_pos = GetBossexplosionPos();

	//円で判定
	if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
		bossexplosion_pos.x, bossexplosion_pos.y, GetBossexplosionRad())) {
		HitPlayerforBoss();
		SetPlayerisHit();
	}
}

//ゴーストの当たり判定
void ToGhost(void) {
	//弾とゴーストの当たり判定
	for (int i = 0; i < ENEMY_MAX; i++)	{
		//生きている敵だけ処理
		if (!GetEnemyState(i)) {
			continue;
		}
		//ゴーストの位置を取得
		enemy_pos = GetEnemyPos(i);

		for (int j = 0; j < BULLET_MAX; j++) {
			//出現している弾だけ処理
			if (!GetBulletUse(j)) {
				continue;
			}
			//弾の位置を取得
			bullet_pos = GetBulletPos(j);

			//当たり判定
			if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
				enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
				//当たったら弾と敵を消す
				HitBulletforEnemy(i);
				EraseBullet(j);
				//ライフが０以下になったら死ぬ
				if (GetEnemyLife(i) < 1) {
					DieEnemy(i);
				}
				break;
			}
		}
	}

	//チャージ弾とゴーストの当たり判定
	for (int i = 0; i < ENEMY_MAX; i++) {
		//生きている敵だけ処理
		if (!GetEnemyState(i)) {
			continue;
		}
		//ゴーストの位置を取得
		enemy_pos = GetEnemyPos(i);

		for (int j = 0; j < CHARGEBULLET_MAX; j++) {
			//出現している弾だけ処理
			if (!GetChargeBulletUse(j)) {
				continue;
			}
			//弾の位置を取得
			chargebullet_pos = GetChargeBulletPos(j);

			//当たり判定
			if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
				enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
				//当たったら弾と敵を消す
				HitChrageBulletforEnemy(i);
				EraseChargeBullet(j);
				//ライフが０以下になったら死ぬ
				if (GetEnemyLife(i) < 1) {
					DieEnemy(i);
				}
				break;
			}
		}
	}

	//剣とゴーストの当たり判定
	for (int i = 0; i < ENEMY_MAX; i++) {
		//生きている敵だけ処理
		if (!GetEnemyState(i)) {
			continue;
		}
		//ゴーストの位置を取得
		enemy_pos = GetEnemyPos(i);

		//出現している剣だけ処理
		if (!GetSwordUse()) {
			continue;
		}
		//弾の位置を取得
		sword_pos = GetSwordPos();
		swordin_pos = GetInhitPos();

		//当たり判定
		if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			enemy_pos.x, enemy_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT) ||
			CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
				enemy_pos.x, enemy_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT)) {
			//敵を消す
			DieEnemy(i);
			break;
		}
	}
}

//骨の当たり判定
void ToHone(void) {
	//弾と骨の当たり判定
	for (int i = 0; i < HONE_MAX; i++)	{
		//生きている敵だけ処理
		if (!GetHoneLive(i)) {
			continue;
		}
		//骨の位置を取得
		hone_pos = GetHonePos(i);

		for (int j = 0; j < BULLET_MAX; j++) {
			//出現している弾だけ処理
			if (!GetBulletUse(j)) {
				continue;
			}
			//弾の位置を取得
			bullet_pos = GetBulletPos(j);

			//当たり判定
			if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
				hone_pos.x, hone_pos.y, HONE_RAD)) {
				//当たったら弾と敵を消す
				HitBulletforHone(i);
				EraseBullet(j);
				if (GetHoneLife(i) < 1) {
					DieHone(i);
				}
				break;
			}
		}
	}

	//チャージ弾と骨の当たり判定
	for (int i = 0; i < HONE_MAX; i++) {
		//生きている敵だけ処理
		if (!GetHoneLive(i)) {
			continue;
		}
		//骨の位置を取得
		hone_pos = GetHonePos(i);

		for (int j = 0; j < CHARGEBULLET_MAX; j++) {
			//出現している弾だけ処理
			if (!GetChargeBulletUse(j)) {
				continue;
			}
			//弾の位置を取得
			chargebullet_pos = GetChargeBulletPos(j);

			//当たり判定
			if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
				hone_pos.x, hone_pos.y, HONE_RAD)) {
				//当たったら弾と敵を消す
				HitChrageBulletforHone(i);
				EraseChargeBullet(j);
				if (GetHoneLife(i) < 1) {
					DieHone(i);
				}
				break;
			}
		}
	}

	//剣と骨の当たり判定
	for (int i = 0; i < HONE_MAX; i++) {
		//生きている敵だけ処理
		if (!GetHoneLive(i)) {
			continue;
		}
		//骨の位置を取得
		hone_pos = GetHonePos(i);

		//出現している剣だけ処理
		if (!GetSwordUse()) {
			continue;
		}
		//弾の位置を取得
		sword_pos = GetSwordPos();
		swordin_pos = GetInhitPos();

		//当たり判定
		if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			hone_pos.x, hone_pos.y, HONE_WIDTH, HONE_HEIGHT) ||
			CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
				hone_pos.x, hone_pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			//敵を消す
			DieHone(i);
			break;
		}
	}
}

//ボスの当たり判定
void ToBoss(void) {
	//弾とボスの当たり判定
	if (!GetFightState()) {
		return;
	}
	//ボスの位置を取得
	boss_pos = GetBossPos();

	for (int j = 0; j < BULLET_MAX; j++) {
		//出現している弾だけ処理
		if (!GetBulletUse(j)) {
			continue;
		}
		//弾の位置を取得
		bullet_pos = GetBulletPos(j);

		//当たり判定
		if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
			boss_pos.x, boss_pos.y, BOSS_RAD)) {
			//当たったら弾と敵を消す
			HitBulletforBoss();
			EraseBullet(j);
			break;
		}
	}


	//チャージ弾とボスの当たり判定
	if (!GetFightBoss()) {
		return;
	}
	//ボスの位置を取得
	boss_pos = GetBossPos();

	for (int j = 0; j < CHARGEBULLET_MAX; j++) {
		//出現している弾だけ処理
		if (!GetChargeBulletUse(j)) {
			continue;
		}
		//弾の位置を取得
		chargebullet_pos = GetChargeBulletPos(j);

		//当たり判定
		if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
			boss_pos.x, boss_pos.y, BOSS_RAD)) {
			//当たったら弾を消してボスの体力を減らす
			HitChrageBulletforBoss();
			EraseChargeBullet(j);
			break;
		}
	}


	//剣とボスの当たり判定
		//生きている敵だけ処理
	if (!GetFightBoss()) {
		return;
	}
	//ボスの位置を取得
	boss_pos = GetBossPos();

	//出現している剣だけ処理
	if (!GetSwordUse()) {
		return;
	}
	//剣の位置を取得
	sword_pos = GetSwordPos();
	swordin_pos = GetInhitPos();

	//当たり判定
	if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
		boss_pos.x, boss_pos.y, HONE_WIDTH, HONE_HEIGHT) ||
		CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			boss_pos.x, boss_pos.y, HONE_WIDTH, HONE_HEIGHT)) {
		//ボスを弱点状態にする
		HitSwordforBoss();
	}
}