#ifndef TABLE_PARSER_H
#define TABLE_PARSER_H

#include <vector>
#include <qtextdocument>
#include "sfa_sign.h"
#include "StringCovert.h"
#include "XpmTextEdit.h"


using namespace std;

namespace xpm {
	
	class TableParser{
	public:
		TableParser() {}
		int parse(QTextDocument* doc, sfa_model& model, sfa_sign& r);	
	private:
		int parse_head(QTextTable* t, sfa_model& model, sfa_sign& r);
		int parse_body(QTextTable* t, sfa_model& model, sfa_sign& r);
	};

	
}
#endif
