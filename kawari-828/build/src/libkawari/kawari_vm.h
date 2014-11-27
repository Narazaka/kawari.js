//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
//  ���������۵���(����饤�󥹥���ץȥ��󥸥�)
//
//      Programed by NAKAUE.T (Meister) / Suikyo
//
//  2001.05.24  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//  2001.06.12  Phase 5.3.2   ���ߡ�����ƥ�����
//  2001.06.17  Phase 6.0     ����饤�󥹥���ץ�������򻲾ȤΥХ�����
//  2001.07.21  Phase 6.2     �ؿ����󻲾�
//  2001.08.08  Phase 6.2     �ؿ��ơ��֥뻲��
//  2002.03.10  Phase 7.9.0   kawari_engine_base.h�ѻ�
//                            �����ľ�ܥ��������ػ�
//  2002.03.17                KIU�˹�碌��TKisEngine����TKawariVM��̾���ѹ�
//                            Ʊ����TKawariCode~����TKVMCode~��̾���ѹ�
//  2002.04.18  Phase 8.0.0   ��ľ��
//                            ����ƥ����ȼ���ϼ����
//                            �㳰���֤μ¸�
//
//---------------------------------------------------------------------------
#ifndef KAWARI_VM_H__
#define KAWARI_VM_H__
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_code.h"
#include "libkawari/wordcollection.h"
#include "libkawari/kawari_dict.h"
//---------------------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
#include <climits>
#include <cstdlib>
#include <set>
#include <iostream>
//---------------------------------------------------------------------------

//==========================================================================
//
//  ���������۵���
//  Kawari Virtual Machine  (former TKisEngine)
//
//==========================================================================
class TKawariVM {
public:

	// ���󥿥ץ꥿��������
	struct InterpState {
		enum StateValue {
			NORMAL,
			CONTINUE,
			BREAK,
			RETURN,
			EXCEPTION
		} state;
		std::string mes;
		bool override;
		InterpState (StateValue s, std::string str, bool ovr=true);
	};

private:
	// ������
	class TKawariEngine &engine;

	// ����
	class TNS_KawariDictionary &dictionary;

	// �ؿ��ơ��֥�
	std::map<std::string, class TKisFunction_base *> FunctionTable;

	// �ؿ��ꥹ��
	std::vector<class TKisFunction_base*> FunctionList;

	// ��������
	InterpState state;

	// ����
	class TKawariLogger &logger;
public:
	TKawariVM(class TKawariEngine &e, class TNS_KawariDictionary &d, class TKawariLogger &lgr);

	~TKawariVM();

	// "System.function."
	static const std::string SYSTEM_FUNCTION_PREFIX;

	//=====================================================================
	// ��֥����ɼ¹Ե�ǽ
	//=====================================================================

	// ����������ƥ����Ȥ�������������ɤ�¹Ԥ��롣
	// �¹Ԥ���λ����ȥ���ƥ����Ȥ��˴���
	std::string RunWithNewContext(class TKVMCode_base *code);

	// ���ߤΥ���ƥ����Ⱦ�ǥ����ɤ�¹Ԥ��롣
	// �¹Ԥ���λ��������򻲾ȥ����å���¹����Υݥ���ȤޤǴ����᤹��
	// ���ߤΥ���ƥ����Ȥ��ʤ�����RunWithNewContext()��Ƥ֡�
	std::string RunWithCurrentContext(class TKVMCode_base *code);

	// �ؿ��¹�
	std::string FunctionCall(const std::vector<std::string>& args);

	// �㳰����(continue, break, return, exception)�Ǥ��뤫���Τ�
	bool IsOnException(void) { return (state.state!=InterpState::NORMAL); }

	// �夤�㳰����(break, return, exception)�Ǥ��뤫���Τ�
	bool IsOnWeakException(void) { return (state.state>InterpState::CONTINUE); }

	// �����ͤ�����
	const InterpState &GetState(void) { return state; }

	// ���֤Υ��å�
	void SetState(const InterpState &st) { state=st; }

	// ���֤Υꥻ�å�
	void ResetState(void) { state=InterpState(InterpState::NORMAL, ""); }

	// ������֤Υꥻ�å�
	void ResetState(InterpState::StateValue test) {
		if (state.state==test)
			state=InterpState(InterpState::NORMAL, "");
	}

	//=====================================================================
	// �桼�ƥ���ƥ�
	//=====================================================================

	// ��
	inline class TKawariLogger &GetLogger(void) {
		return logger;
	}

	// ����
	TNS_KawariDictionary &Dictionary(void){
		return dictionary;
	}

	// �ӥ�ȥ��󥳥ޥ�ɤξ��������
	// name : (����)���ޥ��̾
	// info : (����)���ޥ�ɾ���
	// �����(bool) : ¸�ߤ����true
	bool GetFunctionInfo(const std::string &name, struct TKisFunctionInfo &info);

	// �ӥ�ȥ��󥳥ޥ�ɤΥꥹ��
	// list : (����)���ޥ��̾�ꥹ��
	// �����(unsigned int) : ���ޥ�ɤο�
	unsigned int GetFunctionList(std::vector<std::string> &list) const;
};
//---------------------------------------------------------------------------
#endif // KAWARI_VM_H__
