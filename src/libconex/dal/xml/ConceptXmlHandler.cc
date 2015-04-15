#include "ConceptXmlHandler.h"

namespace conex {
	void ConceptXmlHandler::Parse() {
		//Interpretation* inter = _concept->CreateInterpretation();
		//XMLCh attrName[100];
		//char content[500];
		//DOMNode* node = NULL;
		//XMLString::transcode("TextExplanation", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	//XMLString::transcode(node->getTextContent(), content, 499);
		//	inter->SetTextExplanation(node->getTextContent());
		//}
		//XMLString::transcode("IidExplanation", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	XMLString::transcode(node->getTextContent(), content, 499);
		//	inter->SetIidExplanation(content);
		//}
		//XMLString::transcode("LogicalExpression", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	XMLString::transcode(node->getTextContent(), content, 499);
		//	inter->SetIidExplanation(content);
		//}
	}

	void ConceptXmlHandler::Serialize() {
		XMLCh tempStr[500];	
		DOMElement* node = NULL;
		// set iid			
		XMLCh* text = XMLString::transcode("sdf:text");
		node = _doc->createElement(text);
		//XMLString::transcode(_concept->text().c_str(), tempStr, 499);
		node->setTextContent(_concept->text().c_str());
		_conceptNode->appendChild(node);
		XMLString::release(&text);
		// set phonetic		
		if(_concept->phonetic().size() > 0) {
			XMLCh* phonetic = XMLString::transcode("sdf:phonetic");
			node = _doc->createElement(phonetic);
			XMLString::transcode(_concept->phonetic().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_conceptNode->appendChild(node);
			XMLString::release(&phonetic);
		}
		// set picture
		if(_concept->picturePath().size() > 0) {
			XMLCh* picture = XMLString::transcode("sdf:picture");
			node = _doc->createElement(picture);
			XMLString::transcode(_concept->picturePath().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_conceptNode->appendChild(node);
			XMLString::release(&picture);
		}
		// set film		
		if(_concept->filmPath().size() > 0) {
			XMLCh* film = XMLString::transcode("sdf:film");
			node = _doc->createElement(film);
			XMLString::transcode(_concept->filmPath().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_conceptNode->appendChild(node);
			XMLString::release(&film);
		}
	}
}