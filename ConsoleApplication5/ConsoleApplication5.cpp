// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>




/*     词法分析相关定义*/
#define N 100
#define LEN 70
typedef struct token{    /*用于存放单词符号的机内表示*/
	int label;        //单词序号
	char name[30];    //单词名
	int code;	     //单词机内码
	int addr;        //入口地址
}token;
typedef struct KeyWord{
	char name[30];
	int code;
}KeyWord;
typedef struct symble{  /*用于存放源程序中标识符、正常数等*/
	int number;      //序号
	int type;        //机内码
	char name[30];   //单词本身
}symble;
char ch;
int var_count, error_count, label_count, code_count, addr_count, LineOfPro1;//,D_addr,D_code;
char filename[30];
FILE *KeyFin;
FILE *SourceFin;
FILE *TokenFout;
FILE *SymbleFout;
KeyWord key[LEN];
token CToken;
symble CSymble;
symble SymbleList[N];
void Scanner();
void ScannerIint();
void IsBLOrBS();
void IsShuZi();
void IsZhuShi();
void IsZiFuChuan();
void IsOther();
void Out();
void Wordmain();
void Error(int a);
int Word();
int strcmp(char *s, char *t){   //比较函数
	for (; *s == *t; s++, t++)
		if (*s == 0)
			return 0;
	return 1;
}





/*         语法语义相关定义*/
typedef struct stack//相当于token表
{
	char name[20];
	int cod;
	int addr;
}stack;
typedef struct equ//四元式结构
{
	int op;//操作码
	int op1;//第一操作数
	int op2;//第二操作数
	int result;//结果变量的符号地址
}equ;
typedef struct var//变量说明标识符结构
{
	char name[20];//变量名
	int addr;//地址
	int type;//类型
	int value;//值域
}var;
#define EXP_LEN 100
#define EQU_LEN 1024
#define ONE     50001
#define ZERO    50000
#define E_EXPR  7
#define IF      8
#define WHILE   9
#define FOR     10
#define REPEAT  11
#define B_EXP   12
FILE *TokenFin;
FILE *SymbleFin;
FILE *EquFout;//equ文件的指针
int code;//机内码
int address;//地址
int LineOfPro;//程序当前所在行数
int LineOfEqu;//四元式的序号
stack var_list[EXP_LEN];//存放标识符的结构数组
stack expr[EXP_LEN];//作为桟存放code和address的数组
equ Equ[EQU_LEN];//存放四元式的结构数组
var TempList[EXP_LEN];//存放变量的结构数组
char ID[20];//用于存放token表取出的单词名称
int var_count1;//标识符数量
int len_count;
int pos;//作为桟的下标
int now_addr;//当前的单词地址
int temp_count;
int error_count1;//错误数量
int let_count;//在语句分析如果是条件循环语句for,while的话，则用来做初始条件标示符的地址
int x_count; //循环语句中四元式的数量
int E_Contrl;//控制加减乘除
int E_rtn;//将四元式存进数组时的四元式序号
int True_address;//正确的地址
int False_address;//错误的地址
int gen_pos;//将四元式写入文件时的数组下标
int EquPush(int op, int a, int b, int r);/*四元式存进数组*/
void GetNext();//读取token表单词
void Error1(int num);//语法错误
int gen(int op, int a, int b, int r);//输出四元式到文件
void Declear();//变量说明
void InitStack();//初始化桟
void yfyymain();
void Push(int cod, int addr);//进栈
int NewTemp();//产生临时变量，返回入口地址
void BackPatch(int addr, int addr2);

/********布尔表达式分析函数************************/
int B_Analize();//调用B_OR()函数
void B_Init();//将运算符压桟
int B_OR();//调用L_AND和B1_OR函数
int B1_OR(int a);//或运算
int L_AND();//调用M_NOT和L1_AND函数
int L1_AND(int a);// 处理与，调用M_NOT函数
int M_NOT();//处理否定 M->notM|K  ，调用K_END函数
int K_END();//处理真假问题   K->i|false|true|(B)  ，调用K_CMP函数
int K_CMP();//处理关系表达式 K->iSi    !=|=|<=|>=|>|<

/********算术表达式分析函数************************/
int E_Analize();//调用E_init和E_AddSub函数
void E_Init();//将算术表达式进栈
int E_AddSub();//计算加减，调用T_MulDiv和E1_AddSub函数
int E1_AddSub(int a);//计算加减，调用T_MulDiv函数
int T_MulDiv();//计算乘除，调用F_Number函数
int T1_MulDiv(int a);//计算乘除
int F_Number();//查看是否有括号或者是提取将要计算的表达式


/********语句分析函数******************************/
void parser();//总函数
int S_Let(int a);//赋值分析函数
int S_If();//if语句函数
int S_While();//while语句
int S_For();//for语句
int S_Repeat();//重复语句
int S_Begin();//复合语句分析函数
int L_Analize();//判断语句类型







/*     目标代码生成相关定义*/
#define STACKLONG 100
#define TOKENLENGTH 1024
typedef struct GenStruct{
	int label;//序号
	char op[4];//四元式的操作符
	int code;//机内码
	int addr1;//第一操作数的地址
	int addr2;//第二操作数的地址
	int result;//结果地址
	int out_port;//四元式是否为基本块的入口，是则1，不是则为0
}GenStruct;//四元式结构数组

typedef struct Token{
	int label;//序号
	char name[20];//单词名
	int cod1;//机内码
}Token;//提取symble表中的标识符的结构

typedef struct Oper{
	int code;
	char opp[4];//运算操作符
}Oper;//

typedef struct regis{
	int number;
	char name[3];
	int allocate;
}regis;//寄存器结构

Token TokenTable[TOKENLENGTH];
GenStruct GenStack[STACKLONG];
GenStruct CurrentGen;//存放四元式
Oper operation[12];//存放四元式中出现的运算符和不操作的0
regis bx, cx, dx;
FILE *GenFin;
FILE *TokenFin1;
FILE *AssemFout;
char ONE1[2];//对于四元式中one和zero的定义
char ZERO1[2];
int gen_count, token_count;//四元式数量，标识符数量
void InitOp();//初始化，将四元式中的运算符和0存进结构数组以便使用
void Target();
int GetNextGen();
void Tmain();
void InitTarget();
void Produce(int op, int a, int b, int r);
char *GetName(int a);
void SortDGA(int address, int op_code);





int  main()
{
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^编译器^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^第一部分：词法分析^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^第二部分：语法语义分析^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^第三部分：目标代码生成^^^^^^^^^^^^^^^^^\n");
	Wordmain();
	yfyymain();
	Tmain();
	system("pause");
	getchar();
	return 0;
}  

