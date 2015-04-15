#ifndef CONEX_MODEL_RELATION_H
#define CONEX_MODEL_RELATION_H
//#include "Term.h"
#include <string>

namespace conex {
	enum Type {SYNONYM, SIMINYM, ANTONYM, HYPERNYM, CATEGORY};

	using namespace std;

	class Relation {
	public:
		// type
		
		Relation(Type type, string iid): _type(type), m_relatedTerm(iid) {}
		//virtual ~Relation();
		void RelatedTo(string iid);
		string GetRelatedTermIid();
		void SetType(Type type);
		Type type();
	protected:
		string m_relatedTerm;
		Type _type;
	};

	inline void Relation::RelatedTo(string iid) {
		m_relatedTerm = iid;
	}

	inline string Relation::GetRelatedTermIid() {
		return m_relatedTerm;
	}

	inline void Relation::SetType(Type type) {
		_type = type;
	}
	
	inline Type Relation::type() {
		return _type;
	}
}
#endif