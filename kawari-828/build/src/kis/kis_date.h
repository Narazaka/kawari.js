//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ���� --
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.14  Phase 0.50    First version
//  2001.05.19  Phase 5.1     ��������
//  2005.06.21  Phase 8.2.3   mktime�ɲ� (���ȡ�)
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_date);
INLINE_SCRIPT_REGIST(KIS_mktime);
#else
//---------------------------------------------------------------------------
#ifndef KIS_DATE_H
#define KIS_DATE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_date : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="date";
		Format_="date Word1";
		Returnval_="date information";
		Information_="return a date info string formated by Word1(%y,%m,%d,%H,%M,%S,%w)";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_mktime : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="mktime";
		Format_="mktime year month day hour min sec";
		Returnval_="date information";
		Information_="return progress seconds from 1970/1/1 0:00:00(UTC)";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif

