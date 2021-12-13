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
    TransFunc(string C,string H,string N,string D,string N){
        cur_state=C;
        head=H;
        new_char=N;
        direction=D;
        next_state=N;
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

int TM_solve(string raw_tm,string raw_input,bool flag){
    //TODO
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
    string raw_tm=result[2];
    string raw_input=result[3];
    int res=TM_solve(raw_tm,raw_input,false);
    return res;
}

int main(int argc,char *argv[]){
    vector<string> result;
    for (int i=1;i<argc;i++){
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