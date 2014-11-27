//---------------------------------------------------------------------------
//
// KOSUI - Direct SSTP Adapter
//
//      Programed by Suikyo.
//
//  2002.05.04  Phase 8.0.0   FirstVersion
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <process.h>
//---------------------------------------------------------------------------
#include "tool/kosui_base.h"
#include "tool/kosui_dsstp.h"
#include "misc/phttp.h"
//---------------------------------------------------------------------------
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
#if defined(__CYGWIN__)
static void MsgLoopThread(void *);
DWORD WINAPI ThreadWrapper(void *arg){
	MsgLoopThread(arg);
	return 1;
}
unsigned long _beginthread(void (*start_address)(void *), unsigned stack_size, void *arglist)
{
	DWORD thid;
	::CreateThread(NULL, stack_size, &ThreadWrapper, arglist, 0, &thid);
	return thid;
}
void _endthread (void)
{
	::ExitThread(0);
}
#endif
//---------------------------------------------------------------------------
namespace{
	const unsigned int DIRECT_SSTP_PORT=9801;

	// ����ͤ�ͭ�����ɤ���
	bool isResponseValid=false;

	// ����ͼ����Ϥ����
	string response;

	// �ߥ塼�ƥå������֥�������
	HANDLE mutex;
}
//---------------------------------------------------------------------------
static LRESULT CALLBACK MainWndProc(
		HWND hwnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
{
	COPYDATASTRUCT* pcds;
	switch(message){
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COPYDATA:
		pcds=(COPYDATASTRUCT*)lParam;
		if((pcds!=NULL)&&(pcds->dwData==DIRECT_SSTP_PORT)){
			string tmpstr((LPCSTR)pcds->lpData, 0, (DWORD)pcds->cbData);
			if(::WaitForSingleObject(mutex, 1000)!=WAIT_TIMEOUT){
				response=tmpstr;
				isResponseValid=true;
				::ReleaseMutex(mutex);
			}else{
				// ����
				isResponseValid=true;
			}
			return TRUE;
		}else{
			return FALSE;
		}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0L;
}
//---------------------------------------------------------------------------
static void MsgLoopThread(void *){
	MSG msg;

	/* ��å������������ä�ȯ�Ԥ���롼�� */
	while( GetMessage(&msg, NULL, 0, 0) ){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_endthread();
}
//---------------------------------------------------------------------------
// ���󥹥ȥ饯��
// hwnd : DA�Υ�����ɥ��ϥ�ɥ�
TKosuiDSSTPInterface::TKosuiDSSTPInterface (HWND hwnd, const string& evt)
	: event(evt), dahwnd(hwnd), dumhwnd(0) {
	WNDCLASS wndclass;
	memset(&wndclass, 0, sizeof(WNDCLASS));

	char *szClassName="KOSUI dummy window";
	char *szAppName="KOSUI";

	// ���ߤΥ��󥹥��󥹤�����
	HINSTANCE hInstance=(HINSTANCE)::GetModuleHandle(NULL);

	// ���ߡ��Υ�����ɥ�����
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	RegisterClass(&wndclass);
	dumhwnd = CreateWindow(
			szClassName, szAppName,
			WS_POPUP,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			NULL,NULL, hInstance, NULL);

	// Mutex Object�����
	mutex=::CreateMutex(NULL, FALSE, NULL);

	// ��å������롼�ץ���åɤγ���
	_beginthread(&MsgLoopThread, 0, NULL);
}
//---------------------------------------------------------------------------
// �ǥ��ȥ饯��
TKosuiDSSTPInterface::~TKosuiDSSTPInterface(){
	// ������ɥ��κ��
	DestroyWindow(dumhwnd);
	// �ߥ塼�ƥå������֥������Ȥκ��
	::CloseHandle(mutex);
}
//---------------------------------------------------------------------------
// ���������
string TKosuiDSSTPInterface::GetInformation(void){
	return "Can not get Information.(Direct SSTP mode)";
}
//---------------------------------------------------------------------------
// Ϳ����줿���������ᡦ�¹Ԥ���
string TKosuiDSSTPInterface::Parse(const string& script){
	// ���ܹ�������� �ʰ�SSTP���饤����� �򻲹�
	TPHMessage request;

	char szHWnd[100];
	sprintf(szHWnd, "%d", (unsigned int)dumhwnd);

	request.SetStartline("NOTIFY SSTP/1.0");
	request["Charset"]="Shift_JIS";
	request["Sender"]="KOSUI";
	request["Event"]=event;
	request["HWnd"]=szHWnd;
	request["Reference0"]=script;

	string reqmes=request.Serialize();

	COPYDATASTRUCT cds;
	cds.dwData = DIRECT_SSTP_PORT;
	cds.cbData = reqmes.size();
	cds.lpData = (LPVOID)reqmes.c_str();

	DWORD dwret;
	::SendMessageTimeout(
		dahwnd, WM_COPYDATA, (WPARAM)dumhwnd,
		(LPARAM)&cds, SMTO_ABORTIFHUNG, 1000, &dwret);

	string respmes="";

	if((::WaitForSingleObject(mutex, 1000)!=WAIT_TIMEOUT)&&isResponseValid){
		respmes=response;
		isResponseValid=false;
		::ReleaseMutex(mutex);
	}
	return respmes;
}
//---------------------------------------------------------------------------
