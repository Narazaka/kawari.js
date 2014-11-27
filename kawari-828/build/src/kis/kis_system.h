//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- �����ƥ���� --
//
//      Programed by Suikyo.
//
//  2002.04.20  Phase 8.0.0   ����kawari.ini�ˤ��ä����ޥ�ɤΰ���
//  2002.06.23  Phase 8.0.0   KIS_rccharset �ɲ�
//  2002.11.20  Phase 8.1.0   �����ȥ꡼��򥨥󥸥�ñ�̤Ǵ���
//  2003.11.18  Phase 8.2.0   getenv�ɲ�
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_securitylevel);
INLINE_SCRIPT_REGIST(KIS_logfile);
INLINE_SCRIPT_REGIST(KIS_loglevel);
INLINE_SCRIPT_REGIST(KIS_debugger);
INLINE_SCRIPT_REGIST(KIS_rccharset);
INLINE_SCRIPT_REGIST(KIS_getenv);
#else
//---------------------------------------------------------------------------
#ifndef KIS_SYSTEM_H
#define KIS_SYSTEM_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
#include <fstream>
//---------------------------------------------------------------------------
class KIS_securitylevel : public TKisFunction_base {
private:
	bool initialized;
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="securitylevel";
		Format_="securitylevel level";
		Returnval_="(NULL)";
		Information_="Set security level. This function works only once.";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);

	KIS_securitylevel(void): initialized(false) {}
};
//---------------------------------------------------------------------------
class KIS_logfile : public TKisFunction_base {
private:
	std::ofstream *filestream;
public:
	KIS_logfile(void) : filestream(NULL) {}

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="logfile";
		Format_="logfile [ path ]";
		Returnval_="(NULL)";
		Information_="specify logfile, or stop logging to file";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);

	// �ǥ��ȥ饯��
	virtual ~KIS_logfile();
};
//---------------------------------------------------------------------------
class KIS_loglevel : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="loglevel";
		Format_="loglevel [ level ]";
		Returnval_="(NULL)";
		Information_="set loglevel, or get current loglevel";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_debugger : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="debugger";
		Format_="debugger [ on | off ]";
		Returnval_="(NULL)";
		Information_="turn on/off debugger mode.";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_rccharset : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="rccharset";
		Format_="rccharset charsetname";
		Returnval_="(NULL)";
		Information_="Set character set of system messages.";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_getenv : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="getenv";
		Format_="getenv EnvValName";
		Returnval_="value of an environment variable";
		Information_="answer a value of an environment variable";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif // KIS_SYSTEM_H
//---------------------------------------------------------------------------
#endif
