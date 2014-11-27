//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ���ߥ�˥����� --
//
//      Programed by NAKAUE.T (Meister)
//
//  2002.03.18  Phase 7.9.0   �����ߥ�˥����ȵ���
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_matchall);
INLINE_SCRIPT_REGIST(KIS_communicate);
#else
//---------------------------------------------------------------------------
#ifndef KIS_COMMUNICATE_H
#define KIS_COMMUNICATE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_matchall : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="matchall";
		Format_="matchall Str1 Key1 [Key2 ...]";
		Returnval_="bool";
		Information_="return true if all Keywords found in Str1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_communicate : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="communicate";
		Format_="comunicate Index1 [Entry1]";
		Returnval_="sentence";
		Information_="execute all functions in Index1, and call one matched entry";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif

