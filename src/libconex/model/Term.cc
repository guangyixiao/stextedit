#include "Term.h"

namespace conex {
	Term::Term():_indexWord(L"") {
	}

	Term::Term(std::string iid): Concept(iid) {
	}

	Term::~Term() {
	}

	wstring Term::indexWord() {
		if ( _indexWord.size() > 0) 
			return _indexWord;
		else
			return id()->word();
	}

	void Term::setIndexWord(wstring indexword) {
		_indexWord = indexword;
	}
}