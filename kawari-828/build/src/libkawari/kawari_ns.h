//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ̾������
//
//      Programed by Suikyo
//
//  2002.05.12  Phase 8.0.0   ʬΥ
//
//---------------------------------------------------------------------------
#ifndef KAWARI_NS_H
#define KAWARI_NS_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
//---------------------------------------------------------------------------
#include "libkawari/wordcollection.h"
#include "libkawari/kawari_log.h"
#include "libkawari/kawari_rc.h"
//---------------------------------------------------------------------------
// ñ��ID
typedef unsigned int TWordID;
//---------------------------------------------------------------------------
// ����ȥ�ID
typedef unsigned int TEntryID;
//---------------------------------------------------------------------------
// ����
typedef map<TEntryID,vector<TWordID> > TDictionary;
// �հ�������
typedef map<TWordID,multiset<TEntryID> > TRDictionary;
// �ƥ���ȥ�ǥ��쥯�ȥ�
typedef map<TEntryID,TEntryID> TParentEntryMap;
// ���֥���ȥ�ǥ��쥯�ȥ�
typedef multimap<TEntryID,TEntryID> TSubEntryMap;
//---------------------------------------------------------------------------
// ���٥å����쥯���Υ��󥿡��ե�����
class TGarbageCollector {
public:
	// ñ��˺���ޡ������դ���
	virtual void MarkWordForGC(TWordID id)=0;

	// �������֤�
	virtual TKawariLogger &GetLogger(void)=0;
};
//---------------------------------------------------------------------------
// ̾������
// ����ȥ�̾�Ϲ��˥ԥꥪ��('.')�ˤ�äƳ���Ū�˴�������롣
class TNameSpace {
protected:
	// ����ȥꥳ�쥯�����
	TWordCollection<string,less<string> > EntryCollection;
	// ����
	TDictionary Dictionary;
	// �հ�������
	TRDictionary ReverseDictionary;
	// �ƥ���ȥ�ǥ��쥯�ȥ�
	TParentEntryMap ParentEntry;
	// ���֥���ȥ�ǥ��쥯�ȥ�
	TSubEntryMap SubEntry;
	// ����߶ػߤΥ���ȥ�
	set<TEntryID> ProtectedEntry;

	// �����Х�ʥ��٥å����쥯���ؤλ���
	TGarbageCollector *gc;

	// ���ꤵ�줿����ȥ꤫��Ϥޤ륨��ȥ�ID���������
	unsigned int FindTree(TEntryID entry, vector<class TEntry>& entrycol);

public:
	TNameSpace(TGarbageCollector *col): gc(col) {}
	virtual ~TNameSpace();

	// WordCollection�Υҡ��פ����
	void Reserve(unsigned int n);


	// [ �����API ]

	// ͭ������ȥ�������
	unsigned int Size(void) const;


	// [ ����ȥ���� ]

	// ����ȥ�ID����
	// "."�ϥ롼�Ȥ򼨤���
	// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
	class TEntry Get(const string& entry);

	// ����ȥ����������
	// ���˥���ȥ꤬¸�ߤ�����ϡ�����������ID���֤�
	// "."�ϥ롼�Ȥ򼨤���
	// ����� : ������������ȥ��ID
	class TEntry Create(const string& entry);


	// [ ����ȥ���� ]

	// ���ƤΥ���ȥ��������
	void ClearAllEntry(void);


	// [ ������API ]

	// ����ȥ���������
	// ����� : ����ȥ�θĿ�
	unsigned int FindAllEntry(vector<class TEntry> &entrycol);

	// ����ȥ�̾���.�פ�ʬ�򤹤�
	static void SplitEntryName(const string& entryname,vector<string> &entryname_node);
	// ���ꤵ�줿ñ���ޤ२��ȥ꤬¸�ߤ��뤫Ĵ�٤�
	// ����� : ¸�ߤ����true
	bool ContainsWord(TWordID id) const;

	friend class TEntry;
};
//---------------------------------------------------------------------------
// ����ȥ�
class TEntry {
private:
	TNameSpace * ns;
	TEntryID entry;

	// �񤭹����ݸ�����å�
	// ����� : ������ݸ��оݤʤ��true
	bool AssertIfProtected(void);

public:
	TEntry(TNameSpace *space, TEntryID id) : ns(space), entry(id) {}

