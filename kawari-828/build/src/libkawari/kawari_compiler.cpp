//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����ѥ���(�ż���
//
//      Programed by NAKAUE.T (Meister) / Suikyo
//
//  2002.03.18                KIU�˹�碌��TKawariCompilerʬΥ
//                            �������Ǥ⡢���󥿡��ե�����������äȰ㤦(��)
//  2002.04.18  Phase 8.0.0   ���륫���󥿥å�������ѥ����ý���
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_compiler.h"
#include "libkawari/kawari_lexer.h"
#include "libkawari/kawari_code.h"
#include "libkawari/kawari_codeexpr.h"
#include "libkawari/kawari_codeset.h"
#include "libkawari/kawari_codekis.h"
#include "libkawari/kawari_dict.h"
#include "libkawari/kawari_log.h"
#include "libkawari/kawari_rc.h"
#include "misc/misc.h"
using namespace kawari::resource;
using namespace kawari_log;
//---------------------------------------------------------------------------
#include <cctype>
#include <iostream>
#include <fstream>
#ifdef HAVE_SSTREAM_H
#include <sstream>
#else
#include <strstream>
#endif
using namespace std;
//---------------------------------------------------------------------------
namespace {
	const TKawariLexer::Mode ID_MODE=TKawariLexer::ID_MODE;
	const TKawariLexer::Mode LITERAL_MODE=TKawariLexer::LITERAL_MODE;
	const TKawariLexer::Mode SCRIPT_MODE=TKawariLexer::LITERAL_MODE2;
	const TKawariLexer::Mode BLOCK_MODE=TKawariLexer::LITERAL_MODE3;
}
//=========================================================================
// �����᥽�åɷ�
//=========================================================================

//-------------------------------------------------------------------------
// ���󥹥ȥ饯��
TKawariCompiler::TKawariCompiler(istream &is, TKawariLogger &lgr, const string &filename, bool pp)
	 : logger(lgr) {
	lexer=new TKawariLexer(is, logger, filename, pp);
}
//-------------------------------------------------------------------------
// �ǥ��ȥ饯��
TKawariCompiler::~TKawariCompiler (){
	if (lexer)
		delete lexer;
}
//-------------------------------------------------------------------------
// ���⡼�ɤ�����
TKawariCompiler::Mode TKawariCompiler::GetNextMode(void){
	Token::Type t=lexer->skipWS(ID_MODE);
	if (t==Token::T_MODESWITCH){
		lexer->ResetModeSwitch();
		string line=lexer->getRestOfLine();
		// �⡼����к��
		line=StringTrim(line);
		if (line=="=dict")
			return M_DICT;
		else if (line=="=kis")
			return M_KIS;
		else if (line=="=end")
			return M_END;
		else{
			logger.GetStream(LOG_ERROR) << RC.S(ERR_COMPILER_UNKNOWN_MODE) << line << endl;
			return M_UNKNOWN;
		}
	}else if (t==Token::T_EOF){
		return M_EOF;
	}else{
		// �Ϥμ���⡼�ɤȻפ��
		return M_DICT;
	}
}
//-------------------------------------------------------------------------
// ����ȥ�������ɤ�
bool TKawariCompiler::LoadEntryDefinition(vector<string> &entries, vector<TKVMCode_base *> &sentences){

	Token::Type t=lexer->skipWS(ID_MODE);
	if ((t==Token::T_EOF)||(t==Token::T_MODESWITCH)){
		// �ե�����ν�λ���⡼������
		return false;
	}else{
		// ñ�����

		// ����ȥ�̾�ꥹ�Ȥ����롣
		if(!compileEntryIdList(entries)){
			// ����ȥ�̾���̵꤬��(����)
			lexer->error(RC.S(ERR_COMPILER_NO_ENTRYNAMES));
			lexer->getRestOfLine();
			return true;
		}

		// ':'���ɤࡣ̵����Х��顼�����ɤ��Τޤ�³�ԡ�
		t=lexer->skipS(ID_MODE);
		if (t==(int)':'){
			lexer->skip();
			lexer->skipS();
			// �����Բ�ʸ�¤Ӥ����롣
			if (!compileNRStatementList(sentences))
				lexer->warning(RC.S(WARN_COMPILER_BLANK_DEFINITION));
		}else if (t==(int)'('){
			lexer->skip();
			lexer->skipS();
			// ʸ�¤Ӥ����롣
			if (!compileStatementList(sentences))
				lexer->warning(RC.S(WARN_COMPILER_BLANK_DEFINITION));
			t=lexer->skipWS(ID_MODE);
			if (t==(int)')'){
				lexer->skip();
			}else{
				lexer->error(RC.S(ERR_COMPILER_MLENTRYDEF_NOT_CLOSED));
			}
		}else{
			lexer->error(RC.S(ERR_COMPILER_INVALID_ENTRYDEF));
		}

		if (logger.Check(LOG_DUMP)){
			// ����
			ostream &log=logger.GetStream();
			log << "EntryNames(" << endl;
			for (vector<string>::const_iterator it=entries.begin();it!=entries.end();it++){
				log << "    " << (*it) << endl;
			}
			log << ")" << endl;

			for (TCodePVector::iterator it=sentences.begin();it!=sentences.end();it++){
				if (*it)
					(*it)->Debug(log, 0);
			}
		}

		return true;
	}
}
//-------------------------------------------------------------------------
// KIS������ɤ�
TKVMCode_base *TKawariCompiler::LoadInlineScript(void){
	vector<TKVMCode_base *> tmplist;

	// �ǽ�ιԤ��ɤ�
	TKVMCode_base *code = compileScriptStatement();
	if (code)
		tmplist.push_back(code);

	while (lexer->hasNext()){
		Token::Type t=lexer->skipWS(SCRIPT_MODE);
		if (t==(int)';'){
			// ���ι�
			lexer->skip();
			TKVMCode_base *code = compileScriptStatement();
			if (code)
				tmplist.push_back(code);
		}else if ((t==Token::T_EOF)||(t==Token::T_MODESWITCH)){
			break;
		}else{
			lexer->error(RC.S(ERR_COMPILER_ILLCHAR_IN_SCRIPT));
			break;
		}
	}
	if (tmplist.size())
		return new TKVMCodeInlineScript(tmplist);
	else
		return new TKVMCodeString("");
}
//-------------------------------------------------------------------------
// ʸ�����Statement�Ȥ�����֥����ɤإ���ѥ���
TKVMCode_base *TKawariCompiler::Compile(const string &src, TKawariLogger &logger){
#ifdef HAVE_SSTREAM_H
	istringstream is(src.c_str());
#else
	istrstream is(src.c_str());
#endif
	TKawariCompiler compiler(is, logger, "<unknown>", false);
	return compiler.compileStatement(true, BLOCK_MODE);
}

