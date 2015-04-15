#ifndef CONEX_MODEL_TERM_H
#define CONEX_MODEL_TERM_H

#include "Concept.h"
#include <vector>
#include <string>

namespace conex {
	class Term : public Concept {
	public:
		Term();
		Term(std::string iid);
		virtual ~Term();
		wstring indexWord();
		void setIndexWord(wstring indexword);
		string pos();
		void setPos(string pos);
		string state();
		void setState(string state);
	private:
		wstring _indexWord;
		string _pos;
		string _state;
	};

	inline string Term::pos() {
		return _pos;
	}
	inline void Term::setPos(string pos) {
		_pos = pos;
	}
	inline string Term::state() {
		return _state;
	}
	inline void Term::setState(string state) {
		_state = state;
	}
}
#endif