#ifndef CONEX_DAL_VOCABULARY_DB_ACCESS_H
#define CONEX_DAL_VOCABULARY_DB_ACCESS_H
#include "dal/VocabularyAccess.h"
#include <iostream>
#include <string>

#define OTL_ODBC_MSSQL_2008 // Compile OTL 4/ODBC, MS SQL 2008
//#define OTL_ODBC // Compile OTL 4/ODBC. Uncomment this when used with MS SQL 7.0/ 2000

//#define OTL_UNICODE // Enable Unicode OTL 
#define OTL_ANSI_CPP // Turn on ANSI C++ typecasts
#define OTL_STL  // Turn on STL features
#include "otlv4.h"

using namespace std;

namespace conex {
	class VocabularyDbAccess : public VocabularyAccess{
	public:
		VocabularyDbAccess();
		virtual ~VocabularyDbAccess();
		virtual void Load(Vocabulary& voc);
		virtual void Save(Vocabulary& voc);
	protected:
		void LoadTerms(Vocabulary& voc);
		void DropAndCreateTable();
	private:
		otl_connect db; // connect object
	};
}
#endif