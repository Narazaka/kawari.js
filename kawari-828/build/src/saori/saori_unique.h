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
#ifndef SAORI_UNIQUE_H
#define SAORI_UNIQUE_H
//---------------------------------------------------------------------------
#include "config.h"
#include "saori/saori_module.h"
//---------------------------------------------------------------------------
#include <string>
#include <map>
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
// ���ȥ�����Ȥ��������Load/Unload�ʤɤ�ư�ǸƤӽФ���𥯥饹
// Darwin�ʤɤ�ObjectModel�ʥ����ƥ�Ǥϡ�bipass����ʤ���Фʤ�ʤ���
class TUniqueModuleFactory : public IModuleFactory{
public:
	TUniqueModuleFactory(IModuleFactory *fac)
		: IModuleFactory(fac->GetLogger()), parent(fac) {}
	virtual ~TUniqueModuleFactory();

	// �⥸�塼��θ���������
	// �����: ����������������硢�⥸�塼�롣���Ԥ�����硢NULL��
	virtual TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	// �饤�֥��ξ���FreeLibrary���뤳�ȡ�
	virtual void DeleteModule(TModule *module);

protected:
	IModuleFactory *parent;
	std::map<SAORI_HANDLE, class TUniqueModule *> modules;
};
//---------------------------------------------------------------------------
class TUniqueModule : public TModule{
public:
	// �����
	virtual bool Initialize(void) { return module->Initialize(); }
	// SAORI/1.0 Load
	virtual bool Load(void) { return true; }
	// SAORI/1.0 Unload
	virtual bool Unload(void) { return true; }
	// SAORI/1.0 Request
	virtual std::string Request(const std::string &reqstr) {
		return module->Request(reqstr);
	}
	saori::SAORI_HANDLE GetHandle(void) {
		return module->GetHandle();
	}

	unsigned int GetLoadCount(void){
		return loadcount;
	}
	TUniqueModule(class IModuleFactory &fac, const std::string &p, TModule *m)
		 : TModule(fac, p, m->GetHandle()), module(m), loadcount(1) {}

	virtual ~TUniqueModule () {}
protected:
	TModule *module;
	unsigned int loadcount;
	friend class TUniqueModuleFactory;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif // SAORI_MODULE_H
//---------------------------------------------------------------------------
