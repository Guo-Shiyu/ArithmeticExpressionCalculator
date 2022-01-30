# ArithmeticExpressionCalculator
 将算术表达式解析为语法树， 并支持自定义Pass对语法树进行分析和变换。  
 
 注意: 由于使用了一部分 C++20 的标准库与特性， 因此在较低版本的编译器上可能无法通过编译

# 支持的编译器
 
# EBNF 
 算术表达式的 EBNF 来自于 [这里](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/19_writing_recursive_descent_parser.html) .
 
 ~~~ ebnf 
 expr ::= term { ("+"|"-") term }*
 
 term ::= factor { ( "*" | "/" ) factor }*

 factor ::= "(" expr ")" | NUM 
 ~~~ 


