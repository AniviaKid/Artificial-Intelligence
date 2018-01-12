load res_10W´Î.txt
leng=length(res_10W_);
x=1:1:leng;
plot(x,res_10W_)
for i=1:leng
    if(res_10W_(1,i)=max(res_10W_))
        display(i);
    end
end