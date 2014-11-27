//---------------------------------------------------------------------------
//
// �������ǥХå�
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.24  Phase 0.10    ���󥽡�����
//  2001.08.12  Phase 6.2.1   ���������
//
//---------------------------------------------------------------------------
#ifndef KDB_H
#define KDB_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <map>
using namespace std;
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
#include "tool/kosui_base.h"
//---------------------------------------------------------------------------
// SakuraAPI��
class TKawariInterfaceSakuraAPI : public TKawariInterface_base {
private:

	HWND Target;

public:

	TKawariInterfaceSakuraAPI(HWND target) : Target(target) {}

	// SakuraAPI/1.3 ���٥������
	void SakuraEvent(const string& event,const string& param);

	// ���������
	virtual string GetInformation(void)
	{
		return("UNKNOWN/0.0.0");
	}

	// Ϳ����줿������ץȤ��ᡦ�¹Ԥ���
	virtual string Parse(const string& script)
	{
		SakuraEvent("ShioriEcho",script);
		return("");
	}
};
//---------------------------------------------------------------------------
// ǧ����ǽ��¾�������Ȥ�HWnd�������֤�
void GetGhostList(map<HWND,string> &ghostlist);
//---------------------------------------------------------------------------
#endif

