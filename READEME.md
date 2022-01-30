
# EBNF 
算术表达式的 EBNF 来自于 [这里](https://python3-cookbook.readthedocs.io/zh_CN/latest/c02/p19_writing_recursive_descent_parser.html) .

~~~ ebnf 

expr ::= term { ("+"|"-") term }*

term ::= factor { ( "*" | "/" ) factor }*

factor ::= "(" expr ")" | NUM 

~~~ 

