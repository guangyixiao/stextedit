#ifndef XPM_CONTROL_XPM_SDF_SIGN_CONTROL_H
#define XPM_CONTROL_XPM_SDF_SIGN_CONTROL_H

#include <vector>
#include <algorithm>
#include <string>
#include "model/XpmTermDoc.h"
#include "model/sdf/SDFSign.h"

namespace xpm {
	
#define SDF_ERROR_INVAL -1;
#define SDF_ERROR_PART_OBJECT_ARRAY -2;
#define SDF_ERROR_PART_STRING -3
#define SDF_ERROR_PART_PRIMITIVE -4
#define SDF_ERROR_PART_KEY -5;

	using namespace std;
	class SDFSignControl {
	public:
		SDFSignControl(): _toksuper(0), _toklast(0),_pos(0),_count(0) {}
		int parse(const wstring& syntax, const XpmTermDoc& doc, SDFSign& r);	
		void clear();
	private:
		int parse_string(const wstring& syntax);
		int parse_primitive(const wstring& syntax);
		int parse_atom_signs(const XpmTermDoc& doc, SDFSign& r);
		int _count;
		unsigned int _pos;
		SDFSign* _toksuper;
		SDFSign* _toklast;
	};

	inline void SDFSignControl::clear() {
		_toksuper = 0;
		_toklast = 0;
		_pos = 0;
		_count = 0;
	}
}
#endif