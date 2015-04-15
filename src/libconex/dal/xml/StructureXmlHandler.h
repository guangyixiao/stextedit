#ifndef CONEX_DAL_STRUCTURE_XML_HANDLER_H
#define CONEX_DAL_STRUCTURE_XML_HANDLER_H
#include "XmlHandler.h"
#include "model/Concept.h"

namespace conex {
	using namespace std;
	class StructureXmlHandler : public XmlHandler{
	public:
		StructureXmlHandler(DOMDocument* doc,DOMNode* strNode, Concept* concept): _strNode(strNode), _concept(concept), XmlHandler(doc) {}
		virtual ~StructureXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		DOMNode* _strNode;
		Concept* _concept;
	};
}

#endif