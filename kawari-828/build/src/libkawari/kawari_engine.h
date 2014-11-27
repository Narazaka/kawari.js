//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ���������󥸥�
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.05.27  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//                            ���饹��������
//  2001.05.31  Phase 5.2     �ݼ�Ūpiro
//  2001.06.09  Phase 5.3     �ޥå�����ȥ�
//  2001.06.10  Phase 5.3.1   ��Ĵ��
//  2001.06.17  Phase 5.4     save
//  2001.08.25  Phase 7.0     �������ƥ��к�(WriteProtect)
//  2001.12.08  Phase 7.1.2   �ƥ����ȥե������ɤ߹����б�
//  2002.03.10  Phase 7.9.0   Parse�������base�˰�ư
//                            �ƥ����ȥե�������ɤ߹��ߤ򳰤�(KIS����ʤ��Ρ�)
//                            ���⤽��FIS�γ�ȯ��ߤǰ�̣�Τʤ��ʤäƤ�
//                            TNS_Engine_base���ѻ�
//                            �Ĥ��Ǥ˥ޥå������ѻ�
//                            ���񥢥��������󥿡��ե���������
//  2002.04.18  Phase 8.0.0   VM���ѹ����ɽ�
//                            SAORI�б�
//  2005.06.28  Phase 8.2.3   DecodeEntryName���夨��GetEntryRangeƳ��
//
//---------------------------------------------------------------------------
#ifndef KAWARI_ENGINE_H
#define KAWARI_ENGINE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <set>
using namespace std;
//---------------------------------------------------------------------------
#include "libkawari/kawari_compiler.h"
#include "libkawari/kawari_dict.h"
#include "libkawari/kawari_code.h"
#include "libkawari/kawari_vm.h"
#include "libkawari/kawari_log.h"
#include "saori/saori.h"
//---------------------------------------------------------------------------
struct TEntryRange;
//---------------------------------------------------------------------------
class TKawariEngine {
private:
	// ����ե�����Υѥ�̾
	string DataPath;

	// ��
	TKawariLogger *logger;

	// ����
	TNS_KawariDictionary *Dictionary;

	// ������ץȥ��󥸥�
	class TKawariVM *KawariVM;

	// SAORI
	class saori::TSaoriPark *SaoriPark;

public:
	// �ϰϳ��Υ���ǥå���
	static const unsigned int NPos;

	// �����API

	// ��ñ��������
	// ����� : ñ��θĿ�
	unsigned int WordCollectionSize(void) const;

	// ͭ������ȥ�������
	// ����� : ����ȥ�θĿ�
	unsigned int EntryCollectionSize(void) const;

	// ���ꤵ�줿����ȥ��ñ��������
	// ����� : ñ��θĿ�
	unsigned int EntrySize(const string& entryname) const;


	// ����ȥ�̾API

	// ʸ����򥨥�ȥ�̾�ǻ��Ѳ�ǽ��ʸ����˥��󥳡��ɤ���
	static string EncodeEntryName(const string &orgsen);

	// ʸ����򥨥�ȥ�̾�ȥ���ǥå����˥ǥ����ɤ���
	// �� : Entry        ñ�ȤΥ���ȥ�̾
	//      Entry[i]     ����ȥ��i���ܤ�����
	//      Entry[i..j]  ����ȥ��i���ܤ���j���ܤޤǤ�����
	//      ����ǥå�������ξ��ϡ���������������
	// ����� :  0=ź����̵��
	//           1=ñ�����ǻ���
	//           2=�ϰϻ���
	//          -1=���顼
//	static int DecodeEntryName(const string &orgsen,string &entryname,int &st,int &end);
	// ����ͤ��ϰϤ�ɬ��0�ʾ�ˤʤ뤬������ȥꥵ������Ȥϸ¤�ʤ���
	TEntryRange GetEntryRange(const string &orgsen);

	// ����ID�Ѵ���API

	// ñ��ID����
	// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
	TWordID GetWordID(const string& word) const;

	// ID����ñ����Ѵ�
	// ����� : �ե���ѥ��뤷��ñ��ʸ����
	string GetWordFromID(TWordID id) const;

	// ����ȥ����
	// ����� : 1���ꥸ�󡢸��Ĥ���ʤ����0���֤�
	TEntry GetEntry(const string& entryname) const;


	// �����ɲá������API

	// ����ȥ����������
	// ���˥���ȥ꤬¸�ߤ�����ϡ�����������ID���֤�
	// ����� : ������������ȥ��ID
	TEntry CreateEntry(const string& entryname);

	// ñ�����������
	// ����ñ�줬¸�ߤ�����ϡ�����������ID���֤�
	// ����� : ��������ñ���ID
	TWordID CreateWord(const string& word);
	TWordID CreateStrWord(const string& word);

