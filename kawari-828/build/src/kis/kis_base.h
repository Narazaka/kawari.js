//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript���ܥ��饹
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.14  Phase 0.50    ����饤�󥹥���ץ�����
//  2001.05.24  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//  2002.03.17  Phase 7.9.0   TKisEngine����KIU�˹�碌��TKawariVM��̾���ѹ�
//  2002.03.19  Phase 8.0     ���٤�TKawariEngine���ѹ�
//
//---------------------------------------------------------------------------
#ifndef KIS_BASE_H
#define KIS_BASE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_engine.h"
#include "libkawari/kawari_log.h"
//---------------------------------------------------------------------------
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
// KIS�ؿ��ξ����ɽ����¤��
struct TKisFunctionInfo {
	string name;			// �ؿ�̾
	string format;			// ʸˡ
	string returnval;		// �����
	string information;	// ����
	TKisFunctionInfo (void) {}
	TKisFunctionInfo (const char *n, const char *f, const char *r, const char *i)
		: name(string(n)), format(string(f)),
		  returnval(string(r)), information(string(i)) {}
	TKisFunctionInfo &operator = (const TKisFunctionInfo &r){
		name=r.name;
		format=r.format;
		returnval=r.returnval;
		information=r.information;
		return (*this);
	}
};
//---------------------------------------------------------------------------
// ����饤�󥹥���ץȴؿ��δ��ܥ��饹
// (Th!rd�Υ����ǥ����ե�����)
class TKisFunction_base {
protected:

	// �ؿ�����
	char *Name_;
	char *Format_;
	char *Returnval_;
	char *Information_;

	// ���������󥸥�
	class TKawariEngine *Engine;

	inline TKawariLogger &GetLogger(void) { return Engine->GetLogger(); }
public:

	TKisFunction_base(void)
	{
		Name_="none.";
		Format_="none.";
		Returnval_="none.";
		Information_="none.";
	}

	// �Ǥ��Ȥ餯��
	virtual ~TKisFunction_base() { }

	// �ؿ��������
	const TKisFunctionInfo GetInformation (void) const {
		return TKisFunctionInfo(Name_, Format_, Returnval_, Information_);
	}

	const char* Name(void) const {return(Name_);}
	const char* Format(void) const {return(Format_);}
	const char* Returnval(void) const {return(Returnval_);}
	const char* Information(void) const {return(Information_);}

	// ���������󥸥�����
	void NotifyEngine(class TKawariEngine *engine)
	{
		Engine=engine;
	}

	// �����
	virtual bool Init(void)=0;

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)=0;

	// �������ꥨ�顼�Υ�����
	bool AssertArgument(const vector<string>& args, const unsigned int min){
		using namespace kawari_log;
		bool ret=true;
		if(args.size()<min){
			if(GetLogger().Check(LOG_WARNING))
				GetLogger().GetStream() << "KIS[" << args[0] << "] error : too few arguments." << endl;
			ret=false;
		}
		if ((!ret)&&(GetLogger().Check(LOG_INFO)))
			GetLogger().GetStream() << "usage> " << Format() << endl;
		return ret;
	}

	// �������ꥨ�顼�Υ�����
	bool AssertArgument(const vector<string>& args, const unsigned int min, const unsigned int max){
		using namespace kawari_log;
		bool ret=true;
		if(args.size()<min){
			if(GetLogger().Check(LOG_WARNING))
				GetLogger().GetStream() << "KIS[" << args[0] << "] error : too few arguments." << endl;
			ret=false;
		}else if(args.size()>max){
			if(GetLogger().Check(LOG_WARNING))
				GetLogger().GetStream() << "KIS[" << args[0] << "] error : too many arguments." << endl;
			ret=false;
		}
		if ((!ret)&&(GetLogger().Check(LOG_INFO)))
			GetLogger().GetStream() << "usage> " << Format() << endl;
		return ret;
	}
};
//---------------------------------------------------------------------------
#if 0
class KIS_SampleFunction : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="SampleFunction";
		Format_="SampleFunction [Arg1 ...]";
		Returnval_="string which join all argument";
		Information_="KawariInlineScript sample";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		string ret;
		for(unsigned int i=0;i<args;i++) ret=ret+args[i]+" ";
		return(ret);
	}
};
// �������Ͽ
INLINE_SCRIPT_REGIST(KIS_SampleFunction);
#endif
//---------------------------------------------------------------------------
#endif

