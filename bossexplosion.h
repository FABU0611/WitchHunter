/*==============================================================================

	�o���b�g�Ǘ� [bossexplosion.h]
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

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBossexplosion(void);
void UninitBossexplosion(void);
void UpdateBossexplosion(void);
void DrawBossexplosion(void);

void SetBossexplosion(float posx, float posy);
D3DXVECTOR2 GetBossexplosionPos(void);
bool GetBossexplosionUse(void);
void EraseBossexplosion(void);
float GetBossexplosionRad(void);