//
// Created by shvaikop on 6.12.22.
//

#include "Data_Aggregator.h"

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

vector<string> Data_Aggregator::parse_line(string &s, const char delim) {
    vector<string> result;
    size_t start_pos = 0;   // index of the start of the current substr
    size_t count = 0;       // length of the current subst

    for (size_t i = 0; i < s.size(); i++){
        if (s[i] == delim){
            result.emplace_back(s.substr(start_pos, count));
            start_pos = i + 1;
            count = 0;
        }
        else if (i == s.size() - 1){    // end of s
            result.emplace_back(s.substr(start_pos, count + 1));
        }
        else{
            count++;
        }
    }
    return result;
}

void Data_Aggregator::process_header(){     // processes first 3 rows
    string line;
    cin >> line;
    column_names = move(parse_line(line, ';'));
    for (size_t i = 0; i < column_names.size(); i++){
        name_col[column_names[i]] = i;
    }

    cin >> line;
    for (string t : parse_line(line, ';')){
        if (t == "string"){
            column_types.push_back(String);
        }
        else if (t == "int"){
            column_types.push_back(Int);
        }
        else if (t == "double"){
            column_types.push_back(Double);
        }
        else{
            throw runtime_error("Unknown type encountered while processing second row!");
        }
    }

    data.set_global_local(column_types);
    // Error checking
    if (column_names.size() != column_types.size()){
        throw runtime_error("Number of columns in header does not match");
    }
    cin >> num_rows;
}

void Data_Aggregator::process_data() {
    for (size_t i = 0; i < num_rows; i++){
        data.new_row();
        string temp_s;
        cin >> temp_s;
        auto split_line = parse_line(temp_s, ';');
        // Error checking
        if (split_line.size() != column_names.size()){
            throw runtime_error("Number of columns in data does not match!");
        }

        for (size_t col = 0; col < split_line.size(); col++){
            data.set(split_line[col], column_types[col]);
        }
    }
}

// Given a single Select quesry returns the column it is performed on
string Data_Aggregator::column_from_operation(string s) {
    string res = "";
    for (size_t i = 4; i < s.size(); i++){
        if (s[i] == ')'){
            break;
        }
        res += s[i];
    }
    return res;
}

void Data_Aggregator::process_query() {
    string temp_s = "";
    vector<string> split_line;
    while (temp_s != "SELECT"){     // Ignore input untill "SELECT" is recieved
        cin >> temp_s;
    }
    while (temp_s != "GROUP_BY"){   // Store the select queries
        cin >> temp_s;
        if (temp_s[temp_s.size() - 1] == ','){ // remove the comma at the end
            temp_s.pop_back();
        }
        split_line.push_back(temp_s);
    }
    cin >> Group_By;    // Store the Group_By column name
    for (size_t i = 0; i < split_line.size() - 1; i++){     // Process the select queries
        if (split_line[i].size() < 4){  // cannot possibly be a function
            Selects.emplace_back("", split_line[i]);
        }
        else if (split_line[i].substr(0, 4) == "SUM("){
            Selects.emplace_back("SUM", column_from_operation(split_line[i]));
        }
        else if (split_line[i].substr(0, 4) == "MIN("){
            Selects.emplace_back("MIN", column_from_operation(split_line[i]));
        }
        else if (split_line[i].substr(0, 4) == "MAX("){
            Selects.emplace_back("MAX", column_from_operation(split_line[i]));
        }
        else{
            Selects.emplace_back("", split_line[i]);
        }
    }
}

string Data_Aggregator::Sum(vector<size_t> &vect, Data &data_local, size_t col_i, Type t) {
    if (t == String){
        string res = "";
        for (size_t i = 0; i < vect.size(); i++){
            res += data_local.get(vect[i], col_i, string_val);
        }
        return res;
    }
    if (t == Int){
        int res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            res += data_local.get(vect[i], col_i, int_val);
        }
        return to_string(res);
    }
    if (t == Double){
        double res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            res += data_local.get(vect[i], col_i, double_val);
        }
        return to_string(res);
    }
    throw runtime_error("Something wrong in Sum");
}

string Data_Aggregator::Min(vector<size_t> &vect, Data &data_local, size_t col_i, Type t) {
    if (t == String){
        string res = "";
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, string_val);
            }
            else{
                string new_elem = data_local.get(vect[i], col_i, string_val);
                res = res < new_elem ? res : new_elem;
            }
        }
        return res;
    }
    if (t == Int){
        int res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, int_val);
            }
            else{
                int new_elem = data_local.get(vect[i], col_i, int_val);
                res = res < new_elem ? res : new_elem;
            }
        }
        return to_string(res);
    }
    if (t == Double){
        double res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, double_val);
            }
            else{
                double new_elem = data_local.get(vect[i], col_i, double_val);
                res = res < new_elem ? res : new_elem;
            }
        }
        return to_string(res);
    }
    throw runtime_error("Something wrong in Max");
}

string Data_Aggregator::Max(vector<size_t> &vect, Data &data_local, size_t col_i, Type t) {
    if (t == String){
        string res = "";
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, string_val);
            }
            else{
                string new_elem = data_local.get(vect[i], col_i, string_val);
                res = res > new_elem ? res : new_elem;
            }
        }
        return res;
    }
    if (t == Int){
        int res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, int_val);
            }
            else{
                int new_elem = data_local.get(vect[i], col_i, int_val);
                res = res > new_elem ? res : new_elem;
            }
        }
        return to_string(res);
    }
    if (t == Double){
        double res = 0;
        for (size_t i = 0; i < vect.size(); i++){
            if (i == 0) {
                res = data_local.get(vect[i], col_i, double_val);
            }
            else{
                double new_elem = data_local.get(vect[i], col_i, double_val);
                res = res > new_elem ? res : new_elem;
            }
        }
        return to_string(res);
    }
    throw runtime_error("Something wrong in Max");
}

