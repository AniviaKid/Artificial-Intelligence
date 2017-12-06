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
	
	if (data_name == "train.csv") { //��ȡѵ������7:3�������ѵ����/��֤��
		int num = data.size()*0.7;
		for (int i = 0; i < num; i++) {
			int Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			while (num_train[Rand]) Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			num_train[Rand]++;
		}
	}

	for (int i = 1; i<data.size(); i++) //����ԭʼ����
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
			if (num_train[i]) { //ѵ���� 
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
