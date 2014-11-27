//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- split --
//
//      Programed by ���ȡ�/NAKAUE.T
//
//  2002.01.07  Phase 7.3     findpos���֥��ޥ���ɲ� (���ȡ�)
//                            split���ޥ���ɲ�(���ȡ�)
//  2002.03.17  Phase 7.9.0   7.9�λ��ͤ˹�碌�� (NAKAUE.T)
//                            split���ʤ�expr�ȤޤȤ���Ƥ��뤫����˶줷��Τǰ�ư
//                            split�����ѹ�
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_split);
INLINE_SCRIPT_REGIST(KIS_join);
#else
//---------------------------------------------------------------------------
#ifndef KIS_SPLIT_H__
#define KIS_SPLIT_H__
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_split : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void){
		Name_="split";
		Format_="split Entry1 string delimiter";
		Returnval_="(NULL)";
		Information_="split a string by delimiter";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_join : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void){
		Name_="join";
		Format_="join Entry1 delimiter";
		Returnval_="(NULL)";
		Information_="join all strings in Entry1 by delimiter";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif // KIS_SPLIT_H__
//---------------------------------------------------------------------------
#endif

