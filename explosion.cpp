//--------------------------------
//
// 爆発の処理[explosion.cpp]
// Author：清野蒼翔
//
//--------------------------------

#include "main.h"
#include "explosion.h"


//マクロ定義
#define MAX_EXPLOSION (128)  // 爆発の最大数
#define MAX_EXPLOSION_TEX_PATTERN (8) //爆発アニメーションのパターン数    画像変えたら適宜変更すること
#define TEXTURE_EXTENT (50.0f)  //テクスチャの半径

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;    //位置
	D3DXCOLOR col;      //色
	int nCounterAnim;   //アニメーションカウンター
	int nPatternAnim;   //アニメーションパターンNo.
	bool bUse;          //使用しているかどうか
}Explosion;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;    //頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];               //爆発の情報

//----------------------------
//  爆発の初期化処理
//----------------------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;
	VERTEX_2D* pVtx;

	//デバイスの取得
	pDevice = GetDevice();
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//explosion000.png",
		&g_pTextureExplosion);

	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0, 0.0, 0.0, 0.0);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;   //カウンターを初期化する
		g_aExplosion[nCntExplosion].nPatternAnim = 0;   //パターンNo.を初期化する
		g_aExplosion[nCntExplosion].bUse = false;       //使用していない状態(false)にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_EXPLOSION
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);
	
	//頂点情報の設定
	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20, g_aExplosion[nCntExplosion].pos.y - 20, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20, g_aExplosion[nCntExplosion].pos.y - 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20, g_aExplosion[nCntExplosion].pos.y + 20, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20, g_aExplosion[nCntExplosion].pos.y + 20, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

}

//----------------------------
//  爆発の終了処理
//----------------------------
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//----------------------------
//  爆発の更新処理
//----------------------------
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている

			//カウンタを進めて、パターンNo.を更新する
			g_aExplosion[nCntExplosion].nCounterAnim++;
			
			if ((g_aExplosion[nCntExplosion].nCounterAnim % 6) == 0)
			{//一定時間経過

				g_aExplosion[nCntExplosion].nCounterAnim = 0;
				g_aExplosion[nCntExplosion].nPatternAnim++;
				/*
				//パターンNo.を更新
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % MAX_EXPLOSION_TEX_PATTERN;
				*/
				if (g_aExplosion[nCntExplosion].nPatternAnim >= MAX_EXPLOSION_TEX_PATTERN)
				{//総パターン数を超えた場合
					g_aExplosion[nCntExplosion].bUse = false;   //使用していない状態にする
				}
				else if (g_aExplosion[nCntExplosion].nPatternAnim < MAX_EXPLOSION_TEX_PATTERN)
				{//総パターン数に満たない場合


							 //テクスチャ座標の更新

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);          //　x,yに該当する値はテクスチャを変えた際に必ず変更すること(0.125にしてるが実際に作るときは1.0/パターン数とかにしてもいいかも)
					pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1) * 0.125f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1) * 0.125f, 1.0f);

					
				}
			}
		}
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//----------------------------
//  爆発の描画処理
//----------------------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);      //描画を開始する頂点インデックス
		}
	}

}

//----------------------------
//  爆発の設定処理
//----------------------------
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていない場合
			//爆発の情報の設定
			g_aExplosion[nCntExplosion].nCounterAnim = 0;    //カウンターの初期化
			g_aExplosion[nCntExplosion].nPatternAnim = 0;    //パターン数の初期化

			            //頂点情報の設定
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - TEXTURE_EXTENT, pos.y - TEXTURE_EXTENT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + TEXTURE_EXTENT, pos.y - TEXTURE_EXTENT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - TEXTURE_EXTENT, pos.y + TEXTURE_EXTENT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + TEXTURE_EXTENT, pos.y + TEXTURE_EXTENT, 0.0f);

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

			g_aExplosion[nCntExplosion].bUse = true;   //使用している状態にする
			break;
		}
		pVtx += 4;        //頂点データのポインタを４つ分進める

	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}