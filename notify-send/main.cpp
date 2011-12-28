#include <Windows.h>
#include <tchar.h>
#define WM_SHOWTASK (WM_USER+150) //�Զ��x����Ϣ��

// == ���N��� ======== //
HWND	hWnd = NULL; //handle of Window
// == �o�B�Ĵ��ڔ��� == //
static LPCTSTR lpszAppName = TEXT("�yԇ���w");
// == �бP���� ======== //
NOTIFYICONDATA nid;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// = ������ =================================================================================== //
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

	MSG			msg;	//��Ϣ�Y���w
	WNDCLASS	wc;		//���w��ͽY���w

	// �O�����w���� //
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
	
	// ע�Դ��w��� //
	if(!RegisterClass(&wc)){
		MessageBox(NULL, TEXT("Register Window Class Failed!"), lpszAppName, MB_ICONERROR);
		return 0;
	}

	// �������� //
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

	// �@ʾ���� //
	ShowWindow(hWnd,SW_HIDE);
	UpdateWindow(hWnd);
	
	SetTimer(hWnd, 0, 10000,NULL);
	// ѭ�h��Ϣ //
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	// ጷ�̎�� //
	Shell_NotifyIconW(NIM_DELETE, &nid);
	
	//   �Y��   //
	return msg.wParam;
}

// = ���{���� =================================================================================== //
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