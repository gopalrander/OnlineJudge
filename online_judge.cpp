#include<iostream>
#include<fstream>
#include<stack>
#include<cstring>
#include<map>
#include<queue>
#include<climits>
#include<set>
#include<algorithm>
#include<cstdio>
#include<cmath>

using namespace std;

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
  vector<int> u_list;  // vector of user_id those who have solved this problem
  int ns,nac,nu;     // ns= N submissions , nac = N correct attempts , nu= N users solved
  double f;         // f = frequency at which this problem is solved
  //............
};
struct unode
{
  string name;
  double rating;
  double hi_rating;
  int p_cnt;
  vector<int> p_list;
  vector< pair<double,int> > r_list;
  int lptr,rptr;
  int start_time,lst_time;
  int ns,nac;
};

vector< pnode > problems;
vector< unode > users;
int k=5;        // 2*k nearest users for recommendations.
int month=2;   // period for best coder analysis.


//----------------------------------------------------------

void prec()
{
  cout<<"Loading problems:--------- \n";
  // Loading Problems

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


  fin.close();
  cout<<problems.size()<<" Problems loaded\n\n";
  // problems loaded

  // Now loading users
  cout<<"Now Loading users:----------- \n";

  fin.open ("UserDB.txt");
  int nusers;
  unode unow;

  fin>>nusers;

  cout<<nusers<<endl;
  for(int i=0;i<nusers;i++)
    {
      fin>>unow.name>>unow.rating>>unow.hi_rating;
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
      users.push_back(unow);
    }

  fin.close();

  cout<<users.size()<<" Users Loaded\n";
}

