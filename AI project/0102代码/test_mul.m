clear all;
%train = csvread('newtrain.csv');%read data
%test = csvread('newtest.csv');


load data.txt;
load test.txt;
train = data;
test = test;
[train_num dim] = size(train);%get number of row or col 
[te_num dim] = size(test);

%?¤@¤Æ¨ì[-1,1]ªº?šV
for i=1:dim
    max_=max(train(:,i));
    min_=min(train(:,i));
    train(:,i) = (2*train(:,i)-(min_+max_)*ones(train_num,1))/(max_-min_);
    test(:,i) = (2*test(:,i)-(min_+max_)*ones(te_num,1))/(max_-min_);
end

%¥[¤W°¾¸m

train_input = [ones(train_num,1) train(:,1:dim)];
train_label = train(:,dim+1);
test_input = [ones(te_num,1) test];

dim = dim+1;


%divide trainset and validationset
indices = crossvalind('Kfold', train_num, 5);%5§é¥æ¤e??
val = (indices == 5);
val_input = train_input(val,:);
val_label = train_label(val,:);
tr = ~val;
tr_input = train_input(tr,:);
tr_label = train_label(tr,:);


[tr_num dim] =  size(tr_input);
[val_num dim] = size(val_input);
%initial

%tr_input = [1 1 0 1;1 1 1 0;1 0 0 1];
%tr_label = [1;1;0];
%dim = 4;
%tr_num = 3;

epochs = 5000;%iterations
lr = 0.0009;%learning rate
nodeNum1 = 50;% nodenum of hidden layer
nodeNum2 = 50;% nodenum of hidden layer

w =rand(dim,nodeNum1);%input layer to hidden layer1
W1 = rand(nodeNum1+1,nodeNum2);%hidden layer1 to hidden layer2
W2 = rand(nodeNum2+1,1);%hidden layer2 to output layer

%w = [-0.4 0.2;0.2 -0.3;0.4 0.1;-0.5 0.2];%TEST
%W = [0.1;-0.3;-0.2];%TEST
detw = zeros(dim,nodeNum1);
detW1 = zeros(nodeNum1+1,nodeNum2);
detW2 = zeros(nodeNum2+1,1);

f=@(x)sigmf(x,[1 0]); %activation function
detf=@(x)(sigmf(x,[1 0]).*(1-sigmf(x,[1 0])));

for i=1:epochs % iterations
     %forward propagation
     hl1_input = tr_input*w;
     hl1_output = [ones(tr_num,1) f(hl1_input)];%add bias
     hl2_input = hl1_output*W1;
     hl2_output = [ones(tr_num,1) f(hl2_input)];%add bias
     ol_input = hl2_output*W2;
     ol_output = ol_input; 
     %back
     %getErr
     ol_Err = (tr_label-ol_output);
     hl2_Err = (ol_Err*W2').*[ones(tr_num,1) detf(hl2_input)];
     hl1_Err = (hl2_Err(:,2:nodeNum2+1)*W1').*[ones(tr_num,1) detf(hl1_input)];
     %update
     detW2 = (hl2_output'*ol_Err);
     detW1 = (hl1_output'*hl2_Err(:,2:nodeNum2+1));%(get rid of bias) 
     detw =  (tr_input'*hl1_Err(:,2:nodeNum1+1));%(get rid of bias) 
     W2 = W2 + lr/tr_num*detW2;
     W1 = W1 + lr/tr_num*detW1;
     w = w + lr/tr_num*detw;
     %mse%cov of trainset
     mse_tr(i) = mean((tr_label-ol_output).*(tr_label-ol_output));
     s = corrcoef(tr_label,ol_output);
     cor_tr(i) = s(2,1);
     %validate with validation
     hl1_input = val_input*w;
     hl1_output = [ones(val_num,1) f(hl1_input)];%add bias
     hl2_input = hl1_output*W1;
     hl2_output = [ones(val_num,1) f(hl2_input)];%add bias
     ol_input = hl2_output*W2;
     ol_output = ol_input;
     mse_val(i) = mean((val_label-ol_output).*(val_label-ol_output));
     s = corrcoef(val_label,ol_output);
     cor_val(i) = s(2,1);
     
end

    subplot(2,1,1);
    plot([1:epochs],mse_tr);
    hold on;
    plot([1:epochs],mse_val);
    legend('trainset','validationset');
    title('mean-square error');
    xlabel('iterations');

    subplot(2,1,2);
    plot([1:epochs],cor_tr);
    hold on;
    plot([1:epochs],cor_val);
    title('cov');
    xlabel('iterations');
    legend('trainset','validationset');

