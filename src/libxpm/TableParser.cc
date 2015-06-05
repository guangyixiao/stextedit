#include "TableParser.h"
#include <algorithm>
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
				QTextBlock b1 = t->cellAt(0, 0).firstCursorPosition().block();
				wstring cell1 = b1.text().toStdWString();
				if ( cell1 == L"context" || cell1 == L"±³¾°")
					parse_head(t, model, root);
				else
					parse_body(t, model, root);
			}
			b = b.next();
		}
		return 1;
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
			// precondition of text1 is empty
			if (text1.empty()){
				if (curr == 0)
					break;
			}
			// precondition of find the object in cache
			else if (objects.find(text1) != objects.end()){
				curr = objects[text1];
				curr->set_type(SFA_OBJECT);
			}
			// precondition of toally new object
			else {
				curr = new sfa_sign(SFA_OBJECT);
				curr->set_value_string(text1);
				farther->push_back(curr);
			}
			QTextBlock b2 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text2 = b2.text().toStdWString();
			//precondition of instance of a type
			if (text2 != L"type" && !text2.empty()){
				farther = curr;
				curr = new sfa_sign(SFA_EMPTY);
			}
			// precondition of a member of a array
			else if (text2.empty()){

			}
			// precondition of a new name, value pair of a object
			else{

			}

			QTextBlock b3 = t->cellAt(i, 0).firstCursorPosition().block();
			//precondition of string			
			//precondition of bool
			//precondition of null
			//precondition of number
			//precondition of object
		}
	}

	int TableParser::contain_iid(QTextBlock& b, sfa_model& m, string iid) {
		vector<sfa_map> maps = m.find_maps(b.position(), b.length());
		if (maps.size() == 0)
			return 0;
		for (vector<sfa_map>::iterator i = maps.begin(); i != maps.end(); ++i){
			if (i->id == iid)
				return 1;
		}
		return 0;
	}
	
	int TableParser::contain_iids(QTextBlock& b, sfa_model& m, vector<string>& iids, vector<string>& out_iids) {
		
		vector<sfa_map> maps = m.find_maps(b.position(), b.length());
		if (iids.size() == 0 || maps.size() == 0)
			return 0;
		vector<string> miids;
		for (vector<sfa_map>::iterator i = maps.begin(); i != maps.end(); ++i){
			miids.push_back(i->id);
		}
		std::sort(miids.begin(), miids.end());
		std::sort(iids.begin(), iids.end());
		//std::set_intersection(miids.begin(), miids.end(), iids.begin(), iids.end(), out_iids);		
		if (out_iids.size() == 0)
			return 0;
		else if (out_iids.size() == iids.size())
			return 1;
		else
			return 2;
	}

	int TableParser::parse_head(QTextTable* t, sfa_model& model, sfa_sign& r) {
		return 0;

	}
}
