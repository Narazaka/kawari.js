//---------------------------------------------------------------------------
//
// "�ؘa��" for ����ȊO�̉����ȊO�̉���
// SAORI �C���^�[�t�F�[�X(python���W���[��)
//
//              ABE, Suikyo
//
//  2003.03.07  Phase 8.1.0   ����
//
//---------------------------------------------------------------------------
#ifndef SAORI_PYTHON_H
#define SAORI_PYTHON_H
//---------------------------------------------------------------------------
#include "config.h"
#include "include/shiori.h"
#include "saori/saori_module.h"
//---------------------------------------------------------------------------
#include <Python.h>
#include <string>
#include <map>
//---------------------------------------------------------------------------
// ������
PyObject *wrap_setcallback(PyObject *self, PyObject *args);
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
class TModuleFactoryPython : public IModuleFactory{
public:
	// ���W���[���̌����Ɛ���
	// �߂�l: �����ɐ��������ꍇ�A�C���X�^���X�B���s�����ꍇ�ANULL�B
	virtual TModule *CreateModule(const std::string &path);

	// ���W���[���̊��S�j��
	virtual void DeleteModule(TModule *module);

	// �R���X�g���N�^
	TModuleFactoryPython(class TKawariLogger &lgr);

	// �f�X�g���N�^
	virtual ~TModuleFactoryPython(void);
};
//---------------------------------------------------------------------------
class TModulePython : public TModule{
public:
	// ������
	virtual bool Initialize(void);
	// SAORI/1.0 Load
	virtual bool Load(void);
	// SAORI/1.0 Unload
	virtual bool Unload(void);
	// SAORI/1.0 Request
	virtual std::string Request(const std::string &reqstr);

protected:
	TModulePython(TModuleFactoryPython &fac, const std::string &p, SAORI_HANDLE handle)
		 : TModule(fac, p, handle) {}

	friend class TModuleFactoryPython;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_PYTHON_H
//---------------------------------------------------------------------------
