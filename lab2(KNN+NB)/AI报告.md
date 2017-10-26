###                           中山大学数据科学与计算机学院                                                                                  移动信息工程专业-人工智能                                                                             本科生实验报告                                                                                            (2017-2018学年秋季学期)

-------

|  姓名  |    学号    | 教学班级 |          课程名称           | 专业方向  |
| :--: | :------: | :--: | :---------------------: | :---: |
| 张子豪  | 15352427 | 15M1 | Artificial Intelligence | 移动互联网 |

-----


###一、实验题目

**K近邻与朴素贝叶斯**——分类和回归

###二、实验内容

####1.算法原理

K近邻算法：

将训练集每个文本都用一个特征向量去表示(这个特征向量可以是0-1形式，也可以是tf、tf_idf等值)。对每一个待处理文本，求出该文本的特征向量与训练集每条文本的特征向量的距离，(可以是曼哈顿距离/欧式距离…也可以用余弦相似度衡量)，然后取出与该待处理文本前k个距离最相近的文本。

对于分类问题，这些被取出的文本的标签的**众数**就是K近邻算法得到的结果；对于回归问题，则要将距离作为权值进行加权，计算该文本属于某一标签的概率。



朴素贝叶斯算法：

朴素贝叶斯的前提是特征之间强独立，每个属性在判定文本的标签时的概率分布上独立。

对于分类问题，常见的有伯努利模型和多项式模型。伯努利模型中的特征只有0-1，即是否出现在文档中，多项式模型中的特征是单词，值是单词的出现次数。

以多项式模型为例，设某文档d=(t1,t2,…,tk)，tk是该文档中出现过的单词，假如要求标签为joy的概率：

根据贝叶斯定理，$$P(A|B)=\frac{P(B|A)P(A)}{P(B)}$$ ，可知$$P(joy|d)=\frac{P(d|joy)*P(joy)}{P(d)}$$ ，由于P(d)是常数，归一化后不产生影响，所以计算分子即可。

先验概率P(c)=类c下单词总数/整个训练样本的单词总数

类条件概率P(tk|c)=(类c下单词tk在各个文档中出现过的次数之和+alpha)/(类c下单词总数+不重复单词总数*alpha)，其中alpha的取值在0-1之间

则P(joy|d)=P(d|joy)\*P(joy)=P(t1|c)\*P(t2|c)\*…* P(tk|c)*P(joy)。

对于回归问题，P(joy|d)=P(t1|c,d1)* P(t2|c,d1)*…* P(tk|c,d1)* P(d1,joy)+ P(t1|c,d2)* P(t2|c,d2)*…* P(tk|c,d2)*P(d2,joy)+…，其中P(dn,joy)为先验概率。

直接套用这个公式的话，则要求训练集中某个语句拥有测试文本的所有词，否则结果均为0，而这显然是不切实际的，因此需要使用拉普拉斯平滑，将P(tk|c,di)改为$$\frac{x_k+alpha}{\sum_{k=1}^Kx_k+K*alpha}$$ ，这样为所有值都手动加了一个数，避免了乘法过程中出现0的情况，alpha的取值同样是0-1。

####2.伪代码

kNN分类：

* 处理训练集数据
  * 读取所有原始数据，去掉第一行
  * for遍历每一行，istringstream读取被空格分割的每个字符串
  * 处理读取到的最后一个字符串得到最后一个单词与对应情感值

* 获得特征向量vector

  * map+for循环，统计不重复词语

  * for 遍历训练集

    ​	for 遍历不重复词语

    ​		if(该训练文本存在这个词语)  vector.push_back(TF_IDF)

    ​		else vector.push_back(0)

* 统计每个测试文本与每个训练文本的距离

  * for 遍历测试集

    ​	for 遍历训练集

    ​		计算距离

* 预测各文本的标签

  * for 遍历合适的k

    ​	for遍历测试集，对当前这个测试文本

    ​		取训练集中前k个与该测试文本最近的文本

    ​		根据距离加权，取最大权值对应的标签

    ​		输出结果




