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

vector<string> split(string str,string pattern)//�ַ����ָ��
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//��չ�ַ����Է������
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

int string_to_number(string str)//�ַ���ת�������� 
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
	
	int num_of_train=0;//ѵ�����ı��� 
	int num_of_test=0;//���Լ��ı��� 
	
	int num_of_d=0;//ѵ�����ı���ÿ�е����ݸ���(ά������) 
	
	string eachline_of_train; 
	string eachline_of_test; 
	
    string pattern=" ";
    
    
    /*****************************************ѵ����*******************************************************/
    while(getline(datafile_train,eachline_of_train))//����ѵ������ÿ���ı� 
    {
		int pos[100];//��������λ�� 
		pos[0]=-1; 
		int x=0;//���Ÿ��� ,Ҳ��ÿ���ı��е����ݸ��� 
     	for(int i=0;i<eachline_of_train.length();i++)
     	{
     		if(eachline_of_train[i]==',') 
			 {
			 	x++;
				pos[x]=i; 
			 	//��x������λ��pos[x] 
			 }
		 }
		num_of_d=x; 
		for(int k=0;k<x;k++)
		{
			string data=eachline_of_train.substr(pos[k]+1,pos[k+1]-pos[k]-1);
			string zhengshu;
         	string xiaoshu;
         	double z;//��������(��������) 
         	double x;//С������(��������)  
         	bool f=false;//�ж��Ƿ�Ϊ���� 
         	if(data[0]=='-')//���Ǹ��� 
         	{
         		f=true; 
         		data=data.substr(1,data.length()-1); 
         	}
         	bool e=false;//�ж��Ƿ�С���� 
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
			if(e==false)//����С���� 
			{
				if(f==true) z=(-1)*string_to_number(data);
				else if(f==false) z=string_to_number(data);
				x=0;
			}
		    int b=xiaoshu.length();
		    double a=1;
		    for(int n=0;n<b;n++) a=a*10;    
			    
		    train_data[num_of_train][k+1]=z+x/a;//�и��һ�����־ͷŽ�train_data����           
		}
		string label=eachline_of_train.substr(pos[x]+1,2);
		
		if(label=="1") train_label[num_of_train]=1;
		else if(label=="0") train_label[num_of_train]=0;
		
	    num_of_train++;//ѵ�����ı�����1 
	}
	
	cout<<endl<<" ѵ�����ı���="<<num_of_train<<endl; 
	cout<<endl<<" data��="<<num_of_d<<endl<<endl; 
	
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
	
	
    
    /*****************************************���Լ�*******************************************************/
    for(int i=0;i<2001;i++)
	{
		test_data[i][0]=1;
	    for(int j=1;j<50;j++) test_data[i][j]=0;
	}
	while(getline(datafile_test,eachline_of_test))//�������Լ���ÿ���ı� 
	{
		int pos[100];//��������λ�� 
		pos[0]=-1; 
		int x=0;//���Ÿ��� ,Ҳ��ÿ���ı��е����ݸ��� 
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
	     	double z;//��������(��������) 
	     	double x;//С������(��������)  
	     	bool f=false;//�ж��Ƿ�Ϊ����
			if(data[0]=='-')
			{
				f=true;
				data=data.substr(1,data.length()-1);
			} 
	     	bool e=false;//�ж��Ƿ�С���� 
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
			if(e==false)//����С���� 
			{
				if(f==true) z=(-1)*string_to_number(data);
				else if(f==false) z=string_to_number(data);
				x=0;
			} 
		    int b=xiaoshu.length();
		    double a=1;
		    for(int n=0;n<b;n++) a=a*10;    
			    
		    test_data[num_of_test][k+1]=z+x/a;//�и��һ�����־ͷŽ�test_data����	            
	    	 
		}
		
	    num_of_test++;//���Լ��ı�����1 
	}
	
	cout<<endl<<" ���Լ��ı���="<<num_of_test<<endl; 
	cout<<endl<<" data��="<<num_of_d<<endl<<endl; 
	
	/*cout<<endl<<"---------------test_data[][]------------------"<<endl;
	for(int i=0;i<num_of_test;i++)
	{
		for(int j=0;j<num_of_d+1;j++) cout<<test_data[i][j]<<"  ";
		cout<<endl<<endl<<endl;
	} 
	cout<<"--------------------------------------------------"<<endl;*/
	
    /*****************************************������֤ ������֤��**************************************************/

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
    for(int i=0;i<num_of_d+1;i++) w[i]=1;//��ʼ��ȫΪ1 
	double w_next[num_of_d+1];
    
    int T=0;//��������
	double h=0.01;//����
	double train_eachline_data[num_of_d+1];//����Ƚϵĵ�ǰѵ�����ı���
	while(T>0)
	{
		cout<<"T="<<T<<endl;
	    for(int i=0;i<num_of_d+1;i++)//��ÿ��ά�� 
		{
			double Cost=0;
			for(int j=0;j<num_of_train;j++)//��ÿ��ѵ�����ı� 
			{
				for(int k=0;k<num_of_d+1;k++) train_eachline_data[k]=train_data[j][k];//��õ�ǰѵ�����ı���
				/*cout<<"-----------��ǰѵ�����ı�-------------"<<endl;
				for(int k=0;k<num_of_d+1;k++) cout<<train_eachline_data[k]<<"  ";
				cout<<endl<<"--------------------------------------"<<endl;
				cout<<"-------------------��ǰw--------------"<<endl;
				for(int k=0;k<num_of_d+1;k++) cout<<w[k]<<"  ";
				cout<<endl<<"--------------------------------------"<<endl;*/
				double s=0;//��ѵ�����ı���Ȩ�ط���
				//cout<<endl;
				for(int k=0;k<num_of_d+1;k++){
					//cout<<"w="<<w[k]<<"   train_eachline_data="<<train_eachline_data[k]<<endl; 
					s=s+w[k]*train_eachline_data[k];
					//cout<<"s="<<s<<endl;
				}
				//cout<<endl; 
				//�õ�Ȩ�ط���
				//cout<<"Ȩ�ط���="<<s<<endl;
				Cost=Cost+train_data[j][i]*( (1/ (1+exp(-s)) )-train_label[j]);
			} 
			//�õ���ά���µ�Cost
			//cout<<"Cost="<<Cost<<endl;
			//cout<<"w[i]="<<w[i]<<"   h*Cost="<<h*Cost<<endl; 
			w_next[i]=w[i]-h*Cost; 
			//cout<<"w_next[i]="<<w_next[i]<<endl;
		} 
		for(int g=0;g<num_of_d+1;g++) w[g]=w_next[g];
		//�õ��µ�w 
		/*cout<<"-------------------------�µ�w-----------------------"<<endl;
		cout<<"T="<<T<<"   "; 
		for(int i=0;i<num_of_d+1;i++) cout<<w[i]<<"  ";
		cout<<endl;
		cout<<"-----------------------------------------------------"<<endl;*/
	    T--; 
	} 
   
   
    cout<<"-------------------------�µ�w-----------------------"<<endl;
	cout<<"T="<<T<<"   "; 
	for(int i=0;i<num_of_d+1;i++) cout<<w[i]<<"  ";
	cout<<endl;
	cout<<"-----------------------------------------------------"<<endl;
    
	
	double p;//Ԥ��ĸ��� 
    double test_eachline_data[num_of_d+1];//��ǰ���Լ��ı���
    for(int i=0;i<num_of_test;i++)
    {
    	for(int j=0;j<num_of_d+1;j++) test_eachline_data[j]=test_data[i][j];//��õ�ǰ���Լ��ı���
    	/*cout<<"++++++++++++++++++��ǰ���Լ�+++++++++++++"<<endl;
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
    	cout<<"test"<<i<<"   ����Ϊ"<<p;
    	if(p>=0.5) cout<<"   Ԥ��Ϊ1"<<endl;
    	else cout<<"   Ԥ��Ϊ0"<<endl;
    }
    
    return 0;
    
} 
