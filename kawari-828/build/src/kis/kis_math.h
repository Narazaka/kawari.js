//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ���� --
//
//      Programed by Kouji.U (SUIKYO)
//
//  2001.06.17  Phase 5.4     
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_rand);
INLINE_SCRIPT_REGIST(KIS_srand);
#else
//---------------------------------------------------------------------------
#ifndef KIS_MATH_H
#define KIS_MATH_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <cstdlib>
using namespace std;
//---------------------------------------------------------------------------
#include "misc/misc.h"
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_rand : public TKisFunction_base {
public:
    // Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="rand";
		Format_="rand MAX";
		Returnval_="(NULL)";
		Information_="return a random number (0 to MAX-1)";

		return(true);
	}

	// ���󥿡��ץ꥿
    virtual string Function(const vector<string>& args){
        if (args.size()>=2) 
            return (IntToString(Random(atoi(args[1].c_str()))));
        else
            return ("");
    }

};
//---------------------------------------------------------------------------
class KIS_srand : public TKisFunction_base {
public:
    // Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="srand";
		Format_="srand SEED";
		Returnval_="(NULL)";
		Information_="give a seed to random number generator";

		return(true);
	}

	// ���󥿡��ץ꥿
    virtual string Function(const vector<string>& args){
		if (args.size()>=2) 
			SRandom(atoi(args[1].c_str()));
		return ("");
    }

};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif
