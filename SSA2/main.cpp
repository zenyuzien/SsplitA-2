#include <iostream>

#include <bits/stdc++.h>

using namespace std;
const double BILLION = 1000000000.0;
const string path = "C:/Users/rushi/Downloads/SSA-2_updated/";

struct ssa_utility {
  long nf;
  vector < vector < uint32_t >> table;
  vector < pair < long double, bool >> CL;
  // CL.first -> cost 
  // CL.second -> bool

};

struct rule {
  set < long > lit;
  vector < pair < uint32_t, uint32_t > > d;
};

struct intersections_list {
  long nI;
  vector < vector < long >> vi;
};

void appendIntegersFromLine(const string& inputFileName, int lineNumber, vector<int>& result) {
    ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << inputFileName << endl;
        return;
    }

    string line;
    int currentLineNumber = 1;

    while (getline(inputFile, line)) {
        if (currentLineNumber == lineNumber) {
            istringstream iss(line);
            int number;

            while (iss >> number) {
                result.push_back(number);
            }

            inputFile.close();
            return;
        }

        currentLineNumber++;
    }

    inputFile.close();
    // If the specified line number is not found, result vector remains unchanged
}


void ssa_utility_clear(struct ssa_utility & s) {
  s.nf = 0;
  s.table.clear();
  s.CL.clear();
}

void
writeVectorToFile(const vector < long > & numbers,
  const string & filename) {
  ofstream outputFile(filename);

  if (!outputFile.is_open()) {
    cerr << "in writing vectortofile, Error opening the file " << filename << endl;
    return;
  }

  for (long number: numbers)
    outputFile << number << '\n';

  outputFile.close();
}

bool
areSetsDifferent(const set < long > & set1,
  const set < long > & set2) {

  bool flag = false;
  for (const auto & element: set1)
    if (set2.find(element) == set2.end())
      flag = true;

  for (const auto & element: set2)
    if ((set1.find(element) == set1.end()) && (flag))
      return true;

  return false;
}

void
ruledb(vector < struct rule > & db,
  const string & filename) {
  ifstream l(filename);
  ifstream f(path + "text.txt");
  if (!f.is_open() || !l.is_open()) {
    cerr << "in ruledb, Error opening the file." << endl;
    return; 
  }

  string line;
  uint32_t num;

  vector < int > ldb;

  while (getline(l, line)) {
    istringstream iss(line);
    iss >> num;
    ldb.emplace_back(num);
  }

  unsigned long long ptr = 0;
  long tptr = 0;
  long nR = 0;
  while (ptr < ldb.size()) {
    while (ldb[ptr] != tptr) {
      tptr++;
      getline(f, line);
    }
    istringstream iss(line);

    ptr++;
    struct rule tmp;
    tmp.d.resize(5);
    char delimiter;
    uint32_t first, second;
    int dim = 0;
    while (iss >> first >> delimiter >> second) {
      tmp.d[dim++] = {
        first,
        second
      };
    }
    tmp.lit.insert(nR++);
    db.emplace_back(tmp);
  }

  l.close();
  f.close();
  return;
}

