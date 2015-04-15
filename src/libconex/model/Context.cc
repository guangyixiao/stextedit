#include "Context.h"
#include <algorithm>
#include "Term.h"


namespace conex {
	Context::~Context() {
		ClearRelations();
	}

	Relation* Context::AddRelation(Type type, Term* term) {
		return AddRelation(type, term->id()->iid());
	}

	void Context::DeleteRelation(Relation* t) {
		for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ++it) {
			if((*it) == t) {
				relations.erase(it);
				delete t;
				t = NULL;
				return;
			}
		}		
	}

	void Context::DeleteRelation(Term* t) {
		for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ++it) {
			if((*it)->GetRelatedTermIid() == t->id()->iid()) {
				relations.erase(it);
				delete (*it);
				(*it) = NULL;
				return;
			}
		}		
	}

	void Context::DeleteRelations(Type type) {
		for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ) {
			if((*it)->type() == type) {
				it = relations.erase(it);
				delete (*it);
				(*it) = NULL;
			}
			++it;
		}		
	}

	void Context::ClearRelations() {
		if(!relations.empty()) {
			for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ++it) {
			    delete (*it);
				(*it) = NULL;
			}
		}
		relations.clear();
	}	

	//synonym management
	void Context::GetRelationTerms(Type type, std::vector<string>& terms) {
		for(std::vector<Relation*>::iterator it = relations.begin(); it != relations.end(); ++it) {
			if((*it)->type() == type) {
				string term = (*it)->GetRelatedTermIid();
				if(term.size() > 0)
					terms.push_back(term);
			}
		}
	}
	
}//end namespace