[te_num dim] = size(test);

for i=1:dim
    max_=max(train(:,i));
    min_=min(train(:,i));
    test(:,i) = (2*test(:,i)-(min_+max_)*ones(te_num,1))/(max_-min_);
end


test_input = [ones(te_num,1) test];

hl1_input = test_input*w;
hl1_output = [ones(te_num,1) f(hl1_input)];%add bias
hl2_input = hl1_output*W1;
hl2_output = [ones(te_num,1) f(hl2_input)];%add bias
ol_input = hl2_output*W2;
ol_output = ol_input; 
ans = round(ol_output);
fid=fopen('ans.csv','wt');
fprintf(fid,'%g\n',result);
fclose(fid);