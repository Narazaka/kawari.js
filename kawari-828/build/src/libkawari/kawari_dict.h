//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.04.21  Phase 0.50a1   ��������Υ��饹��
//  2001.05.03  Phase 0.50a4  ����饤�󥹥���ץ�
//  2001.05.26  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//                            API����
//  2001.06.17  Phase 6.0     ʣ������ȥ�ؤ�Ʊ���ɲäΥХ�����
//  2002.03.10  Phase 7.9.0   ���񥢥��������󥿡��ե���������
//  2002.05.09  Phase 8.0.0   ̾�����ֵ�ǽ��Composite Pattern��ʬΥ
//                            KIS++�ؤ����С�
//
//---------------------------------------------------------------------------
#ifndef KAWARI_DICT_H
#define KAWARI_DICT_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
//---------------------------------------------------------------------------
#include "libkawari/kawari_ns.h"
#include "libkawari/kawari_code.h"
#include "libkawari/wordcollection.h"
//---------------------------------------------------------------------------
// ����������
class TNS_KawariDictionary: public TGarbageCollector {
private:
	// ̾�����ִط�

	// �����Х��̾������
	TNameSpace *GlobalNameSpace;

	// ����ƥ�����
	class TContext : public TNameSpace {
	public:
		// ���򻲾ȥ����å�
		std::vector<std::string> history;
		// �롼�פ�����ҿ�
		unsigned int loopcount;

		TContext(TGarbageCollector *col): TNameSpace(col), loopcount(0) {}
		virtual ~TContext() {}
	};

	// NameSpace������
	TNameSpace *SearchNameSpace(const string &entry) {
		if (IsLocalEntry(entry)) return GetCurrentContext();
		else return GlobalNameSpace;
	}


	// ñ��ط�

	// ���٤Ƥ�ñ����ݻ�����
	TWordPointerCollection<TKVMCode_base,TKVMCode_baseP_Less> WordCollection;

	// �������ñ��
	set<TWordID> Garbage;

	// PVW���ݻ�����
	set<TWordID> PVWSet;


	// ����ƥ����ȴط�

	// ����ƥ����ȥ����å�
	std::vector<TContext *> ContextStack;

	// ���ߤΥ���ƥ����Ȥ�����
	// ̵�����NULL���֤�
	TContext *GetCurrentContext(void) const;

	// �����륨��ȥ�̾��Ƚ��
	bool IsLocalEntry(const std::string &entry_name) const;


	// ��
	class TKawariLogger &logger;
public:
	TNS_KawariDictionary (TKawariLogger &lgr) : logger(lgr) {
		GlobalNameSpace = new TNameSpace(this);
		GlobalNameSpace->Reserve(2000);
		WordCollection.Reserve(10000);
	}
	virtual ~TNS_KawariDictionary (){
		delete GlobalNameSpace;
		GlobalNameSpace = NULL;
	}

	// �ϰϳ��Υ���ǥå���
	static const unsigned int NPos;		// UINT_MAX


	// �����API

	// ��ñ��������
	// ����� : ñ��θĿ� (������줿ñ��Ͽ������ʤ�)
	unsigned int WordCollectionSize(void) const;

	// ͭ������ȥ�������
	// ����� : ����ȥ�θĿ�
	unsigned int Size(void) const;

	// ���ꤵ�줿����ȥ��ñ��������
	// ����� : ñ��θĿ�
//	unsigned int EntrySize(TEntryID entry) const;


	// ����ȥ��ϢAPI

	// ����ȥ����
	// ����� : ����ȥ�
	TEntry GetEntry(const string &entry);

	// ����ȥ����������
	// ���˥���ȥ꤬¸�ߤ�����ϡ�����������ID���֤�
	// ����� : ������������ȥ��ID
	TEntry CreateEntry(const string& entry);

	// ����ȥ�ID���������
	// ����� : ����ȥ�θĿ�
	unsigned int FindAllEntry(vector<TEntry> &entrycol) const;

	// ���ꤵ�줿����ȥ����Ƥ˴ޤޤ��ñ���
	// ��貾��ñ���${����ȥ�̾}�פΤ�Ÿ�����ƺƵ�Ū����󤹤�
	// ����� : ñ��θĿ�
	unsigned int GetWordCollection(TEntry entry,set<TWordID> &wordcol);


	// ñ���ϢAPI

	// ñ�����������
	// ����ñ�줬¸�ߤ�����ϡ�����������ID���֤�
	// ��ա�����ñ��ID�����ñ��Ǥ��ä���硢delete�����
	// ����� : ��������ñ���ID
	TWordID CreateWord(TKVMCode_base* word);