//-------------------------------------------------------------------------
// ʸ����򡢤��Τޤ�String�Ȥ�����֥����ɲ�����
TKVMCode_base *TKawariCompiler::CompileAsString(const string &src){
	return new TKVMCodeString(src);
}
//-------------------------------------------------------------------------
// ʸ����򽸹�黻��('$[' �� ']'�����)�Ȥ��ƥ���ѥ���
TKVMSetCode_base *TKawariCompiler::CompileAsEntryExpression(const string &src, TKawariLogger &logger){
#ifdef HAVE_SSTREAM_H
	istringstream is(src.c_str());
#else
	istrstream is(src.c_str());
#endif
	TKawariCompiler compiler(is, logger, "<unknown>", false);
	return compiler.compileSetExpr0();
}

//-------------------------------------------------------------------------
// �����ӥ��᥽�å�
// ʸ����򥨥�ȥ�̾�ǻ��Ѳ�ǽ��ʸ����˥��󥳡��ɤ���
string TKawariCompiler::EncodeEntryName(const string &orgsen){
	return TKawariLexer::EncodeEntryName(orgsen);
}
//-------------------------------------------------------------------------

//=========================================================================
// ������᥽�åɷ�
//=========================================================================

//-------------------------------------------------------------------------
// ����ȥ�̾�¤� ( IdLiteral S | IdLiteral S ',' S EntryIdList )
// return : ����줿����ȥ�̾�ο�
int TKawariCompiler::compileEntryIdList(vector<string> &list){
	vector<string> tmplist;
	if (!lexer->hasNext())
		return 0;
	Token::Type t=lexer->peek(ID_MODE);
	if (t==Token::T_LITERAL){
		// �ǽ�Υ���ȥ�̾���ɤ�
		tmplist.push_back(lexer->getLiteral(ID_MODE));
	}else{
		// ����ϵ�����ʤ�����
		lexer->error(RC.S(ERR_COMPILER_ENTRYID_NOT_FOUND));
		return 0;
	}
	while (lexer->hasNext()){
		Token::Type t=lexer->skipS();
		if (t==(int)','){
			lexer->skip();
			if (lexer->skipS()==Token::T_LITERAL){
				tmplist.push_back(lexer->getLiteral(ID_MODE));
			}else{
				// ����������','�ǽ���äƤ���󤸤�ʤ����ʡ�
				lexer->warning(RC.S(WARN_COMPILER_ENTRYIDLIST_ENDS_WITH_COMMA));
				break;
			}
		}else{
			break;
		}
	}
	list.insert(list.end(), tmplist.begin(), tmplist.end());
	return tmplist.size();
}

