#ifndef XPM_CONTROL_XPM_TABLE_DOC_CONTROL_H
#define XPM_CONTROL_XPM_TABLE_DOC_CONTROL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "sfa.h"
#include "control/TermFactory.h"

namespace xpm {
	using namespace conex;
	using namespace std;

	class XpmTableDocControl : public sfa {
	public:
		XpmTableDocControl(conex::TermFactory* termFactory);
		TermControl* termControl() ;
	private:
		TermFactory* _factory;
	};

	inline XpmTableDocControl::XpmTableDocControl(conex::TermFactory* termFactory):_factory(termFactory) {}

	inline TermControl* XpmTableDocControl::termControl() {
		string l = sfa::lang();
		cout << "current language :" << l << endl;
		return _factory->termControl(l);
	}
}
#endif