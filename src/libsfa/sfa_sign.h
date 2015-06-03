#ifndef SFA_SIGN_H
#define SFA_SIGN_H

#include <string>
#include <vector>
#include "sfa.h"

using namespace std;
// type definitions
#define SFA_NULL '\0'
#define SFA_STRING '\1'
#define SFA_NUMBER '\2'
#define SFA_TRUE '\3'
#define SFA_FALSE '\4'
#define SFA_OBJECT '\5'
#define SFA_ARRAY '\6'
#define SFA_EMPTY '\7'


class sfa_sign;
class sfa_arr {
public:
	typedef std::vector<sfa_sign*>::iterator iterator;
	sfa_arr(int type) : _type(type) {}
	sfa_arr() :_type(SFA_NULL) {}
	~sfa_arr();
	sfa_arr::iterator begin();
	sfa_arr::iterator end();
	int type();
	void set_type(int type);
	// apis 
	int size();
	bool empty();
	void push_back(sfa_sign* sign);
	void clear();
	sfa_sign* at(int index);
	// apis for object

	// apis for array
private:
	std::vector<sfa_sign*> _items;
	int _type;
};

inline sfa_arr::~sfa_arr() {
	for (vector<sfa_sign*>::iterator i = _items.begin(); i != _items.end(); ++i){
		delete *i;
		*i = 0;
	}
	_items.clear();
}

inline int sfa_arr::type() {
	return _type;
}

inline void sfa_arr::set_type(int type){
	_type = type;
}

inline bool sfa_arr::empty() {
	if (size() == 0)
		return true;
	else
		return false;
}
inline int sfa_arr::size() {
	return _items.size();
}
inline sfa_arr::iterator sfa_arr::begin() {
	return _items.begin();
}
inline sfa_arr::iterator sfa_arr::end() {
	return _items.end();
}
inline void sfa_arr::push_back(sfa_sign* sign) {
	_items.push_back(sign);
}
inline sfa_sign* sfa_arr::at(int index) {
	return _items[index];
}

class sfa_value {
public:
	typedef sfa_maps::iterator iterator;
	sfa_value() :_termblock(0), _type(SFA_NULL) {}
	sfa_value(wstring SFA_string) :_termblock(0), _type(SFA_STRING), _string(SFA_string) {}
	sfa_value(float SFA_number) :_termblock(0), _type(SFA_NUMBER), _number(SFA_number) {}
	sfa_value(bool SFA_bool) :_termblock(0) { _type = SFA_bool ? SFA_TRUE : SFA_FALSE; }
	sfa_value::iterator begin();
	sfa_value::iterator end();
	int type();
	void set_type(int type);
	// apis
	wstring as_string();
	void set_string(wstring string);
	float as_number();
	void set_number(float number);
	bool as_bool();
	void set_bool(bool boolvalue);
	bool empty();
	int size();
	void clear();
	void append_literals(wstring literals);
	void append_atom_sign(wstring siginifer, string siginified);
	void append_map(int pos, wstring siginifer, string siginified);
private:
	wstring _string;
	float _number;
	int _type;
	sfa_block _termblock;
};
inline void sfa_value::set_type(int type) {
	switch (type) {
	case SFA_NUMBER:
	case SFA_STRING:
	case SFA_TRUE:
	case SFA_FALSE:
	case SFA_NULL:
		_type = type;
	default:
		_type = SFA_NULL;
	}
}
inline int sfa_value::size() {
	return _termblock.size();
}
inline bool sfa_value::empty() {
	if (size() == 0)
		return true;
	else
		return false;
}
inline void sfa_value::clear() {
	_number = 0;
	_string = L"";
	_termblock.clear();
}
inline wstring sfa_value::as_string() {
	return _string;
}
inline void sfa_value::set_string(wstring str) {
	_string = str;
	_termblock.clear();
	_type = SFA_STRING;
}
inline float sfa_value::as_number() {
	return _number;
}
inline void sfa_value::set_number(float num) {
	_number = num;
	_termblock.clear();
	_type = SFA_NUMBER;
}
inline bool sfa_value::as_bool() {
	return _type == SFA_TRUE ? true : false;
}
inline void sfa_value::set_bool(bool boolvalue) {
	_termblock.clear();
	_type = boolvalue == true ? SFA_TRUE : SFA_FALSE;
}
inline int sfa_value::type() {
	return _type;
}
inline sfa_value::iterator sfa_value::begin() {
	return _termblock.maps().begin();
}
inline sfa_value::iterator sfa_value::end() {
	return _termblock.maps().end();
}
inline void sfa_value::append_literals(wstring literals) {
	if (_type == SFA_STRING) {
		_string.append(literals);
	}
}
inline void sfa_value::append_atom_sign(wstring text, string term) {
	if (_type == SFA_STRING) {
		int startpos = _string.size();
		sfa_map map = { text, startpos, term };
		_termblock.push_back(map);
		_string.append(text);
	}
}
inline void sfa_value::append_map(int pos, wstring text, string term) {
	if (_type == SFA_STRING) {
		sfa_map map = { text, pos, term };
		_termblock.push_back(map);
	}
}

