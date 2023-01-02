/**
 * @file parser.cpp
 * @author Yachen Zhong
 * @brief turing programs parsing module
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<regex>
using namespace std;

/**
 * @brief Turing Machine Class
 * TMSimulator's friend class
 */
class TM{
private:
    unordered_set<string> states;
    unordered_set<string> input_syms;
    unordered_set<string> tape_syms;
    string start_states;
    string blank_sym;
    unordered_set<string> end_states;
    int tape_num;
    unordered_map<string, vector<vector<string>>> tran_funcs;
    friend class TMSimulator;
public:
    /**
     * @brief Turing Machine Construction Function, build TM by reading .tm file
     * 
     */
    TM(char* filename, int if_v){

        regex pattern_q("#Q[ ]=[ ][{]([a-zA-Z0-9_]+)((,[a-zA-Z0-9_]+)*)[}]");
        regex pattern_s("#S[ ]=[ ][{]([a-zA-z0-9'!\"#$%&()./:?@^`\[\\]|~+<=>\\-])((,[a-zA-z0-9'!\"#$%&()./:?@^`\[\\]|~+<=>\\-])*)[}]");
        regex pattern_g("#G[ ]=[ ][{]([a-zA-z0-9_'!\"#$%&()./:?@^`\[\\]|~+<=>\\-])((,[a-zA-z0-9_'!\"#$%&()./:?@^`\[\\]|~+<=>\\-])*)[}]");
        regex pattern_0("#q0[ ]=[ ]([a-zA-Z0-9_]+)");
        regex pattern_b("#B[ ]=[ ][_]");
        regex pattern_f("#F[ ]=[ ][{]([a-zA-Z0-9_]+)((,[a-zA-Z0-9_]+)*)[}]");
        regex pattern_n("#N[ ]=[ ]([1-9][0-9]*)");
        regex pattern_d("([a-zA-Z0-9_]+)[ ]([a-zA-z0-9_'!\"#$%&()./:?@^`\[\\]|~+<=>\\-*]+)[ ]([a-zA-z0-9_'!\"#$%&()./:?@^`\[\\]|~+<=>\\-*]+)[ ]([lr*]+)[ ]([a-zA-Z0-9_]+)");
        regex split1(",");
        regex split2("[ ]");

        ifstream fin(filename);
        string strline;
        int index = 0; // limit each sets definition order
        while(getline(fin, strline)){
            if((int(strline.size()) == 0) || strline[0] == ';') continue;
            else if(strline.find(";") != string::npos){
                int i = 0;
                for(; i < int(strline.size()); i++){
                    if(strline[i] == ';') break;
                }
                i--;
                while(i >= 0 && strline[i] == ' ') i--;
                strline = strline.substr(0, i + 1);
                if(int(strline.size()) == 0) continue;
            }

            if(index == 0 && strline.substr(0, 2) == "#Q"){
                index++;
                if(regex_match(strline, pattern_q) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-1);
                }
                else{
                    sregex_token_iterator pos(strline.begin()+6, strline.end()-1, split1, -1);
                    decltype(pos) end;
                    for(; pos != end; pos++){
                        states.emplace(pos->str());
                    }
                }
            }
            else if(index == 1 && strline.substr(0, 2) == "#S"){
                index++;
                if(regex_match(strline, pattern_s) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-2);
                }
                else{
                    sregex_token_iterator pos(strline.begin()+6, strline.end()-1, split1, -1);
                    decltype(pos) end;
                    for(; pos != end; pos++){
                        input_syms.emplace(pos->str());
                    }
                }
            }
            else if(index == 2 && strline.substr(0, 2) == "#G"){
                index++;
                if(regex_match(strline, pattern_g) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-3);
                }
                else{
                    sregex_token_iterator pos(strline.begin()+6, strline.end()-1, split1, -1);
                    decltype(pos) end;
                    for(; pos != end; pos++){
                        tape_syms.emplace(pos->str());
                    }
                }
            }
            else if(index == 3 && strline.substr(0, 3) == "#q0"){
                index++;
                if(regex_match(strline, pattern_0) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-4);
                }
                else{
                    start_states = strline.substr(6);
                }
            }
            else if(index == 4 && strline.substr(0, 2) == "#B"){
                index++;
                if(regex_match(strline, pattern_b) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-5);
                }
                else{
                    blank_sym = strline.substr(5);
                }
            }
            else if(index == 5 && strline.substr(0, 2) == "#F"){
                index++;
                if(regex_match(strline, pattern_f) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-6);
                }
                else{
                    sregex_token_iterator pos(strline.begin()+6, strline.end()-1, split1, -1);
                    decltype(pos) end;
                    for(; pos != end; pos++){
                        end_states.emplace(pos->str());
                    }
                }
            }
            else if(index == 6 && strline.substr(0, 2) == "#N"){
                index++;
                if(regex_match(strline, pattern_n) == false){
                    fprintf(stderr, "syntax error\n");
                    exit(-7);
                }
                else{
                    tape_num = atoi(strline.substr(5).c_str());
                }
            }
            else if(index == 7){
                if(regex_match(strline, pattern_d) == false){
                    fprintf(stderr, "syntax error\n");
                    fprintf(stderr, "\t%s\n", strline.c_str());
                    exit(-8);
                }
                else{
                    vector<string> temp;
                    sregex_token_iterator pos(strline.begin(), strline.end(), split2, -1);
                    decltype(pos) end;
                    int i = 0;
                    string key = "";
                    for(; pos != end; pos++){
                        if(i == 0) key = pos->str();
                        else temp.emplace_back(pos->str());
                        i++;
                    }
                    bool checker = true;
                    if(int(temp[0].size()) != tape_num || int(temp[1].size()) != tape_num || int(temp[2].size()) != tape_num) checker = false;
                    for(int i = 0; i < tape_num; i++){
                        if(temp[0][i] != '*' && temp[1][i] == '*'){
                            checker = false;
                            break;
                        }
                    }
                    if(states.count(key) == 0 || states.count(temp[3]) == 0) checker = false;
                    if(checker == false){
                        fprintf(stderr, "syntax error\n");
                        exit(-8);
                    }
                    tran_funcs[key].emplace_back(temp);
                }
            }
            else{
                fprintf(stderr, "syntax error\n");
                exit(-9);
            }
        }
        fin.close();
    }
    /**
     * @brief Show you detailed information related to Turing Machine obj
     * 
     * @return ** void 
     */
    void display() const{
        fprintf(stdout, "################### Now, show you TM  #####################\n");
        fprintf(stdout, "States Set:\n");
        for(auto it : states){
            fprintf(stdout, "\t%s\n", it.c_str());
        }
        fprintf(stdout, "Input Symbols Set:\n");
        for(auto it : input_syms){
            fprintf(stdout, "\t%s\n", it.c_str());
        }
        fprintf(stdout, "Tape Symbols Set:\n");
        for(auto it : tape_syms){
            fprintf(stdout, "\t%s\n", it.c_str());
        }
        fprintf(stdout, "Initial Symbol:\n");
        fprintf(stdout, "\t%s\n", start_states.c_str());
        fprintf(stdout, "Blank Symbol:\n");
        fprintf(stdout, "\t%s\n", blank_sym.c_str());
        fprintf(stdout, "Final Symbols:\n");
        for(auto it : end_states){
            fprintf(stdout, "\t%s\n", it.c_str());
        }
        fprintf(stdout, "Tape Number:\n");
        fprintf(stdout, "\t%d\n", tape_num);
        fprintf(stdout, "Delta Functions:\n");
        for(auto it : tran_funcs){
            fprintf(stdout, "\t%s:\n", it.first.c_str());
            for(auto i : it.second){
                fprintf(stdout, "\t\t%s %s %s -> %s\n", i[0].c_str(), i[1].c_str(), i[2].c_str(), i[3].c_str());
            }
        }
        fprintf(stdout, "##################################################################\n");
    }
};


