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
	int num;//��� 
	vector<string> words;//�ı� 
	string mood;//��Ӧ���� 
	string mood_from_train;
	vector<double> tf_idf;
};
vector<Train_set> train;//ѵ����
vector<Train_set> verification;//��֤��
vector<string> not_repeat_words;//�ı��в��ظ�����
set<string> all_mood;//�������
map<string,int> num_of_mood;//���-������µĵ�������
map<string,double> priori_probability;//������е��������
void HandleData(const string data_name)//��������
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//�洢ԭʼ�ı�
	char ch;
	string str;
	bool flag=false;//�����ж��Ƿ��ǵ�һ������ 
	while((ch=inFile.get())!=EOF)
	{
		if(ch=='\n')
		{
			if(!flag) flag=true;//��ʾ��ǰ�ǵ�һ������
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
		str=tmp.words[tmp.words.size()-1];//�����һ��������ȡ���ı������ֵ 
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
		else if(data_name=="validation_set.csv") verification.push_back(tmp);
	} 
}
void Get_num_of_mood()//��num_of_mood����������,�����-������µĵ�������
{
	for(int i=0;i<train.size();i++)
		for(int j=0;j<train[i].words.size();j++) num_of_mood[train[i].mood]++;
}
map<string,int> flag;
void Get_not_repeat_words()//������в��ظ�����
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
int Get_num_of_times_word_occurs(string mood,string word)//���c�µ���t�ڸ����ĵ��г��ֹ��Ĵ���֮��
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
void Get_all_mood()//�������������࣬������
{
	for(int i=0;i<train.size();i++) all_mood.insert(train[i].mood);
}
void Get_priori_probability()//��������������
{
	map<string,int> tmp;
	for(int i=0;i<train.size();i++) tmp[train[i].mood]++;
	for(map<string,int>::iterator it=tmp.begin();it!=tmp.end();it++)
		priori_probability[it->first]=(it->second*1.0)/train.size();
}
void Predict()
{
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
			continue;
		}
		map<string,double> probability;
		//�������ı�����ÿ����еĸ���
		for(set<string>::iterator it=all_mood.begin();it!=all_mood.end();it++)
		{
			for(int j=0;j<tmp.size();j++)
			{
				if(!probability[*it]) probability[*it]=(Get_num_of_times_word_occurs(*it,tmp[j])+0.37)/(num_of_mood[*it]+0.37*not_repeat_words.size());
				else probability[*it]*=(Get_num_of_times_word_occurs(*it,tmp[j])+0.37)/(num_of_mood[*it]+0.37*not_repeat_words.size());
			}
			probability[*it]*=priori_probability[*it];
		}
		double max_ret=0;
		for(map<string,double>::iterator it=probability.begin();it!=probability.end();it++)
		{
			if(it->second>max_ret)
			{
				max_ret=it->second;
				ret=it->first;
			}
		}
		if(ret==verification[i].mood) true_num++;
	}
	cout<<"rate: "<<(double)true_num/verification.size()<<endl;
}
int main()
{
	ios_base::sync_with_stdio(false);
	HandleData("train_set.csv");
	HandleData("validation_set.csv");
	Get_num_of_mood();//��num_of_mood����������,�����-������µĵ�������
	Get_not_repeat_words();
	Get_all_mood();//�������������࣬������
	Get_priori_probability();
	Predict();
}
