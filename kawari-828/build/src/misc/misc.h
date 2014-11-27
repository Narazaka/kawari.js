//---------------------------------------------------------------------------
//
// ����
//
//      Programed by NAKAUE.T (Meister)
//
//---------------------------------------------------------------------------
#ifndef MISC_H
#define MISC_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <cstdlib>
using namespace std;
//---------------------------------------------------------------------------
#include "misc/mt19937ar.h"
#include "libkawari/kawari_log.h"
//---------------------------------------------------------------------------
extern TMTRandomGenerator MTRandomGenerator;
//---------------------------------------------------------------------------
// 0����num-1�ޤǤ������ȯ��
inline int Random(int num)
{
	return((int)(MTRandomGenerator.genrand_real2()*num));
}
//---------------------------------------------------------------------------
// ����Υ����ɤ�����
inline void SRandom(int seed)
{
	MTRandomGenerator.init_genrand((unsigned long)seed);
}
//---------------------------------------------------------------------------
// ʸ���������ζ���������
inline string StringTrim(const string& str)
{
	string::size_type linetop=str.find_first_not_of(" \t\r\n");
//	string::size_type lineend=str.find_last_not_of(" \t\r\n");
	string::size_type lineend=str.find_last_not_of('\0');
	lineend=str.find_last_not_of(" \t\r\n",lineend);
	if(linetop==string::npos) return(string(""));

	return(str.substr(linetop,lineend-linetop+1));
}
//---------------------------------------------------------------------------
// �����ͤ���ʸ���������
string IntToString(int n);
//---------------------------------------------------------------------------
// ������Ƚ��
bool IsInteger (const std::string &str);
//---------------------------------------------------------------------------
// ����Ƚ��
inline bool IsTrue (const std::string &str) {
	return (!((str=="")||(str=="0")||(str=="false")));
}
//---------------------------------------------------------------------------
// �ѥ�ɽ����������
#ifdef KAWARI_MS
#	define FILE_SEPARATOR		'\\'
#	define FILE_SEPARATOR_ALT	'/'
#else
#	define FILE_SEPARATOR		'/'
#	define FILE_SEPARATOR_ALT	'\\'
#endif
string CanonicalPath(const string &path);
string CanonicalPath(const string &basepath, const string &path);
inline bool IsAbsolutePath(const string &path){
#ifdef KAWARI_MS
	return (((path.size()>0)&&(path[0]=='\\'))||((path.size()>1)&&(path[1]==':')));
#else
	return ((path.size()>0)&&(path[0]=='/'));
#endif
}
string PathToFileName(const string &path);
string PathToBaseDir(const string &path);
//---------------------------------------------------------------------------
template <class T>
int StringCompare(const basic_string<T> &str0, const basic_string<T> &str1, size_t s=0, size_t n=string::npos){
#if defined(__GNUC__) && (__GNUC__ < 3)
	return str0.compare(str1, s, n);
#else
	return str0.compare(s, n, str1);
#endif
}
//---------------------------------------------------------------------------
#endif
