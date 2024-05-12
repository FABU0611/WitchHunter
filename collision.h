/*==============================================================================

	�����蔻��Ǘ� [collision.h]
	Author :20106_�c���@
	Date   :23_06_13
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
void InitCollision(void);
void UninitCollision(void);
void UpdateCollision(void);
void DrawCollision(void);

bool CheckHitBCSq(float ccla_x, float ccla_y, float ccla_r,
	float cclb_x, float cclb_y, float cclb_r);
bool CheckDisp(float objx, float objy, float objw, float objh);
bool CheckLoadRight(float objx, float objy, float objw, float objh);

void ToPlayer(void);