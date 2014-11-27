//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- split --
//
//      Programed by ���ȡ�/NAKAUE.T
//
//  2002.01.07  Phase 7.3     findpos���֥��ޥ���ɲ� (���ȡ�)
//                            split���ޥ���ɲ�(���ȡ�)
//  2002.03.17  Phase 7.9.0   7.9�λ��ͤ˹�碌�� (NAKAUE.T)
//                            split���ʤ�expr�ȤޤȤ���Ƥ��뤫����˶줷��Τǰ�ư
//                            split�����ѹ�
//  2002.11.20  Phase 8.1.0   split�������Ǥ��������ʤ��ʤäƤ����Х�����
//                            join�ɲ�
//  2003.11.17  Phase 8.2.0   split���̥���ڤ�ʸ���Ǥ�����Х�����
//                            split,join:���ڤ�ʸ�����ά����ȥ̥�ʸ��������
//  2005.06.21  Phase 8.2.3   �̤��bugfix + ��̣�˱�äƽ��� (suikyo)
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include "kis/kis_split.h"
//---------------------------------------------------------------------------
#include <string>
using namespace std;
//---------------------------------------------------------------------------
#include "libkawari/kawari_engine.h"
#include "misc/l10n.h"
//---------------------------------------------------------------------------
// split�μ���
class TSplitter {
public:
	// source: �ڤ�Ф�ʸ����
	// delim:  ���ڤ�ʸ����
	TSplitter(const string &source, const string &delim);

	// ���Υȡ�������ڤ�Ф�
	string Next(void);

	// �ޤ��ȡ�����¸�ߤ��뤫
	inline bool HasNext(void) { return (pos<max); }

private:
	wstring str;		// �ڤ�Ф�ʸ����
	wstring cs;		// ���ڤ�ʸ����
	wstring::size_type pos;	// ���ϳ��ϰ���
	wstring::size_type max;	// �ڤ�Ф�ʸ�����Ĺ��
};
//---------------------------------------------------------------------------
TSplitter::TSplitter(const string &s, const string &c)
{
	str=ctow(s);
	cs=ctow(c);
	pos=static_cast<wstring::size_type>(0);
	max=str.length();
}
//---------------------------------------------------------------------------
string TSplitter::Next(void)
{
	if (!HasNext())
		return "";

	unsigned int idx;	// ������Υݥ���
	string ret;
	if(cs.length()==0){
		// ���ڤ�ʸ��̵��
		ret=wtoc(str.substr(pos, 1));
		pos++;
	}else if((idx=str.find(cs, pos))!=string::npos) {
		// ȯ��
		ret=wtoc(str.substr(pos,idx-pos));	// ��ʸ�����֤뤳�Ȥ⤢��
		pos=idx+cs.length();			// ���ڤ�ʸ����ʬ�����ݥ�������
	} else {
		// �饹��
		ret=wtoc(str.substr(pos));		// ��ʸ�����֤뤳�Ȥ⤢��
		pos=max;				// �ݥ��󥿤�ʸ����Ǹ�����
	}

	return ret;
}
//---------------------------------------------------------------------------
string KIS_split::Function(const vector<string>& args){

	if(!AssertArgument(args, 3, 4)) return ("");

	TEntry entry=Engine->CreateEntry(args[1]);
	string delim=(args.size()==3)?"":args[3];

	TSplitter splitter(args[2],delim);
	while(splitter.HasNext())
		entry.Push(Engine->CreateStrWord(splitter.Next()));

	return("");
}
//---------------------------------------------------------------------------
string KIS_join::Function(const vector<string>& args)
{
	if(!AssertArgument(args, 2, 3)) return ("");

	TEntry entry=Engine->GetEntry(args[1]);
	unsigned int size=entry.Size();

	string retstr;
	string sep;
	if(args.size()==2) {
		sep="";
	} else {
		sep=args[2];
	}
	for(unsigned int i=0;i<size;i++) {
		retstr+=Engine->IndexParse(entry,i)+sep;
	}

	return(retstr.substr(0,retstr.length()-sep.length()));
}
//---------------------------------------------------------------------------
