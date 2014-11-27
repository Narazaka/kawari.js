//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ������� --
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.14  Phase 0.50    First version
//  2001.05.30  Phase 5.1     
//  2001.07.08  Phase 6.0     eval�Х��ե��å���
//  2001.07.14  Phase 6.1     clear�ɲ�
//  2001.08.08  Phase 6.2     entry�ɲ�
//  2001.08.25  Phase 6.3     entry�����ɲ�
//                            get���ä����ɲ�
//                            size�ɲ�
//  2001.12.18  Phase 7.2     array����ˤ��ä����ɲ�
//  2002.03.10  Phase 7.9.0   �����ľ�ܥ��������ػ�
//                            enumerate,array�ѻ�
//                            get�����ѹ�,exec,enumexecƳ��
//  2002.03.13                ����ȥ�̾��ź������ǰƳ��
//  2002.03.17                �������ؿ�����
//                            list���ؿ��ɲ�
//  2002.04.27  Phase 8.0.0   writeprotect�ɲ�
//  2003.04.27  Phase 8.1.0   shiftcode�Х��ե��å���
//  2008.02.16  Phase 8.2.7   insertstr�Х��ե��å���
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_encode_entryname);
INLINE_SCRIPT_REGIST(KIS_eval);
INLINE_SCRIPT_REGIST(KIS_size);
INLINE_SCRIPT_REGIST(KIS_set);
INLINE_SCRIPT_REGIST(KIS_setstr);
INLINE_SCRIPT_REGIST(KIS_adddict);
INLINE_SCRIPT_REGIST(KIS_adddictstr);
INLINE_SCRIPT_REGIST(KIS_push);
INLINE_SCRIPT_REGIST(KIS_pushstr);
INLINE_SCRIPT_REGIST(KIS_unshift);
INLINE_SCRIPT_REGIST(KIS_unshiftstr);
INLINE_SCRIPT_REGIST(KIS_pop);
INLINE_SCRIPT_REGIST(KIS_shift);
INLINE_SCRIPT_REGIST(KIS_popcode);
INLINE_SCRIPT_REGIST(KIS_shiftcode);
INLINE_SCRIPT_REGIST(KIS_entry);
INLINE_SCRIPT_REGIST(KIS_get);
INLINE_SCRIPT_REGIST(KIS_getrandom);
INLINE_SCRIPT_REGIST(KIS_getcode);
INLINE_SCRIPT_REGIST(KIS_clear);
INLINE_SCRIPT_REGIST(KIS_insert);
INLINE_SCRIPT_REGIST(KIS_insertstr);
INLINE_SCRIPT_REGIST(KIS_writeprotect);
INLINE_SCRIPT_REGIST(KIS_wordcount);
INLINE_SCRIPT_REGIST(KIS_entrycount);
INLINE_SCRIPT_REGIST(KIS_find);
INLINE_SCRIPT_REGIST(KIS_rfind);
INLINE_SCRIPT_REGIST(KIS_cleartree);
INLINE_SCRIPT_REGIST(KIS_listsub);
INLINE_SCRIPT_REGIST(KIS_listtree);
INLINE_SCRIPT_REGIST(KIS_copy);
INLINE_SCRIPT_REGIST(KIS_copytree);
INLINE_SCRIPT_REGIST(KIS_move);
INLINE_SCRIPT_REGIST(KIS_movetree);
#else
//---------------------------------------------------------------------------
#ifndef KIS_DICT_H
#define KIS_DICT_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_encode_entryname : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="encode_entryname";
		Format_="encode_entryname Word1";
		Returnval_="encoded entry name";
		Information_="encode Word1 to entry name";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_eval : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="eval";
		Format_="eval Word1 ...";
		Returnval_="decode result";
		Information_="parse all arguments and return its decode result";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_size : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="size";
		Format_="size Entry1";
		Returnval_="a word number in Entry1";
		Information_="return a word number in Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_set : public TKisFunction_base {
