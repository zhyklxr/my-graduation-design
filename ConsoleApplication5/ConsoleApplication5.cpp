// ConsoleApplication5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>




/*     �ʷ�������ض���*/
#define N 100
#define LEN 70
typedef struct token{    /*���ڴ�ŵ��ʷ��ŵĻ��ڱ�ʾ*/
	int label;        //�������
	char name[30];    //������
	int code;	     //���ʻ�����
	int addr;        //��ڵ�ַ
}token;
typedef struct KeyWord{
	char name[30];
	int code;
}KeyWord;
typedef struct symble{  /*���ڴ��Դ�����б�ʶ������������*/
	int number;      //���
	int type;        //������
	char name[30];   //���ʱ���
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
int strcmp(char *s, char *t){   //�ȽϺ���
	for (; *s == *t; s++, t++)
		if (*s == 0)
			return 0;
	return 1;
}





/*         �﷨������ض���*/
typedef struct stack//�൱��token��
{
	char name[20];
	int cod;
	int addr;
}stack;
typedef struct equ//��Ԫʽ�ṹ
{
	int op;//������
	int op1;//��һ������
	int op2;//�ڶ�������
	int result;//��������ķ��ŵ�ַ
}equ;
typedef struct var//����˵����ʶ���ṹ
{
	char name[20];//������
	int addr;//��ַ
	int type;//����
	int value;//ֵ��
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
FILE *EquFout;//equ�ļ���ָ��
int code;//������
int address;//��ַ
int LineOfPro;//����ǰ��������
int LineOfEqu;//��Ԫʽ�����
stack var_list[EXP_LEN];//��ű�ʶ���Ľṹ����
stack expr[EXP_LEN];//��Ϊ�C���code��address������
equ Equ[EQU_LEN];//�����Ԫʽ�Ľṹ����
var TempList[EXP_LEN];//��ű����Ľṹ����
char ID[20];//���ڴ��token��ȡ���ĵ�������
int var_count1;//��ʶ������
int len_count;
int pos;//��Ϊ�C���±�
int now_addr;//��ǰ�ĵ��ʵ�ַ
int temp_count;
int error_count1;//��������
int let_count;//�����������������ѭ�����for,while�Ļ�������������ʼ������ʾ���ĵ�ַ
int x_count; //ѭ���������Ԫʽ������
int E_Contrl;//���ƼӼ��˳�
int E_rtn;//����Ԫʽ�������ʱ����Ԫʽ���
int True_address;//��ȷ�ĵ�ַ
int False_address;//����ĵ�ַ
int gen_pos;//����Ԫʽд���ļ�ʱ�������±�
int EquPush(int op, int a, int b, int r);/*��Ԫʽ�������*/
void GetNext();//��ȡtoken����
void Error1(int num);//�﷨����
int gen(int op, int a, int b, int r);//�����Ԫʽ���ļ�
void Declear();//����˵��
void InitStack();//��ʼ���C
void yfyymain();
void Push(int cod, int addr);//��ջ
int NewTemp();//������ʱ������������ڵ�ַ
void BackPatch(int addr, int addr2);

/********�������ʽ��������************************/
int B_Analize();//����B_OR()����
void B_Init();//�������ѹ�C
int B_OR();//����L_AND��B1_OR����
int B1_OR(int a);//������
int L_AND();//����M_NOT��L1_AND����
int L1_AND(int a);// �����룬����M_NOT����
int M_NOT();//����� M->notM|K  ������K_END����
int K_END();//�����������   K->i|false|true|(B)  ������K_CMP����
int K_CMP();//�����ϵ���ʽ K->iSi    !=|=|<=|>=|>|<

/********�������ʽ��������************************/
int E_Analize();//����E_init��E_AddSub����
void E_Init();//���������ʽ��ջ
int E_AddSub();//����Ӽ�������T_MulDiv��E1_AddSub����
int E1_AddSub(int a);//����Ӽ�������T_MulDiv����
int T_MulDiv();//����˳�������F_Number����
int T1_MulDiv(int a);//����˳�
int F_Number();//�鿴�Ƿ������Ż�������ȡ��Ҫ����ı��ʽ


/********����������******************************/
void parser();//�ܺ���
int S_Let(int a);//��ֵ��������
int S_If();//if��亯��
int S_While();//while���
int S_For();//for���
int S_Repeat();//�ظ����
int S_Begin();//��������������
int L_Analize();//�ж��������







/*     Ŀ�����������ض���*/
#define STACKLONG 100
#define TOKENLENGTH 1024
typedef struct GenStruct{
	int label;//���
	char op[4];//��Ԫʽ�Ĳ�����
	int code;//������
	int addr1;//��һ�������ĵ�ַ
	int addr2;//�ڶ��������ĵ�ַ
	int result;//�����ַ
	int out_port;//��Ԫʽ�Ƿ�Ϊ���������ڣ�����1��������Ϊ0
}GenStruct;//��Ԫʽ�ṹ����

typedef struct Token{
	int label;//���
	char name[20];//������
	int cod1;//������
}Token;//��ȡsymble���еı�ʶ���Ľṹ

typedef struct Oper{
	int code;
	char opp[4];//���������
}Oper;//

typedef struct regis{
	int number;
	char name[3];
	int allocate;
}regis;//�Ĵ����ṹ

Token TokenTable[TOKENLENGTH];
GenStruct GenStack[STACKLONG];
GenStruct CurrentGen;//�����Ԫʽ
Oper operation[12];//�����Ԫʽ�г��ֵ�������Ͳ�������0
regis bx, cx, dx;
FILE *GenFin;
FILE *TokenFin1;
FILE *AssemFout;
char ONE1[2];//������Ԫʽ��one��zero�Ķ���
char ZERO1[2];
int gen_count, token_count;//��Ԫʽ��������ʶ������
void InitOp();//��ʼ��������Ԫʽ�е��������0����ṹ�����Ա�ʹ��
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
	printf("^^^^^^^^^^^^^^^^������^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^��һ���֣��ʷ�����^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^�ڶ����֣��﷨�������^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^�������֣�Ŀ���������^^^^^^^^^^^^^^^^^\n");
	Wordmain();
	yfyymain();
	Tmain();
	system("pause");
	getchar();
	return 0;
}  

void Wordmain()         //�ʷ�����������
{
	int i = 0, j = 0;
	code_count = 0;//code��
	LineOfPro1 = 0;//����
	var_count = 0;//��ʶ��������
	addr_count = 1;//��ַ��
	label_count = 1;//������
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
	error_count = 0;//������
	ScannerIint();                         //���ó�ʼ��������
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^�ʷ�������^^^^^^^^^^^^^^\n");
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	printf("����Դ�ļ���:");
	for (;;){
		scanf("%c", &filename[i]);
		if (filename[i] == 10)
			break;
		i++;
	}
	filename[i] = '\0';
	if ((SourceFin = fopen(filename, "rt")) == NULL){
		printf("%s �ļ��޷��򿪣�\n", filename);
		exit(1);
	}
	if ((TokenFout = fopen("token.txt", "wt+")) == NULL){
		printf("token.txt �ļ��޷��򿪣�\n");
		exit(1);
	}
	if ((SymbleFout = fopen("symble.txt", "wt+")) == NULL){
		printf("symble.txt �ļ��޷��򿪣�\n");
		exit(1);
	}
	ch = fgetc(SourceFin);     //��ȡ��һ���ַ�
	/*48-57λ����0-9��ASCII��,65-90��26����д��ĸ��ASCII�룬97-122��26��Сдд��ĸ��ASCII��*/
	while (ch != EOF)  //����Դ�����ļ���ÿһ���ַ�
	{
		for (i = 0; i < 30; i++)//ÿһ�ζ���CToken.name[]��ʼ��Ϊ\0
		{
			CToken.name[i] = '\0';
		}
		if ((ch>47) && (ch<58))//�ַ�Ϊ����
		{
			IsShuZi();
		}
		else
		{
			if (((ch>64) && (ch<91)) || ((ch>96) && (ch<123)) || ch == '_')//�ַ�Ϊ��ĸ��_
			{
				IsBLOrBS();
			}
			else
			{
				if (ch == '/')    IsZhuShi();//�ַ���/
				else if (ch == '\'') IsZiFuChuan();
				else              IsOther();//�������
			}
		}
	}
	fclose(SourceFin);
	fclose(TokenFout);
	fclose(SymbleFout);
	printf("�������!\n");
}
void ScannerIint()           /*��ʼ������*/
{
	int i = 1, k = 0;
	if ((KeyFin = fopen("in.txt", "rt")) == NULL){  //in.txtΪ������
		printf("in.txt �ļ��޷��򿪣�\n");
		exit(1);
	}
	for (i = 0; i < 60; i++)
		for (k = 0; k < 30; k++)
			key[i].name[k] = '\0';
	for (i = 1; i < 60; i++){    /*��������KeyFin��*/
		fscanf(KeyFin, "%s%d", key[i].name, &key[i].code);
	}
	fclose(KeyFin);

}
void IsShuZi()     /*���ִ���*/
{
	int k = 0, flag = 0;
	char ch1;
	while (((ch>47) && (ch<58)))//0-9����
	{
		CToken.name[k++] = ch;//�ȸ�ֵȻ��k+1
		ch = fgetc(SourceFin);
		if (ch == '.'){
			flag = 1;
			break;
		}
	}
	CToken.code = 28;
	CToken.addr = addr_count++;
	CToken.label = label_count++;
	if (flag)//��flag=1ʱ����
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
void IsBLOrBS()          /*�����ͱ�ʶ����*/
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
		CToken.addr = addr_count++;//�ȸ�ֵ����1
	}
	CToken.label = label_count++;
	Out();
}
void IsZhuShi()             /*ע�ͺͳ�������*/
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
		CToken.name[0] = '/';       /*�������ע�ͣ���������*/
		CToken.code = 30;
		CToken.addr = -1;
		CToken.label = label_count++;
		Out();
	}
}
void IsZiFuChuan()           /*�ַ�������*/
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
void IsOther()               /*�����������*/
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
void Out()               /*���ģ��*/
{
	int k, flag, i = 0;
	/*��ѯ����д�ַ���*/
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
void Error(int a)              /*������*/
{
	error_count++;
	switch (a){
	case 1:
		printf("error %3d �Ƿ��ַ��� %d�У�\n\n", error_count, LineOfPro1 + 1);
		break;
	case 2:
		printf("error %3d ʵ���������� %d�У�\n\n", error_count, LineOfPro1 + 1);
		break;
	case 3:
		printf("error %3d ������%d��,û��ƥ���ע�ͷ� '*/'\n\n", error_count, LineOfPro1 + 1);
		break;
	case 4:
		printf("error %3d ������%d��,������������ \n\n", error_count, LineOfPro1 + 1);
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
		flag = strcmp(CSymble.name, SymbleList[i].name);//�ж�����name�Ƿ����
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
	error_count1 = 0;//�﷨����������
	LineOfPro = 1;//������������
	temp_count = 0;
	now_addr = 0;//��ǰ��ַ
	//len_count = 0;
	InitStack();//�C
	for (i = 0; i < EXP_LEN - 2; i++)
	{
		TempList[i].addr = 0;
		TempList[i].addr -= i;
		TempList[i].name[0] = 'b';
		TempList[i].name[1] = 'x';
	}
	/*������Ԫʽ�ļ�*/
	EquFout = fopen("equ.txt", "wt+");
	if ((TokenFin = fopen("token.txt", "rt")) == NULL)
	{
		printf("Cannot open file token.txt strike any key exit!");
		exit(1);
	}
	printf("           *           ****            *\n");
	printf("          --------�﷨��������----------\n");
	printf("           *           ****            *\n");
	printf(" �﷨������ʼ����ȴ�......\n");
	GetNext();
	while (code != 18)    //�ж�classǰ������η�
	{
		GetNext();
	}
	if (code == 18)
	{  
		GetNext();
		if (code == 27)//����
		{
			GetNext();
			if (code == 49)//�ж�{
			{
				GetNext();
				LineOfPro++;
				while (code != 56)
				{   //�ж�main �ؼ���ǰ�����δ�
					GetNext();
				}
				if (code == 56){
					GetNext();
					if (code == 32){//�жϣ�
						GetNext();
						if (code == 27){//�ж�String
							GetNext();
							if (code == 27){//�ж�args
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
														Declear();           //����˵������
														LineOfPro++;

													}
													if (code == 10 || code == 12 || code == 13 || code == 26||code==27)
													{
														S_Begin();          //����������
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
		printf("����%6d������С��ѽ��\n", error_count1);
	else
		printf("�����ڴ��󣬷����ɹ���\n");
	fclose(TokenFin);
	printf("������£�\n");
	rewind(EquFout);
	ch1 = fgetc(EquFout);
	while (ch1 != EOF)
	{
		printf("%c", ch1);
		ch1 = fgetc(EquFout);
	}
	fclose(EquFout);

}

void InitStack()//��ʼ���C
{
	int i, j;
	pos = 0;//�C���±�
	now_addr = 0;//�ǰ��ַΪ0
	for (i = 0; i < EXP_LEN; i++)//��expr�Ľṹ�����ʼ��Ϊ��
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
	case 1:printf("��ʽ������1����'class'\n"); break;
	case 2:printf("��ʽ������1���ٳ�����\n"); break;
	case 3:printf("��%4d�У�˵��������\n", LineOfPro); break;
	case 4:printf("��%4d�У���ֵ��������ƥ�����������\n", LineOfPro); break;
	case 5:printf("��%4d�У���end����ƥ��\n", LineOfPro); break;
	case 6:printf("��%4d�У�������\n", LineOfPro); break;
	case 7:printf("��%4d�У����ʽ������\n", LineOfPro); break;
	case 8:printf("��%4d�У�if������\n", LineOfPro); break;
	case 9:printf("��%4d�У�while������\n", LineOfPro); break;
	case 10:printf("��%4d�У�for������\n", LineOfPro); break;
	case 11:printf("��%4d�У�repeat������\n", LineOfPro); break;
	case 12:printf("��%4d�У��������ʽ����\n", LineOfPro); break;
	case 49:printf("����ȱ��'class',���ִ���\n"); break;
	case 50:printf("δд�κ���䣡\n"); break;
	case 51:printf("��%4d�У�ȱ��';'����\n", LineOfPro); break;
	case 52:printf("��%4d�У�ȱ��'{'����\n", LineOfPro); break;
	case 53:printf("��%4d�У�ȱ��'}'����\n", LineOfPro); break;
	case 54:printf("��ʽ������2��ȱ��main����\n"); break;
	default:printf("δ֪���󣡣�\n"); printf("\n"); break;
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

	case 1:fprintf(EquFout, "%5d", LineOfEqu);//��ֵ=
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

	case 2:fprintf(EquFout, "%5d", LineOfEqu);//�ӷ�
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

	case 3:fprintf(EquFout, "%5d", LineOfEqu);//����-
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

	case 4:fprintf(EquFout, "%5d", LineOfEqu);//�˷�*
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

	case 5:fprintf(EquFout, "%5d", LineOfEqu);//����/
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

/************************˵������������**************************/
void Declear()

{
	int a = 1, i = 0, j = 0;
	int df;	
	InitStack();
	i = 0;
	if (code == 3 || code == 6 || code == 14 || code == 19)   //����������Boolean,char,int,double��
	{
		GetNext();
		if (code == 37 || code == 42)    //Ϊ���Ż��߷ֺ�
		{
			Error1(3);
			return;
		}
		while (code != 42)  //����Ϊ�ֺţ����ջ
		{
			Push(code, address);
			GetNext();
		}
		i = 0;
		var_count1 = 0;
		while (pos > 0)
		{
			if (expr[i].cod == 27)    //Ϊ��ʶ��
			{
				i++;
				pos--;
				var_count1++;
			}
			else
			{
				if (expr[i].cod == 37)  //Ϊ����
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
						if (expr[i].cod == 42)  //Ϊ�ֺ�
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
		if (code == 37 || code == 42)    //Ϊ���Ż��߷ֺ�
		{
			Error1(3);
			return;
		}
		while (code != 42)
		{
			if (code == 27)//��ʶ��
			{
				var_count1++;
				D_code = code;
				D_addr = address;
			}
			GetNext();
			if (code == 46)//��ʶ�����Ǹ�ֵ=��
			{
				S_Let(0);
			}
			else 
			{
				if (code == 37)//��ʶ�����Ƕ���
				{
					GetNext();
				}
				if (code == 42)//��ʾ�����Ƿֺ�
				{
					LineOfPro++;
				}
				
			}
		}*/
		
		/***�޸ķ��ű�***/
		for (i = 0; i < EXP_LEN; i++)//��ʼ��var_List����
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

/******************��ֵ������**********************/
int S_Let(int a)
{
	int addr, flag, rtn,t1;
	flag = 0;
	rtn = 0;
	InitStack();
	if (code == 27)
	{
		addr = address;
		if (a)//a��0ʱ
			let_count = addr;
		var_count1++;
		GetNext();//
		if (code == 46)//��ֵ��=
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

/********�������ʽ����************************/
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

void E_Init()      //���������루 ��+ - * /��ջ
{
	int flag = 1, i,j;
	pos = 0;
	E_rtn = 0;
	E_Contrl = 1;//��ʼ��
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
		case 27:Push(code, address); break;//��ʶ��
		case 28:Push(code, address); break;//������
		case 29:Push(code, address); break;//ʵ����
		case 32:Push(code, address); break;//��
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

int NewTemp()//����һ����ʱ�������������ڷ��ű��е���ڵ�ַ
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
	if (expr[now_addr].cod == 35 || expr[now_addr].cod == 36)    //�Ӽ�
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
	if (expr[now_addr].cod == 34 || expr[now_addr].cod == 39)   //�˳�
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
	if (expr[now_addr].cod == 32)   //Ϊ�� ������
	{
		now_addr++;
		rtn = E_AddSub();
		if (expr[now_addr++].cod != 40)           //û����ʲô��˼
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

void Push(int cod, int addr)//ѹ�C��pos�±���1
{
	int j;
	expr[pos].addr = addr;
	expr[pos].cod = cod;
	for (j = 0; j < 20; j++)
		expr[pos].name[j] = ID[j];
	pos++;
	//return;
}

/*********����������************/
int S_Begin()
{
	int rtn = 0;
	if (code == 42)     //Ϊ�ֺ�
	{
		GetNext();
		LineOfPro++;
	}
	if (code == 13 || code == 10 || code == 26 || code == 12||code==27) //�������(if else/switch case/do while/for/��ʶ��)
	{
		rtn = L_Analize();
		/*if (code != 50)   //��Ϊ }ʱ
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

/*****************������з���******************/

int L_Analize()
{
	int rtn = 0;
	switch (code)
	{
	case 27:S_Let(0); rtn = E_rtn; break;      //��ֵ���
	case 13:rtn = S_If(); break;               //if���
	case 26:rtn = S_While(); break;            //while ���
	case 10:rtn = S_Repeat(); break;           //switch���
	case 12:rtn = S_For(); break;             //for ���
	default:Error1(6);
		return 0;
	}
	if (code == 42)//�ֺ�
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

/*****************if������******************/
int S_If()
{
	int rtn = 0;
	int temp = 0;
	int as;
	int b_addr = 0, tt = 0;
	if (code == 13)              //Ϊifʱ
	{
		temp = LineOfEqu + 1;
		GetNext();
		if (code == 32)//��
		{
			B_Init();                //�������ջ
			if (code != 33)
			{
				Error1(8);
			}
		}		
		b_addr = B_Analize();    //�����������㣬�������ĵ�ַ
		True_address = EquPush(11, b_addr, ZERO, 1);
		False_address = EquPush(6, 0, 0, 0);
		GetNext();
		if (code == 49 || code == 27)             //������{ ���߱�ʶ��
		{
			LineOfPro++;//������һ��
			GetNext();
				switch (code)
				{
				case 27:rtn = S_Let(0); break;//��ʶ��
				case 13:rtn = S_If(); break;//if
				case 26:rtn = S_While(); break;//while
				case 10:rtn = S_Repeat(); break;//repeat
				case 12:rtn = S_For(); break;//for
				default:Error1(6); break;
				}
				BackPatch(True_address, rtn + 1);
				tt = LineOfEqu + 1;
				as = EquPush(6, 0, 0, tt);
				if (code == 9)             //else ���
				{
					GetNext();
					if (code == 49 || code == 27)             //������{
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

/******************while������**********************/

int S_While()
{
	int rtn = 0,rtn1;
	int temp;
	int b_addr = 0;
	if (code == 42)    //Ϊ�ֺ�ʱ
	{
		GetNext();
		LineOfPro++;
	}
	if (code == 26)      //Ϊwhile���ʱ
	{
		temp = LineOfEqu + 1;
		GetNext();
		B_Init();                //�������ջ
		GetNext();
		b_addr = B_Analize();    //������������
		True_address = EquPush(11, b_addr, ZERO, 1);
		False_address = EquPush(6, 0, 0, 0);
		if (code == 49 || code == 27)             //������{ ���߱�ʶ��
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

/********************for������***************************/

int S_For()
{
	int rtn = 0, jmp_addr,jmp_addr1;
	int addr2;
	if (code == 12)//for
	{
		GetNext();
		if (code == 32)//������
		{
			GetNext();
		}
		else
		{
			Error1(53); return 0;
		}			
		rtn = S_Let(1);//���ø�ֵ������
		if (code == 42)//�ֺ�֮���ȡ�ж����� ��;i<a
		{
			GetNext();
			if (code == 27)
			{
				
				GetNext();
				if (code == 43 || code == 44 || code == 41 || code == 46 || code == 47 || code == 48)
				{
					GetNext();
					if (code == 27 || code == 28)//��ʶ����������
					{
						addr2 = address;
						GetNext();
						while (code != 33)//������
						{
							GetNext();
						}
						GetNext();
						if (code == 42)//�ֺ�
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

/*****************repeat������*********************/
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
		if (code == 42)      /*;����? */
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

/*****************  �������ʽ����  ***********************/
int  B_Analize()//�������ʽ�������
{
	int rtn = 0;
	rtn = B_OR();
	if (expr[now_addr].cod != 0)
		Error1(12);
	return rtn;
}

void B_Init()//��ȡ�����
{
	int flag = 1;
	InitStack();//��ʼ�������Ԫʽ�����飬���C
	while (flag)
	{
		GetNext();//��ȡtoken����
		switch (code)
		{
		case 27: Push(code, address); break;//��ʶ��
		case 28: Push(code, address); break;//������
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
	rtn = L_AND();//����L_AND���������ص�ַ
	t1 = rtn;
	rtn = B1_OR(t1);//��ַ����������B1_OR����
	return rtn;
}

int B1_OR(int a)
{
	int t1, rtn;
	rtn = a;
	if (expr[now_addr++].cod == 19)//19����real ,�Ѹĳ�double
	{
		int op = 2;//��������+
		int op2 = L_AND();//��ȡ�ڶ�������
		t1 = NewTemp();//��ȡ��ʱ��ַ
		EquPush(op, rtn, op2, t1);//��Ԫʽ��������
		rtn = t1;
		if (expr[now_addr].cod == 19)//�ݹ�����
			rtn = B1_OR(t1);
	}
	return rtn;
}

int L_AND()
{
	int rtn = 0;
	int t1;
	rtn = M_NOT();//����M_NOT����
	t1 = rtn;
	rtn = L1_AND(t1);//����M_NOT�������صĽ����Ϊ��������L1_AND()����
	return rtn;
}

int L1_AND(int a)
{
	int t1, rtn;
	rtn = a;
	if (expr[now_addr++].cod == 1)//cod�������and
	{
		int op = 4;//�������� *�˺�
		int op2 = M_NOT();//�ڶ�������,���ú���M_NOT()
		t1 = NewTemp();//������ʱ��ַ
		EquPush(op, rtn, op2, t1);//��Ԫʽ�������
		rtn = t1;//����ʱ��ַ����rtn
		if (expr[now_addr].cod == 1)//�ݹ麯��
			rtn = L1_AND(t1);
	}
	return rtn;
}

int M_NOT()
{
	int rtn = 0;
	int temp = 0;
	int op2 = 0;
	if (expr[now_addr].cod == 17)//��ǰ��ַ�Ļ��������or
	{
		int op = 3;//�������� - ����
		now_addr++;//��ǰ��ַ��1
		op2 = K_END();//�ڶ�����������K_END()����
		temp = NewTemp();//������ʱ����
		EquPush(op, ONE, op2, temp);
	}
	else {
		rtn = K_END();
		temp = rtn;
	}
	return temp;//���ر�����ַ
}

int K_END()
{
	int rtn = 0;//���ص�ַ
	int temp, a;
	temp = 0;
	switch (expr[now_addr++].cod)
	{
	case 27://��ʶ��
		temp = NewTemp();//��ʱ��ַ
		a = K_CMP();//���ù�ϵ���ʽ����
		if (a){ rtn = a; break; }//��0��Ϊ�棬Ϊ0ʱ��
		else{
			EquPush(1, expr[now_addr - 1].addr, 0, temp);//����ǰ��ַ����һ����ַ��Ԫʽ�Ž����飬����temp��ֵ����һ����ַ
			rtn = temp; break;
		}
	case 28://������
		temp = NewTemp();
		a = K_CMP();
		if (a) { rtn = a; break; }
		else{
			EquPush(1, expr[now_addr - 1].addr, 0, temp);
			rtn = temp; break;
		}
	case 32://������
		rtn = B_Analize();//�ٴε��ò������ʽ��������
		if (expr[now_addr].cod != 40)//ð��
		{
			Error1(B_EXP);//���ʽ����
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

int K_CMP()//�����ϵ���ʽ
{
	int rtn = 0;
	int t1, t2;
	t1 = NewTemp();//temp_count--,temp_count��ȫ�ֱ�������Ϊ��ʱ���������м�����Ľ����ַ�����������г�ʼ��Ϊ0
	t2 = NewTemp();//t2=t1-1
	if ((expr[now_addr].cod > 42) && (expr[now_addr].cod < 49))//< <= != = >  >=
	{
		EquPush(3, expr[now_addr - 1].addr, expr[now_addr + 1].addr, t1);//����ϵ�������ǰ��ı�ʶ���������
		switch (expr[now_addr].cod)
		{
		case 43:EquPush(9, t1, ZERO, LineOfEqu + 4); break;//<  
		case 44:EquPush(8, t1, ZERO, LineOfEqu + 4); break;//<=
		case 41:EquPush(11, t1, ZERO, LineOfEqu + 2); break;//!=
		case 46:EquPush(11, t1, ZERO, LineOfEqu + 4); break;//=
		case 47:EquPush(10, t1, ZERO, LineOfEqu + 4); break;//>
		case 48:EquPush(7, t1, ZERO, LineOfEqu + 4); break;//>=
		}
		EquPush(1, ONE, 0, t2);//1�������'='��ֵ��
		EquPush(6, 0, 0, LineOfEqu + 3);//6�������j��Ӧ������ת
		EquPush(1, ZERO, 0, t2);
		rtn = t2;
		now_addr++;//��ǰ��ַ��1
	}
	return rtn;//������ʱ��ַ
}

/*********************���ڻ����************************/
void BackPatch(int addr, int addr2)
{
	Equ[addr].result = addr2;
	return;
}
int EquPush(int op, int a, int b, int r)/*��Ԫʽ�������*/
{
	int i = LineOfEqu;
	Equ[LineOfEqu].op = op;
	Equ[LineOfEqu].op1 = a;
	Equ[LineOfEqu].op2 = b;
	Equ[LineOfEqu].result = r;
	LineOfEqu++;
	return i;
}











/*              Ŀ�����������غ���*/
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
	InitTarget();//���ó�ʼ������
	control = 0;
	if ((AssemFout = fopen("s.asm", "wt+")) == NULL){
		printf("Cannot open the %s\n", filename);
		exit(1);
	}
	for (m = 0; m < gen_count; m++){
		for (j = 1; j < 12; j++){
			flag = strcmp(operation[j].opp, GenStack[m].op);//�Ƚ���Ԫʽ����Ĳ������������������
			if (!flag)//flag��0
			{
				GenStack[m].code = operation[j].code;//�������������Ÿ�ֵ����Ԫʽ����
				op = operation[j].code;//���丳ֵ��op
				SortDGA(m, op);//���ú���
				break;
			}
		}
	}
	fprintf(AssemFout, "data segment\n");//��Ŀ�����д���ļ�
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
	printf("������£�\n");
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
	printf("~~~~~~~~~Ŀ�����������~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("����Դ�ļ�����");//equ.txt symble.txt
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
	InitOp();//���ó�ʼ������
	bx.number = 1;//��ʼ���Ĵ����Ľṹ
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
	a = GetNextGen();//a�����Ƿ��Ѿ�����Ԫʽ���н�β	
	while (a>0)
	{
		GenStack[i].label = CurrentGen.label;//�����ļ���ȡ����Ԫʽ���и�ֵ���ṹ����
		GenStack[i].addr1 = CurrentGen.addr1;//��һ������
		GenStack[i].addr2 = CurrentGen.addr2;//�ڶ�������
		GenStack[i].result = CurrentGen.result;//�����ַ
		strcpy(GenStack[i].op, CurrentGen.op);//�����
		i++;
		gen_count++;//��Ԫʽ����
		a = GetNextGen();
	}
	i = 0; 
	a = 1;
	while (a > 0)
	{
		fscanf(TokenFin1, "%d%d%s\n", &TokenTable[i].label, &TokenTable[i].cod1, TokenTable[i].name);//��symble���ȡ��ʶ���ȵ���
		if (TokenTable[i].label == 0)break;
		i++;
		token_count++;
	}
	fclose(TokenFin1);
	fclose(GenFin);
}

int GetNextGen()//����Ԫʽ�ļ��л�ȡ��Ԫʽ��ֵ���ṹ����
{
	char a[2], b[2];//�����������
	int d = 1;
	fscanf(GenFin, "%d%s%s%d%d%d%s\n", &CurrentGen.label, a, CurrentGen.op, &CurrentGen.addr1, &CurrentGen.addr2, &CurrentGen.result, b);
	if (CurrentGen.op[0] == '0')//������Ԫʽ���н�β
		d = 0;
	return d;
}

void InitOp()//��ʼ����Ԫʽ���ֵ��������0
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
		fprintf(AssemFout, " mov %s, ax;Ϊ%s��ֵΪ%s\n", GetName(r), GetName(r), GetName(a));
		break;
	case 2:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   add  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   mov  %s,ax;���ӷ�\n", GetName(r));
		break;
	case 3:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   mov  %s,ax;������\n", GetName(r));
		break;
	case 4:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   mov  cx,%s;\n", GetName(b));
		fprintf(AssemFout, "   mul  cx;\n");
		fprintf(AssemFout, "   mov  %s,ax;���˷�\n", GetName(r));
		break;
	case 5:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   mov  cx,%s;\n", GetName(b));
		fprintf(AssemFout, "   div  cx;\n");
		fprintf(AssemFout, "   mov  al,ah;\n");
		fprintf(AssemFout, "   mov  ah,0;\n");
		fprintf(AssemFout, "   mov  %s,ax;������\n", GetName(r));
		break;
	case 6:
		fprintf(AssemFout, "   jmp  lab%d;��ת��lab%d\n", r, r);
		break;
	case 7:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   js  lab%d;��ת�� %d\n", r, r);
		break;
	case 8:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   je  lab%d;��ת�� %d\n", r, r);
		break;
	case 9:
		fprintf(AssemFout, "   mov  ax,%s;\n", a);
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   je  lab%d;��ת�� %d\n", r, r);
		fprintf(AssemFout, "   sub  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   cmp  ax,0;\n");
		fprintf(AssemFout, "   jns  lab%d;��ת�� %d\n", r, r);
		break;
	case 10:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   jb  lab%d;��ת�� %d\n", r, r);
		break;
	case 11:
		fprintf(AssemFout, "   mov  ax,%s;\n", GetName(a));
		fprintf(AssemFout, "   cmp  ax,%s;\n", GetName(b));
		fprintf(AssemFout, "   jne  lab%d;��ת�� %d\n", r, r);
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
	op_cod = op_code;//���ֲ����������
	addr = address;//��Ԫʽ���
	switch (op_cod){
	case 6://j��ֱ����ת
		next_p1 = GenStack[addr].result;//��ǰ��Ԫʽ�Ľ����ַ������Ԫʽ���
		GenStack[next_p1 - 1].out_port = 1;//��ת������Ԫʽ����һ����Ԫʽ�ǻ���������
		break;
	case 7://j<С��ʱ��ת
		next_p1 = GenStack[addr++].result;//��һ����Ԫʽ�Ľ����ַ������ת������Ԫʽ �����
		GenStack[next_p1 - 1].out_port = 1;//
		GenStack[addr - 1].out_port = 1;
		break;
	case 8://j=����ʱ��ת
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 9://j>=���ڵ���ʱ��ת
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 10://j<=С�ڵ���ʱ��ת
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	case 11://j!=������ʱ��ת
		next_p1 = GenStack[addr++].result;
		GenStack[next_p1 - 1].out_port = 1;
		GenStack[addr - 1].out_port = 1;
		break;
	default:break;
	}
	return;
}
