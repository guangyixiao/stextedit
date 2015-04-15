#ifndef CONEX_DAL_IID_XML_HANDLER_H
#define CONEX_DAL_IID_XML_HANDLER_H
#include "XmlHandler.h"
#include "model/Concept.h"

namespace conex {
	class IdXmlHandler : public XmlHandler{
	public:
		IdXmlHandler(DOMDocument* doc, DOMNode* idNode, Concept* concept): _idNode(idNode), _concept(concept), XmlHandler(doc) {}
		virtual ~IdXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		DOMNode* _idNode;
		Concept* _concept;
	};
}

#endif