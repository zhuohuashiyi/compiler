/*
 created by zhuohuashiyi on 2021.12.5
 针对PLO语言的一个简单的词法分析器的实现
 */
#include <iostream>
#include<string>
#include<stack>
#include<vector>
#include<algorithm>
using namespace std;
// 字符12+26+26+10+1=75
char mapping[75] = {    //所有可能出现的字符，'~'代表空字符ε
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
        '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', ';', ',', '(', ')', '=', ':',
        '>', '<', '~'
};
string delimiter[13]={" ","+","-","*","/",",",";","(",")","=",":",">","<"};  //所有可能的界符，理论上任何两个不是界符的词法单元之间不能没有界符
template<typename T>    //模板函数定义，从一个列表中寻找元素，返回元素的位置，找不到则返回-1
int find(vector<T>a,T b){
    for(int i=0;i<a.size();i++){
        if(a[i]==b)return i;
    }
    return -1;
}
int find(string b){   //从delimiter数组中找字符串b
    for(int i=0;i<13;i++){
        if(delimiter[i]==b){
            return i;
        }
    }
    return  -1;
}
int find(char ch){   //从mapping数组中找字符ch
    int answer=0;
    for(;;answer++){
        if(mapping[answer]==ch)
            return answer;
    }
}
int countNFAStates(vector<int>**NFA){  //计算一个NFA中的状态数目
    int answer=0;
    for(int i=0;i<300;i++){
        int isNull=0;
        for(int j=0;j<75;j++){
            if(NFA[i][j].size()!=0){
                isNull=1;
                break;
            }
        }
        if(isNull==0)break;
        answer++;
    }
    return answer;
}
void displayNFA(vector<int>**NFA){   //输出显示一个NFA
    vector<int>indexes;   //寻找该NFA中的有效字符
    for(int i=0;i<100;i++){
        for(int j=0;j<75;j++){
            vector<int>temp=NFA[i][j];

            for(int k=0;k<temp.size();k++){
                if(temp[k]!=-1){
                    if(find(indexes, j)==-1)indexes.push_back(j);
                    break;
                }
            }
        }
    }
    int stateNum= countNFAStates(NFA);
    sort(indexes.begin(), indexes.end());
    for(int i=-1;i<stateNum;i++){
        if(i==-1){
            cout<<"comingChar:";
        }
        else{
            cout<<"state"<<i<<":";
        }
        for(int j=0;j<indexes.size();j++){
            if(i==-1){
                cout<<mapping[indexes[j]]<<" ";
            }
            else{
                vector<int>temp=NFA[i][indexes[j]];
                cout<<"{";
                sort(temp.begin(),temp.end());
                for(int k=0;k<temp.size();k++){
                    if(temp[k]>0){
                        cout<<temp[k];
                        if(k!=temp.size()-1)cout<<",";
                    }
                }
                cout<<"} ";
            }
        }
        cout<<endl;
    }
    cout<<"endStates:"<<endl;  //这里输出NFA的状态，这里默认终态一定是NFA的最后一个状态
    cout<<stateNum-1<<endl;
}
void displayDFA(int **DFA,int num,vector<char>character,vector<int>isEnd){  //DFA输出显示
    for(int i=-1;i<num;i++){
        if(i==-1){
            cout<<"comingChar:";
        }
        else{
            cout<<"states"<<i<<":";
        }
        for(int j=0;j<character.size();j++){
            if(i==-1){
                cout<<character[j]<<" ";
            }
            else{
                cout<<DFA[i][j]<<" ";
            }
        }
        cout<<endl;
    }
    cout<<"endStates:"<<endl;
    for(int i=0;i<isEnd.size();i++){
        if(isEnd[i]==1)cout<<i<<" ";
    }
    cout<<endl;
}
void copyState(vector<int>**temp, vector<int>**e, int begin){  //该函数将e的状态复制到temp中，temp的begin位置开始复制
    for(int i=0;i<500;i++){
        for(int j=0;j<75;j++){
            vector<int>temp1=e[i][j];
            if(temp1.size()==0)continue;
            for(int k=0;k<temp1.size();k++){
                if(temp1[k]>0)temp1[k]+=begin;  //复制过去的状态要重新计算
            }
            temp[begin+i][j]=temp1;
        }
    }
}
vector<int>**orNFA(vector<int>**left, vector<int>**right){  //对两个NFA n1、n2，计算n1|n2
    vector<int>**temp=new vector<int>*[500];
    for(int i=0;i<500;i++){
        temp[i]=new vector<int>[75];
    }
    int stateNum1= countNFAStates(left);
    int stateNum2= countNFAStates(right);
    temp[0][74].push_back(1);
    temp[0][74].push_back(stateNum1+1);
    copyState(temp, left, 1);
    temp[stateNum1][74].push_back(stateNum1+stateNum2+1);
    copyState(temp, right, stateNum1+1);
    temp[stateNum1+stateNum2][74].push_back(stateNum1+stateNum2+1);
    for(int i=0;i<75;i++){
        temp[stateNum1+stateNum2+1][i].push_back(-1);
    }
    return temp;
}
vector<int>**andNFA(vector<int>**left,vector<int>**right){  //对于两个NFA n1,n2,计算n1n2
    vector<int>**temp=new vector<int>*[500];
    for(int i=0;i<500;i++){
        temp[i]=new vector<int>[75];
    }
    int stateNum1= countNFAStates(left);
    int stateNum2= countNFAStates(right);
    copyState(temp, left, 0);
    copyState(temp, right, stateNum1);
    temp[stateNum1-1][74].push_back(stateNum1);
    return temp;
}
vector<int>** closureNFA(vector<int>**item){  //对NFA n1,计算n1*
    vector<int>**temp=new vector<int>*[500];
    for(int i=0;i<500;i++){
        temp[i]=new vector<int>[75];
    }
    int stateNum1= countNFAStates(item);
    temp[0][74].push_back(stateNum1+1);
    copyState(temp, item, 1);
    temp[0][74].push_back(1);
    temp[stateNum1][74].push_back(stateNum1+1);
    temp[stateNum1][74].push_back(1);
    for(int i=0;i<75;i++){
        temp[stateNum1+1][i].push_back(-1);
    }
    return temp;
}
vector<int>**charNFA(char ch){   //对于单个字符的NFA生成
    vector<int>**answer=new vector<int>*[500];
    for(int i=0;i<500;i++)answer[i]=new vector<int>[75];
    answer[0][find(ch)].push_back(1);
    for(int i=0;i<75;i++){
        answer[1][i].push_back(-1);
    }
    return answer;
}
vector<int>** generateNFA(string postfix, int &stateNum)   //生成NFA的算法，根据正规表达式的后缀形式，生成一个NFA并且返回，并且将状态数保存在stateNum中
{
    vector<int>**temp,**n1,**n2,**n;
    int p;
    stack<vector<int>**>operation;
    for(p=0; p<postfix.size(); p++){
        switch(postfix[p]){
            case '|':  //遇到|字符，取出栈顶的两个NFA进行|运算，计算结果在压入栈中
                n2 = operation.top();
                operation.pop();
                n1 = operation.top();
                operation.pop();
                temp=orNFA(n1, n2);
                operation.push(temp);
                break;
            case '*':  //遇到*符号，取出栈顶的一个NFA进行闭包运算，计算结果压入栈中
                n = operation.top();
                operation.pop();
                temp= closureNFA(n);
                operation.push(temp);
                break;
            default:   //遇到单个字符，为其生成一个NFA，压入栈中
                temp= charNFA(postfix[p]);
                operation.push(temp);
                break;
        }
    }  //经过以上操作，栈中存在很多NFA，它们之间是and运算，取出进行运算
    while(operation.size()!=1){
        n1 = operation.top();
        operation.pop();
        n2 = operation.top();
        operation.pop();
        temp= andNFA(n2,n1);
        operation.push(temp);
    }
    n = operation.top();  //当栈中只有一个NFA的时候，就是我们的结果，返回
    operation.pop();
    stateNum= countNFAStates(n);
    return n;
}
int prio(char op){   //定义运算符的优先级，为后缀表达式的生成做准备
    if(op=='(')return 0;
    else if(op=='*')return 2;
    else if(op=='|')return 1;
    return 0;
}
string generateSuffix(string infix){   //生成后缀表达式的算法
    stack<char> s;
    string suffix="";
    int i;
    for (i = 0; i<infix.size(); i++)
    {
        if (infix[i] >= '0' && infix[i] <= '9'||infix[i] >= 'a' && infix[i] <= 'z'||infix[i]>='A'&&infix[i]<='Z'){
            suffix+=infix[i];
        }
        else
        {
            if (s.empty())
                s.push(infix[i]);
            else if (infix[i] == '(')
                s.push(infix[i]);
            else if (infix[i] == ')')
            {
                while (s.top() != '(')
                {
                    suffix+= s.top();
                    s.pop();
                }
                s.pop();
            }
            else
            {
                while (prio(infix[i]) <= prio(s.top()))
                {
                    suffix+= s.top();
                    s.pop();
                    if (s.empty())
                        break;
                }
                s.push(infix[i]);
            }
        }
    }
    while (!s.empty())
    {
        suffix+= s.top();
        s.pop();
    }
    return suffix;
}
vector<int>findEqual(vector<int>**NFA, int init){   //寻找init状态的等价集，即在NFA中，init经过若干个空字符可以大大的状态集合
    vector<int>answer;
    answer.push_back(init);
    int history[500]={0};
    for(int i=0;i<answer.size();i++){
        if(history[answer[i]]==0){
            vector<int>temp1=NFA[answer[i]][74];
            for(int j=0;j<temp1.size();j++){
                if(temp1[j]>0)answer.push_back(temp1[j]);
            }
            history[answer[i]]=1;
        }
    }
    return answer;
}
int findSubArray(vector<vector<int>>temp, vector<int>temp1){  //对于一个二维数组，查找一个数组是否在这个二维数组中
    int symbol;
    for(int i=0;i<temp.size();i++){
        vector<int>temp2=temp[i];
        if(temp1.size()!=temp2.size())continue;
        symbol=1;
        for(int j=0;j<temp1.size();j++){
            if(temp1[j]!=temp2[j]){
                symbol=0;
                break;
            }
        }
        if(symbol==0)continue;
        return i;
    }
    return -1;
}

