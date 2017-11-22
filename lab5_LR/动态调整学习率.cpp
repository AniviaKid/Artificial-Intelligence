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
double step;
const double e = 2.718281828459;
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

	for (int i = 0; i<data.size(); i++) //处理原始数据
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
		if (data_name == "train.csv") {
			if (str.size() != 0) tmp.label = atof(str.c_str());//该行数据处理结束
			if (num_train[i]) train.push_back(tmp);
			else vail.push_back(tmp);
		}
		else if (data_name == "test.csv") {
			tmp.label = 999;
			test.push_back(tmp);
		}
	}
}
void Init()
{
	for (int i = 0; i < train[0].data.size(); i++) w0.push_back(1);
	step = 0.1;
}
double calc(const vector<double> v1, const vector<double> v2)
{
	double sum = 0;
	for (int i = 0; i < v1.size(); i++) sum += v1[i] * v2[i];
	return sum;
}
void Get_w(int num_iterations) {
	for (int i = 0; i < num_iterations; i++) { //迭代次数
		vector<double> s;//权重分数，每次迭代更新
		for (int j = 0; j < train.size(); j++) { //遍历训练集计算权重积分
			s.push_back(calc(w0, train[j].data));
		}

		vector<double> gradient;//每一维的梯度
		for (int j = 0; j < w0.size(); j++) { //遍历每一维
			double grad = 0;
			for (int k = 0; k < train.size(); k++) { //在这一维上遍历训练集
				step = 4 / (1 + i +k) + 0.1;
				grad += ((1.0 / (1 + pow(e, -s[k]))) - train[k].label)*train[k].data[j]*step;
			}
			gradient.push_back(grad);
		}

		bool flag = true;
		for (int j = 0; j < gradient.size(); j++) { //确认梯度是否为0
			if (gradient[j] >=1e-5) {
				flag = false;
				break;
			}
		}
		if (flag) { ////梯度为0则跳出
			cout << "迭代次数" << i + 1 << "已收敛" << endl;
			break;
		}

		for (int j = 0; j < w0.size(); j++) { //更新w0
			w0[j] -= (gradient[j]);
		}
	}
}
void Evaluation_of_w0(vector<Data> v) {
	int TP = 0, FN = 0, TN = 0, FP = 0;
	for (int i = 0; i < v.size(); i++) {
		if (calc(w0, v[i].data) > 0) v[i].label_from_predict = 1;
		else v[i].label_from_predict = 0;
		if (v[i].label == 1 && v[i].label_from_predict == 1) TP++;
		else if (v[i].label == 1 && v[i].label_from_predict == 0) FN++;
		else if (v[i].label == 0 && v[i].label_from_predict == 0) TN++;
		else if (v[i].label == 0 && v[i].label_from_predict == 1) FP++;
	}
	Accuracy = (TP + TN)*1.0 / (TP + FP + TN + FN);
	Recall = TP*1.0 / (TP + FN);
	Precision = TP*1.0 / (TP + FP);
	F1 = (2.0 * Precision*Recall) / (Precision + Recall);
	cout << "TP:" << TP << endl;
	cout << "FN:" << FN << endl;
	cout << "TN:" << TN << endl;
	cout << "FP:" << FP << endl;
	cout << "Accuracy:" << Accuracy << "   Recall:" << Recall << endl;
	cout << "Precision:" << Precision << "   F1:" << F1 << endl;
}
void print_dataset(vector<Data> v,string name) {
	ofstream out;
	out.open(name.c_str());
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].data.size(); j++) {
			out << v[i].data[j] << " ";
		}
		out << v[i].label << endl;
	}
	out.close();
}
int main()
{
	srand(time(NULL));
	HandleData("train.csv");
	HandleData("test.csv");
	Init();
	Get_w(3);
	Evaluation_of_w0(train);
	system("pause");
}

