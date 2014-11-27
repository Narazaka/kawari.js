//---------------------------------------------------------------------------
//
// ����ȊO�̉����ȊO�̉���
// OBJECT SHIORI
//
//      Programed by Suikyo.
//
//  2003.01.24  Phase 8.1.0   �U�ь���Q�l��
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "include/shiori_object.h"
#include "shiori/kawari_shiori.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace {
	// utility(pointer must be free)
	inline const char *string2cstr(const string &s, long *length){
		*length=s.length();
		char *d=new char[(int)*length];
		s.copy(d, (int)*length);
		return d;
	}
}
//---------------------------------------------------------------------------
// ���C�u�����̏���������
int so_library_init(void){
	return 1;
}
//---------------------------------------------------------------------------
// ���C�u�����̏I������
int so_library_cleanup(void){
	TKawariShioriFactory::DisposeFactory();
	return 1;
}
//---------------------------------------------------------------------------
// ���W���[���ԍ���Ԃ�
const char *so_getmoduleversion(long *length){
	string modver=TKawariShioriFactory::GetModuleVersion();
	return string2cstr(modver, length);
}
//---------------------------------------------------------------------------
// �V�K�C���X�^���X�̐���
SO_HANDLE so_create(const char *argument, long length){
	return TKawariShioriFactory::GetFactory().CreateInstance(
		string(argument, length));
}
//---------------------------------------------------------------------------
// �C���X�^���X�̍폜
int so_dispose(SO_HANDLE h){
	return (int)TKawariShioriFactory::GetFactory().DisposeInstance((int)h);
}
//---------------------------------------------------------------------------
// �UAI���N�G�X�g
//   �߂�l�̃|�C���^�͂Ȃ�ׂ�����so_free()�ŉ�����邱�ƁB
//   requeststr : �u�����B(��)�vSHIORI/2.x, SAORI/1.0�K�i�Ɋ�Â�
//                 �G���R�[�h���ꂽ����
//
//   returns NULL if library is not ready.
//
const char *so_request(SO_HANDLE h, const char *reqstr, long *length){
	string res=TKawariShioriFactory::GetFactory().RequestInstance(
		(int)h, string(reqstr, *length));

	return string2cstr(res, length);
}
//---------------------------------------------------------------------------
// Free Memory
void so_free(SO_HANDLE h, const char *ptr){
	char *p=(char *)ptr;
	// 'delete NULL;' is safe.
	delete[] p;
}
//---------------------------------------------------------------------------
