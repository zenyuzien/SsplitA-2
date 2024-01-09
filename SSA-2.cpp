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
    int p, nR , nD ;
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
    vector<int> packet(g.nD);

    // init min values
    for(int x = 0 ; x < g.nD ; x++)
    packet[x] = 0 ; 



    int q,r,z, maxValue= pow(2,g.p) ;
    do // configure p 
    {
        bool at1 = false ;
        bool atf = true ;
        int first ;

        for(q = 0 ; q < g.nR ; q++) // n
        {
            bool full = true ; 
            for( r = 0 ; r < g.nD ; r++ )
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
        // Update vector p to the next combination
                for (z = 0; z < g.nD; ++z) 
                {
                    if (packet[z] < maxValue) 
                    {
                        packet[z]++;
                        break;
                    }
                    else 
                    packet[z] = 0;
                }
    }
    while (z < g.nD);

   /// cout << intersection.nI << endl; ;
    removeDuplicateVectors(intersection.vi);
   /// cout<< intersection.vi.size() << endl ;
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
    cout<<"no.of rules: " << g.nR << " no.of dimensions: " << g.nD <<endl ;
    int i , j ; 
    for(i =0 ; i < g.nR ; i++ )
    {
        cout << "rule "<< i+1 << ": " ;
        for(j=0 ; j< g.nD; j++)
        cout << "[ "<< g.v[i][j].first << " : "<< g.v[i][j].second << " ]" ;
        cout<<endl;
    }
}

int main()
{
    struct input_geometry g ;
    cin >> g.p >> g.nR >> g.nD ;
    g.v.resize(g.nR, vector<pair<int, int>>(g.nD));
    int min=pow(2,g.p), max= 0;

    for (int i = 0; i < g.nR; i++)
    {
        for (int j = 0; j < g.nD; j++)
        {
            cin >> g.v[i][j].first >> g.v[i][j].second;
            if(min > g.v[i][j].first)
            min = g.v[i][j].first;
            if(max  < g.v[i][j].second)
            max = g.v[i][j].second ;
        }
    } 

    display_input_geometry(g); 
    algo1(g);
    
    return 0;
}