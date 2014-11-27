//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ̾������
//
//      Programed by Suikyo
//
//  2002.05.12  Phase 8.0.0   ʬΥ
//  2003.11.17                FindAllSubEntry�˥���ȥ�ºߥ����å�������
//  2003.12.02  Phase 8.2.0   FindAllSubEntry(ľ����꿼�����֥���ȥ��к�)
//  2005.10.28  Phase 8.2.4   RFind(����ǥå����Υǥ�����Ƚ������)
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_ns.h"
#include "libkawari/kawari_log.h"
#include "misc/misc.h"
using namespace kawari_log;
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
// �ϰϳ��Υ���ǥå���
const unsigned int TEntry::NPos=UINT_MAX;
//---------------------------------------------------------------------------
// ����ȥ����������
// ���˥���ȥ꤬¸�ߤ�����ϡ�����������ID���֤�
// ����� : ������������ȥ��ID
TEntry TNameSpace::Create(const string& entry)
{
	// "."�ϥ롼�Ȥ򼨤���
	if(entry==".") return(TEntry(this,0));

	vector<string> entryname_node;
	SplitEntryName(entry,entryname_node);
	if(entryname_node.size()==0) return(TEntry(this,0));

	string entryname;
	TEntryID parent=0,id;

	for(unsigned int i=0, max=entryname_node.size();i<max;i++) {
		entryname=entryname+entryname_node[i];

		id=0;
		if (EntryCollection.Insert(entryname,&id)){
			// �����ɲû��˸¤ꡣ
			ParentEntry[id]=parent;
			SubEntry.insert(pair<TEntryID,TEntryID>(parent,id));
		}
		parent=id;
		entryname=entryname+".";
	}
	return(TEntry(this,id));
}
//---------------------------------------------------------------------------
// ���ƤΥ���ȥ��������
void TNameSpace::ClearAllEntry(void)
{
	vector<TEntry> entrycol;
	FindAllEntry(entrycol);
	for(vector<TEntry>::iterator it=entrycol.begin(); it!=entrycol.end(); it++){
		it->Clear();
	}
}
//---------------------------------------------------------------------------
// ����ȥ�̾���������
// ����� : ����ȥ�θĿ�
unsigned int TNameSpace::FindAllEntry(vector<TEntry> &entrycol)
{
	TDictionary::const_iterator it;
	unsigned int n=0;

	for(it=Dictionary.begin();it!=Dictionary.end();it++) {
		if(it->second.size()) {
			entrycol.push_back(TEntry(this,it->first));
			n++;
		}
	}

	return(n);
}
//---------------------------------------------------------------------------
// ����ȥ�̾���.�פ�ʬ�򤹤�
void TNameSpace::SplitEntryName(const string& entryname,vector<string> &entryname_node)
{
	string::size_type pos=0;
	string::size_type max=entryname.size();
	while(pos<max){
		while((pos<max)&&(entryname[pos]=='.')) pos++;
		if(pos>=max) break;

		string::size_type pos1=pos;

		while((pos<max)&&(entryname[pos]!='.')) pos++;

		entryname_node.push_back(entryname.substr(pos1,pos-pos1));
	}

	return;
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��ñ��������
// ����� : ñ��θĿ�
unsigned int TEntry::Size(void) const
{
	if(!IsValid()) return(0);
	TDictionary::const_iterator it=ns->Dictionary.find(entry);
	if(it==ns->Dictionary.end()) return(0);
	return(it->second.size());
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ����ˤ���
// ����˶�����ȥ��ñ�줬�Ĥ�
// ����� : ������true
bool TEntry::Clear(void)
{
	if((!IsValid())||(ns->Dictionary.count(entry)==0)) return(false);
	if(AssertIfProtected()) return(false);

	// ��ñ����Ф��ơġ�
	for(vector<TWordID>::iterator it=ns->Dictionary[entry].begin();it!=ns->Dictionary[entry].end();it++) {
		TWordID wid=(*it);
		// �հ�������ξõ�
		ns->ReverseDictionary[wid].erase(ns->ReverseDictionary[wid].lower_bound(entry));
		// GC����Ͽ
		ns->gc->MarkWordForGC(wid);
	}

//	ns->Dictionary.erase(entry);
	ns->Dictionary[entry].clear();
	return(true);
}
//---------------------------------------------------------------------------
// ���Υ���ȥ�ʲ��Υ���ȥ�����ƶ��ˤ���
// ����˶�����ȥ��ñ�줬�ĤäƤ��ɤ�
void TEntry::ClearTree(void)
{
	if(!IsValid()) return;
	vector<TEntry> entrycol;
	FindAllSubEntry(entrycol);
	for(vector<TEntry>::iterator it=entrycol.begin(); it!=entrycol.end(); it++){
		it->ClearTree();
	}
	if (IsValid()) Clear();
}
//---------------------------------------------------------------------------
// ����ȥ�Ǹ����ؤ�ñ����ɲ�
void TEntry::Push(TWordID id)
{
	if((!IsValid())||(id==0)) return;
	if(AssertIfProtected()) return;
	ns->Dictionary[entry].push_back(id);
	ns->ReverseDictionary[id].insert(entry);
}
//---------------------------------------------------------------------------
// ����ȥ�Ǹ�����ñ��κ��
TWordID TEntry::Pop(void)
{
	if((!IsValid())||(ns->Dictionary.count(entry)==0)) return(0);
	if(AssertIfProtected()) return(0);
	TWordID id=ns->Dictionary[entry].back();
	ns->Dictionary[entry].pop_back();

	// �հ�������ξõ�
	ns->ReverseDictionary[id].erase(ns->ReverseDictionary[id].lower_bound(entry));
	// GC����Ͽ
	ns->gc->MarkWordForGC(id);
	return id;
}
//---------------------------------------------------------------------------
// ����ȥ�����ؤ�ñ�������
void TEntry::Insert(unsigned int pos,TWordID id)
{
	if((!IsValid())||(id==0)) return;
	if(AssertIfProtected()) return;

	if(ns->Dictionary[entry].size()<pos) return;
	ns->Dictionary[entry].insert(ns->Dictionary[entry].begin()+pos,id);
	ns->ReverseDictionary[id].insert(entry);
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��κ��
void TEntry::Erase(unsigned int st,unsigned int end)
{
	// st, ed�Ͽ��ѤǤ��ʤ����Ȥ����!
	if((!IsValid())||(st>end)||(st==NPos)) return;
	if(AssertIfProtected()) return;

	unsigned int entry_size=ns->Dictionary[entry].size();
	if (entry_size<=st) return;
	if (entry_size<=end) end=entry_size-1;
	vector<TWordID>::iterator itst,itend;
	itst=ns->Dictionary[entry].begin()+st;
	if(end!=NPos) itend=ns->Dictionary[entry].begin()+end+1;
	 else itend=ns->Dictionary[entry].end();

	// �հ�������ξõ�
	for(vector<TWordID>::iterator it=itst;it!=itend;it++) {
		TWordID id=*it;
		ns->ReverseDictionary[id].erase(ns->ReverseDictionary[id].lower_bound(entry));
		// GC����Ͽ
		ns->gc->MarkWordForGC(id);
	}

	ns->Dictionary[entry].erase(itst,itend);
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��������ؤ�
TWordID TEntry::Replace(unsigned int pos,TWordID id)
{
	if((!IsValid())||(id==0)) return(0);
	if(AssertIfProtected()) return(0);

	if(ns->Dictionary[entry].size()<pos) return(0);

	// �հ�������ξõ�
	TWordID oldid=ns->Dictionary[entry][pos];
	ns->ReverseDictionary[oldid].erase(ns->ReverseDictionary[oldid].lower_bound(entry));
	// GC����Ͽ
	ns->gc->MarkWordForGC(oldid);

	ns->Dictionary[entry][pos]=id;
	ns->ReverseDictionary[id].insert(entry);

	return(oldid);
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��������ؤ�(����ǥå������ϰϳ��ξ�硢id2���ɲ�)
// ����� : ������줿ñ��
TWordID TEntry::Replace2(unsigned int pos,TWordID id,TWordID id2)
{
	if((!IsValid())||(id==0)||(AssertIfProtected())) return(0);
	unsigned int size=Size();
	if(pos<size) {
		return Replace(pos,id);
	}else{
		for(unsigned int i=size; i<pos; i++){
			Push(id2);
		}
		Push(id);
		return 0;
	}
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
// ����� : ñ���ID
TWordID TEntry::Index(unsigned int index) const
{
	if(!IsValid()) return(0);
	TDictionary::const_iterator it=ns->Dictionary.find(entry);
	if(it==ns->Dictionary.end()) return(0);
	if(it->second.size()<=index) return(0);

	return(it->second[index]);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�
// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
unsigned int TEntry::Find(TWordID id,unsigned int pos) const
{
	if(!IsValid()) return(0);
	TDictionary::const_iterator it=ns->Dictionary.find(entry);
	if(it==ns->Dictionary.end()) return(NPos);

	for(unsigned int i=pos, max=it->second.size();i<max;i++) {
		if(it->second[i]==id) return(i);
	}

	return(NPos);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�(�ս�)
// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
unsigned int TEntry::RFind(TWordID id,unsigned int pos) const
{
	if(!IsValid()) return(0);
	TDictionary::const_iterator it=ns->Dictionary.find(entry);
	if((it==ns->Dictionary.end())||(!it->second.size())) return(NPos);

	if(pos==NPos) pos=it->second.size()-1;
	unsigned int i=pos;
	while(i<it->second.size()) {
		if (it->second[i]==id) return(i);
		i--;
	}

	return(NPos);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��ñ����������
// ����� : ñ��θĿ�
unsigned int TEntry::FindAll(vector<TWordID> &wordcol) const
{
	if(!IsValid()) return(0);
	if(ns->Dictionary.count(entry)==0) return(0);

	TDictionary::const_iterator it=ns->Dictionary.find(entry);
	wordcol.insert(wordcol.end(),it->second.begin(),it->second.end());

	return(it->second.size());
}
//---------------------------------------------------------------------------
// ���֥���ȥ�ID���������
// ����� : ����ȥ�θĿ�
unsigned int TEntry::FindAllSubEntry(vector<TEntry> &entrycol) const
{
//	if(!IsValid()) return(0);
	// entry==0�λ��ϥ롼��ľ����õ��
	typedef multimap<TEntryID,TEntryID>::const_iterator T;
	pair<T,T> range=ns->SubEntry.equal_range(entry);
	unsigned int n=0;

	vector<TEntry> dmyentrycol;
	for(T it=range.first;it!=range.second;it++) {
		TEntry current(ns,it->second);
		if(current.Size()) {
			entrycol.push_back(current);
			n++;
		}else if(current.FindTree(dmyentrycol)){
			// ���˲��γ��ؤ�(����)+����ȥ꤬����������Ƥ��뤳�Ȥˤ���
			entrycol.push_back(current);
			n++;
		}
	}

	return(n);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�̾����Ϥޤ륨��ȥ�ID���������
// "."�ϥ롼�Ȥ򼨤���
// ���Υ���ȥ��̵��
// ����� : ����ȥ�θĿ�
unsigned int TNameSpace::FindTree(TEntryID entry, vector<TEntry> &entrycol)
{
//	if(!IsValid()) return(0);
	typedef multimap<TEntryID,TEntryID>::const_iterator T;
	pair<T,T> range=SubEntry.equal_range(entry);
	unsigned int n=0;

	for(T it=range.first;it!=range.second;it++) {
		n+=FindTree(it->second, entrycol);
	}

	TEntry current(this,entry);
	if(current.Size()) {
		entrycol.push_back(current);
		n++;
	}

	return(n);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�̾����Ϥޤ륨��ȥ�ID���������
// "."�ϥ롼�Ȥ򼨤���
// ���Υ���ȥ��̵��
// ����� : ����ȥ�θĿ�
unsigned int TEntry::FindTree(vector<TEntry> &entrycol) const
{
	return(ns->FindTree(entry, entrycol));
}
//---------------------------------------------------------------------------
