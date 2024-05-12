/*==============================================================================

	�G�l�~�[�Ǘ� [hone.cpp]
	Author :20106_�c���@
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
// �}�N����`
//*****************************************************************************
#define NUM_SP				(4.0f)

#define HONE_CHECK_WIDTH	(100.0f)
#define HONE_CHECK_HEIGHT	(100.0f)
#define HONE_CHECK_BLOCK	(10.0f)

#define HONE_LIFE			(10)

#define ANIME_SP			(20)		//�A�j���[�V�����̑��x �l���傫���ƒx���Ȃ�
#define ANIME_PTN_YOKO		(3)		//���̃A�j���p�^�[��
#define ANIME_PTN_TATE		(4)		//�c�̃A�j���p�^�[��
#define ANIME_PTN			(ANIME_PTN_YOKO * ANIME_PTN_TATE)	//���ׂẴA�j���p�^�[��

#define PTN_WIDTH			(1.0f / ANIME_PTN_YOKO)	//��l���̉���
#define PTN_HEIGHT			(1.0f / ANIME_PTN_TATE)	//��l���̏c��

#define GRAVITY				(10.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//*****************************************************************************
struct HONE{
	D3DXVECTOR2		pos;		//�ʒu�x�N�g��
	D3DXVECTOR2		GHitpos;	//���n����悤�̈ʒu
	D3DXVECTOR2		dir;		//�����x�N�g��
	float			speed;		//���x
	D3DXCOLOR		color;		//���_�J���[
	float			rot;
	float			U = 0.0f;	//U�l�̉���
	float			V = 0.0f;	//V�l�̍���

	int				Life;		//���C�t

	float			Fall;		//�d�͗p
	bool			isGround;	//���n����
	bool			onScreen;	//��ʓ�����

	int				state;
	int				frame;
	bool			isNohit;	//���G���

	bool			isLive;		//�����Ă邩�t���O
	bool			isLoad;
};

static HONE g_Hone[HONE_MAX];

static int	g_TexNo = 0;
static int	g_SoundNo = 0;

static float	Gravity = 0.0f;		//�d��
//=============================================================================
// ����������
//=============================================================================
void InitHone(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/hone.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/e_guard.wav");
	SetVolume(g_SoundNo, 0.1f);

	//������
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
// �I������
//=============================================================================
void UninitHone(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHone(void)
{
	Gravity = GRAVITY;

	for (int i = 0; i < HONE_MAX; i++) {
		if (g_Hone[i].Life < 1) {
			DieHone(i);
		}

		//��ʂɋ߂Â����烍�[�h
		if (!g_Hone[i].isLive &&
			g_Hone[i].isLoad &&
			CheckLoadRight(g_Hone[i].pos.x, g_Hone[i].pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			g_Hone[i].isLive = true;
			break;
		}
	}

	for (int i = 0; i < HONE_MAX; i++) {
		//�����Ă�Ȃ�
		if (!g_Hone[i].isLive)		{
			continue;
		}

		//��ʓ�����
		if (!CheckDisp(g_Hone[i].pos.x, g_Hone[i].pos.y, HONE_WIDTH, HONE_HEIGHT)) {
			g_Hone[i].onScreen = false;
		}
		else {
			g_Hone[i].onScreen = true;
		}


		//�n�ʂ̏�ɂ��邩
		g_Hone[i].GHitpos.x = g_Hone[i].pos.x;
		g_Hone[i].GHitpos.y = g_Hone[i].pos.y + (HONE_HEIGHT * 0.5f);


		//��ʓ��ɂ����
		if (g_Hone[i].onScreen) {
			//UV�l�X�V
			g_Hone[i].U = PTN_WIDTH * 0;

			//��
			if (g_Hone[i].dir.x < 0)			{
				g_Hone[i].V = PTN_HEIGHT * 1;
			}
			//�E
			else if (g_Hone[i].dir.x > 0)			{
				g_Hone[i].V = PTN_HEIGHT * 2;
			}

			//��ԑJ��
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

		//�}�b�v�Ƃ̓����蔻��-----------------------------------------------------------
		//��
		if (CheckHitBG(g_Hone[i].pos.x + g_Hone[i].dir.x * g_Hone[i].speed, g_Hone[i].pos.y,
			HONE_CHECK_WIDTH, HONE_CHECK_HEIGHT)) {
			g_Hone[i].dir.x *= -1.0f;
		}
		//�u���b�N�ɒ��n���Ă��邩
		if (CheckHitBG(g_Hone[i].GHitpos.x, g_Hone[i].GHitpos.y - g_Hone[i].Fall,
			HONE_CHECK_WIDTH, HONE_CHECK_BLOCK)) {
			g_Hone[i].isGround = true;
		}
		else {
			g_Hone[i].isGround = false;
		}



		//�d�͏���----------------------------------------------------------------------
		g_Hone[i].Fall -= Gravity;

		//���n���Ă���Ƃ��͗����Ȃ�
		if (g_Hone[i].isGround)		{
			g_Hone[i].Fall = 0.0f;
		}
		//�󒆂ɂ���Ƃ������Ă���
		else		{
			g_Hone[i].pos.y -= g_Hone[i].Fall;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHone(void)
{
	//�G�l�~�[�̕`��
	for (int i = 0; i < HONE_MAX; i++)	{
		//�|���ꂽ����ʊO�Ȃ�\�����Ȃ�
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

//�Z�b�^�[-------------------------------------------------------------------------------
//�ʒu�����Ďg���Ă����Ԃɂ���
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
//���G��Ԃ̐F
void SetNohitColor(int index)
{
	g_Hone[index].color = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
	g_Hone[index].U = PTN_WIDTH * 1;
}
//�ʏ��Ԃ̐F
void SetNomalColor(int index)
{
	g_Hone[index].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//�Q�b�^�[-------------------------------------------------------------------------------
//���C�t��n��
int GetHoneLife(int index) {
	return g_Hone[index].Life;
}
//Hone�̈ʒu��n��
D3DXVECTOR2 GetHonePos(int index)
{
	return g_Hone[index].pos;
}
//Hone���\������Ă��邩��n��
bool GetHoneLive(int index)
{
	return g_Hone[index].isLive;
}

//Hone������
void DieHone(int index)
{
	g_Hone[index].isLive = false;
}

//�v���C���[�̒e�ɓ���������
void HitBulletforHone(int idx)
{
	//���G��ԂȂ牽�����Ȃ�
	if (g_Hone[idx].isNohit) {
		return;
	}
	g_Hone[idx].Life -= BULLET_POWER;
}

//�v���C���[�̃`���[�W�e�ɓ���������
void HitChrageBulletforHone(int idx)
{
	//���G��ԂȂ牽�����Ȃ�
	if (g_Hone[idx].isNohit) {
		return;
	}
	g_Hone[idx].Life -= CHARGEBULLET_POWER;
}

//���-----------------------------------------------------------------------------------
//�ˌ����
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

//���G���
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

//�ˌ���ԑҋ@
void HoneStateWaitShot(int idx)
{
	g_Hone[idx].frame++;
	if (g_Hone[idx].frame > 50) {
		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_SHOT;
	}
}

//���G��ԑҋ@
void HoneStateWaitShield(int idx)
{
	g_Hone[idx].frame++;
	if (g_Hone[idx].frame > 30) {
		g_Hone[idx].frame = 0;
		g_Hone[idx].state = ST_SHIELD;
		//���G��Ԉڍs���̉��Đ�
		PlaySound(g_SoundNo, 0);
	}
}