kNN回归：

- 处理训练集数据

  - 读取所有原始数据，去掉第一行
  - for遍历每一行，istringstream读取被空格分割的每个字符串
  - 处理读取到的最后一个字符串得到最后一个单词与对应概率

- 获得特征向量vector

  - map+for循环，统计不重复词语

  - for 遍历训练集

    ​	for 遍历不重复词语

    ​		if(该训练文本存在这个词语)  vector.push_back(TF_IDF)

    ​		else vector.push_back(0)

- 统计每个测试文本与每个训练文本的距离

  - for 遍历测试集

    ​	for 遍历训练集

    ​		计算距离

- 计算测试集各文本的概率

  - for 遍历合适的k

    ​	for遍历测试集，对当前这个测试文本

    ​		取训练集中前k个与该测试文本最近的文本

    ​		for 遍历每个标签    根据距离加权，计算测试文本属于该标签的概率

    ​		归一化

    ​		输出结果



贝叶斯分类：

* 处理训练集数据
  * 读取所有原始数据，去掉第一行
  * for遍历每一行，istringstream读取被空格分割的每个字符串
  * 处理读取到的最后一个字符串得到最后一个单词与对应情感值
* 获得所有标签种类数量
  * for 遍历训练集 
  * set.insert(标签)
  * set.size()
* 获得所有不重复单词
* 获得标签-该标签下的单词总数  map\<string,int>存储
* 获得所有先验概率 P(c)= 类c下文件总数/整个训练样本的文件总数
* 预测各文本标签
  * for 遍历测试集 对当前文本
    * 去除掉训练集中不存在的词语，并相应减小句子长度
    * if(句子长度=0) 所有单词训练集都不存在，无法预测，不如joy一下
    * 根据多项式模型计算各标签概率
    * 取概率最大的标签作为预测结果



贝叶斯回归：

* 处理训练集数据
  * 读取所有原始数据，去掉第一行
  * for遍历每一行，istringstream读取被空格分割的每个字符串
  * 处理读取到的最后一个字符串得到最后一个单词与对应概率
* 获得所有不重复单词
* 计算训练集tf向量，并使用拉普拉斯平滑
* 计算测试集各文本概率
  * 去除掉训练集中不存在的词语，并相应减小句子长度
  * if(句子长度=0) 所有单词训练集都不存在，无法预测，所有标签概率六等分
  * P(joy|d)=P(t1|c,d1)\*P(t2|c,d1)*…* P(tk|c,d1)\*P(d1,joy)+ P(t1|c,d2)\*P(t2|c,d2)*…* P(tk|c,d2)*P(d2,joy)+…
  * ......
  * 计算出所有标签概率，归一化，输出结果

####3.关键代码截图(带注释)

kNN：

```
//求余弦相似度;为正且值越大则差距越小;为负则代表差距较大,负数的绝对值越大则差距越大
double dot=0;
for(int i=0;i<a.size();i++) dot+=a[i]*b[i];
double a_=0,b_=0;
for(int i=0;i<a.size();i++)
{
	a_+=a[i]*a[i];
	b_+=b[i]*b[i];
}
if(a_==0||b_==0) return 999999;//某一向量为零向量，两向量正交，无任何相似可言
return dot/sqrt(a_*b_);
```

```
//KNN分类
for(int k=1;k<=50;k++)//遍历寻找合适的k值
{
	int true_num=0;//分类正确的数量
	for(int i=0;i<verification.size();i++)//遍历验证集
	{
		map<string,double> probability_mood;//标签-该标签出现概率
		stable_sort(verification[i].distance.begin(),verification[i].distance.end(),cmp);//根据距离排序
		for(int j=0;j<k;j++)//取topK个标签，根据距离进行加权
        probability_mood[verification[i].distance[j].mood]+=verification[i].distance[j].dist;
		string ret_mood;//预测结果
		double max_ret=0;
		for(map<string,double>::iterator it=probability_mood.begin();it!=probability_mood.end();it++)//寻找概率最大的标签
		{
			if(it->second>max_ret)
			{
				max_ret=it->second;
				ret_mood=it->first;
			}
		}
		verification[i].mood_from_train=ret_mood;
		if(ret_mood==verification[i].mood) true_num++;
	}
	cout<<"k="<<k<<"  "<<"rate:"<<(double)true_num/verification.size()<<endl;
}
```

