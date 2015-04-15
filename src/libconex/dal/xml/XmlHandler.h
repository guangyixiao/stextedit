#ifndef CONEX_DAL_XML_HANDLER_H
#define CONEX_DAL_XML_HANDLER_H
#include "xercesc/dom/DOM.hpp"

namespace conex {
	using namespace xercesc;

	class XmlHandler {
	public:
		XmlHandler(DOMDocument* doc):_doc(doc) {}
		virtual ~XmlHandler() {}
		virtual void Parse() = 0;
		virtual void Serialize() = 0;
	protected:
		DOMDocument* _doc;
	};
}

#endif