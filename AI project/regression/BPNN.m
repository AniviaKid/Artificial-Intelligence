system HandleData.exe;
load data.txt;
load cnt.txt;
load vail_data.txt;
load vail_cnt.txt
%��һ��
[row,col]=size(data);
[row2,col2]=size(vail_data);
for i=1:col
    if(max(data(:,i))==min(data(:,i))) data(:,i)=ones(row,1);
    else data(:,i)=(data(:,i)-min(data(:,i)))./(max(data(:,i))-min(data(:,i)));
    end
    if(max(vail_data(:,i))==min(vail_data(:,i))) vail_data(:,i)=ones(row2,1);
    else vail_data(:,i)=(vail_data(:,i)-min(vail_data(:,i)))./(max(vail_data(:,i))-min(vail_data(:,i)));
    end
end
p=30;%���ز�ڵ����
maxnum_iterations=500;%����������
alpha=0.004/row;%ѧϰ��
w1=rand(col,p)-0.5;%����㵽���ز��Ȩ���ݶ�
b1=rand(1,p)-0.5;
w2=rand(1,p)-0.5;%���ز㵽������Ȩ���ݶ�
b2=rand(1,1)-0.5;
mse=zeros(1,maxnum_iterations);%ѵ����
mse2=zeros(1,maxnum_iterations);%��֤��

for iterators=1:maxnum_iterations %��������
    %�������
    hiddenInput=data*w1+b1; %row*p
    hiddenOutput=1./(1+exp(-1*hiddenInput)); %row*p
    finalOutput=hiddenOutput*w2'+b2; %row*1
    %�������
    err2=cnt-finalOutput; %row*1
    err1=err2*w2.*hiddenOutput.*(1-hiddenOutput); %row*p
    %����Ȩ��
    w2=w2+alpha*err2'*hiddenOutput; %1*p
    w1=w1+alpha*data'*err1; %col*p
    b2=b2+alpha*err2'*ones(row,1);
    b1=b1+alpha*ones(1,row)*err1;
    %ѵ����Ԥ��
    hiddenInput_train=data*w1+b1; %row*p
    hiddenOutput_train=1./(1+exp(-1*hiddenInput_train)); %row*p
    finalOutput_train=hiddenOutput_train*w2'+b2; %row*1
    mse(1,iterators)=sum((finalOutput_train-cnt).^2)/row;
    %��֤��Ԥ��
    hiddenInput_vail=vail_data*w1+b1; %row*p
    hiddenOutput_vail=1./(1+exp(-1*hiddenInput_vail)); %row*p
    finalOutput_vail=hiddenOutput_vail*w2'+b2; %row*1
    mse2(1,iterators)=sum((finalOutput_vail-vail_cnt).^2)/row2;
    disp(iterators);
end

x=1:1:maxnum_iterations;
plot(x,mse,'b');
hold on;
plot(x,mse2,'r');
legend ѵ���� ��֤��;