// evaluates a single entry of the output
string Data_Aggregator::eval(vector<size_t> &next_rows, pair<string, string> select) {
    size_t col_i = name_col[select.second];
    Type t = column_types[col_i];
    if (select.first == ""){
        if (t == String){
            return data.get(next_rows[0], name_col[select.second], string_val);
        }
        else if (t == Int){
            return to_string(data.get(next_rows[0], name_col[select.second], int_val));
        }
        else if (t == Double){
            return to_string(data.get(next_rows[0], name_col[select.second], double_val));
        }
    }
    else if (select.first == "SUM"){
        return Sum(next_rows, data, col_i, t);
    }
    else if(select.first == "MIN"){
        return Min(next_rows, data, col_i, t);
    }
    else if(select.first == "MAX"){
        return Max(next_rows, data, col_i, t);
    }
    throw runtime_error("Something wrong in eval!");
}

vector<size_t> Data_Aggregator::get_next_rows(set<string> &seen, Data &data_local, size_t Group_by_i) {
    vector<size_t> to_return;
    size_t i = 0;
    while (seen.count(data_local.get(i, Group_by_i, string_val)) != 0){   // reach an unseed group element
        i++;
        if (i >= data_local.num_rows()){     // end of data reached
            return to_return;
        }
    }
    string elem = data_local.get(i, Group_by_i, string_val);
    seen.insert(elem);
    for (size_t j = i; j < data_local.num_rows(); j++){
        if (data_local.get(j, Group_by_i, string_val) == elem){
            to_return.push_back(j);
        }
    }
    return to_return;
}

vector<size_t> Data_Aggregator::get_next_rows(set<int> &seen, Data &data_local, size_t Group_by_i) {
    vector<size_t> to_return;
    size_t i = 0;
    while (seen.count(data_local.get(i, Group_by_i, int_val)) != 0){   // reach an unseed group element
        i++;
        if (i >= data_local.num_rows()){     // end of data reached
            return to_return;
        }
    }
    int elem = data_local.get(i, Group_by_i, int_val);
    seen.insert(elem);
    for (size_t j = i; j < data_local.num_rows(); j++){
        if (data_local.get(j, Group_by_i, int_val) == elem){
            to_return.push_back(j);
        }
    }
    return to_return;
}

vector<size_t> Data_Aggregator::get_next_rows(set<double> &seen, Data &data_local, size_t Group_by_i) {
    vector<size_t> to_return;
    size_t i = 0;
    while (seen.count(data_local.get(i, Group_by_i, double_val)) != 0){   // reach an unseed group element
        i++;
        if (i >= data_local.num_rows()){     // end of data reached
            return to_return;
        }
    }
    double elem = data_local.get(i, Group_by_i, double_val);
    seen.insert(elem);
    for (size_t j = i; j < data_local.num_rows(); j++){
        if (data_local.get(j, Group_by_i, double_val) == elem){
            to_return.push_back(j);
        }
    }
    return to_return;
}

void Data_Aggregator::print_output_header() {
    for(size_t i = 0; i < Selects.size(); i++){
        if (Selects[i].first == ""){
            if (Selects[i].second[Selects[i].second.size() - 1] == ','){
                Selects[i].second.pop_back();
            }
            cout << Selects[i].second;
        }
        else{
            cout << Selects[i].first << "(" << Selects[i].second << ")";
        }
        if (i != Selects.size() - 1){
            cout << ";";
        }
    }
    cout << endl;
}

void Data_Aggregator::produce_output() {
    print_output_header();
    size_t group_col_i = name_col[Group_By];    // index of Group_By column
    if (column_types[group_col_i] == String){
        set<string> seen_elems;                 // Group_By elements already processed
        vector<size_t> next_rows;               // indeces of rows that contain the next unprocessed Group_By elem
        next_rows = get_next_rows(seen_elems, data, name_col[Group_By]);
        while (!next_rows.empty()){     // while there are still unprocessed Group_By elements
            for (size_t c = 0; c < Selects.size(); c++){    // go through all the select queries
                cout << eval(next_rows, Selects[c]);
                if (c != Selects.size() - 1){   // if not the end of an output row
                    cout << ";";
                }
            }
            cout << endl;
            next_rows = get_next_rows(seen_elems, data, name_col[Group_By]); // get rows with the next Group_By element
        }
    }

    else if (column_types[group_col_i] == Int){
        set<int> seen_elems;
        vector<size_t> next_rows;
        next_rows = get_next_rows(seen_elems, data, name_col[Group_By]);
        while (!next_rows.empty()){
            for (size_t c = 0; c < Selects.size(); c++){
                cout << eval(next_rows, Selects[c]);
                if (c != Selects.size() - 1){
                    cout << ";";
                }
            }
            cout << endl;
            next_rows = get_next_rows(seen_elems, data, name_col[Group_By]);
        }
    }

    else if (column_types[group_col_i] == Double){
        set<double> seen_elems;
        vector<size_t> next_rows;
        next_rows = get_next_rows(seen_elems, data, name_col[Group_By]);
        while (!next_rows.empty()){
            for (size_t c = 0; c < Selects.size(); c++){
                cout << eval(next_rows, Selects[c]);
                if (c != Selects.size() - 1){
                    cout << ";";
                }
            }
            cout << endl;
            next_rows = get_next_rows(seen_elems, data, name_col[Group_By]);
        }
    }
    else{
        throw runtime_error("unknown column type in produce output!");
    }
}
