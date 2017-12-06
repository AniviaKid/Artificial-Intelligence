IWINILAU 。UI .W.
-/Users/zhangyu/Documents/MATLAB/BPNN/BPNN.m

hl_ input=tr_input*W;
hl_output = [ones(tr_num,1) f(hl_input) ];%add bias
o1_input= hl_output*W;
%ol_output= f (ol_input );%TEST
ol_output = ol_input ;
%getErr
%ol_Err = detf (ol_input ).*(tr_label-ol_output );%TEST
o1_Err = (tr_label-ol_output );
hl_Err = (ol_Err*ones(1,nodeNum+1) ).*( [ones(tr_num,1) detf(hl_input)].*(ones(tr_num,1)*W'));
%update
detW = detW + (hl_output '*o1_Err) ;
detw = detw + (tr_input'*hl_Err( :,2:nodeNum+1) );
W = W + lr/tr_num*detW;
N = W + lr/tr_num*detw;
mse_tr( i) = mean( (tr_label-ol_output).*(tr_label-ol_output) );
%validate with validation
令行窗口
