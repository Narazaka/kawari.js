//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����
//
//      Programed by Suikyo.
//
//  2002.09.05  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
#ifndef SAORI_MODULE_H
#define SAORI_MODULE_H
//---------------------------------------------------------------------------
#include "config.h"
#include "libkawari/kawari_log.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
namespace saori{
typedef unsigned long SAORI_HANDLE;
//---------------------------------------------------------------------------
// SAORI�⥸�塼����������ե����ȥ�
class IModuleFactory{
public:
	// �⥸�塼��θ���������
	// �����: ����������������硢�⥸�塼�롣���Ԥ�����硢NULL��
	virtual class TModule *CreateModule(const std::string &path)=0;

	// �⥸�塼��δ����˴�
	// �饤�֥��ξ���FreeLibrary���뤳�ȡ�
	virtual void DeleteModule(class TModule *module)=0;

	// ���󥹥ȥ饯��
	IModuleFactory(TKawariLogger &lgr) : logger(lgr) {}

	// ���⤷�ʤ��ǥ��ȥ饯��
	virtual ~IModuleFactory() {}

	// �������֤�
	TKawariLogger &GetLogger(void) { return logger; }
private:
	TKawariLogger &logger;
};
//---------------------------------------------------------------------------
class TModuleFactoryMaster : public IModuleFactory{
public:
	TModuleFactoryMaster(class TKawariLogger &lgr);

	// �⥸�塼��θ���������
	// �����: ����������������硢�⥸�塼�롣���Ԥ�����硢NULL��
	class saori::TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	// �饤�֥��ξ���FreeLibrary���뤳�ȡ�
	void DeleteModule(saori::TModule *module);

	virtual ~TModuleFactoryMaster();

protected:
	std::vector<saori::IModuleFactory *> factory_list;
};
//---------------------------------------------------------------------------
// SAORI�⥸�塼�륤�󥿡��ե�����
// TModule�ϡ�Ʊ��ѥ��ˤĤ���ʣ����������뤳�Ȥ����롣
// ͣ�����γ��ݤϡ�SAORI_HANDLE�˴�Ť��ƾ�̥쥤�䡼�ǹԤ��롣
class TModule{
public:
	// �����
	virtual bool Initialize(void)=0;
	// SAORI/1.0 Load
	virtual bool Load(void)=0;
	// SAORI/1.0 Unload
	virtual bool Unload(void)=0;
	// SAORI/1.0 Request
	virtual std::string Request(const std::string &reqstr)=0;

	saori::SAORI_HANDLE GetHandle(void) {
		return handle;
	}

	// ���⤷�ʤ��ǥ��ȥ饯��
	// delete TModule���줿��硢Library�ϲ������ʤ����ȡ�
	virtual ~TModule (void){}

	virtual saori::IModuleFactory &GetFactory(void) {
		return factory;
	}

	TModule(saori::IModuleFactory &fac, const std::string &p, saori::SAORI_HANDLE hdl)
		 : factory(fac), path(p), handle(hdl) {}

protected:

	saori::IModuleFactory &factory;
	std::string path;
	saori::SAORI_HANDLE handle;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_MODULE_H
//---------------------------------------------------------------------------
