#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<map> 
#include<sstream>
#include<iomanip> 
#include<cmath>
#include<algorithm> 
using namespace std;
struct Train_set
{
	int num;//编号 
	vector<string> words;//文本 
	double mood[6];
	double mood_from_train[6];
	vector<double> tf;
	map<string,double> laplace;
};
vector<Train_set> train;//训练集
vector<Train_set> verification;//验证集
vector<string> not_repeat_words;//不重复单词
string category[6]={"anger","disgust","fear","joy","sad","surprise"};
void HandleData(const string data_name)//处理数据
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//存储原始文本
	char ch;
	string str;
	bool flag=false;//用于判断是否是第一行数据 
	while((ch=inFile.get())!=EOF)
	{
		if(ch=='\n')
		{
			if(!flag) flag=true;//表示当前是第一行数据
			else data.push_back(str);
			str.clear();
			continue;
		}
		str+=ch;
	}
	inFile.close();
	for(int i=0;i<data.size();i++)
	{
		Train_set tmp;
		tmp.num=i+1;
		istringstream stream(data[i]);
		while(stream>>str) tmp.words.push_back(str);
		str=tmp.words[tmp.words.size()-1];//从最后一个词组中取出文本和情感值概率
		tmp.words.pop_back();
		string str1;
		for(int j=0;j<str.size();j++)//取出最后一个文本
		{
			if(str[j]==',')
			{
				str=str.substr(j+1);
				break;
			}
			str1+=str[j];
		}
		tmp.words.push_back(str1);
		//取概率
		str1.clear();
		int k=0;//当前取的是第几个概率
		for(int j=0;j<str.size();j++)
		{
			if(str[j]==',')
			{
				tmp.mood[k]=atof(str1.c_str());
				k++;
				str1.clear();
				continue;
			}
			str1+=str[j];
		}
		tmp.mood[k]=atof(str1.c_str());
		if(data_name=="train_set.csv") train.push_back(tmp);
		else if(data_name=="validation_set.csv") verification.push_back(tmp);
	} 
}
map<string,int> flag;
void Get_not_repeat_words()
{
	for(int i=0;i<train.size();i++)
	{
		for(int j=0;j<train[i].words.size();j++)
		{
			if(!flag[train[i].words[j]])
			{
				not_repeat_words.push_back(train[i].words[j]);
				flag[train[i].words[j]]++;
			}
		}
	}
}
void Get_tf()
{
	for(int i=0;i<train.size();i++)
	{
		map<string,int> tmp;
		for(int j=0;j<train[i].words.size();j++) tmp[train[i].words[j]]++;
		for(int j=0;j<not_repeat_words.size();j++)
		{
			if(!tmp[not_repeat_words[j]])
			{
				train[i].tf.push_back(1.0/not_repeat_words.size());
				train[i].laplace[not_repeat_words[j]]=1.0/not_repeat_words.size();
			}
			else
			{
				double numerator,denominator;
				numerator=(double)tmp[not_repeat_words[j]]+0.0001;
				denominator=train[i].words.size()+0.0001*not_repeat_words.size();
				train[i].tf.push_back(numerator/denominator);
				train[i].laplace[not_repeat_words[j]]=numerator/denominator;
			}
		}
	}
}
void Predict()
{
	ofstream out;
	out.open("rate.txt");
	for(int i=0;i<verification.size();i++)
	{
		double ret[6]={0,0,0,0,0,0};
		//对这行文本，进行回归计算
		vector<string> tmp=verification[i].words;
		for(vector<string>::iterator it=tmp.begin();it!=tmp.end();it++)
		{
			if(!flag[*it])
			{
				tmp.erase(it);
				it--;
			}
		}
		if(tmp.size()==0)
		{
			for(int j=0;j<6;j++) ret[j]=1.0/6;
			for(int j=0;j<6;j++) out<<ret[j]<<"\t";
			out<<endl; 
			continue;
		}
		for(int c=0;c<6;c++)//遍历每种情感
		{
			for(int j=0;j<train.size();j++)//当前情感下遍历训练集所有文本
			{
				double tmp1=train[j].mood[c];
				//遍历这个文本下每个训练集出现过的单词
				for(int k=0;k<tmp.size();k++)
				{
					tmp1*=train[j].laplace[tmp[k]];
				} 
				ret[c]+=tmp1;
			}
		}
		double sum=0;
		for(int j=0;j<6;j++) sum+=ret[j];
		if(sum!=0) for(int j=0;j<6;j++) ret[j]/=sum;
		for(int j=0;j<6;j++) out<<ret[j]<<"\t";
		out<<endl;
	}
	out.close();
}
int main()
{
	ios_base::sync_with_stdio(false);
	HandleData("train_set.csv");
	HandleData("validation_set.csv");
	Get_not_repeat_words();
	Get_tf();
	Predict();
}
