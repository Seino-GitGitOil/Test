//-------------------------------------------
//
// 弾の爆発に関する宣言[explosion.h]
// Author：清野蒼翔
//
//-------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_  //二重インクルード防止のマクロ

#include "main.h"

//プロトタイプ宣言
void InitExplosion();                      //弾の初期化処理
void UninitExplosion();                    //弾の終了処理
void UpdateExplosion();                    //弾の更新処理
void DrawExplosion();                      //弾の描画処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);  //爆発のセット処理

#endif
