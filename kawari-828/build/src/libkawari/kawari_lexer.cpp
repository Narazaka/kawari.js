//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// �������
//
//      Programed by Suikyo
//
//  2002.04.18  Phase 8.0.0   ��ľ��
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_lexer.h"
#include "misc/misc.h"
#include "misc/l10n.h"
#include "libkawari/kawari_crypt.h"
#include "libkawari/kawari_log.h"
#include "libkawari/kawari_rc.h"
using namespace kawari::resource;
//---------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <fstream>
#ifdef HAVE_SSTREAM_H
#include <sstream>
#else
#include <strstream>
#endif
#include <cctype>
using namespace std;
//---------------------------------------------------------------------------
// EntryID ::= ( [0-9A-Za-z._?@] | NAChar ) +
static char id_map[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// BareLiteral.mode1 ::= ( [#x21-#x7E] - ["$'();] | NAChar | [ \t] ) +
static char literal_map1[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// BareLiteral.mode2 ::= ( [#x21-#x7E] - ["$'(),] | NAChar ) +
static char literal_map2[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// BareLiteral.mode3 ::= ( [#x21-#x7E] - ["$'()] | NAChar ) +
static char literal_map3[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static char *lex_map[4] = { id_map, literal_map1, literal_map2, literal_map3 };

//---------------------------------------------------------------------------
// ���󥹥ȥ饯��
TKawariLexer::TKawariLexer (
	istream &input, TKawariLogger &lgr, string filename, bool preprocess, int lineno)
	 : pp(new TKawariPreProcessor(input, lineno, preprocess)), fn(filename), logger(lgr) { }

//---------------------------------------------------------------------------
// �ǥ��ȥ饯��
TKawariLexer::~TKawariLexer(void){
	if (pp) delete pp;
}

//---------------------------------------------------------------------------
// ���Υ٥���ƥ��ȡ����������
// �����٥���ƥ��Ǥʤ��ä��顢""���֤롣ͽ��ʬ���äƤ�����ˤ����Ȥ����ȡ�
string TKawariLexer::getLiteral(Mode m){
	if (!hasNext()) return ("");

	char ch;
	char *map = lex_map[(int)m];
	string ret;
	ret.reserve(20);	// ���󤯤餤�Ǥ���
	while(pp->getch(ch)){
		if (iskanji1st(ch)) {
			ret+=ch;
			if (pp->getch(ch))
				ret+=ch;
			else
				break;
		}else if (map[ch]){
			ret+=ch;
		} else {
			pp->unget();
			break;
		}
	}
	if(((m==LITERAL_MODE)&&(ch==','))||
	   ((m==LITERAL_MODE3)&&(ch==')'))){
		// �ǥ�ߥ����ζ���Ϻ��
		string::size_type litend=ret.find_last_not_of(" \t\x0d\x0a");
		ret=ret.substr(0, litend+1);
	}else{
		ret.resize(ret.size());
	}
//	cout << "getLiteral(" << m << ") = " << ret << endl;
	return ret;
}

//---------------------------------------------------------------------------
// ���Υ������ȥ�ƥ��ȡ�����(��ǥ�����)���֤�
// �����������ȥ�ƥ��Ǥʤ��ä���""���֤롣
string TKawariLexer::getQuotedLiteral(void){
	static const string QUOTE("\"'");			// ��������ʸ����
	if (!hasNext()) return ("");

	char ch, quote;
	pp->peek(ch);
	if (QUOTE.find(ch)==string::npos){
		return "";
	}else{
		quote = ch;
		pp->getch(ch);
	}
	string result;
	result.reserve(20);	// ����ʤ��
	result+=quote;

	bool closed = false;
	while(pp->getch(ch)) {
		if(iskanji1st(ch)) {
			// ����
			result+=ch;
			if(pp->getch(ch))
				result+=ch;
		}else if(ch==quote){
			// ��λ
			result+=ch;
			closed=true;
			break;
		}else if(ch=='\\'){
			// ����������
			result+=ch;
			pp->getch(ch);
			if ((ch==quote)||(ch=='\\'))
				result+=ch;
			else
				pp->unget();
		}else if(0x0a==ch){
			// ����
			error(RC.S(ERR_LEXER_EOL_IN_QUOTED_STRING));
			closed=true;
			break;
		}else{
			result+=ch;
		}
	}
	if (!closed)
		// �������EOF��ã���Ƥ��ޤä�
		error(RC.S(ERR_LEXER_EOL_IN_QUOTED_STRING));
	return result;
}
//---------------------------------------------------------------------------
// '�ޤ���"�ǰϤޤ줿ʸ���󤫤饯������ʸ����Ϥ���
string TKawariLexer::DecodeQuotedString(const string& orgsen)
{
	if(orgsen.size()==0) return("");

	char quote=orgsen[0];

	unsigned int pos=1;
	string retstr;
	retstr.reserve(orgsen.size());

	unsigned int max=orgsen.size();
	while(pos<max) {
		if(orgsen[pos]==quote) break;

		if(orgsen[pos]=='\\') {
			if(((pos+1)<orgsen.size())&&((orgsen[pos+1]==quote)||(orgsen[pos+1]=='\\')))
				pos++;
		}

		if(iskanji1st(orgsen[pos])) {
			// ����
			if((pos+1)<orgsen.size()) retstr+=orgsen[pos++];
		}
		retstr+=orgsen[pos++];
	}

	retstr.resize(retstr.size());
	return(retstr);
}
//---------------------------------------------------------------------------
// !!! check type, before use this !!!
// [0-9]+
// ����ʸ����
string TKawariLexer::getDecimalLiteral(void){
	char ch;
	string result;
	while(pp->getch(ch)){
		if (isdigit(ch)){
			result+=ch;
		}else{
			pp->unget();
			break;
		}
	}
	return result;
}

//---------------------------------------------------------------------------
// 1ʸ�������å�
int TKawariLexer::skip(void){
	char ch;
	if (pp->getch(ch))
		return ch;
	else
		return -1;
}

//---------------------------------------------------------------------------
// ʸ������ȡ����󥿥��פ�����
inline Token::Type TKawariLexer::checkType(Mode m, char ch) const{
	static const string CRLF("\x0d\x0a");		// CRLF
	static const string WHITESPACE(" \t");		// ����ʸ��
	static const string QUOTE("\"'");			// ��������ʸ����

	if (pp->IsWaitingModeSwitch()){
		return Token::T_MODESWITCH;
	}else if ((lex_map[(int)m][ch]|(char)iskanji1st(ch))){
		// �٥�ʸ����
		return Token::T_LITERAL;
	}else if (QUOTE.find(ch)!=string::npos){
		// ��������ʸ����
		return Token::T_QLITERAL;
	}else if (WHITESPACE.find(ch)!=string::npos){
		// ����ʸ��
		return Token::T_SPACE;
	}else if (CRLF.find(ch)!=string::npos){
		// ����ʸ��
		return Token::T_EOL;
	}else{
		// �ü�ʸ��
		return Token::Type(int (ch) & 0xFF);
	}
}

//---------------------------------------------------------------------------
Token::Type TKawariLexer::peek(Mode m){
	if (!hasNext())
		return (Token::T_EOF);

	char ch;
	pp->peek(ch);
	return checkType(m, ch);
}

//---------------------------------------------------------------------------
Token TKawariLexer::next(Mode m){
	static const char CR = 0x0d;				// CR
	static const char LF = 0x0a;				// LF
	static const string WHITESPACE(" \t");		// ����ʸ��
	static const string QUOTE("\"'");			// ��������ʸ����
	static const string NON_LITERAL("\"$'(),; \t\x0d\x0a");
						// �٥�ʸ����˻Ȥ��ʤ�ʸ��(������7E�ʲ�)
	Token result;
	if (!hasNext()) return (result.set(Token::T_EOF));

	char ch;
	pp->getch(ch);
	if ((lex_map[(int)m][ch]|(char)iskanji1st(ch))){
		// �٥�ʸ����
		pp->unget();
		string str=getLiteral(m);
		result.set(Token::T_LITERAL, str);
	}else if (QUOTE.find(ch)!=string::npos){
		// ��������ʸ����
		pp->unget();
		result.set(Token::T_QLITERAL, getQuotedLiteral());
	}else if (WHITESPACE.find(ch)!=string::npos){
		// ����ʸ��
		string ws;
		ws.reserve(5); // ���Τ��餤��
		ws+=ch;
		while (pp->getch(ch)){
			if (WHITESPACE.find(ch)==string::npos){
				pp->unget();
				break;
			}
			ws += ch;
		}
		result.set(Token::T_SPACE, ws);
	}else if (CR==ch){
		// ����ʸ��
		if (!pp->eof()){
			pp->getch(ch);
			if (LF==ch){
				result.set(Token::T_EOL, "\x0d\x0a");
			}else{
				pp->unget();
				result.set(Token::T_EOL, ch);
			}
		}else{
			result.set(Token::T_EOL, ch);
		}
	}else if (LF==ch){
		// ����ʸ��
		result.set(Token::T_EOL, ch);
	}else{
		// �ü�ʸ��
		// 2ʸ���α黻�ҥ����å� '==' '!=' '<=' '>=' '=~' '!~' '**' '||' '&&'
		if (m==ID_MODE){
			result.set(Token::Type(int(ch) & 0xFF), ch);
			if (ch=='='){
				pp->getch(ch);
				if (ch=='=') result.set("==");
				else if (ch=='~') result.set("=~");
				else pp->unget();
			}else if(ch=='!'){
				pp->getch(ch);
				if (ch=='=') result.set("!=");
				else if (ch=='~') result.set("!~");
				else pp->unget();
			}else if(ch=='<'){
				pp->getch(ch);
				if (ch=='=') result.set("<=");
				else pp->unget();
			}else if(ch=='>'){
				pp->getch(ch);
				if (ch=='=') result.set(">=");
				else pp->unget();
			}else if(ch=='*'){
				pp->getch(ch);
				if (ch=='*') result.set("**");
				else pp->unget();
			}else if(ch=='|'){
				pp->getch(ch);
				if (ch=='|') result.set("||");
				else pp->unget();
			}else if(ch=='&'){
				pp->getch(ch);
				if (ch=='&') result.set("&&");
				else pp->unget();
			}
		}else{
			result.set(Token::Type(int(ch) & 0xFF), ch);
		}
	}
	// ���ｪλ
	//	lt = result;
	return(result);
}
//---------------------------------------------------------------------------
// �ե�����̾���֤�
const string &TKawariLexer::getFileName(void) const {
	return fn;
}
//---------------------------------------------------------------------------
// ���ߤι��ֹ���֤�
int TKawariLexer::getLineNo(void) const{
	// ¿ʬunget�Хåե��˲��Ԥ�����Ȥ���-1���٤���
	return pp->getLineNo();
}
//---------------------------------------------------------------------------
// ���߹ԤλĤ�������֤�
string TKawariLexer::getRestOfLine(void){
	return pp->getline();
}
//---------------------------------------------------------------------------
// �����ӥ��᥽�å�
// ����ʸ���Ȳ���ʸ���Υȡ�������ɤ����Ф���
void TKawariLexer::skipWS(){
	static const string WS(" \t\x0d\x0a");
	char ch;
	while (pp->peek(ch)){
		if (WS.find(ch)==string::npos)
			return;
		else
			pp->getch(ch);
	}
}
//---------------------------------------------------------------------------
// �����ӥ��᥽�å�
// ����ʸ���Ȳ���ʸ���Υȡ�������ɤ����Ф���
// ���μ��Υȡ�����Υ����פ��֤�
Token::Type TKawariLexer::skipWS(Mode m){
	static const string WS(" \t\x0d\x0a");
	char ch;
	while (pp->peek(ch)){
		if (WS.find(ch)==string::npos)
			return checkType(m, ch);
		else
			pp->getch(ch);
	}
	return Token::T_EOF;
}
//---------------------------------------------------------------------------
// �����ӥ��᥽�å�
// ����ʸ���Υȡ�������ɤ����Ф������μ��Υȡ�����Υ����פ��֤�
// ����ʸ�������Ф��ʤ���
Token::Type TKawariLexer::skipS(Mode m){
#if 0
	while (hasNext()){
		Token::Type t=peek(m);
		if (t!=Token::T_SPACE)
			return t;
		else
			next(m);
	}
	return Token::T_EOF;
#else
	static const string SPACE(" \t");
	char ch;
	while (pp->peek(ch)){
		if (SPACE.find(ch)==string::npos)
			return checkType(m, ch);
		else
			pp->getch(ch);
	}
	return Token::T_EOF;
#endif
}
//---------------------------------------------------------------------------
// �����ӥ��᥽�å�
// ʸ����򥨥�ȥ�̾�ǻ��Ѳ�ǽ��ʸ����˥��󥳡��ɤ���
string TKawariLexer::EncodeEntryName(const string &orgsen){
	char *map = lex_map[(int)ID_MODE];
	string entsen(orgsen);
	unsigned cntsen = entsen.size();

	for(unsigned int pos=0;pos<cntsen;pos++) {
		char c = entsen[pos];
		if(iskanji1st(c)) {
			// �����ϼ���
			if((pos+1)<cntsen) pos++;
			else break;
		} else if(!map[c]) {
			// �Ȥ��ʤ�ʸ��������'_'���Ѵ�
			entsen[pos] = '_';
		}
	}
	
	return(entsen);
}
//---------------------------------------------------------------------------
// ����Ĺ�������꡼�ɥݥ��󥿤��᤹
bool TKawariLexer::UngetChars(unsigned int length){
	for (unsigned int i=0; i<length; i++)
		if (!pp->unget())
			return false;
	return true;
}
//---------------------------------------------------------------------------
// ����ʸ���μ����ޤǥ����å� (ʸˡ���顼�����������)
// noret : ���Ԥ�����ʤ����true
bool TKawariLexer::simpleSkipTo(char ch, bool noret){
	static const string CRLF("\x0d\x0a");
	while (hasNext()){
		char c;
		pp->peek(c);
		if (c==ch)
			return true;
		else if (noret && (CRLF.find(c)!=string::npos))
			return true;
		pp->getch(c);
	}
	return false;
}

//--------------------------------------------------------------------------
// TKawariPreProcessor
//--------------------------------------------------------------------------
bool TKawariPreProcessor::processNextLine(void){
	if (is.eof()) return false;

	// ��������
	std::getline(is, linebuf, '\x0a');
	if((linebuf.size()>0)&&(linebuf[linebuf.size()-1]=='\x0d')) linebuf.erase(linebuf.size()-1);
	ln++;
	pos=0;
	if (pp){
		// �ץ�ץ���

		// �������
		if (CheckCrypt(linebuf))
			linebuf=DecryptString(linebuf);

		if (mc && linebuf.find(":endrem")==0){
			linebuf="";
			mc=false;
		}else if (mc){
			// �������ΰ���
			linebuf="";
		}else if (linebuf[0]==':'){
			// pragma
			if (linebuf.find(":rem")==0){
				mc=true;
			}else{
				// unknown
			}
			linebuf="";
		}else if (linebuf[0]=='='){
			// �⡼������
			isMS=true;
		}else{
			// 1�ԥ����Ȥν���
			unsigned int max=linebuf.size();
			for(unsigned int i=0; i<max; i++){
				if ((linebuf[i]==' ')||(linebuf[i]=='\t')){
					continue;
				}else if (linebuf[i]=='#'){
					// comment
					linebuf="";
				}
				break;
			}
		}
	}
	linebuf=StringTrim(linebuf);
	linebuf+='\x0a';
	return true;
}
string TKawariPreProcessor::getline(void){
	string retstr=linebuf.substr(pos, linebuf.size()-pos);
	pos=linebuf.size();
	return retstr;
}
// �Хåե��ΰ������ڤ�Ф�
string TKawariPreProcessor::substring(int index, int length){
	if ((index>=0)&&(length>0)&&((index+length)<(int)linebuf.size()))
		return linebuf.substr(index, length);
	else
		return ("");
}
//---------------------------------------------------------------------------
