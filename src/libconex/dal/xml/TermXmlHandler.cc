#include "TermXmlHandler.h"
#include "IdXmlHandler.h"
#include "StructureXmlHandler.h"
#include "ConceptXmlHandler.h"
#include "TermContextXmlHandler.h"

namespace conex {
	void TermXmlHandler::Parse() {
		//to do: parse the attributes
		//DOMNamedNodeMap* map = _termNode->getAttributes();
		if(_termNode ) {
			IdXmlHandler iidHandler(_doc, _termNode, _term);
			iidHandler.Parse();
			StructureXmlHandler strcHandler(_doc, _termNode, _term);
			strcHandler.Parse();
			ConceptXmlHandler interHandler(_doc, _termNode, _term);
			interHandler.Parse();
			//TermContext* context = _term->GetTermContext();
			TermContextXmlHandler termContextHandler(_doc, _termNode, _term);
			termContextHandler.Parse();
			//to do: parse the chindnode
		}
	}

	void TermXmlHandler::Serialize() {
		//to do: write the attributes
		DOMNamedNodeMap* map = _termNode->getAttributes();
		XMLCh tempStr[500];	
		DOMAttr* attr = NULL;
		DOMAttr* attr2 = NULL;
		DOMElement* nodeElement = NULL;
		// set pos			
		const static XMLCh* pos = XMLString::transcode("sdf:pos");
		attr = _doc->createAttribute(pos);
		XMLString::transcode(_term->pos().c_str(), tempStr, 499);
		attr->setNodeValue(tempStr);
		map->setNamedItem(attr);
		// set state
		const static XMLCh* state = XMLString::transcode("sdf:state");
		attr2 = _doc->createAttribute(state);
		XMLString::transcode(_term->state().c_str(), tempStr, 499);
		attr2->setNodeValue(tempStr);
		map->setNamedItem(attr2);

		if(_term->id()) {
			const static XMLCh* id = XMLString::transcode("sdf:id");
			nodeElement = _doc->createElement(id);
			_termNode->appendChild(nodeElement);
			IdXmlHandler iidHandler(_doc, nodeElement, _term);
			iidHandler.Serialize();
		}

		const static XMLCh* concept = XMLString::transcode("sdf:concept");
		nodeElement = _doc->createElement(concept);
		_termNode->appendChild(nodeElement);
		ConceptXmlHandler conHandler(_doc, nodeElement, _term);
		conHandler.Serialize();
		
		if(_term->hasStructure()) {
			XMLCh* structure = XMLString::transcode("sdf:structure");
			nodeElement = _doc->createElement(structure);
			_termNode->appendChild(nodeElement);
			StructureXmlHandler strHandler(_doc, nodeElement, _term);
			strHandler.Serialize();
			XMLString::release(&structure);
		}		
		
		//to do: write the attributes for context
		//TermContext* context = _term->GetTermContext();
		if(_term->hasContext()) {
			const static XMLCh* context = XMLString::transcode("sdf:context");
			nodeElement = _doc->createElement(context);
			_termNode->appendChild(nodeElement);
			TermContextXmlHandler contextHandler(_doc, nodeElement, _term);
			contextHandler.Serialize();
		}
		//to do: write the chindnode
		//there is no child nodes for term!!!
	}
}