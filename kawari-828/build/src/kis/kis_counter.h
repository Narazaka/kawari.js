//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ������ --
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.30  Phase 5.1     First version
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_inc);
INLINE_SCRIPT_REGIST(KIS_dec);
#else
//---------------------------------------------------------------------------
#ifndef KIS_COUNTER_H
#define KIS_COUNTER_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_inc : public TKisFunction_base {
protected:

	virtual string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="inc";
		Format_="inc Index1 increment upperbound";
		Returnval_="(NULL)";
		Information_="increase a value of Index1 in an increment";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_dec : public KIS_inc {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="dec";
		Format_="dec Index1 decrement lowerbound";
		Returnval_="(NULL)";
		Information_="decrease a value of Index1 in a decrement";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif

