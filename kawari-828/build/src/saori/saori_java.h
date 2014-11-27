//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����(java�⥸�塼��)
//
//      Programed by Suikyo.
//
//  2003.02.25  Phase 8.1.0   Ƴ��
//
//---------------------------------------------------------------------------
/**
	Java�������б��Τ���ˡ�Java¦��ɬ�פʤ��:
	- VM
	  o Java(TM)2�ʹ�
	  o ���餫����ˡ�ǵ�ư�Ѥ�VM��¤�ΤؤΥݥ��󥿤��Ϥ���Ƥ���
	- �ե����ȥꥯ�饹
	  o public�ʰ����ʤ����󥹥ȥ饯�������
	  o public Object createModule(byte[] name)�᥽�åɤ����
	  o �����ƥ�ץ�ѥƥ�"jp.nanika.saoriinterface"�˴������ꥯ�饹̾����Ͽ
        ex) net/sf/kawari/SaoriFactory
	- �⥸�塼�륯�饹
	  o �ե����ȥ��createModule�ˤ�ä����������
	  o public boolean load(byte[] libpath);
	    public boolean unload(void);
	    public byte[] request(byte[] reqstr);
	    �λ��ĤΥ᥽�åɤ����
	*/
//---------------------------------------------------------------------------
#ifndef SAORI_JAVA_H
#define SAORI_JAVA_H
//---------------------------------------------------------------------------
#include "config.h"
#include "saori/saori_module.h"
//---------------------------------------------------------------------------
#include <jni.h>
#include <string>
#include <map>
//---------------------------------------------------------------------------
namespace saori{
//---------------------------------------------------------------------------
class TModuleFactoryJava : public IModuleFactory{
protected:
	// �ե����ȥ�ؤΥ����Х뻲��
	jclass cls_SaoriFactory;
	jobject obj_SaoriFactory;

	// �ե����ȥꥯ�饹�Υ᥽�åɡ�
	// "java.lang.Object createModule(java.lang.String name)"�Υ᥽�å�ID
	// �����ͥ���� "(Ljava/lang/String;)Ljava/lang/Object"
	jmethodID mid_createModule;

public:
	// �⥸�塼��θ���������
	// �����: ����������������硢���󥹥��󥹡����Ԥ�����硢NULL��
	virtual TModule *CreateModule(const std::string &path);

	// �⥸�塼��δ����˴�
	virtual void DeleteModule(TModule *module);

	// ���󥹥ȥ饯��
	TModuleFactoryJava(class TKawariLogger &lgr);

	// �ǥ��ȥ饯��
	virtual ~TModuleFactoryJava(void);

	friend class TModuleJava;
};
//---------------------------------------------------------------------------
class TModuleJava : public TModule{
public:
	// �����
	virtual bool Initialize(void);
	// SAORI/1.0 Load
	virtual bool Load(void);
	// SAORI/1.0 Unload
	virtual bool Unload(void);
	// SAORI/1.0 Request
	virtual std::string Request(const std::string &reqstr);

	virtual ~TModuleJava();

protected:
	// SAORI_HANDLE�ϡ������ꥪ�֥������ȼ��ΤؤΥ����Х뻲��
	TModuleJava(TModuleFactoryJava &fac, const std::string &p, SAORI_HANDLE handle)
		: TModule(fac, p, handle){}

	// ������ؤΥ����Х뻲��
	jclass cls_saori;
	jobject obj_saori;

	// �����ꥯ�饹�γƥ᥽�å�ID
	// boolean load(byte[] libpath)		"([B)Z"
	// boolean unload(void);			"(V)Z"
	// byte[] request(byte[] reqstr);	"([B)[B"
	jmethodID mid_load;
	jmethodID mid_unload;
	jmethodID mid_request;

	friend class TModuleFactoryJava;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_JAVA_H
//---------------------------------------------------------------------------
