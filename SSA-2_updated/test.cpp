#include <iostream>
#include <bits/stdc++.h>
using namespace std;
const double BILLION = 1000000000.0;
const string path = "C:/Users/rushi/Downloads/SSA-2_updated/";


struct ssa_utility
{
  long nf;
    vector < vector < long >>table;
    vector < pair < long double, bool >>CL;
  // CL.first -> cost 
  // CL.second -> bool
};

struct rule
{
  set < long >lit;
    vector < pair < uint32_t, uint32_t > >d;

};

struct intersections_list
{
  long nI;
    vector < vector < long >>vi;
};

void
writeVectorToFile (const vector < long >&numbers, const string & filename)
{
  ofstream outputFile (filename);

  if (!outputFile.is_open ())
	{
	  cerr << "Error opening the file " << filename << endl;
	  return;
	}
	
for (long number:numbers)
	outputFile << number << '\n';
	

  // Close the file
  outputFile.close ();
}

bool
areSetsDifferent (const set < long >&set1, const set < long >&set2)
{

  // cout<<set1.size() <<" "<<set2.size()<<" ..\n";
  bool flag = false;
for (const auto & element:set1)
	if (set2.find (element) == set2.end ())
	  flag = true;

for (const auto & element:set2)
	if ((set1.find (element) == set1.end ()) && (flag))
	  return true;

  return false;
}

void
SSA (struct ssa_utility &g, vector < long >&s1, vector < long >&s2,
	 vector < long >&s3)
{
  //for (auto xx:q)
  //priority_queue< pair<long,long>> pq; 
  //priority_queue< pair<long,long>, vector<pair<long,long>>, greater<pair<long,long>> > pq; 
 // for (long i = g.nf-1 ; i >=0 ; i--)
 // pq.push({g.table[i].size(),i});

for (long i = g.nf-1 ; i >=0 ; i--)
  //while(pq.size()>0)
	  {
		//long i = pq.top().second;
		//pq.pop();
		//auto i = xx.second;		//top().second; 
		//  q.pop();
		// filter no. i 
		//display_input_geometry(g);
		/// cout<<"in "<<i+1<<endl;
		long double psum = 0, nsum = 0;
		// check sum of cost of CLAUSES 
		if (g.table[i].size () == 0)
		  {
			s3.push_back (i + 1);
			continue;
		  }
	  for (auto j:g.table[i])
		  {
			//  cout << "@ " << j <<" "<< g.CL[j].flag << g.CL[j+1].flag<<endl ;
			if (g.CL[j].second)
			  psum += g.CL[j].first;
			if (g.CL[j + 1].second)
			  nsum += g.CL[j + 1].first;
		  }
		//cout << "p: "<<psum << " n: "<<nsum << endl; 


		if (psum > nsum)
		  {
			s1.push_back (i + 1);
			//  cout << "pushed to set 1 : " << i+1 << endl ;
		  for (auto j:g.table[i])
			  g.CL[j].second = false;
		  for (auto j:g.table[i])
			  g.CL[j + 1].first *= 2;

		  }
		else
		  {
			s2.push_back (i + 1);
			// cout << "pushed to set 2 : " << i+1 << endl ;
		  for (auto j:g.table[i])
			  g.CL[j + 1].second = false;
		  for (auto j:g.table[i])
			  g.CL[j].first *= 2;
		  }

	  }				

  sort (s1.begin (), s1.end ());
  sort (s2.begin (), s2.end ());
}

void
ruledb (vector < struct rule >&db, const string & filename)
{



  ifstream l (filename);
  ifstream f (path + "text.txt");
  if (!f.is_open () || !l.is_open ())
	{
	  cerr << "Error opening the file." << endl;
	  return;					// Exit with an error code
	}

  string line;
  uint32_t num;

  vector < int >ldb;
  //vector<struct rule> rdb;

  while (getline (l, line))
	{
	  istringstream iss (line);
	  iss >> num;
	  // cout<<"_ "<<num;
	  ldb.push_back (num);
	}
  // cout << endl;

  long long unsigned ptr = 0;
  long tptr = 0;
  long nR = 0;
  while (ptr < ldb.size ())
	{
	  while (ldb[ptr] != tptr)
		{
		  tptr++;
		  getline (f, line);
		}
	  istringstream iss (line);
	  //  cout << line << endl;
	  ptr++;
	  struct rule tmp;
	  tmp.d.resize (5);
	  char delimiter;
	  uint32_t first, second;
	  int dim = 0;
	  while (iss >> first >> delimiter >> second)
		{
		  //cout << ".";
		  //wr << "dissect - "<<first << " " << second << endl ;
		  tmp.d[dim++] = { first, second };
		}
	  tmp.lit.insert (nR++);
	  //if(nR%50 ==0)cout<<"*";
	  db.push_back (tmp);
	}
  //cout << nR;

  l.close ();
  f.close ();
  return;
}


