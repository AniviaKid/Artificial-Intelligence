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
struct Point
{
	int num;//Ŀ���ı����
	double dist;//Ŀ���ı�����
	string mood;//Ŀ���ı����
};
struct Train_set
{
	int num;//��� 
	vector<string> words;//�ı� 
	string mood;//��Ӧ���� 
	string mood_from_train;
	vector<double> tf_idf;
	vector<Point> distance;
};
bool cmp(Point a,Point b)
{
	return a.dist>b.dist;//��Ϊ���õľ������������ƶȣ����Ӧ��ȡ�ϴ���
}
vector<Train_set> train;//ѵ����
vector<Train_set> verification;//��֤��
vector<string> not_repeating_words;//���ظ�����,����ѵ��������֤��
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
		else if(data_name=="test_set.csv") verification.push_back(tmp);
	} 
}
map<string,int> exist;
map<string,double> IDF;
void Get_tf_idf()
{
	map<string,int> flag;
	for(int i=0;i<train.size();i++)//��ò��ظ��ʻ㣬ͳ�ƴ�����ִ���(exist)
	{
		flag.clear();
		for(int j=0;j<train[i].words.size();j++)
		{
			if(!exist[train[i].words[j]])
			{
				not_repeating_words.push_back(train[i].words[j]);
				exist[train[i].words[j]]++;
				flag[train[i].words[j]]++;
			}
			else
			{
				if(!flag[train[i].words[j]])
				{
					exist[train[i].words[j]]++;
					flag[train[i].words[j]]++;
				}
			}
		}
	}
	int size=train.size();
	for(int i=0;i<not_repeating_words.size();i++) IDF[not_repeating_words[i]]=log((double)size/(1.0+exist[not_repeating_words[i]]));//���IDF����
	for(int i=0;i<train.size();i++)//���ѵ������TF-IDF
	{
		map<string,int> tmp;
		for(int j=0;j<train[i].words.size();j++) tmp[train[i].words[j]]++;
		for(int j=0;j<not_repeating_words.size();j++)
		{
			if(!tmp[not_repeating_words[j]])
			{
				train[i].tf_idf.push_back(0);
			}
			else
			{
				train[i].tf_idf.push_back((double)(tmp[not_repeating_words[j]])/(train[i].words.size())*IDF[not_repeating_words[j]]);
			}
		}
	}
	for(int i=0;i<verification.size();i++)
	{
		map<string,int> tmp;
		for(int j=0;j<verification[i].words.size();j++) tmp[verification[i].words[j]]++;
		for(int j=0;j<not_repeating_words.size();j++)
		{
			if(!tmp[not_repeating_words[j]])
			{
				verification[i].tf_idf.push_back(0);
			}
			else
			{
				verification[i].tf_idf.push_back((double)(tmp[not_repeating_words[j]])/(train[i].words.size())*IDF[not_repeating_words[j]]);
			}
		}
	}
}
double Get_distance_of_two_vector(vector<double> a,vector<double> b,int select)
{
	if(select==1)//�����پ���
	{
		double sum=0;
		for(int i=0;i<a.size();i++) sum+=fabs((a[i]-b[i]));
		return sum;
	}
	else if(select==2)//ŷʽ����
	{
		double sum=0;
		for(int i=0;i<a.size();i++) sum+=(a[i]-b[i])*(a[i]-b[i]);
		return sqrt(sum);
	}
	else if(select==3)//�������ƶ�;Ϊ����ֵԽ������ԽС;Ϊ���������ϴ�,�����ľ���ֵԽ������Խ��
	{
		double dot=0;
		for(int i=0;i<a.size();i++) dot+=a[i]*b[i];
		double a_=0,b_=0;
		for(int i=0;i<a.size();i++)
		{
			a_+=a[i]*a[i];
			b_+=b[i]*b[i];
		}
		if(a_==0||b_==0) return 999999;//ĳһ����Ϊ�����������������������κ����ƿ���
		return dot/sqrt(a_*b_);
	}
}
void Get_distance()
{
	for(int i=0;i<verification.size();i++)//������֤��
		for(int j=0;j<train.size();j++)
		{
			Point tmp;
			tmp.num=train[j].num;
			tmp.dist=Get_distance_of_two_vector(verification[i].tf_idf,train[j].tf_idf,3);
			tmp.mood=train[j].mood;
			verification[i].distance.push_back(tmp);
		}
}
int main()
{
	ios_base::sync_with_stdio(false);
	HandleData("train_set.csv");
	HandleData("test_set.csv");
	Get_tf_idf();//���tf-idf����
	Get_distance();
	ofstream out;
	out.open("ret_kNN_classification.txt");
	int k=11;
	//for(int k=1;k<=50;k++)
	//{
		int true_num=0;//������ȷ������
		for(int i=0;i<verification.size();i++)
		{
			map<string,double> probability_mood;
			stable_sort(verification[i].distance.begin(),verification[i].distance.end(),cmp);
			for(int j=0;j<k;j++) probability_mood[verification[i].distance[j].mood]+=verification[i].distance[j].dist;
			string ret_mood;
			double max_ret=0;
			for(map<string,double>::iterator it=probability_mood.begin();it!=probability_mood.end();it++)
			{
				if(it->second>max_ret)
				{
					max_ret=it->second;
					ret_mood=it->first;
				}
			}
			verification[i].mood_from_train=ret_mood;
			out<<i+1<<" "<<ret_mood<<endl;
			if(ret_mood==verification[i].mood) true_num++;
		}
		//cout<<"k="<<k<<"  "<<"rate:"<<(double)true_num/verification.size()<<endl;
	//}
}
