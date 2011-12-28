#include <Windows.h>
#include <tchar.h>
#define WM_SHOWTASK (WM_USER+150) //自定義的消息名

// == 各種句柄 ======== //
HWND	hWnd = NULL; //handle of Window
// == 靜態的窗口數據 == //
static LPCTSTR lpszAppName = TEXT("測試窗體");
// == 托盤氣泡 ======== //
NOTIFYICONDATA nid;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// = 主函數 =================================================================================== //
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow){
	LPWSTR title   = LPWSTR("");
	LPWSTR message = LPWSTR("");
	LPWSTR iconPath = LPWSTR("");

	LPWSTR *szArgList;
	int argCount = 0;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if ((szArgList != NULL) && (argCount > 4)) {
		iconPath = szArgList[2];
		title    = szArgList[3];
		message  = szArgList[4];
	}

	MSG			msg;	//消息結構體
	WNDCLASS	wc;		//窗體類型結構體

	// 設定窗體數據 //
	wc.style			= CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground	= 0;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= lpszAppName;
	
	// 注冊窗體類型 //
	if(!RegisterClass(&wc)){
		MessageBox(NULL, TEXT("Register Window Class Failed!"), lpszAppName, MB_ICONERROR);
		return 0;
	}

	// 創建窗口 //
	hWnd = CreateWindow(lpszAppName,
						lpszAppName,
						WS_OVERLAPPEDWINDOW,
						100,100,
						250,250,
						NULL,
						NULL,
						hInstance,
						NULL);
	if(hWnd==NULL){return 0;}
	
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 0;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = (HICON)LoadImage(NULL, iconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	wcscpy_s(nid.szTip, title);
	wcscpy_s(nid.szInfo, message);
	wcscpy_s(nid.szInfoTitle, title);
	nid.dwInfoFlags = NIIF_INFO;
	nid.uTimeout = 5000;
	
	Shell_NotifyIconW(NIM_ADD, &nid);

	// 顯示窗口 //
	ShowWindow(hWnd,SW_HIDE);
	UpdateWindow(hWnd);
	
	SetTimer(hWnd, 0, 10000,NULL);
	// 循環信息 //
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	// 釋放處理 //
	Shell_NotifyIconW(NIM_DELETE, &nid);
	
	//   結束   //
	return msg.wParam;
}

// = 回調函數 =================================================================================== //
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_CREATE:
		return 0;
	case WM_TIMER:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,msg,wParam,lParam));
}