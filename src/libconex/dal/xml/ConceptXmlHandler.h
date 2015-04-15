#ifndef CONEX_DAL_CONCEPT_XML_HANDLER_H
#define CONEX_DAL_CONCEPT_XML_HANDLER_H
#include "XmlHandler.h"
#include "model/Concept.h"

namespace conex {
	class ConceptXmlHandler : public XmlHandler{
	public:
		ConceptXmlHandler(DOMDocument* doc, DOMNode* conceptNode, Concept* concept): _conceptNode(conceptNode), _concept(concept), XmlHandler(doc) {}
		virtual ~ConceptXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		DOMNode* _conceptNode;
		Concept* _concept;
	};
}

#endif