	// ñ��ID����
	// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
	TWordID GetWordID(TKVMCode_base* word) const;

	// ID����ñ����Ѵ�
	// ����� : ��֥����ɥĥ꡼
	TKVMCode_base *GetWordFromID(TWordID id) const;

	// ñ��˺���ޡ������դ���
	void MarkWordForGC(TWordID id);


	// ����ƥ����ȴ�ϢAPI

	// ���ߤΥ���ƥ����Ȥ����򻲾ȥ����å��Υݥ��󥿤����
	unsigned int LinkFrame(void);

	// ���ߤΥ���ƥ����Ȥ����򻲾ȥ����å��Υݥ��󥿤�����
	void UnlinkFrame(unsigned int pos);

	// �롼�פ�����
	void StartLoop(void);

	// �롼�פ�Ф�
	void EndLoop(void);

	// ���ߤΥ롼�׳��ؤ�����
	unsigned int CurrentLoop(void);

	// ����������ƥ����Ȥ�������������å���push
	void CreateContext(void);

	// ����ƥ����Ȥ�pop
	// ���줬�Ǹ�Υ���ƥ����ȤǤ����硢���٥å����쥯�����
	void DeleteContext(void);

	// ����ƥ����ȥ����å��θ��ߤο���
	unsigned int GetContextStackDepth(void);

	// ���򻲾ȥ����å����ִ����ʸ�����push���롣
	void PushToHistory (const string &str);

	// ���򻲾�
	string GetHistory (int index);

	// �������֤�
	TKawariLogger &GetLogger(void) {
		return logger;
	}

};
//--------------------------------------------------------------------------
// �����륨��ȥ�̾��Ƚ��
inline bool TNS_KawariDictionary::IsLocalEntry(const std::string &entry_name) const{
	return (entry_name.size()&&(entry_name[0]=='@'));
}
//---------------------------------------------------------------------------
// ����ȥ�ID����
inline TEntry TNS_KawariDictionary::GetEntry(const string& entry)
{
	TNameSpace *ns=SearchNameSpace(entry);
	return (ns) ? ns->Get(entry):TEntry(GlobalNameSpace, 0);
}
//---------------------------------------------------------------------------
// ��ñ��������
inline unsigned int TNS_KawariDictionary::WordCollectionSize(void) const
{
	return(WordCollection.Size());
}
//---------------------------------------------------------------------------
// ͭ������ȥ�������
inline unsigned int TNS_KawariDictionary::Size(void) const
{
	return(GlobalNameSpace->Size());
}
//---------------------------------------------------------------------------
// ñ��ID����
inline TWordID TNS_KawariDictionary::GetWordID(TKVMCode_base* word) const
{
	return(WordCollection.Find(word));
}
//---------------------------------------------------------------------------
// ID����ñ����Ѵ�
// ����� : ��֥����ɥĥ꡼
inline TKVMCode_base *TNS_KawariDictionary::GetWordFromID(TWordID id) const
{
	TKVMCode_base *const*word=WordCollection.Find(id);
	return(word?(*word):NULL);
}
//---------------------------------------------------------------------------
// ñ��˺���ޡ������դ���
inline void TNS_KawariDictionary::MarkWordForGC(TWordID id)
{
	Garbage.insert(id);
}
//---------------------------------------------------------------------------
// ����ȥ�̾���������
// ����� : ����ȥ�θĿ�
inline unsigned int TNS_KawariDictionary::FindAllEntry(vector<TEntry> &entrycol) const
{
	return GlobalNameSpace->FindAllEntry(entrycol);
}
//--------------------------------------------------------------------------
// ���ߤΥ���ƥ����Ȥ�����
inline TNS_KawariDictionary::TContext *TNS_KawariDictionary::GetCurrentContext(void) const{
	if (ContextStack.size())
		return ContextStack.back();
	else
		return NULL;
}
//--------------------------------------------------------------------------
// ����ƥ����ȥ����å��θ��ߤο���
inline unsigned int TNS_KawariDictionary::GetContextStackDepth(void){
	return ContextStack.size();
}
//--------------------------------------------------------------------------
// �롼�פ�����
inline void TNS_KawariDictionary::StartLoop(void){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return;
	ctx->loopcount++;
}
//--------------------------------------------------------------------------
// �롼�פ�Ф�
inline void TNS_KawariDictionary::EndLoop(void){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return;
	if (ctx->loopcount) ctx->loopcount--;
}
//--------------------------------------------------------------------------
// ���ߤΥ롼�׳��ؤ�����
inline unsigned int TNS_KawariDictionary::CurrentLoop(void){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return 0;
	return ctx->loopcount;
}
//--------------------------------------------------------------------------
#endif
