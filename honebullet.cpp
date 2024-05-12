/*==============================================================================

	�o���b�g�Ǘ� [honebullet.cpp]
	Author :20106_�c���@
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "honebullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HONEBULLET_HEIGHT	(30.0f)
#define HONEBULLET_SPEED	(15.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct HONEBULLET {
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		dir;	//�ړ�����
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	bool			use;
};

static HONEBULLET g_Honebullet[HONEBULLET_MAX];

static int g_TexNo = 0;
static int g_SoundNo = 0;

static float g_U = 0.0f;		//U�l�̉���
static float g_V = 0.0f;		//V�l�̍���

//=============================================================================
// ����������
//=============================================================================
void InitHonebullet(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet01.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/e_bullet.wav");
	SetVolume(g_SoundNo, 0.1f);

	//������
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		g_Honebullet[i].pos.x = 0.0f;
		g_Honebullet[i].pos.y = 0.0f;
		g_Honebullet[i].dir.x = 0.0f;
		g_Honebullet[i].dir.y = 0.0f;
		g_Honebullet[i].speed = HONEBULLET_SPEED;
		g_Honebullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Honebullet[i].rot = 0.0f;
		g_Honebullet[i].use = false;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitHonebullet(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHonebullet(void)
{
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (!g_Honebullet[i].use)
		{
			continue;
		}

		//���W�X�V
		g_Honebullet[i].pos += g_Honebullet[i].dir * g_Honebullet[i].speed;
		g_Honebullet[i].rot -= (D3DX_PI * 0.05f);
		
		//��ʊO�ɏo�������
		if (!CheckDisp(g_Honebullet[i].pos.x, g_Honebullet[i].pos.y, HONEBULLET_WIDTH, HONEBULLET_HEIGHT) ||
			CheckHitBG(g_Honebullet[i].pos.x, g_Honebullet[i].pos.y, HONEBULLET_WIDTH, HONEBULLET_HEIGHT))
		{
			EraseHonebullet(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHonebullet(void)
{
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (!g_Honebullet[i].use)
		{
			continue;
		}
		//�o���b�g�̕`��
		DrawSpriteColorRotateBase(g_TexNo,
			g_Honebullet[i].pos.x, g_Honebullet[i].pos.y,
			HONEBULLET_WIDTH, HONEBULLET_HEIGHT,
			g_U, g_V,
			1.0f, 1.0f,
			g_Honebullet[i].color.r, g_Honebullet[i].color.g, g_Honebullet[i].color.b, g_Honebullet[i].color.a,
			g_Honebullet[i].rot);
	}
}

//�e��ł�---------------------------------------------------------------------
void SetHonebullet(float posx, float posy, float dirx, float diry)
{
	//�o���b�g�̔z��𑖍����ē����Ă��Ȃ�����������
	for (int i = 0; i < HONEBULLET_MAX; i++)
	{
		if (g_Honebullet[i].use)
		{
			continue;
		}
		g_Honebullet[i].pos.x = posx;
		g_Honebullet[i].pos.y = posy;
		g_Honebullet[i].dir.x = dirx;
		g_Honebullet[i].dir.y = diry;
		g_Honebullet[i].rot = atan2f(g_Honebullet[i].dir.y, g_Honebullet[i].dir.x) + (D3DX_PI / 2);

		g_Honebullet[i].use = true;

		PlaySound(g_SoundNo, 0);
		break;
	}
}

//Honebullet�̈ʒu��n��-----------------------------------------------------------
D3DXVECTOR2 GetHonebulletPos(int index)
{
	return g_Honebullet[index].pos;
}

//Honebullet���\������Ă��邩��n��-----------------------------------------------
bool GetHonebulletUse(int index)
{
	return g_Honebullet[index].use;
}

//Honebullet������-----------------------------------------------------------------
void EraseHonebullet(int index)
{
	g_Honebullet[index].use = false;
}