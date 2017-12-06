clear all;
clc;
%读取数据
load data.txt;
load cnt.txt;
%归一化
[row,col]=size(data);
for i=1:col
    data(:,i)=(data(:,i)-min(data(:,i))+1)./(max(data(:,i))-min(data(:,i))+1);
end
cnt=(cnt-min(cnt)+1)./(max(cnt)-min(cnt)+1);
p=100;%隐藏层节点个数
maxnum_iterations=500;%最大迭代次数
alpha=0.009/row;%学习率
error_goal=0.002;%误差阈值
w1=rand(col,p)-0.5;%输入层到隐藏层的权重梯度
b1=rand(1,p);
w2=rand(1,p)-0.5;%隐藏层到输出层的权重梯度
b2=rand(1,1);
mse=zeros(1,maxnum_iterations);
% w1=[0.2,-0.3;0.4,0.1;-0.5,0.2];
% w2=[-0.3,-0.2];
% b1=[-0.4,0.2];
% b2=0.1;

for iterators=1:maxnum_iterations %迭代次数
    w2_tmp=w2;
    w1_tmp=w1;
    b1_tmp=b1;
    b2_tmp=b2;
    for i=1:row %遍历所有样本
        %正向遍历
        hiddenInput=data(i,:)*w1_tmp+b1_tmp;% 1*p
        hiddenOutput=1./(1+exp(-1*hiddenInput)); % 1*p
        finalOutput=hiddenOutput*w2_tmp'+b2_tmp;
        %反向遍历
        err2=cnt(i)-finalOutput;
        err1=err2*hiddenOutput.*(1-hiddenOutput).*w2_tmp; %隐藏层误差 1*p
        %更新权重
        w2=w2+alpha*err2*hiddenOutput;
        w1=w1+alpha*data(i,:)'*err1;
        b2=b2+alpha*err2;
        b1=b1+alpha*err1;
    end
    for i=1:row
        hiddenInput_new=data(i,:)*w1+b1;
        hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
        finalOutput_new=hiddenOutput_new*w2'+b2;
        mse(1,iterators)=mse(1,iterators)+0.5*(finalOutput_new-cnt(i))*(finalOutput_new-cnt(i));
    end
end

x=1:1:maxnum_iterations;
plot(x,mse);
