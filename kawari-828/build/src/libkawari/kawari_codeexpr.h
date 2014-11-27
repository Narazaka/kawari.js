//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��֥�����(�黻��)
//
//      Programed by Suikyo.
//
//  2002.04.18  Phase 8.0.0   �黻����֥����ɺ���
//
//---------------------------------------------------------------------------
#ifndef CODE_EXPR_H__
#define CODE_EXPR_H__
//---------------------------------------------------------------------------
#include "libkawari/kawari_code.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
// ��(�����ѹ��Բ�)
class TValue {
private:
	std::string str;
	int i; bool b;
	enum { vSTRING, vINTEGER, vBOOL, vERROR } state;
	TValue(void) : str(""), i(0), b(true), state(vERROR) {}
	TValue &operator = (const TValue &val);
public:
	inline const std::string &AsString(void) { return str; }
	inline int AsInteger(void);
	inline bool CanInteger(void);
	inline bool IsTrue(void);
	inline bool IsError(void) { return (state==vERROR); }
	inline TValue(const std::string &orgsen):str(orgsen), i(0), b(true), state(vSTRING){}
	inline TValue(const int orgi);
	inline TValue(const TValue &val):str(val.str), i(val.i), b(val.b), state(val.state) {}
	inline TValue(bool bv){
		if(bv){ str="true"; b=true; } else { str="false"; b=false; }
		i=0; state=vBOOL;
	}
	static inline TValue Error(void);
};
//--------------------------------------------------------------------------
// class TKVMExprCode_base;
// �黻����֥����ɤδ��쥯�饹
class TKVMExprCode_base : public TKVMCode_base {
public:
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// ��ɾ��
	virtual class TValue Evaluate(class TKawariVM &vm)=0;
	// �ǥ��ȥ饯��
	virtual ~TKVMExprCode_base () {}
};

//--------------------------------------------------------------------------
// class TKVMExprBinaryCode_base;
// ���黻�Ҥδ��쥯�饹
class TKVMExprBinaryCode_base : public TKVMExprCode_base {
protected:
	TKVMExprCode_base *lhs;
	TKVMExprCode_base *rhs;
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
	TKVMExprBinaryCode_base(TKVMExprCode_base *l, TKVMExprCode_base *r) : lhs(l), rhs(r) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMExprBinaryCode_base(void){
		if (lhs) delete lhs;
		if (rhs) delete rhs;
	}
};

//--------------------------------------------------------------------------
// class TKVMExprBinaryCode_base;
// ñ��黻�Ҥδ��쥯�饹
class TKVMExprUnaryCode_base : public TKVMExprCode_base {
protected:
	TKVMExprCode_base *code;
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
	TKVMExprUnaryCode_base(TKVMExprCode_base *c) : code(c) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMExprUnaryCode_base(void){
		if (code) delete code;
	}
};

//--------------------------------------------------------------------------
// ��
class TKVMCodeExpression : public TKVMCode_base {
	TKVMExprCode_base *code;
public:
	// �¹�
	virtual std::string Run(TKawariVM &vm);
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& r_) const;
	// �ե���ѥ���
	virtual std::string DisCompile(void) const{
		return "$["+code->DisCompile()+"]";
	}
	// �黻���Τߵե���ѥ���
	virtual std::string DisCompileExpression(void) const{
		return code->DisCompile();
	}
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���󥹥ȥ饯��
	TKVMCodeExpression(TKVMExprCode_base *c) : code(c) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMCodeExpression(void){
		if (code) delete code;
	}
};

