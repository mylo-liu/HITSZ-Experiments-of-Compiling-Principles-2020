# HITSZ-Experiments-of-Compiling-Principles-2020
哈工大深圳-编译原理实验-2020



## 实验一：**词法分析**

**实验内容：**

（1）编写一个词法分析程序，读取文件，对文件内自定义的C语言程序段进行词法分析

（2）输出以文件形式存放的TOKEN串和符号表

（3）要求所输入的C语言程序段包含常见的关键字，标识符，常数，运算符和分界符等



**输入文件test.txt:**

```
int var1 = 24;
int var2 = 38;
int n = 55;
int i;
for(i=0; i<n; i++){
    var2 += 9;
    if (var1 > var2)
    {
        break;
    }
}
```

**输出文件token.txt：**

```
( 17 , int )
( 70 , var1 )
( 54 , = )
( 66 , 24 )
( 61 , ; )
( 17 , int )
( 70 , var2 )
( 54 , = )
( 66 , 38 )
( 61 , ; )
( 17 , int )
( 70 , n )
( 54 , = )
( 66 , 55 )
( 61 , ; )
( 17 , int )
( 70 , i )
( 61 , ; )
( 14 , for )
( 31 , ( )
( 70 , i )
( 54 , = )
( 66 , 0 )
( 61 , ; )
( 70 , i )
( 56 , < )
( 70 , n )
( 61 , ; )
( 70 , i )
( 47 , ++ )
( 32 , ) )
( 64 , { )
( 70 , var2 )
( 49 , += )
( 66 , 9 )
( 61 , ; )
( 16 , if )
( 31 , ( )
( 70 , var1 )
( 55 , > )
( 70 , var2 )
( 32 , ) )
( 64 , { )
( 2 , break )
( 61 , ; )
( 65 , } )
( 65 , } )
```

**输出文件table.txt：**

```
( var1 , -1 , ? )
( var2 , -1 , ? )
( n , -1 , ? )
( i , -1 , ? )
```



## 实验二：语法分析

**实验内容：**

（1）利用 LR(1)分析法，设计一个语法分析程序，对输入符号串进行语法分析

（2）输出推导过程中所用产生式序列并保存在输出文件中

**输入待分析代码：**

```
ID = ID + NUM * ID - ( ID + NUM ) $
```

**输入LR1分析表：**