	TEntry(const TEntry& e) : ns(e.ns),entry(e.entry) {}
	TEntry &operator = (const TEntry& e) { ns=e.ns; entry=e.entry; return *this; }
	bool operator == (const TEntry& e) const {
		return ((ns==e.ns)&&(entry==e.entry));
	}
	bool operator != (const TEntry& e) const {
		return ((ns!=e.ns)||(entry!=e.entry));
	}
	bool operator < (const TEntry& r) const {
		if(ns<r.ns) return true;
		else if (ns>r.ns) return false;
		else return (entry<r.entry);
	}

	// �ϰϳ��Υ���ǥå���
	static const unsigned int NPos;		// UINT_MAX
		// ���ơ���������class���Ф�const�Ϥ������ξ������������ʤꤽ����
		// ��Τ�����VC++6.0�ΥХ��ǡ���ǰ�ʤ��餽���⤤���ʤ���
		// ���ΥХ��ϡ������ͤ�������뤳�Ȥˤ�äƲ���Ǥ��롣
		// �ºݤ��ͤ��ɤ��ʤäƤ뤫�ϡ��������������β������򸫤Ƥ�������������
		// �ܤ����ϲ������ȡ�(�դ������)
		// BUG: C2258 and C2252 on in Place Initialization of Static Const Members (Q241569)
		// http://support.microsoft.com/default.aspx?scid=kb;en-us;Q241569

	// ���Υ���ȥ���ä���硢�ٹ����˻Ĥ���
	// �޾줷�Τ�Ū��
	// ����� : ���ξ��true
	bool AssertIfEmpty(const string& name);


	// [ �����API ]

	// ���ꤵ�줿����ȥ��ñ��������
	// ����� : ñ��θĿ�
	unsigned int Size(void) const;

	// ͭ���ʥ���ȥ�Ǥ��뤫��Ĵ�٤�
	// ����� : ͭ���ʤ鿿
	bool IsValid(void) const { return ((ns!=NULL)&&(entry!=0)); }


	// [ ID�Ѵ��� ]

	// ����ȥ�̾������
	// ����� : ����ȥ�̾ʸ����
	string GetName(void) const;

	// ID������
	TEntryID GetID(void) const { return entry; }


	// [ �����ɲá������API ]

	// ���ꤵ�줿����ȥ����ˤ���
	// ����˶�����ȥ��ñ�줬�Ĥ�
	// ����� : ������true
	bool Clear(void);

	// ���Υ���ȥ�ʲ��Υ���ȥ�����ƶ��ˤ���
	// ����˶�����ȥ��ñ�줬�ĤäƤ��ɤ�
	void ClearTree(void);

	// ����ȥ�Ǹ����ؤ�ñ����ɲ�
	void Push(TWordID id);

	// ���ꤵ�줿����ȥ����ˤ��Ƥ���ñ����ɲ�
	// ����ȥ���ѿ�Ū�����Ѥ�����˻��Ѥ���
	void PushAfterClear(TWordID id);

	// ����ȥ�Ǹ�����ñ��κ��
	// ����� : ������줿ñ��
	TWordID Pop(void);

	// ����ȥ�����ؤ�ñ�������
	void Insert(unsigned int pos,TWordID id);

	// ����ȥ������ñ��κ��
	void Erase(unsigned int st,unsigned int end);

	// ����ȥ������ñ��������ؤ�
	// ����� : ������줿ñ��
	TWordID Replace(unsigned int pos,TWordID id);

	// ����ȥ������ñ��������ؤ�(����ǥå������ϰϳ��ξ�硢id2���ɲ�)
	// ����� : ������줿ñ��
	TWordID Replace2(unsigned int pos,TWordID id,TWordID id2);

	// ����ȥ�ؤν񤭹��ߤ�ػߤ���
	void WriteProtect(void);


	// [ ������API ]

	// ���ꤵ�줿����ȥ꤬�ޤޤ�뤫�ݤ����֤�
	// ����� : �ޤޤ����true
//	bool Contains(TEntryID id) const;

	// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
	// ����� : ñ���ID
	TWordID Index(unsigned int index=0) const;

	// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�
	// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
	unsigned int Find(TWordID id,unsigned int pos=0) const;

	// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�(�ս�)
	// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
	unsigned int RFind(TWordID id,unsigned int pos=NPos) const;

	// ���ꤵ�줿����ȥ��ñ����������
	// ����� : ñ��θĿ�
	unsigned int FindAll(vector<TWordID> &wordcol) const;

	// �ƥ���ȥ����
	// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
	TEntry GetParent(void) const;

