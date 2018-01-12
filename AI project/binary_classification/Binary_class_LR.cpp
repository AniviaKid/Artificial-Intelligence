// Binary_class_LR.cpp: �������̨Ӧ�ó������ڵ㡣
//

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
#include<sstream>
#include<time.h>
using namespace std;
struct Data
{
	vector<double> data;//����
	int label;//+1 or 0
	int label_from_predict;
};
vector<Data> train;//ѵ��������
vector<Data> vail;//��֤������
vector<Data> test;//���Լ�����
vector<double> w0;//Ȩ������
double Accuracy, Recall, Precision, F1;
const double e = 2.718281828459;
const int num_iterations = 1000;//��������
double step = 0.1;//����
const int type = 0;//���ݶ�or����ݶ�
void Normalized(vector<Data>& v) {
	for (int i = 0; i < v[0].data.size(); i++) { //����ÿһ��
		double min = 9999, max = 0;
		for (int j = 0; j < v.size(); j++) { //����һ���ϱ���ÿһ��
			if (v[j].data[i] >= max) max = v[j].data[i];
			if (v[j].data[i] <= min) min = v[j].data[i];
		}
		if((max==0&&min==1)||(max==min)){
			
		}
		else{
			for (int j = 0; j < v.size(); j++) { //����һ���ϱ���ÿһ��
				v[j].data[i] = (v[j].data[i] - min ) / (max - min );
			}
		}
	}
}
void HandleData(const string data_name)
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//���ڴ洢ԭʼ�ı�
	char ch; string str;
	map<int, int> num_train;//���������֤������map�洢ѵ�������±�
	while ((ch = inFile.get()) != EOF)//��ȡԭʼ����
	{
		if (ch == '\n')
		{
			data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);//��ȡԭʼ���ݽ���
											 //cout<<"data.size="<<data.size()<<endl;

	if (data_name == "train.csv") { //��ȡѵ������7:3�������ѵ����/��֤��
		int num = data.size()*0.68;
		for (int i = 0; i < num; i++) {
			int Rand = rand() % data.size();
			while (num_train[Rand]) Rand = rand() % data.size();
			num_train[Rand]++;
			//cout<<i<<endl;
		}
	}
	//cout<<123<<endl;

	for (int i = 0; i<data.size(); i++) //����ԭʼ����
	{
		Data tmp;
		tmp.data.push_back(1);//ÿ������ǰ��ӳ�����1
		str.clear();
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] != ',')//��ǰ�ַ����Ƕ���
			{
				str += data[i][j];
			}
			else
			{
				if (str[str.length() - 1] >= 'A'&&str[str.length() - 1] <= 'Z') {
					int num=str[str.length() - 1]-'A'+1;//1~5
					for(int k=1;k<=5;k++){
						if(k==num) tmp.data.push_back(1);
						else tmp.data.push_back(0);
					}
				}
				else tmp.data.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if (data_name == "train.csv") {
			if (str.size() != 0) tmp.label = atof(str.c_str());//�������ݴ������
			if (num_train[i]) train.push_back(tmp);
			else vail.push_back(tmp);
		}
		else if (data_name == "test.csv") {
			tmp.data.push_back(atof(str.c_str()));
			tmp.label = 999;
			test.push_back(tmp);
		}
	}
}
void Init()
{
	//double w_tmp[train[0].data.size()]={-0.813142,-0.483693,-0.405343,0.201174,1.35966,-0.470274,0.407862,0.193191,0.0796585,0.521978,0.668514,0.484506,0.702339,0.581488,0.480573,0.224012,-0.0147228,-0.0903558};
	for (int i = 0; i < train[0].data.size(); i++) w0.push_back(rand()/(RAND_MAX+1.0));
	//for (int i = 0; i < train[0].data.size(); i++) w0.push_back(w_tmp[i]);
}
double calc(const vector<double> v1, const vector<double> v2)
{
	double sum = 0;
	for (int i = 0; i < v1.size(); i++) sum += v1[i] * v2[i];
	return sum;
}
ofstream out;
void Evaluation_of_w0(vector<Data> v) {
	int TP = 0, FN = 0, TN = 0, FP = 0;
	for (int i = 0; i < v.size(); i++) {
		if ((1/(1+pow(e,-calc(w0, v[i].data))))>=0.5) v[i].label_from_predict = 1;
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
	//out<<Accuracy<<" ";
	//if (Accuracy > 0.7) {
		out << "TP:" << TP << endl;
		out << "FN:" << FN << endl;
		out << "TN:" << TN << endl;
		out << "FP:" << FP << endl;
		out << "Accuracy:" << Accuracy << "   Recall:" << Recall << endl;
		out << "Precision:" << Precision << "   F1:" << F1 << endl;
		for (int i = 0; i < w0.size(); i++) out << w0[i] << " ";
		out << endl;
		cout << "��д��";
	//}
}
void Get_w(int num_iterations, double step, int type) { //typeΪ0Ϊ���ݶȣ�Ϊ1Ϊ����ݶ�
	for (int i = 0; i < num_iterations; i++) { //��������
		cout << "iterations:" << i << endl;
		//if(i>=500) step=0.01;
		vector<double> s;//Ȩ�ط�����ÿ�ε�������
		for (int j = 0; j < train.size(); j++) { //����ѵ��������Ȩ�ػ���
			s.push_back(calc(w0, train[j].data));
		}

		vector<double> gradient;//ÿһά���ݶ�
		if (type == 0) {
			for (int j = 0; j < w0.size(); j++) { //����ÿһά
				double grad = 0;
				for (int k = 0; k < train.size(); k++) { //����һά�ϱ���ѵ����
					grad += ((1.0 / (1 + pow(e, -s[k]))) - train[k].label)*train[k].data[j];
				}
				gradient.push_back(grad / train.size());
			}
		}
		else if (type == 1) {
			for (int j = 0; j < w0.size(); j++) { //����ÿһά
				int k = rand() % train.size();
				double grad = ((1.0 / (1 + pow(e, -s[k]))) - train[k].label)*train[k].data[j];
				gradient.push_back(grad);
			}
		}

		bool flag = true;
		for (int j = 0; j < gradient.size(); j++) { //ȷ���ݶ��Ƿ�Ϊ0
			if (gradient[j] >= 1e-12) {
				flag = false;
				break;
			}
		}
		if (flag) { ////�ݶ�Ϊ0������
			cout << "��������" << i + 1 << "������" << endl;
			break;
		}

		for (int j = 0; j < w0.size(); j++) { //����w0
			w0[j] -= (gradient[j] * step);
		}
		//Evaluation_of_w0(vail);
		/*if (type == 1) {
			cout<<"ces"<<endl;
			Evaluation_of_w0(vail);
		}*/
	}
}
ofstream out2;
void GetRes(vector<Data> v){
	for (int i = 0; i < v.size(); i++) {
		if ((1/(1+pow(e,-calc(w0, v[i].data))))>=0.5) v[i].label_from_predict = 1;
		else v[i].label_from_predict = 0;
		out2<<v[i].label_from_predict<<endl;
	}
}
int main()
{
	srand(time(NULL));
	out.open("res.txt");
	out2.open("test_res.csv");
	HandleData("train.csv");
	HandleData("test.csv");
	cout << "�������" << endl;
	Normalized(train);
	Normalized(vail);
	Normalized(test);
	/*for (int i = 0; i < test.size(); i++) {
		for (int j = 0; j < test[i].data.size(); j++) cout << test[i].data[j] << " ";
		cout << test[i].label << endl;
	}*/
	Init();
	Get_w(num_iterations, step, type);
	Evaluation_of_w0(vail);
	GetRes(test);
	system("pause");
}