int
INTERSECTIONS(vector < struct rule > & db, ofstream & wr) {
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, & start);
  //wr << "no.of rules: " << db.size() << endl;

  uint32_t nR = db.size();
  int starter = db.size() - 1, after;
  int dbptr= starter; 

  map< vector<unsigned long long> , pair< set<long> , int > > box ;
  bool change ;

 do 
 {
    change = false ;
    starter = db.size() -1 ;

    cout << "new iteration, new starter value: " << starter << endl;
    //cout<<".";
    for (int i = starter; i > 0; i--) 
    {  
      if( (i > 1000) && ((i % 1000) == 0) ) cout<<"*";
      for (int j = i - 1; j >= 0; j--) 
      {
      

        if (!areSetsDifferent(db[i].lit, db[j].lit)) 
        {
          continue;
        }

        struct rule tmp;
        vector<unsigned long long > cmp ;
        cmp.resize(5);
        tmp.d.resize(5);
        bool noInt = false;

        for (int dim = 0; dim <= 4; dim++) 
        {
          tmp.d[dim].second =
            db[i].d[dim].second <
            db[j].d[dim].second ? db[i].d[dim].second : db[j].d[dim].second;

          tmp.d[dim].first =
            db[i].d[dim].first >
            db[j].d[dim].first ? db[i].d[dim].first : db[j].d[dim].first;

          if (tmp.d[dim].first > tmp.d[dim].second) {

            noInt = true;
            break;
          }

            cmp[dim] = tmp.d[dim].first ;
            cmp[dim] = cmp[dim] << 32 ;
            cmp[dim] += tmp.d[dim].second; 
        }
        if (noInt)
          continue;

        tmp.lit.insert(db[i].lit.begin(), db[i].lit.end());
        tmp.lit.insert(db[j].lit.begin(), db[j].lit.end());

        auto isThere = box.find( cmp );
        if( isThere == box.end() )
        {
            db.emplace_back(tmp);
            dbptr++;change=true;
            auto pointer = prev(db.end());
            box.insert({cmp,{ tmp.lit , dbptr}}); 
        }
        else
        {
            int bef = (*isThere).second.first.size(); 
            (*isThere).second.first.insert(tmp.lit.begin(), tmp.lit.end());
            if( (*isThere).second.first.size() > bef ) change = true;
            db[(*isThere).second.second].lit.insert(tmp.lit.begin(), tmp.lit.end());

        }
      }
    }
  }
  while (change);

  //wr << " no. of intersections: " << db.size() - nR << endl;
  

  clock_gettime(CLOCK_REALTIME, & end);
double time_spent = (end.tv_sec - start.tv_sec) +
    (end.tv_nsec - start.tv_nsec) / BILLION;
    wr << " -" << db.size() - nR << " "<< time_spent ;
  

    return db.size()- nR; 

 // wr << "The total elapsed time to find intersections: " << time_spent <<
  //  " seconds" << endl;
}


void
SSA_ratioMethod(struct ssa_utility & g, vector < long > & s1, vector < long > & s2,
  vector < long > & s3, bool eq) {
  vector < bool > visit(g.nf, false);
  long done = g.nf;

  while (done)
  {
    long double psum = 0, nsum = 0;
    long max_i = -1;
    long double max_ratio = INT_MIN;
    for (long i = g.nf - 1; i >= 0; i--) {
      psum = 0, nsum = 0;
      if (visit[i] == true) continue;
      for (auto j: g.table[i]) {

        if (g.CL[j].second)
          psum += g.CL[j].first;
        if (g.CL[j + 1].second)
          nsum += g.CL[j + 1].first;
      }

      long double ratio;
      if (nsum == 0)
        ratio = psum;
      else
        ratio = psum / nsum;

      if (eq && (ratio >= max_ratio)) {
        max_i = i;
        max_ratio = ratio;
      } else if (ratio > max_ratio) {
        max_i = i;
        max_ratio = ratio;
      }
    }

    long i = max_i;
    visit[max_i] = true;
    done--;

    psum = 0;
    nsum = 0;
    if (g.table[i].size() == 0) {
      s3.emplace_back(i + 1);
      continue;
    }

    for (auto j: g.table[i]) {

      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }

    if (psum > nsum) {
      s1.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j + 1].second = false;
      for (auto j: g.table[i])
        g.CL[j].first *= 2;
    }

  }
}

void
SSA_Linear(struct ssa_utility & g, vector < long > & s1, vector < long > & s2,
  vector < long > & s3, bool asc) {
  int i, cnd;

  if (asc) {
    i = 0;
    cnd = g.nf;
  } else {
    i = g.nf - 1;
    cnd = -1;
  }

  while (true) {

    if (i == cnd)
      break;
    long double psum = 0, nsum = 0;
    if (g.table[i].size() == 0) {
      s3.emplace_back(i + 1);
	  if(asc)i++;
	  else i--;
      continue;
    }
    for (auto j: g.table[i]) {
      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }

    if (psum > nsum) {
      s1.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j + 1].second = false;
      for (auto j: g.table[i])
        g.CL[j].first *= 2;
    }

    if (asc )
	i++;    
    else i--;
  }

}

