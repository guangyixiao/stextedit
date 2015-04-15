#include "TermContextXmlHandler.h"

namespace conex {
	void TermContextXmlHandler::Parse() {

	}

	void TermContextXmlHandler::Serialize() {
		XMLCh tempStr[500];	
		DOMElement* node = NULL;
		// set iid			
		const static XMLCh* TX_SYNONYMS = XMLString::transcode("synonyms");
		const static XMLCh* TX_SIMINYMS = XMLString::transcode("siminyms");
		const static XMLCh* TX_CATEGORIES = XMLString::transcode("categories");
		const static XMLCh* TX_ANTONYMS = XMLString::transcode("antonyms");
		const static XMLCh* TX_HYPERNYMS = XMLString::transcode("hypernyms");
		const static XMLCh* TX_DERIVATION = XMLString::transcode("derivation");
		const static XMLCh* TX_EXAMPLES = XMLString::transcode("examples");

		//syn
		vector<string> rels;
		_term->context()->GetRelationTerms(SYNONYM, rels);
		if(rels.size() >0) {
			node = _doc->createElement(TX_SYNONYMS);
			string rel_str = rels.begin()->c_str();
			for(vector<string>::iterator it = rels.begin() + 1; it != rels.end(); ++it) {
				rel_str = rel_str + "," + it->c_str();
			}
			XMLString::transcode(rel_str.c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		rels.clear();
		// sim
		_term->context()->GetRelationTerms(SIMINYM, rels);
		if(rels.size() >0) {
			node = _doc->createElement(TX_SIMINYMS);
			string rel_str = rels.begin()->c_str();
			for(vector<string>::iterator it = rels.begin() + 1; it != rels.end(); ++it) {
				rel_str = rel_str + "," + it->c_str();
			}
			XMLString::transcode(rel_str.c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		rels.clear();
		// cat
		_term->context()->GetRelationTerms(CATEGORY, rels);
		if(rels.size() >0) {
			node = _doc->createElement(TX_CATEGORIES);
			string rel_str = rels.begin()->c_str();
			for(vector<string>::iterator it = rels.begin() + 1; it != rels.end(); ++it) {
				rel_str = rel_str + "," + it->c_str();
			}
			XMLString::transcode(rel_str.c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		rels.clear();
		// ant
		_term->context()->GetRelationTerms(ANTONYM, rels);
		if(rels.size() >0) {
			node = _doc->createElement(TX_ANTONYMS);
			string rel_str = rels.begin()->c_str();
			for(vector<string>::iterator it = rels.begin() + 1; it != rels.end(); ++it) {
				rel_str = rel_str + "," + it->c_str();
			}
			XMLString::transcode(rel_str.c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		rels.clear();
		// hym
		_term->context()->GetRelationTerms(HYPERNYM, rels);
		if(rels.size() >0) {
			node = _doc->createElement(TX_HYPERNYMS);
			string rel_str = rels.begin()->c_str();
			for(vector<string>::iterator it = rels.begin() + 1; it != rels.end(); ++it) {
				rel_str = rel_str + "," + it->c_str();
			}
			XMLString::transcode(rel_str.c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		rels.clear();

		// set derivation
		if(_term->context()->derivation().size() > 0) {
			node = _doc->createElement(TX_DERIVATION);
			XMLString::transcode(_term->context()->derivation().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
		// set examples		
		if(_term->context()->examples().size() > 0) {
			node = _doc->createElement(TX_EXAMPLES);
			XMLString::transcode(_term->context()->examples().c_str(), tempStr, 499);
			node->setTextContent(tempStr);
			_contextNode->appendChild(node);
		}
	}
}