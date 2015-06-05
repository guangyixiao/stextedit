#ifndef XPM_CONTROL_XPM_SDF_SIGN_CONTROL_H
#define XPM_CONTROL_XPM_SDF_SIGN_CONTROL_H

#include <vector>
#include <algorithm>
#include <string>
#include "sfa.h"
#include "sfa_sign.h"


#define SFA_ERROR_INVAL -1;
#define SFA_ERROR_PART_OBJECT_ARRAY -2;
#define SFA_ERROR_PART_STRING -3
#define SFA_ERROR_PART_PRIMITIVE -4
#define SFA_ERROR_PART_KEY -5;

using namespace std;

class sfa_parser {
public:
	sfa_parser() : _toksuper(0), _toklast(0), _pos(0), _count(0) {}
	// two input with syntax and doc, one output with sfa_sign
	int parse(const wstring& syntax, sfa_model& doc, sfa_sign& r);
	void clear();
	int parse_atom_signs(sfa_model& doc, sfa_sign& r);
private:
	int parse_string(const wstring& syntax);
	int parse_primitive(const wstring& syntax);	
	int _count;
	unsigned int _pos;
	sfa_sign* _toksuper;
	sfa_sign* _toklast;
};

inline void sfa_parser::clear() {
	_toksuper = 0;
	_toklast = 0;
	_pos = 0;
	_count = 0;
}

#endif