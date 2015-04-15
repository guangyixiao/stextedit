#include "VocabularyXmlAccess.h"
#include "xercesc\dom\DOM.hpp"
#include "xercesc\parsers\XercesDOMParser.hpp"
#include "xercesc\sax\HandlerBase.hpp"

namespace conex {
	VocabularyXmlAccess::VocabularyXmlAccess() {
		try {
			XMLPlatformUtils::Initialize();
		}catch(const XMLException& toCatch) {

		}
	}

	VocabularyXmlAccess::~VocabularyXmlAccess() {
		XMLPlatformUtils::Terminate();
	}
		
	void VocabularyXmlAccess::Load(Vocabulary& voc) {

	}
		
	void VocabularyXmlAccess::Save(const Vocabulary& voc) {
		SaveAs(voc, m_filename);
	}

	void VocabularyXmlAccess::SaveAs(const Vocabulary& voc, char* filename) {

	}

	int VocabularyXmlAccess::Parse(char* filename, char* schemaname) {
		XercesDOMParser* parser = new XercesDOMParser();
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);
		
		ErrorHandler* errHandler = (ErrorHandler*)new HandlerBase();
		parser->setErrorHandler(errHandler);

		try {
			parser->parse(filename);
		}catch(const XMLException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			XMLString::release(&message);
			return false;
		}catch(const DOMException& toCatch) {
			return false;
		}catch(...) {
			return false;
		}

		delete parser;
		delete errHandler;

		return true;
	}
}