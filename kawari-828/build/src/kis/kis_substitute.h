//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- toupper, tolower --
//
//      Programed by Chikara.H (MDR)
//
//  2001.09.04  created(�����kis_escape.h�����Ҽ�)
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_toupper);
INLINE_SCRIPT_REGIST(KIS_tolower);
#else
//---------------------------------------------------------------------------
#ifndef KIS_SUBSTITUTE_H
#define KIS_SUBSTITUTE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_toupper : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="toupper";
		Format_="toupper Word1 ...";
		Returnval_="Word1 ...";
		Information_="replace a lower character into an upper character in all arguments";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_tolower : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="tolower";
		Format_="tolower Word1 ...";
		Returnval_="Word1 ...";
		Information_="replace an upper character into a lower character in all arguments";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif

