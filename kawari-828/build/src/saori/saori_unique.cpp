//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����(ͣ�����ݾ�)
//
//      Programed by Suikyo.
//
//  2002.09.05  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
#include "config.h"
#include "saori/saori_unique.h"
#include "libkawari/kawari_log.h"
using namespace kawari_log;
//---------------------------------------------------------------------------
using namespace std;
using namespace saori;
//---------------------------------------------------------------------------
// �⥸�塼��θ���������
// �����: ����������������硢�⥸�塼�롣���Ԥ�����硢NULL��
TModule *TUniqueModuleFactory::CreateModule(const string &path){
	TModule *mod=parent->CreateModule(path);
	if (!mod) return NULL;

	TUniqueModule *ret=NULL;
	SAORI_HANDLE handle=mod->GetHandle();
	if (modules.count(handle)){
		ret=modules[handle];
		ret->loadcount++;
		delete mod;
	}else{
		ret=new TUniqueModule((*this), path, mod);
		modules[handle]=ret;
		mod->Load();
	}
	GetLogger().GetStream(LOG_INFO) << "[SAORI Unique] CreateModule loadcount=" << ret->loadcount << endl;
	return ret;
}
//---------------------------------------------------------------------------
// �⥸�塼��δ����˴�
// �饤�֥��ξ���FreeLibrary���뤳�ȡ�
void TUniqueModuleFactory::DeleteModule(TModule *module){
	GetLogger().GetStream(LOG_INFO) << "[SAORI Unique] DeleteModule " << endl;
	if (!module) return;
	SAORI_HANDLE handle=module->GetHandle();
	if (!modules.count(handle)) return;

	TUniqueModule *mod=modules[handle];
	GetLogger().GetStream(LOG_INFO) << "               loadcount=" << mod->loadcount << endl;
	if ((--mod->loadcount)==0) {
		modules.erase(handle);
		mod->module->Unload();
		parent->DeleteModule(mod->module);
		delete mod;
	}
}
//---------------------------------------------------------------------------
TUniqueModuleFactory::~TUniqueModuleFactory(){
	map<SAORI_HANDLE, TUniqueModule *>::iterator it=modules.begin();
	for (; it!=modules.end(); it++){
		TUniqueModule *mod=it->second;
		modules.erase(it);
		mod->module->Unload();
		parent->DeleteModule(mod->module);
		delete mod;
	}
	delete parent;
}
//---------------------------------------------------------------------------
