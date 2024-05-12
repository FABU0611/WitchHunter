/*==============================================================================

	�o���b�g�Ǘ� [honebullet.h]
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
#define HONEBULLET_MAX	(10)
#define HONEBULLET_WIDTH	(30.0f)
#define HONEBULLET_RAD	(HONEBULLET_WIDTH * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitHonebullet(void);
void UninitHonebullet(void);
void UpdateHonebullet(void);
void DrawHonebullet(void);

void SetHonebullet(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetHonebulletPos(int index);
bool GetHonebulletUse(int index);
void EraseHonebullet(int index);