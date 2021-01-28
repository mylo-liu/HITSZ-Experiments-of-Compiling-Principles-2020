#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_SYMB_LEN 10
#define TERN_LIST_LEN 20
#define REG_NUM 6
#define MAX_SYMB_NUM 10
#define MAX_ASEM_NUM 10

typedef struct ternary
{
    char symb[3][MAX_SYMB_LEN];
    char op;
}TERNARY;

char reg[REG_NUM][MAX_SYMB_LEN];

char asemCode[MAX_ASEM_NUM][MAX_SYMB_LEN*3];
int ac_p = 0;


// typedef struct symblist
// {
//     char name[MAX_SYMB_LEN];
//     int reg_num;
// }SYMBLIST;

int isSpace(char ch);
int readTernaryList(TERNARY *tl, int *tl_len);
void dealTernaryList(TERNARY *tl, int tl_len);
int findSymbInReg(char *symb);
int getreg(char *symb);
void initreg();
void writeInAsem(char *op, char *arg1, char *arg2);
void writeInFile();

int main() {
    int ternartListLen = 0;
    TERNARY ternaryList[TERN_LIST_LEN];
    readTernaryList(ternaryList, &ternartListLen);
    dealTernaryList(ternaryList, ternartListLen);
    writeInFile();
    return 0;
}

int readTernaryList(TERNARY *tl, int *tl_len)
{
    FILE *fp;
    char ch;
    int tl_p = 0;
    int symb_p = 0;
    int temp_p = 0;
    char tempsymb[MAX_SYMB_LEN];
    //如果文件不存在，给出提示并退出
    if( (fp=fopen("input.txt","r")) == NULL ){
        puts("Fail to open file testgram.txt !");
        return 0;
    }
    while( (ch = fgetc(fp)) != EOF ){
        if (isSpace(ch))
        {
            if (temp_p == 0)
            {
                continue;
            }            
            strcpy(tl[tl_p].symb[symb_p], tempsymb);
            symb_p++;
            if (symb_p >= 3) { printf("ERROR IN SYMB_P!\n"); }
            temp_p = 0;
        }
        else if (ch == '\n')
        {
            strcpy(tl[tl_p].symb[symb_p], tempsymb);
            symb_p = 0;
            temp_p = 0;
            tl_p++;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            tl[tl_p].op = ch;
        }     
        else if (ch == '=')
        {
            continue;
        }   
        else
        {
            tempsymb[temp_p] = ch;
            temp_p++;
            tempsymb[temp_p] = '\0';
        }
    }
    fclose(fp);
    *tl_len = tl_p;
}

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


void dealTernaryList(TERNARY *tl, int tl_len)
{
    initreg();
    char arg1[MAX_SYMB_LEN];
    char arg2[MAX_SYMB_LEN];
    char res[MAX_SYMB_LEN];
    char op;
    char asem_op[MAX_SYMB_LEN];
    char asem_arg1[MAX_SYMB_LEN];
    char asem_arg2[MAX_SYMB_LEN];
    char numstr[MAX_SYMB_LEN];
    int regnum;
    int resregnum;

    for (int i = 0; i <= tl_len; i++)
    {
        strcpy(res, tl[i].symb[0]);
        strcpy(arg1, tl[i].symb[1]);
        strcpy(arg2, tl[i].symb[2]);
        op = tl[i].op;

        regnum = findSymbInReg(arg1);
        if(regnum == -1)
        {
            regnum = getreg(arg1);
            strcpy(asem_op, "MOV");
            strcpy(asem_arg1, arg1);
            strcpy(asem_arg2, "R");
            itoa(regnum, numstr, 10);   
            strcat(asem_arg2, numstr);            
            writeInAsem(asem_op, asem_arg1, asem_arg2);
        }
        else
        {
            strcpy(asem_arg2, "R");
            itoa(regnum, numstr, 10);
            strcat(asem_arg2, numstr);
        }
        resregnum = regnum;
        regnum = findSymbInReg(arg2);
        if (regnum == -1)
        {
            strcpy(asem_arg1, arg2);
        }
        else
        {
            strcpy(asem_arg1, "R");
            itoa(regnum, numstr, 10);
            strcat(asem_arg1, numstr);
        }
        
        strcpy(reg[resregnum], res);
        switch (op)
        {
        case '+': strcpy(asem_op, "ADD"); break;
        case '-': strcpy(asem_op, "SUB"); break;
        case '*': strcpy(asem_op, "MUTI"); break;
        case '/': strcpy(asem_op, "DEV"); break;        
        default:
            break;
        }

        writeInAsem(asem_op, asem_arg1, asem_arg2);

        if (i == tl_len)
        {
            strcpy(asem_op, "MOV");
            strcpy(asem_arg2, res);
            strcpy(asem_arg1, "R");
            itoa(resregnum, numstr, 10);
            strcat(asem_arg1, numstr);      
            writeInAsem(asem_op, asem_arg1, asem_arg2);      
        }
    }

}

int findSymbInReg(char *symb)
{
    int i;
    int sign = 0;
    for (i = 0; i < REG_NUM; i++)
    {
        if (strcmp(reg[i], symb) == 0)
        {
            sign = 1;
            break;
        }        
    }    
    if (sign)
    {
        return i;
    }
    else
    {
        return -1;
    }    
}

int getreg(char *symb)
{
    int i;
    for (i = 0; i < REG_NUM; i++)
    {
        if (reg[i][0] == 0)
        {
            strcpy(reg[i], symb);
            break;
        }
    }    
    return i;
}

void initreg()
{
    for (int i = 0; i < REG_NUM; i++)
    {
        strcpy(reg[i], "\0");
    }    
}

void writeInAsem(char *op, char *arg1, char *arg2)
{
    strcpy(asemCode[ac_p], op);
    strcat(asemCode[ac_p], " ");
    strcat(asemCode[ac_p], arg1);
    strcat(asemCode[ac_p], ",");
    strcat(asemCode[ac_p], arg2);
    puts(asemCode[ac_p]);
    ac_p++;
}

void writeInFile()
{
    FILE *fp;
    if( (fp=fopen("asemcode.txt", "w")) == NULL ){
        puts("Fail to open file!");
    }    
    for (int i = 0; strcmp(asemCode[i], "\0") != 0; i++)
    {
        strcat(asemCode[i], "\n");
        fputs(asemCode[i], fp);
    }
    fclose(fp);
}