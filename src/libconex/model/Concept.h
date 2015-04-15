#ifndef CONEX_MODEL_UPPER_CONCEPT_H
#define CONEX_MODEL_UPPER_CONCEPT_H

#include "Id.h"
#include "Structure.h"
#include "Interpretation.h"
#include "Context.h"

namespace conex {
	class Context;

	class Concept {
	public:
		Concept() {}
		Concept(std::string iid): _id(iid), _structure(0), _context(0){
		}
		Context* context();
		Id* id();
		void setIid(string iid);
		Structure* CreateStructure();
		Structure* structure();
		wstring text();
		void setText(wstring text);
		string phonetic();
		void setPhonetic(string phonetic);
		string picturePath();
		void setPicturePath(string path);
		string filmPath();
		void setFilmPath(string path);
		bool hasContext();
		bool hasStructure();
	protected:
		Id _id;
		Structure* _structure;
		Context* _context;
		wstring _text;
		string _phonetic;
		string _picturePath;
		string _filmPath;
	};

	inline bool Concept::hasStructure() {
		return _context == 0 ? false : true;
	}
	inline bool Concept::hasContext() {
		return _structure == 0 ? false : true;
	}
	inline void Concept::setIid(string iid) {
		_id.setIid(iid);
	}
	inline wstring Concept::text() {
		return _text;
	}
	inline void Concept::setText(wstring text) {
		_text = text;
	}
	inline string Concept::phonetic() {
		return _phonetic;
	}
	inline void Concept::setPhonetic(string phonetic) {
		_phonetic = phonetic;
	}
	inline string Concept::picturePath() {
		return _picturePath;
	}
	inline void Concept::setPicturePath(string path) {
		_picturePath = path;
	}
	inline string Concept::filmPath() {
		return _filmPath;
	}
	inline void Concept::setFilmPath(string path) {
		_filmPath = path;
	}

	inline Context* Concept::context() {
		if(_context == 0) 
			_context = new Context();
		return _context;
	}

	
	inline Id* Concept::id() {
		return &_id;
	}

	inline Structure* Concept::structure() {
		if(_structure == 0)
			_structure = new Structure();
		return _structure;
	}

}
#endif