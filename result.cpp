/*==============================================================================

	�^�C�g����ʊǗ� [result.cpp]
	Author :20106_�c���@
	Date   :23_07_06
--------------------------------------------------------------------------------

==============================================================================*/
#include "result.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "second.h"
#include "scene.h"
#include "fade.h"
#include "clearrunk.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_POLYGON 100	//�\������|���S���̐�

#define ANIME_PTN_YOKO 10
#define ANIME_PTN_TATE 1
#define ANIME_PTN_WAIT 5

#define ANIME_PTN (ANIME_PTN_YOKO * ANIME_PTN_TATE)
#define ANIME_PTN_U (1.0f / ANIME_PTN_YOKO)
#define ANIME_PTN_V (1.0f / ANIME_PTN_TATE)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int			g_TexNo = 0;
static int			g_BackNo = 0;
static int			g_SecondTexNo = 0;

static unsigned int g_ClearTime = 0;

static float		g_U;

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/result01.png");
	g_BackNo = LoadTexture((char*)"data/TEXTURE/resultback.png");
	g_SecondTexNo = LoadTexture((char*)"data/TEXTURE/number000.png");

	g_ClearTime = GetClearTime();
	InitClearrunk();
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	UninitClearrunk();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	//�L�[�{�[�h
	if (GetKeyboardTrigger(DIK_SPACE) ||
		IsButtonTriggered(0, BUTTON_Y))
	{
		//���݃t�F�[�h���łȂ��Ȃ�
		if (GetFadeState() == SCENE_NONE) {
			//�Q�[���V�[���J��
			SetScene(SCENE_TITLE);
		}
	}
	UpdateClearrunk();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	DrawSpriteLeftTop(g_BackNo,
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,//UV�l�̎n�_
		1.0f, 1.0f);

	DrawSpriteLeftTop(g_TexNo,
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f,//UV�l�̎n�_
		1.0f, 1.0f);

	int second = g_ClearTime;
	//���ԕ\��
	for (int i = 0; i < 3; i++)
	{
		//�X�R�A�̕`��
		DrawSpriteColor(g_SecondTexNo,
			1280.0f - (150.0f * i), 465.0f,
			150.0f, 200.0f,
			ANIME_PTN_U * (second % 10), 0.0f,//UV�l�̎n�_
			ANIME_PTN_U, ANIME_PTN_V,
			1.0f, 1.0f, 1.0f, 1.0f);

		second = second / 10;
	}
	DrawClearrunk();
}
