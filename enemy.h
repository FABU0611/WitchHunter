/*==============================================================================

	�G�l�~�[�Ǘ� [enemy.h]
	Author :20106_�c���@
	Date   :23_06_09
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MAX	(5)
#define ENEMY_WIDTH		(100.0f)	//�e�N�X�`���̉���
#define ENEMY_HEIGHT	(100.0f)	//�e�N�X�`���̏c��
#define ENEMY_RAD		(ENEMY_WIDTH * 0.5f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(float posx, float posy, float dirx, float diry);
D3DXVECTOR2 GetEnemyPos(int index);
bool GetEnemyState(int index);
void DieEnemy(int index);

void HitBulletforEnemy(int idx);
void HitChrageBulletforEnemy(int idx);
int GetEnemyLife(int index);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************