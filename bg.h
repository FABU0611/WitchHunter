/*==============================================================================

	���_�Ǘ�[bg.h]
	Author :20106_�c���@
	Date   :23_07_06
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SIZE	(96.0f)
#define MAP_SCREAN	(20)
#define SCREAN_NUM	(5)
#define MAP_X_MAX	(SIZE * (MAP_SCREAN * (SCREAN_NUM -1)))


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);
void DrawBgObj(void);

void BgSetEnemy(void);
void BgSetHone(void);
void BgSetBoss(void);


bool CheckHitBG(float x, float y, float width, float height);