#ifndef CONEX_CONTROL_VOCABULARY_CONTROL_H
#define CONEX_CONTROL_VOCABULARY_CONTROL_H
#include "model/Vocabulary.h"

namespace conex {
	class VocabularyAccess;

	class VocabularyControl {
	public:
		enum Model {DB, XML_DOM, XML_SAX2};
		//VocabularyControl(Model m);
		VocabularyControl(VocabularyAccess* vocAccess);
		VocabularyControl(Model model);
		VocabularyControl() {}
		~VocabularyControl() {}
		void Load(Vocabulary& voc);
		void Save(Vocabulary& voc);
		void Clear(Vocabulary& voc);
	protected:
		VocabularyAccess* m_vocAccess;
	};	
}
#endif