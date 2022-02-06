# 算术表达式处理及计算
 将算术表达式解析为语法树, 并支持自定义Pass对语法树进行分析和变换. 

# 简述 
 + 近似于生成器/迭代器的词法、语法分析器
 + LL(1) 递归下降 Parser
 + 语法树的节点基于 ADT （variant）实现
 + 非 继承/虚函数 实现的Pass系统, <!-- 类似于 简化的 llvm 中的 New Pass System. 支持自定义Pass及调整Pass流程, --> 提供了操作语法树的接口以及在各个Pass开始结束阶段的钩子  
 + 默认提供 `HelloPass`、 `格式化`、 `求值`、 `积化和差` 三个Pass, 位于 `operations.h/cpp` 中, 以展示 Pass的工作流程 和对 AST 的 遍历/分析/变换 操作. 

+  其中算术表达式的 EBNF 来自于 [这里](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/19_writing_recursive_descent_parser.html) .  为了便于实现， 数字字面量只支持整数， 不支持浮点数。  
 
 ~~~ ebnf 
 expr ::= term { ("+"|"-") term }*
 
 term ::= factor { ( "*" | "/" ) factor }*

 factor ::= "(" expr ")" | NUM 
 ~~~ 


# Build
 由于使用了 `C++20` 中的 `concept` 与 `format` 特性,因此需要使用支持 C++20 标准的编译器及标准库.    
 
 可用方式：
 + 通过 VS 打开 `.sln` 文件直接使用, 要求 `MSVC >= v142` 以及 语言标准 `/std=latest`
 + TODO: xmake/cmake + Clang 构建 

# 测试样例
 参见 `test` 目录, 默认情况下编译执行 `Main.cpp` 将自动寻找 `test` 目录并测试目录下所有文件.   
  
 注意：  
 + 在 `MSVC` 默认栈大小的情况下， `z_InvalidExpr` 中最后一项测试将会导致栈溢出
 + `divide by zero` 异常不会被捕获， 因此未被列入测试     

默认情况下输出如下:
 ~~~
file: Basic :
=================
raw expr:|1 + 2|
Dump:|1 + 2|
Eval:3

raw expr:|1 - 3 - 4 - 5 + 6|
Dump:|1 - 3 - 4 - 5 + 6|
Eval:-5

raw expr:|5 * 6|
Dump:|5 * 6|
Eval:30

raw expr:|12 / 3|
Dump:|12 / 3|
Eval:4

raw expr:|5 * 6 * 12 / 4 / 3 |
Dump:|5 * 6 * 12 / 4 / 3|
Eval:30

raw expr:|( 1 )|
Dump:|( 1 )|
Eval:1

raw expr:|( 2 + 3 )|
Dump:|( 2 + 3 )|
Eval:5

raw expr:|( 5 / 5 )|
Dump:|( 5 / 5 )|
Eval:1

raw expr:|( 2 + 3 - 4 )|
Dump:|( 2 + 3 - 4 )|
Eval:1

raw expr:|( 2 + 3 * 4 )|
Dump:|( 2 + 3 * 4 )|
Eval:14

raw expr:|( 2 + 3 * 5 / 6 * 4)|
Dump:|( 2 + 3 * 5 / 6 * 4 )|
Eval:10

raw expr:|1 - ( 3 + 4 )|
Dump:|1 - ( 3 + 4 )|
Eval:-6

raw expr:|2 + ( 2 * 4 )|
Dump:|2 + ( 2 * 4 )|
Eval:10

raw expr:|1 * ( 2 + 3 - 4 ) / ( 4 + 2 )|
Dump:|1 * ( 2 + 3 - 4 ) / ( 4 + 2 )|
Eval:0

raw expr:|2 * ( 7 * 8 / 2) * ( 12 - 5 + 5 )|
Dump:|2 * ( 7 * 8 / 2 ) * ( 12 - 5 + 5 )|
Eval:672

raw expr:|((4 * 3) * 2 ) + 4|
Dump:|( ( 4 * 3 ) * 2 ) + 4|
Eval:28

raw expr:|((100 - 100) * 0) + 4|
Dump:|( ( 100 - 100 ) * 0 ) + 4|
Eval:4

raw expr:|((((3 + 2) - 2) + 5) * 9) * 10|
Dump:|( ( ( ( 3 + 2 ) - 2 ) + 5 ) * 9 ) * 10|
Eval:720

file: BigNumber :
=================
raw expr:|10187246 + 99999999|
Dump:|10187246 + 99999999|
Eval:110187245

raw expr:|(192831 * 9123) + 88888812381234|
Dump:|( 192831 * 9123 ) + 88888812381234|
Eval:88890571578447

raw expr:|99999999999999999999|
error:stoull argument out of range

file: LongExpression :
=================
raw expr:|((((((((((((((((((((((((((1 + 2))))))))))))))))))))))))))|
Dump:|( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( 1 + 2 ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) )|
Eval:3

raw expr:|(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((( 1 ))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))|
Dump:|( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( 1 ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) )|
Eval:1

file: UnexactlyDivide :
=================
raw expr:|1 / 2|
Dump:|1 / 2|
Eval:0

raw expr:|3 / 1|
Dump:|3 / 1|
Eval:3

raw expr:|4 / 1|
Dump:|4 / 1|
Eval:4

raw expr:|5 / 1|
Dump:|5 / 1|
Eval:5

raw expr:|4 / 5|
Dump:|4 / 5|
Eval:0

raw expr:|5 / 4|
Dump:|5 / 4|
Eval:1

raw expr:|4 / 2|
Dump:|4 / 2|
Eval:2

raw expr:|0 / 1|
Dump:|0 / 1|
Eval:0

raw expr:|0 / 17|
Dump:|0 / 17|
Eval:0

file: Unformat :
=================
raw expr:|000000123|
Dump:|123|
Eval:123

raw expr:|1/2|
Dump:|1 / 2|
Eval:0

raw expr:|(                                   1 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 0)       - 4|
Dump:|( 1 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 0 ) - 4|
Eval:6

file: z_InvalidExpr :
=================
raw expr:|0999999999999999999999999999|
error:stoull argument out of range

raw expr:|1)|
error:unfinished parsing with unmatched token: )

raw expr:|1 +|
error:unexpected eof

raw expr:|2 *|
error:unexpected eof

raw expr:|1 + +|
error:'number' or '(' expected here, but + was found

raw expr:|1 * /|
error:'number' or '(' expected here, but / was found

raw expr:|2 / 2 * (|
error:unexpected eof

raw expr:|2 * 3 + ()|
error:'number' or '(' expected here, but ) was found

raw expr:|)|
error:'number' or '(' expected here, but ) was found

raw expr:|(|
error:unexpected eof

raw expr:|()|
error:'number' or '(' expected here, but ) was found

raw expr:|(((())))|
error:'number' or '(' expected here, but ) was found

raw expr:|4 / abcd|
error:unexpected characher in source: a

raw expr:|1+ 2)|
error:unfinished parsing with unmatched token: )

raw expr:|(1 + 2|
error:')' expected here, but EOF was found

 ~~~

 