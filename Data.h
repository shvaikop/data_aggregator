#ifndef RECODEX_HW_1_DATA_H
#define RECODEX_HW_1_DATA_H

#endif //RECODEX_HW_1_DATA_H

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef MAP
#define MAP
#include <map>
#endif

#include "Type.h"

using namespace std;


class Data{
    constexpr static int empty = -1;
    // 3 2d vectors to store input of each data type
    vector<vector<string>> data_string;
    vector<vector<int>> data_int;
    vector<vector<double>> data_double;

    // defines a mapping from the index of column in the input to the index of this column
    // in one of the above 2d vectors
    map<size_t, size_t> col_global_local;
    size_t cur_row = empty;
public:
    void set_global_local(vector<Type> &types_col);

    int get(size_t r, size_t c, int identifier);    // getters, for each data type
    double get(size_t r, size_t c, double identifier);
    string get(size_t r, size_t c, string identifier);

    void set(string &elem, Type t);

    // Prepare another row
    void new_row();

    size_t num_rows();
};