//-------------------------------------------------------------------------
// �����Բ�ʸ�¤ӡ�(���)���ԥ����ɤ��ɤ߹���ǽ�λ��
// return : ����줿ʸ�ο�
int TKawariCompiler::compileNRStatementList(vector<TKVMCode_base *> &list){
	vector<TKVMCode_base *> tmplist;
	if (!lexer->hasNext())
		return 0;
	Token::Type t=lexer->skipS();
	if (t==Token::T_EOL){
		// �����ʤ꽪��äƤ��ޤä�
		lexer->skip();
		return 0;
	}else if (t!=(int)','){
		// �ǽ��ʸ���ɤ�
		TKVMCode_base *stmt=compileStatement(true);
		if (stmt){
			tmplist.push_back(stmt);
		}else{
			lexer->error(RC.S(ERR_COMPILER_FIRST_STATEMENT_NOT_FOUND));
			lexer->getRestOfLine();
			return 0;
		}
	}
	while (lexer->hasNext()){
		Token::Type t=lexer->skipS();
		if (t==(int)','){
			lexer->skip();
			lexer->skipS();
			TKVMCode_base *stmt=compileStatement(true);
			if (stmt)
				tmplist.push_back(stmt);
			// else ','�ǹԤ�����äƤ��롩 �⤷���� ', ,'�ߤ����ˤʤäƤ롩
			// �����ǡ�NULLʸ����פ������櫓�ˤϤ����ʤ���
		}else if (t==Token::T_EOL){
			// ��λ
			break;
		}else{
			lexer->error(RC.S(ERR_COMPILER_STATEMENTLIST_SEPARATOR_NOT_FOUND));
			lexer->simpleSkipTo(',');
			if (lexer->peek()!=(int)',')
				break;
		}
	}
	// ���ιԤ������ɤ߹���
	lexer->getRestOfLine();
	list.insert(list.end(), tmplist.begin(), tmplist.end());
	return tmplist.size();
}

//-------------------------------------------------------------------------
// ʸ�¤ӡ�')'�ǽ�λ
// return : ����줿ʸ�ο�
int TKawariCompiler::compileStatementList(vector<TKVMCode_base *> &list){
	vector<TKVMCode_base *> tmplist;
	if (!lexer->hasNext())
		return 0;
	Token::Type t=lexer->skipWS(ID_MODE);
	if (t==')'){
		// �����ʤ꽪��äƤ��ޤä�
		return 0;
	}else if (t!=(int)','){
		// �ǽ��ʸ���ɤ�
		TKVMCode_base *stmt=compileStatement(false);
		if (stmt){
			tmplist.push_back(stmt);
		}else{
			lexer->error(RC.S(ERR_COMPILER_FIRST_STATEMENT_NOT_FOUND));
			lexer->getRestOfLine();
			return 0;
		}
	}
	while (lexer->hasNext()){
		Token::Type t=lexer->skipWS(ID_MODE);
		if (t==(int)','){
			lexer->skip();
			lexer->skipS();
			TKVMCode_base *stmt=compileStatement(false);
			if (stmt)
				tmplist.push_back(stmt);
			// else ','�ǹԤ�����äƤ��롩 �⤷���� ', ,'�ߤ����ˤʤäƤ롩
			// �����ǡ�NULLʸ����פ������櫓�ˤϤ����ʤ���
		}else if (t==')'){
			// ��λ
			break;
		}else{
			lexer->error(RC.S(ERR_COMPILER_STATEMENTLIST_SEPARATOR_NOT_FOUND));
			lexer->simpleSkipTo(',');
			if (lexer->peek()!=(int)',')
				break;
		}
	}
	list.insert(list.end(), tmplist.begin(), tmplist.end());
	return tmplist.size();
}

//-------------------------------------------------------------------------
// ʸ ( WS (Word WS)* / S (Word S)* )
// NULL���֤��ʤ���
TKVMCode_base *TKawariCompiler::compileStatement(bool noret, int m){
	vector<TKVMCode_base *> tmplist;
	if (noret){
		// ���Ԥ�����ʤ�
		while (lexer->hasNext()){
			// �㤤�Ϥ��ιԤΤߡ�
			lexer->skipS();
			TKVMCode_base *code=compileWord(m);
			if (code)
				tmplist.push_back(code);
			else
				break;
		}
	}else{
		// ���Ԥ����(���餯�֥�å���Τ�)
		while (lexer->hasNext()){
			// �㤤�Ϥ��ιԤΤߡ�
			lexer->skipWS();
			TKVMCode_base *code=compileWord(m);
			if (code)
				tmplist.push_back(code);
			else
				break;
		}
	}
	if (tmplist.size()==0){
		return new TKVMCodeString("");
	}else if (tmplist.size()==1){
		return tmplist[0];
	}else{
		return new TKVMCodeList(tmplist);
	}
}

