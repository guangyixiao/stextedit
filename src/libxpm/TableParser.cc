#include "TableParser.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QTextTable>
#include <QTextTableCell>

namespace xpm {

	int TableParser::parse(QTextDocument* doc, sfa_model& model, sfa_sign& root) {
		QTextBlock b = doc->begin();
		QTextTable* t = 0;
		while (b.isValid()) {
			QTextCursor c(b);
			QTextTable* t2 = c.currentTable();
			if (t2 && t2 != t) {
				t = t2;
				wstring cell1 = t->cellAt(0, 0).firstCursorPosition().block().text().toStdWString();
				if ( cell1 == L"context" || cell1 == L"±³¾°")
					parse_head(t, model, root);
				else
					parse_body(t, model, root);
			}
			b = b.next();
		}

	}

	int TableParser::parse_body(QTextTable* t, sfa_model& model, sfa_sign& r) {
		int rows = t->rows();
		int cols = t->columns();
		if (cols != 3)
			return -1;
		// add a new root object
		map<wstring, sfa_sign*> objects;
		sfa_sign* farther = &r;
		sfa_sign* curr = 0;
		for (int i = 0; i < rows; ++i){
			QTextBlock b1 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text1 = b1.text().toStdWString();
			if (text1.size() > 0 && objects[text1] != 0){
				curr = new sfa_sign(SFA_OBJECT);
				curr->set_value_string(text1);
				farther->push_back(curr);
			}
			else {
				curr = objects[text1];
				curr->set_type(SFA_OBJECT);
			}
			QTextBlock b2 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text2 = b2.text().toStdWString();
			if (text2 != L"type" && !text2.empty()){
				farther = curr;
				curr = new sfa_sign(SFA_EMPTY);
			}
			else if (text2.empty()){

			}
			else{

			}

			QTextBlock b3 = t->cellAt(i, 0).firstCursorPosition().block();

		}
	}
	
	int TableParser::parse_head(QTextTable* t, sfa_model& model, sfa_sign& r) {


	}
}
