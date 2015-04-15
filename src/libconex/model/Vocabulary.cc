#include "Vocabulary.h"
#include <algorithm>
#include "Term.h"
#include "util/SdfEncoder.h"

namespace conex {
	Vocabulary::Vocabulary():_coder(0){
		setIid("0x5107df000000");		
	}

	Vocabulary::~Vocabulary() {
		SAFE_DELETE(_coder);
		ClearTerms();
	}

	
	std::string Vocabulary::CreateIidStr(int pos) {
		if(_coder == 0) {
			string sti = "";
			if(terms.size() > 0) {
				sti = terms.rbegin()->first;				
			}
			_coder = new SdfEncoder(sti);
		}	
		return _coder->sti_hex(pos);
	}

	char Vocabulary::posToIndex(string pos) {
		if(pos == "raw")
			return '0';
		else if(pos == "ncm")
			return '1';
		else if(pos == "npp")
			return '2';
		else if(pos == "ngp")
			return '3';
		else if(pos == "nop")
			return '4';
		else if(pos == "adj")
			return '5';
		else if(pos == "adv")
			return '6';
		else if(pos == "vtr")
			return '7';
		else if(pos == "vit")
			return '8';
		else if(pos == "vdi")
			return '9';
		else if(pos == "phr")
			return 'e';
		else if(pos == "dif")
			return 'f';
		else
			return '0';
	}

	std::string Vocabulary::changeIidForPos(std::string iid, string pos, string yearToHex) {
		if( iid.size() != 14 )
			return iid;
		char xpos = posToIndex(pos);
		if(xpos != pos[3]) {
			iid[3] = xpos;
		}
		// can't garity the size is 4
		if(yearToHex.size() == 4 && yearToHex != iid.substr(4,4)) {
			iid.replace(4, 4, yearToHex);
		}
		return iid;
	}

	void Vocabulary::changeIidForPos(Term* term, string pos, string yearToHex) {
		string oid = term->id()->iid();
		string iid = changeIidForPos(oid, pos, yearToHex);
		if(iid != oid) {
			terms.erase(oid);
			terms[iid] = term;
			term->setPos(pos);
			term->setIid(iid);
		}
	}

	//term management
	Term* Vocabulary::CreateTerm() {
		char pos = 0;
		std::string siid = CreateIidStr(pos);
		Term* term = new Term(siid);
		term->setPos("sdf:raw");
		terms[siid] = term;
		return term;
	}

	//term management
	Term* Vocabulary::CreateTerm(std::string siid) {
		Term* term = new Term(siid);
		term->setPos("sdf:raw");
		terms[siid] = term;
		return term;
	}

	Term* Vocabulary::GetTermByIid(std::string iid) {
		Term* term = terms[iid];
		return term;
	}

	//term management
	int Vocabulary::RemoveTermByIid(std::string iid) {
		map<std::string, Term*>::iterator it = terms.find(iid);
		if(it != terms.end()) {
			Term* term = it->second;
		    delete term;
			term = NULL;
			terms.erase(it);
		}
		return true;
	}

	int Vocabulary::ClearTerms() {
		if(!terms.empty()) {
			for(std::map<std::string, Term*>::iterator it = terms.begin(); it != terms.end(); ++it) {
				Term* term = it->second;
				delete term;
				term = 0 ;
			}
		}
		terms.clear();
		return true;
	}	

	void Vocabulary::GetTopkTerms(int k, std::vector<Term*>& tms) {
		int count = 0;
		if(k == 0)
			k = terms.size();
		for(map<string, Term*>::iterator it = terms.begin(); it != terms.end()&&count != k; ++it, ++count) {
			tms.push_back(it->second);
		}
	}
}