//-------------------------------------------------------------------------
// ������ץ�ʸ ( WS (Word WS)* )
// NULL���֤��ʤ���
TKVMCode_base *TKawariCompiler::compileScriptStatement(void){
	vector<TKVMCode_base *> tmplist;

	// �ǽ��ñ����ɤࡣ
	Token::Type t=lexer->skipWS(SCRIPT_MODE);
	if (t==Token::T_LITERAL){
		string str=lexer->getLiteral(SCRIPT_MODE);
		if (str=="if"){
			return compileScriptIF();
		}else{
			lexer->UngetChars(str.size());
		}
	}

	// ���Ԥ����
	while (lexer->hasNext()){
		lexer->skipWS();
		TKVMCode_base *code=compileWord(SCRIPT_MODE);
		if (code){
			tmplist.push_back(code);
		}else{
			break;
		}
	}
	if (!tmplist.size()) return NULL;
	return new TKVMCodeScriptStatement(tmplist);
}
//-------------------------------------------------------------------------
// if
TKVMCode_base *TKawariCompiler::compileScriptIF(void){
	// 'if'�θ夫��Ϥޤ�
	vector<TKVMCode_base *> condlist;
	vector<TKVMCode_base *> list;

	while (lexer->hasNext()){
		// ���ʸ
		lexer->skipWS();
		TKVMCode_base *code=compileWord(SCRIPT_MODE);
		if (!code) break;
		condlist.push_back(code);

		// �¹�ʸ
		lexer->skipWS();
		code=compileWord(SCRIPT_MODE);
		if (!code) break;
		list.push_back(code);

		// ͽ���õ��
		Token::Type t=lexer->skipWS(SCRIPT_MODE);
		if (t!=Token::T_LITERAL) break;

		string str=lexer->getLiteral(SCRIPT_MODE);
		if (str=="else"){
			t=lexer->skipWS(SCRIPT_MODE);
			if (t==Token::T_LITERAL){
				str=lexer->getLiteral(SCRIPT_MODE);
				if (str=="if"){
					continue;
				}else{
					lexer->UngetChars(str.size());
					code=compileWord(SCRIPT_MODE);
					if (!code) break;
					list.push_back(code);
					break;
				}
			}else{
				code=compileWord(SCRIPT_MODE);
				if (!code) break;
				list.push_back(code);
				break;
			}
		}else{
			lexer->UngetChars(str.size());
			break;
		}
	}

	return new TKVMKISCodeIF(condlist, list);
}

//-------------------------------------------------------------------------
// ñ�� ( (QLiteral | Literal | Block | Subst | S)* )
// ������ץ���ñ�� ( (QLiteral | Literal | Block | Subst)* )
// NULL���֤���
TKVMCode_base *TKawariCompiler::compileWord(int m){
	TKawariLexer::Mode mode=(TKawariLexer::Mode)m;
	vector<TKVMCode_base *> tmplist;
	bool escape=false;	// �롼��æ�Хե饰
	while (lexer->hasNext()&&(!escape)){
		Token::Type t=lexer->peek(mode);
		TKVMCode_base *code=NULL;
		if((t==Token::T_LITERAL)||(t==Token::T_QLITERAL)){
			string s;
			bool force=false;
			while (true){
				Token::Type t=lexer->peek(mode);
				if (t==Token::T_LITERAL){
					string tmps=lexer->getLiteral(mode);
					if (!tmps.size()){
						// ���Ǥ⤢���������ä���
						escape=true;
						break;
					}
					s+=tmps;
				}else if (t==Token::T_QLITERAL){
					s+=TKawariLexer::DecodeQuotedString(lexer->getQuotedLiteral());
					force=true;
				}else{
					break;
				}
			}
			if (s.size()||force)
				code=new TKVMCodeString(s);
			else
				break;	// ̵�¥롼����
		}else if(t==(int)'('){
			// Block
			code=compileBlock();
		}else if(t==(int)'$'){
			// Subst
			code=compileSubst();
		}else{
			break;
		}
		if (code)
			tmplist.push_back(code);
	}
	if (tmplist.size()==0){
		return NULL;
	}else if(tmplist.size()==1){
		return tmplist[0];
	}else{
		return new TKVMCodeList(tmplist);
	}
}
//-------------------------------------------------------------------------
// �ִ� ( '$' ( EntryCallSubst | EntryIndexSubst | InlineScriptSubst | ExprSubst ) )
// NULL���֤�
TKVMCode_base *TKawariCompiler::compileSubst(void){
	if (lexer->peek(ID_MODE)!=(int)'$'){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_SUBST));
		lexer->getRestOfLine();
		return NULL;
	}
	lexer->skip();
	Token::Type t=lexer->peek(ID_MODE);
	if (t==(int)'{'){
		// Macro Entry
		return compileEntryCallSubst();
	}else if(t==(int)'('){
		// Macro Inline
		return compileInlineScriptSubst();
	}else if((t==Token::T_LITERAL)||(t==(int)'$')){
		// Macro Array
		return compileEntryIndexSubst();
	}else if(t==(int)'['){
		// Macro Expr
		return compileExprSubst();
	}else{
		return NULL;
	}
}

