//-----------------------------------------
//
// プレイヤー処理の宣言 [player.cpp]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_  //二重インクルード防止のマクロ

#define MAX_PLAYER_TEX_PATTERN (8)   //runningman000プレイヤーのテクスチャのパターン数
#define TEXTURE_WIDTH (50.0f) //テクスチャの横の長さ
#define TEXTURE_HEIGHT (50.0f) //テクスチャの縦の長さ
#define PLAYER_OFFSET (2.0f)  //1フレームあたりのプレイヤーの移動量

//プロトタイプ宣言
void InitPlayer();              //プレイヤーの初期化処理
void UninitPlayer();            //プレイヤーの終了処理
void UpdatePlayer();            //プレイヤーの更新処理
void DrawPlayer();              //プレイヤーの描画処理

#endif