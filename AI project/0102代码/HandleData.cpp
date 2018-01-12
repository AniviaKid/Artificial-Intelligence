// HandleData.cpp: �������̨Ӧ�ó������ڵ㡣
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
#include<time.h>
using namespace std;
struct Data
{
	vector<double> data;//����
	int label;
	int label_from_predict;
};
vector<Data> train;//ѵ��������
vector<Data> vail;//��֤������
vector<Data> test;//���Լ�����

void HandleData(const string data_name)
{
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//���ڴ洢ԭʼ�ı�
	char ch; string str;
	while ((ch = inFile.get()) != EOF)
	{
		if (ch == '\n')
		{
			if(str[0]!='0') data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);//ԭʼ�ı���ȡ���
	data.erase(data.begin());
	//cout << data.size() << endl;

	/*map<int, int> index_train;
	if (data_name == "train.csv") {
		int num = data.size()*0.7;
		for (int i = 0; i < num; i++) {
			int Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			while (index_train[Rand]) Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			index_train[Rand]++;
		}
	}*/
	for (int i = 0; i<data.size(); i++)
	{
		Data tmp;
		//tmp.data.push_back(1);//ÿ������ǰ��ӳ�����1
		str.clear();
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] != ',')//��ǰ�ַ����Ƕ���
			{
				str += data[i][j];
			}
			else
			{
				if (str.size()>6&&str[4] == '/') { //��ǰstrΪ����
					int year = 0, month = 0, day = 0;
					string str_tmp=str.substr(0,4);
					year = atof(str_tmp.c_str()) - 2011;
					str = str.substr(5);
					str_tmp.clear();
					for (int k = 0; k < str.size(); k++) {
						if (str[k] != '/') str_tmp += str[k];
						else {
							month = atof(str_tmp.c_str());
							str = str.substr(k + 1);
							break;
						}
					}
					day = atof(str.c_str());
					tmp.data.push_back(year);
					tmp.data.push_back(month);
					tmp.data.push_back(day);
					/*for (int k = 0; k <= 1; k++) {
						if (k == year) tmp.data.push_back(1);
						else tmp.data.push_back(0);
					}
					for (int k = 1; k <= 12; k++) {
						if (k == month) tmp.data.push_back(1);
						else tmp.data.push_back(0);
					}
					for (int k = 1; k <= 31; k++) {
						if (k == day) tmp.data.push_back(1);
						else tmp.data.push_back(0);
					}*/
				}
				else tmp.data.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if (str.size() != 0) tmp.label = atof(str.c_str());
		//tmp�洢����������ϣ����Ƕ�����
		vector<double>::iterator it = tmp.data.begin();
		//it++;
		tmp.data.erase(it);//����ID
		if (data_name == "train.csv") {
			/*if (index_train[i]) train.push_back(tmp);
			else vail.push_back(tmp);*/
			train.push_back(tmp);
		}
		else  if (data_name == "val.csv" || data_name == "test.csv") {
			test.push_back(tmp);
		}
	}
}


int main()
{
	srand(time(NULL));
	HandleData("train.csv");
	HandleData("test.csv");
	cout << "success" << endl;
	ofstream out,out2,out3,out4,out5;
	out.open("data.txt");
	//out2.open("cnt.txt");
	/*out3.open("vail_data.txt");
	out4.open("vail_cnt.txt");*/
	out5.open("test.txt");
	for (int i = 0; i < train.size(); i++) {
		for (int k = 0; k < train[i].data.size(); k++) {
			out << train[i].data[k] << " ";
		}
		//out<<endl;
		out << train[i].label << endl;
	}
	/*for (int i = 0; i < vail.size(); i++) {
		for (int k = 0; k < vail[i].data.size(); k++) {
			out3 << vail[i].data[k] << " ";
		}
		out3<<endl;
		out4 << vail[i].label << endl;
	}*/
	for (int i = 0; i < test.size(); i++) {
		for (int k = 0; k < test[i].data.size(); k++) {
			out5 << test[i].data[k] << " ";
		}
		out5<<endl;
	}
	out.close();
	/*out2.close();
	out3.close();
	out4.close();*/
	out5.close();
	//system("pause");
    return 0;
}


