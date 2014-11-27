//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��֥�����(KIS��ʸ)
//
//      Programed by Suikyo.
//
//  2002.04.18  Phase 8.0.0   KIS��ʸ��֥����ɺ���
//
//---------------------------------------------------------------------------
#ifndef KAWARI_CODEKIS_H__
#define KAWARI_CODEKIS_H__
//---------------------------------------------------------------------------
#include "libkawari/kawari_code.h"
//---------------------------------------------------------------------------
// KIS IFʸ
class TKVMKISCodeIF : public TKVMCode_base {
	std::vector<TKVMCode_base *> condlist;
	std::vector<TKVMCode_base *> list;
public:
	// �¹�
	virtual std::string Run(class TKawariVM &vm);
	// �ե���ѥ���
	virtual std::string DisCompile(void) const;
	// �ǥХå��ѥĥ꡼ɽ��
	virtual std::ostream &Debug(std::ostream& os, unsigned int level=0) const;
	// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
	virtual bool Less(const TKVMCode_base& R_) const;
	// ���󥹥ȥ饯��
	TKVMKISCodeIF(
		const std::vector<TKVMCode_base *> &clist,
		const std::vector<TKVMCode_base *> &l);
	// �ǥ��ȥ饯��
	virtual ~TKVMKISCodeIF();
};
#if 0
//---------------------------------------------------------------------------
// KIS WHILEʸ
class TKVMKIS_WHILE : public TKVMCodeScriptStatement {
public:
	virtual std::string Run(class TKawariVM &vm);
	virtual ~TKVMKIS_IF() { }
	TKVMKIS_WHILE(const std::vector<TKVMCode_base *> &tmplist): TKVMCodeScriptStatement(tmplist) {}
};

#endif
//---------------------------------------------------------------------------
#endif // KAWARI_CODEKIS_H__
