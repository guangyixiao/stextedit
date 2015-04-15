#ifndef CONEX_DAL_VOCABULARY_XML_ACCESS_H
#define CONEX_DAL_VOCABULARY_XML_ACCESS_H

#include <string>
#include "dal/VocabularyAccess.h"

namespace xercesc_3_1 {
	class DOMImplementation;
	class DOMLSParser;
	class DOMLSSerializer;
}

namespace conex {
	using namespace std;
	class TermContext;		

	class VocabularyXmlAccess : public VocabularyAccess{
	public:
		VocabularyXmlAccess(bool isSax2 = true): _isSax2(isSax2) {Init();}
		void Init();
		VocabularyXmlAccess(char* filename, char* schemaname, bool isSax2 = true, string outfile = "out.xml") 
			: _isSax2(isSax2), m_filename(filename), m_schemaname(schemaname), _outfile(outfile) { Init();}
		virtual ~VocabularyXmlAccess();
		virtual void Load(Vocabulary& voc);
		virtual void Save(Vocabulary& voc);
		void SetFilename(const char* filename);
		string GetFilename();
		void SetSchemaname(const char* schemaname);
		string GetSchemaname();
		void SaveAs(Vocabulary& voc, const char* filename);
		//parse the xml document with schema, return the true of false
		//serilizae xml 
		//parses
		void ParseWithDomLS(const char* filename, const char* schamefile, Vocabulary& voc);
		void ParseWithSax2(const char* filename, const char* schamefile, Vocabulary& voc);
	private:
		xercesc_3_1::DOMImplementation* m_impl;
		xercesc_3_1::DOMImplementation* m_docImpl;
		xercesc_3_1::DOMLSParser* m_domparser;
		xercesc_3_1::DOMLSSerializer* m_serializer;
		string m_filename;
		string _outfile;
		string m_schemaname;
		bool _isSax2;
	};

	inline void VocabularyXmlAccess::SetFilename(const char* filename) {
		m_filename = filename;
	}

	inline string VocabularyXmlAccess::GetFilename() {
		return m_filename;
	}

	inline void VocabularyXmlAccess::SetSchemaname(const char* schemaname) {
		m_schemaname = schemaname;
	}

	inline string VocabularyXmlAccess::GetSchemaname() {
		return m_schemaname;
	}
}
#endif