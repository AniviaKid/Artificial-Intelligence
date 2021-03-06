// Decision_tree.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map> 
#include<sstream>
#include<iomanip> 
#include<cmath>
#include<algorithm> 
#include<vector>
#include<time.h>
#include<queue>
using namespace std;
struct Data {
	vector<int> DataProperty;
	int label;
	int label_from_predict;
};
struct TNode {
	vector<Data> data_set;//当前节点的数据集
	vector<int> remain_property;//这个数据集还剩下的属性，存储属性的下标
	int property_i;//这个非叶节点根据第i个属性来划分子节点
	int property_res;//被划分的属性的值（子节点）
	vector<TNode*> children;//孩子节点
	int res;//叶子节点对应的标签
	int level;
};
double entropy_class;//H(D)
vector<Data> train;//训练集数据
vector<Data> vail;//测试集/验证集数据
map<int,int> num_train;//数据集中训练集的下标
TNode* root;
void HandleData(const string data_name) {
	map<int, int> label;
	ifstream inFile;
	inFile.open(data_name.c_str());
	vector<string> data;//用于存储原始文本
	char ch; string str;
	while ((ch = inFile.get()) != EOF){
		if (ch == '\n'){
			data.push_back(str);
			str.clear();
			continue;
		}
		str += ch;
	}
	if (str.size() != 0) data.push_back(str);
	int number_of_train = data.size()*0.7;
	for (int i = 0; i < number_of_train; i++) {
		int Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
		while (num_train[Rand]) Rand = (data.size()*(rand() / (RAND_MAX + 1.0)));
		num_train[Rand]++;
	}
	for (int i = 0; i < data.size(); i++) {
		Data tmp;
		str.clear();
		for (int j = 0; j < data[i].size(); j++) {
			if (data[i][j] != ',') {//当前字符不是逗号
				str += data[i][j];
			}
			else {
				tmp.DataProperty.push_back(atof(str.c_str()));
				str.clear();
			}
		}
		if (str.size() != 0) tmp.label = atof(str.c_str());
		label[tmp.label]++;
		if (num_train[i]) train.push_back(tmp);
		else vail.push_back(tmp);
		//if (data_name == "train.csv") train.push_back(tmp);
		//else if (data_name == "test.csv") test.push_back(tmp);

	}
	entropy_class = 0;
	for (map<int, int>::iterator it = label.begin(); it != label.end(); it++) {
		entropy_class += -(it->second*1.0 / data.size())*log2(it->second*1.0 / data.size());
	}
	//cout<<"H(D)= " << entropy_class << endl;
}
int choose_attr(vector<Data> v,vector<int> remain_property, int method) {
	int ret=-999;//返回值，返回这个数据集应该选择第几个属性
	if (method == 1) { //ID3
		double max_gain = 0;
		for (int i = 0; i < remain_property.size(); i++) { //遍历所有种类属性
			map<int, int> num_class;//属性-该属性数量，例如age<=30, 30<age<50, age>=50
			map<int, int> num_true_class;//属性-该属性对应标签为1的数量
			for (int j = 0; j < v.size(); j++) {
				num_class[v[j].DataProperty[remain_property[i]]]++;
				if (v[j].label == 1) num_true_class[v[j].DataProperty[remain_property[i]]]++;
			}
			double gain = 0;
			//cout << "v.size= " << v.size() << endl;
			for (map<int, int>::iterator it = num_class.begin(); it != num_class.end(); it++) {
				if (num_true_class[it->first] == 0) continue;
				else if (num_true_class[it->first] == it->second) continue;
				else {
					//cout << "test:" << num_true_class[it->first] <<" "<< (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second))<<endl;
					gain += (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second));
				}
			}
			//cout << "H(D|A" << i << ")= " << gain << "   " << "H(D)-H(D|A" << i << ")=" << entropy_class - gain << endl;
			if (entropy_class - gain > max_gain) {
				max_gain = entropy_class - gain;
				ret = remain_property[i];
			}
		}
		//cout << max_gain << "  " << ret << endl;
		return ret;
	}
	else if (method == 2) { //C4.5
		double max_gain_ratio = 0;
		for (int i = 0; i < remain_property.size(); i++) { //遍历所有种类属性
			map<int, int> num_class;//属性-该属性数量，例如age<=30, 30<age<50, age>=50
			map<int, int> num_true_class;//属性-该属性对应标签为1的数量
			for (int j = 0; j < v.size(); j++) {
				num_class[v[j].DataProperty[remain_property[i]]]++;
				if (v[j].label == 1) num_true_class[v[j].DataProperty[remain_property[i]]]++;
			}
			double gain = 0,split=0;
			//cout << "v.size= " << v.size() << endl;
			for (map<int, int>::iterator it = num_class.begin(); it != num_class.end(); it++) {
				split += -1 * (it->second*1.0 / v.size())*log2(it->second*1.0 / v.size());
				if (num_true_class[it->first] == 0) continue;
				else if (num_true_class[it->first] == it->second) continue;
				else {
					//cout << "test:" << num_true_class[it->first] <<" "<< (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second))<<endl;
					gain += (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second));
				}
			}
			//cout << "H(D|A" << i << ")= " << gain << "   " << "H(D)-H(D|A" << remain_property[i] << ")=" << entropy_class - gain << "  GainRatio(A"<< remain_property[i]<<"): " << (entropy_class - gain) / split<<endl;
			if ((entropy_class - gain)/split > max_gain_ratio) {
				max_gain_ratio = (entropy_class - gain) / split;
				ret = remain_property[i];
			}
		}
		//cout << max_gain_ratio << "  " << ret << endl;
		return ret;
	}
	else if (method == 3) { //CART
		double min_gini = 9999;
		//cout << "test" << remain_property.size() << endl;
		for (int i = 0; i < remain_property.size(); i++) { //遍历所有种类属性
			map<int, int> num_class;//属性-该属性数量，例如age<=30, 30<age<50, age>=50
			map<int, int> num_true_class;//属性-该属性对应标签为1的数量
			int num_true = 0;//这个数据集一共有多少标签为1
			for (int j = 0; j < v.size(); j++) {
				num_class[v[j].DataProperty[remain_property[i]]]++;
				if (v[j].label == 1) {
					num_true_class[v[j].DataProperty[remain_property[i]]]++;
					num_true++;
				}
			}
			int num1 = num_class[remain_property[i]], num2 = v.size() - num1;
			int true1 = num_true_class[remain_property[i]], true2 = num_true - true1;
			int false1 = num1 - true1, false2 = num2 - true2;
			double gini;
			//cout << num1 << " " << num2 << " " << true1 << " " << true2 << " " << false1 << " " << false2 << endl;
			if (num1 == 0) {
				gini= (num2*1.0 / v.size())*(1 - (true2*1.0 / num2)*(true2*1.0 / num2) - (false2*1.0 / num2)*(false2*1.0 / num2));
			}
			else if (num2 == 0) {
				gini = (num1*1.0 / v.size())*(1 - (true1*1.0 / num1)*(true1*1.0 / num1) - (false1*1.0 / num1)*(false1*1.0 / num1));
			}
			else gini = (num1*1.0 / v.size())*(1 - (true1*1.0 / num1)*(true1*1.0 / num1) - (false1*1.0 / num1)*(false1*1.0 / num1)) + (num2*1.0 / v.size())*(1 - (true2*1.0 / num2)*(true2*1.0 / num2) - (false2*1.0 / num2)*(false2*1.0 / num2));
			//cout << "gini(A|" << remain_property[i] << "): " << gini << endl;
			if (gini < min_gini) {
				min_gini = gini;
				ret = remain_property[i];
			}
		}
		//cout << min_gini << " " << ret << endl;
		return ret;
	}
}
void print(vector<Data> tmp) {
	for (int i = 0; i < tmp.size(); i++) {
		for (int j = 0; j < tmp[i].DataProperty.size(); j++) {
			cout << tmp[i].DataProperty[j] << " ";
		}
		cout << tmp[i].label << endl;
	}
}
void Init_root() {
	root = new TNode;
	root->data_set = train;
	for (int i = 0; i < root->data_set[0].DataProperty.size(); i++) {
		root->remain_property.push_back(i);
	}
	root->res = 999;
	root->level = 0;
}
void Build_Tree(TNode* root) {
	if (root->remain_property.size() != 0&&root->data_set.size()!=1) { //非叶子节点
		int chosen_attr = choose_attr(root->data_set, root->remain_property, 1);
		root->property_i = chosen_attr;
		map<int, int> attr;//属性-该属性数量，例如age<=30, 30<age<50, age>=50
		for (int i = 0; i < root->data_set.size(); i++) { //划分数据集
			if (!attr[root->data_set[i].DataProperty[chosen_attr]]) { //这个属性值第一次出现
				TNode* tmp = new TNode;
				tmp->property_res = root->data_set[i].DataProperty[chosen_attr];
				tmp->remain_property = root->remain_property;
				for (vector<int>::iterator it = tmp->remain_property.begin(); it != tmp->remain_property.end(); it++) { //子节点剩余属性中删去被选中的
					if (*it == chosen_attr) {
						tmp->remain_property.erase(it);
						break;
					}
				}
				tmp->data_set.push_back(root->data_set[i]);//填充数据集
				tmp->res = 999;
				tmp->level = root->level + 1;
				root->children.push_back(tmp);
				attr[root->data_set[i].DataProperty[chosen_attr]]++;
			}
			else { //这个属性值不是第一次出现
				for (int j = 0; j < root->children.size(); j++) { //遍历孩子，将这行数据填充进去
					if (root->children[j]->property_res == root->data_set[i].DataProperty[chosen_attr]) {
						root->children[j]->data_set.push_back(root->data_set[i]);
					}
				}
			}
		}
		//递归建树
		for (int i = 0; i < root->children.size(); i++) Build_Tree(root->children[i]);
	}
	else { //叶子节点
		int num_true = 0, num_false = 0;
		for (int i = 0; i < root->data_set.size(); i++) {
			if (root->data_set[i].label == 1) num_true++;
			else num_false++;
		}
		if (num_true >= num_false) root->res = 1;
		else root->res = -1;
	}
}
ofstream out;
void test(TNode* t) {
	if (t != NULL) {
		out << "数据集大小: " << t->data_set.size() << " 剩余属性数量: " << t->remain_property.size() << " 标签: " << t->res << "   深度: " << t->level << "  孩子数量: " << t->children.size() << endl;
		for (int i = 0; i < t->children.size(); i++) test(t->children[i]);
	}
}
void BFS() {
	queue<TNode*> q;
	q.push(root);
	out << root->data_set.size() << "," << root->remain_property.size() << "," << root->children.size() << endl;
	while (!q.empty()) {
		TNode* t = q.front();
		q.pop();
		for (int i = 0; i < t->children.size(); i++) {
			for (int j = 0; j < t->children[i]->level; j++) out << "  ";
			out << t->children[i]->data_set.size() << "," << t->children[i]->remain_property.size() << "," << t->children[i]->children.size() << endl;
			q.push(t->children[i]);
		}
	}
}
void Traverse_Tree(Data& vail) {
	queue<TNode*> q;
	q.push(root);
	while (!q.empty()) {
		TNode* t = q.front();
		q.pop();
		//cout << "数据集大小: " << t->data_set.size() << " 剩余属性数量: " << t->remain_property.size() << " 标签: " << t->res << "   深度: " << t->level << "  孩子数量: " << t->children.size() << endl;
		if (t->res ==1 || t->res==-1) {
			vail.label_from_predict = t->res;
			break;
		}
		int attr = vail.DataProperty[t->property_i];//这个数据集的第i个属性，i是这个节点的划分点
		bool flag = false;
		for (int i = 0; i < t->children.size(); i++) {
			if (t->children[i]->property_res == attr) {
				q.push(t->children[i]);
				flag = true;
				break;
			}
		}
		if (!flag) {
			int rand_i= (int)(t->children.size()*(rand() / (RAND_MAX + 1.0)));//随机选择一个孩子
			q.push(t->children[rand_i]);
		}
	}
}
int main()
{
	out.open("test.txt");
	//srand(1510383609);
	srand(time(NULL));
	HandleData("train.csv");
	Init_root();
	Build_Tree(root);
	//Traverse_Tree(vail[0]);
	//cout << vail[0].label_from_predict << endl;
	int num_true = 0;
	for (int i = 0; i < vail.size(); i++) {
		Traverse_Tree(vail[i]);
		if (vail[i].label == vail[i].label_from_predict) num_true++;
		//cout << vail[i].label_from_predict << endl;
	}
	cout << "rate: " << num_true*1.0 / vail.size() << endl;
	//test(root);
	//BFS();
	//Traverse_Tree(vail[0], root);
	//cout << vail[0].label_from_predict << endl;
	/*for (int i = 0; i < vail.size(); i++) {
		Traverse_Tree(vail[i], root);
		cout << vail[i].label_from_predict << endl;
	}*/
	/*choose_attr(root->data_set, root->remain_property, 1);
	choose_attr(root->data_set, root->remain_property, 2);
	choose_attr(root->data_set, root->remain_property, 3);*/
	/*int seed = time(NULL);
	ofstream out;
	out.open("seed.txt");
	out << seed << endl;*/
	out.close();
	system("pause");
}

