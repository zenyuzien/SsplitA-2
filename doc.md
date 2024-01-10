# SsplitA-2

## what does the program do ?

in a high level, this program aims to tell how to split the rules from the rule dataset into 2 sets where the intersections are reduced by at least 75%. It tells which all rules to be in first set, and which all in the other.

## how does the program do it ?

well, let this be answered in a comprehensive manner.

### INPUT structure

for the program SSA-2(5).cpp:
seperated by spaces or new lines:
<em>number of rules</em>
<em>max value in each dimension/field (minimum is considered to be zero)</em>
for each rule, 10 values to be input. the range (2 values) of 5 dimensions.

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
