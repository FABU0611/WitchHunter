/*==============================================================================

	サウンド処理 [sound.h]
	Author :20106_田中蓮
	Date   :23_07_07
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include <windows.h>
#include "xaudio2.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);

int LoadSound(char* pFilename);
void PlaySound(int index, int loopCount);
void StopSound(int index);
void StopSoundAll(void);
void SetVolume(int index, float vol);