int **convertDFA(vector<int>**NFA, vector<int>&indexes, int &statesNum, vector<int>&isEnd, int num1){
    /*
     该函数根据NFA生成DFA
     并且将DFA中有效的字符在mapping数组中的序号保存在indexes数组
     把DFA中每个状态是否是状态保存在isEnd数组中
     DFA的状态数保存在statesNum中
     */
    int tNum=0;
    int symbol;
    for(int i=0;i<num1;i++){
        for(int j=0;j<75;j++){
            vector<int>temp=NFA[i][j];
            for(int k=0;k<temp.size();k++){
                if(temp[k]!=-1){
                    if(find(indexes, j)==-1)indexes.push_back(j);
                    break;
                }
            }
        }
    }
    sort(indexes.begin(),indexes.end());  //以上先得到NFA中的有效字符
    int num=indexes.size()-1;
    int **DFA=new int*[500];
    for(int i=0;i<500;i++){
        DFA[i]=new int[num];
        for(int j=0;j<num;j++)DFA[i][j]=-1;
    }
    vector<vector<int> >states;
    vector<int>sign;
    vector<int>temp= findEqual(NFA, 0);
    states.push_back(temp);
    int t2=find(temp, num1-1);   //只有该状态集包括NFA的状态才能成为DFA的终态
    if(t2==-1)isEnd.push_back(0);
    else isEnd.push_back(1);
    sign.push_back(1);    //sign函数标志对应的状态有没有经过计算
    while(true){
        for(int i=0;i<num;i++){
            vector<int>temp1;
            for(int j=0;j<temp.size();j++){
                vector<int>temp2=NFA[temp[j]][indexes[i]];
                for(int k=0;k<temp2.size();k++){
                    if(temp2[k]>0)temp1.push_back(temp2[k]);
                }
            }
            if(temp1.empty())continue;
            vector<int>temp3= findEqual(NFA, temp1[0]);
            for(int j=1;j<temp3.size();j++){
                temp1.push_back(temp3[j]);
            }
            if(findSubArray(states, temp1)==-1){  //保证循环能退出
                states.push_back(temp1);
                sign.push_back(0);
                int t2=find(temp1, num1-1);
                if(t2==-1)isEnd.push_back(0);
                else isEnd.push_back(1);
            }
            DFA[tNum][i]=findSubArray(states, temp1);  //DFA赋值

        }
        symbol=0;
        for(int i=0;i<sign.size();i++){
            if(sign[i]==0){
                temp=states[i];
                sign[i]=1;
                tNum++;
                symbol=1;
                break;
            }
        }
        if(symbol==0)break;   //找不到需要计算的DFA时，则退出该函数
    }
    statesNum=states.size();
    return DFA;
}
int lexicalAnalysis(int **DFA, string input, vector<char>character, vector<int>isEnd, vector<string>&answer,int&pos,int symbol,int &tryNum){
    /*
     词法分析器，根据自动机DFA，对input进行词法分析，并且将结果保存在answer中
     pos表示当前扫描到的input字符串的位置
     symbol表示四种词法单元中的一种
     tryNum对于某个字符，已经使用了几个自动机还没有识别出
     */
    int nowState=0;
    string s1="";
    while(true){
        if(input[pos]==' '){
            if(isEnd[nowState]){
                if(symbol==2){
                    s1="<V," + s1+ ">";
                }
                else{
                    s1="<"+s1+">";
                }
                answer.push_back(s1);
                nowState=0;
                tryNum=0;
                s1="";
            }
            else{
                return -1;
            }
            pos++;
        }
        if(pos>input.size()-1){
            if(s1.size()!=0){
                if(symbol==2){
                    s1="<V," + s1+ ">";
                }
                else{
                    s1="<"+s1+">";
                }
                tryNum=0;
                answer.push_back(s1);

            }
            break;
        }
        int t1=find(character, input[pos]);
        if(t1==-1||DFA[nowState][t1]==-1){
            if(isEnd[nowState]){
                if(symbol==1||(pos<input.size()&&find(string(1,input[pos]))>=0)){
                    if(symbol==2){
                        s1="<V," + s1+ ">";
                    }
                    else{
                        s1="<"+s1+">";
                    }
                    answer.push_back(s1);
                    nowState=0;
                    tryNum=0;
                    s1="";
                }
                else{
                    cout<<input[pos]<<" ";   //这里直接返回-2，表示因为两个非界符的词法单元之间无界符的错误
                    return -2;
                }
            }
            else{
                return -1;
            }
        }
        else{
            nowState=DFA[nowState][t1];
            s1+=input[pos++];
        }
    }
    return 0;
}
void lexicalAnalyzer(){
    /*
     词法分析器：
     输入一个字符串进行词法分析，如果不输入则使用默认字符串"VAR i1=1,j2=3,k3=i1*j2;"
     */
    string keyword_1="PROGRAM";
    string keyword_2="BEGIN";
    string keyword_3="END";
    string keyword_4="CONST";
    string keyword_5="VAR";
    string keyword_6="WHILE";
    string keyword_7="DO";
    string keyword_8="IF";
    string keyword_9="THEN";
    //以下是PLO语言中关键词定义的词法规则
    string str1="a|b|c|d|e|f|q|w|r|t|y|u|i|o|p|s|g|h|j|k|l|z|x|v|n|m";
    string str2="Q|W|E|R|T|Y|U|I|O|P|A|S|D|F|G|H|J|K|L|Z|X|C|V|B|N|M";
    string str3="0|1|2|3|4|5|6|7|8|9";
    string identifier_1=str1+"|"+str2;
    string identifier_2="("+str1+"|"+str2+"|"+str3+")*";
    //以上是标识符的词法规则，只能以字母开头，包含字符或者数字的字符串
    string integer_1 = "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*";
    string integer_2 = "0";
    //以上是整数的词法规则，要么是0，或者以1到9开头包含任意数字的字符串
    char delimiter_1 = '+';
    char delimiter_2 = '-';
    char delimiter_3 = '*';
    char delimiter_4='/';
    char delimiter_5='=';
    char delimiter_6='>';
    char delimiter_7='<';
    char delimiter_8=',';
    char delimiter_9=';';
    char delimiter_10='(';
    char delimiter_11=')';
    string delimiter_12=":=";
    string delimiter_13="<>";
    string delimiter_14=">=";
    string delimiter_15="<=";
    //以上是所有界符的词法规则
    vector<int>isEndForInteger,isEndForKeyword,isEndForIdentifier,isEndForDelimiter;
    vector<int>indexesForInteger,indexesForKeyword,indexesForIdentifier,indexesForDelimiter;
    vector<string>answer;
    vector<char>characterForInteger,characterForKeyword,characterForIdentifier,characterForDelimiter;
    int **DFAForKeyword,**DFAForIdentifier,**DFAForInteger,**DFAForDelimiter,**wholeDFA;
    int DFAForKeywordStatesNum,DFAForIdentifierStatesNum,DFAForIntegerStatesNum,DFAForDelimiterStatesNum,wholeStatesNum;
    vector<int>**NFA_1,**NFA_2,**NFA_3,**NFA_4,**NFA_5,**NFA_6,**NFA_7,**NFA_8,**NFA_9;
    vector<int>**NFA_10,**NFA_11,**NFA_12,**NFA_13,**NFA_14,**NFA_15;
    int num_1,num_2,num_3,num_4,num_5,num_6,num_7,num_8,num_9;
    vector<int>**NFAForInteger;
    vector<int>**NFAForDelimiter;
    vector<int>**NFAForIdentifier;
    vector<int>**NFAForKeyword;
    int NFAForIntegerStatesNum,NFAForDelimiterStatesNum,NFAForIdentifierStatesNum,NFAForKeywordStatesNum;
    //以上定义所有用到的变量
    NFA_1= generateNFA(generateSuffix(integer_1), num_1);
    NFA_2=generateNFA(generateSuffix(integer_2), num_2);
    NFAForInteger= orNFA(NFA_1, NFA_2);
    NFAForIntegerStatesNum=num_1+num_2+2;
    NFA_1= generateNFA(generateSuffix(identifier_1), num_1);
    NFA_2= generateNFA(generateSuffix(identifier_2), num_2);
    NFAForIdentifier=andNFA(NFA_1,NFA_2);
    NFAForIdentifierStatesNum=num_1+num_2;
    NFA_1= generateNFA(generateSuffix(keyword_1), num_1);
    NFA_2= generateNFA(generateSuffix(keyword_2), num_2);
    NFA_3= generateNFA(generateSuffix(keyword_3), num_3);
    NFA_4= generateNFA(generateSuffix(keyword_4), num_4);
    NFA_5= generateNFA(generateSuffix(keyword_5), num_5);
    NFA_6= generateNFA(generateSuffix(keyword_6), num_6);
    NFA_7= generateNFA(generateSuffix(keyword_7), num_7);
    NFA_8= generateNFA(generateSuffix(keyword_8), num_8);
    NFA_9= generateNFA(generateSuffix(keyword_9), num_9);
    NFAForKeyword=orNFA(NFA_1,NFA_2);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_3);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_4);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_5);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_6);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_7);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_8);
    NFAForKeyword= orNFA(NFAForKeyword,NFA_9);
    NFAForKeywordStatesNum=num_1+num_2+num_3+num_4+num_5+num_6+num_7+num_8+num_9+2*8;
    NFA_1= charNFA(delimiter_1);
    NFA_2= charNFA(delimiter_2);
    NFA_3= charNFA(delimiter_3);
    NFA_4= charNFA(delimiter_4);
    NFA_5= charNFA(delimiter_5);
    NFA_6= charNFA(delimiter_6);
    NFA_7= charNFA(delimiter_7);
    NFA_8= charNFA(delimiter_8);
    NFA_9= charNFA(delimiter_9);
    NFA_10= charNFA(delimiter_10);
    NFA_11= charNFA(delimiter_11);
    NFA_12= charNFA(':');
    NFAForDelimiter= orNFA(NFA_1,NFA_2);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_3);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_4);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_5);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_6);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_7);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_8);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_9);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_10);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_11);
    NFA_13= andNFA(NFA_12,NFA_5);
    NFA_14= andNFA(NFA_7,NFA_6);
    NFA_15= andNFA(NFA_6,NFA_5);
    NFA_1= andNFA(NFA_7,NFA_5);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_13);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_1);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_14);
    NFAForDelimiter=orNFA(NFAForDelimiter,NFA_15);
    NFAForDelimiterStatesNum=66;
    //以上生成四种类别的NFA
    DFAForInteger= convertDFA(NFAForInteger, indexesForInteger, DFAForIntegerStatesNum, isEndForInteger, NFAForIntegerStatesNum);
    for(int i=0;i<indexesForInteger.size()-1;i++)characterForInteger.push_back(mapping[indexesForInteger[i]]);
    DFAForIdentifier= convertDFA(NFAForIdentifier, indexesForIdentifier, DFAForIdentifierStatesNum, isEndForIdentifier, NFAForIdentifierStatesNum);
    for(int i=0;i<indexesForIdentifier.size()-1;i++)characterForIdentifier.push_back(mapping[indexesForIdentifier[i]]);
    DFAForKeyword= convertDFA(NFAForKeyword, indexesForKeyword, DFAForKeywordStatesNum, isEndForKeyword, NFAForKeywordStatesNum);
    for(int i=0;i<indexesForKeyword.size()-1;i++)characterForKeyword.push_back(mapping[indexesForKeyword[i]]);
    DFAForDelimiter = convertDFA(NFAForDelimiter, indexesForDelimiter, DFAForDelimiterStatesNum, isEndForDelimiter, NFAForDelimiterStatesNum);
    for(int i=0;i<indexesForDelimiter.size()-1;i++)characterForDelimiter.push_back(mapping[indexesForDelimiter[i]]);
   //以上用四种NFA生成四种DFA
     string input;
    cout<<"Please enter the lexical rule:"<<endl;
    getline(cin,input);
    int pos=0;
    int code;
    int tryNum=0;
    //以下进行循环，进行词法分析，只有当函数返回-2或者四种词法分析都不能识别的时候识别失败，用tryNum表示当前字符失败的DFA识别
    while(pos<input.size()){
        code=lexicalAnalysis(DFAForKeyword, input, characterForKeyword, isEndForKeyword, answer,pos,0, tryNum);
        if(code==-1) tryNum++;
        else if(code==-2){
            answer.push_back("error");
            break;
        }
        code=lexicalAnalysis(DFAForDelimiter, input, characterForDelimiter, isEndForDelimiter, answer,pos,1,tryNum);
        if(code==-1){
            tryNum++;
        }
        else if(code==-2){
            answer.push_back("error");
            break;
        }
        code=lexicalAnalysis(DFAForIdentifier, input, characterForIdentifier, isEndForIdentifier, answer,pos,2,tryNum);
        if(code==-1){
            tryNum++;
        }
        else if(code==-2){
            answer.push_back("error");
            break;
        }
        code=lexicalAnalysis(DFAForInteger, input, characterForInteger, isEndForInteger, answer,pos,3,tryNum);
        if(code==-1){
            tryNum++;
        }
        else if(code==-2){
            answer.push_back("error");
            break;
        }
        if(tryNum>=4){
            answer.push_back("error");
            break;
        }
        else continue;
    }
    cout<<"the lexical elements as belows:"<<endl;
    for(int i=0;i<answer.size();i++)cout<<answer[i]<<" ";
    cout<<endl;
}