void
SSA_nClauses(struct ssa_utility & g, vector < long > & s1, vector < long > & s2,
  vector < long > & s3, bool asc) {

  priority_queue < pair < long, long >> pq1;
  priority_queue < pair < long, long > , vector < pair < long, long >> , greater < pair < long, long >> > pq2;

  for (long i = g.nf - 1; i >= 0; i--)
    if (asc)
      pq2.push({
        g.table[i].size(),
        i
      });
    else
      pq1.push({
        g.table[i].size(),
        i
      });

  while (1) {
    long double psum = 0, nsum = 0;
    long i;
    if (asc) {
      if (pq2.size() == 0) break;
      i = pq2.top().second;
      pq2.pop();
    } else {
      if (pq1.size() == 0) break;
      i = pq1.top().second;
      pq1.pop();
    }
    if (g.table[i].size() == 0) {
      s3.emplace_back(i + 1);
      continue;
    }

    for (auto j: g.table[i]) {

      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }

    if (psum > nsum) {
      s1.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j + 1].second = false;
      for (auto j: g.table[i])
        g.CL[j].first *= 2;
    }
  }
}

void
SSA_HYBRID(struct ssa_utility & g, vector < long > & s1, vector < long > & s2,
  vector < long > & s3, bool eq ) {

  vector < bool > visit(g.nf, false);
  long done = g.nf;

  while (done)
  {
    long double psum = 0, nsum = 0;
    long max_i = -1;
    long double max_ratio = INT_MIN;
    for (long i = g.nf - 1; i >= 0; i--) 
    {
      psum = 0, nsum = 0;
      if (visit[i] == true) continue;
      for (auto j: g.table[i]) {

        if (g.CL[j].second)
          psum += g.CL[j].first;
        if (g.CL[j + 1].second)
          nsum += g.CL[j + 1].first;
      }

      long double ratio;
      if (nsum == 0)
        ratio = psum;
      else
        ratio = psum / nsum;

      if(eq)
      {
        if (( ratio > max_ratio) || (ratio == max_ratio && (g.table[i].size() < g.table[max_i].size() ))) {
        max_i = i;
        max_ratio = ratio;
      }
      }
      else 
      { // eq false ->nclauses max
        if (( ratio > max_ratio) || (ratio == max_ratio && (g.table[i].size() > g.table[max_i].size() ))) {
        max_i = i;
        max_ratio = ratio;
      }
      }

      
    }

    long i = max_i;
    visit[max_i] = true;
    done--;

    psum = 0;
    nsum = 0;
    if (g.table[i].size() == 0) {
      s3.emplace_back(i + 1);
      continue;
    }

    for (auto j: g.table[i]) {

      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }

    if (psum > nsum) {
      s1.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);

      for (auto j: g.table[i])
        g.CL[j + 1].second = false;
      for (auto j: g.table[i])
        g.CL[j].first *= 2;
    }

  }
}

void
PRE_SSA(struct ssa_utility & ssa_g, vector < struct rule > & db,
  const string & str) {
  long ic = 0;
  long nI = 0 ;
  long double I1;
  ofstream wr(str);
  auto itr = db.begin();
  while( (*itr).lit.size()== 1 )
  itr++;
  int bigINT = 0 ;
  int nINT = 0 ; 
  for (; itr < db.end(); ++itr) {

      ++nI; 
    I1 = 0;
    nINT = 0 ; 
    for (auto itr_ = ( * itr).lit.begin(); itr_ != ( * itr).lit.end(); ++itr_) 
    {
      wr << * (itr_) + 1 << " ";
      ssa_g.table[ * (itr_)].emplace_back(ic);
      I1++;
      nINT++;
    }
    bigINT = (bigINT < nINT) ? nINT : bigINT ;  
    wr << endl;
    ic += 2;
    I1 = 1 / pow(2, I1);
    if (I1) {
      ssa_g.CL.push_back({
        I1,
        true
      });
      ssa_g.CL.push_back({
        I1,
        true
      });
    }
  }
  wr<<"big INT: "<< bigINT << endl ;
  wr.close();
}


