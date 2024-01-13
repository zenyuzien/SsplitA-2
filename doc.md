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

## algo1 - the main function which does the job

```cpp
void algo1(const struct input_geometry& g )
```
parameter: const reference of struct input_geometry, argument variable name: g. Will be used in below snippets. To refer to this data structure.

### Algorithm:
### 1) Initialise data structures for storing intersections
```cpp
struct intersections_list 
{
    int nI ;
    vector< vector<int> > vi ;
};
```
nI --<em> the number of intersections</em><br>
vi -- <em>will contain all the distinct intersections generated</em><br>

An instance of this structure will hold all the intersections generated by the literals or the filters from the input dataset.<br>
```cpp
struct intersections_list intersection ;
intersection.nI = 0 ;
```


As mentioned earlier, to generate all possible packets and to check with all rules in the dataset,


### 2. initialise data structures for SSA-2
structure for SSA:
```cpp
struct ssa_utility 
{
    int nf ;
    vector< vector<int> > table ;
    vector< struct ssa_clause > CL;
};
```
nf -- <em>number of rules</em><br>
table  -- <em>stores all the indicies of CL where the rule number is included in the positive clause</em><br>
CL -- -- <em>list of all clauses- positive and negative</em><br>
eg. if CL = {{1,2,3},{-1,-2,-3},{2,3},{-2,-3},{1,3},{-1,-3}}
the table would be table = {{0,4},{0,2},{0,2,4}}
index 0 ~ rule 1 -- {0,4} => CL[0], CL[4] are positive clauses which include rule 1. And CL[1], CL[5] are negative clauses which include rule 1<br>

a ssa_clause structure :
```cpp
struct ssa_clause 
{
    double cost; 
    vector<int> C;
    bool flag;
}; 
```
cost -- <em>weight of the clause</em><br>
C -- <em>the rule numbers involved in intersection, positive integers mean positive cluase, negative integers mean negative cluase</em><br>
flag -- <em>used in SSA-2 algorithm</em><br>
```cpp
struct ssa_utility ssa_g ;
vector <int> s1,s2 ;
ssa_g.nf = g.nR ;
ssa_g.table.resize(ssa_g.nf);
```
the s1 is the set1, will contain the rule numbers to be split to set 1. And the s2 is set2. If a rule in not in any of s1,s2, it means it can be in any set.


### 3. generating all possible packets possible

```cpp
vector<int> packet(5,0);

    for (int i0 = 0; i0 <= g.range[0]; ++i0) {
    packet[0] = i0;
    for (int i1 = 0; i1 <= g.range[1]; ++i1) {
    packet[1] = i1;
    for (int i2 = 0; i2 <= g.range[2]; ++i2) {
    packet[2] = i2;
    for (int i3 = 0; i3 <= g.range[3]; ++i3) {
    packet[3] = i3;
    for (int i4 = 0; i4 <= g.range[4]; ++i4) {
    packet[4] = i4;

    // process the packet in furthur steps

}}}}}
```
5 loops, 1 loop for each dimension taking all possible values given by the user. 

### 4. check which all filters the generated packet satisfies

let the packet iterate through the rules, a loop for that. Within each rule, let it iterate through fields. A nested loop for that.
If a packet satisfies every field of at least 2 rules, that's an intersection.

so, a flag to check if the packet satisfied every dimension -- bool full <br>
Now if the packet checks out to be full, how do we know if it is the first filter to be satisfied? It needs to satisfy 2 filters minimum for generating an intersection. Hence a flag -- bool at1 <br>
It will be false at first, when the first packet satisfies the 'full' flag, at1 will be set true. So when the packet satisfies a second literal, a new intersection will be instantized. <br>
But there is yet another concern - when the packet satisfies a 2nd rule, it will instantize an intersection, but it should only append the first satisfied filter once. So a flag for that again -- bool atf <br>
and an integer to store the first rule --  int first <br> 
code snippet for above description:
```cpp
at1 = false ;
atf = true ;

for(q = 0 ; q < g.nR ; q++) // n
{
     bool full = true ; 
     for( r = 0 ; r < 5 ; r++ )
     {
          if( packet[r] < g.v[q][r].first || packet[r] > g.v[q][r].second  )
          {
               full = false ;
               break ;
          }
     }
          if(full)
          {
                if(at1) // if already satisfied at least 1 rule
                {
                    if(atf) // if it is the 2nd rule satisfied, create new intersection
                    {
                        intersection.nI++;
                        intersection.vi.push_back(vector<int>());
                        intersection.vi[intersection.nI -1].push_back(first);  
                        atf =false;
                    }
                    // directly appending to existing intersection
                    intersection.vi[intersection.nI -1].push_back(q+1);
                }
                else // first rule satisfied
                {
                    first = q+1 ; // store the first rule so append when another rule satisfied
                    at1= true; 
                }
         }
}
```