//-------------------------------------------------------------------------
// ����饤�󥹥���ץ� ( '(' ScriptStatementSeq ') )
// NULL���֤��ʤ�
TKVMCode_base *TKawariCompiler::compileInlineScriptSubst(void) {
	if (lexer->peek(ID_MODE)!=(int)'('){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_INLINE_SCRIPT));
		lexer->getRestOfLine();
		return NULL;
	}
	lexer->skip();

	vector<TKVMCode_base *> tmplist;

	// �ǽ�ιԤ��ɤ�
	TKVMCode_base *code = compileScriptStatement();
	if (code)
		tmplist.push_back(code);

	bool closed=false;
	while (lexer->hasNext()){
		Token::Type t=lexer->skipWS(SCRIPT_MODE);
		if (t==(int)';'){
			// ���ι�
			lexer->skip();
			TKVMCode_base *code = compileScriptStatement();
			if (code)
				tmplist.push_back(code);
		}else if (t==(int)')'){
			// That's end. �⤦�����
			lexer->skip();
			closed=true;
			break;
		}else{
//			lexer->error("Illegal character in Inline Script");
			break;
		}
	}
	if (!closed)
		lexer->error(RC.S(ERR_COMPILER_SCRIPT_SUBST_NOT_CLOSED));

	return new TKVMCodeInlineScript(tmplist);
}

//-------------------------------------------------------------------------
// �֥�å� ( '(' WS Statement ')' )
// ɬ������'('����ʤ���Фʤ�ʤ�
// NULL���֤����Ȥ����롣
TKVMCode_base *TKawariCompiler::compileBlock(void) {
	if (lexer->peek(ID_MODE)!=(int)'('){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_BLOCK));
		lexer->getRestOfLine();
		return NULL;
	}
	lexer->skip();
	Token::Type t=lexer->skipWS(BLOCK_MODE);

	if (t==(int)')'){
		// �����ʤ꽪λ
		lexer->skip();
		return NULL;
	}

	TKVMCode_base *code=compileStatement(false, BLOCK_MODE);

	t=lexer->skipWS(BLOCK_MODE);
	if (t==(int)')'){
		// ��λ
		lexer->skip();
	}else{
		lexer->error(RC.S(ERR_COMPILER_BLOCK_NOT_CLOSED));
	}
	return code;
}


//--------------------------------------------------------------------------
// ź�����դ�����ȥ�ƤӽФ��ʰ���
// ( EntryWord '[' WS Expr WS ']' )
// ��Ƭ��'$'�ϴ��˾ä��Ƥ��롣
TKVMCode_base *TKawariCompiler::compileEntryIndexSubst(void){
	// �����Ϥ��ʤ��㳰Ū�����顼������Ȥ����ʤ�ȴ���롣

	TKVMCode_base *tmp_id=compileEntryWord();
	if (!tmp_id){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_INDEX));
		return NULL;
	}

	Token::Type t=lexer->skipWS(ID_MODE);
	if (t!=(int)'['){
		lexer->error(RC.S(ERR_COMPILER_INDEX_BRACKET_NOT_FOUND));
		delete tmp_id;
		return NULL;
	}

	TKVMCode_base *tmp_expr=compileExprSubst();
	if (!tmp_expr){
		lexer->error(RC.S(ERR_COMPILER_INDEX_EXPR_NOT_FOUND));
		delete tmp_id;
		return NULL;
	}

	return new TKVMCodeEntryIndex(tmp_id, tmp_expr);
}

//--------------------------------------------------------------------------
// ����ȥ�̾ ( (IdLiteral | Subst)+ WS )
TKVMCode_base *TKawariCompiler::compileEntryWord(void){
	vector<TKVMCode_base *> tmplist;

	lexer->skipWS();
	while (lexer->hasNext()){
		Token::Type t=lexer->peek(ID_MODE);
		if (t==Token::T_LITERAL){
			TKVMCode_base *code=new TKVMCodeIDString(lexer->getLiteral(ID_MODE));
			tmplist.push_back(code);
		}else if (t==(int)'$'){
			tmplist.push_back(compileSubst());
		}else{
			break;
		}
	}
	if (!tmplist.size()){
		return NULL;
	}else if (tmplist.size()==1){
		return tmplist[0];
	}else{
		return new TKVMCodeList(tmplist);
	}
}

//=====================================================================
// ��
//=====================================================================