void Wordmain()         //词法分析主函数
{
	int i = 0, j = 0;
	code_count = 0;//code数
	LineOfPro1 = 0;//行数
	var_count = 0;//标识符变量数
	addr_count = 1;//地址数
	label_count = 1;//单词数
	for (i = 0; i < N; i++){
		SymbleList[i].number = 0;
		SymbleList[i].type = 0;
		for (j = 0; j < 30; j++)
			SymbleList[i].name[j] = '\0';
	}
	Scanner();
}
void Scanner()
{
	int i = 0;
	error_count = 0;//错误数
	ScannerIint();                         //调用初始化函数！
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^词法分析器^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("输入源文件名:");
	for (;;){
		scanf("%c", &filename[i]);
		if (filename[i] == 10)
			break;
		i++;
	}
	filename[i] = '\0';
	if ((SourceFin = fopen(filename, "rt")) == NULL){
		printf("%s 文件无法打开！\n", filename);
		exit(1);
	}
	if ((TokenFout = fopen("token.txt", "wt+")) == NULL){
		printf("token.txt 文件无法打开！\n");
		exit(1);
	}
	if ((SymbleFout = fopen("symble.txt", "wt+")) == NULL){
		printf("symble.txt 文件无法打开！\n");
		exit(1);
	}
	ch = fgetc(SourceFin);     //读取第一个字符
	/*48-57位数字0-9的ASCII码,65-90是26个大写字母的ASCII码，97-122是26个小写写字母的ASCII码*/
	while (ch != EOF)  //遍历源程序文件的每一个字符
	{
		for (i = 0; i < 30; i++)//每一次都将CToken.name[]初始化为\0
		{
			CToken.name[i] = '\0';
		}
		if ((ch>47) && (ch<58))//字符为数字
		{
			IsShuZi();
		}
		else
		{
			if (((ch>64) && (ch<91)) || ((ch>96) && (ch<123)) || ch == '_')//字符为字母或_
			{
				IsBLOrBS();
			}
			else
			{
				if (ch == '/')    IsZhuShi();//字符是/
				else if (ch == '\'') IsZiFuChuan();
				else              IsOther();//其他情况
			}
		}
	}
	fclose(SourceFin);
	fclose(TokenFout);
	fclose(SymbleFout);
	printf("分析完毕!\n");
}
void ScannerIint()           /*初始化函数*/
{
	int i = 1, k = 0;
	if ((KeyFin = fopen("in.txt", "rt")) == NULL){  //in.txt为缓冲区
		printf("in.txt 文件无法打开！\n");
		exit(1);
	}
	for (i = 0; i < 60; i++)
		for (k = 0; k < 30; k++)
			key[i].name[k] = '\0';
	for (i = 1; i < 60; i++){    /*读入编码表KeyFin中*/
		fscanf(KeyFin, "%s%d", key[i].name, &key[i].code);
	}
	fclose(KeyFin);

}
void IsShuZi()     /*数字处理*/
{
	int k = 0, flag = 0;
	char ch1;
	while (((ch>47) && (ch<58)))//0-9数字
	{
		CToken.name[k++] = ch;//先赋值然后k+1
		ch = fgetc(SourceFin);
		if (ch == '.'){
			flag = 1;
			break;
		}
	}
	CToken.code = 28;
	CToken.addr = addr_count++;
	CToken.label = label_count++;
	if (flag)//当flag=1时进入
	{
		ch1 = fgetc(SourceFin);
		if ((ch1 > 47) && (ch1 < 58))
			CToken.name[k++] = ch;
		else
			Error(2);
		ch = ch1;
		while ((ch > 47) && (ch < 58))
		{
			CToken.name[k++] = ch;
			ch = fgetc(SourceFin);
		}
		CToken.code = 29;
		if (ch == '.')
		{
			Error(2);
			ch = fgetc(SourceFin);
			while ((ch > 47) && (ch < 58))
				ch = fgetc(SourceFin);
		}
		if (((ch > 64) && (ch < 90)) || ((ch > 96) && (ch < 123)))
		{
			Error(2);
			while (((ch > 64) && (ch < 90)) || ((ch > 96) && (ch < 123)))
			{
				ch = fgetc(SourceFin);
				while ((ch > 47) && (ch < 58))
					ch = fgetc(SourceFin);
			}
		}
	}
	Out();
}
void IsBLOrBS()          /*保留和标识处理*/
{
	int i = 0, h = 0;
	while (((ch > 64) && (ch < 90)) || ((ch > 96) && (ch < 123)) || ch == '_'){
		CToken.name[i++] = ch;
		ch = fgetc(SourceFin);
	}
	for (i = 1; i < LEN; i++){
		h = strcmp(CToken.name, key[i].name);
		if (!h)
			break;
	}
	if (!h){
		CToken.code = key[i].code;
		CToken.addr = -1;
	}
	else{
		CToken.code = 27;
		CToken.addr = addr_count++;//先赋值在增1
	}
	CToken.label = label_count++;
	Out();
}
void IsZhuShi()             /*注释和除法处理*/
{
	char ch1;
	ch1 = ch;
	ch = fgetc(SourceFin);
	if (ch == '*')
		for (;;){
			ch = fgetc(SourceFin);
			if (ch == EOF){
				Error(3);
				break;
			}
			if (ch == '*'){
				ch1 = ch;
				ch = fgetc(SourceFin);
				if (ch == '/'){
					ch = fgetc(SourceFin);
					break;
				}
			}
		}
	else{
		CToken.name[0] = '/';       /*如果不是注释，则当作除法*/
		CToken.code = 30;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
	}
}
void IsZiFuChuan()           /*字符串处理*/
{
	int i = 0;
	for (;;){
		ch = fgetc(SourceFin);
		CToken.code = 30;
		if (ch == '\''){
			CToken.name[i++] = ch;
		}
		else
			break;
	}
	CToken.addr = addr_count++;
	CToken.label = label_count++;
	Out();
	ch = fgetc(SourceFin);
}
void IsOther()               /*其他情况处理*/
{
	char ch1;
	int i;
	for (i = 0; i < 30; i++)
		CToken.name[i] = '\0';
	switch (ch){
	case '(':
		CToken.name[0] = '(';
		CToken.code = 32;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case ')':
		CToken.name[0] = ')';
		CToken.code = 33;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '*':
		CToken.name[0] = '*';
		CToken.code = 34;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '+':ch1 = fgetc(SourceFin);
		if (ch1 == '+'){
			CToken.name[0] = '+';
			CToken.name[1] = '+';
			CToken.code = 57;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
			ch1 = fgetc(SourceFin);
		}
		else{
			CToken.name[0] = '+';
			CToken.code = 35;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
		}
		ch = ch1;
		break;
	case '-':ch1 = fgetc(SourceFin);
		if (ch1 == '-'){
			CToken.name[0] = '-';
			CToken.name[1] = '-';
			CToken.code = 58;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
			ch1 = fgetc(SourceFin);
		}
		else{
			CToken.name[0] = '-';
			CToken.code = 36;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
		}
		ch = ch1;
		break;
	case ',':
		CToken.name[0] = ',';
		CToken.code = 37;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '.':
		CToken.name[0] = '.';
		CToken.code = 38;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case ':':
		CToken.name[0] = ':';
		CToken.code = 40;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '!':ch1 = ch;
		ch = fgetc(SourceFin);
		if (ch == '='){
			CToken.name[0] = '!';
			CToken.name[1] = '=';
			CToken.code = 41;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
		}
		else{
			CToken.name[0] = '!';
			CToken.code = 45;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
			ch = fgetc(SourceFin);
		}
		break;
	case ';':
		CToken.name[0] = ';';
		CToken.code = 42;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '<':ch1 = fgetc(SourceFin);
		if (ch1 == '='){
			CToken.name[0] = '<';
			CToken.name[1] = '=';
			CToken.code = 44;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
			ch1 = fgetc(SourceFin);
		}
		else{
			CToken.name[0] = '<';
			CToken.code = 43;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
		}
		ch = ch1;
		break;
	case '=':
		CToken.name[0] = '=';
		CToken.code = 46;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '>':
		ch1 = fgetc(SourceFin);
		if (ch1 == '='){
			CToken.name[0] = '>';
			CToken.name[1] = '=';
			CToken.code = 48;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
			ch1 = fgetc(SourceFin);
		}
		else{
			CToken.name[0] = '>';
			CToken.code = 47;
			CToken.addr = -1;
			CToken.label = label_count++;
			Out();
		}
		ch = ch1;
		break;
	case '{':
		CToken.name[0] = '{';
		CToken.code = 49;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '}':
		CToken.name[0] = '}';
		CToken.code = 50;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '"':
		CToken.name[0] = '"';
		CToken.code = 51;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case '[':
		CToken.name[0] = '[';
		CToken.code = 52;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case ']':
		CToken.name[0] = ']';
		CToken.code = 53;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
		ch = fgetc(SourceFin);
		break;
	case 10:
		LineOfPro1++;
		ch = fgetc(SourceFin);
		break;
	case 13:
		LineOfPro++;
		ch = fgetc(SourceFin);
		break;
	case ' ':
		ch = fgetc(SourceFin);
		break;
	case EOF:
		Error(4);
		break;
	default:
		Error(1);
		ch = fgetc(SourceFin);
		break;
	}
}
void Out()               /*输出模块*/
{
	int k, flag, i = 0;
	/*查询与填写字符表*/
	if ((CToken.code == 27) || (CToken.code == 30) || (CToken.code == 28) || (CToken.code == 29)){
		CSymble.number = CToken.addr;
		CSymble.type = CToken.code;
		strcpy(CSymble.name, CToken.name);
		flag = Word();
		if (((CToken.code == 27) && (flag == 1)) || ((CToken.code == 28) && (flag == 1)) || ((CToken.code == 29) && (flag == 1)) || ((CToken.code == 30) && (flag == 1)))
			fprintf(SymbleFout, "%3d %3d %s \n", CSymble.number, CSymble.type, CSymble.name);
	}
	for (;;)
		if (CToken.name[i++] == '\0')
			break;
	fprintf(TokenFout, "%3d  %s", CToken.label, CToken.name);
	printf("%3d  %s", CToken.label, CToken.name);
	for (k = 20 - i; k > 0; k--){
		fprintf(TokenFout, "");
		printf("");
	}
	fprintf(TokenFout, "%3d  %3d \n", CToken.code, CToken.addr);
	printf("%3d  %3d\n", CToken.code, CToken.addr);
}
void Error(int a)              /*出错处理*/
{
	error_count++;
	switch (a){
	case 1:
		printf("error %3d 非法字符于 %d行！\n\n", error_count, LineOfPro1 + 1);
		break;
	case 2:
		printf("error %3d 实常数出错于 %d行！\n\n", error_count, LineOfPro1 + 1);
		break;
	case 3:
		printf("error %3d 出错于%d行,没有匹配的注释符 '*/'\n\n", error_count, LineOfPro1 + 1);
		break;
	case 4:
		printf("error %3d 出错于%d行,非正常结束！ \n\n", error_count, LineOfPro1 + 1);
		break;
	default:
		break;
	}
	return;
}
int Word()
{
	int flag, i = 0;
	for (i = 0; i < var_count; i++){
		flag = strcmp(CSymble.name, SymbleList[i].name);//判断两个name是否相等
		if (flag == 0){
			CToken.addr = SymbleList[i].number;
			return 0;
		}
	}
	SymbleList[var_count].number = CToken.addr;
	SymbleList[var_count].type = CToken.code;
	strcpy(SymbleList[var_count].name, CToken.name);
	var_count++;
	return 1;
}







