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
double rate;//准确率
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
		//if (num_train[i]) train.push_back(tmp);
		//else vail.push_back(tmp);
		if (data_name == "train.csv") train.push_back(tmp);
		else if (data_name == "test.csv") vail.push_back(tmp);

	}
	entropy_class = 0;
	for (map<int, int>::iterator it = label.begin(); it != label.end(); it++) {
		entropy_class += -(it->second*1.0 / data.size())*log2(it->second*1.0 / data.size());
	}
}
int choose_attr(vector<Data> v,vector<int> remain_property, int method) {
	int ret=-999;//返回值，返回这个数据集应该选择第几个属性
	if (method == 1) { //ID3
		double max_gain = 0;
		//更新entropy
		map<int,int> label;
		for(int i=0;i<v.size();i++){
			label[v[i].label]++;
		} 
		entropy_class = 0;
		for (map<int, int>::iterator it = label.begin(); it != label.end(); it++) {
			entropy_class += -(it->second*1.0 / v.size())*log2(it->second*1.0 / v.size());
		}
		for (int i = 0; i < remain_property.size(); i++) { //遍历所有种类属性
			map<int, int> num_class;//属性-该属性数量，例如age<=30, 30<age<50, age>=50
			map<int, int> num_true_class;//属性-该属性对应标签为1的数量
			for (int j = 0; j < v.size(); j++) {
				num_class[v[j].DataProperty[remain_property[i]]]++;
				if (v[j].label == 1) num_true_class[v[j].DataProperty[remain_property[i]]]++;
			}
			double gain = 0;
			for (map<int, int>::iterator it = num_class.begin(); it != num_class.end(); it++) {
				if (num_true_class[it->first] == 0) continue;
				else if (num_true_class[it->first] == it->second) continue;
				else {
					gain += (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second));
				}
			}
			cout<<entropy_class-gain<<endl;
			if (entropy_class - gain > max_gain) {
				max_gain = entropy_class - gain;
				ret = remain_property[i];
			}
		}
		cout<<"ID3选值: "<< ret <<"   "<< max_gain<<endl;
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
			for (map<int, int>::iterator it = num_class.begin(); it != num_class.end(); it++) {
				split += -1 * (it->second*1.0 / v.size())*log2(it->second*1.0 / v.size());
				if (num_true_class[it->first] == 0) continue;
				else if (num_true_class[it->first] == it->second) continue;
				else {
					gain += (it->second*1.0 / v.size())*(-(num_true_class[it->first] * 1.0 / it->second)*log2(num_true_class[it->first] * 1.0 / it->second) - ((it->second - num_true_class[it->first])*1.0 / it->second)*log2((it->second - num_true_class[it->first])*1.0 / it->second));
				}
			}
			if ((entropy_class - gain)/split > max_gain_ratio) {
				max_gain_ratio = (entropy_class - gain) / split;
				ret = remain_property[i];
			}
		}
		return ret;
	}
	else if (method == 3) { //CART
		double min_gini = 9999;
		//cout << "test" << remain_property.size() << endl;
		for (int i = 0; i < remain_property.size(); i++) { //遍历所有种类属性
			map<int, int> num_class;//属性-该属性数量，例如age<=30有多少个, 30<age<50有多少个, age>=50有多少个
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
			if (num1 == 0) {
				gini= (num2*1.0 / v.size())*(1 - (true2*1.0 / num2)*(true2*1.0 / num2) - (false2*1.0 / num2)*(false2*1.0 / num2));
			}
			else if (num2 == 0) {
				gini = (num1*1.0 / v.size())*(1 - (true1*1.0 / num1)*(true1*1.0 / num1) - (false1*1.0 / num1)*(false1*1.0 / num1));
			}
			else gini = (num1*1.0 / v.size())*(1 - (true1*1.0 / num1)*(true1*1.0 / num1) - (false1*1.0 / num1)*(false1*1.0 / num1)) + (num2*1.0 / v.size())*(1 - (true2*1.0 / num2)*(true2*1.0 / num2) - (false2*1.0 / num2)*(false2*1.0 / num2));
			if (gini < min_gini) {
				min_gini = gini;
				ret = remain_property[i];
			}
		}
		return ret;
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
	bool flag=false;
	map<int,int> tmp;
	for(int i=0;i<root->data_set.size();i++){
		tmp[root->data_set[i].label]++;
	}
	if(tmp.size()==1) flag=true;
	if (root->remain_property.size() != 0&&root->data_set.size()!=1&&!flag) { //非叶子节点
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
void Traverse_Tree(Data& vail) {
	queue<TNode*> q;
	q.push(root);
	while (!q.empty()) {
		TNode* t = q.front();
		q.pop();
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
			int num_true = 0, num_false = 0;
			for (int i = 0; i < t->data_set.size(); i++) {
				if (t->data_set[i].label == 1) num_true++;
				else num_false++;
			}
			if (num_true >= num_false) vail.label_from_predict = 1;
			else vail.label_from_predict = -1;
			break;
		}
	}
}
void Vail_model() {
	int num_true = 0;
	for (int i = 0; i < vail.size(); i++) {
		Traverse_Tree(vail[i]);
		if (vail[i].label == vail[i].label_from_predict) num_true++;
	}
	rate = num_true*1.0 / vail.size();
}
void Pruning() {
	double now_rate = rate;
	queue<TNode*> q;
	q.push(root);
	while (!q.empty()) {
		TNode* t = q.front();
		q.pop();
		vector<TNode*> children = t->children;
		t->children.clear();
		int num_true = 0, num_false = 0;
		for (int i = 0; i < t->data_set.size(); i++) {
			if (t->data_set[i].label == 1) num_true++;
			else num_false++;
		}
		if (num_true >= num_false) t->res = 1;
		else t->res = -1;
		Vail_model();
		if (rate > now_rate) now_rate = rate;
		else {
			t->children = children;
			if(t->children.size()!=0) t->res = 999;
			for (int i = 0; i < t->children.size(); i++) q.push(t->children[i]);
		}
	}
}
void BFS() {
	queue<TNode*> q;
	q.push(root);
	cout<<root->property_res<<","<<root->remain_property.size()<<","<<root->res<<endl;
	while (!q.empty()) {
		TNode* t = q.front();
		q.pop();
		for (int i = 0; i < t->children.size(); i++) {
			for (int j = 0; j < t->children[i]->level; j++) cout << "  ";
			cout << t->children[i]->property_res << "," << t->children[i]->remain_property.size() << "," << t->children[i]->res<<endl;
			q.push(t->children[i]);
		}
	}
}
int main()
{
	//srand(1510715696);
	srand(time(NULL));
	ofstream out;
	HandleData("test.csv");
	num_train.clear();
	HandleData("train.csv");
	Init_root();
	Build_Tree(root);
	//BFS();
	for (int i = 0; i<vail.size(); i++) {
		Traverse_Tree(vail[i]);
		cout << vail[i].label_from_predict << endl;
	}
}


