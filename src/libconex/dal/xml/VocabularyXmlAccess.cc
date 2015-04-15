#include "VocabularyXmlAccess.h"
#include <iostream>
#include "xercesc/dom/DOM.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"
#include "xercesc/sax2/ContentHandler.hpp"
#include "xercesc/sax2/Attributes.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/framework/StdOutFormatTarget.hpp"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "VocabularyXmlHandler.h"
#include "VocabularySax2Handler.h"


namespace conex {	
	using namespace xercesc;

	void VocabularyXmlAccess::Init() {
		try {
			XMLPlatformUtils::Initialize();
		}catch(const XMLException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Error during initialization!:/n" << message << endl;
			XMLString::release(&message);
		}

		XMLCh tempStr[100];
		XMLString::transcode("LS", tempStr, 99);
		m_impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
		m_domparser = ((DOMImplementationLS*)m_impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

		XMLString::transcode("core", tempStr, 99);
		m_docImpl = DOMImplementationRegistry::getDOMImplementation(tempStr);

		m_serializer = ((DOMImplementationLS*)m_impl)->createLSSerializer();

		// optionally you can set some features on this serializer
		if (m_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
			m_serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

		if (m_serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			m_serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		// optionally you can implement your DOMLSSerializerFilter (e.g. MyDOMLSSerializerFilter)		
	}

	VocabularyXmlAccess::~VocabularyXmlAccess() {
		if(m_domparser) {
			m_domparser->release();
			m_domparser = NULL;
		}
		XMLPlatformUtils::Terminate();
	}

	void VocabularyXmlAccess::Load(Vocabulary& voc) {
		if(_isSax2) 
			ParseWithSax2(m_filename.c_str(), m_schemaname.c_str(), voc) ;
		else
		    ParseWithDomLS(m_filename.c_str(), m_schemaname.c_str(), voc) ;
	}

	void VocabularyXmlAccess::Save(Vocabulary& voc) {
		SaveAs(voc, m_filename.c_str());
	}

	void VocabularyXmlAccess::SaveAs(Vocabulary& voc, const char* filename) {
		// StdOutFormatTarget prints the resultant XML stream
		// to stdout once it receives any thing from the serializer.
		// XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
		DOMLSOutput* output = ((DOMImplementationLS*)m_impl)->createLSOutput();  
		XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(filename);
		output->setByteStream(myFormTarget);
		DOMDocument* document = NULL;
		/*
		Create an empty DOMDocument. When an empty document 
		is created specify the namespace of the document (optional)
		and also the root node. Both of these values can be any 
		valid text (ie. no spaces or weird characters).
		The "root node" in this example is "Hello_World". 
		(XML is case-sensitive) This root node 
		*owns everything* beneath it, just as if it were a separate object... 
		in fact, it is. It's defined by the 'DOMNode' class.

		NOTE: The namespace in this example happens to be a website
		within the 'example.com' domain [RFC2606]
		However, it does NOT have to be a domain name. 
		It could be your company name, or the name of your dog.
		It's just that domain names tend to be unique. 
		Since any name will do, there does not even have to 
		be a website at the address specified.
		*/
		//XMLCh tempStr[100];
		//XMLString::transcode("voc", tempStr, 99);		
		XMLCh* qualifiedNAME = XMLString::transcode("sdf:voc");
		XMLCh* namespaceURL = XMLString::transcode("sdf:std:voc:schema:2012:10:01");
		document = m_docImpl->createDocument(namespaceURL,qualifiedNAME, 0);
		XMLString::release(&qualifiedNAME);
		XMLString::release(&namespaceURL);
		VocabularyXmlHandler vocHandler(&voc, document);
		vocHandler.Serialize();
		m_serializer->write(document, output);
		document->release();
		output->release();
		//output->release();
		delete myFormTarget;
	}

	void VocabularyXmlAccess::ParseWithDomLS(const char* filename, const char* schemafile, Vocabulary& voc) {
		int result = true;
		/*XercesDOMParser* parser = new XercesDOMParser();
		parser->setExternalSchemaLocation(schemafile);
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);

		ErrorHandler* errHandler = (ErrorHandler*)new HandlerBase();
		parser->setErrorHandler(errHandler);*/
		try {			
			DOMDocument* document = NULL;
			//document = m_parser->getDocument();
			document = m_domparser->parseURI(filename);
			if(document){
				VocabularyXmlHandler vocHandler(&voc, document);
				vocHandler.Parse();
				//ParseVocabulary(xmlRoot, voc);
				//ParseVocabulary			
			    document->release();
			}
			
		}catch(const XMLException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Exception message is: /n" << message << endl;
			XMLString::release(&message);
			result = false;
		}catch(const DOMException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Exception message is: /n" << message << endl;
			XMLString::release(&message);
			result = false;
		}catch(...) {
			cout << "Unexpected Exception /n";
			result = false;
		}				
	}

	void VocabularyXmlAccess::ParseWithSax2(const char* filename, const char* schamefile, Vocabulary& voc) {
		SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();
		VocabularySax2Handler sax2Handler(&voc);
		pParser->setContentHandler(&sax2Handler);
		pParser->setErrorHandler(&sax2Handler);
		try {					
			pParser->parse(filename);
		}catch(const XMLException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Exception message is: /n" << message << endl;
			XMLString::release(&message);
		}catch(const DOMException& toCatch) {
			char* message = XMLString::transcode(toCatch.getMessage());
			cout << "Exception message is: /n" << message << endl;
			XMLString::release(&message);
		}catch(...) {
			cout << "Unexpected Exception /n";
		}			
	}
}