//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ������� --
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.25  Phase 5.1     First version
//  2001.06.17  Phase 5.4     enumerate����
//  2001.07.08  Phase 6.0     eval�Х��ե��å���
//  2001.07.14  Phase 6.1     clear�ɲ�
//  2001.08.08  Phase 6.2     entry�ɲ�
//  2001.08.25  Phase 6.3     entry�����ɲ�
//                            get���ä����ɲ�
//                            size�ɲ�
//  2001.12.18  Phase 7.2     array����ˤ��ä����ɲ�
//  2002.03.10  Phase 7.9.0   �����ľ�ܥ��������ػ�
//                            enumerate,array�ѻ�
//                            get�����ѹ�,exec,enumexecƳ��
//  2002.03.13                ����ȥ�̾��ź������ǰƳ��
//  2002.03.17                �������ؿ�����
//                            list���ؿ��ɲ�
//  2005.06.28  Phase 8.2.3   TEntryRange�б�
//  2008.02.16  Phase 8.2.7   entrycount�Х��ե��å���
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_dict.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_engine.h"
#include "libkawari/kawari_rc.h"
#include "misc/misc.h"
using namespace kawari::resource;
using namespace kawari_log;
//---------------------------------------------------------------------------
#include <cstdlib>
#include <algorithm>
using namespace std;
//---------------------------------------------------------------------------
string KIS_encode_entryname::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return("");

	return(TKawariEngine::EncodeEntryName(args[1]));
}
//---------------------------------------------------------------------------
string KIS_eval::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2)) return("");

	string word = args[1];
	for(unsigned int i=2;i<args.size();i++) word+=string(" ")+args[i];

	return(Engine->Parse(word));
}
//---------------------------------------------------------------------------
string KIS_size::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return("");

	return(IntToString(Engine->EntrySize(args[1])));
}
//---------------------------------------------------------------------------
string KIS_set::Function_(const vector<string>& args,bool flag_str)
{
	if(!AssertArgument(args, 3)) return("");

	string word = args[2];
	for(unsigned int i=3;i<args.size();i++) word+=string(" ")+args[i];

#if 0
	string entryname;
	int st,end;

	int range=TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	switch(range) {
	case 0:		// �ϰϻ���̵��
		if (flag){
			TEntry entry=Engine->CreateEntry(entryname);
			TWordID id=Engine->CreateStrWord(word);
			entry.PushAfterClear(id);
		}else{
			Engine->PushAfterClear(entryname,word);
		}
		break;
	case 1:		// ñ�����ǻ���
	case 2:		// �ϰϻ���
		{
			unsigned int size=Engine->EntrySize(entryname);
			if(st<0) st=size+st;
			if(end<0) end=size+end;
			if((st<0)||(end<0)||(st>end)) {
				// ���顼
				GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
				return("");
			}

			TEntry entry=Engine->CreateEntry(entryname);

			TWordID id;
			if(flag) id=Engine->CreateStrWord(word);
			 else id=Engine->CreateWord(word);

			TWordID nullstr=Engine->CreateStrWord("");

			for(unsigned int i=st;i<=(unsigned int)end;i++) {
				entry.Replace2(i,id,nullstr);
			}
		}
		break;
	}

#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	if(range.Start==TKawariEngine::NPos){
		GetLogger().GetStream(LOG_ERROR) << args[0]
			<< RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return("");
	}

	TWordID wid=(flag_str)?
		Engine->CreateStrWord(word):Engine->CreateWord(word);
	if(!range.Range){
		range.Entry.PushAfterClear(wid);
	}else{
		TWordID nullstr=Engine->CreateStrWord("");
		for(unsigned int i=range.Start;i<=range.End;i++) 
			range.Entry.Replace2(i,wid,nullstr);
	}
