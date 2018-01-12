依次运行：

HandleData.cpp 用于处理原数据集并生成处理后的文本

test_mul.m BPNN建模，在验证集上预测结果

test_continue.m 如果对test_mul.m得到的RMSE不满意的话，可以在不清空matlab工作区的情况下运行这份代码，这样就相当于在之前的结果上继续迭代

getans.m 预测测试集结果，同样要求不清空matlab工作区

