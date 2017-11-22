#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <math.h>
#include <time.h>

using namespace std; 

typedef struct weight{
	vector<double> vector_w;
	int num;//编号 
}Weight;

typedef struct x_test{
	vector<double> x;
	int num;//编号 
	int lable;
}X_test;

vector<Weight> w;//权值向量集w，w0为第一次迭代的
vector<X_test> train,validation,alltest;

string train_name="train.csv";

X_test string_to_double(string s) {//将读入的训练集train中的每个样本，从string转成单个X_test存储起来 
	X_test a; 
	string temp=""; 
	for(int i=0;i<s.length();i++){
		if(s[i]!=','){
			temp+=s[i];
		}
		else{
			istringstream Ans(temp);
			double ans=0;
			Ans>>ans;
			a.x.push_back(ans);
			temp="";
		}
	}
	if(temp!=""){
		istringstream Ans(temp);
			double ans=0;
			Ans>>ans;
			a.lable=(int)ans;
			temp="";
	}
	return a;
}

void Get_xvector(string filename){
	ifstream f(filename.c_str());
	string test;
	int nth_num=0;
	map<int,int> mark;//<num,mark>
	while(getline(f,test)){
		X_test a=string_to_double(test);
		a.num=nth_num++;
		alltest.push_back(a);
	} 
	random_shuffle(alltest.begin(),alltest.end());
	/*
	srand(time(NULL));//随机打乱 
	int cnt=0,b=9000; 
	while(mess_alltest.size()!=alltest.size()){
		int r=rand()%(alltest.size()-cnt);
		if(mark.find(alltest[r].num)==mark.end()){//没出现过 
			mess_alltest.push_back(alltest[r]);
			mark[alltest[r].num]++;
			cnt++;
		} 
	}
	cout<<"打乱后，检查messalltest大小:"<<mess_alltest.size()<<endl;
	*/
	int cnt=(int)alltest.size()*0.7;
	for(int i=0;i<alltest.size();i++){//3:1分 
		if(i<cnt){
			alltest[i].x.insert(alltest[i].x.begin(),1);//初始化x0=1 
			train.push_back(alltest[i]);
		}
		else validation.push_back(alltest[i]);
	}
}


double logistic(double s){//sigmoid函数 
	
	return 1/(1+exp(-s));
}

double calcul_s(Weight w,X_test test_x){//返回每个样例的权重分数 
	double ans=0;
	for(int i=0;i<w.vector_w.size();i++){
		ans+=w.vector_w[i]*test_x.x[i];
	}
	return ans;
}

vector<int> mark_costzero;

double calcul_cost(Weight w,int j){//计算第j维的梯度 
	double ans=0;
	for(int i=0;i<train.size();i++){
		double wx=calcul_s(w,train[i]);
		ans+=(logistic(wx)-train[i].lable*1.0)*train[i].x[j];
	}
	if(ans<10e-5){
	//	ans=0;//小于一定阈值，近似认为梯度为0 
		mark_costzero.push_back(j);//记录这个维的数 
	}
	return ans;
}

Weight new_w(Weight old_w,double step){//更新W 
	Weight new_w;
	new_w.num=old_w.num+1;
	double m=train.size()*1.0;
	for(int i=0;i<old_w.vector_w.size();i++){
	//	if(find(mark_costzero.begin(),mark_costzero.end(),i)==mark_costzero.end()) 
			new_w.vector_w.push_back(old_w.vector_w[i]-(step/m)*calcul_cost(old_w,i));
	}
	return new_w;
}

void vali_test(double time){
	int k=w.size()-1;//取这么多次更新后的最后一个 
	int correct_num=0;
	for(int i=0;i<validation.size();i++){
		int predict=0;
		double pre=logistic( calcul_s(w[k],validation[i]) );
		if(pre>=0.5) predict=1;
		else predict=0;
		if(predict==validation[i].lable) correct_num++;
	}
	double cn=correct_num*1.0,vali_size=validation.size()*1.0;
	cout<<"correct/total: "<<cn/vali_size<<endl;
} 

void LR(int times,double step){//迭代次数,步长 
	while(times-- ){//迭代 、、&& mark_costzero.size()!=train[0].x.size() 
		int last=w.size()-1;
		Weight wi=new_w(w[last],step);
		w.push_back(wi);
		if(times==900||times==700||times==500||times==300||times==100)vali_test(step);
	}
	vali_test(step);
}



int main(){
	Get_xvector(train_name);//获取训练集、验证集 
/*小数据集 
	X_test a;
	a.x.push_back(1);a.x.push_back(1);a.x.push_back(2);a.lable=1;
	train.push_back(a);
	X_test b;
	b.x.push_back(1);b.x.push_back(2);b.x.push_back(-1);b.lable=1;
	train.push_back(b);
	X_test c;
	c.x.push_back(3);c.x.push_back(3);c.lable=1;
	validation.push_back(c);
*/	
	Weight w0;//init W0=1
	for(int i=0;i<train[0].x.size();i++) w0.vector_w.push_back(1);
	w.push_back(w0);

	
/*小数据集 
	cout<<"权重分数：\n";
	for(int i=0;i<train.size();i++){
		int k=w.size()-1;
		cout<<calcul_s(w[k],train[i])<<endl;
	} 
	
	cout<<"每一维梯度:\n";
	for(int i=0;i<w[0].vector_w.size();i++){
		int k=w.size()-1;
		cout<<calcul_cost(w[k],i)<<endl;
	} 
	Weight wi=new_w(w[w.size()-1],0.1);
	w.push_back(wi);
	for(int i=0;i<w[0].vector_w.size();i++){
		cout<<w[w.size()-1].vector_w[i]<<" ";
	}
	vali_test(0.1);
*/
	
	int times=1000;
	//for(double step=0.1;step<=0.4;step+=0.1){
		double step=0.1;
		LR(times,step);
		//vali_test(step);
//	}
} 
