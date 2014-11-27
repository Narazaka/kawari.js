//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����
//
//      Programed by Suikyo.
//
//  2002.09.05  Phase 8.1.0   Ƴ��
//  2005.06.05  Phase 8.2.3   ninix-aya�б�
//
//---------------------------------------------------------------------------
#include "config.h"
#include "saori/saori_module.h"
#include "saori/saori_unique.h"
#ifdef ENABLE_SAORI_NATIVE
#	include "saori/saori_native.h"
#endif
#ifdef ENABLE_SAORI_JAVA
#	include "saori/saori_java.h"
#endif
#ifdef ENABLE_SAORI_PYTHON
#	include "saori/saori_python.h"
#endif
using namespace kawari_log;
//---------------------------------------------------------------------------
using namespace std;
using namespace saori;
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
TModuleFactoryMaster::TModuleFactoryMaster(TKawariLogger &lgr)
	: IModuleFactory(lgr)
{
#ifdef ENABLE_SAORI_JAVA
	factory_list.push_back(new TModuleFactoryJava(GetLogger()));
#endif
#ifdef ENABLE_SAORI_PYTHON
	factory_list.push_back(new TModuleFactoryPython(GetLogger()));
#endif
#ifdef ENABLE_SAORI_NATIVE
	factory_list.push_back(new TUniqueModuleFactory(new TModuleFactoryNative(GetLogger())));
#endif
}
//---------------------------------------------------------------------------
// �⥸�塼��θ���������
// �����: ����������������硢�⥸�塼�롣���Ԥ�����硢NULL��
TModule *TModuleFactoryMaster::CreateModule(const string &path){
	vector<IModuleFactory *>::iterator it=factory_list.begin();
	for (; it!=factory_list.end(); it++){
		TModule *ret=(*it)->CreateModule(path);
		if (ret) return ret;
	}
	return NULL;
}
//---------------------------------------------------------------------------
// �⥸�塼��δ����˴�
// �饤�֥��ξ���FreeLibrary���뤳�ȡ�
void TModuleFactoryMaster::DeleteModule(TModule *module){
	IModuleFactory &factory=module->GetFactory();
	factory.DeleteModule(module);
}
//---------------------------------------------------------------------------
TModuleFactoryMaster::~TModuleFactoryMaster(){
	vector<IModuleFactory *>::iterator it=factory_list.begin();
	for (; it!=factory_list.end(); it++){
		IModuleFactory *factory=(*it);
		delete factory;
	}
}
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