void postc()
{
  // write back problems
  cout<<"Writing back problems-------\n";

  fstream fout;

  fout.open ("ProblemDB.txt");
  fout<<problems.size()<<"\n";
  for(int i=0;i<problems.size();i++)
    {
      fout<<problems[i].name<<" ";
      fout<<problems[i].tag_cnt<<" ";
      for(int j=0;j<problems[i].tag_cnt;j++)
        fout<<problems[i].tag_list[j]<<" ";
      fout<<problems[i].start_time<<" "<<problems[i].lst_time<<" ";
      fout<<problems[i].rating<<" ";
      fout<<problems[i].u_cnt<<" ";
      for(int j=0;j<problems[i].u_cnt;j++)
        fout<<problems[i].u_list[j]<<" ";
      fout<<problems[i].ns<<" "<<problems[i].nac<<" "<<problems[i].nu<<" ";
      fout<<problems[i].f<<" ";
      fout<<"\n";
    }
  fout.close();
  cout<<"Writing back of problems Done\n";
  //....................

  //write back users
  cout<<"Writing back Users-------\n";

  fout.open ("UserDB.txt");

  fout<<users.size()<<"\n";
  for(int i=0;i<users.size();i++)
    {
      fout<<users[i].name<<" "<<users[i].rating<<" "<<users[i].hi_rating<<" ";
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
  now.u_cnt=0;
  problems.push_back(now);
}

void add_user()
{
  unode now;

  cout<<"Enter name of the user: ";
  cin>>now.name;
  now.rating=0;
  now.hi_rating=0;
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
  now.lptr=0;now.rptr=0;
  now.ns=0;
  now.nac=0;

  users.push_back(now);
}

bool copy_check(int p_id,int u_id)
{
  bool flag=true;
  for(int i=0;i<users[u_id].p_list.size();i++)
    if(users[u_id].p_list[i]==p_id)
      {flag=false;break;}
  return flag;
}

double normalize(double res)
{
  return ((double)0.75 + (res/5000) );
}

double cal_rating(int p_id,int u_id,int time)
{
  double res;
  double num=0,den=0;
  if (problems[p_id].u_list.size()==0)
    return (double)1;

  for(int i=0;i<problems[p_id].u_list.size();i++)
    {
      num+=(users[problems[p_id].u_list[i]].rating * users[problems[p_id].u_list[i]].rating);
      den+=(users[problems[p_id].u_list[i]].rating);
    }
  res=(num/den);
  res*=((problems[p_id].u_list.size())/(problems[p_id].u_list.size()+1)*(problems[p_id].u_list.size()+1));
  res*=((problems[p_id].lst_time+time)/(problems[p_id].lst_time));
  res*=(1.01-((problems[p_id].nac)/(problems[p_id].ns)));

  res=normalize(res);

  return res;
}

void query()
{
  int p_id,u_id,time,status;
  cout<<"Enter the query in form as shown below:\n";
  cout<<"\t <p_id> <u_id> <time> <status> : status( 0=AC  1={WA , TLE} )\n\n";
  cin>>p_id>>u_id>>time>>status;

  if (p_id >= problems.size() || u_id >= users.size())
    {
        cout<<"Invalid user or problem code. Retry!\n";
        return;
    }

  if(status==0 && copy_check(p_id,u_id))
    {   cout<<"Normal"<<endl;
      double p_,deltap;
      p_ = (problems[p_id].rating * cal_rating(p_id,u_id,time));
      deltap = (p_ - problems[p_id].rating);

      for(int i=0;i<problems[p_id].u_list.size();i++)
	{
	  users[problems[p_id].u_list[i]].rating += deltap;
	}
      users[u_id].rating+=p_;
      (problems[p_id].u_list).push_back(u_id);
      problems[p_id].rating=p_;

      // pending user updates
      users[u_id].p_cnt++;
      (users[u_id].p_list).push_back(p_id);
      users[u_id].lst_time=max(users[u_id].lst_time+time,problems[p_id].lst_time);
      users[u_id].ns++;
      users[u_id].nac++;
      users[u_id].r_list.push_back(make_pair(users[u_id].rating,users[u_id].lst_time));
      users[u_id].rptr++;
      users[u_id].hi_rating=max(users[u_id].hi_rating,users[u_id].rating);


      // pending problem updates
      problems[p_id].lst_time=max(problems[p_id].lst_time,users[u_id].lst_time);
      problems[p_id].u_cnt++;
      problems[p_id].ns++;
      problems[p_id].nac++;
      problems[p_id].nu++;
      problems[p_id].f= (double)((double)problems[p_id].nu/(double)(problems[p_id].lst_time-problems[p_id].start_time));
      cout<<"Your query is processed!! Cheers !!!\n\n";
    }
  else if(copy_check(p_id,u_id))
    {
      // updating userDB
      users[u_id].ns++; // no. of submissions increased by one

      // updating problemDB
      problems[p_id].ns++; // no. of submissions increased by one
      cout<<"Your query is processed ....Cheers !!!\n\n";
    }
  else
    {   cout<<"Copy query...  Invalid input\n";
    }
  return;
}

void rec1(int u_id,set<int> st,int &p1,int &p2)
{
  map<string,int> mp;
  vector<string> tags;
  string now;
  vector< pair<int,string> > vec;
  vector<int> p_level(problems.size(),0);
  vector< pair<int,int> > res_vec;

  for(int i=0;i<users[u_id].p_list.size();i++)
    {
      for(int j=0;j<problems[users[u_id].p_list[i]].tag_list.size();j++)
      {
	  now = problems[users[u_id].p_list[i]].tag_list[j];
	  if(mp[now]==0)
	    {
	      tags.push_back(now);
	      mp[now]=1;
	    }
	  else
	    mp[now]++;
	  }
    }
  for(int i=0;i<tags.size();i++)
    {
      vec.push_back(make_pair(mp[tags[i]],tags[i]));
    }
  sort(vec.rbegin(),vec.rend());
  // now vec contains the priortized list of tags

  for(int i=0;i<problems.size();i++)
    {
      if(st.count(i)!=0)
            continue;
      for(int j=0;j<vec.size();j++)
	{
	  bool flag=false;
	  for(int k=0;k<problems[i].tag_list.size();k++)
	    if(problems[i].tag_list[k]==vec[j].second)
	      {flag=true;break;}
	  if(flag)
	    p_level[i]++;
	  else
	    break;
	}
    }

  for(int i=0;i<problems.size();i++)
    if(st.count(i)==0)
            res_vec.push_back(make_pair(p_level[i],i));

  sort(res_vec.rbegin(),res_vec.rend());

  if(res_vec.size()>0)
        p1=res_vec[0].second;
    else p1 =-1;
    if(res_vec.size()>1)
        p2=res_vec[1].second;
    else p2 = -1;
  return;
}

double aggr(vector<int> u_list)
{
  double num=0,den=0;

  for(int i=0;i<u_list.size();i++)
    {
      num+=( (double)(users[u_list[i]].rating)*(double)(users[u_list[i]].rating) );
      den+=(double)(users[u_list[i]].rating);
    }
  return (num/den);
}

void rec2(int u_id,set<int> st,int &p1,int &p2)
{
  vector< pair<double,int> > vec;
  double aggr_val;
  pair<double,int> pr;

  for(int i=0;i<problems.size();i++)
    {
      if(st.count(i)!=0)
        continue;
      aggr_val=aggr(problems[i].u_list);
      vec.push_back(make_pair(abs(aggr_val-users[u_id].rating),i));     //taking ditance.
    }

  sort(vec.rbegin(),vec.rend());
    if(vec.size()>0)
        p1=vec[0].second;
    else p1 = -1;

    if(vec.size()>1)
        p2=vec[1].second;
    else p2 = -1;

  return;
}
void rec3(int u_id,set<int> st,int &p1,int &p2)
{
  vector< pair<double,int> > vec;
  int pos=-1;
  int lpos,rpos;
  vector<int> u_rel; // list of relevant users
  vector<int> p_rel;
  map<int,int> mp;
  vector< pair<int,int> > res_vec;

  for(int i=0;i<users.size();i++)
    vec.push_back(make_pair(users[i].rating,i));

  sort(vec.rbegin(),vec.rend());

  for(int i=0;i<users.size();i++)
    if(vec[i].second==u_id)
      {pos=i;break;}

  lpos=max(0,pos-k);
  rpos=min((int)users.size()-1,pos+k);

  for(int i=lpos;i<pos;i++)
    u_rel.push_back(vec[i].second);
    for(int i=pos+1;i<=rpos;i++)
    u_rel.push_back(vec[i].second);

    for(int i=0;i<u_rel.size();i++)
      {
	for(int j=0;j<users[u_rel[i]].p_list.size();j++)
	  {
	    if(mp[users[u_rel[i]].p_list[j]]==0)
	      {
		mp[users[u_rel[i]].p_list[j]]=1;
		p_rel.push_back(users[u_rel[i]].p_list[j]);
	      }
	    else
	      {
		mp[users[u_rel[i]].p_list[j]]++;
	      }
	  }
      }

    for(int i=0;i<p_rel.size();i++)
      { if (st.count(p_rel[i])!= 0)
            continue;
        res_vec.push_back(make_pair(mp[p_rel[i]],p_rel[i]));
      }
    sort(res_vec.rbegin(),res_vec.rend());
    if(res_vec.size()>0)
        p1=res_vec[0].second;
    else p1 =-1;
    if(res_vec.size()>1)
        p2=res_vec[1].second;
    else p2 = -1;
    return;
}

void get_top_from_last(vector<int> &res)
{
  int tmx;
  vector< pair<double,int> > vec;
  double deltar;

  for(int i=0;i<users.size();i++)
    tmx=max(tmx,users[i].lst_time);
  for(int i=0;i<problems.size();i++)
    tmx=max(tmx,problems[i].lst_time);

  for(int i=0;i<users.size();i++)
    {
      while(users[i].r_list[users[i].lptr].second<tmx-month)
        users[i].lptr++;

      if(users[i].lptr<= users[i].rptr)
	deltar=((users[i].r_list[users[i].rptr].first)-(users[i].r_list[users[i].lptr].first));
      else
	deltar=0;
      vec.push_back(make_pair(deltar,i));
    }

  sort(vec.rbegin(),vec.rend());

  for(int i=0;i<min((int)users.size(),5);i++)
    {
      res.push_back(vec[i].second);
    }

  return;
}

void compare_users(int u_id1,int u_id2)
{
  cout<<"\t"<<"User2"<<"\t\t\t"<<"User2"<<"\n";
  cout<<"Name:\n";
  cout<<"\t"<<users[u_id1].name<<"\t\t\t"<<users[u_id2].name<<"\n";
  cout<<"Rating:\n";
  cout<<"\t"<<users[u_id1].rating<<"\t\t\t"<<users[u_id2].rating<<"\n";
  cout<<"Higest_Rating_ever_obtained:\n";
  cout<<"\t"<<users[u_id1].hi_rating<<"\t\t\t"<<users[u_id2].hi_rating<<"\n";
  cout<<"No. of Problems solved:\n";
  cout<<"\t"<<users[u_id1].p_list.size()<<"\t\t\t"<<users[u_id2].p_list.size()<<"\n";
  cout<<"Accuracy:\n";
  cout<<"\t"<<((double)users[u_id1].nac/(double)users[u_id1].ns)<<"\t\t\t"<<((double)users[u_id2].nac/(double)users[u_id2].nac)<<"\n";
  cout<<"Joining Time:\n";
  cout<<"\t"<<users[u_id1].start_time<<"\t\t\t"<<users[u_id2].start_time<<"\n";
  cout<<"Last Time when active:\n";
  cout<<"\t"<<users[u_id1].lst_time<<"\t\t\t"<<users[u_id2].lst_time<<"\n";
  cout<<"Frequency of Problem Solving:\n";
  cout<<"\t"<<((double)users[u_id1].ns)/((double)((users[u_id1].lst_time)-(users[u_id1].start_time)))<<"\t\t\t"<<((double)users[u_id2].ns)/(double)((users[u_id2].lst_time)-(users[u_id2].start_time))<<"\n";



  //-------------user1
  vector<string> vec1;
  map<string,int> mp1;
  vector< pair<int,string> > res_vec1;

  for(int i=0;i<users[u_id1].p_list.size();i++)
    {
      for(int j=0;j<problems[users[u_id1].p_list[i]].tag_list.size();j++)
	{
	  if(mp1[problems[users[u_id1].p_list[i]].tag_list[j]]==0)
	    {
	      mp1[problems[users[u_id1].p_list[i]].tag_list[j]]=1;
	      vec1.push_back(problems[users[u_id1].p_list[i]].tag_list[j]);
	    }
	  else
	    {
	      mp1[problems[users[u_id1].p_list[i]].tag_list[j]]++;
	    }
	}
    }
  for(int i=0;i<vec1.size();i++)
    res_vec1.push_back(make_pair(mp1[vec1[i]],vec1[i]));

  sort(res_vec1.rbegin(),res_vec1.rend());

  //----------- user2
  vector<string> vec2;
  map<string,int> mp2;
  vector< pair<int,string> > res_vec2;

  for(int i=0;i<users[u_id2].p_list.size();i++)
    {
      for(int j=0;j<problems[users[u_id2].p_list[i]].tag_list.size();j++)
	{
	  if(mp2[problems[users[u_id2].p_list[i]].tag_list[j]]==0)
	    {
	      mp2[problems[users[u_id2].p_list[i]].tag_list[j]]=1;
	      vec2.push_back(problems[users[u_id2].p_list[i]].tag_list[j]);
	    }
	  else
	    {
	      mp2[problems[users[u_id2].p_list[i]].tag_list[j]]++;
	    }
	}
    }
  for(int i=0;i<vec2.size();i++)
    res_vec2.push_back(make_pair(mp2[vec2[i]],vec2[i]));

  sort(res_vec2.rbegin(),res_vec2.rend());

  //-------------
  cout<<"Tag associated with problems being solved (in decreasing order)\n\t";

  for(int i=0;i<min(5,(int)vec1.size());i++)
    cout<<res_vec1[i].second<<" ";
  cout<<"\t\t\t";
  for(int i=0;i<min(5,(int)vec2.size());i++)
    cout<<res_vec2[i].second<<" ";
    cout<<"\n";
    return;
}

void a_tools()
{
  int n;
  cout<<"Enter one of the options: \n";
  cout<<"1)Get me top 6 recommended problems for a particular user: \n";
  cout<<"2)Get me top 5 coders of last month \n";
  cout<<"3)Compare two coder's performance statistics for me \n";
    cin>>n;
  if(n==1)
    {
      int u_id;
      int p1,p2;
      set<int> st;
      vector<int> rec_problems;

      cout<<"Enter the user_id for which the recommendations are needed:\n";
      cin>>u_id;

      for(int i=0;i<users[u_id].p_list.size();i++)
        st.insert(users[u_id].p_list[i]);

      rec1(u_id,st,p1,p2);
      //cout<<"--"<<p1<<" "<<p2<<endl;
      //.........................
      if (p1!=-1) {
        rec_problems.push_back(p1);
        st.insert(p1);
      }
      if (p2!=-1) {
        rec_problems.push_back(p2);
        st.insert(p2);
      }
      //.........................

      rec2(u_id,st,p1,p2);
      //cout<<"--"<<p1<<" "<<p2<<endl;
      //.........................
      if (p1!=-1) {
        rec_problems.push_back(p1);
        st.insert(p1);
      }
      if (p2!=-1) {
        rec_problems.push_back(p2);
        st.insert(p2);
      }
      //.........................
      rec3(u_id,st,p1,p2);
      //cout<<"--"<<p1<<" "<<p2<<endl;
      //.........................
      if (p1!=-1) {
        rec_problems.push_back(p1);
        st.insert(p1);
      }
      if (p2!=-1) {
        rec_problems.push_back(p2);
        st.insert(p2);
      }
      //.........................

      cout<<"So, We have got U 6 problems which we would like to recomend U :) \n";
      for(int i=0;i<rec_problems.size();i++)
	cout<<rec_problems[i]<<" ";
      cout<<"\n";
    }
  if(n==2)
    {
      vector<int> res;
      get_top_from_last(res);
      cout<<"User_ID\t\t User_Name\n";
      for (int i=0; i<(int)res.size();i++)
        cout<<res[i]<<"\t\t"<<users[res[i]].name<<"\n";
    }
  if(n==3)
    {
      int u_id1,u_id2;
      cout<<"Eneter two users to compare the performance statistics of: ";
      cin>>u_id1>>u_id2;
      compare_users(u_id1,u_id2);
    }


return;
}

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
    {   cout<<"===============================================================\n";
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
