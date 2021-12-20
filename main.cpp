/*
 created by zhuohuashiyi on 2021.12.5
 */
#include <iostream>
#include<string>
#include<stack>
#include<vector>
#include<algorithm>
#include<tabulate/table.hpp>

using namespace std;

// 字符12+26+26+10=75
char mapping[75] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
        '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', ';', ',', '(', ')', '=', ':',
        '>', '<', '~'
};
int findVec(vector<int>temp, int t){
    int ans=-1;
    for(int i=0;i<temp.size();i++){
        if(temp[i]==t)return i;
    }
    return ans;
}
void display(vector<int>**NFA){
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
    int stateNum=0;
    for(int i=0;i<100;i++){
        for(int j=0;j<75;j++){
            vector<int>temp1=NFA[i][j];
            for(int k=0;k<temp1.size();k++){
                if(temp1[k]!=-1){
                    stateNum++;
                    break;
                }
            }
        }
    }
    stateNum++;

    sort(indexes.begin(), indexes.end());
    tabulate::Table simpleNFA;
    //tabulate::Row row1;
    //row1.add_cell("state");
    //for(int i=0;i<indexes.size();i++)cout<<mapping[indexes[i]]<<" ";
    simpleNFA.add_row({"state", string(1, mapping[indexes[0]]), string(1, mapping[indexes[1]]),
                       string(1, mapping[indexes[2]])});
    for(int i=0;i<stateNum;i++){
        string s1="{";
        vector<int>temp=NFA[i][indexes[0]];
        sort(temp.begin(), temp.end());
        for(int j=0;j<temp.size();j++){
            if(temp[j]!=-1){
                s1+=char(temp[j]+48);
                if(j!=temp.size()-1)s1+=",";
            }

        }
        s1+="}";
        string s2="{";
        temp=NFA[i][indexes[1]];
        sort(temp.begin(), temp.end());
        for(int j=0;j<temp.size();j++){
            if(temp[j]!=-1){
                s2+=char(temp[j]+48);
                if(j!=temp.size()-1)s2+=",";
            }

        }
        s2+="}";
        string s3="{";
        temp=NFA[i][indexes[2]];
        sort(temp.begin(), temp.end());
        for(int j=0;j<temp.size();j++){
            if(temp[j]!=-1){
                s3+=char(temp[j]+48);
                if(j!=temp.size()-1)s3+=",";
            }

        }
        s3+="}";
        simpleNFA.add_row({string(1, char(i+48)), s1, s2, s3});
    }
    cout<<simpleNFA;
}

