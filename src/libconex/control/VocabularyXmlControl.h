#ifndef CONEX_CONTROL_VOCABULARY_XML_CONTROL_H
#define CONEX_CONTROL_VOCABULARY_XML_CONTROL_H
#include "VocabularyControl.h"

#include <string>

namespace conex {
	class VocabularyXmlAccess;
	class VocabularyXmlControl : public VocabularyControl {
	public:
		//VocabularyControl(Model m);
		VocabularyXmlControl(VocabularyXmlAccess* vocAccess, std::string file);
		VocabularyXmlControl(Model model);
		~VocabularyXmlControl() {}
		void SetFile(std::string file);
		std::string GetFile();
	};	
}
#endif