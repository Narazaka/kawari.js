//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��֥�����(����黻��)
//
//      Programed by Suikyo.
//
//  2002.04.18  Phase 8.0.0   ����黻����֥����ɺ���
//
//---------------------------------------------------------------------------
#ifndef KAWARI_CODESET_H__
#define KAWARI_CODESET_H__
//---------------------------------------------------------------------------
#include "libkawari/kawari_code.h"
#include "libkawari/kawari_dict.h"
//---------------------------------------------------------------------------
#include <string>
#include <set>
//---------------------------------------------------------------------------
// ����黻����֥����ɤδ��쥯�饹
class TKVMSetCode_base : public TKVMCode_base {
public:
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// ��ɾ��
	virtual void Evaluate(class TKawariVM &vm, std::set<TWordID> &wordcol)=0;
	// �ǥ��ȥ饯��
	virtual ~TKVMSetCode_base () {}
};

//---------------------------------------------------------------------------
// ����黻�����黻�ҥ����ɤδ��쥯�饹
class TKVMSetBinaryCode_base : public TKVMSetCode_base {
protected:
	TKVMSetCode_base *lhs;
	TKVMSetCode_base *rhs;
	// �黻��ʸ������֤�
	virtual std::string GetOperator(void)const=0;
public:
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���󥹥ȥ饯��
	TKVMSetBinaryCode_base(TKVMSetCode_base *l, TKVMSetCode_base *r) : lhs(l), rhs(r) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMSetBinaryCode_base(void){
		if (lhs) delete lhs;
		if (rhs) delete rhs;
	}
};

//---------------------------------------------------------------------------
// ��
class TKVMSetCodePLUS : public TKVMSetBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "+"; }
public:
	// ��ɾ��
	virtual void Evaluate(class TKawariVM &vm, std::set<TWordID> &wordcol);
	// ���󥹥ȥ饯��
	TKVMSetCodePLUS(TKVMSetCode_base *l, TKVMSetCode_base *r):TKVMSetBinaryCode_base(l, r) {}
};
//---------------------------------------------------------------------------
// ��
class TKVMSetCodeMINUS : public TKVMSetBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "-"; }
public:
	// ��ɾ��
	virtual void Evaluate(class TKawariVM &vm, std::set<TWordID> &wordcol);
	// ���󥹥ȥ饯��
	TKVMSetCodeMINUS(TKVMSetCode_base *l, TKVMSetCode_base *r):TKVMSetBinaryCode_base(l, r) {}
};
//---------------------------------------------------------------------------
// ��
class TKVMSetCodeAND : public TKVMSetBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "&"; }
public:
	// ��ɾ��
	virtual void Evaluate(class TKawariVM &vm, std::set<TWordID> &wordcol);
	// ���󥹥ȥ饯��
	TKVMSetCodeAND(TKVMSetCode_base *l, TKVMSetCode_base *r):TKVMSetBinaryCode_base(l, r) {}
};
//---------------------------------------------------------------------------
// Set Expression Word
class TKVMSetCodeWord : public TKVMSetCode_base {
	TKVMCode_base *code;
public:
	// ��ɾ��
	virtual void Evaluate(class TKawariVM &vm, std::set<TWordID> &wordcol);
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const{
		return code->DisCompile();
	}
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const{
		return code->Debug(os, level);
	}
	// �⤷code��TKVMCodeIDString�ʤ�С�������֤���
	const TKVMCodeIDString *GetIfPVW(void) const;
	// ���󥹥ȥ饯��
	TKVMSetCodeWord(TKVMCode_base *c) : code(c) {}
	// �ǥ��ȥ饯��
	~TKVMSetCodeWord(void){
		if (code) delete code;
	}
};
//---------------------------------------------------------------------------
// ����ȥ�ƤӽФ��ʰ��� ( '${' EntryExpr '}' )
// �ʰ���done.
class TKVMCodeEntryCall : public TKVMCode_base {
	TKVMSetCode_base *code;
public:
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���󥹥ȥ饯��
	TKVMCodeEntryCall(TKVMSetCode_base *c) : code(c) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMCodeEntryCall();
};

//---------------------------------------------------------------------------
// ��貾��ñ��(�ü쥨��ȥ�ƤӽФ�) ( '${' Literal '}' )
class TKVMCodePVW : public TKVMCode_base {
	std::string entry;
public:
	// ���ȥ���ȥ�̾���֤�
	virtual std::string Get(void) { return entry; }
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���󥹥ȥ饯��
	TKVMCodePVW(const std::string &c) : entry(c) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMCodePVW() {}
};

//---------------------------------------------------------------------------
// ���򻲾� ( '${' Integer '}' )
class TKVMCodeHistoryCall : public TKVMCode_base {
	int index;
public:
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���󥹥ȥ饯��
	TKVMCodeHistoryCall(int i) : index(i) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMCodeHistoryCall() {}
};

//---------------------------------------------------------------------------
#endif // KAWARI_CODESET_H__