	// ���֥���ȥ���������
	// ����� : ����ȥ�θĿ�
	unsigned int FindAllSubEntry(vector<TEntry> &entrycol) const;

	// ���ꤵ�줿����ȥ�̾����Ϥޤ륨��ȥ���������
	// ���Υ���ȥ��̵��
	// ����� : ����ȥ�θĿ�
	unsigned int FindTree(vector<TEntry> &entrycol) const;
};
//---------------------------------------------------------------------------
inline TNameSpace::~TNameSpace()
{
	ProtectedEntry.clear();
	ClearAllEntry();
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ꤬�ޤޤ�뤫�ݤ����֤�
// ����� : �ޤޤ����true
#if 0
inline bool TNameSpace::Contains(TEntryID id) const
{
	return EntryCollection.Contains(id);
}
#endif
//---------------------------------------------------------------------------
// WordCollection�Υҡ��פ����
inline void TNameSpace::Reserve(unsigned int n)
{
	EntryCollection.Reserve(n);
}
//---------------------------------------------------------------------------
// ͭ������ȥ�������
inline unsigned int TNameSpace::Size(void) const
{
	return Dictionary.size();
}
//---------------------------------------------------------------------------
// ����ȥ�ID����
// "."�ϥ롼�Ȥ򼨤���
// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
inline TEntry TNameSpace::Get(const string& entry)
{
	return((entry==".")?TEntry(this,0):TEntry(this, EntryCollection.Find(entry)));
}
//--------------------------------------------------------------------------
// ���ꤵ�줿ñ���ޤ२��ȥ꤬¸�ߤ��뤫Ĵ�٤�
// ����� : ¸�ߤ����true
inline bool TNameSpace::ContainsWord(TWordID id) const
{
	if (ReverseDictionary.count(id)==0) return false;
	TRDictionary::const_iterator it=ReverseDictionary.find(id);
	return (it->second.size()!=0);
}
//---------------------------------------------------------------------------
// �ƥ���ȥ����
inline TEntry TEntry::GetParent(void) const
{
	map<TEntryID,TEntryID>::const_iterator it=ns->ParentEntry.find(entry);
	return((it!=ns->ParentEntry.end())?(TEntry(ns,it->second)):(TEntry(ns,0)));
}
//---------------------------------------------------------------------------
// ����ȥ�ID���饨��ȥ�̾���Ѵ�
// ����� : ����ȥ�̾ʸ����
inline string TEntry::GetName(void) const
{
	string const*entryname=ns->EntryCollection.Find(entry);
	if (!entryname) return("");
	else return (*entryname);
}
//--------------------------------------------------------------------------
// �񤭹����ݸ�����å�
// ����� : ������ݸ��оݤʤ��true
inline bool TEntry::AssertIfProtected(void)
{
	if(IsValid()&&ns->ProtectedEntry.count(entry)){
		ns->gc->GetLogger().GetStream(kawari_log::LOG_ERROR)
			<< RC.S(kawari::resource::ERR_NS_ASSERT_PROTECTED_ENTRY1) << GetName()
			<< RC.S(kawari::resource::ERR_NS_ASSERT_PROTECTED_ENTRY2) << endl;
		return true;
	}else{
		return false;
	}
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ����ˤ��Ƥ���ñ����ɲ�
// ����ȥ���ѿ�Ū�����Ѥ�����˻��Ѥ���
inline void TEntry::PushAfterClear(TWordID id)
{
	Clear();
	Push(id);
}
//--------------------------------------------------------------------------
// ����ȥ�ؤν񤭹��ߤ�ػߤ���
inline void TEntry::WriteProtect(void)
{
	if(IsValid()) ns->ProtectedEntry.insert(entry);
}
//--------------------------------------------------------------------------
// ���Υ���ȥ���ä���硢�ٹ����˻Ĥ���
// �޾줷�Τ�Ū��
// ����� : ���ξ��true
inline bool TEntry::AssertIfEmpty(const string& name)
{
	if(((!IsValid())||(!Size()))&&ns->gc->GetLogger().Check(kawari_log::LOG_DECL)){
		ns->gc->GetLogger().GetStream() << RC.S(kawari::resource::WARN_NS_READ_EMPTY_ENTRY1) << name << RC.S(kawari::resource::WARN_NS_READ_EMPTY_ENTRY2) << endl;
		return true;
	}else{
		return false;
	}
}
//--------------------------------------------------------------------------
#endif
