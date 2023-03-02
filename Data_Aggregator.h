//
// Created by shvaikop on 6.12.22.
//

#ifndef RECODEX_HW_1_DATA_AGGREGATOR_H
#define RECODEX_HW_1_DATA_AGGREGATOR_H

#endif //RECODEX_HW_1_DATA_AGGREGATOR_H

#include "Data.h"

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef MEMORY
#define MEMORY
#include <memory>
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

#ifndef SET
#define SET
#include <set>
#endif

using namespace std;

constexpr int int_val = 1;
constexpr double double_val = 0.1;
const string string_val = "";

class Data_Aggregator{
    vector<string> column_names;
    map<string, size_t> name_col;   // maps columns names to their indices in the input
    vector<Type> column_types;
    size_t num_rows;                // the number of rows of data
    string Group_By = "";           // Group_By column name
    vector<pair<string, string>> Selects;   // select queries

    Data data;

    // splits a string based on a fixed delimiter character
    static vector<string> parse_line(string &s, const char delim);

    // method do extract the column name from Select query
    static string column_from_operation(string s);

    static string Sum(vector<size_t> &vect, Data &data_local, size_t col_i, Type t);

    static string Min(vector<size_t> &vect, Data &data_local, size_t col_i, Type t);

    static string Max(vector<size_t> &vect, Data &data_local, size_t col_i, Type t);

    string eval(vector<size_t> &next_rows, pair<string, string> select);

    // Returns the indices of the next distict Group_By element
    static vector<size_t> get_next_rows(set<string> &seen, Data &data_local, size_t Group_by_i);

    static vector<size_t> get_next_rows(set<int> &seen, Data &data_local, size_t Group_by_i);

    static vector<size_t> get_next_rows(set<double> &seen, Data &data_local, size_t Group_by_i);

    void print_output_header();

public:
    void process_header();

    void process_data();

    void process_query();

    void produce_output();
};
