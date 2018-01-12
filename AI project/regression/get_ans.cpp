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
int main(){
	ofstream out;
	out.open("final_ans.csv");
	ifstream in1,in2;
	in1.open("final_ans1.txt");
	in2.open("final_ans2.txt");
	char ch;string str;
	while ((ch = in1.get()) != EOF)
	{
		if (ch == '\n')
		{
			out <<str << endl;
			str.clear();
			continue;
		}
		str += ch;
	}
	in1.close();
	str.clear();
	while ((ch = in2.get()) != EOF)
	{
		if (ch == '\n')
		{
			out <<str << endl;
			str.clear();
			continue;
		}
		str += ch;
	}
	in2.close();
}
