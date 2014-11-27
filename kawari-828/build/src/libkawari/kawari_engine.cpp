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
//  2001.06.10  Phase 5.3.1   ���٥��̾�ǻȤ��뵭����_�פ����.�פ��ѹ�
//  2001.06.17  Phase 5.4     ʣ������ȥ�ؤ�Ʊ���ɲäΥХ�����
//                            save
//  2001.08.06  Phase 6.2     ��³�Ԥ��б�
//  2001.08.07  Phase 6.2     ostrstream�Х�����
//                            ��³�Ԥ��(��ʸˡ�Ƹ�Ƥ)
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
//  2004.08.28  Phase 8.2.1   DecodeEntryName�����̸����˼��Ԥ��������н�
//  2005.06.28  Phase 8.2.3   DecodeEntryName���夨��GetEntryRangeƳ��
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_engine.h"
#include "libkawari/kawari_dict.h"
#include "libkawari/kawari_compiler.h"
#include "libkawari/kawari_vm.h"
#include "libkawari/kawari_crypt.h"
#include "libkawari/kawari_codeset.h"
#include "libkawari/kawari_rc.h"
#include "misc/misc.h"
#include "saori/saori.h"
using namespace kawari::resource;
//---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
//---------------------------------------------------------------------------
// �ϰϳ��Υ���ǥå���
const unsigned int TKawariEngine::NPos=TNS_KawariDictionary::NPos;
//---------------------------------------------------------------------------
TKawariEngine::TKawariEngine(void)
{
	logger=new TKawariLogger();
	Dictionary=new TNS_KawariDictionary((*logger));
	KawariVM=new TKawariVM(*this, *Dictionary, (*logger));
	SaoriPark=new saori::TSaoriPark((*logger));
}
//---------------------------------------------------------------------------
TKawariEngine::~TKawariEngine()
{
	delete SaoriPark;
	delete KawariVM;
	delete Dictionary;
	delete logger;
}
//---------------------------------------------------------------------------
string TKawariEngine::EncodeEntryName(const string &orgsen){
	return TKawariCompiler::EncodeEntryName(orgsen);
}
//---------------------------------------------------------------------------
// ʸ����򥨥�ȥ�̾�ȥ���ǥå����˥ǥ����ɤ���
#if 0
int TKawariEngine::DecodeEntryName(const string &orgsen,string &entryname,int &st,int &end)
{
	entryname=orgsen;
	st=0; end=-1;

	string::size_type pos3=orgsen.size()-1;
	string::size_type pos1=orgsen.rfind('[');
	if((orgsen.size()==0)||(orgsen[pos3]!=']')||(pos1==string::npos))
		// ����ǥå�����̵���ä�
		return 0;

	string::size_type pos2=orgsen.find("..",pos1);
	if(pos2!=string::npos) {
		// �ϰϻ���
		string s_st=orgsen.substr(pos1+1,pos2-(pos1+1));
		string s_end=orgsen.substr(pos2+2,pos3-(pos2+2));

		if ((s_st.find_first_not_of("0123456789- ")==string::npos)&&
			(s_end.find_first_not_of("0123456789- ")==string::npos)){

			entryname=orgsen.substr(0,pos1);
			st=atoi(s_st.c_str());
			end=atoi(s_end.c_str());
			return 2;
		}

		// �����δְ㤤���ä�
		return 0;
	}

	string s_idx=orgsen.substr(pos1+1,pos3-(pos1+1));
	if (s_idx.find_first_not_of("0123456789- ")!=string::npos)
		// ���������Ǥ��襫�ƥ��ʤ���
		return 0;

	// ñ�����ǻ���
	entryname=orgsen.substr(0, pos1);
	st=end=atoi(s_idx.c_str());
	return 1;
}
#endif
//---------------------------------------------------------------------------
TEntryRange TKawariEngine::GetEntryRange(const string &orgsen)
{
	string::size_type idx_last=orgsen.size()-1;
	string::size_type idx_obrk=orgsen.rfind('[');
	if((orgsen.size()==0)||(orgsen[idx_last]!=']')||(idx_obrk==string::npos)){
		// ����ǥå���̵��
		TEntry entry=CreateEntry(orgsen);
		return TEntryRange(orgsen, entry);
	}

	string::size_type idx_range=orgsen.find("..",idx_obrk);
	if(idx_range!=string::npos) {
		// �ΰ����
		string s_st=StringTrim(orgsen.substr(idx_obrk+1,idx_range-(idx_obrk+1)));
		string s_end=StringTrim(orgsen.substr(idx_range+2,idx_last-(idx_range+2)));
		string entryname=orgsen.substr(0,idx_obrk);
		TEntry entry=CreateEntry(entryname);

		// �����Τߤ�����դ���
		if(IsInteger(s_st)&&IsInteger(s_end)){
			int st=atoi(s_st.c_str());
			int end=atoi(s_end.c_str());
			if(st<0) st+=entry.Size();
			if(end<0) end+=entry.Size();
			if((st>=0)&&(end>=0)&&(st<=end))
				return TEntryRange(entryname, entry,
								   static_cast<unsigned int>(st),
								   static_cast<unsigned int>(end));
		}
		// error
		return TEntryRange(entryname, entry, NPos, NPos);

	}else{
		// ñ�����ǻ���
		string s_idx=StringTrim(orgsen.substr(idx_obrk+1,idx_last-(idx_obrk+1)));
		string entryname=orgsen.substr(0, idx_obrk);
		TEntry entry=CreateEntry(entryname);

		if (IsInteger(s_idx)){
			int idx=atoi(s_idx.c_str());
			if(idx<0) idx+=entry.Size();
			if(idx>=0)
				return TEntryRange(entryname, entry,
								   static_cast<unsigned int>(idx),
								   static_cast<unsigned int>(idx));
		}
		// error
		return TEntryRange(entryname, entry, NPos, NPos);
	}
}
//---------------------------------------------------------------------------
// ID����ñ����Ѵ�
string TKawariEngine::GetWordFromID(TWordID id) const
{
	if(!id) return("");
	TKVMCode_base *code=Dictionary->GetWordFromID(id);
	if(!code) return("");

	return(code->DisCompile());
}
//---------------------------------------------------------------------------
// ���ꤵ�줿����ȥ�̾����Ϥޤ륨��ȥ�����ƶ��ˤ���
// ����˶�����ȥ��ñ�줬�ĤäƤ��ɤ�
void TKawariEngine::ClearTree(const string& spacename)
{
	if(spacename!=".") {
		TEntry space=CreateEntry(spacename);
		space.ClearTree();
	}else{
		vector<TEntry> entrycol;
		Dictionary->FindAllEntry(entrycol);
		for(vector<TEntry>::iterator it=entrycol.begin(); it!=entrycol.end(); it++)
			it->Clear();
	}
}
//---------------------------------------------------------------------------
// ����ȥ꽸��黻��Ԥ�����̤���󤹤�
// ����� : ñ��θĿ�
unsigned int TKawariEngine::CalcEntryExpression(const string &entryexpr, set<TWordID> &wordcol) const{
	TKVMSetCode_base *code=TKawariCompiler::CompileAsEntryExpression(entryexpr, (*logger));
	if (!code) return 0;
	code->Evaluate(*KawariVM, wordcol);
	delete code;
	return wordcol.size();
}
//---------------------------------------------------------------------------
// ���ꤵ�줿ID��ñ��(������ץ�)��¹Ԥ���
string TKawariEngine::Parse(TWordID id)
{
	if(id==0) return("");

	TKVMCode_base *code=Dictionary->GetWordFromID(id);
	if(!code) return("");

	return(KawariVM->RunWithNewContext(code));
}
//---------------------------------------------------------------------------
// Ϳ����줿������ץȤ��ᡦ�¹Ԥ���
string TKawariEngine::Parse(const string& script)
{
	TKVMCode_base *code=TKawariCompiler::Compile(script, GetLogger());
	if(!code) return("");

	// �ǥХå�
	if(logger->Check(kawari_log::LOG_DUMP))
		code->Debug(logger->GetStream());

	string retstr=KawariVM->RunWithNewContext(code);
	delete code;

	return(retstr);
}

