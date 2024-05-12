/*==============================================================================

	�V�[����ʊǗ� [scene.cpp]
	Author :20106_�c���@
	Date   :23_07_06
--------------------------------------------------------------------------------

==============================================================================*/
#include "scene.h"

#include "title.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static SCENE g_Scene = SCENE_NONE;
static SCENE g_NextScene = SCENE_NONE;

//=============================================================================
// ����������
//=============================================================================
void InitScene(void)
{
	//�V�[�����X�V
	g_Scene = g_NextScene;

	switch (g_Scene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	default:
		break;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_RESULT:
		UninitResult();
		break;
	default:
		break;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	default:
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	default:
		break;
	}
}


void SetScene(SCENE next_scene)
{
	//�ڍs��V�[���̐ݒ���s��
	g_NextScene = next_scene;
}

void CheckScene(void)
{
	if (g_Scene != g_NextScene)
	{
		//���ݎ��s���̃V�[�����I��������
		UninitScene();

		//�ڍs��̃V�[����������
		InitScene();
	}
}