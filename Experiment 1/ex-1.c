#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define N 200
#define KEYWORD_NUM 30
#define OPERATOR_NUM 30
#define BOUNDARY_NUM 5
#define WORD_MAX_LENGTH 16


int isDigit(char ch);
int isLetter(char ch);
int isSpace(char ch);
int inKeywordList(char ch[WORD_MAX_LENGTH]);
int inOperatorList(char ch[WORD_MAX_LENGTH]);
int inBoundaryList(char ch[WORD_MAX_LENGTH]);
void dealWord();
void dealNumber();
void dealSymbol();
void writeTable(char ch[WORD_MAX_LENGTH]);
void writeInFile();

char *keyword[KEYWORD_NUM] = {  "auto",  "break",  "case",  "char",  "const",
                                "continue",  "default",  "do",  "double",  "else",
                                "enum",  "extern",  "float",  "for",  "goto",
                                "if",  "int",  "long",  "return",  "short",
                                "signed",  "sizeof",  "static",  "struct",  "switch", 
                                "typeof",  "union",  "unsigned",  "void", "while"
                                };
char *operator[OPERATOR_NUM] = { "(",   ")",   "[",   "]",   ",", 
                                 "!",   "&",   "&&",  "|",   "||",
                                 "+",   "-",   "*",   "/",   "%",
                                 "^",   "++",  "--",  "+=",  "-=",
                                 "*=",  "%=",  "^=",  "=",   ">",
                                 "<",   ">=",  "<=",  "==",  "!="
                                 };
char *boundary[BOUNDARY_NUM] = { 59, 34, 58, 123, 125 };

struct TOKEN{
    int id;
    char content[WORD_MAX_LENGTH];
};

struct TABLE{
    char name[WORD_MAX_LENGTH];
    int value;
    char attr[WORD_MAX_LENGTH];
};
struct TABLE table[N];

int p_table = 0;
struct TOKEN token[N];
int p_token = 0;


char code[N];
int code_length = 0;
int p_code = 0;

int main(){
    FILE *fp;
    char ch;
    //如果文件不存在，给出提示并退出
    if( (fp=fopen("test.txt","rt")) == NULL ){
        puts("Fail to open file!");
        return 0;
    }
    //每次读取一个字节，直到读取完毕
    while( (ch = fgetc(fp)) != EOF ){
        if (ch >= 32 && ch <= 126)
        {
            if (ch == 32 && code_length >= 1 && code[code_length-1] == 32)
            {
                continue;
            }
            // putchar(ch);
            code[code_length] = ch;

            code_length ++;
            if (code_length >= N)
            {
                printf("ERROR!");
                break;
            }
        }
    }
    putchar('\n');  //输出换行符

    while (p_code < code_length)
    {
    	
        while (isSpace(code[p_code]))
        {
            p_code ++;
        }
        
        if (isLetter(code[p_code]))
        {
            dealWord();
        }
        else if (isDigit(code[p_code]))
        {
            dealNumber();
        }
        else
        {
			if(isSpace(code[p_code])){
				continue;
			}
            dealSymbol();
        }
    }
    
    int i;
    for ( i = 0; i < code_length; i++)
    {
        putchar(code[i]);
    }
    fclose(fp);
    putchar('\n');
    for ( i = 0; i < p_token; i++)
    {
        printf("( %d, %s )\n", token[i].id, &token[i].content);
    }
    for ( i = 0; i < p_table; i++)
    {
        printf("( %s, %d, %s )\n", &table[i].name, table[i].value, &table[i].attr);
    }

    writeInFile();
    
    return 0;
}


void dealWord(){
    char word[WORD_MAX_LENGTH];
    int i = 0;
    int id;

    while (isLetter(code[p_code]) || isDigit(code[p_code]))
    {
        word[i] = code[p_code];
        i++;
        p_code++;
    }
    word[i] = '\0';

    id = inKeywordList(word);
    if (id > 0)
    {
        token[p_token].id = id;
    }
    else
    {
        token[p_token].id = 70;
        writeTable(word);
    }
    strcpy(token[p_token].content, word);
    p_token++;
}

void dealNumber(){
    char word[WORD_MAX_LENGTH];
    int i = 0;
    int isInteger = 1;

    while (isDigit(code[p_code]) || code[p_code] == '.'){
        if (code[p_code] == '.')
        {
            isInteger = 0;
        }
        word[i] = code[p_code];
        i++;
        p_code++;
    }
    word[i] = '\0';
    if (isInteger)
    {
        token[p_token].id = 66;
    }
    else
    {
        token[p_token].id = 67;
    }
    strcpy(token[p_token].content, word);


    p_token++;
}

void dealSymbol(){
    char word1[WORD_MAX_LENGTH];
    char word2[WORD_MAX_LENGTH];
    word1[0] = code[p_code];
    word1[1] = '\0';
    word2[0] = code[p_code];
    word2[1] = code[p_code+1];
    word2[2] = '\0';
    
    int id1_ol = inOperatorList(word1);
    int id1_bl = inBoundaryList(word1);
    int id2 = inOperatorList(word2);
    
    if (id1_bl)
    {
        token[p_token].id = id1_bl;
        strcpy(token[p_token].content, word1);
        p_code += 1;
    }
    else if(id2)
    {
        token[p_token].id = id2;
        strcpy(token[p_token].content, word2);  
		p_code += 2;      
    }
    else if(id1_ol)
    {
        token[p_token].id = id1_ol;
        strcpy(token[p_token].content, word1);  
		p_code += 1;  
    }
    p_token++;
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

//判断字符串是否在关键字列表中
int inKeywordList(char ch[WORD_MAX_LENGTH]){
    int i;
    int ret = 0;
    for (i = 0; i < KEYWORD_NUM; i++)
    {
        if (strcmp(ch, keyword[i]) == 0)
        {
            ret = i + 1;
            break;
        }
    }
    return ret;
}

//判断字符串是否在操作符列表中
int inOperatorList(char ch[WORD_MAX_LENGTH]){
    int i;
    int ret = 0;
    for (i = 0; i < OPERATOR_NUM; i++)
    {
        if (strcmp(ch, operator[i]) == 0)
        {
            ret = i + 31;
            break;
        }
    }
    return ret;
}

//判断字符是否在界符列表中
int inBoundaryList(char ch[WORD_MAX_LENGTH]){
    int i;
    int ret = 0;
    
    for (i = 0; i < BOUNDARY_NUM; i++)
    {
        if (ch[0] == boundary[i])
        {
            ret = i + 61;
            break;
        }
    }
    return ret;
}

void writeTable(char ch[WORD_MAX_LENGTH]){
    int i;
    int isExist = 0;
    for(i=0; i<p_table; i++){
        if (strcmp(table[i].name, ch) == 0)
        {
            isExist = 1;
            break;
        }
    }
    if (!isExist)
    {
        strcpy(table[p_table].name ,ch);
        strcpy(table[p_table].attr, "?");
        table[p_table].value = -1;
        p_table++;
    }
}

void writeInFile(){
    FILE *fp1;
    FILE *fp2;
    char str[WORD_MAX_LENGTH];
    char numstr[WORD_MAX_LENGTH];
    char left[3] = "( ";
    char right[3] = " )";
    int i = 0;

    if( (fp1=fopen("token.txt", "w")) == NULL ){
        puts("Fail to open file!");
    }
    if( (fp2=fopen("table.txt", "w")) == NULL ){
        puts("Fail to open file!");
    }

    str[WORD_MAX_LENGTH] = "\0";
    for ( i = 0; i < p_token; i++)
    {
        strcpy(str, left);
        itoa(token[i].id, numstr, 10);
        strcat(str, numstr);
        strcat(str, " , ");
        strcat(str, token[i].content);
        strcat(str, right);
        strcat(str, "\n");
        fputs(str, fp1);
    }
    for ( i = 0; i < p_table; i++)
    {
        strcpy(str, left);
        strcat(str, table[i].name);
        strcat(str, " , ");
        itoa(table[i].value, numstr, 10);
        strcat(str, numstr);
        strcat(str, " , ");
        strcat(str, table[i].attr);
        strcat(str, right);
        strcat(str, "\n");
        fputs(str, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}