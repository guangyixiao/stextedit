#include "VocabularyAccessFactory.h"
#include "xml/VocabularyXmlAccess.h"
//#include "db/VocabularyDbAccess.h"

namespace conex {	
	
	VocabularyXmlAccess* VocabularyAccessFactory::GetXmlAccess(bool isSax2) {
		//VocabularyAccess* access = new 
		if(isSax2) {
			if(xmlSax2Access == NULL) {
				xmlSax2Access = new VocabularyXmlAccess("Vocabulary_1.0.xml", "XPMVoc_V09R65.xsd");
			}
			return xmlSax2Access;
		}else {
			if(xmlDomAccess == NULL) {
				xmlDomAccess = new VocabularyXmlAccess("Vocabulary_1.0.xml", "XPMVoc_V09R65.xsd", false);
			}
			return xmlDomAccess;
		}
			return NULL;
	}

	VocabularyAccess* VocabularyAccessFactory::GetAccess(int type) {
		//VocabularyAccess* access = new 
		switch(type) {
		case 1:
			return GetXmlAccess(true);
		case 2:
			return GetXmlAccess(false);
		//case 3:
		//	return GetDbAccess();
		}
			return NULL;
	}

	//VocabularyDbAccess* VocabularyAccessFactory::GetDbAccess() {
	//	//VocabularyAccess* access = new 
	//	if(dbAccess == NULL) {
	//		dbAccess = new VocabularyDbAccess();
	//	}
	//	return dbAccess;
	//}
}