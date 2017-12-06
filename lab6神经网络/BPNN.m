system handle_data.exe;
clear all;
clc;
%读取数据
load data.txt;
load cnt.txt;
load vail_data.txt;
load vail_cnt.txt
load dec_data.txt;
load dec_cnt.txt;
load test.txt;
%归一化
[row,col]=size(data);
[row2,col2]=size(vail_data);
[row3,col3]=size(dec_data);
[row4,col4]=size(test);
for i=1:col
    data(:,i)=(data(:,i)-min(data(:,i))+1)./(max(data(:,i))-min(data(:,i))+1);
    vail_data(:,i)=(vail_data(:,i)-min(vail_data(:,i))+1)./(max(vail_data(:,i))-min(vail_data(:,i))+1);
end
%cnt=(cnt-min(cnt)+1)./(max(cnt)-min(cnt)+1);
%vail_cnt=(vail_cnt-min(vail_cnt)+1)./(max(vail_cnt)-min(vail_cnt)+1);
p=100;%隐藏层节点个数
maxnum_iterations=500;%最大迭代次数
alpha=0.009/row;%学习率
error_goal=0.002;%误差阈值
w1=rand(col,p)-0.5;%输入层到隐藏层的权重梯度
b1=rand(1,p)-0.5;
w2=rand(1,p)-0.5;%隐藏层到输出层的权重梯度
b2=rand(1,1)-0.5;
mse=zeros(1,maxnum_iterations);%训练集
mse2=zeros(1,maxnum_iterations);%验证集
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
    for i=1:row %训练集预测
        hiddenInput_new=data(i,:)*w1+b1;
        hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
        finalOutput_new=hiddenOutput_new*w2'+b2;
        mse(1,iterators)=mse(1,iterators)+(finalOutput_new-cnt(i))*(finalOutput_new-cnt(i));
    end 
    for i=1:row2 %验证集预测
        hiddenInput_new=vail_data(i,:)*w1+b1;
        hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
        finalOutput_new=hiddenOutput_new*w2'+b2;
        mse2(1,iterators)=mse2(1,iterators)+(finalOutput_new-vail_cnt(i))*(finalOutput_new-vail_cnt(i));
    end 
    disp(iterators);
end

mse=mse/row;
mse2=mse2/row2;
x=1:1:maxnum_iterations;
subplot(1,2,1);
plot(x,mse,'b');
hold on;
plot(x,mse2,'r');
legend 验证集 训练集;

predict_dec=zeros(1,row3);
for i=1:row3 %预测十二月最后二十天的cnt
    hiddenInput_new=dec_data(i,:)*w1+b1;
    hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
    finalOutput_new=hiddenOutput_new*w2'+b2;
    predict_dec(1,i)=finalOutput_new;
end
subplot(1,2,2);
x=1:1:row3;
plot(x,dec_cnt,'b');
hold on;
plot(x,predict_dec,'r');
legend 真实值 预测值;

result=zeros(row4,1);
for i=1:row3
    hiddenInput_new=test(i,:)*w1+b1;
    hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
    finalOutput_new=hiddenOutput_new*w2'+b2;
    result(i,1)=round(finalOutput_new);
end
fid=fopen('15352427_zhangzihao.txt','wt');
fprintf(fid,'%g\n',result);
fclose(fid);