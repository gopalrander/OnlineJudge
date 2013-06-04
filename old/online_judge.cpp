#include<iostream>
#include<stack>
#include<cstring>
#include<map>
#include<queue>
#include<climits>
#include<set>
#include<algorithm>
#include<cstdio>
#include <fstream>
#include<cmath>

using namespace std;

#define month 10

struct pnode
{
  string name;
  int tag_cnt;
  vector<string> tag_list;
  int start_time;
  int lst_time;
  double rating; // rating of the problem
  //...........
  int u_cnt;
  vector<double> u_list;  // vector of user_id those who have solved this problem
  int ns,nac,nu;     // ns= N submissions , nac = N correct attempts , nu= N users solved
  double f;         // f = frequency at which this problem is solved
  //............
};
struct unode
{
  string name;
  double rating;
  int p_cnt;
  vector<int> p_list;
  vector< pair<double,int> > r_list;
  int lptr,rptr;
  int start_time,lst_time;
  int ns,nac;
};

vector< pnode > problems;
vector< unode > users;

void prec()
{
  cout<<"Loading problems:--------- \n";
  // Loading Problems
  //freopen ("ProblemDB.txt","r",fin);
  ifstream fin;
  fin.open ("ProblemDB.txt");
  int nprobs;
  fin>>nprobs;
  pnode now;

  for(int i=0;i<nprobs;i++)
    {
      fin>>now.name;
      fin>>now.tag_cnt;
      now.tag_list.resize(now.tag_cnt);
      for(int j=0;j<now.tag_cnt;j++)
        fin>>now.tag_list[j];
      fin>>now.start_time>>now.lst_time;
      fin>>now.rating;
      fin>>now.u_cnt;
      now.u_list.resize(now.u_cnt);
      for(int j=0;j<now.u_cnt;j++)
        fin>>now.u_list[j];
      fin>>now.ns>>now.nac>>now.nu;
      fin>>now.f;
      problems.push_back(now);
    }
    //fflush(stdin);
    fin.close();
  cout<<problems.size()<<" problems loaded\n\n";
  // problems loaded

  // Now loading users
  cout<<"Now Loading users:----------- \n";
  //freopen ("UserDB.txt","r",fin);
    fin.open ("UserDB.txt");
    int nusers;
    unode unow;

  fin>>nusers;
  for(int i=0;i<nusers;i++)
    {
      fin>>unow.name>>unow.rating;
      fin>>unow.p_cnt;
      unow.p_list.resize(unow.p_cnt);
      for(int j=0;j<unow.p_cnt;j++)
            fin>>unow.p_list[j];
      unow.r_list.resize(unow.p_cnt);
      for(int j=0;j<unow.p_cnt;j++)
        fin>>unow.r_list[j].first>>unow.r_list[j].second;
      fin>>unow.lptr>>unow.rptr;
      fin>>unow.start_time>>unow.lst_time;
      fin>>unow.ns>>unow.nac;
      users.push_back (unow);
    }

    fin.close();
    cout<<users.size()<<" users Loaded\n";
  //...........
}

void postc()
{
    ofstream fout, gout;
    cout<<"Writing back Users-------\n";
  //freopen ("UserDB.txt","w",stdout);
    fout.open ("UserDB.txt");

  fout<<users.size()<<"\n";
  for(int i=0;i<users.size();i++)
    {
      fout<<users[i].name<<" "<<users[i].rating<<" ";
      fout<<users[i].p_cnt<<" ";
      for(int j=0;j<users[i].p_cnt;j++)
        fout<<users[i].p_list[j]<<" ";
      for(int j=0;j<users[i].p_cnt;j++)
        fout<<users[i].r_list[j].first<<" "<<users[i].r_list[j].second<<" ";

      fout<<users[i].lptr<<" "<<users[i].rptr<<" ";
      fout<<users[i].start_time<<" "<<users[i].lst_time<<" ";
      fout<<users[i].ns<<" "<<users[i].nac<<" ";
      fout<<"\n";
    }
    fout.close();
    cout<<"Writing back of Users Done\n";
  //................
  // write back problems
  cout<<"Writing back problems-------\n";
  //freopen ("ProblemDB.txt","w",stdout);

  gout.open ("ProblemDB.txt");
  gout<<problems.size()<<"\n";
  for(int i=0;i<problems.size();i++)
    {
      gout<<problems[i].name<<" ";
      gout<<problems[i].tag_cnt<<" ";
      for(int j=0;j<problems[i].tag_cnt;j++)
        gout<<problems[i].tag_list[j]<<" ";
      gout<<problems[i].start_time<<" "<<problems[i].lst_time<<" ";
      gout<<problems[i].rating<<" ";
      gout<<problems[i].u_cnt<<" ";
      for(int j=0;j<problems[i].u_cnt;j++)
        gout<<problems[i].u_list[j]<<" ";
      gout<<problems[i].ns<<" "<<problems[i].nac<<" "<<problems[i].nu<<" ";
      gout<<problems[i].f<<" ";
      gout<<"\n";
    }
  gout.close();
  cout<<"Writing back of problems Done\n";
  //....................

  //write back users



}

