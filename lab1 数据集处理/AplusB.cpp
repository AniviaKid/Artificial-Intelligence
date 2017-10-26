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
bool cmp(const vector<int> &a,const vector<int> &b)
{
	if(a[0]<b[0]) return a<b;
	else if(a[1]<b[1]) return a<b;
	else return a<b;
}
class Sparse_Matrix
{
public:
	int row;//行 
	int col;//列 
	int num;//数值个数 
	vector< vector<int> > sequence;
	Sparse_Matrix()
	{
		
	}
	Sparse_Matrix operator+(Sparse_Matrix &second_matrix)
	{
		Sparse_Matrix res;
		res.row=this->row;
		res.col=this->col;
		vector< vector<int> > tmp=this->sequence;
		for(int i=0;i<second_matrix.sequence.size();i++) tmp.push_back(second_matrix.sequence[i]);//合并两个三元表
		sort(tmp.begin(),tmp.end(),cmp);
		for(vector< vector<int> >::iterator it=tmp.begin();it!=tmp.end();it++)
		{
			if(it!=tmp.end()-1)
			{
				vector< vector<int> >::iterator next=it+1;
				if((*it)[0]==(*next)[0]&&(*it)[1]==(*next)[1])//三元表前两个元素相同
				{
					//合并这两项
					(*next)[2]+=(*it)[2];
					tmp.erase(it);
				}
			}
		}
		for(vector< vector<int> >::iterator it=tmp.begin();it!=tmp.end();it++)
		{
			if((*it)[2]==0) tmp.erase(it);
		}
		res.num=tmp.size();
		res.sequence=tmp;
		return res;
	}
	void operator+=(Sparse_Matrix &second_matrix)
	{
		*this=*this+second_matrix;
	}
	friend ostream &operator<<(ostream &stream,Sparse_Matrix a)
	{
		stream<<a.row<<"\n"<<a.col<<"\n"<<a.num<<"\n";
		for(int i=0;i<a.num;i++)
		{
			for(int j=0;j<a.sequence[i].size();j++)
			{
				stream<<a.sequence[i][j]<<" ";
			}
			stream<<"\n";
		}
		return stream;
	}
}; 
int main()
{
	Sparse_Matrix a,b;
	ifstream in1,in2;
	in1.open("A.txt");
	in2.open("B.txt");
	in1>>a.row>>a.col>>a.num;
	for(int i=0;i<a.num;i++)
	{
		vector<int> tmp;
		for(int j=0;j<3;j++)
		{
			int t;
			in1>>t;
			tmp.push_back(t);
		}
		a.sequence.push_back(tmp);
	}
	in2>>b.row>>b.col>>b.num;
	for(int i=0;i<b.num;i++)
	{
		vector<int> tmp;
		for(int j=0;j<3;j++)
		{
			int t;
			in2>>t;
			tmp.push_back(t);
		}
		b.sequence.push_back(tmp);
	}
	in1.close();
	in2.close();
	ofstream out;
	out.open("res_A+B.txt");
	out<<a+b;
	out.close();
}
