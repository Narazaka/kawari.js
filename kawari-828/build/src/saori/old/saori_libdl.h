//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����(libdl)
//
//      Programed by Suikyo.
//
//  2002.09.05  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
#ifndef SAORI_LIBDL_H
#define SAORI_LIBDL_H
//---------------------------------------------------------------------------
#include "config.h"
#include "libkawari/kawari_log.h"
#include "saori/saori_module.h"
//---------------------------------------------------------------------------
#include <string>
#include <map>
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
class TModuleFactoryLdl : public IModuleFactory{
public:
	// �⥸�塼��θ���������
	// �����: ����������������硢���󥹥��󥹡����Ԥ�����硢NULL��
	virtual TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	virtual void DeleteModule(TModule *module);

	// ���󥹥ȥ饯��
	TModuleFactoryLdl(TKawariLogger &lgr);

	// �ǥ��ȥ饯��
	virtual ~TModuleFactoryLdl(void);
};
//---------------------------------------------------------------------------
class TModuleLdl : public TModule{
public:
	// �����
	virtual bool Initialize(void);
	// SAORI/1.0 Load
	virtual bool Load(void);
	// SAORI/1.0 Unload
	virtual bool Unload(void);
	// SAORI/1.0 Request
	virtual std::string Request(const std::string &reqstr);

protected:
	TModuleLdl(TModuleFactoryLdl &fac, const std::string &p, SAORI_HANDLE handle)
		 : TModule(fac, p, handle) {}

	int (*func_load)(const char *datapath);
	char *(*func_request)(const char *requeststr);
	int (*func_unload)(void);

	friend class TModuleFactoryLdl;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_LIBDL_H
//---------------------------------------------------------------------------
