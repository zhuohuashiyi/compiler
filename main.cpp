/*
 created by zhuohuashiyi on 2021.12.5
 */
#include <iostream>
#include<string>
#include<stack>
#include<vector>
#include<algorithm>

using namespace std;

// 字符12+26+26+10=75
char mapping[75] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
        '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', ';', ',', '(', ')', '=', ':',
        '>', '<', '~'
};
int countNFAStates(vector<int>**NFA){
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
int findVec(vector<int>temp, int t){
    int ans=-1;
    for(int i=0;i<temp.size();i++){
        if(temp[i]==t)return i;
    }
    return ans;
}
void displayNFA(vector<int>**NFA){
    vector<int>indexes;
    for(int i=0;i<100;i++){
        for(int j=0;j<75;j++){
            vector<int>temp=NFA[i][j];
            for(int k=0;k<temp.size();k++){
                if(temp[k]!=-1){
                    if(findVec(indexes, j)==-1)indexes.push_back(j);
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
                for(int k=0;k<temp.size();k++){
                    cout<<temp[k];
                    if(k!=temp.size()-1)cout<<",";
                }
                cout<<"} ";
            }
        }
        cout<<endl;
    }
    cout<<"endStates:"<<endl;
    cout<<stateNum-1<<endl;
}
int find(char ch){
    int answer=0;
    for(;;answer++){
        if(mapping[answer]==ch)
            return answer;
    }
}
void copyState(vector<int>**temp, vector<int>**e, int begin){
    for(int i=0;i<500;i++){
        for(int j=0;j<75;j++){
            vector<int>temp1=e[i][j];
            if(temp1.size()==0)continue;
            for(int k=0;k<temp1.size();k++){
                if(temp1[k]>0)temp1[k]+=begin;
            }
            temp[begin+i][j]=temp1;
        }
    }
}
vector<int>**orNFA(vector<int>**left, vector<int>**right){
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
vector<int>**andNFA(vector<int>**left,vector<int>**right){
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
vector<int>** closureNFA(vector<int>**item){
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
vector<int>** generateNFA(string postfix, int &stateNum)
{
    vector<int>**temp,**e1,**e2,**e;
    int p;
    stack<vector<int>**>operation;
    for(p=0; p<postfix.size(); p++){
        switch(postfix[p]){
            case '|':
                e2 = operation.top();
                operation.pop();
                e1 = operation.top();
                operation.pop();
                temp=orNFA(e1, e2);
                operation.push(temp);
                break;
            case '*':
                e = operation.top();
                operation.pop();
                temp= closureNFA(e);
                operation.push(temp);
                break;
            default:
                temp=new vector<int>*[500];
                for(int i=0;i<500;i++){
                    temp[i]=new vector<int> [75];
                }
                int index=find(postfix[p]);
                temp[0][index].push_back(1);
                for(int i=0;i<75;i++)temp[1][i].push_back(-1);
                operation.push(temp);
                break;
        }
    }
    while(operation.size()!=1){
        e1 = operation.top();
        operation.pop();
        e2 = operation.top();
        operation.pop();
        temp= andNFA(e2,e1);
        operation.push(temp);
    }
    e = operation.top();
    operation.pop();
    stateNum= countNFAStates(e);
    return e;
}
int prio(char op){
    if(op=='(')return 0;
    else if(op=='*')return 2;
    else if(op=='|')return 1;
    return 0;
}
string generateSuffix(string infix){
    stack<char> s;
    string suffix="";
    int i;
    for (i = 0; i<infix.size(); i++)
    {
        if (infix[i] >= '0' && infix[i] <= '9'||infix[i] >= 'a' && infix[i] <= 'z'||infix[i]>='A'&&infix[i]<='Z')      {
            suffix+=infix[i];
        }
        else                          //否则不是数字
        {
            if (s.empty())            //栈空则入站
                s.push(infix[i]);
            else if (infix[i] == '(')   //左括号入栈
                s.push(infix[i]);
            else if (infix[i] == ')')    //如果是右括号，只要栈顶不是左括号，就弹出并输出
            {
                while (s.top() != '(')
                {
                    suffix+= s.top();
                    s.pop();
                }
                s.pop();                 //弹出左括号，但不输出
            }
            else
            {
                while (prio(infix[i]) <= prio(s.top()))   //栈顶优先级大于等于当前运算符，则输出
                {
                    suffix+= s.top();
                    s.pop();
                    if (s.empty())      //栈为空，停止
                        break;
                }
                s.push(infix[i]);   //把当前运算符入栈
            }
        }
    }
    while (!s.empty())        //最后，如果栈不空，则弹出所有元素并输出
    {
        suffix+= s.top();
        s.pop();
    }

    return suffix;
}
vector<int>findEqual(vector<int>**NFA, int init){
    vector<int>answer;
    answer.push_back(init);
    int history[500]={0};
    for(int i=0;i<answer.size();i++){
        if(history[answer[i]]==0){
            vector<int>temp1=NFA[answer[i]][74];
            for(int j=0;j<temp1.size();j++){
                if(temp1[j]>0)answer.push_back(temp1[j]);
            }
            //cout<<endl;
            history[answer[i]]=1;
        }
    }
    return answer;
}
int findSubArray(vector<vector<int>>temp, vector<int>temp1){
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
int find(vector<int>a, int b){
    for(int i=0;i<a.size();i++){
        if(a[i]==b)return i;
    }
    return -1;
}
int **convertDFA(vector<int>**NFA, vector<int>&indexes, int &statesNum, vector<int>&isEnd, int num1){
    int tNum=0;
    int symbol;
    for(int i=0;i<num1;i++){
        for(int j=0;j<75;j++){
            vector<int>temp=NFA[i][j];
            for(int k=0;k<temp.size();k++){
                if(temp[k]!=-1){
                    if(findVec(indexes, j)==-1)indexes.push_back(j);
                    break;
                }
            }
        }
    }
    sort(indexes.begin(),indexes.end());
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
    int t2=find(temp, num1-1);
    if(t2==-1)isEnd.push_back(0);
    else isEnd.push_back(1);
    sign.push_back(1);
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
            if(findSubArray(states, temp1)==-1){
                states.push_back(temp1);
                sign.push_back(0);
                int t2=find(temp1, num1-1);
                if(t2==-1)isEnd.push_back(0);
                else isEnd.push_back(1);
            }
            DFA[tNum][i]=findSubArray(states, temp1);

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
        if(symbol==0)break;
    }
    statesNum=states.size();
    return DFA;
}
int **simplifyDFA(int **DFA, vector<int>isEnd){
    return NULL;
}
int find(vector<char>a, char b){
    for(int i=0;i<a.size();i++){
        if(a[i]==b)return i;
    }
    return -1;
}
int lexicalAnalysis(int **DFA, string input, vector<char>character, vector<int>isEnd, vector<string>&answer){
    int pos=0;
    int nowState=0;
    string s1="";
    while(true){
        if(input[pos]==' '){
            if(isEnd[nowState]){
                answer.push_back(s1);
                nowState=0;
                s1="";
            }
            else{
                return -1;
            }
            pos++;
        }
        if(pos>input.size()-1){
            if(s1.size()!=0){
                answer.push_back(s1);
            }
            break;
        }
        int t1=find(character, input[pos]);
        if(t1==-1||DFA[nowState][t1]==-1){
            if(isEnd[nowState]){
                answer.push_back(s1);
                nowState=0;
                s1="";
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
void displayDFA(int **DFA,int num,vector<char>character,vector<int>isEnd){
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
vector<int>**charNFA(char ch){
    vector<int>**answer=new vector<int>*[500];
    for(int i=0;i<500;i++)answer[i]=new vector<int>[75];
    answer[0][find(ch)].push_back(1);
    for(int i=0;i<75;i++){
        answer[1][i].push_back(-1);
    }
    return answer;
}
int main() {
    string keyword_1="PROGRAM";
    string keyword_2="BEGIN";
    string keyword_3="END";
    string keyword_4="CONST";
    string keyword_5="VAR";
    string keyword_6="WHILE";
    string keyword_7="DO";
    string keyword_8="IF";
    string keyword_9="THEN";
    string str1="a|b|c|d|e|f|q|w|r|t|y|u|i|o|p|s|g|h|j|k|l|z|x|v|n|m";
    string str2="Q|W|E|R|T|Y|U|I|O|P|A|S|D|F|G|H|J|K|L|Z|X|C|V|B|N|M";
    string str3="0|1|2|3|4|5|6|7|8|9";
    string identifier_1=str1+"|"+str2;
    string identifier_2="("+str1+"|"+str2+"|"+str3+")*";
    string integer_1 = "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*";
    string integer_2 = "0";
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
    vector<int>**wholeNFA;
    int NFAForIntegerStatesNum,NFAForDelimiterStatesNum,NFAForIdentifierStatesNum,NFAForKeywordStatesNum;
    int wholeNFAStatesNum;
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
    wholeNFA=orNFA(NFAForInteger,NFAForIdentifier);
    wholeNFA=orNFA(wholeNFA, NFAForDelimiter);
    wholeNFA= orNFA(wholeNFA,NFAForKeyword);
    wholeNFAStatesNum= countNFAStates(wholeNFA);
    wholeDFA=
    /*DFAForInteger= convertDFA(NFAForInteger, indexesForInteger, DFAForIntegerStatesNum, isEndForInteger, NFAForIntegerStatesNum);
    for(int i=0;i<indexesForInteger.size()-1;i++)characterForInteger.push_back(mapping[indexesForInteger[i]]);
    DFAForIdentifier= convertDFA(NFAForIdentifier, indexesForIdentifier, DFAForIdentifierStatesNum, isEndForIdentifier, NFAForIdentifierStatesNum);
    for(int i=0;i<indexesForIdentifier.size()-1;i++)characterForIdentifier.push_back(mapping[indexesForIdentifier[i]]);
    DFAForKeyword= convertDFA(NFAForKeyword, indexesForKeyword, DFAForKeywordStatesNum, isEndForKeyword, NFAForKeywordStatesNum);
    for(int i=0;i<indexesForKeyword.size()-1;i++)characterForKeyword.push_back(mapping[indexesForKeyword[i]]);
    DFAForDelimiter = convertDFA(NFAForDelimiter, indexesForDelimiter, DFAForDelimiterStatesNum, isEndForDelimiter, NFAForDelimiterStatesNum);
    for(int i=0;i<indexesForDelimiter.size()-1;i++)characterForDelimiter.push_back(mapping[indexesForDelimiter[i]]);*/
    displayDFA(DFAForDelimiter, DFAForDelimiterStatesNum, characterForDelimiter, isEndForDelimiter);
    string input="VAR i1,j2,k3;";
    int code=lexicalAnalysis(DFAForDelimiter, input, characterForDelimiter, isEndForDelimiter, answer);
    cout<<code<<endl;
    for(int i=0;i<answer.size();i++)cout<<answer[i]<<" ";
    return 0;
}
