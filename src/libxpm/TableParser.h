#ifndef TABLE_PARSER_H
#define TABLE_PARSER_H

#include <vector>
#include <string>
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
		// 1 as found, 0 as not found
		int contain_iid(QTextBlock& b, sfa_model& m, string iid);
		// 1 as exactly match, 2 as partly match, 0 as no match
		int contain_iids(QTextBlock& b, sfa_model& m, vector<string>& iids, vector<string>& out_iids);
	private:
		int parse_head(QTextTable* t, sfa_model& model, sfa_sign& r);
		int parse_body(QTextTable* t, sfa_model& model, sfa_sign& r);
	};

	
}
#endif
