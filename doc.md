# SsplitA-2

## what does the program do ?

in a high level, this program aims to tell how to split the rules from the rule dataset into 2 sets where the intersections are reduced by at least 75%. It tells which all rules to be in first set, and which all in the other.

## how does the program do it ?

well, let this be answered in a comprehensive manner.

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
The complexity of this method is O(product(all dimension ranges)* no.of rules* no.of dimensions)




