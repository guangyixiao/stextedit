#ifndef CONEX_MODEL_CONTEXT_H
#define CONEX_MODEL_CONTEXT_H

#include <vector>
#include <string>
#include <algorithm>
#include "Relation.h"

namespace conex {
	class Term;
	using namespace std;
	class Context {
	public:
		Context() {}
		~Context();		
		
		//relations management
		Relation* AddRelation(Type type, string termiid);
		Relation* AddRelation(Type type, Term* term);
		void GetRelationTerms(Type type, std::vector<string>& termiids);
		void DeleteRelation(Relation* relation);
		void DeleteRelation(Term* term);
		void DeleteRelations(Type type);
		void ClearRelations();
		string derivation();
		void setDerivation(string derivation);
		string examples();
		void setExamples(string examples);
	protected:
		std::vector<Relation*> relations;
		string _derivation;
		string _examples;
	};	

	inline string Context::derivation() {
		return _derivation;
	}
	inline void Context::setDerivation(string derivation) {
		_derivation = derivation;
	}
	inline string Context::examples() {
		return _examples;
	}
	inline void Context::setExamples(string examples) {
		_examples = examples;
	}

	//relation to single member
	inline Relation* Context::AddRelation(Type type, string termiid) {
		for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ++it) {
			if((*it)->GetRelatedTermIid() == termiid) {
				if((*it)->type() != type) {
					(*it)->SetType(type);
				}
				return (*it);
			}
		}	
		Relation* t = new Relation(type, termiid);
		relations.push_back(t);
		return t;
	}	
}
#endif