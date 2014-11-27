//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����
//
//      Programed by Suikyo
//
//  2002.04.12  Phase 8.0.0   Engine����ʬΥ
//
//---------------------------------------------------------------------------
#ifndef KAWARI_LOG_H
#define KAWARI_LOG_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
using namespace std;

namespace kawari_log {
	enum LOGLEVEL {
		LOG_ERROR		=0x0001,
		LOG_WARNING		=0x0002,
		LOG_INFO		=0x0004,
		LOG_DECL		=0x0008,
		LOG_DUMP		=0x0010,
		LOG_BASEEVENTS	=0x0100,
		LOG_RSCEVENTS	=0x0200,
		LOG_MOUSEEVENTS	=0x0400,
		LOG_TIMEEVENTS	=0x0800
	};
}

//---------------------------------------------------------------------------
class TKawariLogger {
private:
	// �����ȥ꡼��
	std::ostream *LogStream;

	// �ɤ��ˤ������Ϥ��ʤ����ȥ꡼��
	std::ostream *NullStream;

	// ���顼��٥� (������)
	unsigned int errlevel;

public:
	// ���ϥ��ȥ꡼�����
	std::ostream& GetStream(void) const;

	// ���ϥ��ȥ꡼�����
	std::ostream& GetStream(kawari_log::LOGLEVEL lvl) const;

	// ���ϥ��ȥ꡼�������
	void SetStream(std::ostream *outstream);

	// ���ϥ��ȥ꡼���ɸ����Ϥ�����
	void SetStreamStdOut(void);

	// ���顼��٥������
	unsigned int ErrLevel(void) const { return errlevel; }

	// ���顼��٥������
	void SetErrLevel(unsigned int level);

	// ���顼��٥�Υ����å�
	bool Check(unsigned int lvl) const{
		return ((errlevel&lvl)!=0);
	}
	TKawariLogger (void);
	~TKawariLogger ();
};
//---------------------------------------------------------------------------
// ���ϥ��ȥ꡼�����
inline std::ostream& TKawariLogger::GetStream(void) const
{
	return(*LogStream);
}
//---------------------------------------------------------------------------
// ���ϥ��ȥ꡼�����
inline std::ostream& TKawariLogger::GetStream(kawari_log::LOGLEVEL lvl) const{
	if ((errlevel&lvl)!=0)
		return(*LogStream);
	else
		return(*NullStream);
}
//---------------------------------------------------------------------------
// ���ϥ��ȥ꡼�������
inline void TKawariLogger::SetStream(std::ostream *outstream)
{
	if (outstream)
		LogStream=outstream;
	else
		LogStream=NullStream;
}
//---------------------------------------------------------------------------
// ���顼��٥������
inline void TKawariLogger::SetErrLevel(unsigned int level){
	errlevel=level;
}
//---------------------------------------------------------------------------
// ���ϥ��ȥ꡼���ɸ����Ϥ�����
inline void TKawariLogger::SetStreamStdOut(void){
	LogStream=&cout;
}
//---------------------------------------------------------------------------
//extern TKawariLogger Logger;
//---------------------------------------------------------------------------
#endif // KAWARI_LOG_H