int intfor(long size1,long size2, ofstream & wr,
  const string tmp) {

  struct ssa_utility ssa_s;
  ssa_s.nf = size1;
  ssa_s.table.resize(ssa_s.nf);
  vector < struct rule > set_db;

  ruledb(set_db, tmp + "set1.txt");
  //wr << "\nIn set1:\n";
  int m1 = INTERSECTIONS(set_db, wr);
  PRE_SSA(ssa_s, set_db, tmp + "s1_int.txt");

  set_db.clear();
  ssa_utility_clear(ssa_s);
  ssa_s.nf = size2;
  ssa_s.table.resize(ssa_s.nf);

  ruledb(set_db, tmp + "set2.txt");
  //wr << "\nIn set2:\n";
  int m2 = INTERSECTIONS(set_db, wr);
  PRE_SSA(ssa_s, set_db, tmp + "s2_int.txt");
  return m1+m2;
}


void mySSA(struct ssa_utility & g, int rounds, vector < long > & s1, vector < long > & s2,
  vector < long > & s3, ofstream& wr, string tmp)
{
    ofstream swr("C:/Users/rushi/Downloads/SSA-2_updated/SSA_rand.txt");
    uint32_t min_int = UINT32_MAX; 
    int min_int_arr = -1 ; 
    srand(time(0));
    unordered_set<int> uniqueHashes;
    int hash = 0 ;
    int arr[ g.nf ];
      for(int i = 0; i< g.nf ; i++)
        arr[i] = i ;
    auto backup = g.CL ;
    int swrptr =0 ; 
    wr<<endl;
    while(rounds--)
    {
      do 
      {
        random_shuffle(arr, arr + g.nf);
        for (int i = 0; i < g.nf; ++i) {
        hash *= 10;
        hash ^= (arr[i]+15559)*(i+40277);
        }
        
      }
      while(uniqueHashes.find(hash) != uniqueHashes.end());
      uniqueHashes.insert(hash);
      
      for(int i = 0 ; i < g.nf ; i++)
      swr << arr[i] << " ";
      swr << endl ;
      swrptr++;


      g.CL = backup ;
      
      //cout<<"round: "<<rounds<<endl; 
      for(auto i : arr)
      {
       // cout << i << endl;
        /*for(auto x : g.CL)
        {
          cout << x.first <<" "<< x.second<<endl;
        }*/
        double psum = 0;
        double nsum = 0;

          if (g.table[i].size() == 0) {
           s3.emplace_back(i + 1);
            continue;
          }

          for (auto j: g.table[i]) {

            if (g.CL[j].second)
              psum += g.CL[j].first;
            if (g.CL[j + 1].second)
              nsum += g.CL[j + 1].first;
          }
         // cout << "psum: "<<psum << " nsum: "<< nsum<<endl;

          if (psum > nsum) 
          {
            s1.emplace_back(i + 1);

            for (auto j: g.table[i])
              g.CL[j].second = false;
            for (auto j: g.table[i])
              g.CL[j + 1].first *= 2;
          } 
          else 
          {
            s2.emplace_back(i + 1);

            for (auto j: g.table[i])
              g.CL[j + 1].second = false;
            for (auto j: g.table[i])
              g.CL[j].first *= 2;
          }
      }
     // cout<<endl;
       sort(s1.begin(), s1.end());
       sort(s2.begin(), s2.end());
       wr << swrptr << ": " << s1.size() << " " << s2.size() <<" "<< s3.size() ;
       //wr << "\nafter SSA-2 " << rounds << endl << "set1: " << s1.size() << " rules \nset2: " << s2.
       //size() << " rules \n";
       //wr << "set3(any set): " << s3.size() << " rules\n";

       writeVectorToFile(s1, tmp + "set1.txt");
       writeVectorToFile(s2, tmp + "set2.txt");
       writeVectorToFile(s3, tmp + "set3.txt");

      auto tot_i = intfor(s1.size(),s2.size(), wr, tmp);
      if( tot_i < min_int )
      {
        min_int = tot_i;
        min_int_arr = swrptr ;
      }
      wr<<endl;
      s1.clear();
      s2.clear();
      s3.clear();

    }
    cout << min_int << " "<<  min_int_arr << endl;
    vector<int> newV;
    appendIntegersFromLine("C:/Users/rushi/Downloads/SSA-2_updated/SSA_rand.txt",min_int_arr, newV);  


    g.CL = backup ;
    for(auto i : newV)
      {
       // cout << i << endl;
        /*for(auto x : g.CL)
        {
          cout << x.first <<" "<< x.second<<endl;
        }*/
        double psum = 0;
        double nsum = 0;

          if (g.table[i].size() == 0) {
           s3.emplace_back(i + 1);
            continue;
          }

          for (auto j: g.table[i]) {

            if (g.CL[j].second)
              psum += g.CL[j].first;
            if (g.CL[j + 1].second)
              nsum += g.CL[j + 1].first;
          }
         // cout << "psum: "<<psum << " nsum: "<< nsum<<endl;

          if (psum > nsum) 
          {
            s1.emplace_back(i + 1);

            for (auto j: g.table[i])
              g.CL[j].second = false;
            for (auto j: g.table[i])
              g.CL[j + 1].first *= 2;
          } 
          else 
          {
            s2.emplace_back(i + 1);

            for (auto j: g.table[i])
              g.CL[j + 1].second = false;
            for (auto j: g.table[i])
              g.CL[j].first *= 2;
          }
      }
     // cout<<endl;
       sort(s1.begin(), s1.end());
       sort(s2.begin(), s2.end());
       wr << swrptr << ": " << s1.size() << " " << s2.size() <<" "<< s3.size() ;
       //wr << "\nafter SSA-2 " << rounds << endl << "set1: " << s1.size() << " rules \nset2: " << s2.
       //size() << " rules \n";
       //wr << "set3(any set): " << s3.size() << " rules\n";

       writeVectorToFile(s1, tmp + "set1.txt");
       writeVectorToFile(s2, tmp + "set2.txt");
       writeVectorToFile(s3, tmp + "set3.txt");
      wr<< "min ints: " << min_int << " " << min_int_arr << endl;
      auto tot_i = intfor(s1.size(),s2.size(), wr, tmp);





  return;
}



