/**
 * @file simulator.cpp
 * @author Yachen Zhong
 * @brief turing runtime simulating module
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<stdio.h>
#include<iostream>
#include<deque>
#include<unordered_set>

#include"parser.cpp"
using namespace std;

/**
 * @brief Simulator Class, simulating TM input and runtime
 * 
 */
class TMSimulator{
private:
    TM* tm;
    int if_v;
    string str;
    string cur_state;
    vector<vector<deque<char>>> tapes;
    vector<vector<deque<int>>> indexs;
public:
    /**
     * @brief TMSimulator's construction function
     * 
     */
    TMSimulator(TM* t, int v, string s){
        tm = t;
        if_v = v;
        str = s;
        cur_state = tm->start_states;
        for(int i = 0; i < tm->tape_num; i++){
            tapes.emplace_back(vector<deque<char>>(2, deque<char>()));
            indexs.emplace_back(vector<deque<int>>(2, deque<int>()));
            if(i == 0){
                if(int(s.size()) == 0){
                    indexs[i][1].emplace_back(0);
                    tapes[i][1].emplace_back('_');
                }
                else{
                    for(int j = int(s.size()) - 1; j >= 0; j--){
                        indexs[i][1].emplace_back(j);
                        tapes[i][1].emplace_back(s[j]);
                    }
                }
            }
            else{
                indexs[i][1].emplace_back(0);
                tapes[i][1].emplace_back(tm->blank_sym[0]);
            }
        }
    }
    /**
     * @brief TMSimulator top-level run function
     * 
     * @return ** void 
     */
    void run(){
        // check if input string is legal
        int i = 0;
        for(; i < int(str.size()); i++){
            if(tm->tape_syms.count(str.substr(i, 1)) == 0) break;
        }
        if(i != int(str.size())){
            if(if_v == 1){
                fprintf(stderr, "Input:\t%s\n", str.c_str());
                fprintf(stderr, "==================== ERR ====================\n");
                fprintf(stderr, "error:\t\'%c\' was not declared in the set of input symbols\n", str[i]);
                fprintf(stderr, "Input:\t%s\n", str.c_str());
                fprintf(stderr, "      \t");
                for(int j = 0; j < i; j++) fprintf(stderr, " ");
                fprintf(stderr, "^\n");
                fprintf(stderr, "==================== END ====================\n");
            }
            else{
                fprintf(stderr, "illegal input\n");
            }
            exit(-10);
        }
        // simulating TM's runtime
        int step = 0;
        if(if_v == 1){
            fprintf(stdout, "Input:\t%s\n", str.c_str());
            fprintf(stdout, "==================== RUN ====================\n");
            display_single_step(step++);
        }
        while(run_single_step()){
            if(if_v) display_single_step(step++);
        }
        string rst = "";
        for(auto it = tapes[0][0].begin(); it < tapes[0][0].end(); it++) rst.push_back(*it);
        for(auto it = tapes[0][1].rbegin(); it < tapes[0][1].rend(); it++) rst.push_back(*it);
        while(!rst.empty() && rst.back() == '_') rst.pop_back();
        if(if_v == 1){
            fprintf(stdout, "Result:\t%s\n", rst.c_str());
            fprintf(stdout, "==================== END ====================\n");
        }
        else fprintf(stdout, "%s\n", rst.c_str());
    }
    /**
     * @brief TMSimulator run, but single step
     * 
     * @return true  : switch to next state successfully
     * @return false : get input char, but have not responding transition functions
     */
    bool run_single_step(){
        string cur_chars = "";
        for(int i = 0; i < tm->tape_num; i++){
            cur_chars.push_back(tapes[i][1].back());
        }
        string old_chars = "";
        string new_chars = "";
        string new_direc = "";
        string new_state = "";
        int found = 0;
        for(auto vec : tm->tran_funcs[cur_state]){
            old_chars = vec[0];
            new_chars = vec[1];
            new_direc = vec[2];
            new_state = vec[3];
            int i = 0;
            for(; i < tm->tape_num; i++){
                if(old_chars[i] == '*' && cur_chars[i] != '_' || cur_chars[i] == old_chars[i]) continue;
                else break;
            }
            if(i == tm->tape_num){
                found = 1;
                break;
            }
        }
        if(found == 1){
            cur_state = new_state;
            for(int i = 0; i < tm->tape_num; i++){
                if(new_chars[i] == '*') continue;
                else tapes[i][1].back() = new_chars[i];
            }
            for(int i = 0; i < tm->tape_num; i++){
                if(new_direc[i] == 'l'){
                    if((tapes[i][0].size()) == 0){
                        tapes[i][1].emplace_back('_');
                        indexs[i][1].emplace_back(indexs[i][1].back() - 1);
                    }
                    else{
                        tapes[i][1].emplace_back(tapes[i][0].back());
                        indexs[i][1].emplace_back(indexs[i][0].back());
                        tapes[i][0].pop_back();
                        indexs[i][0].pop_back();
                    }
                }
                else if(new_direc[i] == 'r'){
                    if((tapes[i][1].size()) == 1){
                        tapes[i][1].emplace_front('_');
                        indexs[i][1].emplace_front(indexs[i][1].front() + 1);
                    }
                    tapes[i][0].emplace_back(tapes[i][1].back());
                    indexs[i][0].emplace_back(indexs[i][1].back());
                    tapes[i][1].pop_back();
                    indexs[i][1].pop_back();
                }
                else if(new_direc[i] == '*');
                while(!tapes[i][0].empty() && tapes[i][0].front() == '_'){
                    tapes[i][0].pop_front();
                    indexs[i][0].pop_front();
                }
                while(int(tapes[i][1].size()) > 1 && tapes[i][1].front() == '_'){
                    tapes[i][1].pop_front();
                    indexs[i][1].pop_front();
                }
            }
        }
        else return false;
        return true;
    }
    /**
     * @brief Show you current each tapes and locations of read-write headers
     * 
     * @param step_index : steps number
     * @return ** void 
     */
    void display_single_step(int step_index) const{
        fprintf(stdout, "Step\t: %d\n", step_index);
        fprintf(stdout, "State\t: %s\n", cur_state.c_str());
        for(int i = 0; i < tm->tape_num; i++){
            fprintf(stdout, "Index%d\t: ", i);
            for(auto it = indexs[i][0].begin(); it < indexs[i][0].end(); it++){
                fprintf(stdout, "%d ", *it);
            }
            for(auto it = indexs[i][1].rbegin(); it < indexs[i][1].rend(); it++){
                fprintf(stdout, "%d ", *it);
            }
            fprintf(stdout, "\n");

            fprintf(stdout, "Tape%d\t: ", i);
            {
                auto it2 = indexs[i][0].begin();
                for(auto it1 = tapes[i][0].begin(); it1 < tapes[i][0].end() && it2 < indexs[i][0].end(); it1++, it2++){
                    fprintf(stdout, "%c", *it1);
                    fprintf(stdout, "%s", string(int(to_string(*it2).size()), ' ').c_str());
                }
            }
            {
                auto it2 = indexs[i][1].rbegin();
                for(auto it1 = tapes[i][1].rbegin(); it1 < tapes[i][1].rend() && it2 < indexs[i][1].rend(); it1++, it2++){
                    fprintf(stdout, "%c", *it1);
                    fprintf(stdout, "%s", string(int(to_string(*it2).size()), ' ').c_str());
                }
            }
            fprintf(stdout, "\n");

            fprintf(stdout, "Head%d\t: ", i);
            {
                auto it2 = indexs[i][0].begin();
                for(int it1 = 0; it1 < int(tapes[i][0].size()) && it2 < indexs[i][0].end(); it1++, it2++){
                    fprintf(stdout, "%s ", string(int(to_string(*it2).size()), ' ').c_str());
                }
            }
            fprintf(stdout, "^\n");
        }
        fprintf(stdout, "---------------------------------------------\n");
    }
};
