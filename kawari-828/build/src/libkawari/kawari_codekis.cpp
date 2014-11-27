//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��֥�����(KIS��ʸ)
//
//      Programed by Suikyo.
//
//  2002.04.18  Phase 8.0.0   KIS��ʸ��֥����ɺ���
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "libkawari/kawari_vm.h"
#include "libkawari/kawari_codekis.h"
#include "misc/misc.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
// KIS IFʸ

// �¹�
string TKVMKISCodeIF::Run(TKawariVM &vm){
	const unsigned int cmax=condlist.size();
	bool finished=false;
	string retstr;
	unsigned int i=0;
	unsigned int frame=vm.Dictionary().LinkFrame();
	for (; i<cmax; i++){
		string cond=condlist[i]->Run(vm);
		vm.Dictionary().UnlinkFrame(frame);
		if (IsTrue(cond)){
			vm.Dictionary().PushToHistory(cond);
			retstr=list[i]->Run(vm);
			finished=true;
			break;
		}
	}
	if ((!finished)&&(i<list.size())){
		vm.Dictionary().UnlinkFrame(frame);
		retstr=list[i]->Run(vm);
	}
	vm.Dictionary().UnlinkFrame(frame);
	vm.Dictionary().PushToHistory(retstr);
	return retstr;
}
// �ե���ѥ���
string TKVMKISCodeIF::DisCompile(void) const{
	const unsigned int cmax=condlist.size();
	const unsigned int max=list.size();
	string retstr;
	unsigned int i=0;
	for (; i<cmax; i++){
		retstr+="if "+condlist[i]->DisCompile()+" "+list[i]->DisCompile();
		if ((i+1)<max) retstr+=" else ";
	}
	if (i<max)
		retstr+=list[i]->DisCompile();
	return retstr;
}
// �ǥХå��ѥĥ꡼ɽ��
ostream &TKVMKISCodeIF::Debug(ostream& os, unsigned int level) const{
	const unsigned int cmax=condlist.size();
	const unsigned int max=list.size();
	unsigned int i=0;
	DebugIndent(os, level) << "(" << endl;
	for (; i<cmax; i++){
		DebugIndent(os, level) << "IF(" << endl;
		condlist[i]->Debug(os, level+1);
		DebugIndent(os, level) << ")THEN(" << endl;
		list[i]->Debug(os, level+1);
		if (i<max)
			DebugIndent(os, level) << "ELSE" << endl;
	}
	if (i<max){
		list[i]->Debug(os, level+1);
		DebugIndent(os, level) << ")" << endl;
	}
	return os;
}
// ���� Ʊ�����饹�ξ��Τ߸ƤФ�롣
bool TKVMKISCodeIF::Less(const TKVMCode_base& r_) const{
	const TKVMKISCodeIF &r=dynamic_cast<const TKVMKISCodeIF &>(r_);
	unsigned int cmax=condlist.size();
	unsigned int lmax=list.size();
	if (cmax!=r.condlist.size()) return(cmax<r.condlist.size());
	if (lmax!=r.list.size()) return(lmax<r.list.size());

	for(unsigned int i=0; i<cmax; i++){
		if (TKVMCode_baseP_Less()(condlist[i], r.condlist[i])) return true;
		if (TKVMCode_baseP_Less()(r.condlist[i], condlist[i])) return false;
	}
	for(unsigned int i=0; i<lmax; i++){
		if (TKVMCode_baseP_Less()(list[i], r.list[i])) return true;
		if (TKVMCode_baseP_Less()(r.list[i], list[i])) return false;
	}
	return false;
}
// ���󥹥ȥ饯��
TKVMKISCodeIF::TKVMKISCodeIF(const vector<TKVMCode_base *> &clist,
		   const vector<TKVMCode_base *> &l){
	if ((clist.size()==l.size())||((clist.size()+1)==l.size())){
		condlist.insert(condlist.end(), clist.begin(), clist.end());
		list.insert(list.end(), l.begin(), l.end());
	}
}
// �ǥ��ȥ饯��
TKVMKISCodeIF::~TKVMKISCodeIF(){
	for (TCodePVector::iterator it=condlist.begin(); it!=condlist.end(); it++)
		if (*it) delete (*it);
	for (TCodePVector::iterator it=list.begin(); it!=list.end(); it++)
		if (*it) delete (*it);
}
//---------------------------------------------------------------------------
