#include "StructureXmlHandler.h"
#include <iostream>

using namespace std;

namespace conex {
	void StructureXmlHandler::Parse() {
		//Structure* strc = _concept->CreateStructure();
		//XMLCh attrName[100];
		//char content[500];
		//DOMNode* node = NULL;
		//XMLString::transcode("Text", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	//XMLString::transcode(node->getTextContent(), content, 499);
		//	//strc->SetText(node->getTextContent());
		//	cout << content << endl;
		//}
		//XMLString::transcode("Pos", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	XMLString::transcode(node->getTextContent(), content, 499);
		//	//strc->SetPos(content);
		//}
		//XMLString::transcode("Phonetics", attrName, 99);		
		//node = _attrs->getNamedItem(attrName);
		//if(node) {
		//	XMLString::transcode(node->getTextContent(), content, 499);
		//	//strc->SetPhonetics(content);
		//}
	}

	void StructureXmlHandler::Serialize() {
		XMLCh tempStr[500];	
		DOMElement* node = NULL;
		// set iids		
		if(_concept->structure()->iids().size() > 0) {
			XMLCh* iids = XMLString::transcode("sdf:iids");
			node = _doc->createElement(iids);
			XMLString::transcode(_concept->structure()->iids().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_strNode->appendChild(node);
			XMLString::release(&iids);
		}
		// set phonetic		
		if(_concept->structure()->graphPath().size() > 0) {
			XMLCh* graph = XMLString::transcode("sdf:graph");
			node = _doc->createElement(graph);
			XMLString::transcode(_concept->structure()->graphPath().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_strNode->appendChild(node);
			XMLString::release(&graph);
		}
		// set sound		
		if(_concept->structure()->soundPath().size() > 0) {
			XMLCh* sound = XMLString::transcode("sdf:sound");
			node = _doc->createElement(sound);
			XMLString::transcode(_concept->structure()->soundPath().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_strNode->appendChild(node);
			XMLString::release(&sound);
		}
		// set video	
		if(_concept->structure()->videoPath().size() > 0) {
			XMLCh* video = XMLString::transcode("sdf:video");
			node = _doc->createElement(video);
			XMLString::transcode(_concept->structure()->videoPath().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_strNode->appendChild(node);
			XMLString::release(&video);
		}
	}
}