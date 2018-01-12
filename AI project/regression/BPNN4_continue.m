[row,col]=size(data);
maxnum_iterations=100;%����������
alpha=0.002/row;%ѧϰ��
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
% rmse=get_ans(w1,b1,w2,b2,w3,b3);
% disp(rmse);
% x=1:1:maxnum_iterations;
% plot(x,mse_train,'b');
% hold on;
% plot(x,mse_vail,'r');
% legend ѵ���� ��֤��;