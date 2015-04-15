#ifndef CONEX_UTIL_SDF_ENCODER_H
#define CONEX_UTIL_SDF_ENCODER_H

#include <time.h>
#include <string>

namespace conex {
    using namespace std;

	struct STI{
		bool yearTag;
		int year;
		int seqs;
		char pos;
		char tag;
	};

	class SdfEncoder {
	public:
		SdfEncoder(string sti_hex = "");
		string sti_hex(char pos);
		string sti_change_pos(string sti_hex, char pos);
		STI decoding_hex(const string & sti_hex);   // return the date time from Iid
		string sti_hex(STI sti);
	private:
		
		string _sti_hex;
		STI _sti;
	};
}

#endif