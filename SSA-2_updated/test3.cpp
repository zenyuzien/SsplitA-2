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
  // Close the file
  outputFile.close();
}

bool
areSetsDifferent(const set < long > & set1,
  const set < long > & set2) {

  // cout<<set1.size() <<" "<<set2.size()<<" ..\n";
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
        //  cout << "@ " << j <<" "<< g.CL[j].flag << g.CL[j+1].flag<<endl ;
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
    //long i = pq.top().second;
    //pq.pop();
    //auto i = xx.second;		//top().second; 
    //  q.pop();
    // filter no. i 
    //display_input_geometry(g);
    /// cout<<"in "<<i+1<<endl;

    // check sum of cost of CLAUSES 
    psum = 0;
    nsum = 0;
    if (g.table[i].size() == 0) {
      s3.emplace_back(i + 1);
      continue;
    }

    for (auto j: g.table[i]) {
      //  cout << "@ " << j <<" "<< g.CL[j].flag << g.CL[j+1].flag<<endl ;
      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }
    //cout << "p: "<<psum << " n: "<<nsum << endl; 

    if (psum > nsum) {
      s1.emplace_back(i + 1);
      //  cout << "pushed to set 1 : " << i+1 << endl ;
      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);
      // cout << "pushed to set 2 : " << i+1 << endl ;
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
      //  cout << "pushed to set 1 : " << i+1 << endl ;
      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);
      // cout << "pushed to set 2 : " << i+1 << endl ;
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
  //for (auto xx:q)
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
      //  cout << "@ " << j <<" "<< g.CL[j].flag << g.CL[j+1].flag<<endl ;
      if (g.CL[j].second)
        psum += g.CL[j].first;
      if (g.CL[j + 1].second)
        nsum += g.CL[j + 1].first;
    }

    if (psum > nsum) {
      s1.emplace_back(i + 1);
      //  cout << "pushed to set 1 : " << i+1 << endl ;
      for (auto j: g.table[i])
        g.CL[j].second = false;
      for (auto j: g.table[i])
        g.CL[j + 1].first *= 2;
    } else {
      s2.emplace_back(i + 1);
      // cout << "pushed to set 2 : " << i+1 << endl ;
      for (auto j: g.table[i])
        g.CL[j + 1].second = false;
      for (auto j: g.table[i])
        g.CL[j].first *= 2;
    }
  }

}

