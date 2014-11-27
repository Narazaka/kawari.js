//--------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// �������
//
//      Programed by Suikyo
//
//  2002.04.18  Phase 8.0.0   ��ľ��
//
//--------------------------------------------------------------------------
#ifndef LEXER_H__
#define LEXER_H__
//--------------------------------------------------------------------------
#include "config.h"
//--------------------------------------------------------------------------
#include "libkawari/kawari_log.h"
//--------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
//--------------------------------------------------------------------------
class Token;
class TKawariLexer;
class TKawariPreProcessor;
//--------------------------------------------------------------------------
class Token {
public:
	// �ü�ʸ���ϡ�����ʸ�������ɤ����Τޤ�int�ˤʤä��֤롣
	enum Type {
		T_ERROR = 0x100,	//
		T_LITERAL,		//
		T_QLITERAL,		//
		T_SPACE,		// ' ', '\t'
		T_EOL,			// CR, LF, CRLF
		T_SPECIAL,		// special marks (in peek() only)
		T_MODESWITCH,	// '=xxxxx'
		T_EOF,
		T_DUMMY
	} type;
	std::string str;
	Token (Type t, const std::string &s) : type(t), str(s) {}
	Token (void) : type(T_DUMMY), str("") {}
	Token &set (Type t) { type=t; return *this; }
	Token &set (const std::string &s) { str=s; return *this; }
	Token &set (Type t, const std::string &s)
		{ type=t; str=s; return *this; }
	Token &set (Type t, const char &ch)
		{ type=t; str+=ch; return *this; }
};

//-------------------------------------------------------------------------
// class TKawariLexer;
//
class TKawariLexer{
public:
	// ���󥹥ȥ饯��
	TKawariLexer (std::istream &input, TKawariLogger &lgr,
				  std::string filename="<unknown>",
				  bool preprocess=true, int lineno=0);

	// Lexer�Υ⡼��
	enum Mode {
		ID_MODE = 0,			// ����ȥ�̾�⡼��
		LITERAL_MODE = 1,		// ʸ����⡼��(����ȥ������)
		LITERAL_MODE2 = 2,		// ʸ����⡼��(����饤�󥹥���ץ���)
		LITERAL_MODE3 = 3		// ʸ����⡼��(�֥�å���)
	};

	// ���Υȡ����󤬤��뤫
	bool hasNext(void);

	// ���Υȡ�����Υ����פ��֤���
	// �ɤ�����ǥݥ��󥿤�ʤ�ʤ�
	// mode : Lexer�Υ⡼�ɻ���(LITERAL_MODE, ID_MODE)
	Token::Type peek(Mode mode=LITERAL_MODE);

	// ���Υȡ�������֤�
	// mode : Lexer�Υ⡼�ɻ���(LITERAL_MODE, ID_MODE)
	Token next(Mode mode=LITERAL_MODE);

	// !!! check type before use this !!!
	// ���Υ�ƥ��ȡ�������֤�
	// �����٥���ƥ��Ǥʤ��ä��顢""���֤롣ͽ��ʬ���äƤ�����ˤ����Ȥ����ȡ�
	// mode : Lexer�Υ⡼�ɻ���(LITERAL_MODE, ID_MODE)
	std::string getLiteral(Mode mode=LITERAL_MODE);

	// !!! check type, before use this !!!
	// ���Υ������ȥ�ƥ��ȡ�����(��ǥ�����)���֤�
	// �����������ȥ�ƥ��Ǥʤ��ä���""���֤롣
	std::string getQuotedLiteral(void);

	// !!! check type before use this !!!
	// [0-9]+
	// ����ʸ����
	std::string getDecimalLiteral(void);

	// 1ʸ�������å�
	int skip(void);

	// ����ʸ���μ����ޤǥ����å� (ʸˡ���顼�����������)
	// noret : ���Ԥ�����ʤ����true
	bool simpleSkipTo(char ch, bool noret=true);

	// �ե�����̾���֤�
	const std::string &getFileName(void) const;

	// ���ߤι��ֹ���֤�
	int getLineNo(void) const;

	// ���߹ԤλĤ�������֤�
	std::string getRestOfLine(void);

	// �⡼�ɥ����å��Υꥻ�å�
	inline void ResetModeSwitch(void);

	// �����ӥ��᥽�å�
	// ����(����ʸ���Ȳ���ʸ��)�Υȡ�������ɤ����Ф���
	void skipWS(void);

