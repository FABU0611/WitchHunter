/*==============================================================================

	�o���b�g�Ǘ� [bossbullet.cpp]
	Author :20106_�c���@
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "bossbullet.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSSBULLET_WIDTH	(50.0f)
#define BOSSBULLET_HEIGHT	(50.0f)
#define BOSSBULLET_SPEED	(15.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct BOSSBULLET {
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXVECTOR2		dir;	//�ړ�����
	float			speed;	//���x
	D3DXCOLOR		color;	//���_�J���[
	float			rot;
	bool			use;
};

static BOSSBULLET g_Bossbullet[BOSSBULLET_MAX];

static int g_TexNo = 0;
static int g_SoundNo = 0;

static float g_U = 0.0f;		//U�l�̉���
static float g_V = 0.0f;		//V�l�̍���

//=============================================================================
// ����������
//=============================================================================
void InitBossbullet(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet00.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/b_bullet.wav");
	SetVolume(g_SoundNo, 0.2f);

	//������
	for (int i = 0; i < BOSSBULLET_MAX; i++)	{
		g_Bossbullet[i].pos.x = 0.0f;
		g_Bossbullet[i].pos.y = 0.0f;
		g_Bossbullet[i].dir.x = 0.0f;
		g_Bossbullet[i].dir.y = 0.0f;
		g_Bossbullet[i].speed = BOSSBULLET_SPEED;
		g_Bossbullet[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Bossbullet[i].rot = 0.0f;
		g_Bossbullet[i].use = false;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossbullet(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossbullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++)
	{
		if (!g_Bossbullet[i].use)		{
			continue;
		}

		//���W�X�V
		g_Bossbullet[i].pos += g_Bossbullet[i].dir * g_Bossbullet[i].speed;
		
		//��ʊO�ɏo�������
		if (!CheckDisp(g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y, BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT) ||
			CheckHitBG(g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y, BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT))		{
			EraseBossbullet(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossbullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++)
	{
		if (!g_Bossbullet[i].use)
		{
			continue;
		}
		//�o���b�g�̕`��
		DrawSpriteColorRotateBase(g_TexNo,
			g_Bossbullet[i].pos.x, g_Bossbullet[i].pos.y,
			BOSSBULLET_WIDTH, BOSSBULLET_HEIGHT,
			g_U, g_V,
			1.0f, 1.0f,
			g_Bossbullet[i].color.r, g_Bossbullet[i].color.g, g_Bossbullet[i].color.b, g_Bossbullet[i].color.a,
			g_Bossbullet[i].rot);
	}
}

//�e��ł�---------------------------------------------------------------------
void SetBossbullet(float posx, float posy, float dirx, float diry)
{
	//�o���b�g�̔z��𑖍����ē����Ă��Ȃ�����������
	for (int i = 0; i < BOSSBULLET_MAX; i++)	{
		if (g_Bossbullet[i].use)		{
			continue;
		}
		g_Bossbullet[i].pos.x = posx;
		g_Bossbullet[i].pos.y = posy;
		g_Bossbullet[i].dir.x = dirx;
		g_Bossbullet[i].dir.y = diry;
		g_Bossbullet[i].rot = atan2f(g_Bossbullet[i].dir.y, g_Bossbullet[i].dir.x) + (D3DX_PI / 2);

		g_Bossbullet[i].use = true;

		PlaySound(g_SoundNo, 0);
		break;
	}
}

//Bossbullet�̈ʒu��n��-----------------------------------------------------------
D3DXVECTOR2 GetBossbulletPos(int index){
	return g_Bossbullet[index].pos;
}

//Bossbullet���\������Ă��邩��n��-----------------------------------------------
bool GetBossbulletUse(int index){
	return g_Bossbullet[index].use;
}

//Bossbullet������-----------------------------------------------------------------
void EraseBossbullet(int index){
	g_Bossbullet[index].use = false;
}