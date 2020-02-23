#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>

using namespace std;
// symbol table
map<string, string> m;
// rule 9. offset of each module
map<int, int> moduleSize;
//rule 7, used in pass two
map<string, bool> usedOrNot;

int total;
int offset;
int indexAddr;
int moduleIndex = 1;
int lastline = 0;
int line = 1;
int lineoffset = 0;
ifstream fin;

int ReturnNum(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (s.at(i) == '*' || s.at(i) == '/')
			return atoi(s.substr(0, i).c_str());
	}
	return atoi(s.c_str());
}
void ParseError(int errcode, int len = 0) {
	static char* errstr[] = { "NUM_EXPECTED", // Number expect
			"SYM_EXPECTED", // Symbol Expected
			"ADDR_EXPECTED", // Addressing Expected
			"SYM_TOLONG", // Symbol Name is to long
			"TO_MANY_DEF_IN_MODULE", // > 16
			"TO_MANY_USE_IN_MODULE", //>16
			"TO_MANY_INSTR" }; // total num_instr exceeds memory size (512)
	int first = lineoffset == 0 ? line - 1 : line;
	int second = lineoffset == 0 ? lastline : lineoffset;
	second = second - len + 1;
	if (errcode >= 4 && errcode <= 6)
		printf("Parse Error line %d offset %d: %s\n", first, 1,
				errstr[errcode]);
	else
		printf("Parse Error line %d offset %d: %s\n", first, second,
				errstr[errcode]);
}

string PrintOutEW(int rule, string symName, int moduleNum, int symOffset,
		int moduleSize) {
	string res;
	switch (rule) {
	case 2:
		res =
				"Error: This variable is multiple times defined; first value used";
		break;
	case 3:
		res = "Error: " + symName + " is not defined; zero used";
		break;
	case 4:
		res = "Warning: Module " + to_string(moduleNum) + ": " + symName
				+ " was defined but never used\n";
		break;
	case 5:
		res = "Warning: Module " + to_string(moduleNum) + ": " + symName
				+ " to big " + to_string(symOffset) + " (max="
				+ to_string(moduleSize) + ") assume zero relative\n";
		break;
	case 6:
		res =
				"Error: External address exceeds length of uselist; treated as immediate";
		break;
	case 7:
		res = "Warning: Module " + to_string(moduleNum) + ": " + symName
				+ " appeared in the uselist but was not actually used\n";
		break;
	case 8:
		res = "Error: Absolute address exceeds machine size; zero used";
		break;
	case 9:
		res = "Error: Relative address exceeds module size; zero used";
		break;
	case 10:
		res = "Error: Illegal immediate value; treated as 9999";
		break;
	case 11:
		res = "Error: Illegal opcode; treated as 9999";
		break;
	}
	return res;
}
string GetWord() {
	string res;
	char c;
	while (!fin.eof()) {
		fin.get(c);
		if (c == '\n') {
			if (fin.eof())
				line--;
			line++;
//			cout << line << endl;
			lastline = lineoffset;
			lineoffset = 0;
                      
                        
			if (res.length() != 0 && res != "") {
				return res;
			}
                         	} 
                      else if (c == ' ' || c == '\t') {
			lineoffset++;
                        
			if (res.length() != 0 && res != "") {
				return res;
                           }
                        
                           
			
		} 
        /*
                 
         else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
				|| (c >= '0' && c <= '9')) {
			lineoffset++;
			res += c;
		}
         */
                
       
         else if(isdigit(c) || isalpha(c))
            {
              lineoffset++;
	      res += c;
            }
         
	}
	return "$";
}

