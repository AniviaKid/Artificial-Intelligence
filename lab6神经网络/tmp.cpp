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
ofstream out;
ofstream out2; 
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
		tmp.label=(atof(str.c_str()));
		for(int j=0;j<tmp.data.size();j++){
			if(j!=1&&j!=2&&j!=4) out<<tmp.data[j]<<" ";
		}
		out<<"\n";
		out2<<tmp.label<<"\n";
	}
}
int main()
{
	out.open("data.txt");
	out2.open("cnt.txt");
	HandleData("train.csv");
}