void yfyymain()
{
	system("pause");
	parser();
	//return 0;
}

void parser()
{
	int gen_pos = 0;
	int Line;
	int i;
	char ch1;
	pos = 0;
	let_count = 0;
	error_count1 = 0;//语法分析错误数
	LineOfPro = 1;//代码所在行数
	temp_count = 0;
	now_addr = 0;//当前地址
	//len_count = 0;
	InitStack();//桟
	for (i = 0; i < EXP_LEN - 2; i++)
	{
		TempList[i].addr = 0;
		TempList[i].addr -= i;
		TempList[i].name[0] = 'b';
		TempList[i].name[1] = 'x';
	}
	/*建立四元式文件*/
	EquFout = fopen("equ.txt", "wt+");
	if ((TokenFin = fopen("token.txt", "rt")) == NULL)
	{
		printf("Cannot open file token.txt strike any key exit!");
		exit(1);
	}
	printf("           *           ****            *\n");
	printf("          --------语法分析程序----------\n");
	printf("           *           ****            *\n");
	printf(" 语法分析开始，请等待......\n");
	GetNext();
	while (code != 18)    //判断class前面的修饰符
	{
		GetNext();
	}
	if (code == 18)
	{  
		GetNext();
		if (code == 27)//类名
		{
			GetNext();
			if (code == 49)//判断{
			{
				GetNext();
				LineOfPro++;
				while (code != 56)
				{   //判断main 关键字前的修饰词
					GetNext();
				}
				if (code == 56){
					GetNext();
					if (code == 32){//判断（
						GetNext();
						if (code == 27){//判断String
							GetNext();
							if (code == 27){//判断args
								GetNext();
								if (code == 52){//[
									GetNext();
									if (code == 53){//]
										GetNext();
										if (code == 33)//)
										{
											GetNext();
											if (code == 49)//{
											{
												GetNext();
												LineOfPro++;
												while (code!=50)
												{
													if (code == 3 || code == 6 || code == 14 || code == 19)
													{
														Declear();           //变量说明分析
														LineOfPro++;

													}
													if (code == 10 || code == 12 || code == 13 || code == 26||code==27)
													{
														S_Begin();          //复合语句分析
													}
													GetNext();

												}
												LineOfPro++;
												GetNext();
												if (code != 50)
												{
													Error1(53);
												}
											}
											else Error1(51);
										}
										else Error1(6);
									}
								}

							}
						}
					}
					else
						Error1(6);
				}
				else
					Error1(54);
			}
			else
				Error1(51);

		}
		else Error1(2);
	}
	else
		Error1(1);
	Line = LineOfEqu;
	LineOfEqu = 0;
	while (gen_pos < Line)
	{
		gen(Equ[gen_pos].op, Equ[gen_pos].op1, Equ[gen_pos].op2, Equ[gen_pos].result);
		gen_pos++;
	}
	gen(0, 0, 0, 0);
	if (error_count1)
		printf("共计%6d个错误！小心呀！\n", error_count1);
	else
		printf("不存在错误，分析成功。\n");
	fclose(TokenFin);
	printf("结果如下：\n");
	rewind(EquFout);
	ch1 = fgetc(EquFout);
	while (ch1 != EOF)
	{
		printf("%c", ch1);
		ch1 = fgetc(EquFout);
	}
	fclose(EquFout);

}

void InitStack()//初始化桟
{
	int i, j;
	pos = 0;//桟的下标
	now_addr = 0;//令当前地址为0
	for (i = 0; i < EXP_LEN; i++)//让expr的结构数组初始化为空
	{
		expr[i].addr = 0;
		expr[i].cod = 0;
		for (j = 0; j < 20; j++)
			expr[i].name[j] = 0;
	}
}

void GetNext()
{
	int d1, d2, d3;
	fscanf(TokenFin, "%d %s %d %d\n", &d1, ID, &d2, &d3);
	code = d2;
	address = d3;
}

