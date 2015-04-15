#ifndef CONEX_DAL_VOCABULARY_XML_ACCESS_H
#define CONEX_DAL_VOCABULARY_XML_ACCESS_H
#include "VocabularyAccess.h"
#include "xercesc/util/PlatformUtils.hpp"

#pragma comment(lib, "xerces-c_3.lib")

namespace conex {
	using namespace xercesc;

	class VocabularyXmlAccess : public VocabularyAccess{
	public:
		VocabularyXmlAccess() {m_filename = ""; m_schemaname = "";}
		VocabularyXmlAccess(char* filename, char* schemaname) {m_filename = filename; m_schemaname = schemaname; }
		virtual ~VocabularyXmlAccess();
		virtual void Load(Vocabulary& voc);
		virtual void Save(const Vocabulary& voc);
		void SetFilename(char* filename);
		void SetSchemaname(char* schemaname);
		void SaveAs(const Vocabulary& voc, char* filename);
		//parse the xml document with schema, return the true of false
		int Parse(char* filename, char* schemaname);
	private:
		char* m_filename;
		char* m_schemaname;
	};

	inline void VocabularyXmlAccess::SetFilename(char* filename) {
		m_filename = filename;
	}

	inline void VocabularyXmlAccess::SetSchemaname(char* schemaname) {
		m_schemaname = schemaname;
	}
}
#endif