void CheckSym(string s, int arg = 0) {
	if (s == "$")
		return;
	if (!((s.at(0) >= 'a' && s.at(0) <= 'z')
			|| ((s.at(0) >= 'A' && s.at(0) <= 'Z')))) {
		ParseError(1, s.length() - arg);
		exit(1);
	}
	if (s.length() > 16) {
		ParseError(3, s.length() - arg);
		exit(1);
	}
}

	
int main() {
        string filename;
        cout << "Enter Filename:" << endl;
       // getline(cin, filename);
       cin >> filename;
   // Pass one Starts
	fin.open(filename.c_str());
        while (!fin.eof()) {
		string s = GetWord();
               // cout << "And we go:" << s << endl;
                // cout << "Here we go:" << s << endl;
		// definition list
               //  cout << "Line 176" << endl;
		 // int defcount = stoi(s);
                int defcount = atoi(s.c_str());
                // cout << "Def " << defcount << endl; 
		if (defcount > 16) 
                 {
                        if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << 1 << ": " << "TOO_MANY_DEF_IN_MODULE" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << 1 << ": " << "TOO_MANY_DEF_IN_MODULE" << endl;
                         }
			exit(1);
		}
		for (int i = 0; i < defcount; i++) {
			//symbol
			string S = GetWord();
			// check symbol
			CheckSym(S, -1);
// Relative address
			string tmp = GetWord();
                        
			if (tmp == "" || tmp.length() == 0) {
				if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << lastline + 1 << ": " << "ADDR_EXPECTED" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << lineoffset + 1 << ": " << "ADDR_EXPECTED" << endl;
                         }
				exit(1);
			} else if (tmp.find_first_not_of("1234567890") != string::npos) {
				ParseError(0, tmp.length());
				exit(1);
			}
                        
                        // cout << "Line 207" << endl;
			int R = stoi(tmp);
                        // int R = atoi(tmp.c_str());
			map<string, string>::iterator iter = m.find(S);
			if (iter!=m.end()) {
				// rule 2 multiple definition
				m[S] = iter->second + "*";
			} else {
				m[S] = to_string(offset + R) + "/" + to_string(moduleIndex);
			}
		}

		// use list
		s = GetWord();
                // cout << "Line 221" << endl;
		//  int usecount = stoi(s);
                 int usecount = atoi(s.c_str());
		if (usecount > 16) {

                 if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << 1 << ": " << "TOO_MANY_USE_IN_MODULE" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << 1 << ": " << "TOO_MANY_USE_IN_MODULE" << endl;
                         }
			exit(1);
		}
		for (int i = 0; i < usecount; i++) {
			s = GetWord();
			CheckSym(s);
		}

		//text
		s = GetWord();
                // cout << "Line 241" << endl;
		//  int codecount = stoi(s);
                int codecount = atoi(s.c_str());
		offset += codecount;
		if (offset > 512) {
			if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << 1 << ": " << "TOO_MANY_INSTR" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << 1 << ": " << "TOO_MANY_INSTR" << endl;
                         }
			exit(0);
		}
		for (int i = 0; i < codecount; i++) {
			s = GetWord();
			if (s != "I" && s != "A" && s != "R" && s != "E") {
			 if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << lastline + 1 << ": " << "ADDR_EXPECTED" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << lineoffset + 1 << ": " << "ADDR_EXPECTED" << endl;
                         }  
                         exit(1);
			}
			s = GetWord();
		}

		moduleSize[moduleIndex++] = codecount;
		//map<string, string>::iterator it2 = m.begin();
		//for (; it2 != m.end(); it2++) {
                for (auto const& it2 : m) {
			int temp = ReturnNum(it2.second);
			if (temp >= offset) {
				cout
						<< PrintOutEW(5, it2.first, moduleIndex - 1, temp,
								codecount - 1);
				m[it2.first] = offset - codecount;
			}
		}
	}
	total = --offset;
	offset = 0;
	moduleIndex = 1;


	cout << "Symbol Table" << endl;
	// map<string, string>::iterator it = m.begin();
	for (auto const& it : m) {
		cout << it.first << "=" << ReturnNum(it.second);
		string::size_type idx = it.second.find("*");
		if (idx != string::npos) {
			cout << " " << PrintOutEW(2, "", 0, 0, 0);
		}
		cout << endl;
	}
	cout << "\n" << "Memory Map" << endl;
	return 0;
}

