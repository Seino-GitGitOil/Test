//-------------------------------
//
// 入力処理の宣言[input.h]
// Author：清野蒼翔
//
//-------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);   //キーボード入力の初期化処理
void UninitKeyboard(void);            //キーボード入力の終了処理
void UpdateKeyboard(void);            //キーボード入力の更新処理
bool GetKeyboardPress(int nKey);      //プレス処理
bool GetKeyboardTrigger(int nKey);    //トリガー処理

#endif