```
//KNN回归
for(int i=0;i<verification.size();i++)//对这条语句，取离他最近的k个邻居
{
	sort(verification[i].distance.begin(),verification[i].distance.end(),cmp);//根据距离排序
	for(int m=0;m<6;m++) verification[i].mood_from_train[m]=0;//遍历每种标签，并将结果初始化为0
	for(int j=0;j<k;j++)//取topK个标签
	{
		for(int m=0;m<6;m++)//遍历每种标签
		{
verification[i].mood_from_train[m]+=verification[i].distance[j].dist*verification[i].distance[j].mood[m];//由于使用余弦作为距离衡量，因此用余弦值乘概率就可以完成加权
		}
	}
	double sum=0;
	for(int m=0;m<6;m++) sum+=verification[i].mood_from_train[m];
	for(int m=0;m<6;m++)
	{
		verification[i].mood_from_train[m]/=sum;//归一化
		out<<verification[i].mood_from_train[m]<<"\t";
	}
	out<<endl;
}
```



贝叶斯：

```
//贝叶斯分类
map<string,double> probability;//标签-该标签对应概率
//对这条文本，求每种情感的概率
for(set<string>::iterator it=all_mood.begin();it!=all_mood.end();it++)//遍历每种情感
{
	for(int j=0;j<tmp.size();j++)//遍历删去训练集中不存在的单词后的句子
	{
		if(!probability[*it]) probability[*it]=(Get_num_of_times_word_occurs(*it,tmp[j])+0.37)/(num_of_mood[*it]+0.37*not_repeat_words.size());
		else probability[*it]*=(Get_num_of_times_word_occurs(*it,tmp[j])+0.37)/(num_of_mood[*it]+0.37*not_repeat_words.size());
	}
	probability[*it]*=priori_probability[*it];
}
string ret;
double max_ret=0;
for(map<string,double>::iterator it=probability.begin();it!=probability.end();it++)//寻找最大概率
{
	if(it->second>max_ret)
	{
		max_ret=it->second;
		ret=it->first;
	}
}
```

```
//贝叶斯回归
double ret[6]={0,0,0,0,0,0};
for(int c=0;c<6;c++)//遍历每种情感
{
	for(int j=0;j<train.size();j++)//当前情感下遍历训练集所有文本
	{
		double tmp1=train[j].mood[c];//先验概率
		//遍历这个文本下每个训练集出现过的单词
		for(int k=0;k<tmp.size();k++)
		{
			tmp1*=train[j].laplace[tmp[k]];
		} 
		ret[c]+=tmp1;
	}
}
double sum=0;
for(int j=0;j<6;j++) sum+=ret[j];
if(sum!=0) for(int j=0;j<6;j++) ret[j]/=sum;
for(int j=0;j<6;j++) out<<ret[j]<<"\t";
out<<endl;
```

####4.创新点 &优化(如果有)

kNN：

* 使用余弦相似度衡量距离，也就是向量夹角，不需要进行归一化，而且较欧氏距离来说，在这份训练集上的表现余弦要优秀一些。
* 使用tf_idf值作为文本的特征向量，tf_idf不仅能衡量某一特定文件内的高词语频率，也能衡量该词语在整个文件集合中的低文件频率，较one_hot而言属性要优秀的多。
* 取出topK后使用距离进行加权，取权值较大者，不取众数。这样不会被数量众多但实际上与测试样本近似于正交的训练样本干扰，我只是进行了简单的相加运算作为加权，但实际上一组余弦值为1的向量肯定要比两组余弦值为0.5的向量要准确的多的多，由于时间问题因此没有多加探索，只是实现了一个简单的思想，但饶是这样准确率也有4%的提升。
* 其实还有一些小优化，比如取出topK个标签之后，去除掉其中余弦值小于1e20的向量，也就是近似于正交的向量；排序使用stable_sort而不用sort。但是在使用上面所说的加权的方法后，这些优化对结果的提升都很小了，因此没有在代码中写出，只在这里提及一下。



