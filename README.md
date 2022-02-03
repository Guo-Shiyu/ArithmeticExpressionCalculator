# 算术表达式处理及计算
 将算术表达式解析为语法树, 并支持自定义Pass对语法树进行分析和变换.   
 
 作为学习编译器前端技术及 modern cpp 过程中的练习. 

# 简述 
 + 近似于生成器/迭代器的词法、语法分析器
 + LL(1) 递归下降 Parser
 + 语法树的节点基于 ADT （variant）实现
 + 非 继承/虚函数 实现的Pass系统, <!-- 类似于 简化的 llvm 中的 New Pass System. 支持自定义Pass及调整Pass流程, --> 提供了操作语法树的接口以及在各个Pass开始结束阶段的钩子  
 + 默认提供 `HelloPass`、 `格式化`、 `求值`、 `积化和差` 三个Pass, 位于 `operations.h/cpp` 中, 以展示 Pass的工作流程 和对 AST 的 遍历/分析/变换 操作. 

# 测试样例
 参见 `test` 目录, 默认情况下编译执行 ArithmeticExpressionCalculator.cpp 将自动寻找 `test` 目录并进行测试. 

# EBNF 
 算术表达式的 EBNF 来自于 [这里](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/19_writing_recursive_descent_parser.html) .
 
 ~~~ ebnf 
 expr ::= term { ("+"|"-") term }*
 
 term ::= factor { ( "*" | "/" ) factor }*

 factor ::= "(" expr ")" | NUM 
 ~~~ 
 为了便于实现， 数字字面量只支持整数， 不支持浮点数

# 编译器要求
 由于使用了 `C++20` 中的 concept 与 format 特性,因此需要使用支持 C++20 标准的编译器.  
 + 通过 VS 打开 `.sln` 文件直接使用, 要求 `MSVC v142` 以及 语言标准 `/std=latest`
 + TODO: xmake/cmake + Clang 构建 
 