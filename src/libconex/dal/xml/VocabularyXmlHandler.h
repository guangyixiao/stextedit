#ifndef CONEX_DAL_VOCABULARY_XML_HANDLER_H
#define CONEX_DAL_VOCABULARY_XML_HANDLER_H
#include "XmlHandler.h"
#include "model/Vocabulary.h"
namespace conex {
	class VocabularyXmlHandler : public XmlHandler {
	public:
		VocabularyXmlHandler(Vocabulary* voc, DOMDocument* doc):_voc(voc), XmlHandler(doc) {}
		virtual ~VocabularyXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		Vocabulary* _voc;
	};
}

#endif