class sfa_name {
public:
	typedef sfa_maps::iterator iterator;
	sfa_name() :_termblock(0) {}
	sfa_name::iterator begin();
	sfa_name::iterator end();
	// apis
	wstring name_str() const;
	void set_name_str(wstring denotationstr);
	void append_literals(wstring literals);
	void append_atom_sign(wstring text, string term);
	void append_map(int pos, wstring text, string term);
	int size();
	bool empty();
	void clear();
private:
	wstring _name_str;
	sfa_block _termblock;
};
inline int sfa_name::size() {
	return _termblock.size();
}
inline bool sfa_name::empty() {
	if (size() == 0)
		return true;
	else
		return false;
}
inline void sfa_name::clear() {
	_termblock.clear();
}
inline sfa_name::iterator sfa_name::begin() {
	return _termblock.maps().begin();
}
inline sfa_name::iterator sfa_name::end() {
	return _termblock.maps().end();
}
inline wstring sfa_name::name_str() const {
	return _name_str;
}
inline void sfa_name::set_name_str(wstring denotationstr) {
	_name_str = denotationstr;
}
inline void sfa_name::append_literals(wstring literals) {
	_name_str.append(literals);
}
inline void sfa_name::append_atom_sign(wstring text, string term) {
	int startpos = _name_str.size();
	sfa_map map = { text, startpos, term };
	_termblock.push_back(map);
	_name_str.append(text);
}
inline void sfa_name::append_map(int pos, wstring text, string term) {
	sfa_map map = { text, pos, term };
	_termblock.push_back(map);
}

class sfa_sign {
public:
	// iterators
	typedef sfa_name::iterator name_iterator;
	typedef sfa_value::iterator value_iterator;
	typedef sfa_arr::iterator iterator;
	// construct and deconstruct
	sfa_sign(int type);
	// construct with different reification types
	sfa_sign(wstring name_str, wstring value_str);
	sfa_sign(wstring name_str, float value_num);
	sfa_sign(wstring name_str, bool value_bool);
	~sfa_sign();
	// common apis
	int type();
	void set_type(int type);
	// common iterator apis		

	// denotation apis
	sfa_sign::name_iterator name_begin();
	sfa_sign::name_iterator name_end();
	wstring name_str() const;
	void set_name_str(wstring name_str);
	int name_size();
	bool name_empty();
	void name_clear();
	void name_append_literals(wstring literals);
	void name_append_atom_sign(wstring signifier, string signified);
	void name_append_map(int pos, wstring signifier, string signified);
	// reification apis
	sfa_sign::value_iterator value_begin();
	sfa_sign::value_iterator value_end();
	wstring value_as_string();
	void set_value_string(wstring str);
	float value_as_number();
	void set_value_number(float num);
	bool value_as_bool();
	void set_value_bool(bool bl);
	int value_size();
	bool value_empty();
	void value_clear();
	void value_append_literals(wstring literals);
	void value_append_atom_sign(wstring signifier, string signified);
	void value_append_map(int pos, wstring signifier, string signified);
	// connotation apis		
	sfa_sign::iterator begin();
	sfa_sign::iterator end();
	void push_back(sfa_sign* sign);
	int size();
	bool empty();
	void clear();
	sfa_sign* at(int index);
	// to parse apis in syntax s
	int s_start();
	int s_end();
	int s_size();
	void set_s_start(int start);
	void set_s_end(int end);
	// for values
	int v_start();
	int v_end();
	int v_size();
	void set_v_start(int start);
	void set_v_end(int end);
	// for parent link and childen
	sfa_sign* parent();
	sfa_sign* next();
	sfa_sign* next_start();
	void set_parent(sfa_sign* parent);
private:
	int _type;
	// for syntax parse
	// for key
	int _s_start;
	int _s_end;
	// for value
	int _v_start;
	int _v_end;
	// parent link
	sfa_sign* _parent;
	// next link
	sfa_sign* _next_sign;
	sfa_sign::iterator _next;
	string _id;
	sfa_name _name;
	sfa_value _value;
	sfa_arr _arr;
};
inline int sfa_sign::type() {
	return _type;
}
// to parse apis in syntax s
inline sfa_sign* sfa_sign::parent() {
	return _parent;
}
inline void sfa_sign::set_parent(sfa_sign* parent) {
	_parent = parent;
}
// for key
inline int sfa_sign::s_start() {
	return _s_start;
}
inline int sfa_sign::s_end() {
	return _s_end;
}
inline int sfa_sign::s_size() {
	return _s_end - _s_start;
}
inline void sfa_sign::set_s_start(int start) {
	_s_start = start;
}
inline void sfa_sign::set_s_end(int end) {
	_s_end = end;
}
// for value
inline int sfa_sign::v_start() {
	return _v_start;
}
inline int sfa_sign::v_end() {
	return _v_end;
}
inline int sfa_sign::v_size() {
	return _v_end - _v_start;
}
inline void sfa_sign::set_v_start(int start) {
	_v_start = start;
}
inline void sfa_sign::set_v_end(int end) {
	_v_end = end;
}

