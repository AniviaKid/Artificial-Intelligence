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
#include<time.h>
using namespace std;
struct Data
{
	vector<double> data;//向量
	int label;//+1 or 0
	int label_from_predict;
};
vector<Data> train;//训练集数据
vector<Data> vail;//验证集数据
vector<Data> test;//测试集数据
vector<double> w0;//权重向量
double Accuracy, Recall, Precision, F1;
const double e = 2.718281828459;
ofstream out1;//data_train
ofstream out2; //cnt_train
ofstream out3;//data_vail
ofstream out4;//cnt_vail
ofstream out5;//test
ofstream out6;//data_dec
ofstream out7;//cnt_dec
void HandleData(const string data_name)
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//用于存储原始文本
	char ch; string str;
	map<int, int> num_train;//随机划分验证集，该map存储训练集的下标
	while ((ch = inFile.get()) != EOF)//读取原始数据
	{
		if (ch == '\n')
		{
			data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);//读取原始数据结束
	
	if (data_name == "train.csv") { //读取训练集则7:3随机划分训练集/验证集
		int num = data.size()*0.7;
		for (int i = 0; i < num; i++) {
			int Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			while (num_train[Rand]) Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			num_train[Rand]++;
		}
	}

	for (int i = 1; i<data.size(); i++) //处理原始数据
	{
		Data tmp;
		tmp.data.push_back(1);//每个样本前添加常数项1
		str.clear();
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] != ',')//当前字符不是逗号
			{
				str += data[i][j];
			}
			else if (data[i][j] == '/')
			{
				while(data[i][j] != ',') j++;
				str.clear();
			}
			else
			{
				tmp.data.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if(data.size()-i<=500&&data_name == "train.csv"){
			int label=atof(str.c_str());
			for(int j=0;j<tmp.data.size();j++){
				if(j!=1&&j!=2&&j!=4) out6<<tmp.data[j]<<" ";
			}
			out6<<"\n";
			out7<<label<<"\n";
		}
		if (data_name == "train.csv") {
			tmp.label=(atof(str.c_str()));
			if (num_train[i]) { //训练集 
				for(int j=0;j<tmp.data.size();j++){
					if(j!=1&&j!=2&&j!=4) out1<<tmp.data[j]<<" ";
				}
				out1<<"\n";
				out2<<tmp.label<<"\n";
			}
			else {
				for(int j=0;j<tmp.data.size();j++){
					if(j!=1&&j!=2&&j!=4) out3<<tmp.data[j]<<" ";
				}
				out3<<"\n";
				out4<<tmp.label<<"\n";
			}
		}
		else if (data_name == "test.csv") {
			for(int j=0;j<tmp.data.size();j++){
				if(j!=1&&j!=2&&j!=4) out5<<tmp.data[j]<<" ";
			}
			out5<<"\n";
		}
	}
}
int main()
{
	srand(time(NULL));
	out1.open("data.txt");
	out2.open("cnt.txt");
	out3.open("vail_data.txt");
	out4.open("vail_cnt.txt");
	out5.open("test.txt");
	out6.open("dec_data.txt");
	out7.open("dec_cnt.txt");
	HandleData("train.csv");
	HandleData("test.csv");
}
