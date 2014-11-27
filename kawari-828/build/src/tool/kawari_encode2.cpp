//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// �Ź沽�ġ���
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.03.19  Phase 7.9.0   ���Ź�!KAWA0001����
//
//---------------------------------------------------------------------------
#include "libkawari/kawari_crypt.h"
#include <iostream>
#include <fstream>
using namespace std;
//---------------------------------------------------------------------------
// ʸ���������ζ���������
string StringTrim(string str)
{
	string::size_type linetop=str.find_first_not_of(" \t\r\n");
	string::size_type lineend=str.find_last_not_of(" \t\r\n");
	if(linetop==string::npos) return(string(""));

	return(str.substr(linetop,lineend-linetop+1));
}
//---------------------------------------------------------------------------
// ʸ����θ��β��Ԥ������
string StringTrim2(string str)
{
	string::size_type lineend=str.find_last_not_of("\r\n");
	return(str.substr(0,lineend+1));
}
//---------------------------------------------------------------------------
int main(int argc,char *argv[])
{
	string key;
	cout << "Input Keyword : ";
	cin >> key;

	for(int i=1;i<argc;i++) {
		string infile=argv[i];
		string::size_type pos=infile.find_last_of(".");
		string outfile=infile.substr(0,pos)+".kaw";

		ifstream ifs;
		ifs.open(infile.c_str());
		ofstream ofs;
		ofs.open(outfile.c_str());

		if((ifs.is_open())&&(ofs.is_open())) {
			string buff;
			bool cryptmode=false;
			while(getline(ifs,buff)) {
				buff=StringTrim2(buff);
				string buff2=StringTrim(buff);
				if(buff2==":crypt") {
					cryptmode=true;
				} else if(buff2==":endcrypt") {
					cryptmode=false;
				} else {
					if(!cryptmode) {
						ofs << buff << endl;
					} else {
						ofs << EncryptString2(buff,key) << endl;
					}
				}
			}
			
		}

		ifs.close();
		ofs.close();
	}

	return(0);
}

