#include "IdXmlHandler.h"

namespace conex {
	void IdXmlHandler::Parse() {
		//Id* iid = _concept->CreateId();
		//XMLCh* attrName = XMLString::transcode("Iid");
		//char* content = NULL;
		//DOMNode* node = NULL;
		////node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	content = XMLString::transcode(node->getTextContent());
		//	iid->setIid(content);
		//}
		//XMLString::release(&attrName);
		//XMLString::release(&content);
	}

	void IdXmlHandler::Serialize() {
		XMLCh tempStr[500];	
		DOMElement* node = NULL;
		// set iid			
		XMLCh* iid = XMLString::transcode("sdf:iid");
		node = _doc->createElement(iid);
		XMLString::transcode(_concept->id()->iid().c_str(), tempStr, 499);
		node->setTextContent(tempStr);
		_idNode->appendChild(node);
		XMLString::release(&iid);
		// set word			
		XMLCh* word = XMLString::transcode("sdf:word");
		node = _doc->createElement(word);
		XMLString::release(&word);
		//XMLString::transcode(_concept->id()->word().c_str(), tempStr, 499);
		node->setTextContent(_concept->id()->word().c_str());
		_idNode->appendChild(node);
		// set iid			
		if(_concept->id()->pronunciation().size() > 0) {
			XMLCh* pronunciation = XMLString::transcode("sdf:pronunciation");
			node = _doc->createElement(pronunciation);
			XMLString::release(&pronunciation);
			XMLString::transcode(_concept->id()->pronunciation().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_idNode->appendChild(node);
		}
	}
}