/*==============================================================================

	�v���C���[�Ǘ� [player.cpp]
	Author :20106_�c���@
	Date   :23_06_09
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "boss.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "bullet.h"
#include "chargebullet.h"
#include "sword.h"
#include "camera.h"
#include "bg.h"
#include "life.h"
#include "collision.h"

#include "scene.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_SP		(0.7f)
#define PLAYER_WIDTH	(90.0f)	//�e�N�X�`���̉���
#define PLAYER_HEIGHT	(90.0f)	//�e�N�X�`���̏c��
#define PLAYER_CHECK_BLOCK	(20.0f)	//�e�N�X�`���̏c��

#define PLAYER_X_MAX	(MAP_X_MAX + (MAP_SCREAN * SIZE) - (PLAYER_WIDTH * 0.5f))
#define PLAYER_Y_MAX	(0.0f + (PLAYER_WIDTH * 0.5f))

#define RIGHT_MAX		(1000.0f)	//�s���ł���E�[
#define LEFT_MAX		(400.0f)	//�s���ł��鍶�[

#define ANIME_SP	(10)			//�A�j���[�V�����̑��x �l���傫���ƒx���Ȃ�
#define ANIME_PTN_YOKO (3)			//���̃A�j���p�^�[��
#define ANIME_PTN_TATE (4)			//�c�̃A�j���p�^�[��
#define ANIME_PTN (ANIME_PTN_YOKO * ANIME_PTN_TATE)	//���ׂẴA�j���p�^�[��

#define PTN_WIDTH	(1.0f / ANIME_PTN_YOKO)	//��l���̉���
#define PTN_HEIGHT	(1.0f / ANIME_PTN_TATE)	//��l���̏c��

#define JUMP_P (25.0f)
#define GRAVITY	(5.0f)
#define SLOW_DOWN	(-1.0f)

#define CHARGE_TIME		(50)

#define PLAYER_CAMERABASE_X	(SCREEN_WIDTH * 0.3f)
#define PLAYER_CAMERABASE_Y	(SCREEN_HEIGHT * 0.5f)

#define NOHITFRAME	(90)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetChargeColor(void);
void RemoveColor(void);
void ErasePlayer(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//�T�E���h�p
enum SOUND_NUM {
	maxcharge,
	jump,
	hit,
	dead,

	sound_max,
};
enum BGM_NUM {
	fight,
	boss,

	bgm_max,
};

struct PLAYER{
	D3DXVECTOR2		pos;		//�ʒu�x�N�g��
	D3DXVECTOR2		GHitpos;	//���n����悤�̈ʒu
	D3DXVECTOR2		HHitpos;	//���������Ă��锻��悤�̈ʒu
	D3DXVECTOR2		dir;		//���x�x�N�g��
	float			speed;
	D3DXCOLOR		color;		//���_�J���[
	float			rot;

	float			U = 0.0f;		//U�l�̉���
	float			V = 0.0f;		//V�l�̍���

	bool			isUse;
	bool			isDead;
	int				NoHitFrame;
	bool			isHit;
	int				FrashFrame;

	bool			isCharge;		//�`���[�W��Ԃ�
	bool			ChargeSound;

	float			JumpPos;		//�W�����v�J�n�n�_
	float			JumpHeight;		//�ő�W�����v�n�_
	float			Fall;			//�W�����v�p
	bool			isGround;		//���n����
	bool			isJump;			//�W�����v������
	bool			isPressJump;	//�W�����v�{�^������������
	bool			isHead;			//���Ԃ�������
	bool			isSlowDown;
	int				SlowCount;

	bool			FightBoss;


	DIRECTION		direction;	//����

	int				Life;
};

static PLAYER g_Player;

static int		g_TexNo = 0;

static int		g_SoundNo[sound_max];		//�T�E���h
static int		g_BgmNo[bgm_max];		//�T�E���h

static float	Gravity = 0.0f;		//�d��

static int g_AnimeSkipFrame = 0;	//�t���[��������p
static int g_AnimePattern = 0;		//���t���[���Ɉ�񐔂���
static int g_Anime = 0;				//�A�j���[�V�����p
static int g_AnimeWalk[4] = { 0,1,2,1 };	//�Y��ɕ�����悤��

static int g_ChargeFrame = 0;

static int				g_Dead_frame;

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/majo.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo[maxcharge] = LoadSound((char*)"data/sound/p_chargemax.wav");
	SetVolume(g_SoundNo[maxcharge], 0.2f);
	g_SoundNo[jump] = LoadSound((char*)"data/sound/p_jump.wav");
	SetVolume(g_SoundNo[jump], 0.2f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/p_hit.wav");
	SetVolume(g_SoundNo[hit], 0.5f);
	g_SoundNo[dead] = LoadSound((char*)"data/sound/p_dead.wav");
	SetVolume(g_SoundNo[dead], 0.2f);

	g_BgmNo[fight] = LoadSound((char*)"data/sound/bgm_fight.wav");
	SetVolume(g_BgmNo[fight], 0.2f);
	PlaySound(g_BgmNo[fight], 1);
	g_BgmNo[boss] = LoadSound((char*)"data/sound/bgm_boss.wav");
	SetVolume(g_BgmNo[boss], 0.2f);

	//������
	g_Player.pos.x = LEFT_MAX + PLAYER_WIDTH;
	g_Player.pos.y = SCREEN_HEIGHT / 2;
	g_Player.GHitpos.x = g_Player.pos.x;
	g_Player.GHitpos.y = g_Player.pos.y + (PLAYER_HEIGHT * 0.5f);
	g_Player.HHitpos.x = g_Player.pos.x;
	g_Player.HHitpos.y = g_Player.pos.y - (PLAYER_HEIGHT);
	g_Player.dir.x = 0.0f;
	g_Player.dir.y = 0.0f;
	g_Player.speed = NUM_SP;
	g_Player.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Player.rot = 0.0f;

	g_Player.isUse = true;
	g_Player.isDead = false;
	g_Player.NoHitFrame = 0;
	g_Player.isHit = false;
	g_Player.FrashFrame = 0;

	g_Player.isCharge = false;
	g_Player.ChargeSound = false;

	g_Player.Fall = 0.0f;
	g_Player.isGround = false;
	g_Player.isJump = false;
	g_Player.isPressJump = false;
	g_Player.isHead = false;
	g_Player.isSlowDown = false;
	g_Player.SlowCount = 0;

	g_Player.JumpPos = 0.0f;
	g_Player.JumpHeight = PLAYER_HEIGHT * 3;

	g_Player.direction = right;

	g_Player.U = PTN_WIDTH * 1;
	g_Player.V = PTN_HEIGHT * 0;

	g_Player.FightBoss = false;

	g_Player.Life = PLAYER_LIFE;

	Gravity = 0.0f;

	g_AnimeSkipFrame = 0;
	g_AnimePattern = 0;
	g_Anime = 0;

	g_ChargeFrame = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < sound_max; i++) {
		StopSound(g_SoundNo[i]);
	}
	for (int i = 0; i < bgm_max; i++) {
		StopSound(g_BgmNo[i]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (!g_Player.isUse) {
		return;
	}
	//�����Ă���Ԃ�������
	if (!g_Player.isDead) {
		//�������ĂȂ��Ȃ瓖���蔻�菈��
		if (!g_Player.isHit) {
			ToPlayer();
		}

		//�n�ʂ̏�ɂ��邩
		g_Player.GHitpos.x = g_Player.pos.x;
		g_Player.GHitpos.y = g_Player.pos.y + (PLAYER_HEIGHT * 0.5f);
		//�����������Ă��邩
		g_Player.HHitpos.x = g_Player.pos.x;
		g_Player.HHitpos.y = g_Player.pos.y - (PLAYER_HEIGHT);

		Gravity = GRAVITY;

		//�A�j���[�V����---------------------------------------------------------------------
		g_AnimeSkipFrame++;					//�t���[���𐔂���
		if (g_AnimeSkipFrame >= ANIME_SP) {
			g_AnimeSkipFrame = 0;			//0�ɖ߂�

			g_AnimePattern++;				//20�t���[���Ɉ�񐔂���
			g_Anime = g_AnimeWalk[g_AnimePattern % 4];
			if (g_AnimePattern > ANIME_PTN_YOKO) {
				g_AnimePattern = 0;
			}
		}


		//�v���C���[�ړ��i�L�[�{�[�h�A�p�b�h�j----------------------------------------------------------
		if (GetKeyboardPress(DIK_A) ||
			IsButtonPressed(0, BUTTON_LEFT)) {	//��	
			g_Player.direction = left;
			g_Player.speed = NUM_SP * 10;
			g_Player.U = PTN_WIDTH * g_Anime;
		}
		else if (GetKeyboardPress(DIK_D) ||
			IsButtonPressed(0, BUTTON_RIGHT)) {	//�E
			g_Player.direction = right;
			g_Player.speed = NUM_SP * 10;
			g_Player.U = PTN_WIDTH * g_Anime;
		}
		else {									//�ړ��L�[�������Ă��Ȃ��Ƃ������~�܂��Ă���G
			g_Player.U = PTN_WIDTH * 1;
			g_Player.speed = 0.0f;
		}


		//�����Ă�������ŊG��ς���
		if (g_Player.direction == left) {
			g_Player.V = PTN_HEIGHT * 1;
			g_Player.dir.x = -1.0f;
			g_Player.dir.y = 0.0f;
		}
		if (g_Player.direction == right) {
			g_Player.V = PTN_HEIGHT * 2;
			g_Player.dir.x = 1.0f;
			g_Player.dir.y = 0.0f;
		}


		//�e��ł�------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_J) ||
			IsButtonPressed(0, BUTTON_Y)) {
			//�`���[�W�J�n
			g_ChargeFrame++;
			if (g_ChargeFrame >= CHARGE_TIME) {
				g_Player.isCharge = true;
			}

			//�`���[�W���ĂȂ��Ȃ�ꔭ��������
			if (GetKeyboardTrigger(DIK_J) ||
				IsButtonTriggered(0, BUTTON_Y)) {
				//��������Ă���
				if (g_Player.direction == up) {
					SetBullet(g_Player.pos.x, g_Player.pos.y - (PLAYER_HEIGHT * 0.5f), g_Player.dir.x, g_Player.dir.y);
				}
				//���������Ă���
				if (g_Player.direction == down) {
					SetBullet(g_Player.pos.x, g_Player.pos.y + (PLAYER_HEIGHT * 0.5f), g_Player.dir.x, g_Player.dir.y);
				}
				//���������Ă���
				if (g_Player.direction == left) {
					SetBullet(g_Player.pos.x - (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
				}
				//�E�������Ă���
				if (g_Player.direction == right) {
					SetBullet(g_Player.pos.x + (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
				}
				//��������0�ɖ߂�
				g_ChargeFrame = 0;
			}
		}
		//�`���[�W�V���b�g--------------------------------------------------------------------
		if (GetKeyboardRelease(DIK_J) &&
			g_Player.isCharge ||
			IsButtonPressed(0, BUTTON_Y) &&
			g_Player.isCharge) {
			//���������Ă���
			if (g_Player.direction == left) {
				SetChargeBullet(g_Player.pos.x - (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//�E�������Ă���
			if (g_Player.direction == right) {
				SetChargeBullet(g_Player.pos.x + (PLAYER_HEIGHT * 0.5f), g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//��������0�ɖ߂�
			g_ChargeFrame = 0;
			g_Player.isCharge = false;
		}

		//�`���[�W�������̐F�ύX����
		if (g_Player.isCharge) {
			SetChargeColor();
			if (!g_Player.ChargeSound) {
				//���Đ�
				PlaySound(g_SoundNo[maxcharge], 0);
				g_Player.ChargeSound = true;
			}
		}
		else {
			RemoveColor();
			g_Player.ChargeSound = false;
		}


		//����U��------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, BUTTON_X)) {
			//���������Ă���
			if (g_Player.direction == left) {
				//����U�����Ƃ������_�b�V��������
				SetSword(g_Player.pos.x, g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
			//�E�������Ă���
			if (g_Player.direction == right) {
				SetSword(g_Player.pos.x, g_Player.pos.y, g_Player.dir.x, g_Player.dir.y);
			}
		}


		//�v���C���[�̍s���͈�------------------------------------------------------------
		//��	��ʊO�ɍs���Ȃ��悤��
		if (g_Player.pos.y > SCREEN_HEIGHT - (PLAYER_WIDTH / 2)) {
			g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_WIDTH / 2);
			g_Player.isGround = true;
		}
		//��
		else if (g_Player.pos.y < (PLAYER_WIDTH / 2)) {
			g_Player.pos.y = (PLAYER_WIDTH / 2);
		}
		//��
		if (g_Player.pos.x + g_Player.dir.x * g_Player.speed < PLAYER_Y_MAX) {
			g_Player.dir.x = 0.0f;
		}
		//�[�ɂ��Ȃ��Ȃ�߂�
		else if (g_Player.pos.x + g_Player.dir.x * g_Player.speed > PLAYER_X_MAX) {
			g_Player.dir.x = 0.0f;
		}

		//�}�b�v�Ƃ̓����蔻��
		//��
		if (CheckHitBG(g_Player.pos.x + g_Player.dir.x * g_Player.speed, g_Player.pos.y,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_SIZE)) {
			g_Player.dir.x = 0.0f;
		}
		//�u���b�N�ɒ��n���Ă��邩
		if (CheckHitBG(g_Player.GHitpos.x, g_Player.GHitpos.y - g_Player.Fall,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_BLOCK)) {
			g_Player.isGround = true;
		}
		else {
			g_Player.isGround = false;
		}
		//�u���b�N�Ɠ����������Ă��邩����
		if (CheckHitBG(g_Player.HHitpos.x, g_Player.HHitpos.y - g_Player.Fall,
			PLAYER_CHECK_SIZE, PLAYER_CHECK_BLOCK)) {
			g_Player.isHead = true;
		}
		else {
			g_Player.isHead = false;
		}

		//���W�X�V
		g_Player.pos.x += g_Player.dir.x * g_Player.speed;


		//�W�����v����----------------------------------------------------------------------
		if (g_Player.isGround) {
			if (GetKeyboardTrigger(DIK_K) ||
				IsButtonTriggered(0, BUTTON_A)) {
				//�W�����v�J�n�n�_���擾
				g_Player.JumpPos = g_Player.pos.y;
				//�W�����v�L�[�������ꂽ��W�����v���ɂ���
				g_Player.isJump = true;
				//�������ł���񂵂��W�����v���Ȃ��悤��
				g_Player.isPressJump = true;
				//�T�E���h�Đ�
				PlaySound(g_SoundNo[jump], 0);
			}
			else {
				//��������W�����v���ł͂Ȃ�
				g_Player.isJump = false;
			}
		}
		if (g_Player.isJump &&
			g_Player.isPressJump) {
			//�W�����v�ō����B�_��艺�ɂ���ԏ㏸����
			if (GetKeyboardPress(DIK_K) &&
				(g_Player.JumpPos - g_Player.JumpHeight) < g_Player.pos.y &&
				!g_Player.isHead ||
				IsButtonPressed(0, BUTTON_A) &&
				(g_Player.JumpPos - g_Player.JumpHeight) < g_Player.pos.y &&
				!g_Player.isHead) {
				//�W�����v���ɃL�[�������ꂽ�珈���J�n
				g_Player.Fall = JUMP_P;
				//�󒆂ɍs���̂Œ��n��false
				g_Player.isGround = false;
				g_Player.SlowCount = 0;
			}
			else {
				//�L�[�������ꂽ��W�����v���ł͂Ȃ�
				g_Player.isJump = false;
			}
		}
		else {
			//�W�����v���ɃW�����v�{�^���ł�����藎����
			if (GetKeyboardTrigger(DIK_K) ||
				IsButtonTriggered(0, BUTTON_A)) {
				g_Player.isSlowDown = true;
			}
			if (GetKeyboardPress(DIK_K) &&
				g_Player.isSlowDown ||
				IsButtonPressed(0, BUTTON_A) &&
				g_Player.isSlowDown) {
				g_Player.SlowCount++;
				g_Player.Fall = SLOW_DOWN;
				if (g_Player.SlowCount > 30) {
					g_Player.isSlowDown = false;
					g_Player.SlowCount = 0;
				}
			}
			//�W�����v���łȂ��Ȃ痎����
			g_Player.Fall -= Gravity;
		}

		//���n���Ă���Ƃ��͗����Ȃ�
		if (g_Player.isGround) {
			g_Player.Fall = 0.0f;
			Gravity = 0.0f;
			g_Player.isPressJump = false;
		}
		//�󒆂ɂ���Ƃ������Ă���
		else {
			g_Player.pos.y -= g_Player.Fall;
			//�󒆂ŗ������ςȂ��̊G�ɂ��Ȃ�
			if (g_Player.direction == left) {
				g_Player.U = PTN_WIDTH * 2;
			}
			if (g_Player.direction == right) {
				g_Player.U = PTN_WIDTH * 0;
			}
		}


		//�J�����̍��W�X�V
		D3DXVECTOR2 cameraPos;
		//��ʍ��[
		cameraPos.x = g_Player.pos.x - PLAYER_CAMERABASE_X;
		if (cameraPos.x < 0.0f) {
			cameraPos.x = 0.0f;
		}
		//�{�X�����ŃJ�����Œ�
		if (g_Player.FightBoss) {
			cameraPos.x = MAP_X_MAX;
		}
		cameraPos.y = g_Player.pos.y - PLAYER_CAMERABASE_Y;
		//��ʉE�[
		if (cameraPos.x > MAP_X_MAX &&
			!g_Player.FightBoss) {
			g_Player.FightBoss = true;
			//�{�X��키��Ԃɂ���
			SetFightState(true);
			StopSound(g_BgmNo[fight]);
			PlaySound(g_BgmNo[boss], 1);
		}
		//��
		if (cameraPos.y < 60.0f) {
			cameraPos.y = 60.0f;
		}
		//��
		if (cameraPos.y > 60.0f) {
			cameraPos.y = 60.0f;
		}
		SetCameraPos(cameraPos);
	}


	//���G����---------------------------------------------------------------------------
	if (g_Player.isHit && !g_Player.isDead) {
		EraseLife(g_Player.Life - 1);
		//���G���Ԃ𐔂���
		g_Player.NoHitFrame++;
		//�U���ɓ����������̓_�ŏ���
		g_Player.FrashFrame++;
		g_Player.color.a = 1.0f;
		if (g_Player.FrashFrame == 20) {
			g_Player.color.a = 0.0f;
			g_Player.FrashFrame = 0;
		}
	}
	//���G���ԏI��
	if (g_Player.NoHitFrame >= NOHITFRAME) {
		g_Player.color.a = 1.0f;
		g_Player.isHit = false;
		g_Player.NoHitFrame = 0;
	}

	//���S����---------------------------------------------------------------------------
	if (g_Player.Life <= 0) {
		g_Player.isDead = true;
	}
	if (g_Player.isDead) {
		//�������������Ă���G
		g_Player.V = (1.0f / 4) * 0;
		if (g_Dead_frame == 0) {
			//SE�Đ�
			PlaySound(g_SoundNo[dead], 0);
			g_Dead_frame++;
		}
		//��]���Ȃ�������Ă���
		else if (g_Dead_frame > 0) {
			g_Player.rot += 0.1f;
			g_Player.color.a -= 0.008f;
			//�����ɂȂ�����
		}
		if (g_Player.color.a <= 0.0f) {
			ErasePlayer();
			SetScene(SCENE_TITLE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (!g_Player.isUse) {
		return;
	}
	//�v���C���[�̕`��
	DrawSpriteColorRotateBase(g_TexNo,
		g_Player.pos.x, g_Player.pos.y,
		PLAYER_WIDTH, PLAYER_HEIGHT,
		g_Player.U, g_Player.V,
		PTN_WIDTH, PTN_HEIGHT,
		g_Player.color.r, g_Player.color.g, g_Player.color.b, g_Player.color.a,
		g_Player.rot);
}

//�v���C���[�̈ʒu��n��-------------------------------------------------------
D3DXVECTOR2 GetPlayerPos(void){
	return g_Player.pos;
}

//�`���[�W�������̐F
void SetChargeColor(void){
	g_Player.color = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
}

//�ʏ�̐F
void RemoveColor(void){
	g_Player.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

bool GetFightBoss(void) {
	return g_Player.FightBoss;
}

bool GetPlayerDead(void) {
	return g_Player.isDead;
}

void HitPlayer(void) {
	g_Player.Life--;
}
void HitPlayerforBoss(void) {
	g_Player.Life = g_Player.Life - 2;
}

void SetPlayerisHit(void) {
	g_Player.isHit = true;
	PlaySound(g_SoundNo[hit], 0);
}
//���C�t���擾
int GetLife(void) {
	return g_Player.Life;
}

//�v���C���[���g��Ȃ���Ԃ�
void ErasePlayer(void) {
	g_Player.isUse = false;
}