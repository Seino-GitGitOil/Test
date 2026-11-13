//-------------------------------------------
//
// 弾の処理に関する宣言[bullet.h]
// Author：清野蒼翔
//
//-------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_  //二重インクルード防止のマクロ

//プロトタイプ宣言
void InitBullet();                      //弾の初期化処理
void UninitBullet();                    //弾の終了処理
void UpdateBullet();                    //弾の更新処理
void DrawBullet();                      //弾の描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);  //弾のセット処理

#endif