	// ���ꤵ�줿����ȥ����ˤ���
	// ����˶�����ȥ��ñ�줬�ĤäƤ��ɤ�
	void ClearEntry(const string& entryname);

	// ����ȥ�Ǹ����ؤ�ñ����ɲ�
	// ����� : ������true
	void Push(const string& entryname,const string& word);

	// ����ȥ�Ǹ�����ñ��κ��
	void Pop(const string& entryname);

	// ����ȥ�����ؤ�ñ�������
	void Insert(const string& entryname,unsigned int pos,const string& word);

	// ����ȥ������ñ��κ��
	// end��������뤳�Ȥ���ա�
	void Erase(const string& entryname,unsigned int st,unsigned int end);

	// ����ȥ������ñ��������ؤ�
	void Replace(const string& entryname,unsigned int pos,const string& word);

	// ����ȥ������ñ��������ؤ�(����ǥå������ϰϳ��ξ�硢id2���ɲ�)
	void Replace2(const string& entryname,unsigned int pos,const string& word,const string& word2);

	// ���ꤵ�줿����ȥ����ˤ��Ƥ���ñ����ɲ�
	// ����ȥ���ѿ�Ū�����Ѥ�����˻��Ѥ���
	void PushAfterClear(const string& entryname,const string& word);
	void PushStrAfterClear(const string& entryname,const string& word);

	// ���ꤵ�줿����ȥ�ؤν񤭹��ߤ�ػߤ���
	void WriteProtect(const string& entryname);

	// ���ꤵ�줿����ȥ�̾����Ϥޤ륨��ȥ�����ƶ��ˤ���
	// ����˶�����ȥ��ñ�줬�ĤäƤ��ɤ�
	// "."��������ȥꡦ������������
	void ClearTree(const string& spacename);


	// ������API

	// ���ꤵ�줿����ȥ꤬�����ɤ�����
	// ����� : ���ʤ�true
	bool Empty(const string& entryname) const;

	// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
	// ����� : �ե���ѥ��뤷��ñ��ʸ����
	string IndexWord(TEntry entry,unsigned int index=0) const;
	string IndexWord(const string& entryname,unsigned int index=0) const;

	// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�
	// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
	unsigned int Find(const string& entryname,const string& word,unsigned int pos=0) const;

	// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�(�ս�)
	// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
	unsigned int RFind(const string& entryname,const string& word,unsigned int pos=NPos) const;

	// ����ȥ꽸��黻��Ԥ�����̤���󤹤�
	// ����� : ñ��θĿ�
	unsigned int CalcEntryExpression(const string &entryexpr, set<TWordID> &wordcol) const;

	// ����ȥ���������
	// ����� : ����ȥ�θĿ�
	unsigned int FindAllEntry(vector<TEntry> &entrycol) const;


	// �¹Է�API

	// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ���¹Ԥ���
	// ����� string : ����
	string IndexParse(TEntry entry,unsigned int index=0);
	string IndexParse(const string& entryname,unsigned int index=0);

	// ���ꤵ�줿ID��ñ��(������ץ�)��¹Ԥ���
	// ����� string : ����
	string Parse(TWordID id);

	// Ϳ����줿������ץȤ��ᡦ�¹Ԥ���
	// ����� string : ����
	string Parse(const string& script);

	// KIS�ؿ�(�ӥ�ȥ���ڤӥ桼�����)�μ¹�
	string FunctionCall(const vector<string> &args);

	// �ӥ�ȥ���ؿ��ξ��������
	// name : (����)�ؿ�̾
	// info : (����)�ؿ�̾��ʸˡ������͡����ͤΥꥹ��
	// �����(bool) : �ؿ���¸�ߤ����true
	bool GetFunctionInfo(const std::string &name, struct TKisFunctionInfo &info);

	// �ӥ�ȥ���ؿ��Υꥹ��
	// list : (����)�ؿ�̾�ꥹ��
	// �����(unsigned int) : �ؿ��ο�
	unsigned int GetFunctionList(std::vector<std::string> &list) const;


	// SAORI�ط�API

	enum SAORILOADTYPE {
		PRELOAD=saori::PRELOAD,
		LOADONCALL,
		NORESIDENT
	};

	// SAORI�⥸�塼�����Ͽ
	void RegisterSAORIModule(const std::string &aliasname, const std::string &path, const SAORILOADTYPE type);

	// SAORI�⥸�塼����Ͽ�κ��
	void EraseSAORIModule(const std::string &aliasname);

