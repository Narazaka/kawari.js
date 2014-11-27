//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����(Win32)
//
//      Programed by Suikyo.
//
//  2002.09.05  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
#ifndef SAORI_WIN32_H
#define SAORI_WIN32_H
//---------------------------------------------------------------------------
#include <windows.h>
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
class TModuleFactoryWin32 : public IModuleFactory{
public:
	// �⥸�塼��θ���������
	// �����: ����������������硢���󥹥��󥹡����Ԥ�����硢NULL��
	virtual TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	virtual void DeleteModule(TModule *module);

	// ���󥹥ȥ饯��
	TModuleFactoryWin32(TKawariLogger &lgr);

	// �ǥ��ȥ饯��
	virtual ~TModuleFactoryWin32(void);
};
//---------------------------------------------------------------------------
class TModuleWin32 : public TModule{
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
	TModuleWin32(TModuleFactoryWin32 &fac, const std::string &p, SAORI_HANDLE handle)
		 : TModule(fac, p, handle) {}

	BOOL	(__cdecl *func_load)(HGLOBAL, long);
	HGLOBAL	(__cdecl *func_request)(HGLOBAL, long *);
	BOOL	(__cdecl *func_unload)();

	friend class TModuleFactoryWin32;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_WIN32_H
//---------------------------------------------------------------------------
