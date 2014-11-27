//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����ѥ���(�ż���
//
//      Programed by NAKAUE.T (Meister)
//
//  2002.03.18                KIU�˹�碌��TKawariCompilerʬΥ
//                            �������Ǥ⡢���󥿡��ե�����������äȰ㤦(��)
//  2002.04.18  Phase 8.0.0   ���륫���󥿥å�������ѥ����ý���
//
//---------------------------------------------------------------------------
#ifndef COMPILER_H__
#define COMPILER_H__
//---------------------------------------------------------------------------
#include <string>
#include <vector>
//---------------------------------------------------------------------------
class TKVMCode_base;		// ��֥�����
class TKVMExprCode_base;	// ����֥�����
class TKVMSetCode_base;		// ����黻����֥�����
//-------------------------------------------------------------------------
//
// ����ѥ����ý�
//
//-------------------------------------------------------------------------
class TKawariCompiler {
public:
	//=====================================================================
	// �����᥽�å�
	//=====================================================================

	enum Mode {
		M_DICT,		// ����ȥ�����⡼��
		M_KIS,		// KIS�⡼��
		M_END,		// �⡼�ɤ�1�Ľ�λ
		M_UNKNOWN,	// ???
		M_EOF		// EOF
	};

	// ���󥹥ȥ饯��
	TKawariCompiler(std::istream &is, class TKawariLogger &lgr, const std::string &filename, bool pp=true);

	// �ǥ��ȥ饯��
	~TKawariCompiler();

	// ���⡼�ɤ�����
	Mode GetNextMode(void);

	// ����ȥ�������ɤ�
	// ����� : �ե�����ν�λ���⡼���ڤ��ؤ���false. ����ʳ���true.
	bool LoadEntryDefinition(std::vector<std::string> &entries, std::vector<TKVMCode_base *> &sentences);

	// ���Ĥ�KIS���ʣʸ�������ɤ�
	TKVMCode_base *LoadInlineScript(void);


	// ʸ�����Statement�Ȥ�����֥����ɤإ���ѥ���
	static TKVMCode_base *Compile(const std::string &src, class TKawariLogger &logger);

	// ʸ����򡢤��Τޤ�String�Ȥ�����֥����ɲ�����
	static TKVMCode_base *CompileAsString(const std::string &src);

	// ʸ����򽸹�黻��('$['��]'�����)�Ȥ��ƥ���ѥ���
	static TKVMSetCode_base *CompileAsEntryExpression(const std::string &src, class TKawariLogger &logger);

	// ʸ����򥨥�ȥ�̾�ǻ��Ѳ�ǽ��ʸ����˥��󥳡��ɤ���
	static std::string EncodeEntryName(const std::string &orgsen);

private:
	// ������ϴ�
	class TKawariLexer *lexer;

	// ����
	class TKawariLogger &logger;

	//=====================================================================
	// ������᥽�å�
	//=====================================================================

	// ����ȥ�̾�¤� ( IdLiteral S | IdLiteral S ',' S EntryIdList )
	// return : ����줿����ȥ�̾�ο�
	int compileEntryIdList(std::vector<std::string> &list);

	// �����Բ�ʸ�¤ӡ�(���)���ԥ����ɤ��ɤ߹���ǽ�λ��
	// return : ����줿ʸ�ο�
	int compileNRStatementList(std::vector<TKVMCode_base *> &list);

	// ��ʸ�¤ӡ�')'�ǽ�λ��
	// return : ����줿ʸ�ο�
	int compileStatementList(std::vector<TKVMCode_base *> &list);

	//=====================================================================
	// ñ��

	// ʸ ( (Word WS)* )
	// ���ǿ������λ��⥤�󥹥��󥹤��֤��������$(NULL)���������롣
	// 
	// noret : ���Ԥ�����ʤ����true.
	// mode : TKawariLexer::Mode��Ʊ�͡��ǥե���Ȥϥ���ȥ�����⡼�ɡ�
	TKVMCode_base *compileStatement(bool noret, int mode=1);

	// ʸ���� ( Literal )�ˤϥ���ѥ����¸�ߤ��ʤ���

	// ñ�� ( (Literal | Block | Subst )* )
	// ���ǿ��������ä���硢NULL���֤���
	// mode : TKawariLexer::Mode��Ʊ��
	TKVMCode_base *compileWord(int mode);

	// �ִ� ( '$' ( EntryCallSubst | EntryIndexSubst | InlineScriptSubst | ExprSubst ) )
	// ɬ����Ƭ��'$'����뤳�ȡ�
	TKVMCode_base *compileSubst(void);

	// ������ץ�ʸ ( WS ( Word WS ) * )
	TKVMCode_base *compileScriptStatement(void);

	// ����饤�󥹥���ץ� ( '(' ScriptStatementSeq ') )
	TKVMCode_base *compileInlineScriptSubst(void);

	// �֥�å� ( '(' WS Statement ')' )
	// ���ǿ������λ��⥤�󥹥��󥹤��֤��������$(NULL)���������롣
	TKVMCode_base *compileBlock(void);

	// ź�����դ�����ȥ�ƤӽФ��ʰ���
	// ( ( EntryName | Subst ) '[' WS Expr WS ']' )
	TKVMCode_base *compileEntryIndexSubst(void);

	// ����ȥ�̾ ( (IdLiteral | Subst)+ WS )
	TKVMCode_base *compileEntryWord(void);

	//=====================================================================
	// ������ץȹ�ʸ

	// if
	TKVMCode_base *compileScriptIF(void);


	//=====================================================================
	// ��

	// �� ( '[' Expression  ']' )
	TKVMCode_base *compileExprSubst(void);

	// ������ ( '||' )
	TKVMExprCode_base *compileExpr0(void);

	// ������ ( '&&' )
	TKVMExprCode_base *compileExpr1(void);

	// ���� ( '=' | '==' | '!=' )
	TKVMExprCode_base *compileExpr2(void);

	// ��� ( '<' | '<=' | '>' | '>=' )
	TKVMExprCode_base *compileExpr3(void);

	// �ӥå�OR ('|' | '^')
	TKVMExprCode_base *compileExpr4(void);

	// �ӥå�AND ('&')
	TKVMExprCode_base *compileExpr5(void);

	// �ø��� ('+' | '-')
	TKVMExprCode_base *compileExpr6(void);

	// ����� ('*' | '/' | '%')
	TKVMExprCode_base *compileExpr7(void);

	// ñ��黻�� ('+' | '-' | '!' | '~')
	TKVMExprCode_base *compileExpr8(void);

	// �߾� ( '**' )
	TKVMExprCode_base *compileExpr9(void);

	// Expr���� ( '(' Expression ')' | ExprWord )
	TKVMExprCode_base *compileExprFactor(void);

	// Exprñ�� ( DecimalLiteral WS | (QuotedLiteral | Subst)+ WS )
	TKVMExprCode_base *compileExprWord(void);

	//=====================================================================
	// ����ȥ꽸��黻��

	// ����ȥ�ƤӽФ� ( '{' EntryExpr '}' )
	TKVMCode_base *compileEntryCallSubst(void);

	// �º� ('+' | '-')
	TKVMSetCode_base *compileSetExpr0(void);

	// �� ('&')
	TKVMSetCode_base *compileSetExpr1(void);

	// ����黻������ ( '(' EntryExpr ')' | EntryWord )
	TKVMSetCode_base *compileSetExprFactor(void);

	// ����黻��ñ�� ( (IdLiteral | Subst)+ WS )
	TKVMSetCode_base *compileSetExprWord(void);
};
//---------------------------------------------------------------------------
#endif // COMPILER_H__