	// SAORI�ꥯ�����Ȥ�Ԥ�
	bool RequestToSAORIModule(
		const std::string &aliasname,
		const TPHMessage &request, TPHMessage &response);

	// ��Ͽ���줿SAORI�⥸�塼��Υꥹ�Ȥ�����
	int ListSAORIModule(std::vector<string> &list);

	// ����SAORI�⥸�塼��ξ��������


	// ����ƥ����ȴط� API

	// ���򻲾�
	string GetHistory(int index);


	// �ե�����API

	// ����ե�����Υѥ�̾�����ꤹ��
	void SetDataPath(const string &datapath);

	// ����ե�����Υѥ�̾���������
	string GetDataPath(void) const;

	// �������ե����ޥåȼ���ե�������ɤ߹���
	// ����� : ������true
	bool LoadKawariDict(const string &filename);

	// �������ե����ޥåȼ���ե������񤭹���
	// ����� : ������true
	bool SaveKawariDict(const string &filename,const vector<string>& entry,bool crypt=false) const;

public:

	// �����μ���
	TKawariLogger &GetLogger(void) const;

	TKawariEngine(void);

	~TKawariEngine();

};
//---------------------------------------------------------------------------
// ����ȥ���ΰ����
// range == false : no index (Start=0, End=max(Size()-1, 0))
// start == end : single word
// start < end  : multiple words
// start == TKawariEngine::NPos  : error
struct TEntryRange {
	string Name;
	TEntry Entry;
	bool Range;
	unsigned int Start;
	unsigned int End;