	// �����ӥ��᥽�å�
	// ����(����ʸ���Ȳ���ʸ��)�Υȡ�������ɤ����Ф���
	// ���μ��Υȡ�����Υ����פ��֤�
	Token::Type skipWS(Mode m);

	// �����ӥ��᥽�å�
	// ����ʸ���Υȡ�������ɤ����Ф������μ��Υȡ�����Υ����פ��֤�
	// ����ʸ�������Ф��ʤ���
	Token::Type skipS(Mode m=LITERAL_MODE);

	// ����Ĺ�������꡼�ɥݥ��󥿤��᤹
	bool UngetChars(unsigned int length);

	// �����ӥ��᥽�å�
	// ��������ʸ�����ǥ�����
	static std::string DecodeQuotedString(const std::string &orgsen);

	// �����ӥ��᥽�å�
	// ʸ����򥨥�ȥ�̾�ǻ��Ѳ�ǽ��ʸ����˥��󥳡��ɤ���
	static std::string EncodeEntryName(const std::string &orgsen);

	~TKawariLexer ();

	// ����ѥ��륨�顼
	void error(const std::string &message);

	// ����ѥ��륨�顼
	void warning(const std::string &message);

private:
	// ʸ������ȡ����󥿥��פ�����
	Token::Type checkType(Mode m, char ch) const;

	class TKawariPreProcessor *pp;
	std::string fn;

	TKawariLogger &logger;
};
//---------------------------------------------------------------------------
// �ץ�ץ��å�
//
class TKawariPreProcessor {
public:
	TKawariPreProcessor(std::istream &input, int lineno=0, bool preprocess=true)
		: is(input), pp(preprocess), mc(false), isMS(false), ln(lineno), pos(0) {}
	bool peek(char &ch);
	bool getch(char &ch);
	bool eof(void);
	std::string getline(void);
	bool unget(void);
	int getLineNo(void){ return ln; }

	int getColumn(void){ return pos; }

	// �Хåե��ΰ������ڤ�Ф�
	std::string substring(int index, int length);

	virtual ~TKawariPreProcessor() {}

	// ���ߡ��⡼�������Ԥ���
	bool IsWaitingModeSwitch(void){ return isMS; }

	// �⡼�ɥե饰�Υꥻ�å�
	void ResetModeSwitch(void){ isMS=false; }

private:
	bool processNextLine(void);
	// ���ϥ��ȥ꡼��
	std::istream &is;
	// pre-process���뤫�ݤ��Υե饰
	bool pp;
	// ʣ���ԥ����ȤΥե饰
	bool mc;

	// Mode Switch
	bool isMS;
	// ���߹ԤιԿ�
	unsigned int ln;
	// ���ߥ����
	unsigned int pos;
	// ���ϥХåե�
	std::string linebuf;
};
//---------------------------------------------------------------------------
// ���Υȡ����󤬤��뤫
inline bool TKawariLexer::hasNext(void){
	return !pp->eof();
}
//---------------------------------------------------------------------------
inline void TKawariLexer::ResetModeSwitch(void){
	pp->ResetModeSwitch();
}
//---------------------------------------------------------------------------
inline void TKawariLexer::error(const std::string &message){
	logger.GetStream(kawari_log::LOG_ERROR) << getFileName() << " " << getLineNo()
		<< ": error: " << message << std::endl;
}
//---------------------------------------------------------------------------
inline void TKawariLexer::warning(const std::string &message){
	logger.GetStream(kawari_log::LOG_WARNING) << getFileName() << " " << getLineNo()
		<< ": warning: " << message << std::endl;
}
//---------------------------------------------------------------------------
inline bool TKawariPreProcessor::peek(char &ch){
	bool ret = getch(ch);
	unget();
	return ret;
}
//---------------------------------------------------------------------------
inline bool TKawariPreProcessor::getch(char &ch){
	if (pos>=linebuf.size())
		if (!processNextLine())
			return false;
	ch=linebuf[pos++];
	return true;
}
//---------------------------------------------------------------------------
inline bool TKawariPreProcessor::unget (void){
	if (pos==0) return false;
	pos--;
	return true;
}
//---------------------------------------------------------------------------
inline bool TKawariPreProcessor::eof (void){
	return ((pos>=linebuf.size())&&(is.eof()));
}
//---------------------------------------------------------------------------
#endif  // LEXER_H__