// common iterator apis
inline sfa_sign::name_iterator sfa_sign::name_begin() {
	return _name.begin();
}
inline sfa_sign::name_iterator sfa_sign::name_end() {
	return _name.end();
}
inline sfa_sign::value_iterator sfa_sign::value_begin() {
	return _value.begin();
}
inline sfa_sign::value_iterator sfa_sign::value_end() {
	return _value.end();
}
inline sfa_sign::iterator sfa_sign::begin() {
	return _arr.begin();
}
inline sfa_sign::iterator sfa_sign::end() {
	return _arr.end();
}
// denotation apis
inline wstring sfa_sign::name_str() const{
	return _name.name_str();
}
inline void sfa_sign::set_name_str(wstring name_str) {
	_name.set_name_str(name_str);
}
inline int sfa_sign::name_size() {
	return _name.size();
}
inline bool sfa_sign::name_empty() {
	return _name.empty();
}
inline void sfa_sign::name_clear() {
	_name.clear();
}
inline void sfa_sign::name_append_literals(wstring literals) {
	_name.append_literals(literals);
}
inline void sfa_sign::name_append_atom_sign(wstring signifier, string signified) {
	_name.append_atom_sign(signifier, signified);
}
inline void sfa_sign::name_append_map(int pos, wstring signifier, string signified) {
	// covert to denotation position
	pos -= _s_start;
	if (_name.name_str().substr(pos, signifier.length()) == signifier)
		_name.append_map(pos, signifier, signified);
}
// reification apis
inline wstring sfa_sign::value_as_string() {
	return _value.as_string();
}
inline void sfa_sign::set_value_string(wstring str) {
	_value.set_string(str);
}
inline float sfa_sign::value_as_number() {
	return _value.as_number();
}
inline void sfa_sign::set_value_number(float num) {
	_value.set_number(num);
}
inline bool sfa_sign::value_as_bool() {
	return _value.as_bool();
}
inline void sfa_sign::set_value_bool(bool bl) {
	_value.set_bool(bl);
}
inline int sfa_sign::value_size() {
	return _value.size();
}
inline bool sfa_sign::value_empty() {
	return _value.empty();
}
inline void sfa_sign::value_clear() {
	_value.clear();
}
inline void sfa_sign::value_append_literals(wstring literals) {
	_value.append_literals(literals);
}
inline void sfa_sign::value_append_atom_sign(wstring signifier, string signified) {
	_value.append_atom_sign(signifier, signified);
}
inline void sfa_sign::value_append_map(int pos, wstring signifier, string signified) {
	// covert to reification position
	pos -= _v_start;
	if (_type == SFA_STRING && _value.as_string().substr(pos, signifier.length()) == signifier)
		_value.append_map(pos, signifier, signified);
}
// connotation apis		
inline void sfa_sign::push_back(sfa_sign* sign) {
	sign->set_parent(this);
	_arr.push_back(sign);
}
inline int sfa_sign::size() {
	return _arr.size();
}
inline bool sfa_sign::empty() {
	return _arr.empty();
}
inline void sfa_sign::clear() {
	_arr.clear();
}
inline sfa_sign* sfa_sign::at(int index) {
	return _arr.at(index);
}


#endif