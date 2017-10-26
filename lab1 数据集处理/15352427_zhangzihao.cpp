#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<map> 
#include<sstream>
#include<iomanip> 
#include<cmath>
using namespace std;
class Sparse_Matrix
{
public:
	int row;//行
	int col;//列
	int num;//数值个数
	vector< vector<int> > sequence;
	Sparse_Matrix()
	{
		
	}
}; 
int main()
{
	ios_base::sync_with_stdio(false);
	ifstream inFile;
	inFile.open("text.txt");
	vector<string> data;
	char ch;
	string str;
	while((ch=inFile.get())!=EOF)//读取数据
	{
		if(ch=='\n')
		{
			data.push_back(str);
			str.clear();
			continue;
		}
		str+=ch;
	}
	inFile.close();
	cout<<"读取原始数据完毕."<<endl;
	for(vector<string>::iterator it=data.begin();it!=data.end();it++)//遍历原始文本的每一行
	{
		//寻找该行词汇表的起点
		int pos_begin;
		for(int i=0;i<(*it).size();i++)
		{
			if((*it)[i]>='0'&&(*it)[i]<='9'&&((*it)[i+1]=='\t'||(*it)[i+1]==' '))//数字后是空格或\t
			{
				pos_begin=i+2;
				i++;
				continue;
			}
		} 
		(*it)=(*it).substr(pos_begin);
	}
	vector< vector<string> > words;//训练文本 
	for(vector<string>::iterator it=data.begin();it!=data.end();it++)
	{
		vector<string> tmp;
		istringstream stream(*it);
	 	while(stream>>str) tmp.push_back(str);
	 	words.push_back(tmp);
	}
	/*for(vector< vector<string> >::iterator it=words.begin();it!=words.end();it++)
	{
		for(vector<string>::iterator k=(*it).begin();k!=(*it).end();k++)
			cout<<*k<<" ";
		cout<<endl;
	}*/
	map<string,int> exist;//记录每个词语出现次数,同一个文本出现多次算一次 
	vector<string> not_repeating_words;
	map<string,int> flag;
	for(vector< vector<string> >::iterator it=words.begin();it!=words.end();it++)//遍历所有文本 
	{
		flag.clear();
		for(vector<string>::iterator k=(*it).begin();k!=(*it).end();k++)//遍历当前这个训练文本 
		{
			if(!exist[*k])//表里没有这个词 
			{
				not_repeating_words.push_back(*k);
				exist[*k]++;
				flag[*k]++;
			}
			else//表里有这个词 
			{
				if(!flag[*k])//这个文本里第一次出现这个词
				{
					exist[*k]++;
					flag[*k]++;
				}
			} 
		}
	}
	map<string,double> IDF;//IDF向量，分母为出现次数
	map<string,double> IDF_;//IDF向量，分母为出现次数+1
	int size=words.size();
	for(vector<string>::iterator k=not_repeating_words.begin();k!=not_repeating_words.end();k++)
	{
		IDF[*k]=log((double)size/exist[*k]);
		IDF_[*k]=log((double)size/(1.0+exist[*k]));
	}
	/*for(map<string,double>::iterator it=IDF_.begin();it!=IDF_.end();it++)
		cout<<it->first<<" "<<it->second<<endl;
	return 0;*/
	ofstream outFile1,outFile2,outFile3,outFile4;
	outFile1.open("one-hot.txt");
	outFile2.open("TF.txt");
	outFile3.open("TF-IDF.txt");
	outFile4.open("TF-IDF_plus1.txt");
	Sparse_Matrix table;//稀疏矩阵
	table.row=words.size();
	table.col=not_repeating_words.size();
	for(vector< vector<string> >::iterator it=words.begin();it!=words.end();it++)//遍历每个训练文本
	{
		map<string,int> tmp;
		for(vector<string>::iterator k=(*it).begin();k!=(*it).end();k++)//遍历这个训练文本
			tmp[*k]++;
		//遍历所有不重复单词
		for(vector<string>::iterator k=not_repeating_words.begin();k!=not_repeating_words.end();k++)
		{
			if(!tmp[*k])//这个训练文本里不存在这个词
			{
				outFile1<<setw(8)<<0;
				outFile2<<setw(12)<<0;
				outFile3<<setw(12)<<0;
				outFile4<<setw(12)<<0;
			}
			else
			{
				vector<int> a;
				a.push_back((int)(it-words.begin()));
				a.push_back((int)(k-not_repeating_words.begin()));
				a.push_back(1);
				table.sequence.push_back(a);
				outFile1<<setw(8)<<1;
				outFile2<<setw(12)<<(double)(tmp[*k])/(*it).size();
				outFile3<<setw(12)<<((double)(tmp[*k])/(*it).size())*IDF[*k];
				outFile4<<setw(12)<<((double)(tmp[*k])/(*it).size())*IDF_[*k];
			}
		}
		outFile1<<'\n';
		outFile2<<'\n';
		outFile3<<'\n';
		outFile4<<'\n';
	}
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	table.num=table.sequence.size();
	ofstream outFile5;
	outFile5.open("smatrix.txt");
	outFile5<<table.row<<"\n"<<table.col<<"\n"<<table.num<<"\n";
	for(int i=0;i<table.sequence.size();i++)
	{
		outFile5<<table.sequence[i][0];
		outFile5<<setw(5)<<table.sequence[i][1];
		outFile5<<setw(5)<<table.sequence[i][2];
		outFile5<<"\n";
	}
	outFile5.close();
	cout<<"统计、写值完毕."<<endl;
	//for(vector<string>::iterator it=not_repeating_words.begin();it!=not_repeating_words.end();it++) cout<<*it<<endl;
}