//---------------------------------------------------------------------------
// �������ե����ޥåȼ���ե�������ɤ߹���
bool TKawariEngine::LoadKawariDict(const string &filename)
{
	// Debug Assertion��
	if(!filename.size()) return(false);

	ifstream ifs;
	ifs.open(filename.c_str(), ios::binary|ios::in);

	if(!ifs.is_open()) return(false);

	// ������ε��ҥ⡼�ɤδ���
	vector<TKawariCompiler::Mode> mode;
	mode.push_back(TKawariCompiler::M_DICT);

	TKawariCompiler compiler(ifs, (*logger), filename);

	while(true) {
		if(mode.back()==TKawariCompiler::M_DICT){
			// ��������ļ����ɤ߹���
			while(true){
				vector<string> entrynames;
				vector<TKVMCode_base *> sentence_list;
				if (!compiler.LoadEntryDefinition(entrynames, sentence_list))
					break;

				vector <TEntry> entrylist;
				for(vector<string>::iterator it=entrynames.begin(); it!=entrynames.end(); it++) {
					TEntry entry=Dictionary->CreateEntry(*it);
					if(entry.IsValid())
						entrylist.push_back(entry);
				}

				for (vector<TKVMCode_base *>::iterator it=sentence_list.begin(); it!=sentence_list.end(); it++){
					TWordID wid=Dictionary->CreateWord(*it);
					if (wid)
						for (vector<TEntry>::iterator eit=entrylist.begin(); eit!=entrylist.end(); eit++)
							eit->Push(wid);
				}
			}

		}else if(mode.back()==TKawariCompiler::M_KIS){
			// KIS�ɤ߹���
			TKVMCode_base *code=compiler.LoadInlineScript();

			string outstr=KawariVM->RunWithNewContext(code);
			if (outstr.size())
				logger->GetStream(kawari_log::LOG_INFO) << "KIS zone output:" << outstr << endl;
			delete code;
		}else{
			// ??
		}
		TKawariCompiler::Mode m=compiler.GetNextMode();
		if (m==TKawariCompiler::M_DICT){
			// ����⡼��
			mode.push_back(TKawariCompiler::M_DICT);
		}else if (m==TKawariCompiler::M_KIS){
			// ������ץȥ⡼��
			mode.push_back(TKawariCompiler::M_KIS);
		}else if (m==TKawariCompiler::M_END){
			// �⡼�ɽ�λ
			if(mode.size()>1) mode.pop_back();
		}else if (m==TKawariCompiler::M_UNKNOWN){
			// �Τ�ʤ��⡼��
			logger->GetStream(kawari_log::LOG_ERROR) << RC.S(ERR_ENGINE_UNKNOWN_MODE) << endl;
		}else if (m==TKawariCompiler::M_EOF){
			break;
		}
	}

	return(true);
}