int find(char ch){
    int answer=0;
    for(;;answer++){
        if(mapping[answer]==ch)
            return answer;
    }
}
void copyState(vector<int>**temp, vector<int>**e, int begin){
    for(int i=0;i<100;i++){
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
vector<int>** generateNFA(string postfix, int &stateNum)
{
    vector<int>**temp,**e1,**e2,**e;
    int p;
    int stateNum1=0,stateNum2=0;
    stack<vector<int>**>operation;
    for(p=0; p<postfix.size(); p++){
        switch(postfix[p]){
            case '|':
                e2 = operation.top();
                operation.pop();

                e1 = operation.top();
                for(int i=0;i<100;i++){
                    for(int j=0;j<74;j++){
                        vector<int>temp=e1[i][j];
                        for(int k=0;k<temp.size();k++)cout<<temp[k];
                        cout<<" ";
                    }
                    cout<<endl;
                }
                operation.pop();
                temp=new vector<int>*[100];
                for(int i=0;i<100;i++){
                    temp[i]=new vector<int> [75];

                }

                stateNum1=0, stateNum2=0;
                for(int i=0;i<100;i++){
                    int isNull=0;
                    for(int j=0;j<75;j++){
                        if(e1[i][j].size()!=0){
                            isNull=1;
                            break;
                        }
                    }

                    if(isNull==0)break;
                    stateNum1++;
                }
                for(int i=0;i<100;i++){
                    int isNull=0;
                    for(int j=0;j<75;j++){
                        if(e2[i][j].size()!=0){
                            isNull=1;
                            break;
                        }
                    }

                    if(isNull==0)break;
                    stateNum2++;
                }
                temp[0][74].push_back(1);
                copyState(temp, e1, 1);
                temp[stateNum1][74].push_back(stateNum1+stateNum2+1);
                temp[0][74].push_back(stateNum1+1);
                copyState(temp, e2, stateNum1+1);
                temp[stateNum1+stateNum2][74].push_back(stateNum1+stateNum2+1);
                for(int i=0;i<75;i++){
                    temp[stateNum1+stateNum2+1][i].push_back(-1);
                }
                //display(temp);
                operation.push(temp);
                break;
            case '*':
                e = operation.top();
                //display(e);
                operation.pop();
                temp=new vector<int>*[100];
                for(int i=0;i<100;i++){
                    temp[i]=new vector<int> [75];

                }
                stateNum1=0;
                for(int i=0;i<100;i++){
                    int isNull=0;
                    for(int j=0;j<75;j++){
                        if(e[i][j].size()!=0){
                            isNull=1;
                            break;
                        }
                    }

                    if(isNull==0)break;
                    stateNum1++;
                }
                //cout<<stateNum1<<endl;
                temp[0][74].push_back(stateNum1+1);
                copyState(temp, e, 1);
                temp[0][74].push_back(1);
                temp[stateNum1][74].push_back(stateNum1+1);
                temp[stateNum1][74].push_back(1);
                for(int i=0;i<75;i++){
                    temp[stateNum1+1][i].push_back(-1);
                }
                //display(temp);
                operation.push(temp);
                break;
            default:
                temp=new vector<int>*[100];
                for(int i=0;i<100;i++){
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
        //display(e1);
        e2 = operation.top();
        //display(e2);
        operation.pop();
        temp=new vector<int>*[100];
        for(int i=0;i<100;i++){
            temp[i]=new vector<int> [75];

        }
        stateNum1=0, stateNum2=0;
        for(int i=0;i<100;i++){
            int isNull=0;
            for(int j=0;j<75;j++){
                if(e1[i][j].size()!=0){
                    isNull=1;
                    break;
                }
            }

            if(isNull==0)break;
            stateNum1++;
        }
        for(int i=0;i<100;i++){
            int isNull=0;
            for(int j=0;j<75;j++){
                if(e2[i][j].size()!=0){
                    isNull=1;
                    break;
                }
            }

            if(isNull==0)break;
            stateNum2++;
        }
        copyState(temp, e2, 0);
        copyState(temp, e1, stateNum2);
        temp[stateNum2-1][74].push_back(stateNum2);
        operation.push(temp);
        //display(temp);
    }
    e = operation.top();
    operation.pop();
    stateNum=0;
    for(int i=0;i<100;i++){
        int isNull=0;
        for(int j=0;j<75;j++){
            if(e1[i][j].size()!=0){
                isNull=1;
                break;
            }
        }

        if(isNull==0)break;
        stateNum++;
    }
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
    int history[100]={0};
    for(int i=0;i<answer.size();i++){
        if(history[answer[i]]==0){
            vector<int>temp1=NFA[answer[i]][74];
            for(int j=0;j<temp1.size();j++){
                //cout<<temp1[j]<<" ";
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
    int **DFA=new int*[100];
    for(int i=0;i<100;i++){
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
        while(input[pos]==' '){
            pos++;
        }
        int t1=find(character, input[pos]);
        if(t1==-1||DFA[nowState][t1]==-1){
            if(isEnd[nowState]){
                answer.push_back(s1);
                s1="";
            }
            else{
                return -1;
            }
            if(pos>=input.size()-1)break;
        }
        else{

            nowState=DFA[nowState][t1];
            s1+=input[pos++];
        }

    }
    return 0;
}

int main() {
    string str1="l(l|d|c)*";
    string keyword="PROGRAM|BEGIN|END|CONST|VAR|WHILE|DO|IF|THEN";
    string identifier_1="a|b|c|d|e|f|q|w|r|t|y|u|i|o|p|s|g|h|j|k|l|z|x|v|n|m";
    string identifier_2="Q|W|E|R|T|Y|U|I|O|P|A|S|D|F|G|H|J|K|L|Z|X|C|V|B|N|M";
    string identifier_3="0|1|2|3|4|5|6|7|8|9";
    string identifier="("+identifier_1+"|"+identifier_2+")"+"("+identifier_3+"|"+
            identifier_1+"|"+identifier_2+")*";
    string integer = "(0|((1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*))";
    string delimiter = "+|-|*|/|=|:=|>|<|,|;|(|)|<>|>=|<=";
    string str2=generateSuffix(str1);
    cout<<str2;
    int stateNum;
    vector<int>** NFA= generateNFA(str2, stateNum);

    //display(NFA);
    vector<int>isEnd;
    vector<int>indexes;
    vector<string>answer;
    vector<char>character;
    int num;
    int ** DFA= convertDFA(NFA, indexes, num, isEnd, stateNum);
    for(int i=0;i<indexes.size()-1;i++)character.push_back(mapping[indexes[i]]);
    string input="llld lld";
    int code=lexicalAnalysis(DFA, input, character, isEnd, answer);
    for(int i=0;i<answer.size();i++)cout<<answer[i]<<" ";
    //int ** simpleDFA=simplifyDFA(DFA, isEnd);
    return 0;
}
