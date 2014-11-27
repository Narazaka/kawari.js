//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- SHIORI/2.5 URL�ꥹ�� --
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.08.08  Phase 6.2     First version
//  2001.08.12  Phase 6.2.1   ������[2]�ɲ�
//                            ���ͽ�Х��ˤ��줿(;_;)
//  2001.08.25  Phase 6.3     chr�ɲá������������餷����
//
//---------------------------------------------------------------------------
// �ؿ��ơ��֥�ؤ���Ͽ
#ifdef INLINE_SCRIPT_REGIST
INLINE_SCRIPT_REGIST(KIS_urllist);
INLINE_SCRIPT_REGIST(KIS_chr);
#else
//---------------------------------------------------------------------------
#ifndef KIS_URLLIST_H
#define KIS_URLLIST_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_base.h"
//---------------------------------------------------------------------------
class KIS_urllist : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="urllist";
		Format_="urllist SITE1 URL1 BannerURL1 ... SITEn URLn BannerURLn";
		Returnval_="SITE1[1]URL1[1]BannerURL1[2]...SITEn[1]URLn[1]BannerURLn[2]";
		Information_="return a recommend list for SHIORI/2.5 standard";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
class KIS_chr : public TKisFunction_base {
public:

	// Init��̾������¾�ξ�������ꤷ�Ƥ�������
	virtual bool Init(void)
	{
		Name_="chr";
		Format_="chr CharCode1";
		Returnval_="a character";
		Information_="return a character which character code is CharCode1";

		return(true);
	}

	// ���󥿡��ץ꥿
	virtual string Function(const vector<string>& args);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
#endif

