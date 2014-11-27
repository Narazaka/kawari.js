//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- file attribute --
//
//      Programed by Kouji.U (sky)
//
//  2001.06.03  Created
//  2001.06.17  Phase 6.0     Phase6.0�ˤ��碌�ƽ��� (NAKAUE.T)
//  2001.07.14  Phase 6.1     load�����Хѥ������ǽ�� (NAKAUE.T)
//  2001.07.19  Phase 6.2     Mingw�б� (NAKAUE.T)
//  2001.12.09  Phase 7.1.2   kis_textfile������
//                            readdir�ɲ�
//  2001.12.18  Phase 7.2     VC������FindFirstFileA��FindNextFileA��Ȥä�
//                            opendir/readdir/closedir����(Thanks: ���Ӥ���)
//  2002.03.17  Phase 7.9.0   Phase7.9�˹�碌���ѹ� (NAKAUE.T)
//                            textload�����ѹ�/textsave�ɲ�
//                            readdir��.��..�����
//  2002.04.16  Phase 8.0.0   �ѥ�ʸ��������misc/misc.cpp�ذ�ư
//  2002.07.10  Phase 8.1.0   isdir/isexist/isfile�ɲ�
//  2008.01.23  Phase 8.2.5   isexist(�롼�ȥǥ��쥯�ȥ��ǧ�к�)
//  2008.03.30  Phase 8.2.8   textsave����(ʣ���Υ���ȥ����¸�Ǥ��ʤ�)
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_file.h"
#include "libkawari/kawari_engine.h"
#include "libkawari/kawari_rc.h"
#include "misc/misc.h"
#include "misc/l10n.h"
using namespace kawari::resource;
using namespace kawari_log;
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(_WIN32) && defined(NOHAVE_READDIR)
#	include "misc/_dirent.h"
#	define S_ISDIR(m) ((m&_S_IFDIR)!=0)
#	define S_ISREG(m) ((m&_S_IFREG)!=0)
#else
#	include <dirent.h>
#endif
using namespace std;
//---------------------------------------------------------------------------
void KIS_save::Run(const vector<string>& args, bool crypt)
{
	if(!AssertArgument(args, 3)) return;

	vector<string> entry;
	entry.insert(entry.end(),args.begin()+2,args.end());

	// save�����Хѥ��Τ�
	string filename;
	if (IsAbsolutePath(CanonicalPath(args[1])))
		filename=PathToFileName(args[1]);
	else
		filename=CanonicalPath(Engine->GetDataPath(), args[1]);

	if(!Engine->SaveKawariDict(filename,entry,crypt)) {
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_FILE_SAVE_FAILED) << filename << endl;
	}

	return;
}
//---------------------------------------------------------------------------
string KIS_load::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	string filename=CanonicalPath(Engine->GetDataPath(), args[1]);

	if(!Engine->LoadKawariDict(filename)) {
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_FILE_LOAD_FAILED) << filename << endl;
	}

	return("");
}
//---------------------------------------------------------------------------
string KIS_textload::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 3, 3)) return ("");

	string filename=CanonicalPath(Engine->GetDataPath(), args[2]);

	ifstream ifs;
	ifs.open(filename.c_str());
	if(!ifs.is_open()) {
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_FILE_LOAD_FAILED) << filename << endl;
		return("");
	}

	TEntry entry=Engine->CreateEntry(args[1]);

	string buff;
	while(getline(ifs,buff)) {
		entry.Push(Engine->CreateStrWord(buff));
	}

	ifs.close();

	return("");
}
//---------------------------------------------------------------------------
string KIS_textsave::Function_(const vector<string>& args,bool flag)
{
	if(!AssertArgument(args, 3)) return ("");

	// save�����Хѥ��Τ�
	string filename;
	if (IsAbsolutePath(CanonicalPath(args[1])))
		filename=PathToFileName(args[1]);
	else
		filename=CanonicalPath(Engine->GetDataPath(), args[1]);

	ofstream ofs;
	if(flag) ofs.open(filename.c_str());
	 else ofs.open(filename.c_str(), ios::app);
	if(!ofs.is_open()) {
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_FILE_LOAD_FAILED) << filename << endl;
		return("");
	}

	for(unsigned int i=2;i<args.size();i++) {
		TEntry entry=Engine->CreateEntry(args[i]);
		unsigned int size=entry.Size();

		for(unsigned int l=0;l<size;l++) {
			ofs << Engine->IndexParse(entry,l) << endl;
		}
	}

	ofs.close();

	return("");
}
//---------------------------------------------------------------------------
string KIS_readdir::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 3, 3)) return ("");

	string dirname=CanonicalPath(Engine->GetDataPath(), args[2]);

	DIR *dirhandle;
	struct dirent *direntry;
	if((dirhandle=opendir(dirname.c_str()))==NULL) return("");

	Engine->ClearEntry(args[1]);

	while((direntry=readdir(dirhandle))!=NULL) {
		string entry=direntry->d_name;
		if((entry!=".")&&(entry!="..")) Engine->CreateEntry(args[1]).Push(Engine->CreateStrWord(entry));
	}

	closedir(dirhandle);

	return("");
}
//---------------------------------------------------------------------------
string KIS_cncpath::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 3)) return ("");

	if (args.size()==2){
		return CanonicalPath(args[1]);
	}else{
		return CanonicalPath(args[1], args[2]);
	}
}
//---------------------------------------------------------------------------
string KIS_dirname::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	return PathToBaseDir(CanonicalPath(args[1]));
}
//---------------------------------------------------------------------------
string KIS_filename::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	return PathToFileName(CanonicalPath(args[1]));
}
//---------------------------------------------------------------------------
string KIS_isdir::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	string dirname=CanonicalPath(Engine->GetDataPath(), args[1]);
	struct stat buf;

	if(stat(dirname.c_str(),&buf)) return("");

	if(S_ISDIR(buf.st_mode)) {
		return("1");
	}else{
		return("0");
	}
}
//---------------------------------------------------------------------------
string KIS_isfile::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	string pathname=CanonicalPath(Engine->GetDataPath(), args[1]);
	struct stat buf;

	if(stat(pathname.c_str(),&buf)) return("");

	if(S_ISREG(buf.st_mode)) {
		return("1");
	}else{
		return("0");
	}
}
//---------------------------------------------------------------------------
string KIS_isexist::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	string dirname=PathToBaseDir(CanonicalPath(Engine->GetDataPath(), args[1]));
	string filename=PathToFileName(CanonicalPath(Engine->GetDataPath(), args[1]));

	unsigned int pos=ctow(dirname).rfind(FILE_SEPARATOR);
	if(pos==string::npos) {
		// �ե�ѥ��ʤΤ�dirname��FILE_SEPARATOR���ʤ���硢����ǥ롼��
		// FILE_SEPARATOR���ɲä������ȥǥ��쥯�ȥ�Ȥθ�ǧ���ɤ�
		dirname+=FILE_SEPARATOR;
	}

	DIR *dirhandle;
	struct dirent *direntry;
	if((dirhandle=opendir(dirname.c_str()))==NULL) return("");

	string str="0";
	while((direntry=readdir(dirhandle))!=NULL) {
		string entry=direntry->d_name;
		if((entry!=".")&&(entry!="..")&&(entry==filename)) {
			str="1";
			break;
		}
	}

	closedir(dirhandle);

	return(str);
}
//---------------------------------------------------------------------------
