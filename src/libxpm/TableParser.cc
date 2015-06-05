#include "TableParser.h"
#include <algorithm>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextTable>
#include <QTextTableCell>
#include "sfa_parser.h"

namespace xpm {

	int TableParser::parse(QTextDocument* doc, sfa_model& model, sfa_sign& root) {
		_count = 0;
		_toksuper = &root;
		_toklast = 0;
		QTextBlock b = doc->begin();
		QTextTable* t = 0;
		while (b.isValid()) {
			QTextCursor c(b);
			QTextTable* t2 = c.currentTable();
			if (t2 && t2 != t) {
				t = t2;
				QTextBlock b1 = t->cellAt(0, 0).firstCursorPosition().block();
				wstring cell1 = b1.text().toStdWString();
				// context with id: 0x5107df009291
				if ( contain_iid(b1, model, "0x5107df009291"))
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
		int result = 0;
		// add a new root object
		map<wstring, sfa_sign*> objects;
		sfa_sign* token = 0;
		for (int i = 0; i < rows; ++i){
			QTextBlock b1 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text1 = b1.text().toStdWString();
			QTextBlock b2 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text2 = b2.text().toStdWString();
			QTextBlock b3 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text3 = b3.text().toStdWString();
			// precondition of text1 is empty
			if (text1.empty()){
				if (_toklast == 0)
					return -1;
			}
			// precondition of find the object in cache
			else if (objects.find(text1) != objects.end()){
				_toklast = objects[text1];
				_toklast->set_type(SFA_OBJECT);
				_toksuper = _toklast;
			}
			// precondition of toally new object
			else {
				_toklast = new sfa_sign(SFA_OBJECT);
				_toksuper->push_back(_toklast);
				_toksuper = _toklast;
			}
			
			//precondition of instance of a type
			if (text2.empty()){
				if (!text1.empty())
					return -1;
				if (text3.empty())
					return 0;
			}else{
				// precondition of a member of a array
				if (i < rows && t->cellAt(i + 1, 1).firstCursorPosition().block().text().isEmpty()
					&& !t->cellAt(i + 1, 2).firstCursorPosition().block().text().isEmpty()){
					++_count;
					token = new sfa_sign(SFA_ARRAY);
					token->set_value_start(b2.position());
					token->set_value_last(b2.position() + b2.length());
					token->set_name_str(text2);
					_toksuper->push_back(token);
					_toklast = token;
				}
				else{
					++_count;
					token = new sfa_sign(SFA_EMPTY);
					token->set_name_start(b2.position());
					token->set_name_last(b2.position() + b2.length());
					token->set_name_str(text2);
					_toksuper->push_back(token);
					_toklast = token;					
				}
			}
						
			if (text3.empty()) {
				return -1;
			}
			else {
				_pos = 0;
				for (; _pos < text3.size(); ++_pos){
					wchar_t c = text3[_pos];
					switch (c){
					case '\t': case '\r': case '\n': case ' ':
						break;
					case '-': case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						result = parse_primitive(text3);
						if (result < 0) return result;
						break;
					default:
						/* is the name if the type of last token is Object, NUMBER, BOOL*/
						if (_toklast->type() == SFA_EMPTY){
							_toklast->set_type(SFA_STRING);
							_toklast->set_value_start(b3.position());
							_toklast->set_value_last(b3.position()+b3.length());
							_toklast->set_value_string(text3);
						}
						else {
							++_count;
							token = new sfa_sign(SFA_STRING);
							token->set_value_start(b3.position());
							token->set_value_last(b3.position()+b3.length());
							token->set_value_string(text3);
							_toksuper->push_back(token);
							_toklast = token;
						}						
						objects[text3] = _toklast;
					}
				}
			}			
		}
		sfa_parser parser;
		return parser.parse_atom_signs(model, r);
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

	int TableParser::contain_iids(QTextBlock& b, sfa_model&m) {
		vector<sfa_map> maps = m.find_maps(b.position(), b.length());
		if (maps.size() > 0)
			return 1;
		else
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
		std::set_intersection(miids.begin(), miids.end(), iids.begin(), iids.end(), out_iids.begin());		
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

	int TableParser::parse_primitive(const wstring& syntax) {
		sfa_sign *token;
		int start = _pos;
		int type = SFA_NUMBER;
		for (; _pos < syntax.size(); ++_pos) {
			if (syntax[_pos] < 32 || syntax[_pos] >= 127) {
				return SFA_ERROR_INVAL;
			}			
		}
		/* found */
		if (_toklast->type() == SFA_EMPTY) {
			token = _toklast;
			token->set_type(type);
			token->set_value_start(start);
			token->set_value_last(_pos);
		}
		else {
			++_count;
			token = new sfa_sign(type);
			token->set_value_start(start);
			token->set_value_last(_pos);
			_toksuper->push_back(token);
			_toklast = token;
		}
		if (type == SFA_NUMBER) {
			token->set_value_number(_wtof(syntax.substr(start, _pos - start).c_str()));
		}
		return 1;		
	}
}