// �� ( '[' Expression  ']' )
// NULL���֤뤫�⡣
TKVMCode_base *TKawariCompiler::compileExprSubst(void){
	if (lexer->peek(ID_MODE)!=(int)'['){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_EXPR));
		lexer->getRestOfLine();
		return NULL;
	}
	lexer->skip();
	TKVMExprCode_base *code=compileExpr0();
	if (!code) {
		lexer->simpleSkipTo(']');
		lexer->skip();
		return NULL;
	}

	TKVMCodeExpression *ret=new TKVMCodeExpression(code);

	if (lexer->peek(ID_MODE)!=(int)']'){
		lexer->error(RC.S(ERR_COMPILER_EXPR_NOT_CLOSED));
	}else{
		lexer->skip();
	}
	return ret;
}

// ������ ( '||' )
TKVMExprCode_base *TKawariCompiler::compileExpr0(void){
	TKVMExprCode_base *l=compileExpr1();
	if (!l) return NULL;
	while(true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="||"){
			TKVMExprCode_base *r=compileExpr1();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'||'"); return l; }
			l=new TKVMExprCodeLOR(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// ������ ( '&&' )
TKVMExprCode_base *TKawariCompiler::compileExpr1(void){
	TKVMExprCode_base *l=compileExpr2();
	if (!l) return NULL;
	while(true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="&&"){
			TKVMExprCode_base *r=compileExpr2();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'&&'"); return l; }
			l=new TKVMExprCodeLAND(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// ���� ( '=' | '==' | '!=' | '=~' | '!~" )
TKVMExprCode_base *TKawariCompiler::compileExpr2(void){
	TKVMExprCode_base *l=compileExpr3();
	if (!l) return NULL;
	lexer->skipWS();
	Token token=lexer->next(ID_MODE);
	if ((token.str=="=")||(token.str=="==")){
		TKVMExprCode_base *r=compileExpr3();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'=='"); return l; }
		return new TKVMExprCodeEQ(l, r);
	}else if (token.str=="!="){
		TKVMExprCode_base *r=compileExpr3();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'!='"); return l; }
		return new TKVMExprCodeNEQ(l, r);
	}else if (token.str=="=~"){
		TKVMExprCode_base *r=compileExpr3();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'=~'"); return l; }
		return new TKVMExprCodeMATCH(l, r);
	}else if (token.str=="!~"){
		TKVMExprCode_base *r=compileExpr3();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'!~'"); return l; }
		return new TKVMExprCodeNMATCH(l, r);
	}else{
		lexer->UngetChars(token.str.size());
		return l;
	}
}

// ��� ( '<' | '<=' | '>' | '>=' )
TKVMExprCode_base *TKawariCompiler::compileExpr3(void){
	// ���󤹤뤳�ȤϤ������ʤ�
	TKVMExprCode_base *l=compileExpr4();
	if (!l) return NULL;
	lexer->skipWS();
	Token token=lexer->next(ID_MODE);
	if (token.str=="<"){
		TKVMExprCode_base *r=compileExpr4();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'<'"); return l; }
		return new TKVMExprCodeLT(l, r);
	}else if (token.str=="<="){
		TKVMExprCode_base *r=compileExpr4();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'<='"); return l; }
		return new TKVMExprCodeLTE(l, r);
	}else if (token.str==">"){
		TKVMExprCode_base *r=compileExpr4();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'>'"); return l; }
		return new TKVMExprCodeGT(l, r);
	}else if (token.str==">="){
		TKVMExprCode_base *r=compileExpr4();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'>='"); return l; }
		return new TKVMExprCodeGTE(l, r);
	}else{
		lexer->UngetChars(token.str.size());
		return l;
	}
}

// �ӥå�OR ('|' | '^')
TKVMExprCode_base *TKawariCompiler::compileExpr4(void){
	TKVMExprCode_base *l=compileExpr5();
	if (!l) return NULL;
	lexer->skipWS();
	while(true){
		Token token=lexer->next(ID_MODE);
		if (token.str=="|"){
			TKVMExprCode_base *r=compileExpr5();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'|'"); return l; }
			l=new TKVMExprCodeBOR(l, r);
		}else if (token.str=="^"){
			TKVMExprCode_base *r=compileExpr5();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'~'"); return l; }
			l=new TKVMExprCodeBXOR(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// �ӥå�AND ('&')
TKVMExprCode_base *TKawariCompiler::compileExpr5(void){
	TKVMExprCode_base *l=compileExpr6();
	if (!l) return NULL;
	while(true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="&"){
			TKVMExprCode_base *r=compileExpr6();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'&'"); return l; }
			l=new TKVMExprCodeBAND(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// �ø��� ('+' | '-')
TKVMExprCode_base *TKawariCompiler::compileExpr6(void){
	TKVMExprCode_base *l=compileExpr7();
	if (!l) return NULL;
	while(true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="+"){
			TKVMExprCode_base *r=compileExpr7();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'+'"); return l; }
			l=new TKVMExprCodePLUS(l, r);
		}else if (token.str=="-"){
			TKVMExprCode_base *r=compileExpr7();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'-'"); return l; }
			l=new TKVMExprCodeMINUS(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// ����� ('*' | '/' | '%')
TKVMExprCode_base *TKawariCompiler::compileExpr7(void){
	TKVMExprCode_base *l=compileExpr8();
	if (!l) return NULL;
	while(true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="*"){
			TKVMExprCode_base *r=compileExpr8();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'*'"); return l; }
			l=new TKVMExprCodeMUL(l, r);
		}else if (token.str=="/"){
			TKVMExprCode_base *r=compileExpr8();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'/'"); return l; }
			l=new TKVMExprCodeDIV(l, r);
		}else if (token.str=="%"){
			TKVMExprCode_base *r=compileExpr8();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'%'"); return l; }
			l=new TKVMExprCodeMOD(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// ñ��黻�� ('+' | '-' | '!' | '~')
TKVMExprCode_base *TKawariCompiler::compileExpr8(void){
	// ñ��黻�Ҽ��Τϱ����
	lexer->skipWS();
	Token token=lexer->next(ID_MODE);
	if (token.str=="+"){
		TKVMExprCode_base *c=compileExpr8();
		if (!c) return NULL;
		return new TKVMExprCodeUPLUS(c);
	}else if (token.str=="-"){
		TKVMExprCode_base *c=compileExpr8();
		if (!c) return NULL;
		return new TKVMExprCodeUMINUS(c);
	}else if (token.str=="!"){
		TKVMExprCode_base *c=compileExpr8();
		if (!c) return NULL;
		return new TKVMExprCodeNOT(c);
	}else if (token.str=="~"){
		TKVMExprCode_base *c=compileExpr8();
		if (!c) return NULL;
		return new TKVMExprCodeCOMP(c);
	}else{
		lexer->UngetChars(token.str.size());
		return compileExpr9();
	}
}

// �߾� ( '**' )
TKVMExprCode_base *TKawariCompiler::compileExpr9(void){
	TKVMExprCode_base *l=compileExprFactor();
	if (!l) return NULL;
	while (true){
		lexer->skipWS();
		Token token=lexer->next(ID_MODE);
		if (token.str=="**"){
			TKVMExprCode_base *r=compileExprFactor();
			if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'**'"); return l; }
			l=new TKVMExprCodePOW(l, r);
		}else{
			lexer->UngetChars(token.str.size());
			return l;
		}
	}
}

// Expr���� ( '(' Expression ')' | ExprWord )
TKVMExprCode_base *TKawariCompiler::compileExprFactor(void){
	Token::Type t=lexer->skipWS(ID_MODE);
	if (t==(int)'('){
		lexer->skip();
		TKVMExprCode_base *c=compileExpr0();
		if (!c) return NULL;
		t=lexer->skipWS(ID_MODE);
		if (t!=(int)')'){
			lexer->error(RC.S(ERR_COMPILER_EXPR_BLOCK_NOT_CLOSED));
		}else{
			lexer->skip();
		}
		return new TKVMExprCodeGroup(c);
	}else{
		return compileExprWord();
	}
}

// Exprñ�� ( (NumericLiteral | QuotedLiteral | Subst)+ WS )
TKVMExprCode_base *TKawariCompiler::compileExprWord(void){
	lexer->skipWS();
	vector<TKVMCode_base *> tmplist;
	bool escape=false;	// �롼��æ�Хե饰
	while (lexer->hasNext()&&(!escape)){
		Token::Type t=lexer->peek(ID_MODE);
		if ((t==Token::T_QLITERAL)||(t==Token::T_LITERAL)){
			string s;
			while (true){
				Token::Type t=lexer->peek(ID_MODE);
				if (t==Token::T_QLITERAL){
					s+=TKawariLexer::DecodeQuotedString(lexer->getQuotedLiteral());
				}else if (t==Token::T_LITERAL){
					// ���ʿ��ͤ���������ʤ�
					string tmps=lexer->getDecimalLiteral();
					if (!tmps.size()){
						lexer->error(RC.S(ERR_COMPILER_ILLCHAR_IN_EXPRESSION));
						escape=true;
						break;
					}
					s+=tmps;
				}else{
					break;
				}
			}
			TKVMCode_base *code=new TKVMCodeString(s);
			tmplist.push_back(code);
		}else if (t==(int)'$'){
			tmplist.push_back(compileSubst());
		}else{
			break;
		}
	}
	if (!tmplist.size()){
		return NULL;
	}else if (tmplist.size()==1){
		return new TKVMExprCodeWord(tmplist[0]);
	}else{
		return new TKVMExprCodeWord(new TKVMCodeList(tmplist));
	}
}

//=====================================================================
// ����ȥ꽸��黻��
//=====================================================================

//-------------------------------------------------------------------------
// ����ȥ�ƤӽФ� ( '{' EntryExpr '}' )
TKVMCode_base *TKawariCompiler::compileEntryCallSubst(void) {
	if (lexer->peek(ID_MODE)!=(int)'{'){
		lexer->error(RC.S(ERR_COMPILER_INTERNAL_ENTRYCALL));
		lexer->getRestOfLine();
		return NULL;
	}
	lexer->skip();
	if (lexer->skipWS(ID_MODE)==(int)'-'){
		// ��ο������򻲾�
		lexer->skip();
		string fnum=lexer->getDecimalLiteral();
		if (lexer->skipWS(ID_MODE)!=(int)'}'){
			lexer->error(RC.S(ERR_COMPILER_ENTRYCALL_NOT_CLOSED));
		}else{
			lexer->skip();
		}
		return new TKVMCodeHistoryCall(-1*atoi(fnum.c_str()));
	}

	TKVMSetCode_base *code=compileSetExpr0();

	if (lexer->peek(ID_MODE)!=(int)'}'){
		lexer->error(RC.S(ERR_COMPILER_ENTRYCALL_NOT_CLOSED));
	}else{
		lexer->skip();
	}

	if (!code) return NULL;

	// ���ƥ����å�
	const TKVMSetCodeWord *cw=dynamic_cast<const TKVMSetCodeWord *>(code);
	if (cw){
		const TKVMCodeIDString *cs=cw->GetIfPVW();
		if (cs){
			if (IsInteger(cs->s)){
				// ���򻲾�
				TKVMCode_base *ret=new TKVMCodeHistoryCall(atoi(cs->s.c_str()));
				delete code;
				return ret;
			}else{
				// ��貾��ñ��
				TKVMCode_base *ret=new TKVMCodePVW(cs->s);
				delete code;
				return ret;
			}
		}
	}

	return new TKVMCodeEntryCall(code);
}

// �º� ('+' | '-')
TKVMSetCode_base *TKawariCompiler::compileSetExpr0(void){
	TKVMSetCode_base *l=compileSetExpr1();
	if (!l) return NULL;
	lexer->skipWS();
	Token token=lexer->next(ID_MODE);
	if (token.str=="+"){
		TKVMSetCode_base *r=compileSetExpr0();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'+'"); return l; }
		return new TKVMSetCodePLUS(l, r);
	}else if (token.str=="-"){
		TKVMSetCode_base *r=compileSetExpr0();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'-'"); return l; }
		return new TKVMSetCodeMINUS(l, r);
	}else{
		lexer->UngetChars(token.str.size());
		return l;
	}
}

// �� ('&')
TKVMSetCode_base *TKawariCompiler::compileSetExpr1(void){
	TKVMSetCode_base *l=compileSetExprFactor();
	if (!l) return NULL;
	lexer->skipWS();
	Token token=lexer->next(ID_MODE);
	if (token.str=="&"){
		TKVMSetCode_base *r=compileSetExpr1();
		if (!r) { lexer->error(RC.S(ERR_COMPILER_PARSE_ERROR_AFTER)+"'&'"); return l; }
		return new TKVMSetCodeAND(l, r);
	}else{
		lexer->UngetChars(token.str.size());
		return l;
	}
}

// ����黻������ ( '(' EntryExpr ')' | EntryWord )
TKVMSetCode_base *TKawariCompiler::compileSetExprFactor(void){
	Token::Type t=lexer->skipWS(ID_MODE);
	if (t==(int)'('){
		lexer->skip();
		TKVMSetCode_base *c=compileSetExpr0();
		if (!c) return NULL;
		t=lexer->skipWS(ID_MODE);
		if (t!=(int)')'){
			lexer->error(RC.S(ERR_COMPILER_SETEXPR_BLOCK_NOT_CLOSED));
		}else{
			lexer->skip();
		}
		return c;
	}else{
		return compileSetExprWord();
	}
}

// ����黻��ñ�� ( (IdLiteral | Subst)+ WS )
TKVMSetCode_base *TKawariCompiler::compileSetExprWord(void){
	vector<TKVMCode_base *> tmplist;

	lexer->skipWS();
	while (lexer->hasNext()){
		Token::Type t=lexer->peek(ID_MODE);
		if (t==Token::T_LITERAL){
			TKVMCode_base *code=new TKVMCodeIDString(lexer->getLiteral(ID_MODE));
			tmplist.push_back(code);
		}else if (t==(int)'$'){
			tmplist.push_back(compileSubst());
		}else{
			break;
		}
	}
	if (!tmplist.size()){
		return NULL;
	}else if (tmplist.size()==1){
		return new TKVMSetCodeWord(tmplist[0]);
	}else{
		return new TKVMSetCodeWord(new TKVMCodeList(tmplist));
	}
}
//---------------------------------------------------------------------------
