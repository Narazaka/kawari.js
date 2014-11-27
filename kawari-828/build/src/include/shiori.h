//---------------------------------------------------------------------------
/**
  * @brief		SHIORI/2.0 ��AI�⥸�塼�� ����ݡ��ȥإå��ե�����
  *				���Υե������PDS(Public Domain Software)�Ȥ��롣
  *
  * ���١פȤϡ��ֻǤ������ѵ�AI�⥸�塼�뵬�ʤ�̾�ΤǤ��롣
  * �ٵ��ʤϡ�HTTP�˹�������ꥯ�����ȡ��쥹�ݥ󥹷���å������󥰤򼴤Ȥ���
  * �·���ǥ�Ǥ��ꡢ��AI�⥸�塼��¦����٥����Сס���������Ѥ���¦��
  * ���٥��饤����ȡפȸƤ֡�
  *
  * API�桢�����Ϥ����Ѥ���������ΰ�ϡ��ץ�åȥե������¸���ΰ����
  * �ؿ��ǳ��ݤ��졢�������¦���������롣
  * �ΰ���ݡ��������Ѥ���ؿ��ϰʲ���
  * Win32�ץ�åȥե����� : GlobalAlloc(GMEM_FIXED, len), GlobalFree()
  * POSIX�ץ�åȥե����� : malloc(len), free()
  *	                        (�����������饤����ȸ�ͭC�饤�֥���¸)
  */
//
//  2003.02.25                ���� <suikyo@yk.rim.or.jp>
//
//---------------------------------------------------------------------------
#ifndef SHIORI_H__
#define SHIORI_H__
//-------------------------------------------------------------------------
#if defined(WIN32)||defined(_WIN32)||defined(_Windows)||defined(__CYGWIN__)
// Win32
#	include <windows.h>
#	ifdef __BORLANDC__
// 		Borland C++
#		define SHIORI_EXPORT	extern "C"
#	else
// 		Visual C++ / Cygwin32 / Mingw32
#		define SHIORI_EXPORT	extern "C" __declspec(dllexport)
#	endif
#	define SHIORI_CALL			__cdecl
#	define MEMORY_HANDLE		HGLOBAL
#	define SHIORI_MALLOC(len)	::GlobalAlloc(GMEM_FIXED, len)
#	define SHIORI_FREE(ptr)		::GlobalFree((HGLOBAL)ptr)
#else
// Other Platform
#	define SHIORI_EXPORT		extern "C"
#	define SHIORI_CALL
#	define MEMORY_HANDLE		char *
#	define SHIORI_MALLOC(len)	malloc(len)
#	define SHIORI_FREE(ptr)		free((void *)ptr)
#endif
#ifndef BOOL
#	define BOOL					int
#	define TRUE					1
#	define FALSE				0
#endif
//---------------------------------------------------------------------------
/**
  * @brief		�⥸�塼���ɤ߹���ľ��˸ƤФ��
  *
  * @param h	(I) DLL�ե����뤬¸�ߤ���ǥ��쥯�ȥ�ѥ���ɽ��ʸ����ؤ�
  *				�ݥ��󥿡������ΰ���٥����Ф��������뤳�ȡ�
  * @param len	(I) h������ʸ�����Ĺ��
  * @return		�����ΤȤ���TRUE�����ԤΤȤ���FALSE��
  */
SHIORI_EXPORT BOOL SHIORI_CALL load(const MEMORY_HANDLE h,long len);
//---------------------------------------------------------------------------
/**
  * @brief		�⥸�塼���ڤ�Υ��ľ���˸ƤФ��
  *
  * @return		�����ΤȤ���TRUE�����ԤΤȤ���FALSE��
  */
SHIORI_EXPORT BOOL SHIORI_CALL unload(void);
//---------------------------------------------------------------------------
/**
  * @brief		��AI�ꥯ������
  *
  * @param h	(I) �׵�ʸ����ؤΥݥ��󥿡��٥����Ф��������뤳�ȡ�
  * @param len	(IO) �ƤӽФ������׵�ʸ�����byteĹ����Ǽ���졢����ʸ�����
  *				byteĹ����Ǽ������֤롣
  * @return		����ʸ����ؤΥݥ��󥿡��٥����Ф����ݤ����٥��饤����Ȥ�
  *				�������뤳�ȡ�
  */
SHIORI_EXPORT MEMORY_HANDLE SHIORI_CALL request(const MEMORY_HANDLE h,long *len);
//---------------------------------------------------------------------------
#endif // SHIORI_H__
//---------------------------------------------------------------------------
