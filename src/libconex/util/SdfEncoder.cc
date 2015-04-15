#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "SdfEncoder.h"

namespace conex {
	using namespace std;

	SdfEncoder::SdfEncoder(string sti_hex):_sti_hex(sti_hex)
	{
		if( sti_hex.size() == 14) 
			_sti = decoding_hex(sti_hex);
		else {
			_sti.pos = 0;
			_sti.tag = 80;
			_sti.seqs = 0;
			_sti.year = 0;
			_sti.yearTag = 0;
		}
		
	}

	string SdfEncoder::sti_hex(char pos) {
		if(_sti.year == 0) {
			time_t t;
			time(&t);
			struct tm* _tm = localtime(&t);
			_sti.year = 1900 + _tm->tm_year;			
		}
		_sti.pos = pos;
		++ _sti.seqs;
		return sti_hex(_sti);
		//tm _tm;
	}

	string SdfEncoder::sti_change_pos(string sti_str, char pos) {
		STI sti = decoding_hex(sti_str);
		sti.pos  = pos;
		return sti_hex(sti);
	}

	string SdfEncoder::sti_hex(STI sti) {
		stringstream ss;
		int first = sti.tag | sti.pos;
		string result = "";
		ss << "0x"
			<< std::setfill('0') << std::setw(1*2)
			<< std::hex << first;
		int year = sti.yearTag << 15 & sti.year;
		ss << std::setfill('0') << std::setw(2*2)
			<< std::hex << year;
		ss << std::setfill('0') << std::setw(3*2)
			<< std::hex << sti.seqs;
		return ss.str();
	}
	
		
	STI SdfEncoder::decoding_hex(const string & sti_hex) {
		STI sti = {0, 0, 0, 0, 80};
		if(sti_hex.size() == 14) {
			stringstream ss3;
			ss3 << sti_hex[3];
			int pos;
			ss3 >> pos;
			sti.pos = pos;
			string year = sti_hex.substr(4, 4);
			string seqs = sti_hex.substr(8, 6);
			stringstream ss;
			int year_int = 0;
			ss << std::hex << year_int;
			ss >> year;
			sti.year = year_int & 32767;
			sti.yearTag = year_int >> 15;
			stringstream ss2;
			ss2 << std::hex << seqs;
			ss2 >> sti.seqs;
		}
		return sti;
	}// return the date time from Iid
	
}