//---------------------------------------------------------------------------
//
// TPHMessage - ��HTTP��å����� -
//
//      Programed by Suikyo.
//
//---------------------------------------------------------------------------
#ifndef PHTTP_H
#define PHTTP_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "misc/mmap.h"
//---------------------------------------------------------------------------
#include <iostream>
#include <string>
//---------------------------------------------------------------------------
class TPHMessage : public TMMap<std::string, std::string> {
private:
	std::string startline;
public:
	// �������ȥ饤�������
	void SetStartline(const std::string &line) { startline=line; }

	// �������ȥ饤�������
	std::string GetStartline(void) const { return startline; }

	// ���ꥢ�饤��
	std::string Serialize(void) const;

	// �ǥ��ꥢ�饤��
	void Deserialize(const std::string &mes);

	// �����
	void Dump(std::ostream &os) const;
};
//---------------------------------------------------------------------------
#endif // PHTTP_H
