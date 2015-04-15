#include "TermControl.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "ternary_tree/structured_map.hpp"

namespace conex {
	
	void TermControl::SetWord(wstring word, Term* term) {
		//to do index
		term->id()->setWord(word);
	}

	string TermControl::changePos(string pos, string yearToHex, string iid) {		
		return _voc->changeIidForPos(iid, pos, yearToHex);		
	}

	void TermControl::SetPos(string pos, Term* term) {
		//to do index
		if(pos != term->pos()) {
			term->setPos(pos);
		}
	}

	string TermControl::stateIndexToStr(int index) {
		switch(index) {
		case 0:
			return "sdf:waiting";
		case 1:
			return "sdf:using";
		case 2:
			return "sdf:editing";
		case 3:
			return "sdf:outdating";
		default:
			return "sdf:using";
		}
	}
	string TermControl::posIndexToStr(int index) {
		switch(index) {
		case 0:
			return "raw";
		case 1:
			return "ncm";
		case 2:
			return "npp";
		case 3:
			return "ngp";
		case 4:
			return "nop";
		case 5:
			return "adj";
		case 6:
			return "adv";
		case 7:
			return "vtr";
		case 8:
			return "vid";
		case 9:
			return "vdi";
		case 10:
			return "phr";
		case 11:
			return "dif";
		default:
			return "raw";
		}
	}	

	int TermControl::posToViewIndex(string pos) {
		if(pos == "raw")
			return 0;
		else if(pos == "ncm")
			return 1;
		else if(pos == "npp")
			return 2;
		else if(pos == "ngp")
			return 3;
		else if(pos == "nop")
			return 4;
		else if(pos == "adj")
			return 5;
		else if(pos == "adv")
			return 6;
		else if(pos == "vtr")
			return 7;
		else if(pos == "vit")
			return 8;
		else if(pos == "vdi")
			return 9;
		else if(pos == "phr")
			return 10;
		else if(pos == "dif")
			return 11;
		else
			return 0;
	}

	int TermControl::stateToViewIndex(string pos) {
		if(pos == "sdf:waiting")
			return 0;
		else if(pos == "sdf:using")
			return 1;
		else if(pos == "sdf:editing")
			return 2;
		else if(pos == "sdf:outdating")
			return 3;		
		else
			return 0;
	}

	void TermControl::SetPonetenics(string ponetenics, Term* term) {
		//to do index
		term->setPhonetic(ponetenics);
	}

	void TermControl::SetTextExplanation(wstring textExp, Term* term) {
		//to do index
		term->setText(textExp);
	}

	void TermControl::SetIidExplanation(string  iidExp, Term* term) {
		//to do index
		term->structure()->setIids(iidExp);
	}


	// how to change the term with a different pos
	void TermControl::SaveTerm(Term* term, string pos, string yearToHex) {
		string oid = term->id()->iid();
		if(term->pos() != pos || yearToHex != oid.substr(4,4)) {
			_voc->changeIidForPos(term, pos, yearToHex);
		}
		map<string, Term*>::iterator it = _modifyTerms.find(oid);
		if(it != _modifyTerms.end()) {
			it->second = term;
		}else {
			_modifyTerms[term->id()->iid()] = term;
		}		
		IndexTerm(term);
	}

	void TermControl::IndexVoc() {
		if(_voc) {
			map<string, Term*> terms = _voc->GetTerms();
			for(map<string, Term*>::iterator it = terms.begin(); it != terms.end(); ++it ) {
				IndexTerm(it->second);
			}
		}
	}

	void TermControl::ClearVocIndexes() {
		_termmap.clear();
	}

	void TermControl::Clear() {
		ClearVocIndexes();
		_modifyTerms.clear();
	}

	void TermControl::ClearModifyTerms() {
		_modifyTerms.clear();
	}

