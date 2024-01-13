# SsplitA-2

## what does the program do ?

in a high level, this program aims to tell how to split the rules from the rule dataset into 2 sets where the intersections are reduced by at least 75%. It tells which all rules to be in first set, and which all in the other.

## how does the program do it ?

SSA-2.cpp - rules can have any number of dimensions, and the maximum value is fixed for all fields
SSA-2(5).cpp - rules have fixed 5 dimensions, and each dimension can have different maximum values ( different ranges )
well, let this be answered in a comprehensive manner. First, in a high level without code, after that, code will be explained.

### INPUT structure

for the program SSA-2(5).cpp:
seperated by spaces or new lines:<br>
<em>number of rules</em><br>
<em>max value in each dimension/field (minimum is considered to be zero)</em><br>
for each rule, 10 values to be input. the range (2 values) of 5 dimensions.<br>

eg. for this sample rule database with 5 fixed dimensions

| Rule no |   SP   |   DP   |   SIP  |   DIP  |  Ptcl  |
| ------- | ------ | ------ | ------ | ------ | ------ |
|    1    | 0 - 5  | 1 - 7  | 0 - 15 | 0 - 31 | 16 -31 |
|    2    | 6 - 15 | 1 - 7  | 8 - 15 | 16 -31 | 16 -31 |
|    3    | 0 - 5  | 1 - 7  | 0 - 15 | 0 - 31 | 16 -31 |
|    4    | 9 - 9  | 8 - 15 | 0 - 15 | 0 - 31 | 16 -31 |

the input would be:<br>
4<br>
15 15 15 31 31 <br>
0 5 1 7 0 15 0 31 16 31<br>
6 15 1 7 8 15 16 31 16 31<br>
0 5 1 7 0 15 0 31 16 31<br>
9 9 8 15 0 15 0 31 16 31<br>
or all in single line or all in newlines, wouldn't matter as the method of input is just console input ( cin >> ).


for the program SSA-2.cpp:
seperated by spaces or new lines:<br>
<em>maximum value in every field - input is the exponent of 2</em><br>
<em>number of rules, number of dimensions for each rule</em><br>
for each rule, 2*(dimensions) values to be input. the range (2 values) of dimensions.<br>

eg. for this sample rule database with 4 dimensions

| Rule no |  dim1  |  dim2  |  dim3  |  dim4  |
| ------- | ------ | ------ | ------ | ------ |
|    1    | 0 - 5  | 1 - 7  | 0 - 15 | 0 - 31 |
|    2    | 6 - 15 | 1 - 7  | 8 - 15 | 16 -31 |
|    3    | 0 - 5  | 1 - 7  | 0 - 15 | 0 - 31 |
|    4    | 9 - 9  | 8 - 15 | 0 - 15 | 0 - 31 |

the input would be:<br>
5<br>
4 4<br>
0 5 1 7 0 15 0 31<br>
6 15 1 7 8 15 16 31<br>
0 5 1 7 0 15 0 31<br>
9 9 8 15 0 15 <br>

### approach of solving

The problem can be split to 2 sub-problems:<br> 1) to identify intersections <br> 2) to split rules based on intersections

### To identify intersections

This program takes brute force approach to identify all the intersections. All possible packets from the fields' ranges are generated to check which all rules it satisfies. All the rules which are satisfied from each packet are stored in a container, and this small container which contains the satisfied rules for a packet, will be appended to a larger container which contains all the small-containers which has the rules satisfied by every packet. After all packets are tested, the duplicates from the large container are removed. Hence we are left with unique intersections.

### to split intersections

The aim is to split the rules such that intersections are reduced. This problem can be reduced to MAX-SAT problem and it is a known fact that to perfectly split to reduce maximum number of intersections is a NP-hard problem. However, this program uses Set split approach which is based on Johnson's algorithm which is an 3/4 approximation algorithm (it is guaranteed that every intersection contains at least 2 literals ((2^k -1 / 2^k)=3/4) ). The rule numbers which are involved in intersections will be split to 2 sets and the program prints to console on which rule numbers in set 1 and which all in set2. The rules which aren't involved in any intersections in the first place are not sent to any set specifically as it doesn't matter. Detailed explanation of the algorithm will be explained with code.

## Code explanation

### INPUT

for SSA-2(5).cpp
```cpp
struct input_geometry 
{
    int nR ;
    vector<int> range;
    vector< vector< pair<int, int> > > v ;
};
```
nR    -- <em>number of rules</em><br>
range -- <em>stories the maximum value for each of the 5 fields</em><br>
v     -- <em>stores the range of the corresponding field for all fields and for all rules</em><br>

for SSA-2.cpp
```cpp
struct input_geometry 
{
    int p, nR , nD ;
    vector< vector< pair<int, int> > > v ;
};
```
nR    -- <em>number of rules</em><br>
p    -- <em>exponent of 2 for the maximum value possible for any field</em><br>
nD    -- <em>number of dimensions/fields for a rule</em><br>
v     -- <em>stores the range of the corresponding field for all fields and for all rules</em><br>




