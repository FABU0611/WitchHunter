/*==============================================================================

	�o���b�g�Ǘ� [bossexplosion.cpp]
	Author :20106_�c���@
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#include "bossexplosion.h"
#include "collision.h"
#include "sprite.h"
#include "texture.h"
#include "bg.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSSEXPLOSION_SIZE	(700.0f)
#define BOSSEXPLOSION_SPEED	(40.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
struct BOSSEXPLOSION {
	D3DXVECTOR2		pos;	//�ʒu�x�N�g��
	D3DXCOLOR		color;	//���_�J���[
	float			rad;	//���a
	bool			use;

	float			U;		//U�l�̉���
	float			V;		//V�l�̍���
};

static BOSSEXPLOSION g_Bossexplosion;

static int g_TexNo = 0;
static int g_SoundNo = 0;

//=============================================================================
// ����������
//=============================================================================
void InitBossexplosion(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/explosion.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/b_explosion.wav");
	SetVolume(g_SoundNo, 0.2f);

	//������
	g_Bossexplosion.pos.x = 0.0f;
	g_Bossexplosion.pos.y = 0.0f;
	g_Bossexplosion.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Bossexplosion.rad = 0.0f;
	g_Bossexplosion.use = false;
	g_Bossexplosion.U = 0.0f;
	g_Bossexplosion.V = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossexplosion(void)
{
	StopSound(g_SoundNo);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossexplosion(void)
{
	if (!g_Bossexplosion.use) {
		return;
	}

	//�傫���X�V
	if (g_Bossexplosion.rad < BOSSEXPLOSION_SIZE) {
		g_Bossexplosion.rad += BOSSEXPLOSION_SPEED;
	}

	//��ʊO�ɏo�������
	if (g_Bossexplosion.rad >= BOSSEXPLOSION_SIZE) {
		EraseBossexplosion();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossexplosion(void)
{
	if (!g_Bossexplosion.use) {
		return;
	}
	//�o���b�g�̕`��
	DrawSpriteColorBase(g_TexNo,
		g_Bossexplosion.pos.x, g_Bossexplosion.pos.y,
		g_Bossexplosion.rad, g_Bossexplosion.rad,
		g_Bossexplosion.U, g_Bossexplosion.V,
		1.0f, 1.0f,
		g_Bossexplosion.color.r, g_Bossexplosion.color.g, g_Bossexplosion.color.b, g_Bossexplosion.color.a);
}

//�e��ł�---------------------------------------------------------------------
void SetBossexplosion(float posx, float posy){
	if (g_Bossexplosion.use) {
		return;
	}
	g_Bossexplosion.pos.x = posx;
	g_Bossexplosion.pos.y = posy;
	g_Bossexplosion.rad = 50.0f;

	g_Bossexplosion.use = true;

	PlaySound(g_SoundNo, 0);
}

//Bossexplosion�̈ʒu��n��-----------------------------------------------------------
D3DXVECTOR2 GetBossexplosionPos(void){
	return g_Bossexplosion.pos;
}

//Bossexplosion���\������Ă��邩��n��-----------------------------------------------
bool GetBossexplosionUse(void){
	return g_Bossexplosion.use;
}

//Bossexplosion������-----------------------------------------------------------------
void EraseBossexplosion(void){
	g_Bossexplosion.use = false;
}

float GetBossexplosionRad(void) {
	return g_Bossexplosion.rad * 0.5f;
}