// �黻�Ҥ�ͥ����
// [�⤤]
//    **(�߾�)
//    -(ñ��) +(ñ��) !(not) ~(���)
//    * / %
//    + -
//    &(�ӥå�AND)
//    |(�ӥå�OR) ^(�ӥå�XOR)
//    > >= < <=
//    = == != =~ !~
//    &&
//    ||
//    := : (����: ��θ��)
// [�㤤]
//--------------------------------------------------------------------------
// Logical OR
class TKVMExprCodeLOR : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "||"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeLOR(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// Logical AND
class TKVMExprCodeLAND : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "&&"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeLAND(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// EQUAL
class TKVMExprCodeEQ : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "=="; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeEQ(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// NOT EQUAL
class TKVMExprCodeNEQ : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "!="; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeNEQ(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// MATCH
class TKVMExprCodeMATCH : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "=~"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeMATCH(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// NOT MATCH
class TKVMExprCodeNMATCH : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "!~"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeNMATCH(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// GREATER THAN
class TKVMExprCodeGT : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return ">"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeGT(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// GREATER OR EQUAL
class TKVMExprCodeGTE : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return ">="; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeGTE(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// LESS THAN
class TKVMExprCodeLT : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "<"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeLT(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// LESS OR EQUAL
class TKVMExprCodeLTE : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "<="; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeLTE(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// BIT OR
class TKVMExprCodeBOR : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "|"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeBOR(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// BIT XOR
class TKVMExprCodeBXOR : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "^"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeBXOR(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// BIT AND
class TKVMExprCodeBAND : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "&"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeBAND(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// PLUS
class TKVMExprCodePLUS : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "+"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodePLUS(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// MINUS
class TKVMExprCodeMINUS : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "-"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeMINUS(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// MULTIPLY
class TKVMExprCodeMUL : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "*"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeMUL(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// DIVERSION
class TKVMExprCodeDIV : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "/"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeDIV(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// MODULER
class TKVMExprCodeMOD : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "%"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeMOD(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};

//--------------------------------------------------------------------------
// Unary PLUS
class TKVMExprCodeUPLUS : public TKVMExprUnaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "+"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeUPLUS(TKVMExprCode_base *c):TKVMExprUnaryCode_base(c) {}
};

//--------------------------------------------------------------------------
// Unary MINUS
class TKVMExprCodeUMINUS : public TKVMExprUnaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "-"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeUMINUS(TKVMExprCode_base *c):TKVMExprUnaryCode_base(c) {}
};

//--------------------------------------------------------------------------
// NOT
class TKVMExprCodeNOT : public TKVMExprUnaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "!"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeNOT(TKVMExprCode_base *c):TKVMExprUnaryCode_base(c) {}
};

//--------------------------------------------------------------------------
// COMPLIMENTAL
class TKVMExprCodeCOMP : public TKVMExprUnaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "~"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodeCOMP(TKVMExprCode_base *c):TKVMExprUnaryCode_base(c) {}
};

//--------------------------------------------------------------------------
// POWER
class TKVMExprCodePOW : public TKVMExprBinaryCode_base {
protected:
	virtual std::string GetOperator(void)const{ return "**"; }
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
	// ���󥹥ȥ饯��
	TKVMExprCodePOW(TKVMExprCode_base *l, TKVMExprCode_base *r):TKVMExprBinaryCode_base(l, r) {}
};
//--------------------------------------------------------------------------
// ExprWord
class TKVMExprCodeWord : public TKVMExprCode_base {
protected:
	TKVMCode_base *code;
public:
	// ��ɾ��
	virtual TValue Evaluate(class TKawariVM &vm);
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
	// ���󥹥ȥ饯��
	TKVMExprCodeWord(TKVMCode_base *c) : code(c) {}
	// �ǥ��ȥ饯��
	virtual ~TKVMExprCodeWord(void){
		if (code) delete code;
	}
};
//--------------------------------------------------------------------------
// '( �� )'
class TKVMExprCodeGroup : public TKVMExprCodeWord {
public:
	// �ե���ѥ���
	virtual std::string DisCompile(void) const{
		return "("+code->DisCompile()+")";
	}
	// ���󥹥ȥ饯��
	TKVMExprCodeGroup(TKVMCode_base *c) : TKVMExprCodeWord(c) {}
	// �ǥ��ȥ饯��
	~TKVMExprCodeGroup(void){ }
};
//---------------------------------------------------------------------------
#endif// CODE_EXPR_H__
