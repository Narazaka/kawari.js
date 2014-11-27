//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ����
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.04.21  Phase 0.50a1  ��������Υ��饹��
//  2001.05.03  Phase 0.50a4  ����饤�󥹥���ץ�
//  2001.05.26  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//                            API����
//  2001.06.17  Phase 5.4     ʣ������ȥ�ؤ�Ʊ���ɲäΥХ�����
//                            �ե���ѥ���
//  2001.12.16  Phase 7.2     ClearEntry(TEntryID entry)�ΥХ�����
//                            (Thanks: ����)
//  2002.03.10  Phase 7.9.0   ���񥢥��������󥿡��ե���������
//  2002.05.09  Phase 8.0.0   ̾�����ֵ�ǽ��ʬΥ��
//                            KIS++�ؤ����С�
//  2002.05.20                PVW����Υ���ȥ�̾������¹Ի��˻����ۤ�
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_dict.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_compiler.h"
#include "libkawari/kawari_codeset.h"
#include "libkawari/kawari_log.h"
#include "libkawari/kawari_rc.h"
#include "misc/misc.h"
using namespace kawari::resource;
using namespace kawari_log;
//---------------------------------------------------------------------------
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
// �ϰϳ��Υ���ǥå���
const unsigned int TNS_KawariDictionary::NPos=UINT_MAX;
//---------------------------------------------------------------------------
// ����ȥ����������
// ���˥���ȥ꤬¸�ߤ�����ϡ�����������ID���֤�
// ����� : ������������ȥ��ID
TEntry TNS_KawariDictionary::CreateEntry(const string& entry)
{
	TNameSpace *ns=SearchNameSpace(entry);
	return ((ns)?ns->Create(entry): TEntry(GlobalNameSpace,0));
}
//---------------------------------------------------------------------------
// ñ�����������
// ����ñ�줬¸�ߤ�����ϡ�����������ID���֤�
// ��ա�����ñ��ID�����ñ��Ǥ��ä���硢delete�����
// ����� : ������������ȥ��ID
TWordID TNS_KawariDictionary::CreateWord(TKVMCode_base* word)
{
	if(!word) return(0);

	TWordID id=0;
	if(!WordCollection.Insert(word,&id)) {
		// ������Ͽ�Ѥߤ�ñ����ä�
		delete word;
		word=GetWordFromID(id);
	}else{
		// ��貾��ñ��
		TKVMCodePVW *pvw=dynamic_cast<TKVMCodePVW *>(word);
		if (pvw)
			PVWSet.insert(id);
	}

	return(id);
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ����Ƥ˴ޤޤ��ñ���
// ��貾��ñ���${����ȥ�̾}�פΤ�Ÿ�����ƺƵ�Ū����󤹤�
// ����� : ñ��θĿ�
unsigned int TNS_KawariDictionary::GetWordCollection(TEntry start_entry,set<TWordID> &wordcol)
{
	// ���˸����ѤߤΥ���ȥ�
	set<TEntry> donelist;
	// ����ͽ��Υ���ȥ�
	vector<TEntry> parselist;

	parselist.push_back(start_entry);

	while(parselist.size()) {
		// ������
		TEntry entry=parselist.back();
		parselist.pop_back();
		if(donelist.count(entry)) continue;
		donelist.insert(entry);

		vector<TWordID> tmpcol;
		entry.FindAll(tmpcol);
		for(vector<TWordID>::iterator it=tmpcol.begin(); it!=tmpcol.end(); it++) {
			TWordID id=(*it);
			if(PVWSet.count(id)){
				// ��貾��ñ��
				TKVMCode_base *code=GetWordFromID(id);
				if(!code) continue;
				TKVMCodePVW *pvw=dynamic_cast<TKVMCodePVW *>(code);
				if(!pvw) continue;
				TEntry child=GetEntry(pvw->Get());
				if(child.IsValid()&&(donelist.count(child)==0))
					parselist.push_back(child);
			}else{
				wordcol.insert(id);
			}
		}
	}

	return(wordcol.size());
}
//---------------------------------------------------------------------------
// ���ߤΥ���ƥ����ȤΥ����å��ե졼��Υݥ��󥿤����
// (�������á��褦��MC68000��LINK��)
unsigned int TNS_KawariDictionary::LinkFrame(void){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return 0;
	return ctx->history.size();
}
//---------------------------------------------------------------------------
// ���ߤΥ���ƥ����ȤΥ����å��ե졼��Υݥ��󥿤�����
// (�������á��褦��MC68000��UNLINK��)
void TNS_KawariDictionary::UnlinkFrame(unsigned int pos){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return ;
	if (pos<ctx->history.size()) ctx->history.resize(pos);
}
//---------------------------------------------------------------------------
// ����������ƥ����Ȥ�������������å���push
void TNS_KawariDictionary::CreateContext(void){
	ContextStack.push_back(new TContext(this));
}
//---------------------------------------------------------------------------
// ����ƥ����Ȥ�pop
// ���ͽ��ñ����ɲá�ɬ�פʤ�Х��٥å����쥯�����
void TNS_KawariDictionary::DeleteContext(void){
	if (ContextStack.size()){
		if(ContextStack.back()) delete ContextStack.back();
		ContextStack.pop_back();
	}
	if (!ContextStack.size()){
		// GC
		if(logger.Check(LOG_DUMP)){
			ostream &logstream=logger.GetStream(LOG_DUMP);
			for (set<TWordID>::iterator it=Garbage.begin(); it!=Garbage.end(); it++){
				TWordID id=(*it);
				if (!GlobalNameSpace->ContainsWord(id)){
					// �հ�����¸�ߤ��ʤ���
					TKVMCode_base *pword=*WordCollection.Find(id);
					WordCollection.Delete(id);
					if (pword){
						logstream << RC.S(DUMP_DICT_DELETE_WORD1) <<id<< RC.S(DUMP_DICT_DELETE_WORD2) << (pword)->DisCompile() << endl;
						delete (pword);
					}else{
						logger.GetStream(LOG_ERROR) << RC.S(ERR_DICT_CAN_NOT_DELETE_WORD1) << id << RC.S(ERR_DICT_CAN_NOT_DELETE_WORD2) << endl;
					}
				}
			}
		}else{
			for (set<TWordID>::iterator it=Garbage.begin(); it!=Garbage.end(); it++){
				TWordID id=(*it);
				if (!GlobalNameSpace->ContainsWord(id)){
					// �հ�����¸�ߤ��ʤ���
					TKVMCode_base *pword=*WordCollection.Find(id);
					WordCollection.Delete(id);
					if (pword){
						delete (pword);
					}else{
						logger.GetStream(LOG_ERROR) << RC.S(ERR_DICT_CAN_NOT_DELETE_WORD1) << id << RC.S(ERR_DICT_CAN_NOT_DELETE_WORD2) << endl;
					}
				}
			}
		}
		Garbage.clear();
	}
}
//--------------------------------------------------------------------------
// ���򻲾ȥ����å����ִ����ʸ�����push���롣
void TNS_KawariDictionary::PushToHistory (const string &str){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return;
	ctx->history.push_back(str);
}
//---------------------------------------------------------------------------
// ���򻲾�
string TNS_KawariDictionary::GetHistory (int index){
	TContext *ctx=GetCurrentContext();
	if (!ctx) return "";
	if (index<0)
		index = ctx->history.size()+index;
	if ((index<0)||(ctx->history.size()>INT_MAX)||(index>=(int)ctx->history.size())){
		return "";
	}else{
		return ctx->history[index];
	}
}
//---------------------------------------------------------------------------
