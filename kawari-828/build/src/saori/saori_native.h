//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����(native�⥸�塼��)
//
//      Programed by Suikyo.
//
//  2003.02.25  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
#ifndef SAORI_NATIVE_H
#define SAORI_NATIVE_H
//---------------------------------------------------------------------------
#include "config.h"
#include "saori/saori_module.h"
#include "include/shiori.h"
//---------------------------------------------------------------------------
#include <string>
#include <map>
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
class TModuleFactoryNative : public IModuleFactory{
public:
	// �⥸�塼��θ���������
	// �����: ����������������硢���󥹥��󥹡����Ԥ�����硢NULL��
	virtual TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	virtual void DeleteModule(TModule *module);

	// ���󥹥ȥ饯��
	TModuleFactoryNative(class TKawariLogger &lgr);

	// �ǥ��ȥ饯��
	virtual ~TModuleFactoryNative(void);
};
//---------------------------------------------------------------------------
class TModuleNative : public TModule{
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
	TModuleNative(TModuleFactoryNative &fac, const std::string &p, SAORI_HANDLE handle)
		 : TModule(fac, p, handle) {}

	BOOL	(SHIORI_CALL *func_load)(MEMORY_HANDLE, long);
	BOOL	(SHIORI_CALL *func_unload)(void);
	MEMORY_HANDLE	(SHIORI_CALL *func_request)(MEMORY_HANDLE, long *);

	friend class TModuleFactoryNative;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_NATIVE_H
//---------------------------------------------------------------------------
