#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

ifstream in;

map<string, string> m;
map<string, int> use;
map<int, int> moduleSize;

int total;
int offset;
int indexAddr;
int moduleindex = 1;
int lastline = 0;
int line = 1;
int lineoffset = 0;

const char* convert(string str)
{
  return str.c_str();
}

int ReturnNum(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (s.at(i) == '*' || s.at(i) == '/')
			return atoi(s.substr(0, i).c_str());
	}
	return atoi(s.c_str());
}

string getToken()
{
  string token = "";
  char ch = '\0';
  while(!in.eof())
    {
      in.get(ch);
      if(ch == ' ')
        {
          lineoffset = lineoffset + 1;
          return token;
        }
      else if(isdigit(ch) || isalpha(ch))
        {
           lineoffset = lineoffset + 1;
           token = token + ch;
        }
      else if(ch=='\n')
        {
            if(in.eof()) line = line - 1;
            line = line + 1;
            lastline = lineoffset;
            lineoffset = 0;
            return token;
        }
    }
 
 return "0";
}



int main()
{
  bool mode = true;
  int tokenCount = 1;
  int defCount = 0;
  // mode 0 is for debugging
  // mode 1 is for normal program run
  // ifstream in;
  string word;
  char ch;
  string filename;
  cout << "Enter Filename:" << endl;
  // getline(cin, filename);
  cin >> filename;
   // Pass one Starts
  in.open(convert(filename));
while(!in.eof())
{
  string tok;
  tok = getToken();
  /*
  if (in.is_open() && mode == false) {
            while (in >> word) {
              cout << word << endl;
              in.get(ch);
    }
*/
  //Testing! -> 
  //cout << tok << endl;
  if (!isdigit(tok[0]))
    {
      cout << "NUM_EXPECTED" << endl;
// exits are messy, using return is cleaner
      // return 0;
    }

   // cout << "I resist" << endl;

  // deflist => Contains S, R
  defCount = atoi(convert(tok));
  // cout << defCount << endl;
 // define macro for "for"
	  if(defCount > 16)
	    {
		cout << "TOO_MANY_DEF_IN_MODULE" << endl;
	    }

  for(int i = 0; i < defCount; i=i+1)
   {
      string tok2 = getToken();
	      if(tok2.length() > 16)
		{
		   cout << "SYM_LEN_TOO_LONG" << endl;
		}
		   // if(!isalpha(tok2[j]))
                  if (!((tok2.at(0) >= 'a' && tok2.at(0) <= 'z')
			|| ((tok2.at(0) >= 'A' && tok2.at(0) <= 'Z'))))
		      {
		        cout << "SYM_EXPECTED Line 127" << endl;
		        // return 0;
		      }
		

/*

for (int j = 0; j < tok2.length(); j=j+1)
		{
		   // if(!isalpha(tok2[j]))
                  if (!((tok2.at(0) >= 'a' && tok2.at(0) <= 'z')
			|| ((tok2.at(0) >= 'A' && tok2.at(0) <= 'Z'))))
		      {
		        cout << "SYM_EXPECTED Line 127" << endl;
		        // return 0;
		      }
		}

*/

      
   string relAddr;
   relAddr = getToken();
	   if(relAddr.empty())   
	   {
		cout << "ADDR_EXPECTED" << endl;
	   }
	   for (int j = 0; j < relAddr.length(); j=j+1)
		{
		   if(!isdigit(relAddr[j]))
		      {
		        cout << "SYM_EXPECTED Line 142" << endl;
		       //  return 0;
		      }
		}
   // cout << "Line 146 relAddr : " << relAddr << endl;
   int RA;
   RA = atoi(convert(relAddr));
   /*
   if(m[tok2]!=" ")
   {
     cout << "Need to output that syybol table is already in use" << endl;
   }
  */
   map<string, string>::iterator iter = m.find(tok2);
	   if(iter != m.end())
	     {
		 m[tok2] = iter->second + "*";
	     }
	   else
	     {
		m[tok2] = to_string(offset + RA) + "/" + to_string(moduleindex);
	     }
   // cout << "This is token" << endl;
   // cout << tok2 << endl;
  
 

}
 // Use List
  string usea;
  usea = getToken();
  // cout << "Line 173 usea : " << usea << endl; 
  int usecount;
  usecount = atoi(convert(usea));
	  if(usecount > 16)
	    {
	       cout << "TOO_MANY_USE_IN_MODULE" << endl;
	      // return 0;
	    }
for(int i = 0; i < usecount; i=i+1)
	   {
	      string uset = getToken();
	      if(uset.length() > 16)
		{
		   cout << "SYM_LEN_TOO_LONG" << endl;
		}
	      
		  //  if(!isalpha(uset[j]))
                  if (!((uset.at(0) >= 'a' && uset.at(0) <= 'z')
			|| ((uset.at(0) >= 'A' && uset.at(0) <= 'Z'))))
		      {
		        cout << "SYM_EXPECTED LINE 192" << endl;
		        // return 0;
		      }
		
/*
for (int j = 0; j < uset.length(); j=j+1)
		{
		  //  if(!isalpha(uset[j]))
                  if (!((uset.at(0) >= 'a' && uset.at(0) <= 'z')
			|| ((uset.at(0) >= 'A' && uset.at(0) <= 'Z'))))
		      {
		        cout << "SYM_EXPECTED LINE 192" << endl;
		        // return 0;
		      }
		}
*/

	   }

 string cde;
 cde = getToken();
 // cout << "Line 200 cde : " << cde << endl; 
 int codecount;
 codecount = atoi(convert(cde));
 offset = offset + codecount;
		 if(offset > 512)
		  {
		    cout << "TOO_MANY_INSTRUCTIONS" << endl;
		  }
string cod;
string add = "AEIR";
int check = 0;
	  for (int k = 0; k < codecount; k = k + 1)
	   {
	      cod = getToken();
	      for (int j = 0; j < add.length(); j = j + 1)
	       {
		 if(cod[0] == add[j])
		       {
		         check = check + 1;
		       }
	       }
	     if(!check) 
	       {
		  cout << "ADDR_EXPECTED" << endl;
	       }
	      cod = getToken();
	   }    


 moduleindex = moduleindex + 1;
 moduleSize[moduleindex] = codecount;
 map<string, string>::iterator it2 = m.begin();
 

	for(; it2!=m.end(); it2++)
	  {
	   //  int temp = 0;
	    int temp = ReturnNum(it2->second);
	    if(temp >= offset)
	      {
		  cout << "Module too big assume 0 relative" << endl;
		  m[it2->first] = offset - codecount;
	      }
	  }


}

// while brace here
total = offset - 1;
offset = 0;
moduleindex = 1;


// end of pass one

cout << "Symbol Table: " << endl;

map<string, string>::iterator it3 = m.begin();
	for (; it3 != m.end(); ++it3) 
        {
		cout << it3->first << "=" << ReturnNum(it3->second);
               
		//string::size_type idx = it3->second.find("*");
		//if (idx != string::npos) {
		//	cout << " Symbol Table Already in sed " << endl;
		//}
		cout << endl;
                // cout << "I am here!" << endl;
	}


// start pass two
 
cout << "Memory Map: " << endl;

// Main Brace
}