```
,=,+,-,*,/,(,),ID,NUM,int,$,S,A,B,C,S'
0,,,,,,,,shift 2,,shift 3,,,,,,1
1,,,,,,,,,,,accept,,,,,
2,shift 4,,,,,,,,,,,,,,,
3,,,,,,,,shift 5,,,,,,,,
4,,,,,,shift 9,,shift 10,shift 11,,,6,7,8,,
5,,,,,,,,,,,reduce S' -> int ID,,,,,
6,,shift 12,shift 13,,,,,,,,reduce S' -> ID = S,,,,,
7,,reduce S -> A,reduce S -> A,shift 14,shift 15,,,,,,reduce S -> A,,,,,
8,,reduce A -> B,reduce A -> B,reduce A -> B,reduce A -> B,,,,,,reduce A -> B,,,,,
9,,,,,,shift 19,,shift 20,shift 21,,,16,17,18,,
10,,reduce B -> ID,reduce B -> ID,reduce B -> ID,reduce B -> ID,,,,,,reduce B -> ID,,,,,
11,,reduce B -> NUM,reduce B -> NUM,reduce B -> NUM,reduce B -> NUM,,,,,,reduce B -> NUM,,,,,
12,,,,,,shift 9,,shift 10,shift 11,,,,22,8,,
13,,,,,,shift 9,,shift 10,shift 11,,,,23,8,,
14,,,,,,shift 9,,shift 10,shift 11,,,,,24,,
15,,,,,,shift 9,,shift 10,shift 11,,,,,25,,
16,,shift 26,shift 27,,,,shift 28,,,,,,,,,
17,,reduce S -> A,reduce S -> A,shift 29,shift 30,,reduce S -> A,,,,,,,,,
18,,reduce A -> B,reduce A -> B,reduce A -> B,reduce A -> B,,reduce A -> B,,,,,,,,,
19,,,,,,shift 19,,shift 20,shift 21,,,31,17,18,,
20,,reduce B -> ID,reduce B -> ID,reduce B -> ID,reduce B -> ID,,reduce B -> ID,,,,,,,,,
21,,reduce B -> NUM,reduce B -> NUM,reduce B -> NUM,reduce B -> NUM,,reduce B -> NUM,,,,,,,,,
22,,reduce S -> S + A,reduce S -> S + A,shift 14,shift 15,,,,,,reduce S -> S + A,,,,,
23,,reduce S -> S - A,reduce S -> S - A,shift 14,shift 15,,,,,,reduce S -> S - A,,,,,
24,,reduce A -> A * B,reduce A -> A * B,reduce A -> A * B,reduce A -> A * B,,,,,,reduce A -> A * B,,,,,
25,,reduce A -> A / B,reduce A -> A / B,reduce A -> A / B,reduce A -> A / B,,,,,,reduce A -> A / B,,,,,
26,,,,,,shift 19,,shift 20,shift 21,,,,32,18,,
27,,,,,,shift 19,,shift 20,shift 21,,,,33,18,,
28,,reduce B -> ( S ),reduce B -> ( S ),reduce B -> ( S ),reduce B -> ( S ),,,,,,reduce B -> ( S ),,,,,
29,,,,,,shift 19,,shift 20,shift 21,,,,,34,,
30,,,,,,shift 19,,shift 20,shift 21,,,,,35,,
31,,shift 26,shift 27,,,,shift 36,,,,,,,,,
32,,reduce S -> S + A,reduce S -> S + A,shift 29,shift 30,,reduce S -> S + A,,,,,,,,,
33,,reduce S -> S - A,reduce S -> S - A,shift 29,shift 30,,reduce S -> S - A,,,,,,,,,
34,,reduce A -> A * B,reduce A -> A * B,reduce A -> A * B,reduce A -> A * B,,reduce A -> A * B,,,,,,,,,
35,,reduce A -> A / B,reduce A -> A / B,reduce A -> A / B,reduce A -> A / B,,reduce A -> A / B,,,,,,,,,
36,,reduce B -> ( S ),reduce B -> ( S ),reduce B -> ( S ),reduce B -> ( S ),,reduce B -> ( S ),,,,,,,,,
```

**输出产生式序列：**

```
B->ID
A->B
S->A
B->NUM
A->B
B->ID
A->A*B
S->S+A
B->ID
A->B
S->A
B->NUM
A->B
S->S+A
B->(S)
A->B
S->S-A
S'->ID=S
```



## 实验三	语义分析及中间代码生成

**实验内容**

（1）针对自顶向下分析法或者自底向上分析法（二选一）中所使用的文法，在完成实验二（语法分析）的基础上为语法正确的单词串其设计翻译方案，完成语法制导翻译。

（2）利用该翻译方案，对所给程序段，进行分析，输出生成中间代码序列和符号表，并保存在相应文件中。

（3）中间代码可选三地址码或四元式

**输入：简单的赋值语句，默认每个常量和变量均为整形数。**

```
ret = a + 18 * b - ( c + 22 ) $
```

**输出中间代码的四地址码：**
第一项为操作符OP，第二项和第三项为操作对象arg2，arg1，第四项为操作结果RET，四者之间的关系为arg1 OP arg2，结果存储在RET中。

```
* , b , 18 , t1
+ , t1 , a , t2
+ , 22 , c , t3
- , t3 , t2 , t4
= , t4 ,  , ret
```

**扩展符号表：**
第一项为符号表序号，也用来表示地址，第二项为变量名，第三项为变量类型，这里默认为int型。

```
( 1 , ret , int )
( 2 , a , int )
( 3 , b , int )
( 4 , c , int )
```



## 实验四：代码生成

**实验内容：**

（1）将中间代码所给的地址码生成目标代码（汇编指令）

（2）生成汇编指令，将三地址码序列生成代码序列表（参考 P407 页）

（3）减少程序与指令的开销，进行部分优化（可选）

**输入三地址码：**

```
t1 = a - b
t2 = a - c
t3 = t1 + t2
d = t3 + t2
```

**输出汇编指令：**

```
MOV a,R0
SUB b,R0
MOV a,R1
SUB c,R1
ADD R1,R0
ADD R2,R0
MOV R0,d
```

