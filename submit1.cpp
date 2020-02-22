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
map<string, int> symmap;
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


string OffsetFormat(int offset) {
	string res;
	if (offset <= 9) 
         {
                res = res + "00" + to_string(offset);
	 } 
         else if (offset <= 99) 
        {
		res = res + "0" + to_string(offset);
	} 
         else res = res + to_string(offset);
   return res;
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


void AddressEntry(int usecount, string s, string r, string useList[]) {
	if (s == "R") {
		if (atoi(s.c_str()) > 9999) {
			cout << OffsetFormat(indexAddr) << ": " << "9999 ";
			cout << "Error: Illegal opcode; treated as 9999" << endl;
			return;
		}
		string tmp;
                tmp = r[1] + r[2] + r[3];
		int address = atoi(tmp.c_str());
		// rule 9

		map<int, int>::iterator it = moduleSize.find(moduleIndex);
//		cout << moduleIndex<<" "<<it->second << endl;
		if (address > it->second) {
			cout << OffsetFormat(indexAddr) << ": " << r[0]
					<< OffsetFormat(offset) << " ";
			cout << "Error: Relative address exceeds module size; zero used" << endl;
		} 
else 
                {
			address += offset;
			cout << OffsetFormat(indexAddr) << ": " << r[0]
					<< OffsetFormat(address) << endl;
		}
	} else if (s == "A") {
		// rule 8
		string tmp;
                tmp = r.substr(1,3); 
		int address = atoi(tmp.c_str());
		if (address > 512) {
			cout << OffsetFormat(indexAddr) << ": " << r[0] << "000 ";
			cout << "Error: Absolute address exceeds machine size; zero used" << endl;
		} else
			cout << OffsetFormat(indexAddr) << ": " << r << endl;
	} else if (s == "I") {
		if (r.length() > 4) {
			cout << OffsetFormat(indexAddr) << ": " << "9999 ";
			cout << "Error: Illegal immediate value; treated as 9999" << endl;
		} else {
			while(r.length() < 4)
                           {
                               r = "0" + r;
                           }
			cout << OffsetFormat(indexAddr) << ": " << r << endl;
		}
	} else if (s == "E") {
		string tmp;
                tmp = r[0];
		int optcode = atoi(tmp.c_str());
		tmp = r.substr(1,3);
		int operand = atoi(tmp.c_str());
		// rule 6

		if (operand >= usecount) 
                {
			cout << OffsetFormat(indexAddr) << ": " << r << " ";
			cout << "Error: External address exceeds length of uselist; treated as immediate" << endl;
		} 
                 else {
			// rule 7
			usedOrNot[useList[operand]] = true;

			map<string, string>::iterator it = m.find(useList[operand]);

			if (it == m.end()) {
				// rule 3
				cout << OffsetFormat(indexAddr) << ": " << optcode << "000 ";
				cout << "Error: " << useList[operand] << " is not defined; zero used" << endl;
			} else {
				cout << OffsetFormat(indexAddr) << ": " << optcode
						<< OffsetFormat(ReturnNum(it->second)) << endl;
			}
			// rule 7
			map<string, string>::iterator it2 = m.find(useList[operand]);
			if (it2 == m.end()) {

			} else {
				m[useList[operand]] = to_string(ReturnNum(m[useList[operand]]));
			}
		}
	}
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
        int temp = s.length() - arg;
	if (s == "$")
		return;
	if (!((s.at(0) >= 'a' && s.at(0) <= 'z')
			|| ((s.at(0) >= 'A' && s.at(0) <= 'Z')))) {
                if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << lastline - temp + 1 << ": " << "SYM_EXPECTED" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << lineoffset - temp + 1 << ": " << "SYM_EXPECTED" << endl;
                         }
		// ParseError(1, s.length() - arg);
		exit(1);
	}
	if (s.length() > 16) {
                if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << lastline + 1 << ": " << "SYM_TOO_LONG" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << lineoffset + 1 << ": " << "SYM_TOO_LONG" << endl;
                         }
		// ParseError(3, s.length() - arg);
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
                        if(lineoffset == 0)
                         {
                           cout << "Parse Error line " << line-1 << "offset " << lastline -tmp.length() + 1 << ": " << "NUM_EXPECTED" << endl;
                         }
                        else 
                         {
                           cout << "Parse Error line " << line << "offset " << lineoffset - tmp.length() + 1 << ": " << "NUM_EXPECTED" << endl;
                         }
			  //	ParseError(0, tmp.length());
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
                       cout << "printEW number 5" << endl;  
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
			cout << " " << "Error: This variable is multiple times defined; first value used";
		}
		cout << endl;
	}
	cout << "\n" << "Memory Map" << endl;
        fin.close();
        // pass Two begin
        fin.open(filename.c_str());
	// ifstream fin(path.c_str());
        int defcountt;
        int usecountt;
	string s;
	while (fin >> s) {
		// definition list
		defcountt = atoi(s.c_str());
		for (int i = 0; i < defcountt; i++) 
                {
			//symbol
			fin >> s;
			fin >> s;
		}

		// use list
		fin >> s;
		usecountt = atoi(s.c_str());
		string* useList = new string[usecountt];
		usedOrNot.clear();
		for (int i = 0; i < usecountt; i++) 
                {
			fin >> s;
			useList[i] = s;
			usedOrNot[s] = false;
		}

		fin >> s;
		int codecountt = atoi(s.c_str());
		for (int i = 0; i < codecountt; i++) 
                {
			fin >> s;
			string S = s;
			fin >> s;
			string R = s;
			AddressEntry(usecountt, S, R, useList);
			indexAddr++;
		}
		// rule 7
		map<string, bool>::iterator itt = usedOrNot.begin();
		for (; itt != usedOrNot.end(); ++itt) {
			if (itt->second == false)
				cout << PrintOutEW(7, itt->first, moduleIndex, 0, 0);
		}
		offset += codecountt;
		moduleIndex++;
	}
     


	return 0;

//End Brace Below
}

