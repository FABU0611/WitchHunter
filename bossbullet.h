/*==============================================================================

	�o���b�g�Ǘ� [bossbullet.h]
	Author :20106_�c���@
	Date   :23_09_14
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSSBULLET_MAX	(3)
#define BOSSBULLET_RAD	(BOSSBULLET_WIDTH * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBossbullet(void);
void UninitBossbullet(void);
void UpdateBossbullet(void);
void DrawBossbullet(void);

void SetBossbullet(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetBossbulletPos(int index);
bool GetBossbulletUse(int index);
void EraseBossbullet(int index);