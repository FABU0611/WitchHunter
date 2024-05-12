/*==============================================================================

	�`���[�W�o���b�g�Ǘ� [chargebullet.cpp]
	Author :20106_�c���@
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#include "chargebullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHARGEBULLET_WIDTH	(80.0f)
#define CHARGEBULLET_HEIGHT	(80.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct CHARGEBULLET {
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		dir;	//�ړ�����
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	bool			use;
	float			U;		//U�l�̉���
	float			V;		//V�l�̍���
};

static CHARGEBULLET g_ChargeBullet[CHARGEBULLET_MAX];

static int g_TexNo = 0;

static int g_ChargeNo = 0;

//=============================================================================
// ����������
//=============================================================================
void InitChargeBullet(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/enemy00.png");

	//�T�E���h�̓ǂݍ���
	g_ChargeNo = LoadSound((char*)"data/sound/p_charge.wav");
	SetVolume(g_ChargeNo, 0.3f);

	//������
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		g_ChargeBullet[i].pos.x = 0.0f;
		g_ChargeBullet[i].pos.y = 0.0f;
		g_ChargeBullet[i].dir.x = 0.0f;
		g_ChargeBullet[i].dir.y = 0.0f;
		g_ChargeBullet[i].speed = 30.0f;
		g_ChargeBullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_ChargeBullet[i].rot = 0.0f;
		g_ChargeBullet[i].use = false;
		g_ChargeBullet[i].U = 0.0f;
		g_ChargeBullet[i].V = 0.0f;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitChargeBullet(void)
{
	StopSound(g_ChargeNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateChargeBullet(void)
{
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (!g_ChargeBullet[i].use)
		{
			continue;
		}

		//���W�X�V
		g_ChargeBullet[i].pos += g_ChargeBullet[i].dir * g_ChargeBullet[i].speed * 1.2f;
		
		//��ʊO�ɏo�������
		if (!CheckDisp(g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y, CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT) ||
			CheckHitBG(g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y, CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT))
		{
			EraseChargeBullet(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawChargeBullet(void)
{
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (!g_ChargeBullet[i].use)
		{
			continue;
		}
		//�o���b�g�̕`��
		DrawSpriteColorRotateBase(g_TexNo,
			g_ChargeBullet[i].pos.x, g_ChargeBullet[i].pos.y,
			CHARGEBULLET_WIDTH, CHARGEBULLET_HEIGHT,
			g_ChargeBullet[i].U, g_ChargeBullet[i].V,
			1.0f, 1.0f,
			g_ChargeBullet[i].color.r, g_ChargeBullet[i].color.g, g_ChargeBullet[i].color.b, g_ChargeBullet[i].color.a,
			g_ChargeBullet[i].rot);
	}
}

//�e��ł�---------------------------------------------------------------------
void SetChargeBullet(float posx, float posy, float dirx, float diry)
{
	//�o���b�g�̔z��𑖍����ē����Ă��Ȃ�����������
	for (int i = 0; i < CHARGEBULLET_MAX; i++)
	{
		if (g_ChargeBullet[i].use)
		{
			continue;
		}
		g_ChargeBullet[i].pos.x = posx;
		g_ChargeBullet[i].pos.y = posy;
		g_ChargeBullet[i].dir.x = dirx;
		g_ChargeBullet[i].dir.y = diry;
		g_ChargeBullet[i].rot = atan2f(g_ChargeBullet[i].dir.y, g_ChargeBullet[i].dir.x) + (D3DX_PI / 2);

		g_ChargeBullet[i].use = true;

		//���Đ�
		PlaySound(g_ChargeNo, 0);

		break;
	}
}

//ChargeBullet�̈ʒu��n��-----------------------------------------------------------
D3DXVECTOR2 GetChargeBulletPos(int index)
{
	return g_ChargeBullet[index].pos;
}

//ChargeBullet���\������Ă��邩��n��-----------------------------------------------
bool GetChargeBulletUse(int index)
{
	return g_ChargeBullet[index].use;
}

//ChargeBullet������-----------------------------------------------------------------
void EraseChargeBullet(int index)
{
	g_ChargeBullet[index].use = false;
}