#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define GRAM_NUM 20
#define SYMB_NUM 20
#define GRAM_LEN 8
#define SYMB_LEN 8
#define STATE_NUM 40
#define CODE_LEN 100
#define STACK_LEN 100


#define NOOP 0
#define SHIFT 1
#define REDUCE 2
#define GOTO 3
#define ACCEPT 4

int isDigit(char ch);
int getLR1();
int getTest();
int getTokenOrder();
int getOrder(char str[SYMB_LEN]);

typedef struct GRAMMAR {
    char left[SYMB_LEN];
    char right[GRAM_LEN][SYMB_LEN];
    int right_len;
}GRAMMAR;

typedef struct LR1 {
    int useful;
    int operation;
    int state;
    GRAMMAR production;
}LR1;

LR1 lr1[STATE_NUM][SYMB_NUM];
char symb_list[SYMB_NUM][SYMB_LEN];


char code[CODE_LEN];
int code_len = 0;
int code_p = 0;
int pre_code_p = 0;

//栈实现
typedef struct {
    int top;
    int index[STATE_NUM];
}Stack;

void initStack(Stack *s){
    (*s).top = -1;
}
int top(Stack *s){
    return (*s).index[(*s).top];
}
void push(Stack *s, int buf){
    (*s).index[++(*s).top] = buf;
}
void pop(Stack *s){
    (*s).top--;
}
int size(Stack *s){
    return (*s).top+1;
}


int main(){
    getLR1();
    getTest();
    Stack stack1,stack2;
    initStack(&stack1);
    initStack(&stack2);
    int j,i,op,state,k,m,right_len;
    FILE *fp;
    char pl[SYMB_LEN];
    char pr[GRAM_LEN][SYMB_LEN];
    char str[SYMB_LEN*SYMB_NUM];
    push(&stack1, 0);
    push(&stack2, 10); //$
    if( (fp=fopen("out.txt", "w")) == NULL ){
        puts("Fail to open file out.txt !");
    }
    while (1)
    {
        if (size(&stack1) == size(&stack2))
        {
            j = getTokenOrder();
        }
        else
        {
            j = top(&stack2);
        }
        i = top(&stack1);
        op = lr1[i][j].operation;
        state = lr1[i][j].state;
        strcpy(pl, lr1[i][j].production.left);
        right_len = lr1[i][j].production.right_len;
        for ( k = 0; k < right_len; k++)
        {
            strcpy(pr[k], lr1[i][j].production.right[k]);
        }
        
        if (op == SHIFT)
        {
            push(&stack1, state);
            push(&stack2, j);
        }
        else if (op == REDUCE)
        {
            for ( k = 0; k < right_len; k++)
            {
                pop(&stack1);
                pop(&stack2);
            }
            push(&stack2,getOrder(pl));
            code_p = pre_code_p;

            //print
            Print(pl);
            strcpy(str, pl);
            printf("->");
            strcat(str, "->");
            for ( m = 0; m < k; m++)
            {
                Print(pr[m]);
                strcat(str, pr[m]);
            }
            printf("\n");
            strcat(str,"\n");
            //write
            fputs(str, fp);
        }
        else if (op == GOTO)
        {
            push(&stack1, state);
        }
        else if (op == ACCEPT)
        {
            printf("SUCCEED!\n");
            break;
        }
        else
        {
            printf("ERROR!\n");
            break;
        }
    }
    fclose(fp);
    return 0;
}
int getTokenOrder(){
    char str[SYMB_LEN];
    int i = 0;
    pre_code_p = code_p;
    while (1)
    {
        if (code[code_p] != ' ')
        {
            str[i] = code[code_p];
            i++;
            code_p++;
        }
        else
        {
            code_p++;
            break;   
        }
    }
    str[i] = '\0';
    return getOrder(str);
}

int getOrder(char str[SYMB_LEN]){
    int i = 0;
    int rat = 0;
    for ( i = 0; i < SYMB_NUM; i++)
    {
        if (strcmp(symb_list[i], str) == 0)
        {
            rat = i;
            break;
        }
    }
    if (i == SYMB_NUM)
    {
        rat = -1;
    }
    return rat;
}

int getTest(){
    FILE *fp;
    char ch;
    //如果文件不存在，给出提示并退出
    if( (fp=fopen("testgram.txt","r")) == NULL ){
        puts("Fail to open file testgram.txt !");
        return 0;
    }
    while( (ch = fgetc(fp)) != EOF ){
        code[code_len] = ch;
        code_len ++;
        if (code_len >= CODE_LEN)
        {
            printf("ERROR!");
            break;
        }
    }
    code[code_len] = '\0';
    fclose(fp);
}


