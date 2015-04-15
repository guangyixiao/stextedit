#include "VocabularyXmlHandler.h"
#include "IdXmlHandler.h"
#include "StructureXmlHandler.h"
#include "ConceptXmlHandler.h"
#include "TermXmlHandler.h"


namespace conex {
	void VocabularyXmlHandler::Parse() {
		DOMElement* xmlRoot = _doc->getDocumentElement();
		DOMNamedNodeMap* map = xmlRoot->getAttributes();
		DOMNode* node = NULL;
		//parse the attributes
		if(map && map->getLength() > 0) {
			//to do: parse the attributes for context
			//VocabularyContext* context = _voc->GetVocabularyContext();	
		}
		//parse the chindnode
		DOMNodeList* nodeList = xmlRoot->getChildNodes();
		for(XMLSize_t i = 0; i < nodeList->getLength(); ++i) {
			node = nodeList->item(i);
			if(node->getAttributes()) {
			   Term* term = _voc->CreateTerm();
			   TermXmlHandler termHandler(_doc, node, term);
			   termHandler.Parse();
			}
		}
	}

	void VocabularyXmlHandler::Serialize() {		
		DOMElement* xmlRoot = _doc->getDocumentElement();
		// set attribute name sapce
		XMLCh* schemaLocation = XMLString::transcode("sdf:std:voc:schema:2012:10:01 sdfv-schema-v10-2014-11-28.xsd");
		XMLCh* xsiNamespace = XMLString::transcode("xmlns:xsi");
		XMLCh* locationNamespace = XMLString::transcode("xsi:schemaLocation");
		XMLCh* w3uri = XMLString::transcode("http://www.w3.org/2000/xmlns/");
		XMLCh* instanceuri = XMLString::transcode("http://www.w3.org/2001/XMLSchema-instance");
		xmlRoot->setAttributeNS(w3uri ,xsiNamespace,instanceuri);
		xmlRoot->setAttributeNS(w3uri ,locationNamespace,schemaLocation);
		XMLString::release(&schemaLocation);
		XMLString::release(&xsiNamespace);
		XMLString::release(&locationNamespace);
		XMLString::release(&w3uri);
		XMLString::release(&instanceuri);
		// childs
		//to do: parse the attributes
		// to do vocabulary serialize
		DOMElement* nodeElement = NULL;
		DOMElement* nodeElement2 = NULL;
		XMLCh tempStr[500];		
		// set vid
		XMLCh* vidName = XMLString::transcode("sdf:vid");
		nodeElement = _doc->createElement(vidName);
		XMLString::transcode(_voc->vid().c_str(), tempStr, 499);
		nodeElement->setTextContent(tempStr);
		xmlRoot->appendChild(nodeElement);
		XMLString::release(&vidName);
		// set tag
		XMLCh* tagName = XMLString::transcode("sdf:tag");
		nodeElement = _doc->createElement(tagName);
		XMLString::transcode(_voc->tag().c_str(), tempStr, 499);
		nodeElement->setTextContent(tempStr);
		xmlRoot->appendChild(nodeElement);
		XMLString::release(&tagName);
		// set vid
		XMLCh* vi = XMLString::transcode("sdf:vi");
		nodeElement = _doc->createElement(vi);
		XMLString::release(&vi);
		// set vi_sui
		XMLCh* sui = XMLString::transcode("sdf:sui");
		nodeElement2 = _doc->createElement(sui);
		XMLString::transcode(_voc->sui().c_str(), tempStr, 499);
		nodeElement2->setTextContent(tempStr);
		nodeElement->appendChild(nodeElement2);
		XMLString::release(&sui);
		// set vi_author
		XMLCh* author = XMLString::transcode("sdf:author");
		nodeElement2 = _doc->createElement(author);
		XMLString::transcode(_voc->author().c_str(), tempStr, 499);
		nodeElement2->setTextContent(tempStr);
		nodeElement->appendChild(nodeElement2);
		XMLString::release(&author);

		xmlRoot->appendChild(nodeElement);
		// set vx
		XMLCh* vx = XMLString::transcode("sdf:vx");
		nodeElement = _doc->createElement(vx);
		XMLString::release(&vx);
		// set vx_lang
		XMLCh* lang = XMLString::transcode("sdf:lang");
		nodeElement2 = _doc->createElement(lang);
		XMLString::transcode(_voc->lang().c_str(), tempStr, 499);
		nodeElement2->setTextContent(tempStr);
		nodeElement->appendChild(nodeElement2);
		XMLString::release(&lang);
		// set vx_time
		XMLCh* time = XMLString::transcode("sdf:time");
		nodeElement2 = _doc->createElement(time);
		XMLString::transcode(_voc->time().c_str(), tempStr, 499);
		nodeElement2->setTextContent(tempStr);
		nodeElement->appendChild(nodeElement2);
		XMLString::release(&time);

		xmlRoot->appendChild(nodeElement);
		
		std::map<std::string, Term*> terms = _voc->GetTerms();		
		XMLCh* termName = XMLString::transcode("sdf:term");
		for(std::map<std::string, Term*>::iterator it = terms.begin(); it != terms.end(); ++it) {
			nodeElement = _doc->createElement(termName);
			xmlRoot->appendChild(nodeElement);
			TermXmlHandler termHandler(_doc, nodeElement, it->second);
			termHandler.Serialize();
		}
		XMLString::release(&termName);
	}
}