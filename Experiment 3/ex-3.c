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

#define ID 0
#define NUM 1

void writeSymbListOut();
void Print(char str[SYMB_LEN]);
int isDigit(char ch);
int isLetter(char ch);
int isSpace(char ch);
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
///////////////////////////////////////
typedef struct SYMBLISTOUT
{
    char name[SYMB_LEN];
    int type;
}SYMBLISTOUT;
int symblistout_len = 0;
SYMBLISTOUT symblistout[SYMB_NUM];

char outlist[100][200];
int outlist_i = 0;


int outlist_j = 0;

///////////////////////////////////////
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
    ////////////////////////////////////////////
    int tempvar = -1;
    Stack stack3;
    initStack(&stack3);
    int symblistout_p = 1;
    int top_s3;
    char temp_str1[10];
    char temp_str2[10];
    FILE *fp1;
    ////////////////////////////////////////////
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
    if( (fp1=fopen("outlist.txt", "w")) == NULL ){
        puts("Fail to open file!");
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
                ////////////////////////////////////////////////////////////
                if (strcmp(pr[m], "ID") == 0 || strcmp(pr[m], "NUM") == 0)
                {
                    push(&stack3, symblistout_p);
                    symblistout_p ++;
                }
                else if (strcmp(pr[m], "+") == 0 || strcmp(pr[m], "-") == 0|| strcmp(pr[m], "*") == 0|| strcmp(pr[m], "/") == 0)
                {
                    strcpy(outlist[outlist_i], pr[m]);
                    strcat(outlist[outlist_i], " , ");

                    for (int r = 0; r < 2; r++)
                    {
                        top_s3 = top(&stack3);
                        pop(&stack3);
                        if (top_s3 > 0)
                        {
                            strcat(outlist[outlist_i], symblistout[top_s3].name);     
                        }           
                        else
                        {
                            top_s3 = abs(top_s3);
                            strcpy(temp_str1, "t");
                            itoa(top_s3, temp_str2, 10);
                            strcat(temp_str1, temp_str2);                    
                            strcat(outlist[outlist_i], temp_str1);
                        }
                        strcat(outlist[outlist_i], " , ");
                    }                    
                    push(&stack3, tempvar);
                    top_s3 = abs(tempvar);
                    strcpy(temp_str1, "t");
                    itoa(top_s3, temp_str2, 10);
                    strcat(temp_str1, temp_str2); 
                    strcat(outlist[outlist_i], temp_str1);
                    strcat(outlist[outlist_i], "\n");
                    fputs(outlist[outlist_i], fp1);
                    tempvar--;
                    outlist_i++;
                }
                else if (strcmp(pr[m], "=") == 0)
                {
                    strcpy(outlist[outlist_i], pr[m]);
                    strcat(outlist[outlist_i], " , ");
                    
                    top_s3 = stack3.index[0];
                    top_s3 = abs(top_s3);
                    strcpy(temp_str1, "t");
                    itoa(top_s3, temp_str2, 10);
                    strcat(temp_str1, temp_str2);   
                    strcat(outlist[outlist_i], temp_str1);

                    strcat(outlist[outlist_i], " , ");
                    strcat(outlist[outlist_i], " , ");
                    strcat(outlist[outlist_i], symblistout[0].name);
                    fputs(outlist[outlist_i], fp1);
                }
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
    fclose(fp1);
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
    int id = 0;
    int num = 0;
    char temp[SYMB_LEN];
    int temp_p = 0;
    //如果文件不存在，给出提示并退出
    if( (fp=fopen("testgram.txt","r")) == NULL ){
        puts("Fail to open file testgram.txt !");
        return 0;
    }
    while( (ch = fgetc(fp)) != EOF ){
        ////////////////////////
        if (isLetter(ch))
        {
            temp[temp_p] = ch;
            id = 1;
            temp_p++;
            temp[temp_p] = '\0';
        }
        else if (isDigit(ch))
        {
            temp[temp_p] = ch;
            num = 1;
            temp_p++;
            temp[temp_p] = '\0';
        }
        else if (isSpace(ch))
        {
            if (id)
            {
                strcat(code, "ID");
                strcat(code, " ");
                symblistout[symblistout_len].type = ID;
                strcpy(symblistout[symblistout_len].name, temp);
                symblistout_len++;
                id = 0;
                temp_p = 0;
            }
            else if (num)
            {
                strcat(code, "NUM");
                strcat(code, " ");
                symblistout[symblistout_len].type = NUM;
                strcpy(symblistout[symblistout_len].name, temp);
                symblistout_len++;
                num = 0;
                temp_p = 0;
            }
            else
            {
                strcat(code, " ");
            }
        }
        else
        {
            temp[0] = ch;
            temp[1] = '\0';
            strcat(code, temp);            
        }
        
        ////////////////////////
    }
    fclose(fp);
    writeSymbListOut();
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

//判断字符是否是字母
int isLetter(char ch){
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
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
//判断字符是否为空格
int isSpace(char ch){
    if (ch == 32)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void writeSymbListOut(){
    FILE *fp1;
    char left[3] = "( ";
    char right[3] = " )";
    char str[50] = "\0";
    char numstr[10] = "\0";
    int i = 0;
    int j = 1;
    if( (fp1=fopen("tablelist.txt", "w")) == NULL ){
        puts("Fail to open file!");
    }
    for ( i = 0; i<symblistout_len; i++)
    {   
        if(symblistout[i].type == ID)
        {
            strcpy(str, left);
            itoa(j, numstr, 10);
            j++;
            strcat(str, numstr);
            strcat(str, " , ");
            strcat(str, symblistout[i].name);
            strcat(str, " , ");
            strcat(str, "int");
            strcat(str, right);
            strcat(str, "\n");
            fputs(str, fp1);
        }

    }
    fclose(fp1);
}