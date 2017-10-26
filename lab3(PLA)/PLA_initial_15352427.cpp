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
vector<Data> train;//训练集数据
vector<Data> test;//验证集/测试集数据
vector<double> w0;//权重向量
double threshold;//阈值
int iterate_times;
double Accuracy, Recall, Precision, F1;
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
			if (data[i][j]!=',')//当前字符不是逗号
			{
				str += data[i][j];
			}
			else
			{
				tmp.data.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if(str.size()!=0) tmp.label=atof(str.c_str());
		if (data_name == "train.csv") train.push_back(tmp);
		else  if (data_name == "val.csv" || data_name == "test.csv") test.push_back(tmp);
	}
}
void Init() 
{
	for (int i = 0; i < train[0].data.size(); i++) w0.push_back(1);
	threshold = 0;
	iterate_times = 200;
}
int calc(const vector<double> v1, const vector<double> v2)
{
	double sum = 0;
	for (int i = 0; i < v1.size(); i++) sum += v1[i] * v2[i];
	if (sum > threshold) return 1;
	else return -1;
}
void Get_w0(int times)
{
	for (int i = 0; i < times; i++)
	{
		bool flag = true;
		for (int j = 0; j < train.size(); j++)
		{
			if (calc(train[j].data, w0) != train[j].label)//预测错误，更新w0
			{
				flag = false;
				for (int k = 0; k < train[j].data.size(); k++) w0[k] = w0[k] + train[j].data[k] * train[j].label;
				break;
			}
		}
		if (flag) break;
	}
}
void Evaluation_of_w0()
{
	for(int i=0;i<w0.size();i++) cout<<w0[i]<<" ";
	cout<<endl;
	int TP = 0, FN = 0, TN = 0, FP = 0;
	for (int i = 0; i < test.size(); i++)
	{
		test[i].label_from_predict = calc(test[i].data, w0);
		if (test[i].label == 1 && test[i].label_from_predict == 1) TP++;
		else if (test[i].label == 1 && test[i].label_from_predict == -1) FN++;
		else if (test[i].label == -1 && test[i].label_from_predict == -1) TN++;
		else if (test[i].label == -1 && test[i].label_from_predict == 1) FP++;
		cout<<"test"<<i+1<<": "<<test[i].label_from_predict<<endl;
	}
	/*for (int i = 0; i < train.size(); i++)
	{
		train[i].label_from_predict = calc(train[i].data, w0);
	if (train[i].label == 1 && train[i].label_from_predict == 1) TP++;
	else if (train[i].label == 1 && train[i].label_from_predict == -1) FN++;
	else if (train[i].label == -1 && train[i].label_from_predict == -1) TN++;
	else if (train[i].label == -1 && train[i].label_from_predict == 1) FP++;
	}*/
	Accuracy = (TP + TN)*1.0 / (TP + FP + TN + FN);
	Recall = TP*1.0 / (TP + FN);
	if (TP + FP == 0) Precision = 9999;
	else Precision = TP*1.0 / (TP + FP);
	F1 = (2.0 * Precision*Recall) / (Precision + Recall);
	/*cout << "TP:" << TP << endl;
	cout << "FN:" << FN << endl;
	cout << "TN:" << TN << endl;
	cout << "FP:" << FP << endl;
	cout << "Accuracy:" << Accuracy << "   Recall:" << Recall << endl;
	cout << "Precision:" << Precision << "   F1:" << F1 << endl;
	ofstream out;
	out.open("ret.txt");
	out << "iterate_times:" << iterate_times << endl; 
	out << "TP:" << TP << endl;
	out << "FN:" << FN << endl;
	out << "TN:" << TN << endl;
	out << "FP:" << FP << endl;
	out << "Accuracy:" << Accuracy << "   Recall:" << Recall << endl;
	out << "Precision:" << Precision << "   F1:" << F1 << endl;
	out<<"w0= ";
	for (int i = 0; i < w0.size(); i++) out << w0[i] << " ";*/
}
int main()
{
	HandleData("train.csv");
	HandleData("test.csv");
	Init();
	Get_w0(iterate_times);
	Evaluation_of_w0();
}

