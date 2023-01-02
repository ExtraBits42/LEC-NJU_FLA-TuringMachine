/**
 * @file turing.cpp
 * @author Yachen Zhong
 * @brief top-level program interface
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<stdio.h>
#include<iostream>
#include<regex>

#include"simulator.cpp"

using namespace std;

FILE* tm_open(char* filename){
    if(!regex_match(filename, regex(".*[.]tm"))) return nullptr;
    else return fopen(filename, "r");
}

/**
 * @brief top-level interface
 * 
 * @param argc terminal parameters number
 * @param argv terminal parameters array
 * @return ** int
 */
int main(int argc, char* argv[]){
    if(argc == 2 && regex_match(argv[1], regex("(-h)|(--help)"))){
        fprintf(stdout, "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
        return 0;
    }
    else if(argc == 3 || argc == 4){
        char* filename = argv[argc - 2];
        FILE* tm_file = tm_open(filename);
        if(tm_file != nullptr){
            fclose(tm_file);
            int if_v = regex_match(argv[1], regex("(-v)|(--verbose)"));
            string str(argv[argc - 1]);
            TM turing = TM(filename, if_v);
            // turing.display();
            TMSimulator tms = TMSimulator(&turing, if_v, str);
            tms.run();
            return 0;
        }
    }
    fprintf(stderr, "Illegal order, you can get more by using \"turing -h\"\n");
}