void add_problem()
{
  pnode now;

  cout<<"Enter the name of the problem to be added in the judge: ";
  cin>>now.name;
  cout<<"Enter  Number of tags to be given to this problem ";
  cin>>now.tag_cnt;

  (now.tag_list).resize(now.tag_cnt);

  cout<<"Enter All tags: ";
  for(int i=0;i<now.tag_cnt;i++)
    cin>>now.tag_list[i];

  now.ns=0;now.nac=0;now.nu=0;
  now.f=(double)0;


  // for getting the start time of problem
  int tim=INT_MAX;
  for(int i=0;i<users.size();i++)
    tim=min(tim,users[i].lst_time);

  if(tim==INT_MAX)
    tim=0;
  //.....................................
  now.lst_time=tim;
  now.start_time=tim;
  now.rating=(double)3;

  problems.push_back(now);
  cout<<"Problem added sucessfully! \n\n";
}

void add_user()
{
  unode now;

  cout<<"Enter name of the user: ";
  cin>>now.name;
  now.rating=0;;
  now.lst_time=0;

  /// for getting start time of user
  int tim=0;
  for(int i=0;i<users.size();i++)
    tim=max(tim,users[i].lst_time);
  for(int i=0;i<problems.size();i++)
    tim=max(tim,problems[i].lst_time);
  //...............................
  now.start_time=tim;
  now.p_cnt=0;
  now.lst_time=tim;

  users.push_back(now);
  cout<<"User added sucessfully! \n\n";
}

void query()
{
  int p_id,u_id,time,status;
  cout<<"Enter the query in form as shown below:\n";
  cout<<"\t <p_id> <u_id> <time> <status> : status( 0=AC 1=TLE 2=WA )\n\n";
  cin>>p_id>>u_id>>time>>status;
    int newRating =0;
    //int factor = aggregate ();
}

void a_tools()
{}

void display_ratings()
{
  cout<<"\n";
  vector< pair<double,int> > now;
  for(int i=0;i<users.size();i++)
    {
      now.push_back(make_pair(users[i].rating,i));
    }
  sort(now.begin(),now.end());

  int uid;
  cout<<"Printing all users ratings:=======\n";
  cout<<"User_ID\tUser_name\t\tUser_Rating\n";
  for(int i=0;i<now.size();i++)
    {
      uid=now[i].second;
      cout<<uid<<"\t"<<users[uid].name<<"\t"<<now[i].first<<"\n";
    }
  cout<<"====================\n";
}


int main()
{
  // ... This is pre_processing
  prec();
  //.......

  //   Run the user menu
  int n;
  while(1)
    {
      cout<<"Choose one of the following options: \n";
      cout<<"1: Add a problem\n";
      cout<<"2: Add a User\n";
      cout<<"3: Make a problem solving attempt\n";
      cout<<"4: Use analysis tools:\n";
      cout<<"5: Well ,I would like to check ur data\n";
      cout<<"6: Display the ratings of all users\n";
      cout<<"7: Done.. I would like to go offline\n";

      cin>>n;

      if(n==1)
	{
	  add_problem();
	}
      else if(n==2)
	{
	  add_user();
	}
      else if(n==3)
	{
	  query();
	}
      else if(n==4)
	{
	  a_tools();
	}
      else if(n==5)
	{
	  postc();
	}
      else if (n==6)
	{
	  display_ratings();
	}
      else
	{break;}
    }

  postc();
  cout<<"\n\nWell Leaving with a hope of meeting U again: :D \n\n";

  return 0;
}
