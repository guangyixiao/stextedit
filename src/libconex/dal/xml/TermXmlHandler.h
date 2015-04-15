#ifndef CONEX_DAL_TERM_XML_ACCESS_H
#define CONEX_DAL_TERM_XML_ACCESS_H
#include "XmlHandler.h"
#include "model/Term.h"

namespace conex {
	class TermXmlHandler : public XmlHandler {
	public:
		TermXmlHandler(DOMDocument* doc, DOMNode* termNode, Term* term):_termNode(termNode), _term(term), XmlHandler(doc) {}
		virtual ~TermXmlHandler() {}
		virtual void Parse();
		virtual void Serialize();
	private:
		DOMNode* _termNode;
		Term* _term;
	};
}

#endif