int getLR1(){
    FILE *fp;
    char ch;
    char tempsymb[SYMB_LEN];
    int finish_symblist = 0;
    int skip_firstnum = 0;
    int comma_num = 0;
    int tempsymb_p = 0;
    int symblist_p = 0;
    int operation = NOOP;
    int i = 0;
    int j = 0;
    int reduce_left = 0;
    int right = 0;

    if( (fp=fopen("LR1.txt","r")) == NULL ){
        puts("Fail to open file LR1.txt !");
        return 0;
    }
    while( (ch = fgetc(fp)) != EOF ){
        if (finish_symblist)
        {
            if (skip_firstnum)
            {
                if (ch == ',')
                {
                    if (operation == SHIFT)
                    {
                        j = comma_num;
                        lr1[i][j].operation = SHIFT;
                        lr1[i][j].useful = 1;
                        lr1[i][j].state = atoi(tempsymb);
                        tempsymb_p = 0;
                        tempsymb[0] = '\0';
                        operation = NOOP;
                    }
                    else if (operation == NOOP)
                    {
                        if (strcmp(tempsymb, "accept") == 0)
                        {
                            j = comma_num;
                            lr1[i][j].operation = ACCEPT;
                            lr1[i][j].useful = 1;
                        }
                        else if(isDigit(tempsymb[0]))
                        {
                            j = comma_num;
                            lr1[i][j].operation = GOTO;
                            lr1[i][j].useful = 1;
                            lr1[i][j].state = atoi(tempsymb);
                        }
                        tempsymb_p = 0;
                        tempsymb[0] = '\0';
                    }
                    else if (operation == REDUCE)
                    {
                    	j = comma_num;
                        strcpy(lr1[i][j].production.right[right], tempsymb);
                        operation = NOOP;
                        tempsymb_p = 0;
                        tempsymb[0] = '\0';
                        reduce_left = 0;
                        lr1[i][j].production.right_len = right+1;
                        right = 0;
                    }
                    comma_num++;
                }
                else if (ch == '\n')
                {
                    if (tempsymb_p != 0)
                    {
                        j = comma_num;
                        lr1[i][j].operation = GOTO;
                        lr1[i][j].useful = 1;
                        lr1[i][j].state = atoi(tempsymb);
                    }
                    i++;
                    comma_num = 0;
                    tempsymb_p = 0;
                    tempsymb[0] = '\0';
                    skip_firstnum = 0;
                    continue;
                }
                else if (ch == ' ')
                {
                    if (operation == REDUCE)
                    {
                        if (reduce_left == 0)
                        {
                            j = comma_num;
                            lr1[i][j].operation = REDUCE;
                            lr1[i][j].useful = 1;
                            strcpy(lr1[i][j].production.left, tempsymb);
                            tempsymb_p = 0;
                            tempsymb[0] = '\0';
                            reduce_left = 1;
                        }
                        else if (reduce_left == 1)
                        {
                        	if (strcmp(tempsymb, "->") == 0)
							{
								tempsymb_p = 0;
								tempsymb[0] = '\0';
							}
                            else
                            {
                            	j = comma_num;
                            	strcpy(lr1[i][j].production.right[right], tempsymb);
                            	right++;
                            	tempsymb_p = 0;
                            	tempsymb[0] = '\0';
							}
                        }
                    }
                    if (strcmp(tempsymb, "shift") == 0)
                    {
                        operation = SHIFT;
                    }
                    else if (strcmp(tempsymb, "reduce") == 0)
                    {
                        operation = REDUCE;
                    }
					tempsymb_p = 0;
					tempsymb[0] = '\0';
                }
                else
                {
                    tempsymb[tempsymb_p] = ch;
                    tempsymb_p++;
                    tempsymb[tempsymb_p] = '\0';
                }
            }
            else
            {
                if (isDigit(ch))
                {
                    continue;
                }
                else if (ch == ',')
                {
                    skip_firstnum = 1;
                }
            }
        }


        else
        {
            if (ch == ',' || ch == '\n')
            {
                tempsymb[tempsymb_p] = '\0';
                if (tempsymb[0] != '\0')
                {
                    strcpy(symb_list[symblist_p], tempsymb);
                    symblist_p++;
                    tempsymb_p = 0;
                }
                if(ch == '\n')
				{
					finish_symblist = 1;
				}
                continue;
            }
            else
            {
                tempsymb[tempsymb_p] = ch;
                tempsymb_p++;
            }
        }
    }
    fclose(fp);
    return 0;
}

//判断字符是否是数字
int isDigit(char ch){
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Print(char str[SYMB_LEN]){
    int i;
    for ( i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
}