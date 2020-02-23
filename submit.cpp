#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>


using namespace std;



int line = 1;
int lineoffset = 10;
// changed lineoffset for now
ifstream fin;

string getToken()
{
 char ch;
 string val;
   while(!fin.eof())
     {
       fin.get(ch);
       if(isdigit(ch) || isalpha(ch))
              {
                     lineoffset++;
	             val.append(1,ch);
              }
        else if(ch == '\n')
              {
                     line++;
                     if(!val.empty()) return val;
              }
        else if(ch == ' ')
              { 
                     lineoffset++;
                     if(!val.empty()) return val;       
              }
     
    }
}


	
int main() 
{
   string filename;
   cout << "Enter Filename: " << endl;
   cin >> filename;
   char ch;
   int count = 0;
   string words;
   fin.open(filename.c_str());

   int defcount;
   
   // string s;
   string ds;
   string out;
   
          
               string s = getToken();
               cout << s << endl; 
               s = getToken();
               cout << s << endl;
               
            
            
              
         // is_open brace
} // main brace	
