#ifndef CONEX_CONTROL_Term_Factory_H
#define CONEX_CONTROL_Term_Factory_H
#include <string>
#include <iostream>
#include "VocabularyXmlControl.h"
#include "TermControl.h"
#include "model/Vocabulary.h"


namespace conex {
	using namespace std;

	class TermFactory {
	public:
		TermFactory(string enfile = "", string chsfile = "", string enCls = "", string chsCls = "") : _enVocControl(conex::VocabularyControl::XML_SAX2), 
			_chsVocControl(conex::VocabularyControl::XML_SAX2),
			_enControl(0),
			_chsControl(0),
			_enVocClsControl(conex::VocabularyControl::XML_SAX2), 
			_chsVocClsControl(conex::VocabularyControl::XML_SAX2),
			_enClsControl(0),
			_chsClsControl(0)
		{
			setChsFile(chsfile);
			setEnFile(enfile);
			setEnClsFile(enCls);
			setChsClsFile(chsCls);
		}
		~TermFactory() {
			if ( _enControl != 0) {
				delete _enControl;
				_enControl = 0;
			}
			if ( _chsControl != 0) {
				delete _chsControl;
				_chsControl = 0;
			}
			if ( _enClsControl != 0) {
				delete _enClsControl;
				_enClsControl = 0;
			}
			if ( _chsClsControl != 0) {
				delete _chsClsControl;
				_chsClsControl = 0;
			}
		}
		//
		void setEnFile(string file);
		string enFile();
		void setChsFile(string file);
		string chsFile();
		void setEnClsFile(string file);
		string enClsFile();
		void setChsClsFile(string file);
		string chsClsFile();
		//
		TermControl* enControl();
		TermControl* chsControl();
		TermControl* termControl(string lang);
		int openEnVoc();
		int openChsVoc();
		int saveBoth();
		Vocabulary* enVoc();
		Vocabulary* chsVoc();
		//
		TermControl* enClsControl();
		TermControl* chsClsControl();
		TermControl* clsControl(string lang);
		int openEnCls();
		int openChsCls();		
		Vocabulary* enCls();
		Vocabulary* chsCls();
		// an integrated search
		void PrefixSearch(wstring, vector<Term*>& out, vector<Term*>& o2, int k = 10000000);
		void ClearModifyTerms();
	protected:		
		Vocabulary _enVoc;
		Vocabulary _chsVoc;
		TermControl* _enControl;
		TermControl* _chsControl;
		VocabularyXmlControl _enVocControl;
		VocabularyXmlControl _chsVocControl;
		Vocabulary _enCls;
		Vocabulary _chsCls;
		TermControl* _enClsControl;
		TermControl* _chsClsControl;
		VocabularyXmlControl _enVocClsControl;
		VocabularyXmlControl _chsVocClsControl;
	};	
	//
	inline Vocabulary* TermFactory::enVoc() {
		return &_enVoc;
	}
	inline Vocabulary* TermFactory::chsVoc() {
		return &_chsVoc;
	}
	inline string TermFactory::chsFile() {
		return _chsVocControl.GetFile();
	}
	inline void TermFactory::setChsFile(string file) {
		_chsVocControl.SetFile(file);
	}
	//
	inline Vocabulary* TermFactory::enCls() {
		return &_enCls;
	}
	inline Vocabulary* TermFactory::chsCls() {
		return &_chsCls;
	}
	inline string TermFactory::chsClsFile() {
		return _chsVocClsControl.GetFile();
	}
	inline void TermFactory::setChsClsFile(string file) {
		_chsVocClsControl.SetFile(file);
	}
	//
	inline string TermFactory::enFile() {
		return _enVocControl.GetFile();
	}
	inline void TermFactory::setEnFile(string file) {
		_enVocControl.SetFile(file);
	}
	//
	inline string TermFactory::enClsFile() {
		return _enVocClsControl.GetFile();
	}
	inline void TermFactory::setEnClsFile(string file) {
		_enVocClsControl.SetFile(file);
	}
	//
	inline int TermFactory::openEnVoc() {
		if ( _enVocControl.GetFile() != "") {
			std::cout << "loading en voc sucess!" << endl;
			_enVoc.ClearTerms();
			_enVocControl.Load(_enVoc);
			std::cout << "loaded en voc sucess!" << endl;
			return true;
		}
		return false;
	}
	inline int TermFactory::openChsVoc() {
		if ( _chsVocControl.GetFile() != "") {
			std::cout << "loading chs voc sucess!" << endl;
			_chsVoc.ClearTerms();
			_chsVocControl.Load(_chsVoc);
			std::cout << "loaded chs voc sucess!" << endl;
			return true;
		}
		return false;
	}
	//
	inline int TermFactory::openEnCls() {
		if ( _enVocClsControl.GetFile() != "") {
			_enCls.ClearTerms();
			_enVocClsControl.Load(_enCls);
			//std::cout << "loaded en voc sucess!" << endl;
			return true;
		}
		return false;
	}
	inline int TermFactory::openChsCls() {
		if ( _chsVocClsControl.GetFile() != "") {
			_chsCls.ClearTerms();
			_chsVocClsControl.Load(_chsCls);
			//std::cout << "loaded chs voc sucess!" << endl;
			return true;
		}
		return false;
	}
	//
	inline int TermFactory::saveBoth() {
		if ( _enVocControl.GetFile() != "") {
			_enVocControl.Save(_enVoc);
		}
		if ( _chsVocControl.GetFile() != "") {
			_chsVocControl.Save(_chsVoc);
		}
		ClearModifyTerms();
		return true;
	}
	//
	inline TermControl* TermFactory::termControl(string lang) {
		if ( lang == "chs" ) {
			return chsControl();
		}else {
			return enControl();
		}
		
	}
	inline TermControl* TermFactory::enControl() {
		if (_enControl == 0 ) {
			openEnVoc();
			_enControl = new TermControl(&_enVoc);
		}
		return _enControl;
	}
	inline TermControl* TermFactory::chsControl() {
		if (_chsControl == 0 ) {
			openChsVoc();
			_chsControl = new TermControl(&_chsVoc);
		}
		return _chsControl;
	}
	//
	inline TermControl* TermFactory::clsControl(string lang) {
		if ( lang == "chs" ) {
			return chsClsControl();
		}else {
			return enClsControl();
		}
		
	}
	inline TermControl* TermFactory::enClsControl() {
		if (_enClsControl == 0 ) {
			openEnCls();
			_enClsControl = new TermControl(&_enCls);
		}
		return _enClsControl;
	}
	inline TermControl* TermFactory::chsClsControl() {
		if (_chsClsControl == 0 ) {
			openChsCls();
			_chsClsControl = new TermControl(&_chsCls);
		}
		return _chsClsControl;
	}
	inline void TermFactory::PrefixSearch(wstring pre, vector<Term*>& out, vector<Term*>& o2, int k) {
		enControl()->PrefixSearch(pre, out);
		vector<Term*>::iterator i = out.begin();
		for(; i < out.end(); ++i) {
			string iid = (*i)->id()->iid();
			Term* t = chsControl()->GetTermByIid(iid);
			if(!t)
				t = chsControl()->CreateTermWithIid(iid);
			o2.push_back(t);
		}
		int size = out.size();
		if(out.size() < k) {
			chsControl()->PrefixSearch(pre, out);
			for(i = out.begin() + size; i < out.end(); ++i) {
				string iid = (*i)->id()->iid();
				Term* t = enControl()->GetTermByIid(iid);
				if(!t)
					t = enControl()->CreateTermWithIid(iid);
				o2.push_back(t);
			}
		}
	}

	inline void TermFactory::ClearModifyTerms() {
		chsControl()->ClearModifyTerms();
		enControl()->ClearModifyTerms();
	}
}
#endif