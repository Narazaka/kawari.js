//---------------------------------------------------------------------------
//
// ��ݲ����������󥿡��ե�����
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.08.12  Phase 6.2.1   FirstVersion
//
//---------------------------------------------------------------------------
#ifndef KOSUI_BASE_H
#define KOSUI_BASE_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
class TKawariInterface_base {
public:
	// �ǥ��ȥ饯��
	virtual ~TKawariInterface_base() {}

	// ���������
	virtual string GetInformation(void)=0;

	// Ϳ����줿������ץȤ��ᡦ�¹Ԥ���
	virtual string Parse(const string& script)=0;
};
//---------------------------------------------------------------------------
#endif

