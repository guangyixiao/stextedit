#ifndef CONEX_DAL_VOCABULARY_DB_ACCESS_H
#define CONEX_DAL_VOCABULARY_DB_ACCESS_H
#include "VocabularyAccess.h"
namespace conex {
	class VocabularyDbAccess : public VocabularyAccess{
	public:
		VocabularyDbAccess();
		virtual ~VocabularyDbAccess();
		virtual void Load(Vocabulary& voc);
		virtual void Save(const Vocabulary& voc);
	};
}
#endif