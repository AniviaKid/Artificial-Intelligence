%function rmse=get_ans_christmas(w1,b1,w2,b2,w3,b3)
test=load('test_chrismas.txt');
[row_test,col_test]=size(test);
for i=2:row_test
   if(test(i,33)-test(i-1,33)>=20) test(i,33)=test(i-1,33);
   end
end
for i=1:col_test
    if(max(test(:,i))==min(test(:,i))) test(:,i)=ones(row_test,1);
    else test(:,i)=(test(:,i)-min(test(:,i)))./(max(test(:,i))-min(test(:,i)));
    end
end
hiddenInput1_test=test*w1+b1;%row*p1
hiddenOutput1_test=1./(1+exp(-1*hiddenInput1_test));%row*p1
hiddenInput2_test=hiddenOutput1_test*w2+b2;%row*p2
hiddenOutput2_test=1./(1+exp(-1*hiddenInput2_test));%row*p2
finalOutput_test=hiddenOutput2_test*w3'+b3;%row*1
ans_test=round(finalOutput_test);
test_cnt=load('test_true_chrimas.txt');
rmse=sqrt(sum((ans_test-test_cnt).^2)/row_test);
disp(rmse);
fid=fopen('final_ans2.txt','wt');
fprintf(fid,'%g\n',ans_test);
fclose(fid);