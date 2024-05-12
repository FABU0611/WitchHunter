/*==============================================================================

	�{�X�Ǘ� [boss.h]
	Author :20106_�c���@
	Date   :23_09_08
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_CHECK_SIZE	(110.0f)
#define BOSS_RAD		(BOSS_CHECK_SIZE * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void SetBoss(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetBossPos(void);
bool GetBossLive(void);
bool GetFightState(void);
void SetFightState(bool fight);
void DieBoss(void);

void HitBulletforBoss(void);
void HitChrageBulletforBoss(void);
void HitSwordforBoss(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************