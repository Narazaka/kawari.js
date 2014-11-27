//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����ѥ����¸
//
//      Programed by NAKAUE.T (Meister)
//
// ���Τ���autoconf���б��������ꤹ��Τ�����?
//
//---------------------------------------------------------------------------
#ifndef CONFIG_H
#define CONFIG_H

//---------------------------------------------------------------------------
// autoconfig��ɤ�

#ifdef NDEBUG
    #undef _STLP_DEBUG
#else
    #define _STLP_DEBUG 1
#endif

#if defined(__MSDOS__)
#	define KAWARI_MS
#elif defined(WIN32)||defined(_WIN32)||defined(_Windows)||defined(_MSC_VER)
#	define KAWARI_MS
#	define KAWARI_WIN32
#elif defined(__CYGWIN__)
#	define KAWARI_POSIX
#	define KAWARI_WIN32
#	define KAWARI_MS
#elif defined(linux)||defined(_linux)
#	define KAWARI_LINUX
#	define KAWARI_POSIX
#else
#	define KAWARI_POSIX
#endif

//---------------------------------------------------------------------------
#ifdef _MSC_VER

	// STL��Ȥ���ɬ���Ф뼱��̾255ʸ��ĶWarning������
	#pragma warning(disable:4786)

	// for�Υ������פ�ANSI�˽�򤷤Ƥ��ʤ������к�(�ĸ����ͰƤμ�ˡ)
	// ����ʸ�� http://www.ksky.ne.jp/~seahorse/cpp/loopvar.html
	#ifndef for
	#define for if(0);else for
	#endif

	// cstdlib����std̾�����֤�ȤäƤ��ʤ�����˵����륨�顼������
	namespace std{}

	// POSIX opendir/readdir/closedir��̵�����ᡢ�����ʤλ���
	#define NOHAVE_READDIR

#endif
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
	// BCC��vector�ϥǥե���ȤǺǽ��1KB���꡼����ݤ���Τ�
	// ���꡼���Ѹ�Ψ����������
	// ���� : BCC5.5.1 memory.stl��111�����ն�
	#define _RWSTD_CONTAINER_BUFFER_SIZE 4
	
	#define STRING_APPEND_INATOMIC

#endif
//---------------------------------------------------------------------------
#ifdef __MINGW32__
# if defined(_WIN32)
	// ¾�Υإå�������� windows.h ���ɤ߹��� 
	// �ʤǤʤ��������Х��ʥ꤬�㳰�������ߤ����ʤΤǡ� 
# include <windows.h>
# endif
	#define STRING_APPEND_INATOMIC
#endif
//---------------------------------------------------------------------------
#if defined(__GNUC__)
#	if (__GNUC__ >= 3)
#		define HAVE_SSTREAM_H
#	endif
#endif
//---------------------------------------------------------------------------
#endif