protected:

	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="set";
		Format_="set Index1 Word1";
		Returnval_="(NULL)";
		Information_="set Word1 to Index1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_setstr : public KIS_set {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="setstr";
		Format_="setstr Index1 Str1";
		Returnval_="(NULL)";
		Information_="set string Str1 to Index1 (equals to 'set')";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
class KIS_adddict : public TKisFunction_base {
protected:

	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="adddict";
		Format_="adddict Entry1 Word1";
		Returnval_="(NULL)";
		Information_="append Word1 to Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_adddictstr : public KIS_adddict {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="adddictstr";
		Format_="adddictstr Entry1 Str1";
		Returnval_="(NULL)";
		Information_="append string Str1 to Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
class KIS_push : public KIS_adddict {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="push";
		Format_="push Entry1 Word1";
		Returnval_="(NULL)";
		Information_="append Word1 to Entry1(equals to 'adddict')";

		return(true);
	}
};
//---------------------------------------------------------------------------
class KIS_pushstr : public KIS_adddictstr {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="pushstr";
		Format_="pushstr Entry1 Str1";
		Returnval_="(NULL)";
		Information_="append string Str1 to Entry1(equals to 'adddictstr')";

		return(true);
	}
};
//---------------------------------------------------------------------------
class KIS_unshift : public TKisFunction_base {
protected:

	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="unshift";
		Format_="unshift Entry1 Word1";
		Returnval_="(NULL)";
		Information_="insert Word1 to the top of Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_unshiftstr : public KIS_unshift {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="unshiftstr";
		Format_="unshiftstr Entry1 Str1";
		Returnval_="(NULL)";
		Information_="insert string Str1 to the top of Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
class KIS_pop : public TKisFunction_base {
protected:

	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag,bool flag2);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="pop";
		Format_="pop Entry1";
		Returnval_="word";
		Information_="remove last word of Entry1 and returns it";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false,false));
	}
};
//---------------------------------------------------------------------------
class KIS_shift : public KIS_pop {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="shift";
		Format_="shift Entry1";
		Returnval_="word";
		Information_="remove first word of Entry1 and returns it";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true,false));
	}
};
//---------------------------------------------------------------------------
class KIS_popcode : public KIS_pop {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="popcode";
		Format_="popcode Entry1";
		Returnval_="word";
		Information_="remove last word of Entry1 and return it";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false,true));
	}
};
//---------------------------------------------------------------------------
class KIS_shiftcode : public KIS_pop {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="shiftcode";
		Format_="shiftcode Entry1";
		Returnval_="word";
		Information_="remove first word of Entry1 and return it";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true,true));
	}
};
//---------------------------------------------------------------------------
class KIS_getrandom : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="getrandom";
		Format_="getrandom Entry1 [Word1]";
		Returnval_="${Entry1} or Word1";
		Information_="equivalent to \"${Entry1}\"";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_entry : public KIS_getrandom {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="entry";
		Format_="entry Entry1 [Word1]";
		Returnval_="${Entry1} or Word1";
		Information_="equivalent to \"${Entry1}\"";

		return(true);
	}
};
//---------------------------------------------------------------------------
class KIS_get : public TKisFunction_base {
protected:

	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="get";
		Format_="get Index1";
		Returnval_="word";
		Information_="enumerate and join all words in Entry1 from start to end";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_getcode : public KIS_get {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="getcode";
		Format_="get Index1";
		Returnval_="word";
		Information_="enumerate and join all words in Entry1 from start to end";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
class KIS_clear : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="clear";
		Format_="clear Index1";
		Returnval_="(NULL)";
		Information_="clear all content of Index1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_insert : public TKisFunction_base {
protected:
	// ���󥿡��ץ꥿�μ���
	string Function_(const vector<string>& args,bool flag);

public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="insert";
		Format_="insert Index1 Word1";
		Returnval_="(NULL)";
		Information_="Insert Word1 into before Index1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,false));
	}
};
//---------------------------------------------------------------------------
class KIS_insertstr : public KIS_insert {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="insertstr";
		Format_="insertstr Index1 Str1";
		Returnval_="(NULL)";
		Information_="Insert Str1 into before Index1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args)
	{
		return(Function_(args,true));
	}
};
//---------------------------------------------------------------------------
class KIS_writeprotect : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="writeprotect";
		Format_="writeprotect Entry";
		Returnval_="(NULL)";
		Information_="Let Entry be protected. Contents of Entry can't change anymore";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_wordcount : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="wordcount";
		Format_="wordcount";
		Returnval_="number of words in dictionary";
		Information_="Count number of all words in dictionary";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_entrycount : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="entrycount";
		Format_="entrycount";
		Returnval_="number of entries in dictionary";
		Information_="Count number of all entries in dictionary";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_find : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="find";
		Format_="find Entry Word [ StartIndex ]";
		Returnval_="Index(0+) if found, or -1";
		Information_="Search upword for specified Word in Entry, from StartIndex(default:0).";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_rfind : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="rfind";
		Format_="rfind Entry Word [ StartIndex ] ";
		Returnval_="Index(0+) if found, or -1";
		Information_="Search downword for specified Word in Entry, from StartIndex(default:size of Entry).";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_cleartree : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="cleartree";
		Format_="cleartree Entry1";
		Returnval_="(NULL)";
		Information_="Clear all entries whose name starts with \"Entry1\"";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_listsub : public TKisFunction_base {
protected:
	// subflag : true�λ�, ent_listư� false�λ�allentryư�
	void _Function(const vector<string>& args, bool subflag);
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="listsub";
		Format_="listsub Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="List all sub entries of Entry2 to Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, true);
		return ("");
	}
};
//---------------------------------------------------------------------------
class KIS_listtree : public KIS_listsub {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="listtree";
		Format_="listtree Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="List all entries whose name starts with \"Entry2\" to Entry1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, false);
		return ("");
	}
};
//---------------------------------------------------------------------------
class KIS_copy : public TKisFunction_base {
protected:
	// rmflag : true�λ�, moveư��
	void _Function(const vector<string>& args, bool rmflag);
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="copy";
		Format_="copy Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="copy words of Entry1 to Entry2";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, false);
		return ("");
	}
};
//---------------------------------------------------------------------------
class KIS_copytree : public TKisFunction_base {
protected:
	// rmflag : true�λ�, moveư��
	void _Function(const vector<string>& args, bool rmflag);
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="copytree";
		Format_="copytree Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="copy Entry1 to Entry2 recursively";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, false);
		return ("");
	}
};
//---------------------------------------------------------------------------
class KIS_move : public KIS_copy {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="move";
		Format_="move Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="move words of Entry1 to Entry2";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, true);
		return ("");
	}
};
//---------------------------------------------------------------------------
class KIS_movetree : public KIS_copytree {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="movetree";
		Format_="movetree Entry1 Entry2";
		Returnval_="(NULL)";
		Information_="move Entry1 to Entry2 recursively";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args){
		_Function(args, true);
		return ("");
	}
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
// ���
// encode_entryname/eval/size
//
// �񤭹���
// +--------------------------------------------+
// |              | ʸ����       | KIS          |
// |--------------+--------------+--------------|
// | Ǥ�հ���     | setstr       | set          |
// | �����ɲ�     | adddictstr   | adddict      |
// | ��Ƭ�ɲ�     | unshiftstr   | unshift      |
// +--------------------------------------------+
//
// �ɤ߽Ф�
// +--------------------------------------------+
// |              | eval         | ��eval       |
// |--------------+--------------+--------------|
// | ���������� | getrandom    |              |
// | ����         | get          | getcode      |
// | ��Ƭ         | shift        | shiftcode    |
// | ����         | pop          | popcode      |
// +--------------------------------------------+
//
// �õ�
// clear
//---------------------------------------------------------------------------
