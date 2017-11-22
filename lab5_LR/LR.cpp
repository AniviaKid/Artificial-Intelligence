#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

double train_data[8001][50];
int train_label[8001];

double  train_data_a[1600][50]; 
double  train_data_b[1600][50];
double  train_data_c[1600][50];
double  train_data_d[1600][50];
double  train_data_e[1600][50];

double train_data_divide[6400][50];
double validation_data_divide[1600][50];

double test_data[2001][50];
int test_label[2001];

vector<string> split(string str,string pattern)//字符串分割函数
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

int string_to_number(string str)//字符串转换成数字 
{
	double sum=0;
	for(int i=0;i<str.length();i++)
	{
		sum=sum*10+(str[i]-'0');
	}
	return sum;
}


int main()
{
	for(int i=0;i<8001;i++)
    {
    	train_data[i][0]=1;
	    for(int j=1;j<50;j++) train_data[i][j]=0;
    }

	ifstream datafile_train("train.csv");
	ifstream datafile_test("test.csv");
	
	int num_of_train=0;//训练集文本数 
	int num_of_test=0;//测试集文本数 
	
	int num_of_d=0;//训练集文本中每行的数据个数(维度数？) 
	
	string eachline_of_train; 
	string eachline_of_test; 
	
    string pattern=" ";
    
    
    /*****************************************训练集*******************************************************/
    while(getline(datafile_train,eachline_of_train))//遍历训练集的每个文本 
    {
		int pos[100];//逗号所在位置 
		pos[0]=-1; 
		int x=0;//逗号个数 ,也是每行文本中的数据个数 
     	for(int i=0;i<eachline_of_train.length();i++)
     	{
     		if(eachline_of_train[i]==',') 
			 {
			 	x++;
				pos[x]=i; 
			 	//第x个逗号位于pos[x] 
			 }
		 }
		num_of_d=x; 
		for(int k=0;k<x;k++)
		{
			string data=eachline_of_train.substr(pos[k]+1,pos[k+1]-pos[k]-1);
			string zhengshu;
         	string xiaoshu;
         	double z;//整数部分(数字类型) 
         	double x;//小数部分(数字类型)  
         	bool f=false;//判断是否为负数 
         	if(data[0]=='-')//若是负数 
         	{
         		f=true; 
         		data=data.substr(1,data.length()-1); 
         	}
         	bool e=false;//判断是否含小数点 
        	for(int i=0;i<data.length();i++)
        	{
            	if(data[i]=='.')
            	{
            		e=true;
               	    zhengshu=data.substr(0,i);
               	    
               	    if(f==true) z=(-1)*string_to_number(zhengshu);
               	    else if(f==false) z=string_to_number(zhengshu);
               	    
		            xiaoshu=data.substr(i+1,data.length());
		            
		            if(f==true) x=(-1)*string_to_number(xiaoshu);
		            else if(f==false) x=string_to_number(xiaoshu);
		        }  
			}	  
			if(e==false)//不含小数点 
			{
				if(f==true) z=(-1)*string_to_number(data);
				else if(f==false) z=string_to_number(data);
				x=0;
			}
		    int b=xiaoshu.length();
		    double a=1;
		    for(int n=0;n<b;n++) a=a*10;    
			    
		    train_data[num_of_train][k+1]=z+x/a;//切割出一个数字就放进train_data数组           
		}
		string label=eachline_of_train.substr(pos[x]+1,2);
		
		if(label=="1") train_label[num_of_train]=1;
		else if(label=="0") train_label[num_of_train]=0;
		
	    num_of_train++;//训练集文本数加1 
	}
	
	cout<<endl<<" 训练集文本数="<<num_of_train<<endl; 
	cout<<endl<<" data数="<<num_of_d<<endl<<endl; 
	
	/*cout<<endl<<"---------------train_data[][]------------------"<<endl;
	for(int i=0;i<num_of_train;i++)
	{
		for(int j=0;j<=num_of_d;j++) cout<<train_data[i][j]<<"  ";
		cout<<endl<<endl<<endl;
	} 
	cout<<"--------------------------------------------------"<<endl;

    cout<<endl<<"---------------train_label[][]------------------"<<endl;
	for(int i=0;i<num_of_train;i++)
	{
		cout<<train_label[i]<<endl;
	} 
	cout<<"--------------------------------------------------"<<endl;*/
	
	
    
    /*****************************************测试集*******************************************************/
    for(int i=0;i<2001;i++)
	{
		test_data[i][0]=1;
	    for(int j=1;j<50;j++) test_data[i][j]=0;
	}
	while(getline(datafile_test,eachline_of_test))//遍历测试集的每个文本 
	{
		int pos[100];//逗号所在位置 
		pos[0]=-1; 
		int x=0;//逗号个数 ,也是每行文本中的数据个数 
	 	for(int i=0;i<eachline_of_test.length();i++)
	 	{
	 		if(eachline_of_test[i]==',') 
			 {
			 	x++;
				pos[x]=i;  
			 }
		 }
		for(int k=0;k<x;k++)
		{
			string data=eachline_of_test.substr(pos[k]+1,pos[k+1]-pos[k]-1);
			string zhengshu;
	     	string xiaoshu;
	     	double z;//整数部分(数字类型) 
	     	double x;//小数部分(数字类型)  
	     	bool f=false;//判断是否为负数
			if(data[0]=='-')
			{
				f=true;
				data=data.substr(1,data.length()-1);
			} 
	     	bool e=false;//判断是否含小数点 
	    	for(int i=0;i<data.length();i++)
	    	{
	        	if(data[i]=='.')
	        	{
	        		e=true;
	           	    zhengshu=data.substr(0,i);
	           	    
	           	    if(f==true) z=(-1)*string_to_number(zhengshu); 
	           	    else if(f==false) z=string_to_number(zhengshu); 
	           	    
		            xiaoshu=data.substr(i+1,data.length());
		            
		            if(f==true) x=(-1)*string_to_number(xiaoshu);
		            else if(f==false) x=string_to_number(xiaoshu);
		        }  
			}	  
			if(e==false)//不含小数点 
			{
				if(f==true) z=(-1)*string_to_number(data);
				else if(f==false) z=string_to_number(data);
				x=0;
			} 
		    int b=xiaoshu.length();
		    double a=1;
		    for(int n=0;n<b;n++) a=a*10;    
			    
		    test_data[num_of_test][k+1]=z+x/a;//切割出一个数字就放进test_data数组	            
	    	 
		}
		
	    num_of_test++;//测试集文本数加1 
	}
	
	cout<<endl<<" 测试集文本数="<<num_of_test<<endl; 
	cout<<endl<<" data数="<<num_of_d<<endl<<endl; 
	
	/*cout<<endl<<"---------------test_data[][]------------------"<<endl;
	for(int i=0;i<num_of_test;i++)
	{
		for(int j=0;j<num_of_d+1;j++) cout<<test_data[i][j]<<"  ";
		cout<<endl<<endl<<endl;
	} 
	cout<<"--------------------------------------------------"<<endl;*/
	
    /*****************************************交叉验证 划分验证集**************************************************/

    for(int i=0;i<1600;i++)
    {
    	for(int j=0;j<50;j++) 
		{
		    train_data_a[i][j]=0;
		    train_data_b[i][j]=0;
		    train_data_c[i][j]=0;
		    train_data_d[i][j]=0;
			train_data_e[i][j]=0;
		}
    }

    for(int i=0;i<1600;i++)
    {
    	for(int j=0;j<num_of_d+1;j++) 
		{
			train_data_a[i][j]=train_data[i][j];
			train_data_b[i][j]=train_data[i+1600][j];
			train_data_c[i][j]=train_data[i+3200][j];
			train_data_d[i][j]=train_data[i+4800][j];
			train_data_e[i][j]=train_data[i+6400][j];
		}
    }
   
    cout<<"-----------check---------------"<<endl;
    //cout<<"num_d:"<<num_of_d<<endl; 
    for(int j=0;j<num_of_d+1;j++) 
    {
		cout<<train_data_a[6000][j]<<"  ";
	}
	cout<<endl;
	cout<<"------------------------------"<<endl;


    
    	for(int j=0;j<num_of_d+1;j++) 
		{
			cout<<train_data_e[3][j]<<" * ";
		}
		cout<<endl;
    
   /* int k;
    for(int i=0;i<1600;i++)
    {
    	for(int j=0;j<num_of_d+1;j++) 
    	{
    		train_data_divide[i][j]=train_data_1[i][j];
    		train_data_divide[i+1600][j]=train_data_2[i][j];
    		train_data_divide[i+3200][j]=train_data_3[i][j];
    		train_data_divide[i+4800][j]=train_data_4[i][j];
    	}
    	k=i+4800;
    }
    cout<<"k="<<k<<endl; 
    
    for(int i=0;i<1600;i++)
    {
    	for(int j=0;j<num_of_d+1;j++) 
    	{
    		validation_data_divide[i][j]=train_data_5[i][j];
    	}
    }
   
    	for(int j=0;j<num_of_d+1;j++) 
		{
			cout<<train_data_divide[9500][j]<<"  ";
		}
		cout<<endl;*/
    
   
   cout<<"--------------------------stop------------------------------------"<<endl;
    double w[num_of_d+1];
    for(int i=0;i<num_of_d+1;i++) w[i]=1;//初始化全为1 
	double w_next[num_of_d+1];
    
    int T=0;//迭代次数
	double h=0.01;//步长
	double train_eachline_data[num_of_d+1];//参与比较的当前训练集文本行
	while(T>0)
	{
		cout<<"T="<<T<<endl;
	    for(int i=0;i<num_of_d+1;i++)//对每个维度 
		{
			double Cost=0;
			for(int j=0;j<num_of_train;j++)//对每个训练集文本 
			{
				for(int k=0;k<num_of_d+1;k++) train_eachline_data[k]=train_data[j][k];//获得当前训练集文本行
				/*cout<<"-----------当前训练集文本-------------"<<endl;
				for(int k=0;k<num_of_d+1;k++) cout<<train_eachline_data[k]<<"  ";
				cout<<endl<<"--------------------------------------"<<endl;
				cout<<"-------------------当前w--------------"<<endl;
				for(int k=0;k<num_of_d+1;k++) cout<<w[k]<<"  ";
				cout<<endl<<"--------------------------------------"<<endl;*/
				double s=0;//该训练集文本的权重分量
				//cout<<endl;
				for(int k=0;k<num_of_d+1;k++){
					//cout<<"w="<<w[k]<<"   train_eachline_data="<<train_eachline_data[k]<<endl; 
					s=s+w[k]*train_eachline_data[k];
					//cout<<"s="<<s<<endl;
				}
				//cout<<endl; 
				//得到权重分量
				//cout<<"权重分量="<<s<<endl;
				Cost=Cost+train_data[j][i]*( (1/ (1+exp(-s)) )-train_label[j]);
			} 
			//得到该维度下的Cost
			//cout<<"Cost="<<Cost<<endl;
			//cout<<"w[i]="<<w[i]<<"   h*Cost="<<h*Cost<<endl; 
			w_next[i]=w[i]-h*Cost; 
			//cout<<"w_next[i]="<<w_next[i]<<endl;
		} 
		for(int g=0;g<num_of_d+1;g++) w[g]=w_next[g];
		//得到新的w 
		/*cout<<"-------------------------新的w-----------------------"<<endl;
		cout<<"T="<<T<<"   "; 
		for(int i=0;i<num_of_d+1;i++) cout<<w[i]<<"  ";
		cout<<endl;
		cout<<"-----------------------------------------------------"<<endl;*/
	    T--; 
	} 
   
   
    cout<<"-------------------------新的w-----------------------"<<endl;
	cout<<"T="<<T<<"   "; 
	for(int i=0;i<num_of_d+1;i++) cout<<w[i]<<"  ";
	cout<<endl;
	cout<<"-----------------------------------------------------"<<endl;
    
	
	double p;//预测的概率 
    double test_eachline_data[num_of_d+1];//当前测试集文本行
    for(int i=0;i<num_of_test;i++)
    {
    	for(int j=0;j<num_of_d+1;j++) test_eachline_data[j]=test_data[i][j];//获得当前测试集文本行
    	/*cout<<"++++++++++++++++++当前测试集+++++++++++++"<<endl;
    	for(int j=0;j<num_of_d+1;j++) cout<<test_eachline_data[j]<<"  ";
    	cout<<"_________________________________________"<<endl;*/
    	double w_t=0;
    	for(int j=0;j<num_of_d+1;j++)  
		{
			//cout<<"w="<<w[j]<<"    test_eachline_data"<<test_eachline_data[j]<<endl;
			w_t=w_t+w[j]*test_eachline_data[j];
			//cout<<"w_t"<<w_t<<endl; 
		}
    	p=1/(1+exp(-w_t)) ;
    	cout<<"test"<<i<<"   概率为"<<p;
    	if(p>=0.5) cout<<"   预测为1"<<endl;
    	else cout<<"   预测为0"<<endl;
    }
    
    return 0;
    
} 
