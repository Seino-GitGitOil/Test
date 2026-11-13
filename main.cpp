//---------------------------------------------------
//
// シューティング：メイン処理    敵作って当たり判定作る前  [main.cpp]
// Author：清野蒼翔
//
//---------------------------------------------------
#include "main.h"
#include "player.h"
#include "BG.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"

//マクロ定義
#define CLASS_NAME     "WindowsClass"                      // ウインドウクラスの名前
#define WINDOW_NAME  "シューティング：デバッグ用"          // ウインドウの名前（キャプションに表示）
#define ID_BUTTON_FINISH (101)               //終了ボタンのID
#define ID_BUTTON_COPY   (102)               //コピーボタンのID
#define ID_EDIT_INPUT    (111)               //入力ウインドウのID
#define ID_EDIT_OUTPUT   (121)               //出力ウインドウのID

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);    // 初期化処理
void Update(void);    // 更新処理
void Draw(void);      // 描画処理

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;               
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;   

bool g_bPause = false; //ポーズ状態のON/OFF
bool g_bDispDebug = false; //デバッグ表示のON/OFF

//------------------------------
//        メイン関数            
//------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex =      //　wcex・・・ウインドウクラスの構造体
	{
		sizeof(WNDCLASSEX),                 //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                         //ウインドウのスタイル
		WindowProc,                         //ウインドウプロシージャ
		0,                                  //０にする(通常は使用しない)
		0,                                  //０にする(通常は使用しない)
		hInstance,                          //インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),    //タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),        //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),         //クライアント領域の背景色
		NULL,                               //メニューバー
		CLASS_NAME,                         //ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)     //ファイルのアイコン
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };   //画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウ生成
	hWnd = CreateWindowEx(0,              //拡張ウインドウスタイル
		CLASS_NAME,                       //ウインドウクラスの名前
		WINDOW_NAME,                      //ウインドウの名前
		WS_OVERLAPPEDWINDOW,              //ウインドウスタイル
		CW_USEDEFAULT,                    //ウインドウの左上X座標
		CW_USEDEFAULT,                    //ウインドウの左上Y座標
		(rect.right - rect.left),         //ウインドウの幅
		(rect.bottom - rect.top),         //ウインドウの高さ
		NULL,                             //親ウインドウのハンドル
		NULL,                             //メニューハンドルまたは子ウインドウID
		hInstance,                        //インスタンスハンドル
		NULL);                            //ウインドウ作成データ

	DWORD dwCurrentTime;   //現在時刻
	DWORD dwExecLastTime;  //最後に処理した時刻

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;                  //初期化する
	dwExecLastTime = timeGetTime();     //現在時刻を取得
	

	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理
		return -1;
	}

//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//ウインドウズの処理
			if (msg.message == WM_QUIT)
			{//WM_メッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();   //現在時刻を取得
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の１秒経過
				dwExecLastTime = dwCurrentTime;  //処理開始の時刻 [現在時刻] を保存
				//更新処理
				Update();
				//描画処理
				Draw();
			}
		}
	}
	/*下記を上記に書き換えた
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		//メッセージの設定
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	*/

	//終了処理
	Uninit();
	
	//分解能を戻す
	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}
//-------------------------------
// ウインドウプロシージャ
//-------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN: //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);   //既定の処理を返す
};

//-------------------------------
// 初期化処理
//-------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	//各種オブジェクトの初期化処理をここに
	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);         //テクスチャの繰り返しの設定(U)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);         //テクスチャの繰り返しの設定(V)



	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	
	//背景の初期化処理
	InitBG();

	//プレイヤーの初期化処理
	InitPlayer();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//プレイヤー
	return S_OK;
}




//-------------------------------
// 終了処理
//-------------------------------
void Uninit(void)
{
	

	//キーボードの更新処理
	UninitKeyboard();

	//背景の終了処理
	UninitBG();

	//プレイヤーの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}


//-------------------------------
// 更新処理
//-------------------------------
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	if (g_bPause == false)
	{//ポーズ状態がOFFの時なら

		//背景の更新処理
		UpdateBG();

		//ここに各オブジェクトの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateExplosion();
	}

	if (GetKeyboardTrigger(DIK_P) == true)
	{// Pキーが押された
		g_bPause = g_bPause ^ 1;
	}
}

//-------------------------------
// 描画処理
//-------------------------------
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//各種描画処理を以下に追加すること
		
		//背景の描画処理
		DrawBG();

		//プレイヤーの描画処理
		DrawPlayer();

		//弾の描画処理
		DrawBullet();

		//爆発の初期化処理
		DrawExplosion();

		//描画の終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------
// デバイスの取得
//--------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}