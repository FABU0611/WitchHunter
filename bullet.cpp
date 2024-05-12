/*==============================================================================

	�o���b�g�Ǘ� [bullet.cpp]
	Author :20106_�c���@
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#include "bullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_WIDTH	(30.0f)
#define BULLET_HEIGHT	(30.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct BULLET {
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		dir;	//�ړ�����
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	bool			use;
	float			U;		//U�l�̉���
	float			V;		//V�l�̍���
};

static BULLET g_Bullet[BULLET_MAX];

static int		g_ShotNo = 0;

static int g_TexNo = 0;

//=============================================================================
// ����������
//=============================================================================
void InitBullet(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet00.png");

	//�T�E���h�̓ǂݍ���
	g_ShotNo = LoadSound((char*)"data/sound/p_shot.wav");
	SetVolume(g_ShotNo, 0.3f);

	//������
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].pos.x = 0.0f;
		g_Bullet[i].pos.y = 0.0f;
		g_Bullet[i].dir.x = 0.0f;
		g_Bullet[i].dir.y = 0.0f;
		g_Bullet[i].speed = 30.0f;
		g_Bullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Bullet[i].rot = 0.0f;
		g_Bullet[i].use = false;
		g_Bullet[i].U = 0.0f;
		g_Bullet[i].V = 0.0f;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	StopSound(g_ShotNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_Bullet[i].use)
		{
			continue;
		}

		//���W�X�V
		g_Bullet[i].pos += g_Bullet[i].dir * g_Bullet[i].speed;
		
		//��ʊO�ɏo�������
		if (!CheckDisp(g_Bullet[i].pos.x, g_Bullet[i].pos.y, BULLET_WIDTH, BULLET_HEIGHT) ||
			CheckHitBG(g_Bullet[i].pos.x, g_Bullet[i].pos.y, BULLET_WIDTH, BULLET_HEIGHT))
		{
			EraseBullet(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_Bullet[i].use)
		{
			continue;
		}
		//�o���b�g�̕`��
		DrawSpriteColorRotateBase(g_TexNo,
			g_Bullet[i].pos.x, g_Bullet[i].pos.y,
			BULLET_WIDTH, BULLET_HEIGHT,
			g_Bullet[i].U, g_Bullet[i].V,
			1.0f, 1.0f,
			g_Bullet[i].color.r, g_Bullet[i].color.g, g_Bullet[i].color.b, g_Bullet[i].color.a,
			g_Bullet[i].rot);
	}
}

//�e��ł�---------------------------------------------------------------------
void SetBullet(float posx, float posy, float dirx, float diry)
{
	//�o���b�g�̔z��𑖍����ē����Ă��Ȃ�����������
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use)
		{
			continue;
		}
		g_Bullet[i].pos.x = posx;
		g_Bullet[i].pos.y = posy;
		g_Bullet[i].dir.x = dirx;
		g_Bullet[i].dir.y = diry;
		g_Bullet[i].rot = atan2f(g_Bullet[i].dir.y, g_Bullet[i].dir.x) + (D3DX_PI / 2);

		g_Bullet[i].use = true;

		//���Đ�
		PlaySound(g_ShotNo, 0);

		break;
	}
}

//Bullet�̈ʒu��n��-----------------------------------------------------------
D3DXVECTOR2 GetBulletPos(int index)
{
	return g_Bullet[index].pos;
}

//Bullet���\������Ă��邩��n��-----------------------------------------------
bool GetBulletUse(int index)
{
	return g_Bullet[index].use;
}

//Bullet������-----------------------------------------------------------------
void EraseBullet(int index)
{
	g_Bullet[index].use = false;
}