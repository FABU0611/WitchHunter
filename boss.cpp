/*==============================================================================

	�G�l�~�[�Ǘ� [boss.cpp]
	Author :20106_�c���@
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
// �}�N����`
//*****************************************************************************
#define NUM_SP			(5.0f)

#define BOSS_WIDTH		(120.0f)	//�e�N�X�`���̉���
#define BOSS_HEIGHT		(120.0f)	//�e�N�X�`���̏c��
#define BOSS_CHECK_BLOCK	(10.0f)

#define BOSS_LIFE			(30)
#define BOSS_SHOT_RATE		(20)
#define BOSS_SHOT_NUM		(2)

#define ANIME_SP		(50)		//�A�j���[�V�����̑��x �l���傫���ƒx���Ȃ�
#define ANIME_PTN_YOKO	(3)		//���̃A�j���p�^�[��
#define ANIME_PTN_TATE	(4)		//�c�̃A�j���p�^�[��
#define ANIME_PTN		(ANIME_PTN_YOKO * ANIME_PTN_TATE)	//���ׂẴA�j���p�^�[��

#define PTN_WIDTH		(1.0f / ANIME_PTN_YOKO)	//��l���̉���
#define PTN_HEIGHT		(1.0f / ANIME_PTN_TATE)	//��l���̏c��

#define GRAVITY	(10.0f)

#define SCR_SP		(5.0f)	//�X�N���[�����x
//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//*****************************************************************************
struct BOSS{
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		GHitpos;	//���n����悤�̈ʒu
	D3DXVECTOR2		dir;	//�����x�N�g��
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	float			U = 0.0f;		//U�l�̉���
	float			V = 0.0f;		//V�l�̍���

	float			shot_angle;

	int				Life;

	float			Fall;		//�d�͗p
	bool			isGround;	//���n����
	bool			isJump;		//�W�����v������
	bool			isPressJump;	//�W�����v�{�^������������
	bool			isHead;

	int				state;
	int				count_state;
	int				count_shot;
	int				count_color;
	int				shot_rate;

	bool			onScreen;	//��ʓ�����
	bool			isLive;		//�����邩�t���O
	bool			isFight;	//����Ă�����


	float			JumpPos;	//�W�����v�J�n�n�_
	float			JumpHeight;	//�ő�W�����v�n�_

	int				AnimePattern;
	int				Anime;
	int				AnimeSkipFrame;

	DIRECTION		direction;	//����
};

static BOSS g_Boss;

static int	g_TexNo = 0;
static int	g_SoundNo = 0;

static int	g_AnimeWalk[4] = { 0,1,2,1 };

int g_Dead_frame = 0;
static float	Gravity = 0.0f;		//�d��
//=============================================================================
// ����������
//=============================================================================
void InitBoss(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/boss.png");

	g_SoundNo = LoadSound((char*)"data/sound/b_dead.wav");
	SetVolume(g_SoundNo, 0.2f);

	//������
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
// �I������
//=============================================================================
void UninitBoss(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	Gravity = GRAVITY;

	//��ʂɋ߂Â����烍�[�h
	if (CheckLoadRight(g_Boss.pos.x, g_Boss.pos.y, BOSS_WIDTH, BOSS_HEIGHT)) {
		g_Boss.isLive = true;
	}

	//�����Ă�Ȃ�
	if (!g_Boss.isLive) {
		return;
	}

	//��ʓ�����
	if (!CheckDisp(g_Boss.pos.x, g_Boss.pos.y, BOSS_WIDTH, BOSS_HEIGHT)) {
		g_Boss.onScreen = false;
	}
	else {
		g_Boss.onScreen = true;
	}


	//�n�ʂ̏�ɂ��邩
	g_Boss.GHitpos.x = g_Boss.pos.x;
	g_Boss.GHitpos.y = g_Boss.pos.y + (BOSS_HEIGHT * 0.5f);


	//��ʓ��ɂ����
	if (g_Boss.onScreen) {
		//�A�j���[�V����
		BossAnim();
	}

	//�}�b�v�Ƃ̓����蔻��
	HitBlockforBoss();



	//�U������----------------------------------------------------------------------
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

	//���n���Ă���Ƃ��͗����Ȃ�
	if (g_Boss.isGround) {
		g_Boss.Fall = 0.0f;
		Gravity = 0.0f;
	}
	//�󒆂ɂ���Ƃ������Ă���
	else {
		g_Boss.pos.y -= g_Boss.Fall;
		g_Boss.Fall -= Gravity;
		//�󒆂ŗ������ςȂ��̊G�ɂ��Ȃ�
		if (g_Boss.direction == left) {
			g_Boss.U = PTN_WIDTH * 2;
		}
		if (g_Boss.direction == right) {
			g_Boss.U = PTN_WIDTH * 0;
		}
	}


}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	//�G�l�~�[�̕`��
		//�|���ꂽ����ʊO�Ȃ�\�����Ȃ�
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

//�A�j���[�V����-------------------------------------------------------------------------
void BossAnim(void) {
	g_Boss.AnimeSkipFrame++;					//�t���[���𐔂���
	if (g_Boss.AnimeSkipFrame >= ANIME_SP) {
		g_Boss.AnimeSkipFrame = 0;			//0�ɖ߂�

		g_Boss.AnimePattern++;				//20�t���[���Ɉ�񐔂���
		g_Boss.Anime = g_AnimeWalk[g_Boss.AnimePattern % 4];	//0,1,2,1�����ԂɌ���
		if (g_Boss.AnimePattern > ANIME_PTN_YOKO) {
			g_Boss.AnimePattern = 0;			//0�ɖ߂�
		}
	}
	//UV�l�X�V
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
		//��
		if (g_Boss.dir.x < 0) {
			g_Boss.V = PTN_HEIGHT * 1;
		}
		//�E
		else if (g_Boss.dir.x > 0) {
			g_Boss.V = PTN_HEIGHT * 2;
		}
	}
}

//�ʒu�����Ďg���Ă����Ԃɂ���
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

//Boss�̈ʒu��n��
D3DXVECTOR2 GetBossPos(void){
	return g_Boss.pos;
}

//Boss�������Ă��邩��n��
bool GetBossLive(void){
	return g_Boss.isLive;
}

//Boss������Ă����Ԃ���n��
bool GetFightState(void) {
	return g_Boss.isFight;
}
void SetFightState(bool fight) {
	g_Boss.isFight = fight;
}

//���S����
void DieBoss(void){
	if (g_Dead_frame == 0) {
		PlaySound(g_SoundNo, 0);
	}
	g_Dead_frame++;
	if (g_Dead_frame > 0) {
		//�����Ă���
		g_Boss.color.a -= 0.005f;
		//��������V�[����؂�ւ���
		if (g_Boss.color.a <= 0.0f) {
			g_Boss.isLive = false;
			if (GetFadeState() == SCENE_NONE) {
				SetFadeColor(0.0f, 0.0f, 0.0f);
				//�V�[���̐؂�ւ����s��
				//�Q�[���V�[���J��
				SceneTransition(SCENE_RESULT);
			}
		}
	}
}

void SetDie(void) {
	//���C�t���O�ȉ��ɂȂ����玀��
	if (g_Boss.Life < 1) {
		g_Boss.state = ST_DEAD;
		g_Boss.isFight = false;
		EraseBossexplosion();
		for (int i = 0; i < BOSSBULLET_MAX; i++)		{
			EraseBossbullet(i);
		}
	}
}

//�u���b�N�Ƃ̓����蔻��
void HitBlockforBoss(void) {
	//��
	if (CheckHitBG(g_Boss.pos.x + g_Boss.dir.x * g_Boss.speed, g_Boss.pos.y,
		BOSS_CHECK_SIZE, BOSS_CHECK_SIZE)) {
		g_Boss.dir.x *= -1.0f;
	}
	//�u���b�N�ɒ��n���Ă��邩
	if (CheckHitBG(g_Boss.GHitpos.x, g_Boss.GHitpos.y - g_Boss.Fall,
		BOSS_CHECK_SIZE, BOSS_CHECK_BLOCK)) {
		g_Boss.isGround = true;
	}
	//�u���b�N�ɒ��n���Ă��Ȃ��Ȃ�false
	else {
		g_Boss.isGround = false;
	}
}

//�_���[�W����---------------------------------------------------------------------------
//�v���C���[�̒e�ɓ���������
void HitBulletforBoss(void){
	//��_��ԂȂ��{
	if (g_Boss.state == ST_WEAK) {
		g_Boss.Life -= (BULLET_POWER * 2);
	}
	else {
		g_Boss.Life -= BULLET_POWER;
	}
}
//�v���C���[�̃`���[�W�e�ɓ���������
void HitChrageBulletforBoss(void){
	//��_��ԂȂ��{
	if (g_Boss.state == ST_WEAK) {
		g_Boss.Life -= (CHARGEBULLET_POWER * 2);
	}
	else {
		g_Boss.Life -= CHARGEBULLET_POWER;
	}
}
//�v���C���[�̌��ɓ���������
void HitSwordforBoss(void) {
	//����������ԂȂ��_��Ԃɂ���
	if (g_Boss.state == ST_WAIT_EXPLOSION) {
		g_Boss.state = ST_WEAK;
	}
}



//�{�X��state----------------------------------------------------------------------------
//�ˌ����
void BossStShot(void) {
	g_Boss.shot_rate++;
	RemoveBossColor();
	//�v���C���[�ʒu�擾
	D3DXVECTOR2 player_pos = GetPlayerPos();
	//�p�x���o��
	g_Boss.shot_angle = atan2(g_Boss.pos.y - player_pos.y, g_Boss.pos.x - player_pos.x);
	//�p�x������
	g_Boss.dir.x = -cos(g_Boss.shot_angle);
	g_Boss.dir.y = -sin(g_Boss.shot_angle);
	if (g_Boss.shot_rate > BOSS_SHOT_RATE) {
		SetBossbullet(g_Boss.pos.x, g_Boss.pos.y, g_Boss.dir.x, g_Boss.dir.y);
		g_Boss.shot_rate = 0;
		g_Boss.count_shot++;
	}
	//�ҋ@��ԂɑJ��
	if (g_Boss.count_shot > BOSS_SHOT_NUM) {
		g_Boss.count_shot = 0;
		g_Boss.state = ST_WAIT_EXPLOSION;
	}
	SetDie();
}
//�������
void BossStExplosion(void) {
	RemoveBossColor();
	SetBossexplosion(g_Boss.pos.x, g_Boss.pos.y);
	g_Boss.state = ST_WAIT_SHOT;
	SetDie();
}
//�ˌ��������
void BossStWaitShot(void) {
	g_Boss.count_state++;
	RemoveBossColor();
	if (g_Boss.count_state > 90) {
		g_Boss.count_state = 0;
		g_Boss.state = ST_SHOT;
	}
	SetDie();
}
//�����������
void BossStWaitExplosion(void) {
	g_Boss.count_state++;
	SetWaitExColor();
	//�v���C���[�ʒu�擾
	D3DXVECTOR2 player_pos = GetPlayerPos();
	//�p�x���o��
	g_Boss.shot_angle = atan2(g_Boss.pos.y - player_pos.y, g_Boss.pos.x - player_pos.x);
	//�p�x������
	g_Boss.dir.x = -cos(g_Boss.shot_angle);
	//�߂Â�
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
//��_���
void BossStWeak(void) {
	g_Boss.count_state++;
	SetWeakColor();
	if (g_Boss.count_state > 200) {
		g_Boss.state = ST_WAIT_SHOT;
	}
	SetDie();
}

//boss�̐F-------------------------------------------------------------------------------
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