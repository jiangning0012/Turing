#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

class TransFunc{
private:
    string cur_state;
    string head;
    string new_char;
    string direction;
    string next_state;
public:
    TransFunc(string C,string H,string N,string D,string NEXT){
        cur_state=C;
        head=H;
        new_char=N;
        direction=D;
        next_state=NEXT;
    }
    void _print(){
        cout<<cur_state<<" "<<head<<" "<<new_char<<" "<<direction<<" "<<next_state<<endl;
    }
};

class TM{
private:
    vector<string> Q;
    vector<char> S;
    vector<char> G;
    string q0;
    char B;
    vector<string> F;
    int N;
    vector<TransFunc> trans_funcs;
    bool verbose_flag;
    vector<string> tapes;
    vector<int> head_pos;
    string cur_state;
    vector<int> leftmost_tag;
public:
    TM(vector<string> _Q,vector<char> _S,vector<char> _G,string _q0,char _B,vector<string> _F,
    int _N,vector<TransFunc> _trans_funcs,bool _verbose_flag){
        Q=_Q;S=_S;G=_G;q0=_q0;B=_B;F=_F;N=_N;trans_funcs=_trans_funcs;verbose_flag=_verbose_flag;
    }
    void print_info(){
        cout<<"Q={";
        for (int i=0;i<Q.size()-1;i++) cout<<Q[i]<<","; 
        cout<<Q[Q.size()-1]<<"}\n";
        cout<<"S={";
        for (int i=0;i<S.size()-1;i++) cout<<S[i]<<","; 
        cout<<S[S.size()-1]<<"}\n";
        cout<<"G={";
        for (int i=0;i<G.size()-1;i++) cout<<G[i]<<","; 
        cout<<G[G.size()-1]<<"}\n";
        cout<<"q0="<<q0<<endl;
        cout<<"B="<<B<<endl;
        cout<<"F={";
        for (int i=0;i<F.size()-1;i++) cout<<F[i]<<","; 
        cout<<F[F.size()-1]<<"}\n";
        cout<<"N="<<N<<endl;
        for (int i=0;i<trans_funcs.size();i++){
            trans_funcs[i]._print();
        }
    }
    int check_input(string raw_input){
        for (int i=0;i<raw_input.size();i++){
            int flag=0;
            for (int j=0;j<S.size();j++)
                if (raw_input[i]==S[j]) {flag=1;break;}
            if (!flag){
                cerr<<"Illegal input\n";
                return i;
            }
        }
        return -1;
    }
    void verbose(int step){
        cout<<"Step   : "<<step<<endl;
        for (int i=0;i<N;i++){
            cout<<"Index"<<i<<" : ";
            vector<int> _space;
            for (int j=0;j<tapes[i].size();j++){
                int pos=leftmost_tag[i]+j;
                if (pos<10){
                    _space.push_back(1);
                }
                else if (pos<100) _space.push_back(2);
                cout<<pos<<" ";
            }
            cout<<endl;
            cout<<"Tape"<<i<<"  : ";
            for (int j=0;j<tapes[i].size();j++){
                cout<<tapes[i][j];
                for (int k=0;k<_space[j];k++) cout<<" ";
            }
            cout<<endl;
            cout<<"Head"<<i<<"  : ";
            for (int j=0;j<tapes[i].size();j++){
                if (j==head_pos[i]){
                    cout<<"^";
                }
                else cout<<" ";
                for (int k=0;k<_space[j];k++) cout<<" ";
            }
            cout<<endl;
        }
        cout<<"State  : "<<cur_state<<endl;
        cout<<"---------------------------------------------"<<endl;
    }
    
};
//去掉花括号
string deal_brackets(string str){
    int left=-1,right=-1;
    for (int i=0;i<str.size();i++){
        if (str[i]=='{') left=i;
        if (str[i]=='}') right=i;
    }
    string res=str.substr(left+1,right-left-1);
    return res;
} 
//提取集合的元素
vector<string> split(string str,string _delimiter){
    vector<string> res;
    if (str=="") return res;
    char *s=new char[str.length()+1];
    char *d=new char[_delimiter.length()+1];
    strcpy(s,str.c_str());strcpy(d,_delimiter.c_str());
    char *p=strtok(s,d);
    while (p!=NULL){
        string ans=p;
        res.push_back(ans);
        p=strtok(NULL,d);
    }
    return res;
}

