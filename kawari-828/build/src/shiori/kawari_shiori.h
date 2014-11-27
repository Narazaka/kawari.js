//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// ��AI������
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.02.03  Phase 0.3     �Ρ�������
//  2001.02.11  Phase 0.31    �Ƶ��������
//  2001.02.23  Phase 0.4     ´�����轪�ﵧǰ
//                            ����¤餫�ˤ�̲�겼����������Ϸ����֤��ޤ����
//                                                                �С������
//                            kawari.iniƳ��
//                            ʣ������ե�����
//                            sentence.txt�ѻ�
//  2001.03.15  Phase 0.42    unloadͽ��
//  2001.04.25  Phase 0.50a1  �쥹�ݥ��б�
//  2001.04.27  Phase 0.50a2  SHIORI/2.1�б�
//  2001.04.28  Phase 0.50a3  COMMUNICATE����
//       |
//  2001.05.02
//  2001.05.03  Phase 0.50a4  ����饤�󥹥���ץ�
//  2001.05.12  Phase 0.50    Piroject-X ������
//                            SHIORI/2.2�б�
//                            SHIORI/1.1�ڤ�Τ�
//  2001.05.27  Phase 5.1     ���󥿡��ץ꥿������ѥ��鲽
//  2001.05.31  Phase 5.2     �ݼ�Ūpiro
//  2001.06.10  Phase 5.3.1   GET Version�к�
//                            ��SHIORI/2.4���� (^_^;
//  2001.06.18  Phase 5.4     ����饤�󥹥���ץ���������
//  2001.07.10  Phase 6.0     getmoduleversion�ɲ�
//  2001.07.14  Phase 6.1     BCC����ϲ�������к�
//  2001.07.19  Phase 6.2     SHIORI/2.5����
//  2001.08.08  Phase 6.2     �����Ѥ�LoadSub����
//  2001.08.25  Phase 6.3     �������ƥ����ۡ����к�
//  2001.08.25  Phase 7.0     �������ƥ��к�(WriteProtect)
//  2001.09.23  Phase 7.0.1   �������ƥ��к���redo34�ʹߤ��������б�
//                            Sender��ϩ�����ɲ�(System.Sender.Path)
//                            3����ȥ�ʾ�Ǥ�AND�����Х�����
//                            SHIORI/2.3b�б�
//                            ����饤�󥹥���ץȶ���
//                            ��ư®�٤ι�®��
//                            �ϥ��ե����Ѥ���쥨��ȥ���б����
//  2002.01.12  Phase 8.0     �ʤ��ʤ�����ʤ�Phase8
//                            NID����ե����ɥХå�
//                            kpcg Phase7.3.1����ե����ɥХå�
//  2002.03                   Phase 8.0 (Rhapsody in the Blue)���˴�
//  2002.03.10  Phase 7.9.0   Philedelphia�¸�
//                            kawari_engine_base.h�ѻ�
//  2002.03.20                SAORI����
//  2002.04.12  Phase 8.0.0   ���󥿥ץ꥿������ѥ����ľ��
//                            ���󥿡��ե������ڤ�ľ��
//  2002.12.30  Phase 8.1.0   Winter Comicket Version
//                            ʣ�����󥹥��󥹤����(����Ū��)
//
//
//---------------------------------------------------------------------------
#ifndef KAWARI_SHIORI_H
#define KAWARI_SHIORI_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
#include "libkawari/kawari_engine.h"
#include "misc/phttp.h"
#include "libkawari/kawari_version.h"
//---------------------------------------------------------------------------
#define SHIORIVER	"3.0"
#define SAORIVER	"1.0"
//---------------------------------------------------------------------------
// ����ʳ��β�������AI���ͥ��󥿡��ե�����
// �� TNS_KawariANI
class TKawariShioriAdapter{
private:

	// ���������󥸥�
	TKawariEngine Engine;

	// �����ѤΥ�������̾
	// NOTIFY�������Ƥ���̾��
	std::string GhostName;

	// �������ƥ���٥�
	class TSecurityLevel {
	private:
		unsigned int Level;
	public:
		// ��ã��ϩ����
		enum TSenderPath {
			tsidSystem=0,
			tsidLocal=1,
			tsidUnknown=2,
			tsidExternal=3
		};

