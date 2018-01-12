%system HandleData.exe;
data=load('data_christmas.txt');
cnt=load('christmas_cnt.txt');
% load vail_data.txt;
% load vail_cnt.txt
%数据预处理 去噪声
[row,col]=size(data);
for i=2:row
   if(data(i,33)-data(i-1,33)>=20) data(i,33)=data(i-1,33);
   end
end
%归一化
% [row2,col2]=size(vail_data);
for i=1:col
    if(max(data(:,i))==min(data(:,i))) data(:,i)=ones(row,1);
    else data(:,i)=(data(:,i)-min(data(:,i)))./(max(data(:,i))-min(data(:,i)));
    end
%     if(max(vail_data(:,i))==min(vail_data(:,i))) vail_data(:,i)=ones(row2,1);
%     else vail_data(:,i)=(vail_data(:,i)-min(vail_data(:,i)))./(max(vail_data(:,i))-min(vail_data(:,i)));
%     end
end

p1=30;%第一层隐藏层节点个数
p2=125;%第二层隐藏层节点个数
maxnum_iterations=250;%最大迭代次数
alpha=0.002/row;%学习率
w1=rand(col,p1)-0.5;%输入层到第一层隐藏层的权重梯度
b1=rand(1,p1)-0.5;
w2=rand(p1,p2)-0.5;%第一层隐藏层到第二层隐藏层的权重梯度
b2=rand(1,p2)-0.5;
w3=rand(1,p2)-0.5;%第二层隐藏层到输出层的权重梯度
b3=rand(1,1)-0.5;
mse_train=zeros(1,maxnum_iterations);%训练集
% mse_vail=zeros(1,maxnum_iterations);%验证集

    for iterators=1:maxnum_iterations %迭代次数
        %正向遍历
        hiddenInput1=data*w1+b1;%row*p1
        hiddenOutput1=1./(1+exp(-1*hiddenInput1));%row*p1
        hiddenInput2=hiddenOutput1*w2+b2;%row*p2
        hiddenOutput2=1./(1+exp(-1*hiddenInput2));%row*p2
        finalOutput=hiddenOutput2*w3'+b3;%row*1
        %反向遍历
        err3=cnt-finalOutput;%row*1
        err2=err3*w3.*hiddenOutput2.*(1-hiddenOutput2);%row*p2
        err1=err2*w2'.*hiddenOutput1.*(1-hiddenOutput1);%row*p1
        %更新权重
        w3=w3+alpha*err3'*hiddenOutput2;%1*p2 权重向量
        b3=b3+alpha*err3'*ones(row,1);%1*1 偏置
        w2=w2+alpha*hiddenOutput1'*err2;%p1*p2 权重向量
        b2=b2+alpha*ones(1,row)*err2;%1*p2 偏置
        w1=w1+alpha*data'*err1;%col*p1 权重向量
        b1=b1+alpha*ones(1,row)*err1;%1*p1 偏置
        %训练集预测
        hiddenInput1_train=data*w1+b1;%row*p1
        hiddenOutput1_train=1./(1+exp(-1*hiddenInput1_train));%row*p1
        hiddenInput2_train=hiddenOutput1_train*w2+b2;%row*p2
        hiddenOutput2_train=1./(1+exp(-1*hiddenInput2_train));%row*p2
        finalOutput_train=hiddenOutput2_train*w3'+b3;%row*1
        mse_train(1,iterators)=sum((finalOutput_train-cnt).^2)/row;
        %验证集预测
    %     hiddenInput1_vail=vail_data*w1+b1;%row*p1
    %     hiddenOutput1_vail=1./(1+exp(-1*hiddenInput1_vail));%row*p1
    %     hiddenInput2_vail=hiddenOutput1_vail*w2+b2;%row*p2
    %     hiddenOutput2_vail=1./(1+exp(-1*hiddenInput2_vail));%row*p2
    %     finalOutput_vail=hiddenOutput2_vail*w3'+b3;%row*1
    %     mse_vail(1,iterators)=sum((finalOutput_vail-vail_cnt).^2)/row2;
        %显示迭代次数
         disp(iterators);
    end 
   
%  rmse=get_ans_christmas(w1,b1,w2,b2,w3,b3);
%  disp(rmse);

% x=1:1:maxnum_iterations;
% plot(x,mse_train,'b');
% hold on;
% plot(x,mse_vail,'r');
% legend 训练集 验证集;