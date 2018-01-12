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
using namespace std;
struct Point
{
	int num;//Ŀ�����ݱ��
	double dist;//Ŀ�����ݾ���
	int ret;//Ŀ�����ݽ��
};
struct Data
{
	vector<double> data;//����
	int label;
	int label_from_predict;
	vector<Point> distance;
};
vector<Data> train;//ѵ��������
vector<Data> vail;//��֤������
vector<Data> test;//���Լ�����
bool cmp(Point a, Point b)
{
	return a.dist>b.dist;//��Ϊ���õľ������������ƶȣ����Ӧ��ȡ�ϴ���
}
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
			if (str[0] != '0') data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);//ԭʼ�ı���ȡ���
	data.erase(data.begin());
	//cout << data.size() << endl;

	map<int, int> index_train;
	if (data_name == "train.csv") {
		int num = data.size()*0.7;
		for (int i = 0; i < num; i++) {
			int Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			while (index_train[Rand]) Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
			index_train[Rand]++;
		}
	}
	for (int i = 0; i<data.size(); i++)
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
				if (str.size()>6 && str[4] == '/') { //��ǰstrΪ����
					int year = 0, month = 0, day = 0;
					string str_tmp = str.substr(0, 4);
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
		it++;
		tmp.data.erase(it);//����ID
		if (data_name == "train.csv") {
			if (index_train[i]) train.push_back(tmp);
			else vail.push_back(tmp);
		}
		else  if (data_name == "val.csv" || data_name == "test.csv") {
			test.push_back(tmp);
		}
	}
}
double Get_distance_of_two_vector(vector<double> a, vector<double> b, int select)
{
	if (select == 1)//�����پ���
	{
		double sum = 0;
		for (int i = 0; i<a.size(); i++) sum += fabs((a[i] - b[i]));
		return sum;
	}
	else if (select == 2)//ŷʽ����
	{
		double sum = 0;
		for (int i = 0; i<a.size(); i++) sum += (a[i] - b[i])*(a[i] - b[i]);
		return sqrt(sum);
	}
	else if (select == 3)//�������ƶ�;Ϊ����ֵԽ������ԽС;Ϊ���������ϴ�,�����ľ���ֵԽ������Խ��
	{
		double dot = 0;
		for (int i = 0; i<a.size(); i++) dot += a[i] * b[i];
		double a_ = 0, b_ = 0;
		for (int i = 0; i<a.size(); i++)
		{
			a_ += a[i] * a[i];
			b_ += b[i] * b[i];
		}
		if (a_ == 0 || b_ == 0) return 999999;//ĳһ����Ϊ�����������������������κ����ƿ���
		return dot / sqrt(a_*b_);
	}
}
void Get_distance() {
	for (int i = 0; i < vail.size(); i++) {
		for (int j = 0; j < train.size(); j++) {
			Point tmp;
			tmp.num = j;
			tmp.dist = Get_distance_of_two_vector(vail[i].data, train[j].data, 3);
			tmp.ret = train[j].label;
			vail[i].distance.push_back(tmp);
		}
	}
	for(int i = 0; i < test.size(); i++) {
		for (int j = 0; j < train.size(); j++) {
			Point tmp;
			tmp.dist = Get_distance_of_two_vector(test[i].data, train[j].data, 3);
			tmp.ret = train[j].label;
			test[i].distance.push_back(tmp);
		}
	}
}
int main()
{
	HandleData("train.csv");
	HandleData("test.csv");
	Get_distance();
	ofstream out;
	out.open("5_v2.csv");
	//int k=9;
	for (int k = 1; k <= 50; k++) {
		double loss = 0;
		for (int i = 0; i < vail.size(); i++) {
			sort(vail[i].distance.begin(), vail[i].distance.end(), cmp);
			vector<Point> tmp;
			for (int j = 0; j < k; j++) { //ȡǰk��
				tmp.push_back(vail[i].distance[j]);
			}
			double sum = 0;
			for (int j = 0; j < tmp.size(); j++) sum += tmp[j].dist;
			for (int j = 0; j < tmp.size(); j++) tmp[j].dist /= sum;
			vail[i].label_from_predict = 0;
			for (int j = 0; j < tmp.size(); j++) vail[i].label_from_predict += tmp[j].dist*tmp[j].ret;
			loss += (vail[i].label_from_predict - vail[i].label)*(vail[i].label_from_predict - vail[i].label);
		}
		loss/= vail.size();
		cout << "k=" << k << ",loss=" << sqrt(loss) << endl;
		for (int i = 0; i < test.size(); i++) {
			sort(test[i].distance.begin(), test[i].distance.end(), cmp);
			vector<Point> tmp;
			for (int j = 0; j < k; j++) { //ȡǰk��
				tmp.push_back(test[i].distance[j]);
			}
			double sum = 0;
			for (int j = 0; j < tmp.size(); j++) sum += tmp[j].dist;
			for (int j = 0; j < tmp.size(); j++) tmp[j].dist /= sum;
			test[i].label_from_predict = 0;
			for (int j = 0; j < tmp.size(); j++) test[i].label_from_predict += tmp[j].dist*tmp[j].ret;
			//loss += (vail[i].label_from_predict - vail[i].label)*(vail[i].label_from_predict - vail[i].label);
			out << test[i].label_from_predict << endl;
		}
	}
	system("pause");
	return 0;
}



