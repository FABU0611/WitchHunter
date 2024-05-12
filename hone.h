/*==============================================================================

	�G�l�~�[�Ǘ� [hone.h]
	Author :20106_�c���@
	Date   :23_09_07
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HONE_MAX	(5)
#define HONE_WIDTH		(110.0f)	//�e�N�X�`���̉���
#define HONE_HEIGHT		(110.0f)	//�e�N�X�`���̏c��
#define HONE_RAD		(HONE_WIDTH * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitHone(void);
void UninitHone(void);
void UpdateHone(void);
void DrawHone(void);

void SetHone(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetHonePos(int index);
bool GetHoneLive(int index);
void DieHone(int index);
int GetHoneLife(int index);

void HitBulletforHone(int idx);
void HitChrageBulletforHone(int idx);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************