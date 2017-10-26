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
	int label;//+1 or -1
	int label_from_predict;
};
struct Weight
{
	vector<double> w;
	double Accuracy, Recall, Precision, F1;
};
vector<Data> train;//训练集数据
vector<Data> test;//验证集/测试集数据
vector<double> w0;//权重向量
double threshold;//阈值
int iterate_times;
Weight best_sol;
void HandleData(const string data_name)
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//用于存储原始文本
	char ch; string str;
	while ((ch = inFile.get()) != EOF)
	{
		if (ch == '\n')
		{
			data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);
	for (int i = 0; i<data.size(); i++)
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
			else
			{
				tmp.data.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if (str.size() != 0) tmp.label = atof(str.c_str());
		if (data_name == "train.csv") train.push_back(tmp);
		else  if (data_name == "val.csv" || data_name == "test.csv") test.push_back(tmp);
	}
}
void Init()
{
	for (int i = 0; i < train[0].data.size(); i++) w0.push_back(1);
	threshold = 0;
	best_sol.Accuracy = 0;
	best_sol.F1 = 0;
	best_sol.Precision = 0;
	best_sol.Recall = 0;
	iterate_times = 5000;
}
int calc(const vector<double> v1, const vector<double> v2)
{
	double sum = 0;
	for (int i = 0; i < v1.size(); i++) sum += v1[i] * v2[i];
	if (sum > threshold) return 1;
	else return -1;
}
double Evaluate(Weight tmp)
{
	return 8*tmp.Accuracy+10*tmp.F1+5*tmp.Precision+5*tmp.Recall;
}
void Get_w0(int times)
{
	for (int i = 0; i < times; i++)
	{
		Weight tmp;
		for (int j = 0; j < train.size(); j++)
		{
			if (calc(train[j].data, w0) != train[j].label)//预测错误，更新w0
			{
				for (int k = 0; k < train[j].data.size(); k++) w0[k] = w0[k] + train[j].data[k] * train[j].label;
			}
		}
		tmp.w = w0;
		int TP = 0, FN = 0, TN = 0, FP = 0;
		for (int i = 0; i < train.size(); i++)
		{
			train[i].label_from_predict = calc(train[i].data, w0);
			if (train[i].label == 1 && train[i].label_from_predict == 1) TP++;
			else if (train[i].label == 1 && train[i].label_from_predict == -1) FN++;
			else if (train[i].label == -1 && train[i].label_from_predict == -1) TN++;
			else if (train[i].label == -1 && train[i].label_from_predict == 1) FP++;
		}
		tmp.Accuracy = (TP + TN)*1.0 / (TP + FP + TN + FN);
		tmp.Recall = TP*1.0 / (TP + FN);
		if (TP + FP == 0) tmp.Precision = 9999;
		else tmp.Precision = TP*1.0 / (TP + FP);
		tmp.F1 = (2.0 * tmp.Precision * tmp.Recall) / (tmp.Precision + tmp.Recall);
		if(Evaluate(best_sol)<Evaluate(tmp)) best_sol=tmp;
	}
}
int main()
{
	HandleData("train.csv");
	HandleData("test.csv");
	ofstream out;
	out.open("153527_zhangzihao_PLA.csv");
	Init();
	Get_w0(iterate_times);
	for (int i = 0; i < test.size(); i++)
	{
		test[i].label_from_predict = calc(test[i].data, best_sol.w);
		out<<test[i].label_from_predict<<endl;
	}
	out.close();
}