void
INTERSECTIONS (vector < struct rule >&db, ofstream & wr)
{
  struct timespec start, end;
  clock_gettime (CLOCK_REALTIME, &start);
  wr << "no.of rules: " << db.size () << endl;
  //cout << "no.of rules: " << db.size () << endl;
  uint32_t nR = db.size ();
  long ex = 0;
  long starter = db.size () - 1;
  set < set < long >>junk;
  bool flag = false;
  int before, after;
  do
	{
	  
	  wr << "new iteration, new starter value: " << starter << endl;
	  flag = false;
	  for (long i = starter; i > 0; i--)
		{
		  //if(starter >100)
		  //if(i% (starter/100) == 0 )cout << "*";

		  for (long j = i - 1; j >= 0; j--)
			{
			  //cout<<"i= "<< i << " j= "<< j << endl ;
			  if (!areSetsDifferent (db[i].lit, db[j].lit))
				{
				  
				   cout << "skipping";
				   cout << "i= " << i +1<< " j= " << j+1 << " same set\n";
				  continue;
				}

			  struct rule tmp;
			  tmp.d.resize (5);
			  bool noInt = false;
			  for (int dim = 0; dim <= 4; dim++)
				{
				  tmp.d[dim].second =
					db[i].d[dim].second <
					db[j].d[dim].second ? db[i].d[dim].second : db[j].d[dim].
					second;
				  tmp.d[dim].first =
					db[i].d[dim].first >
					db[j].d[dim].first ? db[i].d[dim].first : db[j].d[dim].
					first;
				  if (tmp.d[dim].first > tmp.d[dim].second)
					{
					   cout << "i= " << i+1 << " j= " << j+1 << " no int\n";
					  noInt = true;
					  break;
					}
				}
			  if (noInt)
				continue;

			  tmp.lit.insert (db[i].lit.begin (), db[i].lit.end ());
			  tmp.lit.insert (db[j].lit.begin (), db[j].lit.end ());



			  bool flag__ = false;
			  if (((tmp.d[0].first == db[i].d[0].first)
				   && (tmp.d[0].second == db[i].d[0].second))
				  && ((tmp.d[1].first == db[i].d[1].first)
					  && (tmp.d[1].second == db[i].d[1].second))
				  && ((tmp.d[2].first == db[i].d[2].first)
					  && (tmp.d[2].second == db[i].d[2].second))
				  && ((tmp.d[3].first == db[i].d[3].first)
					  && (tmp.d[3].second == db[i].d[3].second))
				  && ((tmp.d[4].first == db[i].d[4].first)
					  && (tmp.d[4].second == db[i].d[4].second)))
				{				// tmp is subset of 1st rule 

				  auto del = junk.find (tmp.lit);
				  if (del != junk.end ())
					{
					  for (auto it = db.begin (); it != db.end (); ++it)
						{
						  if ((*it).lit == (*del))
							{
							  db.erase (it);
							  cout << "deleted tmp from  " << i << " " << j << endl;
							  flag__ = true;
							  break;
							}
						}
					}
				  if (flag__)
					continue;

				  cout << "appening " << i +1<< " " << j+1 << " to " << i+1 << endl;
				  //junk.erase (db[i].lit);

				  if (db[i].lit.size () == 1)
					ex++;		// intersections from the first nR entries
				  db[i].lit.insert (tmp.lit.begin (), tmp.lit.end ());
				  junk.insert (db[i].lit);
				  continue;
				}
			  if (((tmp.d[0].first == db[j].d[0].first)
				   && (tmp.d[0].second == db[j].d[0].second))
				  && ((tmp.d[1].first == db[j].d[1].first)
					  && (tmp.d[1].second == db[j].d[1].second))
				  && ((tmp.d[2].first == db[j].d[2].first)
					  && (tmp.d[2].second == db[j].d[2].second))
				  && ((tmp.d[3].first == db[j].d[3].first)
					  && (tmp.d[3].second == db[j].d[3].second))
				  && ((tmp.d[4].first == db[j].d[4].first)
					  && (tmp.d[4].second == db[j].d[4].second)))
				{				// tmp is subset of 2nd rule 


				  auto del = junk.find (tmp.lit);
				  if (del != junk.end ())
					{
					  for (auto it = db.begin (); it != db.end (); ++it)
						{
						  if ((*it).lit == (*del))
							{
							  db.erase (it);
							  cout<<"deleted tmp from "<<i<<" "<<j<<endl;
							  flag__ = true;
							  break;
							}
						}
					}
				  if (flag__)
					continue;
				  cout << "appening " << i+1 << " " << j +1<< " to " << j+1 << endl;
				  //junk.erase (db[j].lit);
				  if (db[j].lit.size () == 1)
					ex++;		// intersections from the first nR entries
				  db[j].lit.insert (tmp.lit.begin (), tmp.lit.end ());
				  junk.insert (db[j].lit);
				  ;				//append tmp lits to 2nd rule
				  continue;
				}

			  before = junk.size ();
			  junk.insert (tmp.lit);
			  after = junk.size ();
			  if (before == after)
				{
				  cout << "i= " << i+1 << " j= " << j+1 << " dupe\n";
				  continue;
				}


			   cout << "i= " << i+1 << " j= " << j+1 << " pusheed\n";
			  db.push_back (tmp);
			  flag = true;
			}
		}
	  starter = db.size () - 1;
	}
  while (flag);
  wr << "no. of intersections: " << db.size () - nR + ex << endl;
  clock_gettime (CLOCK_REALTIME, &end);

  // time_spent = end - start
  double time_spent = (end.tv_sec - start.tv_sec) +
	(end.tv_nsec - start.tv_nsec) / BILLION;

  wr << "The total elapsed time to find intersections: " << time_spent <<
	" seconds" << endl;
}

