#ifndef CONEX_MODEL_VOCABULARY_H
#define CONEX_MODEL_VOCABULARY_H

#include "Concept.h"
#include <map>
#include <vector>
#include <string>


#define SAFE_DELETE(x) if(x){delete x; x = NULL;}

namespace conex {
	using namespace std;
	class SdfEncoder;
	class Term;
	class Vocabulary : public Concept {
	public:
		Vocabulary();
		~Vocabulary();
		//term management
		std::map<std::string, Term*>& GetTerms();
		void GetTopkTerms(int k, std::vector<Term*>& tms);
		std::string CreateIidStr(int pos);
		// this will create a empty term with a new iid
		Term* CreateTerm();
		// this will add a term with the given iid
		Term* CreateTerm(std::string iid);
		Term* GetTermByIid(std::string iid);
		int RemoveTermByIid(std::string iid);
		int ClearTerms();
		std::string lang();
		void setLang(std::string lang);
		std::string changeIidForPos(std::string iid, string pos, string yearToHex);
		void changeIidForPos(Term* term, string pos, string yearToHex);
		//properties
		string tag();
		void setTag(string tag);
		string sui();
		void setSui(string sui);
		string author();
		void setAuthor(string author);
		string time();
		void setTime(string time);
		string vid();
		void setVid(string vid);
		char posToIndex(string pos);
	private:		
		std::map<std::string, Term*> terms;	
		SdfEncoder* _coder;
		std::string _lang;
		string _tag;
		string _sui;
		string _author;
		string _time;
	};
	inline string Vocabulary::vid() {
		return id()->iid();
	}
	inline void Vocabulary::setVid(string vid) {
		id()->setIid(vid);
	}
	inline std::string Vocabulary::lang() {
		return _lang;
	}
	inline void Vocabulary::setLang(std::string lang) {
		_lang = lang;
	}
	inline std::string Vocabulary::tag() {
		return _tag;
	}
	inline void Vocabulary::setTag(std::string tag) {
		_tag = tag;
	}
	inline std::string Vocabulary::sui() {
		return _sui;
	}
	inline void Vocabulary::setSui(std::string sui) {
		_sui = sui;
	}
	inline std::string Vocabulary::author() {
		return _author;
	}
	inline void Vocabulary::setAuthor(std::string author) {
		_author = author;
	}
	inline std::string Vocabulary::time() {
		return _time;
	}
	inline void Vocabulary::setTime(std::string time) {
		_time = time;
	}
	//term management
	
	inline std::map<std::string, Term*>& Vocabulary::GetTerms() {
		return terms;
	}	
}
#endif