#endif
	return("");
}
//---------------------------------------------------------------------------
string KIS_adddict::Function_(const vector<string>& args,bool flag_str)
{
	if(!AssertArgument(args, 3)) return ("");

	string word = args[2];
	for(unsigned int i=3;i<args.size();i++) word+=string(" ")+args[i];
#if 0
	string entryname;
	int st,end;
	TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	TEntry entry=Engine->CreateEntry(entryname);

	TWordID id;
	if(flag) id=Engine->CreateStrWord(word);
	 else id=Engine->CreateWord(word);

	entry.Push(id);
#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	TWordID wid=(flag_str)?
		Engine->CreateStrWord(word):Engine->CreateWord(word);
	range.Entry.Push(wid);
#endif
	return("");
}
//---------------------------------------------------------------------------
string KIS_unshift::Function_(const vector<string>& args,bool flag_str)
{
	if(!AssertArgument(args, 3)) return ("");

	string word = args[2];
	for(unsigned int i=3;i<args.size();i++) word+=string(" ")+args[i];
#if 0
	string entryname;
	int st,end;
	TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	TEntry entry=Engine->CreateEntry(entryname);

	TWordID id;
	if(flag) id=Engine->CreateStrWord(word);
	 else id=Engine->CreateWord(word);

	entry.Insert(0,id);
#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	TWordID wid=(flag_str)?
		Engine->CreateStrWord(word):Engine->CreateWord(word);
	range.Entry.Insert(0,wid);
#endif
	return("");
}
//---------------------------------------------------------------------------
string KIS_pop::Function_(const vector<string>& args,bool flag,bool flag2)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	unsigned int size=Engine->EntrySize(args[1]);
	if(size==0) return("");

	unsigned int pos=flag?0:(size-1);

	string retstr;
	if(flag2)
		retstr=Engine->IndexWord(args[1],pos);
	else
		retstr=Engine->IndexParse(args[1],pos);

	Engine->Erase(args[1],pos,pos);

	return(retstr);
}
//---------------------------------------------------------------------------
string KIS_getrandom::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 3)) return ("");

	string failstr;
	if(args.size()==3) failstr=args[2];

	TEntry entry=Engine->GetEntry(args[1]);
	if(!entry.IsValid()) return(failstr);

	unsigned int size=entry.Size();
	if(size==0) return(failstr);

	string retstr=Engine->IndexParse(entry,Random(size));
	if(retstr.size()==0) return(failstr);

	return(retstr);
}
//---------------------------------------------------------------------------
string KIS_get::Function_(const vector<string>& args,bool flag)
{
	if(!AssertArgument(args, 2, 2)) return ("");
#if 0
	string entryname;
	int st,end;

	TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	TEntry entry=Engine->GetEntry(entryname);
	unsigned int size=entry.Size();
	if(st<0) st=size+st;
	if(end<0) end=size+end;
	if((st<0)||(end<0)||(st>end)) {
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return("");
	}

	string retstr;
	for(unsigned int i=(unsigned int)st;i<=(unsigned int)end;i++) {
		if(flag) retstr+=Engine->IndexWord(entry,i);
		 else retstr+=Engine->IndexParse(entry,i);
	}
#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	if(range.Start==TKawariEngine::NPos){
		GetLogger().GetStream(LOG_ERROR) << args[0]
			<< RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return("");
	}

	string retstr;
	//MEMO: ��������(¸�ߤ��ʤ�ñ��)�εե���ѥ����̤ϻ��ͤȤ����ݾڤ����Τ���
	if(flag)
		for(unsigned int i=range.Start;i<=range.End;i++)
			retstr+=Engine->IndexWord(range.Entry,i);
	else
		for(unsigned int i=range.Start;i<=range.End;i++)
			retstr+=Engine->IndexParse(range.Entry,i);
#endif
	return(retstr);
}
//---------------------------------------------------------------------------
string KIS_clear::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");
#if 0
	string entryname;
	int st,end;

	int range=TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	switch(range) {
	case 0:		// �ϰϻ���̵��
		Engine->ClearEntry(entryname);
		break;
	case 1:		// ñ�����ǻ���
	case 2:		// �ϰϻ���
		{
			unsigned int size=Engine->EntrySize(entryname);
			if(st<0) st=size+st;
			if(end<0) end=size+end;
			if((st<0)||(end<0)||(st>end)) {
				// ���顼
				GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
				return("");
			}

			Engine->Erase(entryname,st,end);
		}
		break;
	}
#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	if(range.Start==TKawariEngine::NPos){
		GetLogger().GetStream(LOG_ERROR) << args[0]
			<< RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return("");
	}

	if(!range.Range)
		range.Entry.Clear();
	else
		range.Entry.Erase(range.Start, range.End);
#endif
	return("");
}
//---------------------------------------------------------------------------
string KIS_insert::Function_(const vector<string>& args,bool flag_str)
{
	if(!AssertArgument(args, 3, 3)) return ("");
#if 0
	string entryname;
	int st,end;

	TKawariEngine::DecodeEntryName(args[1],entryname,st,end);

	unsigned int size=Engine->EntrySize(entryname);
	if(st<0) st=size+st;
	if(st<0){
		// ���顼
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return ("");
	}

	string word = args[2];

	TEntry entry=Engine->CreateEntry(entryname);

	TWordID id;
	if(flag)
		id=Engine->CreateStrWord(word);
	else
		id=Engine->CreateWord(word);

	entry.Insert(st,id);
#else
	TEntryRange range=Engine->GetEntryRange(args[1]);
	if(range.Start==TKawariEngine::NPos){
		GetLogger().GetStream(LOG_ERROR) << args[0]
			<< RC.S(ERR_KIS_DICT_INVALID_INDEX) << endl;
		return("");
	}

	//MEMO: push��unshift������ܰʹߤΰ�����cat����Τˡ�
	//insert�Ϥ�ñ�Ȥ�ñ�줷�����ʤ��ΤϤʤ���  �������Ѥ����ʤ����ɡ�
	TWordID wid=(flag_str)?
		Engine->CreateStrWord(args[2]): Engine->CreateWord(args[2]);
	range.Entry.Insert(range.Start, wid);
#endif
	return("");
}
//---------------------------------------------------------------------------
string KIS_writeprotect::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");

	Engine->WriteProtect(args[1]);
	return ("");
}
//---------------------------------------------------------------------------
string KIS_wordcount::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 1, 1)) return ("");

	return IntToString(Engine->WordCollectionSize());
}
//---------------------------------------------------------------------------
string KIS_entrycount::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 1, 1)) return ("");