void
PRE_SSA (struct ssa_utility &ssa_g, vector < struct rule >&db,
		 const string & str)
{
  long ic = 0;
  long double I1;
  ofstream wr (path + str);
  for (auto itr = db.begin (); itr < db.end (); ++itr)
	{
	  if ((*itr).lit.size () == 1)
		continue;
	  I1 = 0;

	  for (auto itr_ = (*itr).lit.begin (); itr_ != (*itr).lit.end (); ++itr_)
		{
		  wr << *(itr_) + 1 << " ";
		  ssa_g.table[*(itr_)].push_back (ic);
		  I1++;
		}
	  wr << endl;
	  ic += 2;
	  I1 = 1 / pow (2, I1);
	  if (I1)
		{
		  ssa_g.CL.push_back (
							   {
							   I1, true}
		  );
		  ssa_g.CL.push_back (
							   {
							   I1, true}
		  );
		}
	}
  wr.close ();
}

int
main ()
{
  ofstream wr (path + "debug.txt");
  ifstream f (path + "text.txt");
  if (!f.is_open ())
	{
	  cerr << "Error opening the file." << endl;
	  return 1;					
	}

  vector < struct rule >db;
  string line;
  long nR = 0;

  while (getline (f, line))
	{
	  istringstream iss (line);

	  struct rule tmp;
	  tmp.d.resize (5);

	  char delimiter;
	  uint32_t first, second;

	  int dim = 0;
	  while (iss >> first >> delimiter >> second)
		  tmp.d[dim++] = { first, second };

	  tmp.lit.insert (nR++);
	  db.push_back (tmp);
	}

  f.close ();

  wr << "For the input textfile\n";
  INTERSECTIONS (db, wr);


  struct ssa_utility ssa_g;
  vector < long >s1, s2, s3;

  ssa_g.nf = nR;
  ssa_g.table.resize (ssa_g.nf);

  PRE_SSA (ssa_g, db, "text_int.txt");
  SSA (ssa_g, s1, s2, s3);

  wr << "\nafter SSA-2\nset1: " << s1.size () << " rules \nset2: " << s2.
	size () << " rules \n";
  wr << "set3(any set): " << s3.size () << " rules\n";

  writeVectorToFile (s1, path + "set1.txt");
  writeVectorToFile (s2, path + "set2.txt");
  writeVectorToFile (s3, path + "set3.txt");

  struct ssa_utility ssa_s1, ssa_s2;
  ssa_s1.nf = s1.size ();
  ssa_s1.table.resize (ssa_s1.nf);
  ssa_s2.nf = s2.size ();
  ssa_s2.table.resize (ssa_s2.nf);

  vector < struct rule >set1db, set2db;
  ruledb (set1db, path + "set1.txt");
  wr << "\nIn set1:\n";
  INTERSECTIONS (set1db, wr);
  PRE_SSA (ssa_s1, set1db, "s1_int.txt");

  ruledb (set2db, path + "set2.txt");
  wr << "\nIn set2:\n";
  INTERSECTIONS (set2db, wr);
  PRE_SSA (ssa_s2, set2db, "s2_int.txt");

  wr.close ();
  cout << "Program executed successfully :)\n";
  return 0;
}
