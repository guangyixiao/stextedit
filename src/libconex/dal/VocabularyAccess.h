#ifndef CONEX_DAL_VOCABULARY_ACCESS_H
#define CONEX_DAL_VOCABULARY_ACCESS_H
#include "model/Vocabulary.h"
namespace conex {
	class VocabularyAccess {
	public:
		VocabularyAccess() {}
		virtual ~VocabularyAccess() {}
		virtual void Load(Vocabulary& voc) = 0;
		virtual void Save(Vocabulary& voc) = 0;
	};
}
#endif