//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��֥�����
//
//      Programed by NAKAUE.T (Meister) / Suikyo.
//
//  2001.05.27  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//  2001.06.12  Phase 5.3.2   ��貾�ۥ���ȥ�ˤ����륳��ƥ����ȤΥХ�����
//  2001.06.17  Phase 5.4     ����饤�󥹥���ץ����ñ���ڤ�Ф��ΥХ�����
//                            ����饤�󥹥���ץ�������򻲾ȤΥХ�����
//                            �ե���ѥ���
//  2002.03.10  Phase 7.9.0   �����ľ�ܥ��������ػ�
//  2002.03.17                KIU�˹�碌��TKisEngine����TKawariVM��̾���ѹ�
//                            Ʊ����TKawariCode��������KVMCode�����̾���ѹ//  2002.03.18                KIU�˹�碌��TKawariCompilerʬΥ
//  2002.04.18  Phase 8.0.0   ��֥����ɥ��饹������ؤ���
//
//---------------------------------------------------------------------------
#ifndef CODE_H__
#define CODE_H__
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
//---------------------------------------------------------------------------
class TKawariVM;			// VM

class TKVMCode_base;		// ��֥����ɤΥ��󥿡��ե�����
class TKVMCodeList_base;	// ��֥����ɤΥ��󥿡��ե�����(���쥯�饹)
class TKVMCodeList;			// ��֥�������

class TKVMCodeString;		// ʸ����
class TKVMCodeIDString;		// IDʸ����(�ե���ѥ����̤��ۤʤ�)
class TKVMCodeInlineScript;	// ����饤�󥹥���ץ�
class TKVMCodeScriptStatment;	// ������ץ�ʸ
class TKVMCodeEntryIndex;	// ����ȥ����󥢥�����
//--------------------------------------------------------------------------
// class TKVMCode_base;
//
// Interface class.
// ��֥����ɤΥ��󥿡��ե�����
class TKVMCode_base {
public:
	// �¹�
	virtual std::string Run(TKawariVM &vm)=0;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const { return ""; }
	// Debug�ǽ��Ϥ򤽤���
	virtual std::ostream &DebugIndent(std::ostream& os, unsigned int level=0) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const=0;
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& R_) const =0;
	// �ǥ��ȥ饯��
	virtual ~TKVMCode_base () {};
};
//---------------------------------------------------------------------------
// Comparator (for STL)
class TKVMCode_baseP_Less {
public:
	bool operator()(const TKVMCode_base *L,const TKVMCode_base *R) const;
};
//--------------------------------------------------------------------------
typedef std::vector<TKVMCode_base *> TCodePVector;
//--------------------------------------------------------------------------
// class TKVMCodeList_base : public TKVMCode_base;
//
// Interface class.
// �ꥹ�Ȥ������֥����ɤΥ��󥿡��ե�����
// (�ɤ��ʤ���ݲ�����)
class TKVMCodeList_base : public TKVMCode_base{
public:
	std::vector<TKVMCode_base *> list;

	virtual bool Less(const TKVMCode_base &R_) const;
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	virtual ~TKVMCodeList_base();
	TKVMCodeList_base (const std::vector<TKVMCode_base *> &tmplist);
protected:
	virtual const std::string GetName(void) const=0;
};

//--------------------------------------------------------------------------
// ��֥����ɥꥹ��
// ���äƤ���Code��Ϣ³Ū�˼¹Ԥ������Ƥη�̤��礷�ƽ���
class TKVMCodeList : public TKVMCodeList_base {
public:
	virtual std::string Run(class TKawariVM &vm);
	virtual std::string DisCompile(void) const;
	virtual ~TKVMCodeList() {}
	TKVMCodeList(const std::vector<TKVMCode_base *> &tmplist) : TKVMCodeList_base(tmplist) {}
protected:
	virtual const std::string GetName(void) const { return ""; }
};

//--------------------------------------------------------------------------
// ʸ���� (Literal)
// done.
class TKVMCodeString : public TKVMCode_base {
public:
	std::string s;

	virtual std::string Run(class TKawariVM &vm) { return s; }
	virtual std::string DisCompile(void) const;
	virtual bool Less(const TKVMCode_base &r_) const{
		const TKVMCodeString& r=dynamic_cast<const TKVMCodeString&>(r_);
		return(s<r.s);
	}
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const {
		return DebugIndent(os, level) << "S(" << s << ")" << std::endl;
	}
	virtual ~TKVMCodeString() {}
	TKVMCodeString(const std::string &str);
};

//--------------------------------------------------------------------------
// IDʸ���� (IdLiteral)
class TKVMCodeIDString : public TKVMCodeString {
public:
	virtual std::string DisCompile(void) const {
		return s;
	}
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const {
		return DebugIndent(os, level) << "ID(" << s << ")" << std::endl;
	}
	TKVMCodeIDString(const std::string &str) : TKVMCodeString(str) {}
};

//--------------------------------------------------------------------------
// ������ץ�ʸ ( WS ( Word WS ) * )
// ����Ū��Statement��Ʊ����
class TKVMCodeScriptStatement : public TKVMCodeList_base {
public:
	virtual std::string GetArg0(void);
	virtual std::string Run(class TKawariVM &vm);
	virtual std::string DisCompile(void) const;
	virtual ~TKVMCodeScriptStatement() {}
	TKVMCodeScriptStatement(const std::vector<TKVMCode_base *> &tmplist):TKVMCodeList_base(tmplist) {}
protected:
	virtual const std::string GetName(void) const { return "ScriptStatement"; }
};

//--------------------------------------------------------------------------
// ����饤�󥹥���ץ� ( '$(' ScriptStatementSeq ') )
class TKVMCodeInlineScript : public TKVMCodeList_base {
public:
	virtual std::string Run(class TKawariVM &vm);
	virtual std::string DisCompile(void) const;
	virtual ~TKVMCodeInlineScript() {}
	TKVMCodeInlineScript(const std::vector<TKVMCode_base *> &tmplist):TKVMCodeList_base(tmplist) {}
protected:
	virtual const std::string GetName(void) const { return "InlineScriptSubst"; }
};

//--------------------------------------------------------------------------
// ź�����դ�����ȥ�ƤӽФ�
// ( '$' EntryWord '[' WS Expr WS ']' )
class TKVMCodeEntryIndex : public TKVMCode_base {
public:
	TKVMCode_base *entry_id;
	TKVMCode_base *expr;	// ����TKawariExpr������

	virtual std::string Run(class TKawariVM &vm);
	virtual std::string DisCompile(void) const;
	virtual bool Less(const TKVMCode_base &R_) const;
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	virtual ~TKVMCodeEntryIndex();
	TKVMCodeEntryIndex(TKVMCode_base *eid, TKVMCode_base *e):entry_id(eid), expr(e) {}
};
//--------------------------------------------------------------------------
#endif // CODE_H__