		// �������ƥ���٥�����ꤹ��
		// 3 : LOCAL����Ĥ���
		// 2 : LOCAL, UNKNOWN����Ĥ���
		// 1 : LOCAL, UNKNOWN����Ĥ���(2��Ʊ��)
		// 0 : LOCAL, UNKNOWN, EXTERNAL����Ĥ���
		bool Set(unsigned int lv)
		{
			Level=(lv<=3)?lv:Level;
			return(Level==lv);
		}

		unsigned int Get(void)
		{
			return(Level);
		}

		// Sender�����Ĥ���Ƥ��뤫�ɤ��������å�����
		bool Check(TSenderPath path)
		{
			switch(Level) {
			case 3: return(path==tsidLocal);
			case 2: return(path<=tsidUnknown);
			case 1: return(path<=tsidUnknown);
			case 0: return(path<=tsidExternal);
			default: return(path<=tsidUnknown);
			}
		}

		TSecurityLevel(unsigned int lv)
		{
			Set(lv);
		}
	} SecurityLevel;


	// Sender����ã��ϩ�����ʬΥ
	void GetSenderPath(const std::string &senderstr,
	 TSecurityLevel::TSenderPath &sender_path,std::string &sender_path_name);

	// ���ꤷ������ȥ��ñ������ƸƤӽФ�
	std::string EnumExec(const std::string& key);

	// ������Υ쥹�ݥ󥹤������ɤ߽Ф�
	int GetResponse(TPHMessage &response);

	// Load���ƤФ줿��
	bool initialized;

public:
	// ʣ�����󥹥��󥹤����
	TKawariShioriAdapter(void) : SecurityLevel(2), initialized(false) { }

	virtual ~TKawariShioriAdapter() {}

	// SHIORI/2.x API

	// DLL���ɻ��˸ƤФ��
	// const std::string& datapath : DLL�Υǥ��쥯�ȥ�ѥ�
	// ����� bool : ������true
	bool Load(const std::string& datapath);

	// DLL������ɻ��˸ƤФ��
	// ����� bool : ������true
	bool Unload(void);

	// ��AI�ꥯ������
	// TPHMessage &request : �ꥯ�����ȥ�å�����
	// TPHMessage &response : �쥹�ݥ󥹥�å�����
	void Request(TPHMessage &request, TPHMessage &response);

	// SHIORI for POSIX 2.4

	// ��AI�⥸�塼��ΥС�������ֹ���֤�
	// ����� std::string : "����̾��[.���̾��[.���̾��]]/�С�������ֹ�"
	// ex. "KAWARI.meister/7.9.0" Meister�� ������ Phase7.9.0
	static inline std::string GetModuleVersion(void)
	{
		return(KAWARI_NAME "/" KAWARI_MAJOR "." KAWARI_MINOR "." KAWARI_SUBMINOR);
	}
};
//---------------------------------------------------------------------------
class TKawariShioriFactory{
private:
	// Singleton
	static TKawariShioriFactory *instance;
	TKawariShioriFactory (void) {}

	// �٤Υꥹ��
	std::vector<TKawariShioriAdapter *> list;

public:
	// �ե����ȥ�γ���
	static TKawariShioriFactory &GetFactory(void){
		if (!instance)
			instance=new TKawariShioriFactory();
		return (*instance);
	}
	// �ե����ȥ���˴�
	// �˴���˺��������ǽ
	static void DisposeFactory(void){
		// delete NULL is safe.
		delete instance;
		instance=NULL;
	}

	// �ǥ��ȥ饯��
	~TKawariShioriFactory();

	// �⥸�塼��С������
	static inline std::string GetModuleVersion(void){
		return TKawariShioriAdapter::GetModuleVersion();
	}
	// �������󥹥��󥹤κ���
	// ����� : �ϥ�ɥ�
	unsigned int CreateInstance(const std::string &datapath);
	// ���󥹥��󥹤κ��
	bool DisposeInstance(unsigned int handle);
	// ���󥹥��󥹤ؤΥꥯ������
	std::string RequestInstance(unsigned int handle, const std::string &reqstr);
};
//---------------------------------------------------------------------------
#endif // KAWARI_SHIORI_H
//---------------------------------------------------------------------------
