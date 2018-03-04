#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stack>

using namespace std;

string key[12]={"int","main","void","if","else","char","flaot","while","return","break","continue"};
string jiefu[16]={">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")"};
char input[2000];//输入的代码部分
int locationOfInput = 0;//当前扫描点位于输入数组的位置
int locationOfArray = 0;//当前关键字在关键字数字的位置
int locationOfSy=0;//当前符号表的个数
int locationOfToken=0;//语义分析

float consl[100];//常数表
int lenl[100];//长度表
string qt[1000];//定义四元式区
int flag=0;//中间变量的下标
int locOfqt=0;//
int flagOfIf[100]={0};//if出现次数
int locOfIf=0; //当前所在IF层次
int address=0;//活动记录的地址初始值
int locationOfRecord=0;//活动记录的位置
int typeOfbl=0;//变量说明语句的type
int locationOfSynbl=0;//符号表的位置

string integerArr[1000];
string floatArr[1000];
int locOfint=0;
int locOfflo=0;
string constarr[1000];
int locOfconst;

struct RecordNode;//,Pfinfl,Synbl,Typel,Ainfl;
struct Pfinfl;
struct Synbl;
struct Typel;
struct Ainfl;

//stack<RecordNode>recordForm;//定义活动记录表
stack<string>sem;//定义语义栈
stack<int>qtstack;

void input1();
int jugle(char *a);
void analyse();
void output();
void fun_program();
int fun_type2();
int fun_type1();
int fun_xingcan();
int fun_xingcan1();
int fun_xingcanTable();
int fun_chengxuti();
int fun_returnyuju();
int fun_fanhuizhi();
int fun_ljbds();
int fun_ifyuju();
int fun_ifti();
int fun_elseti();
int fun_whileyuju();
int fun_fuzhiyuju();
int fun_fuzhibds();
int fun_ssbds();
int fun_xiang();
int fun_yinzi();
int fun_smysf();
int fun_blsm();
int fun_blbsfb();
int fun_blbsfb1();
int fun_blf();
void output_qt();

int fun_loopTheSyn(string a);//遍历符号表
void show();
void show1();
void show2();
char fun_findTheType(string a);
int loopTheConst(string a);

struct{
    char vacabulary[20];
    int value;
}token[1000];//定义要输出的组织结构

struct RecordNode{
    string name="";
    int address;
}record[500];//定义活动记录单元

struct Pfinfl{//函数表的定义
    int level;//层次号
    int off;//区距
    int fn;//参数个数
    int entry;//入口地址
    Synbl *param;//参数表，指向形参表
};

struct Synbl{//定义符号表

    string name="";
    char typ;//指向类型表
    string cat;//种类
    int addr1=0;
    //Pfinfl *addr2;
}synbl[100],synbl_1[500];

struct Typel{//类型表
    char tval;
    Ainfl *tpoint;
};

struct Ainfl{//数组表
    int low;
    int up;
    Typel *ctp;
    int clen;//长度
};


int loopTheToken(string a){
    int l=0;
    for(int i=0;i<locationOfToken;i++){
        if(token[i].vacabulary==a){
            l=token[i].value;
        }
    }
    return l;
}

int loopTheConst(string a){
    int l=0;
    for(int i=0;i<locOfconst;i++){
        if(constarr[i]==a){
            l=1;
        }
    }
    return l;
}


void input1(){//读取测试代码
    FILE *fp;
    fp = fopen("1.txt","r");
    char c;
    while(1){
        c=fgetc(fp);
        if(c=='#')
            break;
        input[locationOfInput++] = c;
    }
    fclose(fp);
}

int jugle(char *a){//判断关键字
    if(!strcmp(a,"int"))
        return 4;
    else if(!strcmp(a,"main"))
        return 5;
    else if(!strcmp(a,"void"))
        return 6;
    else if(!strcmp(a,"if"))
        return 7;
    else if(!strcmp(a,"else"))
        return 8;
    else if(!strcmp(a,"char"))
        return 9;
    else if(!strcmp(a,"float"))
        return 27;
    else if(!strcmp(a,"string"))
        return 29;
    else if(!strcmp(a,"while"))
        return 30;
    else if(!strcmp(a,"return"))
        return 31;
    else if(!strcmp(a,"break"))
        return 32;
    else if(!strcmp(a,"continue"))
        return 33;
    else
        return 0;
}

