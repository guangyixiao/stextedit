#ifndef CONEX_DAL_VOCABULARY_ACCESS_FACTORY_H
#define CONEX_DAL_VOCABULARY_ACCESS_FACTORY_H
#include "VocabularyAccess.h"
#include "xml/VocabularyXmlAccess.h"
//#include "db/VocabularyDbAccess.h"

namespace conex {
	class VocabularyAccessFactory {
	public:
		VocabularyAccessFactory(): xmlDomAccess(NULL), xmlSax2Access(NULL) /*, dbAccess(NULL) */{}
		VocabularyXmlAccess* GetXmlAccess(bool isSax2);
//		VocabularyDbAccess* GetDbAccess();
		VocabularyAccess* GetAccess(int type);
	private:
		VocabularyXmlAccess* xmlDomAccess;
		VocabularyXmlAccess* xmlSax2Access;
		//VocabularyDbAccess* dbAccess;
	};
}
#endif