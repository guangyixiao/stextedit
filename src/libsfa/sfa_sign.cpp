#include "sfa_sign.h"


sfa_sign::sfa_sign(int type) : _s_start(-1),
_s_end(-1),
_v_start(-1),
_v_end(-1),
_parent(0),
_next_sign(0){
	set_type(type);
}
void sfa_sign::set_type(int type) {
	_type = type;
	_value.set_type(type);
	_arr.set_type(type);
	_next = end();
}
// construct with different reification types
sfa_sign::sfa_sign(wstring de_str, wstring rei_str) : _s_start(-1),
_s_end(-1),
_v_start(-1),
_v_end(-1),
_parent(0),
_next_sign(0){
	_value.set_type(SFA_STRING);
	_name.set_de_str(de_str);
	_value.set_string(rei_str);
	_next = end();
}
sfa_sign::sfa_sign(wstring de_str, float rei_num) : _s_start(-1),
_s_end(-1),
_v_start(-1),
_v_end(-1),
_parent(0),
_next_sign(0){
	_value.set_type(SFA_NUMBER);
	_name.set_de_str(de_str);
	_value.set_number(rei_num);
	_next = end();
}
sfa_sign::sfa_sign(wstring de_str, bool rei_bool) : _s_start(-1),
_s_end(-1),
_v_start(-1),
_v_end(-1),
_parent(0),
_next_sign(0) {
	int type = rei_bool == true ? SFA_TRUE : SFA_FALSE;
	_value.set_type(type);
	_name.set_de_str(de_str);
	_value.set_bool(rei_bool);
	_next = end();
}
sfa_sign* sfa_sign::next() {
	/* queit condition for recursive */
	if (_next == end()){
		return 0;
	}
	sfa_sign* r = 0;
	if (_next_sign == *_next) {
		r = (*_next)->next_start();
	}
	else {
		r = (*_next)->next();
	}
	if (r != 0) {
		_next_sign = r;
		return r;
	}
	else {
		++_next;
		_next_sign = *_next;
		return _next == end() ? 0 : *_next;
	}

}

sfa_sign* sfa_sign::next_start() {
	if (empty())
		return 0;
	else {
		_next = begin();
		_next_sign = *begin();
		return _next_sign;
	}
}

sfa_sign::~sfa_sign() {

}