void SSA_method(struct ssa_utility & ssa_g, vector < long > & s1, vector < long > & s2, vector < long > & s3, ofstream & wr, long nR, vector < struct rule > & db, int method, bool printInt) 
{
  ssa_utility_clear(ssa_g);
  ssa_g.nf = nR;
  ssa_g.table.resize(ssa_g.nf);
  string tmp;
  s1.clear();
  s2.clear();
  s3.clear();
  PRE_SSA(ssa_g, db, "C:/Users/rushi/Downloads/SSA-2_updated/text_int.txt");

  if (method == 1) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/linear_asc/";
    SSA_Linear(ssa_g, s1, s2, s3, true);
  } else if (method == 2) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/linear_dsc/";
    SSA_Linear(ssa_g, s1, s2, s3, false);
  } else if (method == 3) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/no_clauses_mim/";
    SSA_nClauses(ssa_g, s1, s2, s3, true);
  } else if (method == 4) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/no_clauses_max/";
    SSA_nClauses(ssa_g, s1, s2, s3, false);
  } else if (method == 5) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/ratio_1/";
    SSA_ratioMethod(ssa_g, s1, s2, s3, true);
  } else if (method == 6) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/ratio_2/";
    SSA_ratioMethod(ssa_g, s1, s2, s3, false);
  } else if(method == 7) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/HYBRID_min/";
    SSA_HYBRID(ssa_g, s1, s2, s3, true);
  } else if(method == 8) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/HYBRID_max/";
    SSA_HYBRID(ssa_g, s1, s2, s3, false);
  } else if(method == 9)  {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/mySSA/";
    mySSA(ssa_g, 5, s1, s2, s3, wr,tmp);
    return;
  }
  sort(s1.begin(), s1.end());
  sort(s2.begin(), s2.end());
  wr << "\nafter SSA-2 " << tmp << endl << "set1: " << s1.size() << " rules \nset2: " << s2.
  size() << " rules \n";
  wr << "set3(any set): " << s3.size() << " rules\n";

  writeVectorToFile(s1, tmp + "set1.txt");
  writeVectorToFile(s2, tmp + "set2.txt");
  writeVectorToFile(s3, tmp + "set3.txt");

  if (printInt == true)
    intfor(s1.size(),s2.size(), wr, tmp);

}


