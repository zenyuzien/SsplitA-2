#include <bits/stdc++.h>
#include <iostream>
using namespace std;

struct ssa_clause 
{
    double cost; 
    vector<int> C;
    bool flag;
}; 
struct ssa_utility 
{
    int nf ;
    vector< vector<int> > table ;
    vector< struct ssa_clause > CL;
};
struct input_geometry 
{
    int nR ;
    vector<int> range;
    vector< vector< pair<int, int> > > v ;
};
struct intersections_list 
{
    int nI ;
    vector< vector<int> > vi ;
};

// for removing duplicates
bool vectorCompare(const std::vector<int>& a, const std::vector<int>& b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

vector<pair<int, int>> merge(vector<pair<int, int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    vector<pair<int, int>> output;

    for (auto& interval : intervals) {
        if (output.empty() || output.back().second < interval.first) {
            output.push_back(interval);
        } else {
            output.back().second = max(output.back().second, interval.second);
        }
    }
    return output;
}


void removeDuplicateVectors(vector<vector<int>>& vec) {
    // Step 1: Sort the vectors
    sort(vec.begin(), vec.end(), vectorCompare);

    // Step 2: Remove duplicates using unique and erase
    auto last = unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}

void SSA(struct ssa_utility& g, vector<int>& s1, vector<int>& s2)
{
    for(int i = 0 ; i < g.nf ; i++)
    {
        // filter no. i 
        //display_input_geometry(g);
       /// cout<<"in "<<i+1<<endl;
        float psum=0, nsum=0 ;
        // check sum of cost of CLAUSES 
        for( auto j : g.table[i] )
        {
          //  cout << "@ " << j <<" "<< g.CL[j].flag << g.CL[j+1].flag<<endl ;
            if(g.CL[j].flag)
            psum += g.CL[j].cost ;
            if(g.CL[j+1].flag)
            nsum += g.CL[j+1].cost ;
        }
        //cout << "p: "<<psum << " n: "<<nsum << endl; 
        
        if(psum > nsum)
        {
            s1.push_back(i+1);
          //  cout << "pushed to set 1 : " << i+1 << endl ;
            for( auto j : g.table[i] )
            g.CL[j].flag = false; 
            for( auto j : g.table[i] )
            g.CL[j+1].cost *= 2; 
            
        }
        else 
        {
            s2.push_back(i+1);
           // cout << "pushed to set 2 : " << i+1 << endl ;
            for( auto j : g.table[i] )
            g.CL[j+1].flag = false; 
            for( auto j : g.table[i] )
            g.CL[j].cost *= 2; 
        }
        
    }
    cout<<"SET 1: rule numbers: ";
    for(int x =0 ; x < s1.size(); x++)
    cout << s1[x] << " ";
    cout << endl << "SET 2: rule numbers: ";
    for(int x =0 ; x < s2.size(); x++)
    cout << s2[x] << " ";
    
}

void algo1(const struct input_geometry& g )
{
    struct ssa_utility ssa_g ;
    vector <int> s1,s2 ;

    ssa_g.nf = g.nR ;
    ssa_g.table.resize(ssa_g.nf);

    struct intersections_list intersection ;
    intersection.nI = 0 ;
    vector<int> packet(5,0);
    int q,r,first ;
    bool at1,atf;
    
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
                if(at1)
                {
                    if(atf)
                    {
                        intersection.nI++;
                        intersection.vi.push_back(vector<int>());
                        intersection.vi[intersection.nI -1].push_back(first);  
                        atf =false;
                    }
                    intersection.vi[intersection.nI -1].push_back(q+1);
                }
                else 
                {
                    first = q+1 ;
                    at1= true; 
                }
            }
        }

    }}}}}
    
    removeDuplicateVectors(intersection.vi);
    cout<< "intersections before SSA-2: " << intersection.vi.size() << endl ;
    int ic= 0 ;
    for(auto itr = intersection.vi.begin() ; itr < intersection.vi.end(); ++itr )
    {
        struct ssa_clause I1,I2 ;
        cout << "intersections: "<<endl;
        for(auto itr_ = (*itr).begin(); itr_ < (*itr).end(); ++itr_ )
        {
            cout << *itr_ << " "; ///
            ssa_g.table[*itr_ -1].push_back(ic);
            I1.C.push_back(*itr_);
            I2.C.push_back(- *itr_); 
        }
        cout <<endl ; ///
        ic +=2 ;
        I1.cost = 1 / pow(2, I1.C.size()); 
        I1.flag = true; 
        I2.cost = 1 / pow(2, I2.C.size()); 
        I2.flag = true;
        if(I1.C.size())
            {
                ssa_g.CL.push_back(I1);
                ssa_g.CL.push_back(I2);
            }
    }
    
    SSA(ssa_g,s1,s2);
    return ;
}

void display_input_geometry(const struct input_geometry& g)
{
    cout<<"no.of rules: " << g.nR <<endl ;
    int i , j ; 
    for(i =0 ; i < g.nR ; i++ )
    {
        cout << "rule "<< i+1 << ": " ;
        for(j=0 ; j< 5; j++)
        cout << "[ "<< g.v[i][j].first << " : "<< g.v[i][j].second << " ]" ;
        cout<<endl;
    }
}

int main()
{
    struct input_geometry g ;
    g.range.resize(5);
    cin >> g.nR >> g.range[0] >> g.range[1] >> g.range[2] >> g.range[3] >> g.range[4] ;
    g.v.resize(g.nR, vector<pair<int, int>>(5));

    for (int i = 0; i < g.nR; i++)
    {
        for (int j = 0; j < 5; j++)
        cin >> g.v[i][j].first >> g.v[i][j].second;
    } 

    display_input_geometry(g); 
    algo1(g);
    
    return 0;
}
