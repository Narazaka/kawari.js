//---------------------------------------------------------------------------
//
// KOSUI - Direct SSTP Adapter
//
//      Programed by Suikyo.
//
//  2002.05.04  Phase 8.0.0   FirstVersion
//
//---------------------------------------------------------------------------
#ifndef KOSUI_DSSTP_H
#define KOSUI_DSSTP_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------
class TKosuiDSSTPInterface : public TKawariInterface_base {
public:
	// ���󥹥ȥ饯��
	// hwnd : DA�Υ�����ɥ��ϥ�ɥ�
	// event : ���Ѥ��륤�٥��̾
	TKosuiDSSTPInterface (HWND hwnd, const string& evt);

	// �ǥ��ȥ饯��
	virtual ~TKosuiDSSTPInterface();

	// ���������
	virtual std::string GetInformation(void);

	// Ϳ����줿���������ᡦ�¹Ԥ���
	virtual std::string Parse(const std::string& script);

private:
	// �ꥯ�����ȥ�����
	string event;

	// DA�Υ�����ɥ��ϥ�ɥ�
	HWND dahwnd;

	// DSSTP�˻Ȥ�������¦�Υ��ߡ�������ɥ��ϥ�ɥ�
	HWND dumhwnd;
};
//---------------------------------------------------------------------------
#endif // KOSUI_DSSTP_H
