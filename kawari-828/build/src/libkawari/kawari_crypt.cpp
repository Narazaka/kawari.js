//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// �Ź沽/���沽
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.03.15  Phase 0.42    �Ź沽����
//  2001.03.19  Phase 7.9.0   ���Ź�!KAWA0001����
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_crypt.h"
#include "misc/base64.h"
//---------------------------------------------------------------------------
// �Ź沽�������ǧ����
bool CheckCrypt(const string& encodedstr)
{
	string id=encodedstr.substr(0,9);
	return((id=="!KAWA0000")||(id=="!KAWA0001"));
}
//---------------------------------------------------------------------------
// ɸ��Ź沽
string EncryptString(const string& str)
{
	string encodedstr;
	encodedstr.reserve(str.size());

	for(unsigned int i=0,max=str.size();i<max;i++) {
		encodedstr+=str[i]^0xcc;
	}

	return(string("!KAWA0000")+EncodeBase64(encodedstr));
}
//---------------------------------------------------------------------------
// ɸ�����沽
string DecryptString(const string& encodedstr)
{
	string str=DecodeBase64(encodedstr.substr(9));

	unsigned char k=0xcc;
	unsigned int skip=0;

	string id=encodedstr.substr(0,9);
	if(id=="!KAWA0001") {
		k=(unsigned char)str[0];
		skip=1;
	}

	string decodedstr;
	decodedstr.reserve(str.size());
	for(unsigned int i=skip,max=str.size();i<max;i++) {
		decodedstr+=str[i]^k;
	}

	return(decodedstr);
}
#if 0
// ����
// ���ä��򸵤ˤ���С��⤦����ä��ɤ������ɤˤʤ�Ȼפ����������󡦡�����
// (�ͤ���)
string DecryptString(const string& encodedstr)
{
	// long int = 32bit �Ǥ��뤳�Ȥ˰�¸���������ǥ��󥰤ˤʤäƤޤ� 
	string str=DecodeBase64(encodedstr.substr(9));

	unsigned cnt = str.size();
	string decodedstr;
	
	char *s = new char[cnt + 4];
	
	str.copy(s, cnt);
	
	for(unsigned int i=0;i<cnt ;i += 4) {
		*(unsigned long *)(s + i) ^= 0xccccccccUL;
	}

	decodedstr.assign(s, cnt);
	delete[] s;
	return(decodedstr);
}
#endif
//---------------------------------------------------------------------------
// !KAWA0001�������׻�
// ���ΤǤϥ����å�����Ȥ��ա�:-)
static unsigned char Kawa0001CalcKey(const string &key)
{
	unsigned char k=0;
	for(unsigned int i=0,max=key.size();i<max;i++) {
		k+=key[i];
	}

	return(k);
}
//---------------------------------------------------------------------------
// ���Ź沽(�Ź業����)
string EncryptString2(const string& str,const string &key)
{
	unsigned char k=Kawa0001CalcKey(key);
	string encodedstr;
	encodedstr.reserve(str.size()+1);
	encodedstr+=k;

	for(unsigned int i=0,max=str.size();i<max;i++) {
		encodedstr+=str[i]^k;
	}

	return(string("!KAWA0001")+EncodeBase64(encodedstr));
}
//---------------------------------------------------------------------------
// ɸ�����沽
string DecryptString2(const string& encodedstr,const string &key)
{
	string id=encodedstr.substr(0,9);
	if(id!="!KAWA0001") return("");

	string str=DecodeBase64(encodedstr.substr(9));

	unsigned char k=Kawa0001CalcKey(key);
	if(k!=(unsigned char)str[0]) return("");

	string decodedstr;
	decodedstr.reserve(str.size());
	for(unsigned int i=1,max=str.size();i<max;i++) {
		decodedstr+=str[i]^k;
	}

	return(decodedstr);
}
//---------------------------------------------------------------------------
