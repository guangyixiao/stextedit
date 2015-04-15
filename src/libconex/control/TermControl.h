#ifndef CONEX_CONTROL_TERM_CONTROL_H
#define CONEX_CONTROL_TERM_CONTROL_H

#include <string>
#include <stack>
#include "model/Vocabulary.h"
#include "model/Term.h"
#include "ternary_tree/structured_map.hpp"


namespace conex {
	using namespace std;

	typedef containers::structured_multimap<std::wstring, Term*> StringTermMap;
	typedef StringTermMap::iterator MapIter;
	typedef std::pair<MapIter, MapIter> IterPair;

	class TermControl {
	public:
		TermControl(Vocabulary* voc) : _voc(voc) { IndexVoc(); }
		TermControl():_voc(NULL) {}
		~TermControl() {Clear();}
		Term* CreateTermWithIid(string iid);
		Term* CreateTerm();
		void RemoveTerm(Term *);
		void SetVocabulary(Vocabulary* voc);
		void SetWord(wstring word, Term* term);
		void SetPos(string pos, Term* term);
		string changePos(string pos, string yearToHex, string iid);
		void SetPonetenics(string ponetenics, Term* term);
		void SetTextExplanation(wstring textExp, Term* term);
		void SetIidExplanation(string  iidExp, Term* term);
		void SaveTerm(Term* term, string pos, string yearToHex);
		void IndexVoc();
		void RemoveTermIndex(Term* term);
		void RemoveModifyTerm(Term* term);
		void Clear();
		void ClearModifyTerms();
		void ClearVocIndexes();
		void IndexTerm(Term* term);
		void PrefixSearch(wstring, vector<Term*>& out);
		void GetRelationTerms(Term* term, Type type, vector<Term*>& out);
		Term* GetTermByIid(string id);
		void GetModifyTerms(vector<Term*>& terms);
		void RemoveModifyTerms();
		int SizeofModified();
		void SplitText(Term* term, vector<wstring>& words);
		void SplitText(wstring text, vector<wstring>& words);
		wstring TopWord(Term* term);
		void tolower(wstring& text);
		int posToViewIndex(string pos);
		string posIndexToStr(int index);
		int stateToViewIndex(string state);
		string stateIndexToStr(int index);
		int WordsSize();
	private:		
		map<string, Term*> _modifyTerms;
		Vocabulary* _voc;	
		StringTermMap _termmap;
	};
	

	inline void TermControl::SetVocabulary(Vocabulary* voc) {
		if(_voc != voc) {
		    _voc = voc;
			ClearVocIndexes();
			_modifyTerms.clear();
			IndexVoc();
		}
	}

	inline Term* TermControl::CreateTermWithIid(string iid) {
		return _voc->CreateTerm(iid);
	}

	inline Term* TermControl::CreateTerm() {
		return _voc->CreateTerm();
	}

	

	inline void TermControl::RemoveTerm(Term* term) {
		if(term) {
			//_termmap[term->GetStructure()->GetText()] = NULL;
			//pair<string, Term*> termPair(term->GetStructure()->GetText(), term);
			//_termmap.erase(termPair);
			//remove the index
			RemoveTermIndex(term);
			//remove the modified map
			RemoveModifyTerm(term);			
			//remove in the voc
			_voc->RemoveTermByIid(term->id()->iid());
			term = NULL;
		}
	}

	inline void TermControl::RemoveModifyTerm(Term* term) {
		map<string, Term*>::iterator it = _modifyTerms.find(term->id()->iid());
		if(it != _modifyTerms.end()) {
			_modifyTerms.erase(it);
		}
	}

	inline void TermControl::RemoveModifyTerms() {
		_modifyTerms.clear();
	}

	inline int TermControl::SizeofModified() {
		return _modifyTerms.size();
	}
}
#endif