%system HandleData.exe;
load data.txt;
load cnt.txt;
% load vail_data.txt;
% load vail_cnt.txt
%����Ԥ���� ȥ����
[row,col]=size(data);
for i=2:row
   if(data(i,33)-data(i-1,33)>=20) data(i,33)=data(i-1,33);
   end
end
%��һ��
% [row2,col2]=size(vail_data);
for i=1:col
    if(max(data(:,i))==min(data(:,i))) data(:,i)=ones(row,1);
    else data(:,i)=(data(:,i)-min(data(:,i)))./(max(data(:,i))-min(data(:,i)));
    end
%     if(max(vail_data(:,i))==min(vail_data(:,i))) vail_data(:,i)=ones(row2,1);
%     else vail_data(:,i)=(vail_data(:,i)-min(vail_data(:,i)))./(max(vail_data(:,i))-min(vail_data(:,i)));
%     end
end

p1=11;%��һ�����ز�ڵ����
p2=24;%�ڶ������ز�ڵ����
maxnum_iterations=500;%����������
alpha=0.002/row;%ѧϰ��
w1=rand(col,p1)-0.5;%����㵽��һ�����ز��Ȩ���ݶ�
b1=rand(1,p1)-0.5;
w2=rand(p1,p2)-0.5;%��һ�����ز㵽�ڶ������ز��Ȩ���ݶ�
b2=rand(1,p2)-0.5;
w3=rand(1,p2)-0.5;%�ڶ������ز㵽������Ȩ���ݶ�
b3=rand(1,1)-0.5;
mse_train=zeros(1,maxnum_iterations);%ѵ����
% mse_vail=zeros(1,maxnum_iterations);%��֤��
    for iterators=1:maxnum_iterations %��������
        %�������
        hiddenInput1=data*w1+b1;%row*p1
        hiddenOutput1=1./(1+exp(-1*hiddenInput1));%row*p1
        hiddenInput2=hiddenOutput1*w2+b2;%row*p2
        hiddenOutput2=1./(1+exp(-1*hiddenInput2));%row*p2
        finalOutput=hiddenOutput2*w3'+b3;%row*1
        %�������
        err3=cnt-finalOutput;%row*1
        err2=err3*w3.*hiddenOutput2.*(1-hiddenOutput2);%row*p2
        err1=err2*w2'.*hiddenOutput1.*(1-hiddenOutput1);%row*p1
        %����Ȩ��
        w3=w3+alpha*err3'*hiddenOutput2;%1*p2 Ȩ������
        b3=b3+alpha*err3'*ones(row,1);%1*1 ƫ��
        w2=w2+alpha*hiddenOutput1'*err2;%p1*p2 Ȩ������
        b2=b2+alpha*ones(1,row)*err2;%1*p2 ƫ��
        w1=w1+alpha*data'*err1;%col*p1 Ȩ������
        b1=b1+alpha*ones(1,row)*err1;%1*p1 ƫ��
        %ѵ����Ԥ��
        hiddenInput1_train=data*w1+b1;%row*p1
        hiddenOutput1_train=1./(1+exp(-1*hiddenInput1_train));%row*p1
        hiddenInput2_train=hiddenOutput1_train*w2+b2;%row*p2
        hiddenOutput2_train=1./(1+exp(-1*hiddenInput2_train));%row*p2
        finalOutput_train=hiddenOutput2_train*w3'+b3;%row*1
        mse_train(1,iterators)=sum((finalOutput_train-cnt).^2)/row;
        %��֤��Ԥ��
    %     hiddenInput1_vail=vail_data*w1+b1;%row*p1
    %     hiddenOutput1_vail=1./(1+exp(-1*hiddenInput1_vail));%row*p1
    %     hiddenInput2_vail=hiddenOutput1_vail*w2+b2;%row*p2
    %     hiddenOutput2_vail=1./(1+exp(-1*hiddenInput2_vail));%row*p2
    %     finalOutput_vail=hiddenOutput2_vail*w3'+b3;%row*1
    %     mse_vail(1,iterators)=sum((finalOutput_vail-vail_cnt).^2)/row2;
        %��ʾ��������
         disp(iterators);
    end


x=1:1:maxnum_iterations;
plot(x,mse_train,'b');
% rmse=get_ans(w1,b1,w2,b2,w3,b3);
% disp(rmse);
% hold on;
% plot(x,mse_vail,'r');
% legend ѵ���� ��֤��;
