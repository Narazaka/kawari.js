//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// KawariInlineScript -- ʸ������� --
//
//      Programed by 
//
//  2002.03.25  Phase 8        First version
//  2002.07.10  Phase 8.1.0    reverse�ΥХ�����
//  2004.01.04  Phase 8.2.0    indexing policy������. bug fix.
//  2005.06.21  Phase 8.2.3    gcc3.x��warning�б� (suikyo)
//  2008.01.27  Phase 8.2.6    gsub�ΥХ����� (Ϣ³����pattern��""���ִ�ϳ��)
//
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <algorithm>
#include <string>
#include <cstdlib>
#include <vector>
//---------------------------------------------------------------------------
#include "kis/kis_string.h"
#include "libkawari/kawari_engine.h"
#include "misc/misc.h"
#include "misc/l10n.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
// ����ǥå����ͤ������ͤˤ��롣��ξ�硢������������롣
// ʸ����Ĺ��0�λ�������ǥå�����0�ξ��Τ�0���֤���
// ����ǥå������ϰϤ�Ķ�������-1���֤���
//
// idx : ����ǥå���
// len : ʸ����Ĺ
static int normalizeIndex(int idx, int len){
	if(len<0) return -1;
	if(idx<0) idx+=len;
	if((!len)&&(!idx)) return idx;	// len=0, idx=0
	if((idx<0)||(idx>=len)) return -1;
	return idx;
}
//---------------------------------------------------------------------------
// substr target index [length]
// index : ��ξ�硢������������롣������ʸ����-1��������
//         ʸ������ϰϤ�Ķ�����硢����("")��
// length : ���ϰ��֤������������˿����롣
//          ��ά���줿��硢�����ޤ�
//          �Ǹ��ʸ����ۤ�����ϺǸ��ʸ���ޤ�
//          ����ͤϼ���("")
string KIS_substr::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");

	const wstring target=ctow(args[1]);
	int st=normalizeIndex(atoi(args[2].c_str()), target.length());
	int len=(args.size()>=4)?atoi(args[3].c_str()):target.length();
	if((st<0)||(len<0)) return ("");
	if((len+st)>static_cast<int>(target.length())) len=target.length()-st;
	return wtoc(target.substr(st, len));
}
//---------------------------------------------------------------------------
// length target
// target��ʸ����Ĺ���֤���
string KIS_length::Function(const vector<string>& args){
	if(!AssertArgument(args, 2)) return ("");

	return IntToString(ctow(args[1]).length());
}
//---------------------------------------------------------------------------
// target: target string
// pattern: pattern string
// idx: start index
// f: true if search forward.
// returns -1 if failed.
static int _match(const wstring &target, const wstring &pattern, int idx, bool f){
	if((!target.length())&&(!pattern.length())) return 0;
	if(!pattern.length()){
		// -(length+1) <= idx <= target.length()
		if(idx<0) idx+=target.length()+1;
		// 0 <= idx <= target.length()
		if((0<=idx)&&(idx<=static_cast<int>(target.length()))) return idx;
		else return -1;
	}else{
		idx=normalizeIndex(idx, target.length());
		if(idx<0) return -1;
	}
	unsigned int pos=(f)?
		target.find(pattern, idx):target.rfind(pattern, idx);
	if (pos!=string::npos){
		return static_cast<int>(pos);
	}else{
		return -1;
	}
}
//---------------------------------------------------------------------------
// match target pattern [index]
// index���������˸����ä�õ����
// ���index��������������롣��ά�������Ƭ��ʸ����
// index���ϰϤ�Ķ������硢����("-1")
string KIS_match::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");
	return IntToString(_match(ctow(args[1]), ctow(args[2]),
							  (args.size()>3) ? atoi(args[3].c_str()):0,
							  true));
}
//---------------------------------------------------------------------------
// rmatch target pattern [index]
// index������Ƭ�˸����ä�õ����
// ���index��������������롣��ά����ȺǸ��ʸ����
// �ϰϤ�Ķ������硢����("-1")
string KIS_rmatch::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");
	return IntToString(_match(ctow(args[1]), ctow(args[2]),
							  (args.size()>3) ? atoi(args[3].c_str()):-1,
							  false));
}
//---------------------------------------------------------------------------
// match_at target pattern [index]
// index��pattern��¸�ߤ����"1". ¸�ߤ��ʤ����"".
// ���index��������������롣��ά�������Ƭ��ʸ����
// �ϰϤ�Ķ������硢����("")
string KIS_match_at::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");

	const wstring target=ctow(args[1]);
	const wstring pattern=ctow(args[2]);
	if(!pattern.length()){
		int idx=(args.size()>3)?atoi(args[3].c_str()):0;
		if(idx<0) idx+=target.length();
		if((0<=idx)&&(idx<=static_cast<int>(target.length()))) return ("1");
		else return ("");
	}
	int idx=(args.size()>3)?
		normalizeIndex(atoi(args[3].c_str()),target.length()):0;
	if(idx<0) return ("");
	if((!target.length())&&(!pattern.length())&&(!idx)) return ("1");
	if(StringCompare<wchar_t>(target, pattern, idx, pattern.size())==0)
		return ("1");
	else
		return ("");
}
//---------------------------------------------------------------------------
// char_at target index
// ʸ�����index���ͤ��֤����ϰϤ�Ķ������硢����("")
string KIS_char_at::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");

	wstring target=ctow(args[1]);
	int idx=normalizeIndex(atoi(args[2].c_str()), target.length());
	if(idx<0) return ("");
	wstring retstr;
	retstr+=target[idx];
	return wtoc(retstr);
}
//---------------------------------------------------------------------------
// sub target pattern subst [index]
// index�ʹߤ�target��˸��Ĥ��ä��ǽ��pattern��subst���ִ���
// index��ά������Ƭ��ʸ����index���ϰϤ�Ķ������硢�ִ��ϵ����ʤ���
// pattern��""�λ���index���ؤ�ʸ���κ�¦��subst��������
// ���ξ�硢index��targetʸ����Ĺ�ޤǵ�����롣
string KIS_sub::Function(const vector<string>& args){
	if(!AssertArgument(args, 4)) return ("");

	wstring target=ctow(args[1]);
	wstring pattern=ctow(args[2]);
	wstring subst=ctow(args[3]);
	if(!pattern.length()){
		int idx=(args.size()>4)? atoi(args[4].c_str()):0;
		if(idx<0) idx+=target.length();
		if(idx<0) return (args[1]);
		if(idx<static_cast<int>(target.length()))
			return wtoc(target.replace(idx, 0, subst));
		else if (idx==static_cast<int>(target.length()))
			return wtoc(target+subst);
		else
			return (args[1]);
	}
	int pos=_match(target, pattern,
				   ((args.size()>4) ? atoi(args[4].c_str()):0), true);
	if(pos<0) return (args[1]);
	return wtoc(target.replace(pos, pattern.length(), subst));
}
//---------------------------------------------------------------------------
// rsub target pattern subst [index]
// index������target��˸��Ĥ��ä��ǽ��pattern��subst���ִ���
// index��ά����������ʸ����index���ϰϤ�Ķ������硢�ִ��ϵ����ʤ���
// pattern��""�λ���index���ؤ�ʸ���α�¦��subst������
// ���ξ�硢index��-targetʸ����Ĺ-1�ޤǵ�����롣
string KIS_rsub::Function(const vector<string>& args){
	if(!AssertArgument(args, 4)) return ("");

	wstring target=ctow(args[1]);
	wstring pattern=ctow(args[2]);
	wstring subst=ctow(args[3]);
	int pos=_match(target, pattern,
				   ((args.size()>4) ? atoi(args[4].c_str()):-1), false);
	if(pos<0) return (args[1]);
	return wtoc(target.replace(pos, pattern.length(), subst));
}
//---------------------------------------------------------------------------
// gsub target pattern subst [index]
// index�ʹߤ�target��˸��Ĥ��ä����Ƥ�pattern��subst���ִ���
// index��ά���Ϻǽ��ʸ����index���ϰϤ�Ķ������硢�ִ��ϵ����ʤ���
string KIS_gsub::Function(const vector<string>& args){
	if(!AssertArgument(args, 4)) return ("");

	wstring target=ctow(args[1]);
	wstring pattern=ctow(args[2]);
	wstring subst=ctow(args[3]);
	int tmp=(args.size()>4)?
		normalizeIndex(atoi(args[4].c_str()), target.length()):0;
	if(tmp<0) return (args[1]);
	unsigned int pos=tmp;
	if(!target.length()) return args[1];
	if((!pattern.length())&&(!subst.length())) return args[1];
	// int stride=max(subst.length(), 1);
	// int stride=max(subst.length(), static_cast<string::size_type>(1));
	int stride=subst.length();
	if(!pattern.length()) stride++;

	while (pos<target.length()){
		pos=target.find(pattern, pos);
		if(pos==string::npos) break;
		target=target.replace(pos, pattern.length(), subst);
		pos+=stride;
	}
	if(!pattern.length())
		target+=subst;
	return wtoc(target);
}
//---------------------------------------------------------------------------
// reverse target
string KIS_reverse::Function(const vector<string>& args){
	if(!AssertArgument(args, 2)) return ("");

	wstring target=ctow(args[1]);
	reverse(target.begin(), target.end());
	return wtoc(target);
}
//---------------------------------------------------------------------------
// tr target chars substchars
string KIS_tr::Function(const vector<string>& args){
	if(!AssertArgument(args, 4)) return ("");

	wstring target=ctow(args[1]);
	wstring search=ctow(args[2]);
	wstring replace=ctow(args[3]);

	unsigned int pos=0;
	unsigned int replen=replace.length();
	while (pos<target.length()){
		pos=target.find_first_of(search, pos);
		if (pos==string::npos) return wtoc(target);
		unsigned int index=search.find(target[pos]);
		if (index<replen) {
			target[pos]=replace[index];
			pos++;
		}else{
			target.erase(pos, 1);
		}
	}
	return wtoc(target);
}
//---------------------------------------------------------------------------
// compare string1 string2
string KIS_compare::Function(const vector<string>& args){
	if(!AssertArgument(args, 3)) return ("");

	wstring str1=ctow(args[1]);
	wstring str2=ctow(args[2]);

	if (str1>str2) return ("1");
	if (str1==str2) return ("0");
	return ("-1");
}
//---------------------------------------------------------------------------