int main() {
    string str="l(l|d)*";
    int stateNum1,stateNum2;
    vector<int>**NFA= generateNFA(generateSuffix(str), stateNum1);
    cout<<"NFA as belows:"<<endl;
    displayNFA(NFA);
    vector<int>indexes;
    vector<int>isEnd;
    vector<char>character;
    vector<string>answer;
    int pos=0;
    int temp=0;
    int**DFA= convertDFA(NFA,indexes,stateNum2,isEnd,stateNum1);
    for(int i=0;i<indexes.size()-1;i++)character.push_back(mapping[indexes[i]]);
    cout<<"DFA as belows:"<<endl;
    displayDFA(DFA,stateNum2,character,isEnd);
    string input1="lldld";
    string input2="dldl";
    int code= lexicalAnalysis(DFA,input1,character,isEnd,answer,pos,0,temp);
    if(code==-1)answer.push_back("error");
    cout<<"string "<<input1<<" analysis result as belows:"<<endl;
    for(int i=0;i<answer.size();i++)cout<<answer[i];
    cout<<endl;
    answer.clear();
    pos=0;
    code= lexicalAnalysis(DFA,input2,character,isEnd,answer,pos,0,temp);
    if(code==-1)answer.push_back("error");
    cout<<"string "<<input2<<" analysis result as belows:"<<endl;
    for(int i=0;i<answer.size();i++)cout<<answer[i];
    cout<<endl;
    lexicalAnalyzer();
    return 0;
}
