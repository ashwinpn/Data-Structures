#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

ifstream in;

map<string, int> m;
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


string getToken()
{
  string token;
  char ch;
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
            line = line + 1;
            lastline = lineoffset;
            lineoffset = 0;
            return token;
        }
    }
 line = line - 1;
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
  cout << tok << endl;
  if (!isdigit(tok[0]))
    {
      cout << "NUM_EXPECTED" << endl;
// exits are messy, using return is cleaner
      return 0;
    }

   // cout << "I resist" << endl;

  // deflist => Contains S, R
  defCount = stoi(tok);
  cout << defCount << endl;
 // define macro for "for"
  if(defCount > 16)
    {
        cout << "TOO_MANY_DEF_IN_MODULE" << endl;
    }
  for(int i = 0; i < defCount; i=i+1)
   {
      string tok = getToken();
      if(tok.length() > 16)
        {
           cout << "SYM_LEN_TOO_LONG" << endl;
        }
      for (int j = 0; j < tok.length(); j=j+1)
        {
           if(!isalpha(tok[j]))
              {
                cout << "SYM_EXPECTED" << endl;
                return 0;
              }
        }
      
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
                cout << "SYM_EXPECTED" << endl;
                return 0;
              }
        }
   int RA;
   RA = stoi(relAddr);
   if(m[tok])
   {
     cout << "Need to output that syybol table is already in use" << endl;
   }

  
 }
 
// Main Brace
}
