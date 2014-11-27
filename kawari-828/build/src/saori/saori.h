//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// SAORI ���󥿡��ե�����
//
//      Programed by Suikyo.
//
//  2002.04.15  Phase 8.0.0   ���Ӥ��蘆��С������򻲹ͤ�Ƴ��
//
//---------------------------------------------------------------------------
#ifndef SAORI_H
#define SAORI_H
//---------------------------------------------------------------------------
#include "config.h"
#include "misc/phttp.h"
#include "libkawari/kawari_log.h"
//---------------------------------------------------------------------------
#include <string>
#include <map>
#include <vector>
//---------------------------------------------------------------------------
namespace saori{
enum LOADTYPE {
	PRELOAD,
	LOADONCALL,
	NORESIDENT
};
class SAORIInfo{
public:
	std::string path;
	int bindcount;
	int loadcount;

	SAORIInfo(const std::string &p, int bc, int lc): path(p), bindcount(bc), loadcount(lc) {}
};
//---------------------------------------------------------------------------
// SAORI��������
class TBind{
public:
	// SAORI/1.0 �ꥯ�����Ȥ�ȯ��
	// �饤�֥��Υ��ɡܥ�󥯤�ɬ�פ˱����ƹԤ��롣
	// ����� : request/response�ץ�ȥ��뤬���������TRUE
	bool Request(const TPHMessage &request, TPHMessage &response);

	TBind(class IModuleFactory &fac, TKawariLogger &lgr, const std::string &path, const saori::LOADTYPE t);

	~TBind();

private:
	saori::LOADTYPE type;
	std::string libpath;
	class TModule *module;
	class IModuleFactory &factory;
	TKawariLogger &logger;

	bool Query(const TPHMessage &request, TPHMessage &response);
	void Attach(void);
	void Detach(void);

};
//---------------------------------------------------------------------------
// Facade���饹
class TSaoriPark {
public:
	// �⥸�塼�����Ͽ
	void RegisterModule(const std::string &alias, const std::string &path, const LOADTYPE type);

	// �⥸�塼����Ͽ�κ��
	void EraseModule(const std::string &alias);

	// �⥸�塼�������
	TBind *const GetModule(const std::string &alias);

	// ��Ͽ�Ѥߥ⥸�塼��̾�Υꥹ�Ȥ�����
	int ListModule(std::vector<std::string> &list);

	// ���󥹥ȥ饯��
	TSaoriPark(TKawariLogger &logger);

	// ���⥸�塼��Υ������
	~TSaoriPark();

private:
	class TModuleFactoryMaster *factory;
	TKawariLogger &logger;

	// alias -> Module
	std::map<std::string, TBind *> aliasmap;

	friend class TBind;
};
//---------------------------------------------------------------------------
} // namespace saori
//---------------------------------------------------------------------------
#endif // SAORI_H