void Error1(int k){
	switch (k){
	case 1:printf("格式出错，第1行少'class'\n"); break;
	case 2:printf("格式出错，第1行少程序名\n"); break;
	case 3:printf("第%4d行，说明语句出错\n", LineOfPro); break;
	case 4:printf("第%4d行，赋值语句出错，不匹配或其他出错\n", LineOfPro); break;
	case 5:printf("第%4d行，‘end’不匹配\n", LineOfPro); break;
	case 6:printf("第%4d行，语句出错\n", LineOfPro); break;
	case 7:printf("第%4d行，表达式语句出错\n", LineOfPro); break;
	case 8:printf("第%4d行，if语句出错\n", LineOfPro); break;
	case 9:printf("第%4d行，while语句出错\n", LineOfPro); break;
	case 10:printf("第%4d行，for语句出错\n", LineOfPro); break;
	case 11:printf("第%4d行，repeat语句出错\n", LineOfPro); break;
	case 12:printf("第%4d行，布尔表达式出错\n", LineOfPro); break;
	case 49:printf("程序缺少'class',出现错误。\n"); break;
	case 50:printf("未写任何语句！\n"); break;
	case 51:printf("第%4d行，缺乏';'错误\n", LineOfPro); break;
	case 52:printf("第%4d行，缺乏'{'错误\n", LineOfPro); break;
	case 53:printf("第%4d行，缺乏'}'错误\n", LineOfPro); break;
	case 54:printf("格式出错，第2行缺乏main函数\n"); break;
	default:printf("未知错误！！\n"); printf("\n"); break;
	}
	error_count1++;
	return;
}

