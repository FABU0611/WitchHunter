/*==============================================================================

	�G�l�~�[�Ǘ� [enemy.cpp]
	Author :20106_�c���@
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
// �}�N����`
//*****************************************************************************
#define NUM_SP			(4.0f)

#define ENEMY_CHECK_WIDTH	(90.0f)
#define ENEMY_CHECK_HEIGHT	(90.0f)
#define ENEMY_CHECK_BLOCK	(10.0f)

#define ENEMY_LIFE			(5)

#define ANIME_SP		(20)		//�A�j���[�V�����̑��x �l���傫���ƒx���Ȃ�
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


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct ENEMY{
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		GHitpos;	//���n����悤�̈ʒu
	D3DXVECTOR2		dir;	//�����x�N�g��
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	float			U = 0.0f;		//U�l�̉���
	float			V = 0.0f;		//V�l�̍���

	int				Life;

	float			Fall;		//�d�͗p
	bool			isGround;	//���n����
	bool			onScreen;	//��ʓ�����

	bool			isLive;	//�����邩�t���O
	bool			isLoad;

	int				AnimePattern;
	int				Anime;
	int				AnimeSkipFrame;
};

static ENEMY g_Enemy[ENEMY_MAX];

static int	g_TexNo = 0;
static int	g_AnimeWalk[4] = { 0,1,2,1 };

static float	Gravity = 0.0f;		//�d��
//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/hatghost_11.png");

	//������
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
// �I������
//=============================================================================
void UninitEnemy(void)
{

}

//=============================================================================
// �X�V����
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
		//�����Ă�Ȃ�
		if (!g_Enemy[i].isLive)		{
			continue;
		}

		//��ʓ�����
		g_Enemy[i].onScreen = CheckDisp(g_Enemy[i].pos.x, g_Enemy[i].pos.y, ENEMY_WIDTH, ENEMY_HEIGHT);


		//�n�ʂ̏�ɂ��邩
		g_Enemy[i].GHitpos.x = g_Enemy[i].pos.x;
		g_Enemy[i].GHitpos.y = g_Enemy[i].pos.y + (ENEMY_HEIGHT * 0.5f);


		//��ʓ��ɂ����
		if (g_Enemy[i].onScreen) {
			//�A�j���[�V����-------------------------------------------------------------------
			g_Enemy[i].AnimeSkipFrame++;					//�t���[���𐔂���
			if (g_Enemy[i].AnimeSkipFrame >= ANIME_SP)
			{
				g_Enemy[i].AnimeSkipFrame = 0;			//0�ɖ߂�

				g_Enemy[i].AnimePattern++;				//20�t���[���Ɉ�񐔂���
				g_Enemy[i].Anime = g_AnimeWalk[g_Enemy[i].AnimePattern % 4];	//0,1,2,1�����ԂɌ���
				if (g_Enemy[i].AnimePattern > ANIME_PTN_YOKO)
				{
					g_Enemy[i].AnimePattern = 0;			//0�ɖ߂�
				}
			}
			//UV�l�X�V
			g_Enemy[i].U = PTN_WIDTH * g_Enemy[i].Anime;

			//��
			if (g_Enemy[i].dir.x < 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 1;
			}
			//�E
			else if (g_Enemy[i].dir.x > 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 2;
			}
			//��
			if (g_Enemy[i].dir.y < 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 3;
			}
			//��
			else if (g_Enemy[i].dir.y > 0)
			{
				g_Enemy[i].V = PTN_HEIGHT * 0;
			}
		}

		//�}�b�v�Ƃ̓����蔻��
		//��
		if (CheckHitBG(g_Enemy[i].pos.x + g_Enemy[i].dir.x * g_Enemy[i].speed, g_Enemy[i].pos.y,
			ENEMY_CHECK_WIDTH, ENEMY_CHECK_HEIGHT)) {
			g_Enemy[i].dir.x *= -1.0f;
		}
		//�u���b�N�ɒ��n���Ă��邩
		if (CheckHitBG(g_Enemy[i].GHitpos.x, g_Enemy[i].GHitpos.y - g_Enemy[i].Fall,
			ENEMY_CHECK_WIDTH, ENEMY_CHECK_BLOCK)) {
			g_Enemy[i].isGround = true;
		}
		else {
			g_Enemy[i].isGround = false;
		}


		//���W�X�V
		g_Enemy[i].pos += g_Enemy[i].dir * g_Enemy[i].speed;


		//�d�͏���----------------------------------------------------------------------
		g_Enemy[i].Fall -= Gravity;

		//���n���Ă���Ƃ��͗����Ȃ�
		if (g_Enemy[i].isGround)
		{
			g_Enemy[i].Fall = 0.0f;
		}
		//�󒆂ɂ���Ƃ������Ă���
		else
		{
			g_Enemy[i].pos.y -= g_Enemy[i].Fall;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	//�G�l�~�[�̕`��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		//�|���ꂽ����ʊO�Ȃ�\�����Ȃ�
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

//�ʒu�����Ďg���Ă����Ԃɂ���
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

//Enemy�̈ʒu��n��
D3DXVECTOR2 GetEnemyPos(int index)
{
	return g_Enemy[index].pos;
}

//Enemy���\������Ă��邩��n��
bool GetEnemyState(int index)
{
	return g_Enemy[index].isLive;
}

//Enemy������
void DieEnemy(int index)
{
	g_Enemy[index].isLive = false;
}

//�v���C���[�̒e�ɓ���������
void HitBulletforEnemy(int idx)
{
	if (!g_Enemy[idx].isLive) {
		return;
	}
	g_Enemy[idx].Life -= BULLET_POWER;
}

//�v���C���[�̃`���[�W�e�ɓ���������
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