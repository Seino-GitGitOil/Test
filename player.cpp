//---------------------------------------
//
// プレイヤー(自機)の処理[player.cpp]
// Author：清野蒼翔
//
//---------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;       //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;  //頂点バッファへのポインタ
int g_nCounterAnimPlayer;      //アニメーションカウンター
int g_nPatternAnimPlayer;      //アニメーションパターンNo.
D3DXVECTOR3 g_posPlayer;       //位置
D3DXVECTOR3 g_movePlayer;      //移動量
D3DXVECTOR3 g_rotPlayer;       //向き
float g_fLengthPlayer;         //対角線の長さ
float g_fAnglePlayer;          //対角線の角度

//----------------------------------------
// プレイヤーの初期化処理
//----------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;  //デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//runningman000.png",
		&g_pTexturePlayer);

	g_nCounterAnimPlayer = 0;   //カウンターを初期化する
	g_nPatternAnimPlayer = 0;   //パターンNo.を初期化する

	//ポリゴンの位置を初期化
	g_posPlayer = D3DXVECTOR3(640.0f, 600.0f, 0.0f);   //ポリゴンの位置を初期化する
	g_movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      //ポリゴンの移動量を初期化する
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //向きを初期化

	//対角線の長さを算出
	g_fLengthPlayer = sqrtf((TEXTURE_WIDTH * 2) * (TEXTURE_WIDTH * 2) + (TEXTURE_HEIGHT * 2) * (TEXTURE_HEIGHT * 2)) * 0.5f;  //  √幅^2 ＋ 高さ^2 ÷２ = 対角線の半分
																																//対角線の角度を算出
	g_fAnglePlayer = atan2f((TEXTURE_WIDTH * 2), (TEXTURE_HEIGHT * 2));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* 4, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;  //頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報処理
	
	//頂点座標の設定
	/*  改良前の処理
	pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - TEXTURE_WIDTH, g_posPlayer.y - TEXTURE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + TEXTURE_WIDTH, g_posPlayer.y - TEXTURE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - TEXTURE_WIDTH, g_posPlayer.y + TEXTURE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + TEXTURE_WIDTH, g_posPlayer.y + TEXTURE_HEIGHT, 0.0f);
	*/
	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);    //１÷テクスチャの分割数
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------------
// プレイヤーの終了処理
//-------------------------------
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------------------
// プレイヤーの更新処理
//-------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	if (GetKeyboardPress(DIK_SPACE) == true)
	{//SPACEキーが押された場合
		//弾の設定
		SetBullet(D3DXVECTOR3(g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI) * g_fLengthPlayer,g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI) * g_fLengthPlayer, 0),  //プレイヤーの位置
			D3DXVECTOR3(sinf(g_rotPlayer.z + D3DX_PI) * 20.0f, cosf(g_rotPlayer.z + D3DX_PI) * 20.0f, 0.0f));      //弾の移動量
	}

	if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
	{// Aキーが押されたら分岐する
		if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
		{// 同時にWキーが押された場合
			g_movePlayer.x += sinf(-D3DX_PI * 0.75) * PLAYER_OFFSET;
			g_movePlayer.y += cosf(-D3DX_PI * 0.75) * PLAYER_OFFSET;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
		{// 同時にSキーが押された場合
			g_movePlayer.x += sinf(-D3DX_PI * 0.25) * PLAYER_OFFSET;
			g_movePlayer.y += cosf(-D3DX_PI * 0.25) * PLAYER_OFFSET;
		}
		else
		{// 左キーのみが押された場合
			g_movePlayer.x += -PLAYER_OFFSET;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
	{// Dキーが押されたら分岐する

		if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
		{// 同時にWキーが押された場合
			g_movePlayer.x += sinf(D3DX_PI * 0.75) * PLAYER_OFFSET;
			g_movePlayer.y += cosf(D3DX_PI * 0.75) * PLAYER_OFFSET;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
		{// 同時にSキーが押された場合
			g_movePlayer.x += sinf(D3DX_PI * 0.25) * PLAYER_OFFSET;
			g_movePlayer.y += cosf(D3DX_PI * 0.25) * PLAYER_OFFSET;
		}
		else
		{// Dキーのみが押されている場合
			g_movePlayer.x += PLAYER_OFFSET;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
	{// Wキーが押された
		g_movePlayer.y += -PLAYER_OFFSET;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
	{// Sキーが押された
		g_movePlayer.y += PLAYER_OFFSET;
	}
	
	if (GetKeyboardPress(DIK_E) == true)
	{// Eキーが押された
		g_rotPlayer.z += -0.2f;   //右方向への回転量
	}
	else if(GetKeyboardPress(DIK_Q) == true)
	{// Qキーが押された
		g_rotPlayer.z += 0.2f;  //左方向への回転量
	}
	

	//端を越えた場合
	if (g_posPlayer.x - TEXTURE_WIDTH <= 0)
	{
		g_posPlayer.x = TEXTURE_WIDTH;
		/*
		g_movePlayer.x *= -1;          //この文増やすとなんかすっごく壁にバウンドするぞ
		*/
	}
	if (g_posPlayer.x + TEXTURE_WIDTH >= SCREEN_WIDTH)
	{
		g_posPlayer.x = SCREEN_WIDTH - TEXTURE_WIDTH;
		/*
		g_movePlayer.x *= -1;
		*/
	}
	
	if (g_posPlayer.y - TEXTURE_HEIGHT <= 0)
	{
		g_posPlayer.y = TEXTURE_HEIGHT;
		/*
		g_movePlayer.y *= -1;
		*/
	}
	if (g_posPlayer.y + TEXTURE_HEIGHT >= SCREEN_HEIGHT)
	{
		g_posPlayer.y = SCREEN_HEIGHT - TEXTURE_HEIGHT;
		/*
		g_movePlayer.y *= -1;
		*/
	}


	g_nCounterAnimPlayer++; //カウンターを加算
	if ((g_nCounterAnimPlayer % 6) == 0)
	{//一定時間経過
		g_nCounterAnimPlayer = 0;
		g_nPatternAnimPlayer++;

		//パターンNo.を更新
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % MAX_PLAYER_TEX_PATTERN;

		              //テクスチャ座標の更新
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimPlayer * 0.125f, 0.0f);          //　x,yに該当する値はテクスチャを変えた際に必ず変更すること(0.125にしてるが実際に作るときは1.0/パターン数とかにしてもいいかも)
		pVtx[1].tex = D3DXVECTOR2((g_nPatternAnimPlayer + 1) * 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimPlayer * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_nPatternAnimPlayer + 1) * 0.125f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}

	//位置を更新
	g_posPlayer.x += g_movePlayer.x;
	g_posPlayer.y += g_movePlayer.y;


	//移動量を更新
	g_movePlayer.x += (0.0f - g_movePlayer.x) * 0.2f;      // 減速する処理　　　　式：(目的の値 - 現在の値) * 減衰係数
	g_movePlayer.y += (0.0f - g_movePlayer.y) * 0.2f;      // 減速する処理　　   減衰係数は少ない方がよりツルツル滑るぞ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	/*
	pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - TEXTURE_EXTENT, g_posPlayer.y - TEXTURE_EXTENT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + TEXTURE_EXTENT, g_posPlayer.y - TEXTURE_EXTENT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - TEXTURE_EXTENT, g_posPlayer.y + TEXTURE_EXTENT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + TEXTURE_EXTENT, g_posPlayer.y + TEXTURE_EXTENT, 0.0f);
	*/

	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------------
// プレイヤーの描画処理
//-------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}