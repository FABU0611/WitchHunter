/*==============================================================================

	�����蔻��Ǘ� [collision.cpp]
	Author :20106_�c���@
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
// �v���g�^�C�v�錾
//*****************************************************************************
bool CheckHitBB(float boxa_x, float boxa_y, float boxa_w, float boxa_h,
	float boxb_x, float boxb_y, float boxb_w, float boxb_h);

bool CheckHitBC(float ccla_x, float ccla_y, float ccla_r,
	float cclb_x, float cclb_y, float cclb_r);



void ToGhost(void);
void ToHone(void);
void ToBoss(void);

//*****************************************************************************
// �O���[�o���ϐ�
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
// ����������
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
// �I������
//=============================================================================
void UninitCollision(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCollision(void)
{
	//�����蔻��̏���
	if (GetPlayerDead()) {
		return;
	}
	ToGhost();
	ToHone();
	ToBoss();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCollision(void)
{

}

//�l�p�̓����蔻��
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
		// �������Ă�����true
		return true;
	}
	//�������Ă��Ȃ�������false
	return false;
}

//���E�~�����̓����蔻��
//��̉~�̋����Ƃ��ꂼ��̔��a�𑫂��������r
//�������̂ق�����������
bool CheckHitBCSq(float ccla_x, float ccla_y, float ccla_r,
				float cclb_x, float cclb_y, float cclb_r){
	D3DXVECTOR2 cirApos = D3DXVECTOR2(ccla_x, ccla_y);
	D3DXVECTOR2 cirBpos = D3DXVECTOR2(cclb_x, cclb_y);


	// A 100, 80
	// B 330, 200

	// dist 230, 120


	//A����B�̂ق��Ɍ������Ă���x�N�g��
	D3DXVECTOR2 dist = cirBpos - cirApos;

	//�x�N�g���̑傫���̓����v�Z�i��̉~�̋����̂悤�Ȃ��́j
	float length = D3DXVec2LengthSq(&dist);

//	pow(dist.x, 2); ���ɉ����������������
//	float length = dist.x * diat.x + dist.y * dist.y;

	//��̉~�̔��a�����������̓��
	float r = (ccla_r + cclb_r) * (ccla_r + cclb_r);

	//length����������Γ������Ă���
	if (length < r)	{
		return true;
	}

	return false;
}

//���E�~�����̓����蔻��
//��̉~�̋����Ƃ��ꂼ��̔��a�𑫂��������r
bool CheckHitBC(float ccla_x, float ccla_y, float ccla_r,
				float cclb_x, float cclb_y, float cclb_r){
	D3DXVECTOR2 cirApos = D3DXVECTOR2(ccla_x, ccla_y);
	D3DXVECTOR2 cirBpos = D3DXVECTOR2(cclb_x, cclb_y);


	// A 100, 80
	// B 330, 200

	// dist 230, 120


	//A����B�̂ق��Ɍ������Ă���x�N�g��
	D3DXVECTOR2 dist = cirBpos - cirApos;

	//�x�N�g���̑傫�����v�Z�i��̉~�̋����̂悤�Ȃ��́j
	float length = D3DXVec2Length(&dist);

//	pow(dist.x, 2); ���ɉ����������������
//	float length = sqrt(dist.x * diat.x + dist.y * dist.y);

	//��̉~�̔��a����������
	float r = ccla_r + cclb_r;

	//length����������Γ������Ă���
	if (length < r)	{
		return true;
	}

	return false;
}

//��ʓ��ɂ��邩
bool CheckDisp(float objx, float objy, float objw, float objh){
	D3DXVECTOR2 base = GetBasePos();
	if (CheckHitBB(SCREEN_WIDTH * 0.5f - base.x, SCREEN_HEIGHT * 0.5f + base.y, SCREEN_WIDTH, SCREEN_HEIGHT,
		objx, objy, objw, objh))	{
		return true;
	}
	return false;
}
//��ʂɋ߂Â�����
bool CheckLoadRight(float objx, float objy, float objw, float objh){
	D3DXVECTOR2 base = GetBasePos();
	if (CheckHitBB((SCREEN_WIDTH + 200.0f) - base.x, SCREEN_HEIGHT * 0.5f + base.y, 200.0f, SCREEN_HEIGHT,
		objx, objy, objw, objh))	{
		return true;
	}
	return false;
}

void ToPlayer(void) {
	//�v���C���[�ƃS�[�X�g�̓����蔻��
	for (int i = 0; i < ENEMY_MAX; i++) {
		//�����Ă���G��������
		if (!GetEnemyState(i)) {
			continue;
		}
		//�v���C���[�̈ʒu�擾
		player_pos = GetPlayerPos();
		//�S�[�X�g�̈ʒu���擾
		enemy_pos = GetEnemyPos(i);

		//�~�Ŕ���
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}

	//�v���C���[�ƍ��̓����蔻��
	for (int i = 0; i < HONE_MAX; i++) {
		//�����Ă���G��������
		if (!GetHoneLive(i)) {
			continue;
		}
		//�v���C���[�̈ʒu�擾
		player_pos = GetPlayerPos();
		//���̈ʒu���擾
		hone_pos = GetHonePos(i);

		//�~�Ŕ���
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			hone_pos.x, hone_pos.y, HONE_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}

	//���̒e�Ƃ̓����蔻��
	for (int i = 0; i < HONEBULLET_MAX; i++) {
		if (!GetHonebulletUse(i)) {
			continue;
		}
		//�v���C���[�̈ʒu�擾
		player_pos = GetPlayerPos();
		//���̒e�̈ʒu���擾
		honebullet_pos = GetHonebulletPos(i);

		//�~�Ŕ���
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			honebullet_pos.x, honebullet_pos.y, HONEBULLET_RAD)) {
			HitPlayer();
			SetPlayerisHit();
			break;
		}
	}


	//�v���C���[�ƃ{�X�̓����蔻��
	if (!GetFightState()) {
		return;
	}
	//�{�X�̈ʒu���擾
	boss_pos = GetBossPos();
	//�v���C���[�̈ʒu�擾
	player_pos = GetPlayerPos();

	//�~�Ŕ���
	if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
		boss_pos.x, boss_pos.y, BOSS_RAD)) {
		HitPlayerforBoss();
		SetPlayerisHit();
	}

	//�{�X�̒e�Ƃ̓����蔻��
	for (int i = 0; i < BOSSBULLET_MAX; i++) {
		if (!GetBossbulletUse(i)) {
			continue;
		}
		//�v���C���[�̈ʒu�擾
		player_pos = GetPlayerPos();
		//���̒e�̈ʒu���擾
		bossbullet_pos = GetBossbulletPos(i);

		//�~�Ŕ���
		if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
			bossbullet_pos.x, bossbullet_pos.y, HONEBULLET_RAD)) {
			HitPlayerforBoss();
			SetPlayerisHit();
			break;
		}
	}

	//�{�X�̔����Ƃ̓����蔻��
	if (!GetBossexplosionUse()) {
		return;
	}
	//�v���C���[�̈ʒu�擾
	player_pos = GetPlayerPos();
	//�{�X�̈ʒu���擾
	bossexplosion_pos = GetBossexplosionPos();

	//�~�Ŕ���
	if (CheckHitBCSq(player_pos.x, player_pos.y, PLAYER_RAD,
		bossexplosion_pos.x, bossexplosion_pos.y, GetBossexplosionRad())) {
		HitPlayerforBoss();
		SetPlayerisHit();
	}
}

//�S�[�X�g�̓����蔻��
void ToGhost(void) {
	//�e�ƃS�[�X�g�̓����蔻��
	for (int i = 0; i < ENEMY_MAX; i++)	{
		//�����Ă���G��������
		if (!GetEnemyState(i)) {
			continue;
		}
		//�S�[�X�g�̈ʒu���擾
		enemy_pos = GetEnemyPos(i);

		for (int j = 0; j < BULLET_MAX; j++) {
			//�o�����Ă���e��������
			if (!GetBulletUse(j)) {
				continue;
			}
			//�e�̈ʒu���擾
			bullet_pos = GetBulletPos(j);

			//�����蔻��
			if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
				enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
				//����������e�ƓG������
				HitBulletforEnemy(i);
				EraseBullet(j);
				//���C�t���O�ȉ��ɂȂ����玀��
				if (GetEnemyLife(i) < 1) {
					DieEnemy(i);
				}
				break;
			}
		}
	}

	//�`���[�W�e�ƃS�[�X�g�̓����蔻��
	for (int i = 0; i < ENEMY_MAX; i++) {
		//�����Ă���G��������
		if (!GetEnemyState(i)) {
			continue;
		}
		//�S�[�X�g�̈ʒu���擾
		enemy_pos = GetEnemyPos(i);

		for (int j = 0; j < CHARGEBULLET_MAX; j++) {
			//�o�����Ă���e��������
			if (!GetChargeBulletUse(j)) {
				continue;
			}
			//�e�̈ʒu���擾
			chargebullet_pos = GetChargeBulletPos(j);

			//�����蔻��
			if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
				enemy_pos.x, enemy_pos.y, ENEMY_RAD)) {
				//����������e�ƓG������
				HitChrageBulletforEnemy(i);
				EraseChargeBullet(j);
				//���C�t���O�ȉ��ɂȂ����玀��
				if (GetEnemyLife(i) < 1) {
					DieEnemy(i);
				}
				break;
			}
		}
	}

	//���ƃS�[�X�g�̓����蔻��
	for (int i = 0; i < ENEMY_MAX; i++) {
		//�����Ă���G��������
		if (!GetEnemyState(i)) {
			continue;
		}
		//�S�[�X�g�̈ʒu���擾
		enemy_pos = GetEnemyPos(i);

		//�o�����Ă��錕��������
		if (!GetSwordUse()) {
			continue;
		}
		//�e�̈ʒu���擾
		sword_pos = GetSwordPos();
		swordin_pos = GetInhitPos();

		//�����蔻��
		if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			enemy_pos.x, enemy_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT) ||
			CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
				enemy_pos.x, enemy_pos.y, ENEMY_WIDTH, ENEMY_HEIGHT)) {
			//�G������
			DieEnemy(i);
			break;
		}
	}
}

//���̓����蔻��
void ToHone(void) {
	//�e�ƍ��̓����蔻��
	for (int i = 0; i < HONE_MAX; i++)	{
		//�����Ă���G��������
		if (!GetHoneLive(i)) {
			continue;
		}
		//���̈ʒu���擾
		hone_pos = GetHonePos(i);

		for (int j = 0; j < BULLET_MAX; j++) {
			//�o�����Ă���e��������
			if (!GetBulletUse(j)) {
				continue;
			}
			//�e�̈ʒu���擾
			bullet_pos = GetBulletPos(j);

			//�����蔻��
			if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
				hone_pos.x, hone_pos.y, HONE_RAD)) {
				//����������e�ƓG������
				HitBulletforHone(i);
				EraseBullet(j);
				if (GetHoneLife(i) < 1) {
					DieHone(i);
				}
				break;
			}
		}
	}

	//�`���[�W�e�ƍ��̓����蔻��
	for (int i = 0; i < HONE_MAX; i++) {
		//�����Ă���G��������
		if (!GetHoneLive(i)) {
			continue;
		}
		//���̈ʒu���擾
		hone_pos = GetHonePos(i);

		for (int j = 0; j < CHARGEBULLET_MAX; j++) {
			//�o�����Ă���e��������
			if (!GetChargeBulletUse(j)) {
				continue;
			}
			//�e�̈ʒu���擾
			chargebullet_pos = GetChargeBulletPos(j);

			//�����蔻��
			if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
				hone_pos.x, hone_pos.y, HONE_RAD)) {
				//����������e�ƓG������
				HitChrageBulletforHone(i);
				EraseChargeBullet(j);
				if (GetHoneLife(i) < 1) {
					DieHone(i);
				}
				break;
			}
		}
	}

	//���ƍ��̓����蔻��
	for (int i = 0; i < HONE_MAX; i++) {
		//�����Ă���G��������
		if (!GetHoneLive(i)) {
			continue;
		}
		//���̈ʒu���擾
		hone_pos = GetHonePos(i);

		//�o�����Ă��錕��������
		if (!GetSwordUse()) {
			continue;
		}
		//�e�̈ʒu���擾
		sword_pos = GetSwordPos();
		swordin_pos = GetInhitPos();

		//�����蔻��
		if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			hone_pos.x, hone_pos.y, HONE_WIDTH, HONE_HEIGHT) ||
			CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
				hone_pos.x, hone_pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			//�G������
			DieHone(i);
			break;
		}
	}
}

//�{�X�̓����蔻��
void ToBoss(void) {
	//�e�ƃ{�X�̓����蔻��
	if (!GetFightState()) {
		return;
	}
	//�{�X�̈ʒu���擾
	boss_pos = GetBossPos();

	for (int j = 0; j < BULLET_MAX; j++) {
		//�o�����Ă���e��������
		if (!GetBulletUse(j)) {
			continue;
		}
		//�e�̈ʒu���擾
		bullet_pos = GetBulletPos(j);

		//�����蔻��
		if (CheckHitBCSq(bullet_pos.x, bullet_pos.y, BULLET_RAD,
			boss_pos.x, boss_pos.y, BOSS_RAD)) {
			//����������e�ƓG������
			HitBulletforBoss();
			EraseBullet(j);
			break;
		}
	}


	//�`���[�W�e�ƃ{�X�̓����蔻��
	if (!GetFightBoss()) {
		return;
	}
	//�{�X�̈ʒu���擾
	boss_pos = GetBossPos();

	for (int j = 0; j < CHARGEBULLET_MAX; j++) {
		//�o�����Ă���e��������
		if (!GetChargeBulletUse(j)) {
			continue;
		}
		//�e�̈ʒu���擾
		chargebullet_pos = GetChargeBulletPos(j);

		//�����蔻��
		if (CheckHitBCSq(chargebullet_pos.x, chargebullet_pos.y, CHARGEBULLET_RAD,
			boss_pos.x, boss_pos.y, BOSS_RAD)) {
			//����������e�������ă{�X�̗̑͂����炷
			HitChrageBulletforBoss();
			EraseChargeBullet(j);
			break;
		}
	}


	//���ƃ{�X�̓����蔻��
		//�����Ă���G��������
	if (!GetFightBoss()) {
		return;
	}
	//�{�X�̈ʒu���擾
	boss_pos = GetBossPos();

	//�o�����Ă��錕��������
	if (!GetSwordUse()) {
		return;
	}
	//���̈ʒu���擾
	sword_pos = GetSwordPos();
	swordin_pos = GetInhitPos();

	//�����蔻��
	if (CheckHitBB(sword_pos.x, sword_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
		boss_pos.x, boss_pos.y, HONE_WIDTH, HONE_HEIGHT) ||
		CheckHitBB(swordin_pos.x, swordin_pos.y, SWORD_WIDTH, SWORD_HEIGHT,
			boss_pos.x, boss_pos.y, HONE_WIDTH, HONE_HEIGHT)) {
		//�{�X����_��Ԃɂ���
		HitSwordforBoss();
	}
}