int gen(int op, int a, int b, int r)
{
	LineOfEqu++;
	switch (op){
	case 0:fprintf(EquFout, "%5d", LineOfEqu);
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('0', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 1:fprintf(EquFout, "%5d", LineOfEqu);//赋值=
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('=', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 2:fprintf(EquFout, "%5d", LineOfEqu);//加法
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('+', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 3:fprintf(EquFout, "%5d", LineOfEqu);//减法-
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('-', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 4:fprintf(EquFout, "%5d", LineOfEqu);//乘法*
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('*', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 5:fprintf(EquFout, "%5d", LineOfEqu);//除法/
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('/', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 6:fprintf(EquFout, "%5d", LineOfEqu);
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 7:fprintf(EquFout, "%5d", LineOfEqu);//j<
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc('<', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
        fprintf(EquFout, "%5d", r);
        fputc(' ', EquFout);
        fputc(')', EquFout);
        fputc('\n', EquFout);
        break;

	case 8:fprintf(EquFout, "%5d", LineOfEqu);//j=
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc('=', EquFout);
		fputc(' ', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 9:fprintf(EquFout, "%5d", LineOfEqu);//j>=
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc('>', EquFout);
		fputc('=', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 10:fprintf(EquFout, "%5d", LineOfEqu);//j<=
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc('<', EquFout);
		fputc('=', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	case 11:fprintf(EquFout, "%5d", LineOfEqu);//j!=
		fputc(' ', EquFout);
		fputc('(', EquFout);
		fputc(' ', EquFout);
		fputc('j', EquFout);
		fputc('!', EquFout);
		fputc('=', EquFout);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", a);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", b);
		fputc(' ', EquFout);
		fprintf(EquFout, "%5d", r);
		fputc(' ', EquFout);
		fputc(')', EquFout);
		fputc('\n', EquFout);
		break;

	default:break;
	}
	return LineOfEqu;
}

/************************说明变量语句分析**************************/
void Declear()

{
	int a = 1, i = 0, j = 0;
	int df;	
	InitStack();
	i = 0;
	if (code == 3 || code == 6 || code == 14 || code == 19)   //变量声明（Boolean,char,int,double）
	{
		GetNext();
		if (code == 37 || code == 42)    //为逗号或者分号
		{
			Error1(3);
			return;
		}
		while (code != 42)  //若不为分号，则进栈
		{
			Push(code, address);
			GetNext();
		}
		i = 0;
		var_count1 = 0;
		while (pos > 0)
		{
			if (expr[i].cod == 27)    //为标识符
			{
				i++;
				pos--;
				var_count1++;
			}
			else
			{
				if (expr[i].cod == 37)  //为逗号
				{
					i++;
					pos--;
					df = expr[i].cod;
					if (df != 27)
					{
						Error1(3);
						return;
					}
					else
					{
						/*i++;
						pos--;
						if (expr[i].cod == 42)  //为分号
						{
							i++; pos--; LineOfPro++;
						}
						else
						{
							if (df != 27)
							{
								Error1(51); LineOfPro++;
							}
							
						}*/
					}
				}
				else
				{
					Error1(3);
					return;
				}
			}
		}
		/*
		GetNext();
		if (code == 37 || code == 42)    //为逗号或者分号
		{
			Error1(3);
			return;
		}
		while (code != 42)
		{
			if (code == 27)//标识符
			{
				var_count1++;
				D_code = code;
				D_addr = address;
			}
			GetNext();
			if (code == 46)//标识符后是赋值=号
			{
				S_Let(0);
			}
			else 
			{
				if (code == 37)//标识符后是逗号
				{
					GetNext();
				}
				if (code == 42)//标示符后是分号
				{
					LineOfPro++;
				}
				
			}
		}*/
		
		/***修改符号表***/
		for (i = 0; i < EXP_LEN; i++)//初始化var_List数组
		{
			var_list[i].addr = 0;
			var_list[i].cod = 0;
			for (j = 0; j < 30; j++)
				var_list[i].name[j] = '\0';
		}
		if ((SymbleFin = fopen("symble.txt", "rt")) == NULL)
		{
			printf("Cannot open flie symble.txt strike an key exit!");
			exit(1);
		}
		i = 0;
		while (1)
		{
			fscanf(SymbleFin, "%d%d%s\n", &var_list[i].addr, &var_list[i].cod, &var_list[i].name);
			if (var_list[i].addr == 0)	break;
			
			i++;
		}
		fclose(SymbleFin);
		if ((SymbleFin = fopen("symble.txt", "wt")) == NULL)
		{
			printf("Cannot open flie symble.txt strike an key exit!");
			exit(1);
		}
		a=0;
		fprintf(SymbleFin, "%3d  0  0\n", var_count1);
		while(a<i)
		{
			fprintf(SymbleFin, "%3d %3d %s\n", var_list[a].addr, var_list[a].cod, var_list[a].name);
			a++;
		}
		fclose(SymbleFin);
	}
	return;
}

/******************赋值语句分析**********************/
int S_Let(int a)
{
	int addr, flag, rtn,t1;
	flag = 0;
	rtn = 0;
	InitStack();
	if (code == 27)
	{
		addr = address;
		if (a)//a非0时
			let_count = addr;
		var_count1++;
		GetNext();//
		if (code == 46)//赋值号=
		{
			flag = E_Analize();
			if (flag != 0)
				rtn = EquPush(1, flag, 0, addr);
		}
		else if (code == 57)//++
		{
			EquPush(2, addr, ONE, addr);
			GetNext();
		}
		else if (code == 58)//--
		{
			EquPush(3, addr, ONE, addr);
			GetNext();
		}
	}
	else
		Error1(4);
	return rtn;
}

/********算数表达式分析************************/
int E_Analize()
{
	int ans,i;
	ans = 0;
	now_addr = 0;
	E_Init();
	ans = E_AddSub();
	if (expr[now_addr].addr != 0)
		Error1(7);
	return ans;
}

void E_Init()      //将运算数与（ ）+ - * /进栈
{
	int flag = 1, i,j;
	pos = 0;
	E_rtn = 0;
	E_Contrl = 1;//初始化
	for (i = 0; i < EXP_LEN; i++)
	{
		expr[i].addr = 0;
		expr[i].cod = 0;
		for (j = 0; j < 20; j++)
			expr[i].name[j] = 0;
	}
	while (flag)
	{
		GetNext();
		switch (code){
		case 27:Push(code, address); break;//标识符
		case 28:Push(code, address); break;//整常数
		case 29:Push(code, address); break;//实常数
		case 32:Push(code, address); break;//（
		case 33:Push(code, address); break;//)
		case 34:Push(code, address); break;//*
		case 35:Push(code, address); break;//+
		case 36:Push(code, address); break;//-
		case 39:Push(code, address); break;// /
		default:flag = 0; break;
		}
	}
	return;
}

int NewTemp()//产生一个临时变量，返回其在符号表中的入口地址
{
	int a;
	temp_count--;
	a = temp_count;
	return a;
}

int E_AddSub(){
	int t1;
	int rtn = T_MulDiv();
	t1 = rtn;
	rtn = E1_AddSub(t1);
	return rtn;
}

int E1_AddSub(int a)
{
	int rtn, t1;
	rtn = a;
	if (expr[now_addr].cod == 35 || expr[now_addr].cod == 36)    //加减
	{
		int op = expr[now_addr++].cod;
		int opr2 = T_MulDiv();
		t1 = NewTemp();
		if (op == 35){
			if (E_Contrl)
			{
				E_Contrl = 0;
				E_rtn = EquPush(2, rtn, opr2, t1);
			}
			else{
				EquPush(2, rtn, opr2, t1);
			}
			rtn = t1;
		}
		else
		{
			if (E_Contrl)
			{
				E_Contrl = 0;
				E_rtn = EquPush(3, rtn, opr2, t1);
			}
			else{
				EquPush(3, rtn, opr2, t1);
			}
			rtn = t1;
		}
		if (expr[now_addr].cod == 35 || expr[now_addr].cod == 36)
			rtn = E1_AddSub(t1);
	}
	return rtn;
}


int T_MulDiv()
{
	int t1;
	int rtn = F_Number();
	t1 = rtn;
	rtn = T1_MulDiv(t1);
	return rtn;
}


int T1_MulDiv(int a)
{
	int t1, rtn;
	rtn = a;
	if (expr[now_addr].cod == 34 || expr[now_addr].cod == 39)   //乘除
	{
		int op = expr[now_addr++].cod;
		int opr2 = F_Number();
		t1 = NewTemp();
		if (op == 34)
		{
			if (E_Contrl)
			{
				E_Contrl = 0;
				E_rtn = EquPush(4, rtn, opr2, t1);
			}
			else
				EquPush(4, rtn, opr2, t1);
			rtn = t1;
		}
		else
		{
			if (E_Contrl)
			{
				E_Contrl = 0;
				E_rtn = EquPush(5, rtn, opr2, t1);
			}
			else
			{
				EquPush(5, rtn, opr2, t1);
			}
			rtn = t1;
		}
		if (expr[now_addr].cod == 34 || expr[now_addr].cod == 39)
			rtn = T1_MulDiv(t1);
	}
	return rtn;

}

int F_Number()
{
	int rtn = 0;
	if (expr[now_addr].cod == 32)   //为（ 左括号
	{
		now_addr++;
		rtn = E_AddSub();
		if (expr[now_addr++].cod != 40)           //没懂得什么意思
			Error1(7);
	}
	else
	{
		switch (expr[now_addr].cod)
		{
		case 27:rtn = expr[now_addr].addr; break;
		case 28:rtn = expr[now_addr].addr; break;
		case 29:rtn = expr[now_addr].addr; break;
		case 30:rtn = expr[now_addr].addr; break;
		default:Error1(7); break;
		}
		var_count1++;
		now_addr++;
	}
	return rtn;
}

void Push(int cod, int addr)//压桟，pos下标增1
{
	int j;
	expr[pos].addr = addr;
	expr[pos].cod = cod;
	for (j = 0; j < 20; j++)
		expr[pos].name[j] = ID[j];
	pos++;
	//return;
}

/*********复合语句分析************/
int S_Begin()
{
	int rtn = 0;
	if (code == 42)     //为分号
	{
		GetNext();
		LineOfPro++;
	}
	if (code == 13 || code == 10 || code == 26 || code == 12||code==27) //复合语句(if else/switch case/do while/for/标识符)
	{
		rtn = L_Analize();
		/*if (code != 50)   //不为 }时
		{
			Error1(53);
			LineOfPro++;
			return 0;

		}
		else
		{
			LineOfPro++;
		}*/
	}
	return rtn;
}

/*****************语句序列分析******************/

int L_Analize()
{
	int rtn = 0;
	switch (code)
	{
	case 27:S_Let(0); rtn = E_rtn; break;      //赋值语句
	case 13:rtn = S_If(); break;               //if语句
	case 26:rtn = S_While(); break;            //while 语句
	case 10:rtn = S_Repeat(); break;           //switch语句
	case 12:rtn = S_For(); break;             //for 语句
	default:Error1(6);
		return 0;
	}
	if (code == 42)//分号
	{
		LineOfPro++;
		GetNext();
		switch (code)
		{
		case 27:L_Analize(); return rtn;
		case 26:L_Analize(); return rtn;
		case 10:L_Analize(); return rtn;
		case 12:L_Analize(); return rtn;
		case 13:L_Analize(); return rtn;
		default:return rtn;
		}
	}
	return rtn;
}

/*****************if语句分析******************/
int S_If()
{
	int rtn = 0;
	int temp = 0;
	int as;
	int b_addr = 0, tt = 0;
	if (code == 13)              //为if时
	{
		temp = LineOfEqu + 1;
		GetNext();
		if (code == 32)//（
		{
			B_Init();                //运算符进栈
			if (code != 33)
			{
				Error1(8);
			}
		}		
		b_addr = B_Analize();    //进行算数运算，运算结果的地址
		True_address = EquPush(11, b_addr, ZERO, 1);
		False_address = EquPush(6, 0, 0, 0);
		GetNext();
		if (code == 49 || code == 27)             //左括弧{ 或者标识符
		{
			LineOfPro++;//程序到下一行
			GetNext();
				switch (code)
				{
				case 27:rtn = S_Let(0); break;//标识符
				case 13:rtn = S_If(); break;//if
				case 26:rtn = S_While(); break;//while
				case 10:rtn = S_Repeat(); break;//repeat
				case 12:rtn = S_For(); break;//for
				default:Error1(6); break;
				}
				BackPatch(True_address, rtn + 1);
				tt = LineOfEqu + 1;
				as = EquPush(6, 0, 0, tt);
				if (code == 9)             //else 语句
				{
					GetNext();
					if (code == 49 || code == 27)             //左括弧{
					{
						if (code == 27)
						{
							rtn = S_Let(0);
						}
						switch (code)
						{
						case 27:rtn = S_Let(0); break;
						case 13:rtn = S_If(); break;
						case 26:rtn = S_While(); break;
						case 10:rtn = S_Repeat(); break;
						case 12:rtn = S_For(); break;
							//case 2:rtn = S_Begin(); break;
						default:Error1(6); break;
						}
					}
				}
				BackPatch(False_address, tt + 1);
				BackPatch(as, LineOfEqu + 1);
				GetNext();
				LineOfPro++;
				if (code != 50)
				{
					Error1(53);
					return 0;
				}

		}
		else
		{
			Error1(IF);
			return 0;
		}
	}
	return temp;
}

/******************while语句分析**********************/

int S_While()
{
	int rtn = 0,rtn1;
	int temp;
	int b_addr = 0;
	if (code == 42)    //为分号时
	{
		GetNext();
		LineOfPro++;
	}
	if (code == 26)      //为while语句时
	{
		temp = LineOfEqu + 1;
		GetNext();
		B_Init();                //运算符进栈
		GetNext();
		b_addr = B_Analize();    //进行算数运算
		True_address = EquPush(11, b_addr, ZERO, 1);
		False_address = EquPush(6, 0, 0, 0);
		if (code == 49 || code == 27)             //左括弧{ 或者标识符
		{
			LineOfPro++;
			if (code == 27)
			{				
				rtn = S_Let(0);
				return 0;
			}
			GetNext();
			switch (code)
			{
			case 27:rtn = S_Let(0); break;
			case 13:rtn = S_If(); break;
			case 26:rtn = S_While(); break;
			case 10:rtn = S_Repeat(); break;
			case 12:rtn = S_For(); break;
				//case 2:rtn = S_Begin(); break;
			default:Error1(6); break;
			}
			GetNext();
			LineOfPro++;
			while (code != 50)
			{
				switch (code)
				{
				case 27:rtn1 = S_Let(0); break;
				case 13:rtn1 = S_If(); break;
				case 26:rtn1 = S_While(); break;
				case 10:rtn1 = S_Repeat(); break;
				case 12:rtn1 = S_For(); break;
					//case 2:rtn = S_Begin(); break;
				default:Error1(6); break;
				}
				LineOfPro++;
				GetNext();				
			}
			BackPatch(True_address, rtn);

		}
		else{
			Error1(WHILE);
			return 0;
		}
	}
	BackPatch(False_address, LineOfEqu + 1);
	return temp;
}

/********************for语句分析***************************/

int S_For()
{
	int rtn = 0, jmp_addr,jmp_addr1;
	int addr2;
	if (code == 12)//for
	{
		GetNext();
		if (code == 32)//左括号
		{
			GetNext();
		}
		else
		{
			Error1(53); return 0;
		}			
		rtn = S_Let(1);//调用赋值语句分析
		if (code == 42)//分号之后获取判断条件 如;i<a
		{
			GetNext();
			if (code == 27)
			{
				
				GetNext();
				if (code == 43 || code == 44 || code == 41 || code == 46 || code == 47 || code == 48)
				{
					GetNext();
					if (code == 27 || code == 28)//标识符或整常数
					{
						addr2 = address;
						GetNext();
						while (code != 33)//右括号
						{
							GetNext();
						}
						GetNext();
						if (code == 42)//分号
						{
							GetNext();
							LineOfPro++;
						}
						if (code == 49)//{
						{
							LineOfPro++;
							GetNext();
							switch (code)
							{
							case 27:jmp_addr = S_Let(0); break;
							case 13:jmp_addr = S_If(); break;
							case 26:jmp_addr = S_While(); break;
							case 20:jmp_addr = S_Repeat(); break;
							case 12:jmp_addr = S_For(); break;
							case 49:jmp_addr = S_Begin(); break;
							default: Error1(6); return 0;
							}
							LineOfPro++;
							GetNext();
							while (code != 50)
							{
								switch (code)
								{
								case 27:jmp_addr1 = S_Let(0); break;
								case 13:jmp_addr1 = S_If(); break;
								case 26:jmp_addr1 = S_While(); break;
								case 20:jmp_addr1= S_Repeat(); break;
								case 12:jmp_addr1 = S_For(); break;
								case 49:jmp_addr1= S_Begin(); break;
								default: Error1(6); return 0;
								}
								LineOfPro++;
								GetNext();
							}
							EquPush(2, let_count, ONE, let_count);
							EquPush(10, let_count, addr2, jmp_addr);

						}
						else{ Error1(FOR); return 0; }
					}
				}				
			}
			else { Error1(FOR); return 0; }
		}
		else{ Error1(FOR); return 0; }
	}
	return rtn;
}

/*****************repeat语句分析*********************/
int S_Repeat()
{
	int rtn = 0;
	int b_addr = 0;
	if (code == 42) { GetNext(); LineOfPro++; }
	if (code == 20)
	{
		GetNext();
		switch (code)
		{
		case 27:rtn = S_Let(0); break;
		case 14:rtn = S_If(); break;
		case 26:rtn = S_While(); break;
		case 20:rtn = S_Repeat(); break;
		case 12:rtn = S_For(); break;
		case 2: rtn = S_Begin();
		default: Error1(6); return 0;
		}
		if (code == 42)      /*;号吗? */
			LineOfPro++;
		GetNext();
	}
	else { Error1(5l); return 0; }
	if (code == 24)
	{
		B_Init();
		b_addr = B_Analize();
		EquPush(11, b_addr, ZERO, rtn);
	}
	else{ Error1(REPEAT); return 0; }
	return rtn;
}

/*****************  布尔表达式分析  ***********************/
int  B_Analize()//布尔表达式分析入口
{
	int rtn = 0;
	rtn = B_OR();
	if (expr[now_addr].cod != 0)
		Error1(12);
	return rtn;
}

void B_Init()//获取运算符
{
	int flag = 1;
	InitStack();//初始化存放四元式的数组，即桟
	while (flag)
	{
		GetNext();//获取token表单词
		switch (code)
		{
		case 27: Push(code, address); break;//标识符
		case 28: Push(code, address); break;//整常数
		case 15: Push(code, address); break;//not
		case 1:  Push(code, address); break;//and
		case 43: Push(code, address); break;//<
		case 44: Push(code, address); break;//<=
		case 41: Push(code, address); break;//!=
		case 46: Push(code, address); break;//=
		case 47: Push(code, address); break;//>
		case 48: Push(code, address); break;//>=
		case 17: Push(code, address); break;//or
		case 23: Push(code, address); break;//true
		case 11: Push(code, address); break;//false
		default:flag = 0; break;
		}
	}
}


int B_OR()
{
	int rtn = 0;
	int t1;
	rtn = L_AND();//调用L_AND函数，返回地址
	t1 = rtn;
	rtn = B1_OR(t1);//地址做参数调用B1_OR函数
	return rtn;
}

int B1_OR(int a)
{
	int t1, rtn;
	rtn = a;
	if (expr[now_addr++].cod == 19)//19代表real ,已改成double
	{
		int op = 2;//操作码是+
		int op2 = L_AND();//获取第二操作数
		t1 = NewTemp();//获取临时地址
		EquPush(op, rtn, op2, t1);//四元式存入数组
		rtn = t1;
		if (expr[now_addr].cod == 19)//递归条件
			rtn = B1_OR(t1);
	}
	return rtn;
}

int L_AND()
{
	int rtn = 0;
	int t1;
	rtn = M_NOT();//调用M_NOT函数
	t1 = rtn;
	rtn = L1_AND(t1);//利用M_NOT函数传回的结果作为参数调用L1_AND()函数
	return rtn;
}

int L1_AND(int a)
{
	int t1, rtn;
	rtn = a;
	if (expr[now_addr++].cod == 1)//cod代表的是and
	{
		int op = 4;//操作码是 *乘号
		int op2 = M_NOT();//第二操作数,调用函数M_NOT()
		t1 = NewTemp();//生成临时地址
		EquPush(op, rtn, op2, t1);//四元式存进数组
		rtn = t1;//将临时地址赋给rtn
		if (expr[now_addr].cod == 1)//递归函数
			rtn = L1_AND(t1);
	}
	return rtn;
}

int M_NOT()
{
	int rtn = 0;
	int temp = 0;
	int op2 = 0;
	if (expr[now_addr].cod == 17)//当前地址的机内码代表or
	{
		int op = 3;//操作码是 - 减号
		now_addr++;//当前地址加1
		op2 = K_END();//第二操作数调用K_END()函数
		temp = NewTemp();//产生临时变量
		EquPush(op, ONE, op2, temp);
	}
	else {
		rtn = K_END();
		temp = rtn;
	}
	return temp;//返回变量地址
}

int K_END()
{
	int rtn = 0;//返回地址
	int temp, a;
	temp = 0;
	switch (expr[now_addr++].cod)
	{
	case 27://标识符
		temp = NewTemp();//临时地址
		a = K_CMP();//调用关系表达式运算
		if (a){ rtn = a; break; }//非0数为真，为0时假
		else{
			EquPush(1, expr[now_addr - 1].addr, 0, temp);//将当前地址的上一个地址四元式放进数组，即让temp赋值给上一个地址
			rtn = temp; break;
		}
	case 28://整常数
		temp = NewTemp();
		a = K_CMP();
		if (a) { rtn = a; break; }
		else{
			EquPush(1, expr[now_addr - 1].addr, 0, temp);
			rtn = temp; break;
		}
	case 32://左括号
		rtn = B_Analize();//再次调用布尔表达式分析函数
		if (expr[now_addr].cod != 40)//冒号
		{
			Error1(B_EXP);//表达式出错
			rtn = ZERO;
		}
		break;
	case 11://false
		rtn = ZERO; break;
	case 23://true
		rtn = ONE; break;
	default:break;
	}
	return rtn;
}

int K_CMP()//处理关系表达式
{
	int rtn = 0;
	int t1, t2;
	t1 = NewTemp();//temp_count--,temp_count是全局变量，作为临时变量，即中间运算的结果地址，在主函数中初始化为0
	t2 = NewTemp();//t2=t1-1
	if ((expr[now_addr].cod > 42) && (expr[now_addr].cod < 49))//< <= != = >  >=
	{
		EquPush(3, expr[now_addr - 1].addr, expr[now_addr + 1].addr, t1);//将关系运算符的前后的标识符存进数组
		switch (expr[now_addr].cod)
		{
		case 43:EquPush(9, t1, ZERO, LineOfEqu + 4); break;//<  
		case 44:EquPush(8, t1, ZERO, LineOfEqu + 4); break;//<=
		case 41:EquPush(11, t1, ZERO, LineOfEqu + 2); break;//!=
		case 46:EquPush(11, t1, ZERO, LineOfEqu + 4); break;//=
		case 47:EquPush(10, t1, ZERO, LineOfEqu + 4); break;//>
		case 48:EquPush(7, t1, ZERO, LineOfEqu + 4); break;//>=
		}
		EquPush(1, ONE, 0, t2);//1代表的是'='赋值号
		EquPush(6, 0, 0, LineOfEqu + 3);//6代表的是j，应该是跳转
		EquPush(1, ZERO, 0, t2);
		rtn = t2;
		now_addr++;//当前地址加1
	}
	return rtn;//返回临时地址
}

/*********************出口回填函数************************/
void BackPatch(int addr, int addr2)
{
	Equ[addr].result = addr2;
	return;
}
int EquPush(int op, int a, int b, int r)/*四元式存进数组*/
{
	int i = LineOfEqu;
	Equ[LineOfEqu].op = op;
	Equ[LineOfEqu].op1 = a;
	Equ[LineOfEqu].op2 = b;
	Equ[LineOfEqu].result = r;
	LineOfEqu++;
	return i;
}











/*              目标代码生成相关函数*/
void Tmain()
{
	ONE1[0] = '1';
	ONE1[1] = '\0';
	ZERO1[0] = '0';
	ZERO1[1] = '\0';
	Target(); 
	//return 0;
}

void Target()
{
	char ch;
	int i = 0, m, op = 0;
	int j, control, flag = 0;
	char filename[30];
	InitTarget();//调用初始化函数
	control = 0;
	if ((AssemFout = fopen("s.asm", "wt+")) == NULL){
		printf("Cannot open the %s\n", filename);
		exit(1);
	}
	for (m = 0; m < gen_count; m++){
		for (j = 1; j < 12; j++){
			flag = strcmp(operation[j].opp, GenStack[m].op);//比较四元式数组的操作码跟哪种运算符相等
			if (!flag)//flag非0
			{
				GenStack[m].code = operation[j].code;//将其运算符的序号赋值给四元式数组
				op = operation[j].code;//将其赋值给op
				SortDGA(m, op);//调用函数
				break;
			}
		}
	}
	fprintf(AssemFout, "data segment\n");//将目标代码写入文件
	for (i = 4; i < token_count; i++){
		if (i>TokenTable[0].label + 3)
			break;
		if (TokenTable[i].cod1 == 27)
			fprintf(AssemFout, "%s  dw  ?;\n", TokenTable[i].name);
	}
	fprintf(AssemFout, "display db 'this is an example of simple compiler.$'\n");
	fprintf(AssemFout, "data ends\n");
	fprintf(AssemFout, "code segment\n");
	fprintf(AssemFout, "assume cs:code,ds:data\n");
	fprintf(AssemFout, "main proc   far\n");
	fprintf(AssemFout, "start:push ds;  \n");
	fprintf(AssemFout, "       mov  ax,0;\n");
	fprintf(AssemFout, "       push ax;\n");
	fprintf(AssemFout, "       mov  ax,data;\n");
	fprintf(AssemFout, "       mov  ds,ax;\n");
	for (i = 0; i < gen_count; i++){
		if (GenStack[i].out_port == 1 && GenStack[i].label < gen_count + 1)
			fprintf(AssemFout, "lab %d:\n", GenStack[i].label);
		Produce(GenStack[i].code, GenStack[i].addr1, GenStack[i].addr2, GenStack[i].result);
	}
	fprintf(AssemFout, "lab %d:\n", gen_count + 1);
	fprintf(AssemFout, "       mov  ah,9h;\n");
	fprintf(AssemFout, "       mov  dx,offset display;\n");
	fprintf(AssemFout, "      int 21h;\n");
	Produce(12, 0, 0, 0);
	fprintf(AssemFout, "main endp\n");
	fprintf(AssemFout, "code ends\n");
	fprintf(AssemFout, "end start\n");
	printf("结果如下：\n");
	rewind(AssemFout);
	ch = fgetc(AssemFout);
	while (ch != EOF){
		putchar(ch);
		ch = fgetc(AssemFout);
	}
	fclose(AssemFout);
	return;
}

void InitTarget()
{
	int a, j, i = 0;
	char filename[30];
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("~~~~~~~~~目标代码生成器~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("输入源文件名：");//equ.txt symble.txt
	for (;;){
		scanf("%c", &filename[i]);
		if (filename[i] == 10)
			break;
		i++;
	}
	filename[i] = '\0';
	if ((GenFin = fopen(filename, "rt")) == NULL){
		printf("cannot open the file \n", filename); 
		exit(1);
	}
	if ((TokenFin1 = fopen("symble.txt", "rt")) == NULL){
		printf("cannot open the token file \n");
		exit(1);
	}
	for (a = 0; a < STACKLONG; a++){
		for (j = 0; j < 4; j++)
			GenStack[a].op[j] = '\0';
		GenStack[a].out_port = 0;
	}
	InitOp();//调用初始化函数
	bx.number = 1;//初始化寄存器的结构
	bx.allocate = 0;
	bx.name[0] = 'b';
	bx.name[1] = 'x';
	bx.name[2] = '\0';
	cx.number = 2;
	cx.allocate = 0;
	cx.name[0] = 'c';
	cx.name[1] = 'x';
	cx.name[2] = '\0';
	dx.number = 3;
	dx.allocate = 0;
	dx.name[0] = 'd';
	dx.name[1] = 'x';
	dx.name[2] = '\0';
	gen_count = 0;
	i = 0;
	a = GetNextGen();//a代表是否已经到四元式序列结尾	
	while (a>0)
	{
		GenStack[i].label = CurrentGen.label;//将从文件获取的四元式序列赋值给结构数组
		GenStack[i].addr1 = CurrentGen.addr1;//第一操作数
		GenStack[i].addr2 = CurrentGen.addr2;//第二操作数
		GenStack[i].result = CurrentGen.result;//结果地址
		strcpy(GenStack[i].op, CurrentGen.op);//运算符
		i++;
		gen_count++;//四元式数量
		a = GetNextGen();
	}
	i = 0; 
	a = 1;
	while (a > 0)
	{
		fscanf(TokenFin1, "%d%d%s\n", &TokenTable[i].label, &TokenTable[i].cod1, TokenTable[i].name);//从symble表读取标识符等单词
		if (TokenTable[i].label == 0)break;
		i++;
		token_count++;
	}
	fclose(TokenFin1);
	fclose(GenFin);
}

int GetNextGen()//从四元式文件中获取四元式赋值给结构数组
{
	char a[2], b[2];//存放左右括号
	int d = 1;
	fscanf(GenFin, "%d%s%s%d%d%d%s\n", &CurrentGen.label, a, CurrentGen.op, &CurrentGen.addr1, &CurrentGen.addr2, &CurrentGen.result, b);
	if (CurrentGen.op[0] == '0')//即到四元式序列结尾
		d = 0;
	return d;
}

void InitOp()//初始化四元式出现的运算符和0
{
	int i, j;
	for (i = 0; i < 12; i++){
		operation[i].code = i;
		for (j = 0; j < 4; j++)
			operation[i].opp[j] = '\0';
	}
	operation[0].opp[0] = 0;//0

	operation[1].opp[0] = '=';//=

	operation[2].opp[0] = '+';//+

	operation[3].opp[0] = '-';//-

	operation[4].opp[0] = '*';//*

	operation[5].opp[0] = '/';///

	operation[6].opp[0] = 'j';//j

	operation[7].opp[0] = 'j';//j<
	operation[7].opp[1] = '<';

	operation[8].opp[0] = 'j';//j=
	operation[8].opp[1] = '=';

	operation[9].opp[0] = 'j';//j>=
	operation[9].opp[1] = '>';
	operation[9].opp[2] = '=';

	operation[10].opp[0] = 'j';//j<=
	operation[10].opp[1] = '<';
	operation[10].opp[2] = '=';

	operation[11].opp[0] = 'j';//j!=
	operation[11].opp[1] = '!';
	operation[11].opp[2] = '=';

	return;
}

void Produce(int op, int a, int b, int r)
{
	switch (op){
	case 1:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, " mov %s, ax;为%s赋值为%s\n", GetName(r), GetName(r), GetName(a));
		break;
	case 2:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   add  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   mov  %s,ax;做加法\n", GetName(r));
		break;
	case 3:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   mov  %s,ax;做减法\n", GetName(r));
		break;
	case 4:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   mov  cx,%s;\n", GetName(b));
		fprintf(AssemFout, "   mul  cx;\n");
		fprintf(AssemFout, "   mov  %s,ax;做乘法\n", GetName(r));
		break;
	case 5:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   mov  cx,%s;\n", GetName(b));
		fprintf(AssemFout, "   div  cx;\n");
		fprintf(AssemFout, "   mov  al,ah;\n");
		fprintf(AssemFout, "   mov  ah,0;\n");
		fprintf(AssemFout, "   mov  %s,ax;做除法\n", GetName(r));
		break;
	case 6:
		fprintf(AssemFout, "   jmp  lab%d;跳转至lab%d\n", r, r);
		break;
	case 7:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   js  lab%d;跳转至 %d\n", r, r);
		break;
	case 8:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   je  lab%d;跳转至 %d\n", r, r);
		break;
	case 9:
		fprintf(AssemFout, "   mov  ax,%s;\n", a);
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   je  lab%d;跳转至 %d\n", r, r);
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   jns  lab%d;跳转至 %d\n", r, r);
		break;
	case 10:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   jb  lab%d;跳转至 %d\n", r, r);
		break;
	case 11:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   jne  lab%d;跳转至 %d\n", r, r);
		break;
	case 12:
		fprintf(AssemFout, "   mov  ax,4c00h;\n");
		fprintf(AssemFout, "   int  21h;\n");
		break;
	default:break;
	}
	return;
}

char *GetName(int a)
{
	int j, cr;
	if (a == 5001)
		return ONE1;
	if (a == 5000)
		return ZERO1;
	for (j = 1; j < token_count; j++){
		if (TokenTable[j].label == a)
			return TokenTable[j].name;
	}
	if ((bx.allocate == 0) || (bx.allocate == a)){
		bx.allocate = a;
		return bx.name;
	}
	else{
		if ((dx.allocate == 0) || (dx.allocate == a)){
			dx.allocate = a;
			return dx.name;
		}
		else{
			cr = (43 + a) % 2;
			switch (cr){
			case 0:
				bx.allocate = a;
				return bx.name;
			case 2:
				dx.allocate = a;
				return dx.name;
			}
		}
	}
	return bx.name;
}

void SortDGA(int address, int op_code)
{
	int addr, op_cod, next_p1 = 0;
	op_cod = op_code;//各种操作符的序号
	addr = address;//四元式序号
	switch (op_cod){
	case 6://j即直接跳转
		next_p1 = GenStack[addr].result;//当前四元式的结果地址，即四元式序号
		GenStack[next_p1 - 1].out_port = 1;//跳转到的四元式的上一个四元式是基本块的入口
		break;
	case 7://j<小于时跳转
		next_p1 = GenStack[addr++].result;//下一个四元式的结果地址，即跳转到的四元式 的序号
		GenStack[next_p1 - 1].out_port = 1;//
		GenStack[addr - 1].out_port = 1;
		break;
	case 8://j=等于时跳转
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 9://j>=大于等于时跳转
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 10://j<=小于等于时跳转
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 11://j!=不等于时跳转
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	default:break;
	}
	return;
}
