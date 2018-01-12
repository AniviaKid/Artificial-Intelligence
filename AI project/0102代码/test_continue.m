
lr = 0.0009;
epochs = 1000;
for i=1:1000 % iterations
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
    plot([1:1000],mse_tr);
    hold on;
    plot([1:1000],mse_val);
    legend('trainset','validationset');
    title('mean-square error');
    xlabel('iterations');

    subplot(2,1,2);
    plot([1:1000],cor_tr);
    hold on;
    plot([1:1000],cor_val);
    title('cov');
    xlabel('iterations');
    legend('trainset','validationset');

