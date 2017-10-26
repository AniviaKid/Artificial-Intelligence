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
	string mood;//对应情绪 
	string mood_from_train;
	vector<double> tf_idf;
};
vector<Train_set> train;//训练集
vector<Train_set> verification;//验证集
vector<string> not_repeat_words;//文本中不重复单词
set<string> all_mood;//所有情感
map<string,int> num_of_mood;//情感-该情感下的单词总数
map<string,double> priori_probability;//所有情感的先验概率
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
		str=tmp.words[tmp.words.size()-1];//从最后一个词组中取出文本和情感值 
		tmp.words.pop_back();
		string str1,str2;
		int j=0;
		for(;j<str.size();j++)
		{
			if(str[j]!=',') str1+=str[j];
			else break;
		}
		j++;
		for(;j<str.size();j++) str2+=str[j];
		tmp.words.push_back(str1);
		tmp.mood=str2;
		if(data_name=="train_set.csv") train.push_back(tmp);
		else if(data_name=="test_set.csv") verification.push_back(tmp);
	} 
}
void Get_num_of_mood()//向num_of_mood中填入数据,即情感-该情感下的单词总数
{
	for(int i=0;i<train.size();i++)
		for(int j=0;j<train[i].words.size();j++) num_of_mood[train[i].mood]++;
}
map<string,int> flag;
void Get_not_repeat_words()//获得所有不重复单词
{
	for(int i=0;i<train.size();i++)
		for(int j=0;j<train[i].words.size();j++)
		{
			if(!flag[train[i].words[j]])
			{
				not_repeat_words.push_back(train[i].words[j]);
				flag[train[i].words[j]]++;
			}
		}
}
int Get_num_of_times_word_occurs(string mood,string word)//情感c下单词t在各个文档中出现过的次数之和
{
	int ret=0;
	for(int i=0;i<train.size();i++)
	{
		if(train[i].mood==mood)
		{
			for(int j=0;j<train[i].words.size();j++)
			{
				if(train[i].words[j]==word)
				{
					ret++;
					break;
				}
			}
		}
	}
	return ret;
}
void Get_all_mood()//获得所有情感种类，共六种
{
	for(int i=0;i<train.size();i++) all_mood.insert(train[i].mood);
}
void Get_priori_probability()//获得所有先验概率
{
	map<string,int> tmp;
	for(int i=0;i<train.size();i++) tmp[train[i].mood]++;
	for(map<string,int>::iterator it=tmp.begin();it!=tmp.end();it++)
		priori_probability[it->first]=(it->second*1.0)/train.size();
}
void Predict()
{
	/*ofstream out;
	out.open("ret_NB_classification.txt");*/
	int true_num=0;
	for(int i=0;i<verification.size();i++)
	{
		string ret;
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
			ret="joy";
			cout<<i+1<<" "<<ret<<endl;
			continue;
		}
		map<string,double> probability;
		//对这条文本，求每种情感的概率
		for(set<string>::iterator it=all_mood.begin();it!=all_mood.end();it++)
		{
			//cout<<*it<<endl;
			for(int j=0;j<tmp.size();j++)
			{
				if(!probability[*it]) probability[*it]=(Get_num_of_times_word_occurs(*it,tmp[j])+1.0)/(num_of_mood[*it]+1.0*not_repeat_words.size());
				else probability[*it]*=(Get_num_of_times_word_occurs(*it,tmp[j])+1.0)/(num_of_mood[*it]+1.0*not_repeat_words.size());
			}
			probability[*it]*=priori_probability[*it];
		}
		//cout<<probability.size()<<endl;
		double max_ret=0;
		double sum=0; 
		for(map<string,double>::iterator it=probability.begin();it!=probability.end();it++)
		{
			sum+=it->second;
			if(it->second>max_ret)
			{
				max_ret=it->second;
				ret=it->first;
			}
		}
		for(map<string,double>::iterator it=probability.begin();it!=probability.end();it++)
		{
			cout<<it->first<<":"<<it->second/sum<<endl;
		}
		cout<<"result:"<<ret<<endl;
		if(ret==verification[i].mood) true_num++;
	}
	//cout<<"rate: "<<(double)true_num/verification.size()<<endl;
}
int main()
{
	ios_base::sync_with_stdio(false);
	HandleData("train_set.csv");
	HandleData("test_set.csv");
	Get_num_of_mood();//向num_of_mood中填入数据,即情感-该情感下的单词总数
	Get_not_repeat_words();
	Get_all_mood();//获得所有情感种类，共六种
	Get_priori_probability();
	Predict();
}