void
ruledb(vector < struct rule > & db,
  const string & filename) {
  ifstream l(filename);
  ifstream f(path + "text.txt");
  if (!f.is_open() || !l.is_open()) {
    cerr << "in ruledb, Error opening the file." << endl;
    return; // Exit with an error code
  }
  //else 
 // cout << "opening " << filename << "\n" ;

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
    //  cout << line << endl;
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

void
INTERSECTIONS(vector < struct rule > & db, ofstream & wr) {
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, & start);
  wr << "no.of rules: " << db.size() << endl;
  //cout << "_";
  uint32_t nR = db.size();
  int starter = db.size() - 1, after;
  int dbptr= starter; 

  map< vector<unsigned long long> , pair< set<long> , int > > box ;
  bool change ;

 do 
 {
    change = false ;
    starter = db.size() -1 ;
    //wr << "new iteration, new starter value: " << starter << endl;
    cout << "new iteration, new starter value: " << starter << endl;
    
    for (int i = starter; i > 0; i--) // if(starter >100)if(i% (100) == 0 ) cout << "*";
    {  
      for (int j = i - 1; j >= 0; j--) 
      {
         /* int n=1;
         for(auto k : db)
         {
         cout << n++ << ") ";
        for(auto kk : k.lit)
        cout << kk+1 << " ";
        cout << ";";
      }
      cout<<endl;*/
        //cout<<"i= "<< i+1 << " j= "<< j +1<< endl ;
        
        if (!areSetsDifferent(db[i].lit, db[j].lit)) 
        {
          //cout << "skipping i= " << i +1<< " j= " << j+1 << " same set\n";
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
            //cout << "i= " << i+1 << " j= " << j+1 << " no int\n";
            noInt = true;
            break;
          }

            cmp[dim] = tmp.d[dim].first ;
            cmp[dim] << 32 ;
            cmp[dim] += tmp.d[dim].second; 
        }
        if (noInt)
          continue;

        tmp.lit.insert(db[i].lit.begin(), db[i].lit.end());
        tmp.lit.insert(db[j].lit.begin(), db[j].lit.end());

        auto isThere = box.find( cmp );
        if( isThere == box.end() )
        {
          //  cout << "i= " << i+1 << " j= " << j+1 << " pushed\n";
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
          //  cout << "i= " << i+1 << " j= " << j+1 << " appened to "<<(*isThere).second.second +nR +1<<"\n";
        }
      }
    }
  }
  while (change);

  wr << "no. of intersections: " << db.size() - nR << endl;
  /*for(auto x : db)
  {
      cout<<"-> ";
      for(auto y : x.lit)
      cout<<y+1<<" ";
      cout<<endl;
  }cout<<"\n";*/
  clock_gettime(CLOCK_REALTIME, & end);

  // time_spent = end - start
  double time_spent = (end.tv_sec - start.tv_sec) +
    (end.tv_nsec - start.tv_nsec) / BILLION;

  wr << "The total elapsed time to find intersections: " << time_spent <<
    " seconds" << endl;
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
  
  for (; itr < db.end(); ++itr) {
    
      ++nI; 
    I1 = 0;

    for (auto itr_ = ( * itr).lit.begin(); itr_ != ( * itr).lit.end(); ++itr_) {
      wr << * (itr_) + 1 << " ";
      ssa_g.table[ * (itr_)].emplace_back(ic);
      I1++;
    }
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
  //wr << "number of intersections: "<< nI <<endl;
  wr.close();
}

void intfor(long size1,long size2, ofstream & wr,
  const string tmp) {

  struct ssa_utility ssa_s;
  ssa_s.nf = size1;
  ssa_s.table.resize(ssa_s.nf);
  vector < struct rule > set_db;
  
  ruledb(set_db, tmp + "set1.txt");
  wr << "\nIn set1:\n";
  INTERSECTIONS(set_db, wr);
  PRE_SSA(ssa_s, set_db, tmp + "s1_int.txt");

  set_db.clear();
  ssa_utility_clear(ssa_s);
  ssa_s.nf = size2;
  ssa_s.table.resize(ssa_s.nf);

  ruledb(set_db, tmp + "set2.txt");
  wr << "\nIn set2:\n";
  INTERSECTIONS(set_db, wr);
  PRE_SSA(ssa_s, set_db, tmp + "s2_int.txt");
}

void SSA_method(struct ssa_utility & ssa_g, vector < long > & s1, vector < long > & s2, vector < long > & s3, ofstream & wr, long nR, vector < struct rule > & db, int method, bool printInt) {
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
  } else
  if (method == 6) {
    tmp = "C:/Users/rushi/Downloads/SSA-2_updated/ratio_2/";
    SSA_ratioMethod(ssa_g, s1, s2, s3, false);
  }
  sort(s1.begin(), s1.end());
  sort(s2.begin(), s2.end());
  wr << "\nafter SSA-2 " << tmp << endl << "set1: " << s1.size() << " rules \nset2: " << s2.
  size() << " rules \n";
  wr << "set3(any set): " << s3.size() << " rules\n";
 // cout<< tmp << endl ;
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
  INTERSECTIONS(db, wr);/*
  for(auto pr : db)
  {
    for(auto prr : pr.lit)
    cout<< prr +1  << " ";
    cout<<endl;
  }*/

  struct ssa_utility ssa_g;
  vector < long > s1, s2, s3;

  for (int met = 1; met <= 6; met++)
    SSA_method(ssa_g, s1, s2, s3, wr, nR, db, met, true);

  wr.close();
  cout << "Program executed successfully :)\n";
  return 0;
}