int
main() 
{
  ofstream wr(path + "debug.txt");
  
  ifstream f(path + "text.txt");
  if (!f.is_open()) {
    cerr << "in main, Error opening the file." << endl;
    return 1;
  }

  vector < struct rule > db;
  string line;
  long nR = 0;

  while (getline(f, line)) {
    istringstream iss(line);

    struct rule tmp;
    tmp.d.resize(5);

    char delimiter;
    uint32_t first, second;

    int dim = 0;
    while (iss >> first >> delimiter >> second)
      tmp.d[dim++] = {
        first,
        second
      };

    tmp.lit.insert(nR++);
    db.emplace_back(tmp);
  }

  f.close();

  wr << "For the input textfile\n";
  INTERSECTIONS(db, wr);

 

  struct ssa_utility ssa_g;
  vector < long > s1, s2, s3;
    //cout<<".";
   // for(int i = 1 ; i <= 8 ; i++)
   //  SSA_method(ssa_g, s1, s2, s3, wr, nR, db, i, true);
   // SSA_method(ssa_g, s1, s2, s3, wr, nR, db, 9, false);
    SSA_method(ssa_g, s1, s2, s3, wr, nR, db, 7, true);


  //newmethod(db);

  wr.close();
  
  //extractAndWriteLines("C:/Users/rushi/Downloads/SSA-2_updated/SSA_rand.txt","C:/Users/rushi/Downloads/SSA-2_updated/re_formatIP.txt",23);

  cout << "Program executed successfully :)\n";
  return 0;
}


/*

struct rule {
  set < long > lit;
  vector < pair < uint32_t, uint32_t > > d;
};

*/
void newmethod( vector < struct rule > & db )
{
    auto midpoint = db.size() / 2;

    // Create two new vectors
    vector<struct rule> db1(db.begin(), db.begin() + midpoint), db2(db.begin() + midpoint, db.end());

    int rounds = 1 ;
    int i,j ;

    for(j = 0 ; j < db2.size(); j++)
    {
      struct rule tmp;
      tmp.d.resize(1);
      bool noInt = false;

      for (int dim = 0; dim <= 4; dim++) 
      {
        tmp.d[0].second = db1[i].d[dim].second < db2[j].d[dim].second ? db1[i].d[dim].second : db2[j].d[dim].second;
        tmp.d[0].first  = db1[i].d[dim].first  > db2[j].d[dim].first  ? db1[i].d[dim].first  : db2[j].d[dim].first;

        if (tmp.d[0].first > tmp.d[0].second) {
          noInt = true;
          break;
        }
      }

      if (noInt) continue;
    }



    while(rounds--)
    {
      ;
    }

    // 1, 2, 3            // 1, 2
    // dimensions         // dimensions 


  return ;
}