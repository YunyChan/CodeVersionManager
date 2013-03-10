#include<stdlib.h>
#include <string>

wstring s2ws(string str)
{
	wstring wstr(str.begin(),str.end());
	return wstr;
}

string ws2s(wstring wstr)
{
	string str(wstr.begin(),wstr.end());
	return str;
}

int ws2i(wstring wstr)
{
	const wchar_t *wc=wstr.c_str();
	int i=_wtoi(wc);
	return i;
}

wstring i2ws(int i)
{
	wstring wstr;
	wchar_t tmp[15];
	_itow_s(i,tmp,10);
	wstr=tmp;
	return wstr;
}

wstring wsr(wstring wstr,const wchar_t *oldC,const wchar_t *newC)
{
	wstring oldStr=oldC;
	wstring tmp=wstr.assign(wstr.begin(),wstr.end());
	wstring::size_type pos=0;
	pos=tmp.find_first_of(oldC,pos);
	while(pos!=wstring::npos)
	{
		tmp.erase(pos,oldStr.length());
		tmp.insert(pos,newC);
		pos=tmp.find_first_of(oldC,pos);
	}
	return tmp;
}

string sr(string str,const char *oldC,const char *newC)
{
	string oldStr=oldC;
	string tmp=str.assign(str.begin(),str.end());
	string::size_type pos=0;
	pos=tmp.find_first_of(oldC,pos);
	while(pos!=string::npos)
	{
		tmp.erase(pos,oldStr.length());
		tmp.insert(pos,newC);
		pos=tmp.find_first_of(oldC,pos);
	}
	return tmp;
}