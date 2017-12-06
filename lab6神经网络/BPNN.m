system handle_data.exe;
clear all;
clc;
%��ȡ����
load data.txt;
load cnt.txt;
load vail_data.txt;
load vail_cnt.txt
load dec_data.txt;
load dec_cnt.txt;
load test.txt;
%��һ��
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
p=100;%���ز�ڵ����
maxnum_iterations=500;%����������
alpha=0.009/row;%ѧϰ��
error_goal=0.002;%�����ֵ
w1=rand(col,p)-0.5;%����㵽���ز��Ȩ���ݶ�
b1=rand(1,p)-0.5;
w2=rand(1,p)-0.5;%���ز㵽������Ȩ���ݶ�
b2=rand(1,1)-0.5;
mse=zeros(1,maxnum_iterations);%ѵ����
mse2=zeros(1,maxnum_iterations);%��֤��
% w1=[0.2,-0.3;0.4,0.1;-0.5,0.2];
% w2=[-0.3,-0.2];
% b1=[-0.4,0.2];
% b2=0.1;

for iterators=1:maxnum_iterations %��������
    w2_tmp=w2;
    w1_tmp=w1;
    b1_tmp=b1;
    b2_tmp=b2;
    for i=1:row %������������
        %�������
        hiddenInput=data(i,:)*w1_tmp+b1_tmp;% 1*p
        hiddenOutput=1./(1+exp(-1*hiddenInput)); % 1*p
        finalOutput=hiddenOutput*w2_tmp'+b2_tmp;
        %�������
        err2=cnt(i)-finalOutput;
        err1=err2*hiddenOutput.*(1-hiddenOutput).*w2_tmp; %���ز���� 1*p
        %����Ȩ��
        w2=w2+alpha*err2*hiddenOutput;
        w1=w1+alpha*data(i,:)'*err1;
        b2=b2+alpha*err2;
        b1=b1+alpha*err1;
    end
    for i=1:row %ѵ����Ԥ��
        hiddenInput_new=data(i,:)*w1+b1;
        hiddenOutput_new=1./(1+exp(-1*hiddenInput_new));
        finalOutput_new=hiddenOutput_new*w2'+b2;
        mse(1,iterators)=mse(1,iterators)+(finalOutput_new-cnt(i))*(finalOutput_new-cnt(i));
    end 
    for i=1:row2 %��֤��Ԥ��
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
legend ��֤�� ѵ����;

predict_dec=zeros(1,row3);
for i=1:row3 %Ԥ��ʮ��������ʮ���cnt
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
legend ��ʵֵ Ԥ��ֵ;

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