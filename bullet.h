/*==============================================================================

	�o���b�g�Ǘ� [bullet.h]
	Author :20106_�c���@
	Date   :23_06_15
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_MAX	(3)
#define BULLET_WIDTH	(30.0f)
#define BULLET_RAD	(BULLET_WIDTH * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetBulletPos(int index);
bool GetBulletUse(int index);
void EraseBullet(int index);