void analyse(){//词法分析
    int loc = 0;
    while(loc<locationOfInput){
        if(input[loc]==' '||input[loc]=='\t'||input[loc]=='\n')
            loc++;
        else if((input[loc]>='a'&&input[loc]<='z')||(input[loc]>='A'&&input[loc]<='Z')){
            char test[10];
            int i=0;
            if((input[loc]>='a'&&input[loc]<='z')||(input[loc]>='A'&&input[loc]<='Z')||(input[loc]>='0'&&input[loc]<='9')){
                while((input[loc]>='a'&&input[loc]<='z')||(input[loc]>='A'&&input[loc]<='Z')||(input[loc]>='0'&&input[loc]<='9')){
                    test[i++]=input[loc++];
                }
                test[i]='\0';
                int test1 = jugle(test);
                for(int j=0;j<i;j++)
                    token[locationOfArray].vacabulary[j]=test[j];
                token[locationOfArray].value=test1;
                locationOfArray++;
            }
        }
        else if((input[loc]>='0')&&(input[loc]<='9')||(input[loc]=='.')){
            int test[20];
            int i=0;
            int flag=0;//记录小数点个数
            while(input[loc]>='0'&&input[loc]<='9'||input[loc]=='.'){
                if(input[loc]=='.'){
                    flag++;
                }
                test[i++]=input[loc++];
            }
            if(flag>1){
                cout<<"请检查输入的浮点数，您的浮点数出现两个以上小数点！";
                system("pause");
            }
            test[i]='\0';
            for(int j=0;j<i;j++)
                token[locationOfArray].vacabulary[j]=test[j];
           // token[locationOfArray].value=3;
            if(flag==0)
                token[locationOfArray].value=3;
            else if(flag==1)
                token[locationOfArray].value=26;
            locationOfArray++;
        }
        else if(input[loc]=='\''){//单引号17，双引号18
            token[locationOfArray].vacabulary[0]=input[loc];
            loc++;
            token[locationOfArray].vacabulary[1]=input[loc++];
            if(input[loc]=='\''){
                token[locationOfArray].vacabulary[2]=input[loc++];
                token[locationOfArray].value=1;
                locationOfArray++;
            }
            else{
                cout<<"error_字符部分\n";
                system("pause");
            }
        }
        else if(input[loc]=='\"'){//单引号17，双引号18
            token[locationOfArray].vacabulary[0]=input[loc];
            loc++;
            int i=1;
            while(input[loc]!='\"'){
                token[locationOfArray].vacabulary[i++]=input[loc++];
            }
            token[locationOfArray].value=2;
            token[locationOfArray].vacabulary[i]=input[loc];
            //token[locationOfArray].value=18;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='<'){
            if(input[loc+1]=='='){
                token[locationOfArray].vacabulary[0]='<';
                token[locationOfArray].vacabulary[1]='=';
                token[locationOfArray].value=11;
                locationOfArray++;
                loc=loc+2;
            }
            else{
                token[locationOfArray].vacabulary[0]=input[loc];
                token[locationOfArray].value=15;
                locationOfArray++;
                loc++;
            }
        }
        else if(input[loc]=='>'){
            if(input[loc+1]=='='){
                token[locationOfArray].vacabulary[0]='>';
                token[locationOfArray].vacabulary[1]='=';
                token[locationOfArray].value=10;
                locationOfArray++;
                loc=loc+2;
            }
            else{
                token[locationOfArray].vacabulary[0]=input[loc];
                token[locationOfArray].value=14;
                locationOfArray++;
                loc++;
            }
        }
        else if(input[loc]=='='){
            if(input[loc+1]=='='){
                token[locationOfArray].vacabulary[0]='=';
                token[locationOfArray].vacabulary[1]='=';
                token[locationOfArray].value=12;
                locationOfArray++;
                loc=loc+2;
            }
            else{
                token[locationOfArray].vacabulary[0]=input[loc];
                token[locationOfArray].value=13;
                locationOfArray++;
                loc++;
            }
        }
        else if(input[loc]=='+'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=16;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='-'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=17;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='*'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=18;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='/'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=19;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='{'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=20;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='}'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=21;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]==','){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=22;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]==';'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=23;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='('){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=24;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]==')'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=25;
            locationOfArray++;
            loc++;
        }
        else if(input[loc]=='&'&&input[loc+1]=='&'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].vacabulary[1]=input[loc];
            token[locationOfArray].value=34;
            locationOfArray++;
            loc=loc+2;
        }
        else if(input[loc]=='|'&&input[loc+1]=='|'){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].vacabulary[1]=input[loc];
            token[locationOfArray].value=35;
            locationOfArray++;
            loc=loc+2;
        }
        else if(input[loc]=='!'&&input[loc+1]!='='){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].value=36;
            locationOfArray++;
            loc=loc+1;
        }
        else if(input[loc]=='!'&&input[loc+1]=='='){
            token[locationOfArray].vacabulary[0]=input[loc];
            token[locationOfArray].vacabulary[1]=input[loc+1];
            token[locationOfArray].value=37;
            locationOfArray++;
            loc=loc+2;
        }
    }
}

void output(){//输出Token序列
    int i=0;
    cout<<"Token序列如下：\n"<<endl;
    for(int j=0;j<locationOfArray;j++)
    {
        cout<<"<";
        cout<<token[j].vacabulary;
        cout<<","<<token[j].value<<">";
        i++;
        if((i%5)!=0)
            cout<<"\t\t";
        else
            cout<<endl;
    }
    token[locationOfArray].value=1001;
    int a=0;
    int c=0;
    int d=0;
    cout<<"\n\n关键字表\n"<<endl;
    for(int b=0;b<12;b++)
    {
        cout<<b+1<<":"<<key[b]<<"   ";
    }
    cout<<"\n\n界符表\n"<<endl;
    for(int b=0;b<16;b++)
    {
          cout<<b+1<<":"<<jiefu[b]<<"   ";
    }
    cout<<"\n\n常数表\n"<<endl;
    for(int b=0;b<locationOfArray;b++){

         if(token[b].value==3||token[b].value==26){
            if(token[b].value==3){
                integerArr[locOfint++]=token[b].vacabulary;
            }
            else{
                floatArr[locOfflo++]=token[b].vacabulary;
            }
            string a=token[b].vacabulary;
            if(!loopTheConst(a)){
                constarr[locOfconst++]=a;
            }
            cout<<token[b].vacabulary<<"   ";
            d++;
        }
    }
    cout<<endl;
    int e=0;
    for(int b=0;b<locationOfArray;b++){
        if(token[b].value==0||!(strcmp(token[b].vacabulary,"main"))){
            int flag=0;
            for(int j=0;j<locationOfSy;j++){
                string c=token[b].vacabulary;
                if(synbl[j].name==c)
                   flag++;
            }
            if(flag==0){
                synbl[locationOfSy].name=token[b].vacabulary;
                locationOfSy++;
            }
        }

    }
    cout<<"\n符号表：\n\n"<<"名字\t"<<"类型\t"<<"种类\t"<<"地址\t"<<endl;
    for(int j=0;j<locationOfSy;j++){
        cout<<synbl[j].name<<endl;
    }

}