贝叶斯：

* 基于多项式模型，对我建立的模型寻找合适的alpha值，这里我采用的分别是0.37与0.0001（分类与回归），准确率和相关系数都有较大提升。

###三、实验结果及分析

####1.实验结果展示示例（可图可表可文字，尽量可视化）

**kNN分类**，取k=11，预测结果如下图所示

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\分类数据集\1.png)

**kNN回归**，取k=11，预测概率如图所示

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\2.png)

**贝叶斯分类**，预测结果如下：

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\3.png)

**贝叶斯回归**，预测概率如下：

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\4.png)



####2.评测指标展示即分析（如果实验题目有特殊要求，否则使用准确率）

**kNN分类**，取k=1~50，计算准确率，可以看出k=11时达到峰值：

```
k=1  rate:0.430868
k=2  rate:0.430868
k=3  rate:0.401929
k=4  rate:0.434084
k=5  rate:0.450161
k=6  rate:0.446945
k=7  rate:0.463023
k=8  rate:0.485531
k=9  rate:0.475884
k=10  rate:0.466238
k=11  rate:0.488746
k=12  rate:0.475884
k=13  rate:0.488746
k=14  rate:0.482315
k=15  rate:0.485531
k=16  rate:0.472669
k=17  rate:0.469453
k=18  rate:0.469453
k=19  rate:0.466238
k=20  rate:0.472669
k=21  rate:0.463023
k=22  rate:0.463023
k=23  rate:0.456592
k=24  rate:0.453376
k=25  rate:0.463023
k=26  rate:0.466238
k=27  rate:0.463023
k=28  rate:0.459807
k=29  rate:0.463023
k=30  rate:0.466238
k=31  rate:0.469453
k=32  rate:0.466238
k=33  rate:0.459807
k=34  rate:0.456592
k=35  rate:0.466238
k=36  rate:0.466238
k=37  rate:0.466238
k=38  rate:0.456592
k=39  rate:0.459807
k=40  rate:0.459807
k=41  rate:0.459807
k=42  rate:0.453376
k=43  rate:0.453376
k=44  rate:0.453376
k=45  rate:0.459807
k=46  rate:0.459807
k=47  rate:0.459807
k=48  rate:0.459807
k=49  rate:0.463023
k=50  rate:0.463023
```

**kNN回归** ，同样取k=11，相关系数如下：

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\5.png)

**贝叶斯分类** ，准确率如下：

```
rate: 0.469453
```

**贝叶斯回归** ，相关系数如下：

![img](C:\Users\ASUS\Desktop\lab2(KNN+NB)\6.png)



### 四、思考题

1.同一测试样本的各个情感概率总和应该为1 如何处理？ 

答：进行归一化处理，对所有概率求和后计算每个概率的占比即可。



2.在矩阵稀疏程度不同的时候， 曼哈顿距离与欧氏距离表现有什么区别， 为什么？  

答：个人认为没有区别，也可能是我不知道。



3.伯努利模型和多项式模型分别有什么优缺点？

答：多项式模型较伯努利模型而言，能更好的衡量**单词出现次数**对模型的影响。伯努利模型只计算单词有没有出现，所以文本中重复出现的单词与只出现过一次的单词在伯努利模型中影响相同；而多项式模型计算单词出现次数，那么只出现过一次的单词相较重复出现的单词没有优势，因此就这方面而言多项式模型预测更准确。但是相应的，伯努利模型实现起来要比多项式模型容易一些。



4.如果测试集中出现了一个之前全词典中没有出现过的词该如何解决 ？

答：之前全词典中都没有出现这个词，那么这个词在基于训练集得到的模型中是无法被预测的，应该将这个词从测试集中删去。