TM TM_parser(vector<string> content,bool verbose_flag){
    vector<string> Q;
    vector<char> S;
    vector<char> G;
    string q0;
    char B;
    vector<string> F;
    int N;
    vector<TransFunc> trans_funcs;
    //格式合法性
    for (int i=0;i<content.size();i++){
        //去掉注释
        string temp=content[i];
        content[i]="";
        for (int j=0;j<temp.size();j++){
            if (temp[j]==';') break;
            content[i].push_back(temp[j]);
        }
        //empty line
        if (content[i].size()==0) continue;
        //notes line
        if (content[i][0]==';') continue;
        int end_pos;
        for (int j=content[i].size()-1;j>=0;j--) 
            if (content[i][j]!=' '){end_pos=j;break;}
        temp=content[i];
        content[i]="";
        for (int j=0;j<=end_pos;j++){
            content[i].push_back(temp[j]);
        }
        //#B = _
        if (content[i].size()<6){
            cerr<<"syntax error\n";
            exit(-1);
        }
        if (content[i].substr(0,5)=="#Q = "){
            string res=deal_brackets(content[i]);
            Q=split(res,",");
            if (Q.size()==0){
                cerr<<"syntax error\n";
                exit(-1);
            }
        }
        else if (content[i].substr(0,5)=="#S = "){
            string res=deal_brackets(content[i]);
            vector<string> temp=split(res,",");
            if (temp.size()==0){
                cerr<<"syntax error\n";
                exit(-1);
            }
            for (int j=0;j<temp.size();j++){
                if (temp[j].size()!=1){
                    cerr<<"syntax error\n";
                    exit(-1);
                }
                S.push_back(temp[j][0]);
            }
        }
        else if (content[i].substr(0,5)=="#G = "){
            string res=deal_brackets(content[i]);
            vector<string> temp=split(res,",");
            if (temp.size()==0){
                cerr<<"syntax error\n";
                exit(-1);
            }
            for (int j=0;j<temp.size();j++){
                if (temp[j].size()!=1){
                    cerr<<"syntax error\n";
                    exit(-1);
                }
                G.push_back(temp[j][0]);
            }
        }
        else if (content[i].substr(0,6)=="#q0 = "){
            if (content[i].length()==6){
                cerr<<"syntax error\n";
                exit(-1);
            }
            q0=content[i].substr(6,content[i].length()-6);
        }
        else if (content[i].substr(0,5)=="#B = "){
            if (content[i].size()!=6){
                cerr<<"syntax error\n";
                exit(-1);
            }
            char temp=content[i][5];
            if (temp!='_'){
                cerr<<"syntax error\n";
                exit(-1);
            }
            B=temp;
        }
        else if (content[i].substr(0,5)=="#F = "){
            string res=deal_brackets(content[i]);
            F=split(res,",");
            if (F.size()==0){
                cerr<<"syntax error\n";
                exit(-1);
            }
        }
        else if (content[i].substr(0,5)=="#N = "){
            string Num=content[i].substr(5,content[i].size()-5);
            if (Num.size()==0){
                cerr<<"syntax error\n";
                exit(-1);
            }
            for (int j=0;j<Num.size();j++){
                if (!(Num[j]>='0'&&Num[j]<='9')){
                    cerr<<"syntax error\n";
                    exit(-1);
                }
            }
            N=stoi(Num);
        }
        else{
            vector<string> func=split(content[i]," ");
            if (func.size()!=5){
                cerr<<"syntax error\n";
                exit(-1);
            }
            string cur_state=func[0];
            string head=func[1];
            string new_char=func[2];
            string direction=func[3];
            string next_state=func[4];
            trans_funcs.push_back(TransFunc(cur_state,head,new_char,direction,next_state));
        }
    }
    //内容合法性
    for (int i=0;i<Q.size();i++){
        for (int j=0;j<Q[i].size();j++){
            if (!((Q[i][j]>='a'&&Q[i][j]<='z')||(Q[i][j]>='A'&&Q[i][j]<='Z')||(Q[i][j]>='0'&&Q[i][j]<='9')||Q[i][j]=='_')){
                cerr<<"syntax error\n";
                exit(-1);
            }
        }
    }
    for (int i=0;i<S.size();i++){
        if (S[i]==' '||S[i]==','||S[i]==';'||S[i]=='{'||S[i]=='}'||S[i]=='*'||S[i]=='_'){
            cerr<<"syntax error\n";
            exit(-1);
        }
    }
    for (int i=0;i<G.size();i++){
        if (G[i]==' '||G[i]==','||G[i]==';'||G[i]=='{'||G[i]=='}'||G[i]=='*'){
            cerr<<"syntax error\n";
            exit(-1);
        }
    }
    //q0和F是否在状态集Q中
    
    //纸带数大于0
    if (N<=0){
        cerr<<"syntax error\n";
        exit(-1);
    }
    //转移函数语法错误

    TM turing_machine=TM(Q,S,G,q0,B,F,N,trans_funcs,verbose_flag);
    return turing_machine;
}

TM get_TM(string raw_tm,bool verbose_flag){
    vector<string> content;
    ifstream in(raw_tm);
    string line;
    if (in){
        while (getline(in,line)){
            content.push_back(line);
        }
    }
    else {
        cerr<<"syntax error\n";
        exit(-1);
    }
    return TM_parser(content,verbose_flag);
}

int TM_solve(string raw_tm,string raw_input,bool flag){
    //flag:verbose or not
    TM solver=get_TM(raw_tm,flag);
    //int res=solver.solve(raw_input);
    //return res;
    solver.print_info();
    return 0;
}

int command_check_error(vector<string> result){
    if (result.size()<2){
        cerr<<"syntax error\n";
        exit(-1);
    }
    if (result[0]!="turing"&&result[0]!="./turing"){
        cerr<<"syntax error\n";
        exit(-1);
    }
    if (result[1]=="-h"||result[1]=="--help"){
        cout<<"usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
        return 1;
    }
    if (result[1]=="-v"||result[1]=="--verbose"){
        if (result.size()!=4){
            cerr<<"syntax error\n";
            exit(-1);
        }
        string raw_tm=result[2];
        string raw_input=result[3];
        int res=TM_solve(raw_tm,raw_input,true);
        return res;
    }
    if (result.size()!=3){
        cerr<<"syntax error\n";
        exit(-1);
    }
    string raw_tm=result[1];
    string raw_input=result[2];
    int res=TM_solve(raw_tm,raw_input,false);
    return res;
}

int main(int argc,char *argv[]){
    vector<string> result;
    for (int i=0;i<argc;i++){
        string temp=argv[i];
        result.push_back(temp);
    }
    // string command;
    // for (int i=0;i<result.size();i++) command=command+result[i]+" ";
    // cout<<command<<endl;
    int res=command_check_error(result);
    if (res!=1) return -1;
    else return 1;
    return 0;
}