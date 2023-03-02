//
// Created by shvaikop on 5.12.22.
//
#include "Data.h"

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef MAP
#define MAP
#include <map>
#endif

using namespace std;

// Prepare another row
void Data::new_row() {
    size_t cur_size = data_int.size();
    data_int.resize(cur_size + 1);
    data_double.resize(cur_size + 1);
    data_string.resize(cur_size + 1);
    cur_row = cur_size;
}

// defines a mapping from the index of column in the input to the index of this column
// in one of the above 2d vectors
void Data::set_global_local(vector<Type> &types_col) {
    size_t int_index = 0;                                 // CHANGE ???
    size_t double_index = 0;
    size_t string_index = 0;
    for (size_t i = 0; i < types_col.size(); i++){
        if (types_col[i] == Int){
            col_global_local[i] = int_index++;
        }
        else if (types_col[i] == Double){
            col_global_local[i] = double_index++;
        }
        else if (types_col[i] == String){
            col_global_local[i] = string_index++;
        }
        else{
            throw runtime_error("Unknown column type while processing header!");
        }
    }
}

int Data::get(size_t r, size_t c, int identifier) {
    if (r >= data_int.size() || col_global_local[c] >= data_int[r].size()){
        throw runtime_error("Data.get(int) index out of range!");
    }
    return data_int[r][col_global_local[c]];
}

double Data::get(size_t r, size_t c, double identifier) {
    if (r >= data_double.size() || col_global_local[c] >= data_double[r].size()){
        throw runtime_error("Data.get(int) index out of range!");
    }
    return data_double[r][col_global_local[c]];
}

string Data::get(size_t r, size_t c, string identifier) {
    if (r >= data_string.size() || col_global_local[c] >= data_string[r].size()){
        throw runtime_error("Data.get(int) index out of range!");
    }
    return data_string[r][col_global_local[c]];
}

void Data::set(string &elem, Type t) {
    if (cur_row >= data_int.size() || cur_row >= data_double.size() || cur_row >= data_string.size()){
        throw runtime_error("Something wrong with rows in Data!");
    }
    if (t == Int){
        data_int[cur_row].push_back(stoi(elem));
    }
    else if (t == Double){
        data_double[cur_row].push_back(stod(elem));
    }
    else if (t == String){
        data_string[cur_row].emplace_back(elem);
    }
    else {
        throw runtime_error("Wrong type in Data.set !!!");
    }
}

size_t Data::num_rows() {
    if (data_string.size() != data_int.size() || data_int.size() != data_double.size()){
        throw runtime_error("Something wrong in internal size of data!");
    }
    return data_string.size();
}