#if 0
	return IntToString(Engine->EntryCollectionSize());
#else
	int entryno=0;
	TEntry rootentry=Engine->CreateEntry(".");
	vector<TEntry> entrycol;
	if(rootentry.FindTree(entrycol)){
		// sort | uniq
		sort(entrycol.begin(), entrycol.end());
		vector<TEntry>::iterator end=unique(entrycol.begin(), entrycol.end());
		for(vector<TEntry>::iterator it=entrycol.begin(); it!=end; it++){
			if (it->GetName().size()) entryno++;
		}
	}
	return IntToString(entryno);
#endif
}
//---------------------------------------------------------------------------
string KIS_find::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 3, 4)) return ("");

	if(!Engine->EntrySize(args[1])) return ("-1");
	unsigned int min=(args.size()==4)?atoi(args[3].c_str()):0;
	unsigned int index=Engine->Find(args[1], args[2], min);
	return (index==Engine->NPos)? (string("-1")):IntToString((int)index);
}
//---------------------------------------------------------------------------
string KIS_rfind::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 3, 4)) return ("");

	if(!Engine->EntrySize(args[1])) return ("-1");
	unsigned int max=(args.size()==4)?atoi(args[3].c_str()):Engine->NPos;
	unsigned int index=Engine->RFind(args[1], args[2], max);
	return (index==Engine->NPos)? (string("-1")):IntToString((int)index);
}
//---------------------------------------------------------------------------
string KIS_cleartree::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 2)) return ("");
	if(!args[1].size()) return ("");

	Engine->ClearTree(args[1]);

	return ("");
}
//---------------------------------------------------------------------------
void KIS_listsub::_Function(const vector<string>& args, bool subflag){
	// ����ȥ�������ϥ��顼�����å��򿵽Ťˤ��ޤ���
	if(!AssertArgument(args, 3, 3)) return;
	if((!args[1].size())||(!args[2].size())) return;

	// "."�Ǥ��꤯����
	TEntry parententry=Engine->CreateEntry(args[2]);
	TEntry tempentry=Engine->CreateEntry(args[1]);

	vector<TEntry> entrycol;
	if((subflag)?parententry.FindAllSubEntry(entrycol):
	   parententry.FindTree(entrycol)){
		// sort | uniq
		sort(entrycol.begin(), entrycol.end());
		vector<TEntry>::iterator end=unique(entrycol.begin(), entrycol.end());

		for(vector<TEntry>::iterator it=entrycol.begin(); it!=end; it++){
			string entryname=it->GetName();
			if (!entryname.size()) continue;
			tempentry.Push(Engine->CreateStrWord(entryname));
		}
	}
	return;
}
//---------------------------------------------------------------------------
void KIS_copy::_Function(const vector<string>& args, bool rmflag){
	if(!AssertArgument(args, 3, 3)) return;
	if((!args[1].size())||(!args[2].size())) return;

	// �����ν��֤ɤ����뤫�ġ�
	TEntry srcentry=Engine->GetEntry(args[1]);
	TEntry dstentry=Engine->CreateEntry(args[2]);
	if(!srcentry.IsValid()) return;

	vector<TWordID> wordcol;
	srcentry.FindAll(wordcol);

	for(vector<TWordID>::iterator it=wordcol.begin(); it!=wordcol.end(); it++)
		dstentry.Push(*it);

	if (rmflag)
		srcentry.Clear();
}
//---------------------------------------------------------------------------
void KIS_copytree::_Function(const vector<string>& args, bool rmflag){
	if(!AssertArgument(args, 3, 3)) return;
	if((!args[1].size())||(!args[2].size())) return;
	if((args[1].size()<=args[2].size())&&(args[2].substr(0,args[1].size())==args[1])){
		GetLogger().GetStream(LOG_ERROR) << args[0] << RC.S(ERR_KIS_DICT_ILLEGAL_COPY_OPERATION) << endl;
		return;
	}

	const string &srcparent=args[1];
	const string &dstparent=(args[2]==".")?string(""):args[2];
	const int splen=(srcparent==".")?0:srcparent.size();
	TEntry srcparententry=Engine->CreateEntry(srcparent);

	vector<TEntry> srccol;
	srcparententry.FindTree(srccol);

	// sort | uniq
	sort(srccol.begin(), srccol.end());
	vector<TEntry>::iterator end=unique(srccol.begin(), srccol.end());

	for(vector<TEntry>::iterator it=srccol.begin(); it!=end; it++){
		TEntry srcentry=(*it);
		const string src=srcentry.GetName();
		const string dst=dstparent+src.substr(splen,src.size()-splen);
		TEntry dstentry=Engine->CreateEntry(dst);

		vector<TWordID> wordcol;
		srcentry.FindAll(wordcol);
		for(vector<TWordID>::iterator wit=wordcol.begin(); wit!=wordcol.end(); wit++)
			dstentry.Push(*wit);

		if (rmflag)
			srcentry.Clear();
	}
}
//---------------------------------------------------------------------------