	TEntryRange(const string &name, const TEntry &entry)
		:Name(name), Entry(entry), Range(false),
		 Start(0), End((entry.Size()>0)?entry.Size()-1:0) {}
	TEntryRange(const string &name, const TEntry &entry,
				unsigned int start, unsigned int end)
		:Name(name), Entry(entry), Range(true), Start(start), End(end) {}
	TEntryRange(const TEntryRange &range)
		:Name(range.Name), Entry(range.Entry),
		 Range(range.Range), Start(range.Start), End(range.End) {}
};
//---------------------------------------------------------------------------
// ��ñ��������
inline unsigned int TKawariEngine::WordCollectionSize(void) const
{
	return(Dictionary->WordCollectionSize());
}
//---------------------------------------------------------------------------
// ͭ������ȥ�������
inline unsigned int TKawariEngine::EntryCollectionSize(void) const
{
	return(Dictionary->Size());
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��ñ���
inline unsigned int TKawariEngine::EntrySize(const string& entryname) const
{
	return(Dictionary->GetEntry(entryname).Size());
}
//---------------------------------------------------------------------------
// ñ��ID����
inline TWordID TKawariEngine::GetWordID(const string& word) const
{
	TKVMCode_base *code=TKawariCompiler::Compile(word, GetLogger());
	TWordID id=Dictionary->GetWordID(code);
	delete code;

	return(id);
}
//---------------------------------------------------------------------------
// ����ȥ����
inline TEntry TKawariEngine::GetEntry(const string& entryname) const
{
	return(Dictionary->GetEntry(entryname));
}
//---------------------------------------------------------------------------
// ����ȥ����������
inline TEntry TKawariEngine::CreateEntry(const string& entryname)
{
	return(Dictionary->CreateEntry(entryname));
}
//---------------------------------------------------------------------------
// ñ�����������
inline TWordID TKawariEngine::CreateWord(const string& word)
{
	return(Dictionary->CreateWord(TKawariCompiler::Compile(word, (*logger))));
}
//---------------------------------------------------------------------------
inline TWordID TKawariEngine::CreateStrWord(const string& word)
{
	return(Dictionary->CreateWord(TKawariCompiler::CompileAsString(word)));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ����ˤ���
inline void TKawariEngine::ClearEntry(const string& entryname)
{
	Dictionary->GetEntry(entryname).Clear();
}
//---------------------------------------------------------------------------
// ����ؤ�ñ����ɲ�
inline void TKawariEngine::Push(const string& entryname,const string& word)
{
	Dictionary->CreateEntry(entryname).Push(CreateWord(word));
}
//---------------------------------------------------------------------------
// ����ȥ�Ǹ�����ñ��κ��
inline void TKawariEngine::Pop(const string& entryname)
{
	Dictionary->GetEntry(entryname).Pop();
}
//---------------------------------------------------------------------------
// ����ȥ�����ؤ�ñ�������
inline void TKawariEngine::Insert(const string& entryname,unsigned int pos,const string& word)
{
	Dictionary->CreateEntry(entryname).Insert(pos,CreateWord(word));
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��κ��
inline void TKawariEngine::Erase(const string& entryname,unsigned int st,unsigned int end)
{
	Dictionary->GetEntry(entryname).Erase(st, end);
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��������ؤ�
inline void TKawariEngine::Replace(const string& entryname,unsigned int pos,const string& word)
{
	Dictionary->CreateEntry(entryname).Replace(pos,CreateWord(word));
}
//---------------------------------------------------------------------------
// ����ȥ������ñ��������ؤ�(����ǥå������ϰϳ��ξ�硢id2���ɲ�)
inline void TKawariEngine::Replace2(const string& entryname,unsigned int pos,const string& word,const string& word2)
{
	Dictionary->CreateEntry(entryname).Replace2(pos,CreateWord(word),CreateWord(word2));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ����ˤ��Ƥ���ñ����ɲ�
inline void TKawariEngine::PushAfterClear(const string& entryname,const string& word)
{
	Dictionary->CreateEntry(entryname).PushAfterClear(CreateWord(word));
}
//---------------------------------------------------------------------------
inline void TKawariEngine::PushStrAfterClear(const string& entryname,const string& word)
{
	Dictionary->CreateEntry(entryname).PushAfterClear(CreateStrWord(word));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�ؤν񤭹��ߤ�ػߤ���
inline void TKawariEngine::WriteProtect(const string& entryname)
{
	Dictionary->CreateEntry(entryname).WriteProtect();
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ꤬�����ɤ�����
// ����� : ���ʤ�true
inline bool TKawariEngine::Empty(const string& entryname) const
{
	return(GetEntry(entryname).Size()==0);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
inline string TKawariEngine::IndexWord(TEntry entry,unsigned int index) const
{
	TWordID id=entry.Index(index);
	if(id==0) return("");
	return(GetWordFromID(id));
}
//---------------------------------------------------------------------------
inline string TKawariEngine::IndexWord(const string& entryname,unsigned int index) const
{
	return(GetWordFromID(Dictionary->GetEntry(entryname).Index(index)));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�
// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
inline unsigned int TKawariEngine::Find(const string& entryname,const string& word,unsigned int pos) const
{
//	return(Find(GetEntryID(entryname),GetWordID(word),pos));
	return Dictionary->GetEntry(entryname).Find(GetWordID(word),pos);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ��⤫����ꤷ��ñ��򸡺�(�ս�)
// ����� : ����ǥå���(���Ĥ���ʤ����NPos)
inline unsigned int TKawariEngine::RFind(const string& entryname,const string& word,unsigned int pos) const
{
//	return(RFind(GetEntryID(entryname),GetWordID(word),pos));
	return Dictionary->GetEntry(entryname).RFind(GetWordID(word),pos);
}
//---------------------------------------------------------------------------
// ����ȥ�̾���������
inline unsigned int TKawariEngine::FindAllEntry(vector<TEntry> &entrycol) const
{
	return(Dictionary->FindAllEntry(entrycol));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
inline string TKawariEngine::IndexParse(TEntry entry,unsigned int index)
{
	if(!entry.IsValid()) return("");
	return(Parse(entry.Index(index)));
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�λ��ꤷ������(0���ꥸ��)��ñ����֤�
inline string TKawariEngine::IndexParse(const string& entryname,unsigned int index)
{
	return(Parse(Dictionary->GetEntry(entryname).Index(index)));
}
//--------------------------------------------------------------------------
// KIS�ؿ���¹Ԥ���
inline string TKawariEngine::FunctionCall(const vector<string> &args){
	return KawariVM->FunctionCall(args);
}
//---------------------------------------------------------------------------
// �ӥ�ȥ���ؿ��ξ��������
inline bool TKawariEngine::GetFunctionInfo(const string &name, struct TKisFunctionInfo &info){
	return KawariVM->GetFunctionInfo(name, info);
}
//---------------------------------------------------------------------------
// �ӥ�ȥ���ؿ��Υꥹ��
inline unsigned int TKawariEngine::GetFunctionList(vector<string> &list) const{
	return KawariVM->GetFunctionList(list);
}
//--------------------------------------------------------------------------
// ���򻲾�
inline std::string TKawariEngine::GetHistory(int index){
	return Dictionary->GetHistory(index);
}
//---------------------------------------------------------------------------
// ����ե�����Υѥ�̾�����ꤹ��
inline void TKawariEngine::SetDataPath(const string &datapath)
{
	DataPath=datapath;
}
//---------------------------------------------------------------------------
// ����ե�����Υѥ�̾���������
inline string TKawariEngine::GetDataPath(void) const
{
	return(DataPath);
}
//---------------------------------------------------------------------------
// �����μ���
inline TKawariLogger &TKawariEngine::GetLogger(void) const
{
	return (*logger);
}
//---------------------------------------------------------------------------
#endif