void fun_program(){//程序体
    if(fun_type2()){
        if(token[locationOfToken].value==0||!(strcmp(token[locationOfToken].vacabulary,"main"))){

            //修改
            synbl_1[locationOfSynbl].name=token[locationOfToken].vacabulary;
            synbl_1[locationOfSynbl].cat="f";
            synbl_1[locationOfSynbl].typ='/';
            locationOfSynbl++;

            locationOfToken++;
            if(token[locationOfToken].value==24){
                locationOfToken++;
                if(fun_xingcanTable()){
                    if(token[locationOfToken].value==25&&token[locationOfToken+1].value==20){
                        locationOfToken=locationOfToken+2;
                        if(fun_chengxuti()){
                            if(token[locationOfToken].value==21){
                                locationOfToken++;
                                if(token[locationOfToken].value==1001){
                                    output_qt();
                                    synbl_1[0].addr1=address;
                                    cout<<"编译通过！"<<endl;
                                    system("pause");
                                    system("cls");
                                    show();
                                    system("pause");
                                    system("cls");
                                    show1();
                                    exit(0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout<<"编译出错！"<<endl;
    system("pause");
}

int fun_type2(){//函数返回类型
    if(!(strcmp(token[locationOfToken].vacabulary,"int"))||!(strcmp(token[locationOfToken].vacabulary,"char"))||!(strcmp(token[locationOfToken].vacabulary,"float"))||!(strcmp(token[locationOfToken].vacabulary,"void"))||!(strcmp(token[locationOfToken].vacabulary,"string"))){
        locationOfToken++;
        return 1;
    }
    else{
        locationOfToken++;
     //   cout<<"err--函数返回值类型错误";
      //  system("exit");
        return 0;
    }
}

int fun_type1(){//数据类型
    if(!(strcmp(token[locationOfToken].vacabulary,"int"))||!(strcmp(token[locationOfToken].vacabulary,"char"))||!(strcmp(token[locationOfToken].vacabulary,"float"))||!(strcmp(token[locationOfToken].vacabulary,"string"))){
        locationOfToken++;
        return 1;
    }
    else if(token[locationOfToken].value==25){
      //  locationOfToken++;
      //  cout<<"没有形参传入"<<endl;
        return 0;
    }
    else{
        return 0;
    }
}

int fun_xingcan(){//形参
    if(fun_xingcan1()){
        if(fun_type1()){
            if(token[locationOfToken].value==0){
                locationOfToken++;
                return 1;
            }
            else{
                cout<<"err_标识符错误"<<endl;
                system("pasuse");
            }
        }
        else{
            cout<<"err_形参类型错误"<<endl;
            system("pause");
        }
    }
    else if(token[locationOfToken].value==25){
        return 1;
    }
    else{
        cout<<"err_形参错误"<<endl;
        return 0;
    }
}

int fun_xingcan1(){


    if(token[locationOfToken+1].value==0){
        int type=token[locationOfToken].value;
        record[locationOfRecord].address=address;
        record[locationOfRecord].name=token[locationOfToken+1].vacabulary;
        if(token[locationOfToken].value==4){
            address=address+2;
            synbl_1[locationOfSynbl].typ='i';
        //oldArress=address;
        }
        else if(token[locationOfToken].value==9){
            address++;
            synbl_1[locationOfSynbl].typ='c';
        }
        else if(token[locationOfToken].value==27){
            address=address+4;
            synbl_1[locationOfSynbl].typ='r';
        }
          //  locationOfRecord++;
        synbl_1[locationOfSynbl].name=token[locationOfToken+1].vacabulary;
        synbl_1[locationOfSynbl].cat="vf";
        synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
        locationOfSynbl++;
        locationOfRecord++;
    }



    if(fun_type1()){
        if(token[locationOfToken].value==0){
            locationOfToken++;
            if(token[locationOfToken].value==22){
                locationOfToken++;
                fun_xingcan1();
                return 1;
            }
            else{
                locationOfToken=locationOfToken-2;
                return 1;
            }
        }
        else{
            cout<<"err_标识符错误"<<endl;
            system("pause");
        }
    }
    else if(token[locationOfToken].value==25){
        return 0;
    }
    return 0;
}

int fun_xingcanTable(){//形参表
    if(fun_xingcan()){
        return 1;
    }
  /*  else if(token[locationOfToken].value==25){
        return 1;
    }*/
    else{
        cout<<"err_参数表错误"<<endl;
        return 0;
    }
}

int fun_chengxuti(){//程序体
    if(fun_blsm()){

        if(fun_chengxuti()){
            return 1;
        }
        else{
           //cout<<"err_blsm"<<endl;
            return 0;
        }
    }
    else if(fun_fuzhiyuju()){
        //cout<<token[locationOfToken].vacabulary<<endl;
        if(fun_chengxuti()){
            return 1;
        }
        else{
            //cout<<"err_fuyj"<<endl;
            return 0;
        }
    }
    else if(fun_ifyuju()){
       /* cout<<token[locationOfToken-3].vacabulary<<endl;
        cout<<token[locationOfToken].vacabulary<<endl;
        cout<<"-----"<<endl;
        system("pause");*/
        if(fun_chengxuti()){

            return 1;
        }
        else{
            //cout<<"err_ifyuju"<<endl;
            return 0;
        }
    }
    else if(fun_whileyuju()){
        //cout<<"ggg"<<endl;
        if(fun_chengxuti()){
            return 1;
        }
        else{
           //cout<<"err_whileyuju"<<endl;
            return 0;
        }
    }
    else if(fun_returnyuju()){
            return 1;
    }
    else if(token[locationOfToken].value==21&&token[locationOfToken-1].value==20){//程序体为空
        return 1;
    }
    else if(token[locationOfToken].value==21&&token[locationOfToken-1].value==23){
        return 1;
    }
     else if(token[locationOfToken].value==21&&token[locationOfToken-1].value==21){
        return 1;
    }
    return 0;
}

int fun_returnyuju(){//return语句
    if(token[locationOfToken].value==31){
        locationOfToken++;
        if(fun_fanhuizhi()){
            if(token[locationOfToken].value==23){
                locationOfToken++;
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    return 0;
}

int fun_fanhuizhi(){//返回值

    int oldToken=locationOfToken;
    if(fun_ljbds()){
        string a=sem.top();
        sem.pop();
        string s="(";
        s+="ret";
        s+=",";
        s+=a;
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=")";
        qt[locOfqt++]=s;
        return 1;
    }
    else {
        locationOfToken=oldToken;
        if(fun_ssbds()){
            string a=sem.top();
            sem.pop();
            string s="(";
            s+="ret";
            s+=",";
            s+=a;
            s+=",";
            s+="_";
            s+=",";
            s+="_";
            s+=")";
            qt[locOfqt++]=s;
            return 1;
        }
        else if(token[locationOfToken].value==0||token[locationOfToken].value==1||token[locationOfToken].value==2||token[locationOfToken].value==3||token[locationOfToken].value==26){

            if(token[locationOfToken].value==0){
                string a=token[locationOfToken].vacabulary;
                if(!fun_loopTheSyn(a)){
                    cout<<"出现未定义的变量"<<a<<"!!"<<endl;
                    exit(0);
                }
            }

            locationOfToken++;
            string s="(";
            s+="ret";
            s+=",";
            s+=token[locationOfToken-1].vacabulary;
            s+=",";
            s+="_";
            s+=",";
            s+="_";
            s+=")";
            qt[locOfqt++]=s;
            return 1;
        }
        else{
            return 0;
        }
    }
}

int fun_ljbds(){//逻辑表达式
    int oldToken;
    oldToken=locationOfToken-2;
    if(flagOfIf[locOfIf]>=1){
        if(token[oldToken].value==7){
        int z;
        char sz[20];
        string new_qt;
        z=qtstack.top();
        qtstack.pop();
        new_qt=qt[z];
        itoa(locOfqt,sz,10);
        new_qt+=sz;
        new_qt+=")";
        qt[z]=new_qt;
        }
    }
    if((token[locationOfToken].value==0||token[locationOfToken].value==3||token[locationOfToken].value==26)&&token[locationOfToken+1].value==25){

        if(token[locationOfToken].value==0){
            string a=token[locationOfToken].vacabulary;
            if(!fun_loopTheSyn(a)){
                cout<<"表达式中出现未定义的字符"<<a<<"!!"<<endl;
                exit(0);
            }
        }
        locationOfToken++;
        return 1;
    }
    else if(token[locationOfToken].value==36){//判断非
        locationOfToken++;
        if(token[locationOfToken].value==0||token[locationOfToken].value==3||token[locationOfToken].value==26){

            if(token[locationOfToken].value==0){
                string a=token[locationOfToken].vacabulary;
                if(!fun_loopTheSyn(a)){
                    cout<<"表达式中出现未定义的字符"<<a<<"!!"<<endl;
                    exit(0);
                }
            }

            string sys;
            char shuzu[20];
            itoa(flag,shuzu,10);
            sys=shuzu;
            string t="t";
            t=t+sys;
            sem.push(t);
            flag++;
            string s="(";
            s+="！";
            s+=",";
            s+=token[locationOfToken].vacabulary;
            s+=",";
            s+="_";
            s+=",";
            s+=t;
            s+=")";
            qt[locOfqt++]=s;


            locationOfToken++;

            record[locationOfRecord].name=t;
            record[locationOfRecord].address=address;
            address=address+2;
            synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
            synbl_1[locationOfSynbl].name=t;
            synbl_1[locationOfSynbl].typ='i';
            synbl_1[locationOfSynbl].cat="v";
           // cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
            locationOfSynbl++;
            locationOfRecord++;


            return 1;
        }
        else if(token[locationOfToken].value==24){
                locationOfToken++;
                if(fun_ljbds()){
                    if(token[locationOfToken].value==25){
                        locationOfToken++;
                        return 1;
                    }
                }
            }
        else
            return 0;
    }
    else if(token[locationOfToken].value==0||token[locationOfToken].value==3||token[locationOfToken].value==26){

        if(token[locationOfToken].value==0){
            string a=token[locationOfToken].vacabulary;
            if(!fun_loopTheSyn(a)){
                cout<<"表达式中出现未定义的字符"<<a<<"!!"<<endl;
                exit(0);
            }
        }

        if(fun_ssbds()){
            if(fun_smysf()){
                int oldToken=locationOfToken-1;
                if(fun_ssbds()){
                    string a=sem.top();
                    sem.pop();
                    string b=sem.top();
                    sem.pop();
                    string sys;
                    char shuzu[20];
                    itoa(flag,shuzu,10);
                    sys=shuzu;
                    string t="t";
                    t=t+sys;
                    sem.push(t);
                    flag++;
                    string s="(";
                    s+=token[oldToken].vacabulary;
                    s+=",";
                    s+=b;
                    s+=",";
                    s+=a;
                    s+=",";
                    s+=t;
                    s+=")";
                    qt[locOfqt++]=s;



                    record[locationOfRecord].name=t;
                    record[locationOfRecord].address=address;
                    address=address+2;
                    synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                    synbl_1[locationOfSynbl].name=t;
                    synbl_1[locationOfSynbl].typ='i';
                    synbl_1[locationOfSynbl].cat="v";
                   // cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                    locationOfSynbl++;
                    locationOfRecord++;



                    return 1;
                }
            }
            else{
                return 1;
            }
        }
    }
    else if(token[locationOfToken].value==24){
        locationOfToken++;
        if(fun_ljbds()){
            if(token[locationOfToken].value==25){
                locationOfToken++;
                if(fun_smysf()){
                    int oldToken=locationOfToken-1;
                    if(token[locationOfToken].value==24){
                        locationOfToken++;
                        if(fun_ljbds()){
                            if(token[locationOfToken].value==25){
                                string a=sem.top();
                                sem.pop();
                                string b=sem.top();
                                sem.pop();
                                string sys;
                                char shuzu[20];
                                itoa(flag,shuzu,10);
                                sys=shuzu;
                                string t="t";
                                t=t+sys;
                                sem.push(t);
                                flag++;
                                string s="(";
                                s+=token[oldToken].vacabulary;
                                s+=",";
                                s+=b;
                                s+=",";
                                s+=a;
                                s+=",";
                                s+=t;
                                s+=")";
                                qt[locOfqt++]=s;


                                record[locationOfRecord].name=t;
                                record[locationOfRecord].address=address;
                                address=address+2;
                                synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                                synbl_1[locationOfSynbl].name=t;
                                synbl_1[locationOfSynbl].typ='i';
                                synbl_1[locationOfSynbl].cat="v";
                       //         cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                                locationOfSynbl++;
                                locationOfRecord++;




                                locationOfToken++;
                                return 1;
                            }
                        }
                    }
                }
                else {
                    return 0;
                }
            }
        }
    }
   else{
        return 0;
    }
    return 0;
}

int fun_ifyuju(){//if语句
    if(token[locationOfToken].value==7&&token[locationOfToken+1].value==24){
        if(token[locationOfToken-1].value==8){
            flagOfIf[locOfIf]++;
        }
        else{
            locOfIf++;
            flagOfIf[locOfIf]=0;
        }
        int beforeif;
        beforeif=locationOfToken-1;
        locationOfToken=locationOfToken+2;
        if(fun_ljbds()){
            if(token[locationOfToken].value==25){
                string a;
                if(token[locationOfToken-2].value==24){
                    a=token[locationOfToken-1].vacabulary;
                }
                else{
                    a=sem.top();
                    sem.pop();}
                    if(token[beforeif].value==8){
                        string s="(";
                        s+="eli";
                        s+=",";
                        s+=a;
                        s+=",";
                        s+="_";
                        s+=",";
                        qtstack.push(locOfqt);
                        qt[locOfqt++]=s;

                    }
                    else
                    {
                        string s="(";
                        s+="if";
                        s+=",";
                        s+=a;
                        s+=",";
                        s+="_";
                        s+=",";
                        qtstack.push(locOfqt);
                        qt[locOfqt++]=s;
                    }
                locationOfToken++;
                if(fun_ifti()){
                    if(fun_elseti()){
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

int fun_ifti(){//if体
    if(token[locationOfToken].value==23){
        locationOfToken++;
        return 1;
    }

    else if(token[locationOfToken].value==20){
        locationOfToken++;
        if(fun_chengxuti()){
            if(token[locationOfToken].value==21){
                locationOfToken++;
                return 1;
            }
        }
        else{
                return 0;
          //  cout<<"err_程序体"<<endl;
        }
    }
    else if(fun_blsm()){
        return 1;
    }
    else if(fun_fuzhiyuju()){
        return 1;
    }
    else if(token[locationOfToken].value==23){
        locationOfToken++;
        return 1;
    }
    //cout<<"err_if体"<<endl;
    return 0;
}

int fun_elseti(){//else体
    if(token[locationOfToken].value==8&&token[locationOfToken+1].value==20){
        string s="(";
        s+="el";
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=",";
        int z;
        char sz[20];
        string new_qt;
        z=qtstack.top();
        qtstack.pop();
        new_qt=qt[z];
        itoa(locOfqt+1,sz,10);
        new_qt+=sz;
        new_qt+=")";
        qt[z]=new_qt;
        qtstack.push(locOfqt);
        qt[locOfqt++]=s;
        locationOfToken++;
        if(fun_ifti()){
               if(token[locationOfToken-1].value==21){
                    flagOfIf[locOfIf]=0;
                    if(locOfIf>=2&&(flagOfIf[locOfIf-1]!=0)){
                        locOfIf--;
                    }
                    string s="(";
                    s+="ie";
                    s+=",";
                    s+="_";
                    s+=",";
                    s+="_";
                    s+=",";
                    s+="_";
                    s+=")";
                    int z;
                    char sz[20];
                    string new_qt;
                    z=qtstack.top();
                    qtstack.pop();
                    new_qt=qt[z];
                    itoa(locOfqt,sz,10);
                    new_qt+=sz;
                    new_qt+=")";
                    qt[z]=new_qt;
                    qt[locOfqt++]=s;
                    return 1;
                }
            return 0;
        }
    }
    else if(token[locationOfToken].value==8&&token[locationOfToken+1].value==7){
        locationOfToken++;
        if(fun_ifyuju()){
           return 1;
        }
    }
    else
    {
        flagOfIf[locOfIf]=0;
        if(locOfIf>=2&&(flagOfIf[locOfIf-1]!=0)){
            locOfIf--;
        }
        string s="(";
        s+="ie";
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=")";
        int z;
        char sz[20];
        string new_qt;
        z=qtstack.top();
        qtstack.pop();
        new_qt=qt[z];
        itoa(locOfqt,sz,10);
        new_qt+=sz;
        new_qt+=")";
        qt[z]=new_qt;
        qt[locOfqt++]=s;
        return 1;
    }
}

int fun_whileyuju(){//while语句
    if((token[locationOfToken].value==30)&&(token[locationOfToken+1].value==24)){
        string s="(";
        s+="wh";
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=",";
        s+="_";
        s+=")";
        qt[locOfqt++]=s;
        locationOfToken=locationOfToken+2;
        if(fun_ljbds()){
            if(token[locationOfToken].value==25){
                locationOfToken++;
                string a;
                if(token[locationOfToken-3].value==24){
                    a=token[locationOfToken-2].vacabulary;
                }
                else{
                    a=sem.top();
                    sem.pop();
                }
                string s="(";
                s+="do";
                s+=",";
                s+=a;
                s+=",";
                s+="_";
                s+=",";
                qtstack.push(locOfqt);
                qt[locOfqt++]=s;
                if(token[locationOfToken].value==20){
                    locationOfToken++;
                    if(fun_chengxuti()){
                        if(token[locationOfToken].value==21){
                            string s="(";
                            s+="we";
                            s+=",";
                            s+="_";
                            s+=",";
                            s+="_";
                            s+=",";
                            s+="_";
                            s+=")";
                            int z;
                            char sz[20];
                            string new_qt;
                            z=qtstack.top();
                            qtstack.pop();
                            new_qt=qt[z];
                            itoa(locOfqt+1,sz,10);
                            new_qt+=sz;
                            new_qt+=")";
                            qt[z]=new_qt;
                            qt[locOfqt++]=s;
                            locationOfToken++;
                            return 1;
                        }
                        else{
                            return 0;
                        }
                    }
                    else{
                       // cout<<"err_if体"<<endl;
                        return 0;
                    }
                }
            }
            else{
             //   cout<<"err_右括号匹配"<<endl;
                return 0;
            }
        }
        else if(fun_ssbds()){
            if(token[locationOfToken].value==25){
                locationOfToken++;
                if(token[locationOfToken].value==20){
                    locationOfToken++;
                    if(fun_chengxuti()){
                        if(token[locationOfToken].value==21){
                            locationOfToken++;
                            return 1;
                        }
                        else{
                            return 0;
                        }
                    }
                }
            }
            else{
             //   cout<<"err_右括号匹配"<<endl;
                return 0;
            }
        }
       // else if(token[locationOfToken])
        else{
         //   cout<<"err_逻辑表达式"<<endl;
            return 0;
        }
    }
    else{
      //  cout<<"err_while和左括号匹配"<<endl;
        return 0;
    }
}

int fun_fuzhiyuju(){//赋值语句
    if(fun_fuzhibds()){
        if(token[locationOfToken].value==23){
            locationOfToken++;
            return 1;
        }
    }
    return 0;
}

int fun_fuzhibds(){//赋值表达式
    if(token[locationOfToken].value==0&&token[locationOfToken+1].value==13){


        string a=token[locationOfToken].vacabulary;
        if(!fun_loopTheSyn(a)){
            cout<<"赋值表达式中出现未定义的标识符"<<a<<"!!"<<endl;
            exit(0);
        }



        int oldToken=locationOfToken;
        locationOfToken+=2;
        int newtoken=locationOfToken;
        if(fun_ljbds()){
            string a=sem.top();
            sem.pop();
            string s="(";
            s+="=";
            s+=",";
            s+=a;
            s+=",";
            s+="_";
            s+=",";
            s+=token[oldToken].vacabulary;
            s+=")";
            qt[locOfqt++]=s;
            return 1;
        }
        else if(token[newtoken].value==1){//单个字符赋值
            sem.push(token[newtoken].vacabulary);
            string a=sem.top();
            sem.pop();
            string s="(";
            s+="=";
            s+=",";
            s+=a;
            s+=",";
            s+="_";
            s+=",";
            s+=token[newtoken-2].vacabulary;
            s+=")";
            qt[locOfqt++]=s;
            locationOfToken=newtoken+1;
            return 1;
        }
        else if(token[newtoken].value==2){//单个字符串赋值
            sem.push(token[newtoken].vacabulary);
            string a=sem.top();
            sem.pop();
            string s="(";
            s+="=";
            s+=",";
            s+=a;
            s+=",";
            s+="_";
            s+=",";
            s+=token[newtoken-2].vacabulary;
            s+=")";
            qt[locOfqt++]=s;
            locationOfToken=newtoken+1;
            return 1;
        }
        else{
            locationOfToken=newtoken;
            if(fun_ssbds()){
                string a=sem.top();
                sem.pop();
                string s="(";
                s+="=";
                s+=",";
                s+=a;
                s+=",";
                s+="_";
                s+=",";
                s+=token[oldToken].vacabulary;
                s+=")";
                qt[locOfqt++]=s;
                return 1;
            }
             else{
         //   cout<<"err_赋值语句中的表达式"<<endl;
            return 0;
            }
        }

    }
    return 0;
}

char fun_findTheType(string a){
    for(int i=0;i<500;i++){
        if(synbl_1[i].name==a){
            return synbl_1[i].typ;
        }
    }
  /*  char shuzu[100]=a;
    float num=atof(shuzu);*/

}

int fun_ssbds(){//算术表达式
    if(fun_xiang()){
        if(token[locationOfToken].value==16){
            locationOfToken++;
            if(fun_ssbds()){
                string a=sem.top();
                sem.pop();
                string b=sem.top();
                sem.pop();
                string sys;
                char shuzu[20];
                itoa(flag,shuzu,10);
                sys=shuzu;
                string t="t";
               // strcat(t,sys);
                t=t+sys;
                sem.push(t);
                flag++;
                string s="(";
                s+="+";
                s+=",";
                s+=b;
                s+=",";
                s+=a;
                s+=",";
                s+=t;
                s+=")";
                qt[locOfqt++]=s;


                record[locationOfRecord].name=t;
                record[locationOfRecord].address=address;
                //address=address+4;
                char te1=fun_findTheType(a);
                char te2=fun_findTheType(b);
                int te3=loopTheToken(a);
                int te4=loopTheToken(b);
               // cout<<"1--"<<te1<<"2--"<<te2<<endl;
                if(te1=='r'||te2=='r'||te3==26||te4==26){
                    address=address+4;
                    synbl_1[locationOfSynbl].typ='r';
                    //cout<<a<<b<<te1<<te2<<te3<<te4<<endl;
                }
                else{
                    address=address+2;
                    synbl_1[locationOfSynbl].typ='i';
                }
               /* if(te1=='i'&&te2=='i'){
                    address=address+2;
                }
                else{
                    address=address+4;
                }*/
               // address=address+4;
                synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                synbl_1[locationOfSynbl].name=t;
              //  synbl_1[locationOfSynbl].typ='r';
                synbl_1[locationOfSynbl].cat="v";
            //    cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                locationOfSynbl++;
                locationOfRecord++;




                return 1;
            }
        }
        else if(token[locationOfToken].value==17){
            locationOfToken++;
            if(fun_ssbds()){
                string a=sem.top();
                sem.pop();
                string b=sem.top();
                sem.pop();
                string sys;
                char shuzu[20];
                itoa(flag,shuzu,10);
                sys=shuzu;
                string t="t";
               // strcat(t,sys);
                t=t+sys;
                sem.push(t);
                flag++;
                string s="(";
                s+="-";
                s+=",";
                s+=b;
                s+=",";
                s+=a;
                s+=",";
                s+=t;
                s+=")";
                qt[locOfqt++]=s;


                record[locationOfRecord].name=t;
                record[locationOfRecord].address=address;



                char te1=fun_findTheType(a);
                char te2=fun_findTheType(b);
                int te3=loopTheToken(a);
                int te4=loopTheToken(b);
              //  cout<<"1--"<<te1<<"2--"<<te2<<endl;
                if(te1=='r'||te2=='r'||te3==26||te4==26){
                    address=address+4;
                    synbl_1[locationOfSynbl].typ='r';
                }
                else{
                    address=address+2;
                    synbl_1[locationOfSynbl].typ='i';
                }
               /* if(te1=='i'&&te2=='i'){
                    address=address+2;
                }
                else{
                    address=address+4;
                }*/
            //    address=address+4;
                synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                synbl_1[locationOfSynbl].name=t;
             //   synbl_1[locationOfSynbl].typ='r';
                synbl_1[locationOfSynbl].cat="v";
             //   cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                locationOfSynbl++;
                locationOfRecord++;




                return 1;
            }
        }
        else{
            return 1;
        }
    }
   // cout<<"err_项"<<endl;
    return 0;
}

int fun_xiang(){//项
    if(fun_yinzi()){
        if(token[locationOfToken].value==18){
            locationOfToken++;
            if(fun_xiang()){
                string a=sem.top();
                sem.pop();
                string b=sem.top();
                sem.pop();
                string sys;
                char shuzu[20];
                itoa(flag,shuzu,10);
                sys=shuzu;
                string t="t";
               // strcat(t,sys);
                t=t+sys;
                sem.push(t);
                flag++;
                string s="(";
                s+="*";
                s+=",";
                s+=b;
                s+=",";
                s+=a;
                s+=",";
                s+=t;
                s+=")";
                qt[locOfqt++]=s;


                record[locationOfRecord].name=t;
                record[locationOfRecord].address=address;



                char te1=fun_findTheType(a);
                char te2=fun_findTheType(b);
                int te3=loopTheToken(a);
                int te4=loopTheToken(b);
             //   cout<<"aaa"<<"1--"<<te1<<"2--"<<te2<<a<<b<<endl;
            //    cout<<te3<<te4<<"end"<<endl;
                if(te1=='r'||te2=='r'||te3==26||te4==26){
                    address=address+4;
                    synbl_1[locationOfSynbl].typ='r';
                }
                else{
                    address=address+2;
                    synbl_1[locationOfSynbl].typ='i';
                }


                /*if(te1=='i'&&te2=='i'){
                    address=address+2;
                }
                else{
                    address=address+4;
                }*/
                //address=address+4;
                synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                synbl_1[locationOfSynbl].name=t;
             //   synbl_1[locationOfSynbl].typ='r';
                synbl_1[locationOfSynbl].cat="v";
              //  cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                locationOfSynbl++;
                locationOfRecord++;



                return 1;
            }
        }
        else if(token[locationOfToken].value==19){
            locationOfToken++;
            if(fun_xiang()){
                string a=sem.top();
                sem.pop();
                string b=sem.top();
                sem.pop();
                string sys;
                char shuzu[20];
                itoa(flag,shuzu,10);
                sys=shuzu;
                string t="t";
               // strcat(t,sys);
                t=t+sys;
                sem.push(t);
                flag++;
                string s="(";
                s+="/";
                s+=",";
                s+=b;
                s+=",";
                s+=a;
                s+=",";
                s+=t;
                s+=")";
                qt[locOfqt++]=s;


                record[locationOfRecord].name=t;
                record[locationOfRecord].address=address;


                char te1=fun_findTheType(a);
                char te2=fun_findTheType(b);
                int te3=loopTheToken(a);
                int te4=loopTheToken(b);
                //cout<<"1--"<<te1<<"2--"<<te2<<endl;
                if(te1=='r'||te2=='r'||te3==26||te4==26){
                    address=address+4;
                    synbl_1[locationOfSynbl].typ='r';
                }
                else{
                    address=address+2;
                    synbl_1[locationOfSynbl].typ='i';
                }


               // address=address+4;
                synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
                synbl_1[locationOfSynbl].name=t;
             //   synbl_1[locationOfSynbl].typ='r';
                synbl_1[locationOfSynbl].cat="v";
               // cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
                locationOfSynbl++;
                locationOfRecord++;



                return 1;
            }
        }
        else{
            return 1;
        }
    }
    return 0;
}

int fun_yinzi(){//因子
    if(token[locationOfToken].value==0||token[locationOfToken].value==3||token[locationOfToken].value==26){


        if(token[locationOfToken].value==0){
            string a=token[locationOfToken].vacabulary;
            if(!fun_loopTheSyn(a)){
                cout<<"表达式中出现了未定义的标识符"<<a<<"!!"<<endl;
                exit(0);
            }
        }




        sem.push(token[locationOfToken].vacabulary);
        locationOfToken++;
        return 1;
    }
    else if(token[locationOfToken].value==24){
        locationOfToken++;
        if(fun_ssbds()){
            if(token[locationOfToken].value==25){
                locationOfToken++;
                return 1;
            }
        }
        //return 0;
    }
    return 0;
}

int fun_smysf(){//双目运算符
    //cout<<token[locationOfToken].vacabulary<<endl;
    if((token[locationOfToken].value==34)||(token[locationOfToken].value==35)||(token[locationOfToken].value==14)||
       (token[locationOfToken].value==15)||(token[locationOfToken].value==10)||(token[locationOfToken].value==11)||(token[locationOfToken].value==12)||(token[locationOfToken].value==37)){
        locationOfToken++;
      //  cout<<token[locationOfToken].vacabulary<<endl;
        return 1;
    }
    else
        return 0;
}

int fun_blsm(){//变量说明
    if(fun_type1()){
        typeOfbl=token[locationOfToken-1].value;
        if(fun_blbsfb()){
            if(token[locationOfToken].value==23){
                locationOfToken++;
                return 1;
            }
        }
    }
    return 0;
}

int fun_blbsfb(){//变量标识符表
    if(fun_blbsfb1()){
        return 1;
    }
    else{
      //  cout<<"err_变量标识符表"<<endl;
        return 0;
    }
}

int fun_blbsfb1(){//变量标识符表
    if(fun_blf()){
        if(token[locationOfToken].value==22){
            locationOfToken++;
            fun_blbsfb1();
            return 1;
        }
        else{
            if(token[locationOfToken].value==23){
                return 1;
            }
        }
    }
    return 0;
}

int fun_loopTheSyn(string a){
    int l=0;
    for(int i=0;i<locationOfSynbl;i++){
        if(synbl_1[i].name==a){
            l=1;
        }
    }
    return l;
}

int fun_blf(){//变量符
    //cout<<"asd"<<endl;



        string a=token[locationOfToken].vacabulary;
        if(!fun_loopTheSyn(a)){
            record[locationOfRecord].name=token[locationOfToken].vacabulary;
            record[locationOfRecord].address=address;
            if(typeOfbl==4){
                address=address+2;
                synbl_1[locationOfSynbl].typ='i';
            }
            else if(typeOfbl==9){
                address++;
                synbl_1[locationOfSynbl].typ='c';
            }
            else if(typeOfbl==27){
                address=address+4;
                synbl_1[locationOfSynbl].typ='r';
            }
            synbl_1[locationOfSynbl].name=token[locationOfToken].vacabulary;
            synbl_1[locationOfSynbl].cat="v";
            synbl_1[locationOfSynbl].addr1=record[locationOfRecord].address;
           // cout<<synbl_1[locationOfSynbl].name<<synbl_1[locationOfSynbl].typ<<synbl_1[locationOfSynbl].cat<<synbl_1[locationOfSynbl].addr1<<endl;
            locationOfSynbl++;
            locationOfRecord++;
        }
        else{
            cout<<"\n出现了重复定义的变量"<<a<<endl;
            system("pause");
        }




    if(fun_fuzhibds()){
        return 1;
    }
    else if((token[locationOfToken].value==0&&token[locationOfToken+1].value==23)||(token[locationOfToken].value==0&&token[locationOfToken+1].value==22)){
        locationOfToken++;
        return 1;
    }
    return 0;
}

void output_qt(){//输出四元式序列
    for(int i=0;i<locOfqt;i++){
        cout<<i<<":  "<<qt[i]<<endl;
    }
}

void show(){//输出活动记录
    cout<<"    活动记录\t\t"<<endl;
    cout<<"编号\t"<<"地址\t"<<"映像\t"<<endl;
    for(int i=0;i<500;i++){
        if(record[i].name!=""){
            cout<<i<<"\t"<<record[i].address<<"\t"<<record[i].name<<endl;
        }
    }
}

void show1(){//输出符号表
    cout<<"\t   符号表\t\t"<<endl;
    cout<<"Name\t"<<"Type\t"<<"Cat\t"<<"Addr\t"<<endl;
    for(int i=0;i<500;i++){
        if(synbl_1[i].name!=""){
            cout<<synbl_1[i].name<<"\t"<<synbl_1[i].typ<<"\t"<<synbl_1[i].cat<<"\t"<<synbl_1[i].addr1<<endl;
        }
    }
    show2();
}

void show2(){//输出常数表
    cout<<"\n常数表"<<endl;
    //cout<<"Name\t"<<"Type\t"<<"Cat\t"<<"Addr\t"<<endl;
    for(int i=0;i<locOfconst;i++){
        cout<<"\n"<<constarr[i]<<endl;
    }

}


int main()
{
    input1();
    analyse();//词法分析
    output();//词法分析阶段输出
    system("pause");
    system("cls");
    fun_program();
    return 0;
}
