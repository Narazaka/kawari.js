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
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_vm.h"
#include "libkawari/kawari_engine.h"
#include "libkawari/kawari_compiler.h"
#include "libkawari/kawari_code.h"
#include "libkawari/kawari_dict.h"
#include "libkawari/kawari_rc.h"
#include "kis/kis_config.h"
#include "kis/kis_base.h"
#include "misc/misc.h"
using namespace kawari::resource;
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
//---------------------------------------------------------------------------
// �ؿ�����Ͽ���륨��ȥ�Υץ�ե��å���
const string TKawariVM::SYSTEM_FUNCTION_PREFIX="System.Function.";

//---------------------------------------------------------------------------
// ���󥿥ץ꥿�ξ���
TKawariVM::InterpState::InterpState (TKawariVM::InterpState::StateValue s, string str, bool ovr) : state(s), mes(str), override(ovr) {}
//---------------------------------------------------------------------------
// ���󥹥ȥ饯��
TKawariVM::TKawariVM(TKawariEngine &eng, TNS_KawariDictionary &d, TKawariLogger &lgr)
	 : engine(eng), dictionary(d),
	   state(InterpState(TKawariVM::InterpState::NORMAL, "")), logger(lgr) {
	#define INLINE_SCRIPT_REGIST(T) \
	{ \
		TKisFunction_base *func=new T; \
		func->NotifyEngine(&engine); \
		if(func->Init()) { \
			FunctionTable[func->Name()]=func; \
			FunctionList.push_back(func); \
		} else delete func; \
	}

	#include "kis/kis_config.h"

	#undef INLINE_SCRIPT_REGIST
}

//---------------------------------------------------------------------------
// �ǥ��ȥ饯��
TKawariVM::~TKawariVM(){
	for (vector<TKisFunction_base *>::iterator it=FunctionList.begin();it!=FunctionList.end();it++)
		if (*it)
			delete (*it);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �����ɼ¹Է�
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����������ƥ����Ȥ�������������ɤ�¹Ԥ��롣
// �¹Ԥ���λ����ȥ���ƥ����Ȥ��˴���
string TKawariVM::RunWithNewContext(class TKVMCode_base *code){
	if (!code) return "";
	dictionary.CreateContext();
	string retstr=code->Run(*this);
	dictionary.DeleteContext();

	// Return���֤β��
	if (GetState().state==InterpState::RETURN){
		if (GetState().mes.size())
			retstr=GetState().mes;
	}

	// ���֤Υꥻ�å�
	ResetState();

	return retstr;
}
//---------------------------------------------------------------------------
// ���ߤΥ���ƥ����Ⱦ�ǥ����ɤ�¹Ԥ��롣
// �¹Ԥ���λ��������򻲾ȥ����å���¹����Υݥ���ȤޤǴ����᤹��
// ���ߤΥ���ƥ����Ȥ��ʤ�����RunWithNewContext()��Ƥ֡�
string TKawariVM::RunWithCurrentContext(TKVMCode_base *code){
	if (dictionary.GetContextStackDepth()){
		unsigned int frame=dictionary.LinkFrame();
		string retstr=code->Run(*this);
		dictionary.UnlinkFrame(frame);
		return retstr;
	}else{
		// ����ƥ����Ȥ�����ޤ���
		return RunWithNewContext(code);
	}
}
//---------------------------------------------------------------------------
// �ؿ��¹�
string TKawariVM::FunctionCall(const vector<string>& args){
	if (!args[0].size()) return ("");
	// ��Ƭ��'.'��������϶����ӥ�ȥ��󥳥ޥ�ɸƤӽФ�
	if (args[0][0]=='.') {
		string comname=args[0].substr(1);
		if (FunctionTable.count(comname))
			return (FunctionTable[comname]->Function(args));
	}else{
		TEntry entry=dictionary.GetEntry(SYSTEM_FUNCTION_PREFIX+args[0]);
		// �桼������ؿ���ͥ��
		if (entry.IsValid()&&entry.Size()){
			// �ǽ��ñ�����ΤȤ���
			TWordID id=entry.Index();
			if (!id) return "";
			TKVMCode_base *code=dictionary.GetWordFromID(id);

			// ����ƥ����Ⱥ���
			dictionary.CreateContext();

			// ������Ǽ
			if (args.size()){
				TEntry argentry=dictionary.CreateEntry("@arg");
				for(unsigned int i=0; i<args.size(); i++){
					argentry.Push(
						dictionary.CreateWord(
							TKawariCompiler::CompileAsString(args[i])));
				}
			}
			// �����ɼ¹�
			string retstr=code->Run(*this);

			// ����ƥ����Ⱥ��
			dictionary.DeleteContext();

			// Return���֤β��
			if (GetState().state==InterpState::RETURN){
				if (GetState().override)
					retstr=GetState().mes;
			}
			ResetState();

			return retstr;
		}else if(FunctionTable.count(args[0])){
			// �ӥ�ȥ���ؿ�
			return (FunctionTable[args[0]]->Function(args));
		}
	}
	logger.GetStream(kawari_log::LOG_ERROR) << RC.S(ERR_VM_UNDEFINED_FUNCTION1) << args[0] << RC.S(ERR_VM_UNDEFINED_FUNCTION2) << endl;
	return "";
}
//---------------------------------------------------------------------------
// �ӥ�ȥ���ؿ��ξ��������
bool TKawariVM::GetFunctionInfo(const string &name, TKisFunctionInfo &info){
	if (FunctionTable.count(name)){
		info=FunctionTable[name]->GetInformation();
		return true;
	}else{
		return false;
	}
}
//---------------------------------------------------------------------------
// �ӥ�ȥ���ؿ��Υꥹ��
unsigned int TKawariVM::GetFunctionList(vector<string> &list) const{
	for (vector<TKisFunction_base*>::const_iterator it=FunctionList.begin();it!=FunctionList.end();it++)
		list.push_back(string((*it)->Name()));
	return FunctionList.size();
}
//---------------------------------------------------------------------------
