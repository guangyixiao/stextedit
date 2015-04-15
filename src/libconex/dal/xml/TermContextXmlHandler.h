#ifndef CONEX_DAL_TERM_CONTEXT_XML_HANDLER_H
#define CONEX_DAL_TERM_CONTEXT_XML_HANDLER_H
#include "XmlHandler.h"
#include "model/Term.h"

namespace conex {
	class TermContextXmlHandler : public XmlHandler{
	public:
		TermContextXmlHandler(DOMDocument* doc, DOMNode* contextNode, Term* term):_contextNode(contextNode), _term(term),XmlHandler(doc) {}
		virtual ~TermContextXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		DOMNode* _contextNode;
		Term* _term;
	};
}

#endif