//---------------------------------------------------------------------------
// �������ե����ޥåȼ���ե������񤭹���
bool TKawariEngine::SaveKawariDict(const string &filename,const vector<string>& entry,bool crypt) const
{
	ofstream ofs;
	ofs.open(filename.c_str());

	if(!ofs.is_open()) return(false);

	ofs << "#" << endl << "# Kawari saved file" << endl << "#" << endl;

	for(vector<string>::const_iterator it=entry.begin(); it!=entry.end(); it++) {
		ofs << "# Entry " << (*it) << endl;

		vector<TWordID> wordcol;
		TEntry entry=GetEntry(*it);
		if (entry.IsValid())
			entry.FindAll(wordcol);
		if(wordcol.size()) {
			string line;
			line=(*it)+" : "+GetWordFromID(wordcol[0]);

			unsigned int wordcolmax=wordcol.size();
			for(unsigned int j=1;j<wordcolmax;j++) {
				line+=" , ";
				line+=GetWordFromID(wordcol[j]);
			}

			if(!crypt) {
				ofs << line << endl;
			} else {
				ofs << EncryptString(line) << endl;
			}
		}
	}

	ofs.close();

	return(true);
}
//---------------------------------------------------------------------------
// SAORI�⥸�塼�����Ͽ
void TKawariEngine::RegisterSAORIModule(const string &aliasname, const string &path, const SAORILOADTYPE type){
	SaoriPark->RegisterModule(aliasname, path, (saori::LOADTYPE)type);
}
//---------------------------------------------------------------------------
// SAORI�⥸�塼����Ͽ�κ��
void TKawariEngine::EraseSAORIModule(const string &aliasname){
	SaoriPark->EraseModule(aliasname);
}
//---------------------------------------------------------------------------
// SAORI�ꥯ�����Ȥ�Ԥ�
bool TKawariEngine::RequestToSAORIModule(
	const string &aliasname,
	const TPHMessage &request, TPHMessage &response)
{
	saori::TBind *bind=SaoriPark->GetModule(aliasname);
	if (!bind)
		return false;
	else
		return bind->Request(request, response);
}
//---------------------------------------------------------------------------
// ��Ͽ���줿SAORI�⥸�塼��Υꥹ�Ȥ�����
int TKawariEngine::ListSAORIModule(vector<string> &list)
{
	return SaoriPark->ListModule(list);
}
//---------------------------------------------------------------------------
