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
	int num;//目标文本编号
	double dist;//目标文本距离
	double mood[6];//目标文本情感概率
};
struct Train_set
{
	int num;//编号 
	vector<string> words;//文本 
	double mood[6];
	double mood_from_train[6];
	//vector<int> one_hot;
	vector<double> tf_idf;
	vector<Point> distance;
};
bool cmp(Point a,Point b)
{
	return a.dist>b.dist;//因为采用的距离是余弦相似度，因此应该取较大者
}
vector<Train_set> train;//训练集
vector<Train_set> verification;//验证集
vector<string> not_repeating_words;//不重复单词,包括训练集与验证集
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
		else if(data_name=="test_set.csv") verification.push_back(tmp);
	} 
}
map<string,int> exist;
map<string,double> IDF;
void Get_tf_idf()
{
	map<string,int> flag;
	for(int i=0;i<train.size();i++)//获得不重复词汇，统计词语出现次数(exist)
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
	for(int i=0;i<not_repeating_words.size();i++) IDF[not_repeating_words[i]]=log((double)size/(1.0+exist[not_repeating_words[i]]));//获得IDF向量
	for(int i=0;i<train.size();i++)//求出训练集的TF-IDF
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
	if(select==1)//曼哈顿距离
	{
		double sum=0;
		for(int i=0;i<a.size();i++) sum+=fabs((a[i]-b[i]));
		return sum;
	}
	else if(select==2)//欧式距离
	{
		double sum=0;
		for(int i=0;i<a.size();i++) sum+=(a[i]-b[i])*(a[i]-b[i]);
		return sqrt(sum);
	}
	else if(select==3)//余弦相似度;为正且值越大则差距越小;为负则代表差距较大,负数的绝对值越大则差距越大
	{
		double dot=0;
		for(int i=0;i<a.size();i++) dot+=a[i]*b[i];
		double a_=0,b_=0;
		for(int i=0;i<a.size();i++)
		{
			a_+=a[i]*a[i];
			b_+=b[i]*b[i];
		}
		if(a_==0||b_==0) return 9999999;
		return dot/sqrt(a_*b_);
	}
}
void Get_distance()
{
	for(int i=0;i<verification.size();i++)//遍历验证集
		for(int j=0;j<train.size();j++)
		{
			Point tmp;
			tmp.num=train[j].num;
			tmp.dist=Get_distance_of_two_vector(verification[i].tf_idf,train[j].tf_idf,3);
			for(int k=0;k<6;k++) tmp.mood[k]=train[j].mood[k];
			verification[i].distance.push_back(tmp);
		}
}
int main()
{
	ios_base::sync_with_stdio(false);
	HandleData("train_set.csv");
	HandleData("test_set.csv");
	Get_tf_idf();//获得tf-idf矩阵
	Get_distance();
	int k=11;
	//for(int k=1;k<=50;k++)
	//{
		string name="rate";
		stringstream ss;
		ss<<k;
		string s1=ss.str();
		name+=s1;
		name+=".txt";
		ofstream out;
		out.open(name.c_str());
		for(int i=0;i<verification.size();i++)//对这条语句，取离他最近的k个邻居
		{
			sort(verification[i].distance.begin(),verification[i].distance.end(),cmp);
			for(int m=0;m<6;m++) verification[i].mood_from_train[m]=0;//遍历每种情感
			for(int j=0;j<k;j++)
			{
				for(int m=0;m<6;m++)
				{
					verification[i].mood_from_train[m]+=verification[i].distance[j].dist*verification[i].distance[j].mood[m];
				}
			}
			double sum=0;
			for(int m=0;m<6;m++) sum+=verification[i].mood_from_train[m];
			for(int m=0;m<6;m++)
			{
				verification[i].mood_from_train[m]/=sum;
				out<<verification[i].mood_from_train[m]<<"\t";
			}
			out<<endl;
		}
		out.close(); 
	//}
}
