


Input:
- The first row is the names of the columns
- The second row specifies the data type of each column (int, double, string)
- The third row tells the number of rows in the data
- The last row is the query
- The other rows are the data

```<col_name_1>;<col_name_2>;...;<col_name_M>
<col_type_1>;<col_type_2>;...;<col_type_M>
<N>
<val_1_1>;<val_1_2>;...;<val_1_M>
<val_2_1>;<val_2_2>;...;<val_2_M>
...
<val_N_1>;<val_N_2>;...;<val_N_M>
<query>
```
- \<query> includes SELECT then a set of operations that should be performed on the columns plus optionally <group_by_column> without an operation to be performed on it, followed by GROUP_BY <group_by_column> 


Output:
- The first row specifies the names of the output, those that come between SELECT and GROUP_BY in the \<query>
- The rest is aggregated data


Example Input:
```
key;name;access;version
int;string;string;double
6
1;prog_1;r;1.1
2;prog_2;w;2.3
3;prog_1;w;1.2
4;prog_3;x;3.6
5;prog_2;r;2.8
6;prog_1;x;1.8
SELECT name, SUM(access), MIN(version), MAX(version) GROUP_BY name
```

Example Output:
```
name;SUM(access);MIN(version);MAX(version)
prog_1;rwx;1.100000;1.800000
prog_2;wr;2.300000;2.800000
prog_3;x;3.600000;3.600000
```