	void TermControl::GetModifyTerms(vector<Term*>& terms) {
		for(map<string, Term*>::iterator it = _modifyTerms.begin(); it != _modifyTerms.end(); ++it) {
			terms.push_back(it->second);
		}
	}

	void TermControl::GetRelationTerms(Term* term, Type type, vector<Term*>& out) {
		vector<string> iids;
		term->context()->GetRelationTerms(type, iids);
		for(vector<string>::iterator it = iids.begin(); it != iids.end(); ++it) {
			out.push_back(_voc->GetTermByIid(it->c_str()));
		}
	}

	void TermControl::SplitText(wstring text, vector<wstring>& words) {
		const static wstring delimiters = L"\uFF0C\uFF1B\u6216\u3001 ";
		size_t current;
		size_t next = -1;
		do
		{
			current = next + 1;
			next = text.find_first_of( delimiters, current );
			if( next != current) {
				wstring word = text.substr( current, next - current );
				words.push_back(word);
			}
		}
		while (next != string::npos);
	}

	void TermControl::SplitText(Term* term, vector<wstring>& words) {
		SplitText(term->id()->word(), words);
	}

	wstring TermControl::TopWord(Term* term) {
		wstring text = term->id()->word();
		cout << "lang in top word :" << _voc->lang() << endl;
		if (_voc->lang().compare("chs") == 0) {
			vector<wstring> words;
			SplitText(text, words);
			return words.begin()->c_str();
		}else {
			return text;
		}
	}

	void TermControl::tolower(wstring& data) {
		transform(data.begin(), data.end(), data.begin(), ::tolower);
	}

	void TermControl::IndexTerm(Term* term) {
		if(term) {
			wstring text = term->id()->word();
			if(_voc->lang().compare("eng") == 0)
			    tolower(text);
			if(!text.empty() && text.compare(L" ") != 0) {
				if( _voc->lang().compare("eng") == 0) {
					pair<wstring, Term*> termPair(text, term);
					//cout << "text :" << text << endl;
					try {
						_termmap.insert(termPair);
					}catch(...) {
						wcerr << "insert error for term of " << text << endl;
					}
				}else {
					vector<wstring> words;
					SplitText(text, words);
					for(vector<wstring>::iterator it = words.begin(); it != words.end(); ++it) {
						pair<wstring, Term*> termPair(*it, term);
						//cout << "text :" << text << endl;
						try {
							_termmap.insert(termPair);
						}catch(...) {
							wcerr << "insert error for term of " << text << endl;
						}
					}
				}
			}else {
				pair<wstring, Term*> pair(L"##", term);
				_termmap.insert(pair);
			}
		}
	}

	void TermControl::RemoveTermIndex(Term* term) {
		wstring prefix  = term->id()->word();
		if(_voc == 0)
			return;
		if (_voc->lang().compare("eng") == 0 )
		    tolower(prefix);
		IterPair p = _termmap.prefix_range(prefix);
		while(p.first != p.second) {
			if((*p.first).second == term) {
				_termmap.erase(p.first);
				return ;
			}
			p.first++;
		}
	}

	void TermControl::PrefixSearch(wstring prefix, vector<Term*>& out) {
		if(_voc == 0)
			return;
		if (_voc->lang().compare("eng") == 0 )
		    tolower(prefix);
		IterPair p = _termmap.prefix_range(prefix);
		while(p.first != p.second) {
			Term* term  = (*p.first).second;
			wstring word = (*p.first).first;
			if(term) {
				if(_voc->lang().compare("chs") == 0)
				    term->setIndexWord(word);
				out.push_back( term);
			}
			p.first++;			
		}
	}

	Term* TermControl::GetTermByIid(string id) {
		return _voc->GetTermByIid(id);
	}

	int TermControl::WordsSize() {
		int n = 0;
		for( StringTermMap::iterator i = _termmap.begin(); i != _termmap.end(); i = _termmap.upper_bound(i->first))
			++n;
		return n;
	}

	/*int TermControl::WordsSize() {
		
	}*/
}