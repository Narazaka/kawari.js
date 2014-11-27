//---------------------------------------------------------------------------
//
// "������" for ����ʳ��β����ʳ��β���
// �ǡ����ݻ����饹
//
//      Programed by NAKAUE.T (Meister)
//
//  2001.04.24  Phase 0.5     Ƴ��
//  2001.05.26  Phase 5.1     ���󥿡��ץ꥿������ѥ���Ƴ����ȼ��template��
//  2002.05.05  Phase 8.0.0   ���٥å����쥯�����ս��б�
//  2003.01.22  Phase 8.1.1   gcc3�ʹߤǤ�typename����
//  2005.06.21  Phase 8.2.3   gcc3.x��warning�б� (suikyo)
//
//---------------------------------------------------------------------------
#ifndef WORDCOLLECTION_H
#define WORDCOLLECTION_H
//---------------------------------------------------------------------------
#include "config.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
#ifndef FORCE_TYPENAME
#	undef typename
#endif
//---------------------------------------------------------------------------
// ��ݲ����󥿡��ե�����
template<class T,class Compare>
class IWordCollection {
public:
	virtual unsigned int Size(void) const=0;
	virtual bool Insert(const T& word,unsigned int *id_=NULL)=0;
	virtual bool Delete(unsigned int id)=0;
	virtual const T* Find(unsigned int id) const=0;
	virtual unsigned int Find(const T& word) const=0;
	virtual bool Contains(unsigned int id) const=0;
};
//---------------------------------------------------------------------------
// �ǡ������ݻ���ID���꿶�륯�饹
// ID��[1, UINT_MAX]
template<class T,class Compare>
class TWordCollection : public IWordCollection<T, Compare> {
protected:
	// ���ƤΥǡ���
	std::vector<T> WordList;

	// �ӥåȥޥå�
	// 0�����ä��줿�ǡ���
	std::vector<unsigned int> IDMap;

	// �ǡ�����ID���б�ɽ
	std::map<T,unsigned int,Compare> WordIDMap;

	// �ꥵ�������Ԥ�ID
	std::vector<unsigned int> Recycle;

public:
	virtual ~TWordCollection() {}

	inline TWordCollection(void) { IDMap.push_back(0); }

	// ��ñ��������
	unsigned int Size(void) const
	{
		return(WordIDMap.size());
	}

	// �ǡ������ɲ�
	// �����ɲä�true,ID��1���ꥸ��
	bool Insert(const T& word,unsigned int *id_=NULL);

	// �ǡ���������
	// ��������Delete���줿�ǡ����ϡ����θ�˿������ǡ�����Insert
	// �����ޤǥ�������ݻ�����³����(�Ŷ�)
	virtual bool Delete(unsigned int id);

	// ID->�ǡ���
	const T* Find(unsigned int id) const;

	// �ǡ���->ID
	unsigned int Find(const T& word) const;

	// ����ID��ޤफ
	bool Contains(unsigned int id) const;

	// �ҡ��פγ���
	void Reserve(unsigned int n);
};
//---------------------------------------------------------------------------
// �ݥ��󥿤��ݻ���ID���꿶�륯�饹
template<class T,class Compare>
class TWordPointerCollection : public TWordCollection<T*, Compare>{
public:
	// �ǡ���������
	// Delete����NULL������
	virtual bool Delete(unsigned int id){
		if(TWordCollection<T*,Compare>::Delete(id)){
            TWordCollection<T*, Compare>::WordList[id-1]=NULL;
			return true;
		}else{
			return false;
		}
	}
	virtual ~TWordPointerCollection (){
		for (typename std::vector<T*>::iterator it=TWordCollection<T*, Compare>::WordList.begin(); it < TWordCollection<T*, Compare>::WordList.end(); it++){
			if((*it))
				delete (*it);
		}
	}
};
//---------------------------------------------------------------------------
// �ǡ������ɲ�
// �����ɲä�true,ID��1���ꥸ��
template<class T,class Compare>
bool TWordCollection<T,Compare>::Insert(const T& word,unsigned int *id_)
{
	unsigned int id=Find(word);
	if(id_) *id_=id;

	if(id) return(false);

	if(Recycle.size()){
		id=Recycle.back();
		Recycle.pop_back();
		// ��Ф����ʡ�
		WordList[id-1]=word;
		WordIDMap[word]=id;
		IDMap[id]=id;
	}else{
		WordList.push_back(word);
		id=WordList.size();

		IDMap.push_back(id);
		WordIDMap[word]=id;
	}
	if(id_) *id_=id;

	return(true);
}
//---------------------------------------------------------------------------
// �ǡ���������
// ��������Delete���줿�ǡ����ϡ����θ�˿������ǡ�����Insert
// �����ޤǥ�������ݻ�����³����(�Ŷ�)
template<class T,class Compare>
bool TWordCollection<T,Compare>::Delete(unsigned int id){
	if((id==0)||(IDMap[id]==0)||(WordList.size()<=(id-1))) return false;

	IDMap[id]=0;
	Recycle.push_back(id);
	WordIDMap.erase(WordList[id-1]);

	return true;
}
//---------------------------------------------------------------------------
// ID->�ǡ���
template<class T,class Compare>
 const T* TWordCollection<T,Compare>::Find(unsigned int id) const
{
	if((id==0)||(IDMap[id]==0)||(WordList.size()<=(id-1))) return(NULL);

	return(&(WordList[id-1]));
}
//---------------------------------------------------------------------------
// �ǡ���->ID
template<class T,class Compare>
 unsigned int TWordCollection<T,Compare>::Find(const T& word) const
{
	typename std::map<T,unsigned int,Compare>::const_iterator it=WordIDMap.find(word);

	return((it!=WordIDMap.end())?it->second:0);
}
//---------------------------------------------------------------------------
// ����ID��ޤफ
template<class T,class Compare>
bool TWordCollection<T,Compare>::Contains(unsigned int id) const
{
	if((id==0)||(IDMap[id]==0)||(WordList.size()<=(id-1)))
		return(false);
	else
		return(true);
}
//---------------------------------------------------------------------------
// �ҡ��פγ���
template<class T,class Compare>
void TWordCollection<T,Compare>::Reserve(unsigned int n)
{
	WordList.reserve(n);
	IDMap.reserve(n);
	Recycle.reserve(n/2);
}
//---------------------------------------------------------------------------
class TStringP_Less {
public:
	bool operator ()(const std::string *l,const std::string *r) const
	{
		return((*l)<(*r));
	}
};
//---------------------------------------------------------------------------
#endif
