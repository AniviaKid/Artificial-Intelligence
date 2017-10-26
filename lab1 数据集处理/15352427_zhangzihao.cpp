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
	int row;//��
	int col;//��
	int num;//��ֵ����
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
	while((ch=inFile.get())!=EOF)//��ȡ����
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
	cout<<"��ȡԭʼ�������."<<endl;
	for(vector<string>::iterator it=data.begin();it!=data.end();it++)//����ԭʼ�ı���ÿһ��
	{
		//Ѱ�Ҹ��дʻ������
		int pos_begin;
		for(int i=0;i<(*it).size();i++)
		{
			if((*it)[i]>='0'&&(*it)[i]<='9'&&((*it)[i+1]=='\t'||(*it)[i+1]==' '))//���ֺ��ǿո��\t
			{
				pos_begin=i+2;
				i++;
				continue;
			}
		} 
		(*it)=(*it).substr(pos_begin);
	}
	vector< vector<string> > words;//ѵ���ı� 
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
	map<string,int> exist;//��¼ÿ��������ִ���,ͬһ���ı����ֶ����һ�� 
	vector<string> not_repeating_words;
	map<string,int> flag;
	for(vector< vector<string> >::iterator it=words.begin();it!=words.end();it++)//���������ı� 
	{
		flag.clear();
		for(vector<string>::iterator k=(*it).begin();k!=(*it).end();k++)//������ǰ���ѵ���ı� 
		{
			if(!exist[*k])//����û������� 
			{
				not_repeating_words.push_back(*k);
				exist[*k]++;
				flag[*k]++;
			}
			else//����������� 
			{
				if(!flag[*k])//����ı����һ�γ��������
				{
					exist[*k]++;
					flag[*k]++;
				}
			} 
		}
	}
	map<string,double> IDF;//IDF��������ĸΪ���ִ���
	map<string,double> IDF_;//IDF��������ĸΪ���ִ���+1
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
	Sparse_Matrix table;//ϡ�����
	table.row=words.size();
	table.col=not_repeating_words.size();
	for(vector< vector<string> >::iterator it=words.begin();it!=words.end();it++)//����ÿ��ѵ���ı�
	{
		map<string,int> tmp;
		for(vector<string>::iterator k=(*it).begin();k!=(*it).end();k++)//�������ѵ���ı�
			tmp[*k]++;
		//�������в��ظ�����
		for(vector<string>::iterator k=not_repeating_words.begin();k!=not_repeating_words.end();k++)
		{
			if(!tmp[*k])//���ѵ���ı��ﲻ���������
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
	cout<<"ͳ�ơ�дֵ���."<<endl;
	//for(vector<string>::iterator it=not_repeating_words.begin();it!=not_repeating_words.end();it++) cout<<*it<<endl;
}
