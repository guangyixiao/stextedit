#include "TableParser.h"
#include <algorithm>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextTable>
#include <QTextTableCell>
#include "sfa_parser.h"

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
		// add a new root object
		map<wstring, sfa_sign*> objects;
		_toksuper = &r;
		_toklast = 0;
		for (int i = 0; i < rows; ++i){
			QTextBlock b1 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text1 = b1.text().toStdWString();
			// precondition of text1 is empty
			if (text1.empty()){
				if (_toklast == 0)
					break;
			}
			// precondition of find the object in cache
			else if (objects.find(text1) != objects.end()){
				_toklast = objects[text1];
				_toklast->set_type(SFA_OBJECT);
			}
			// precondition of toally new object
			else {
				_toklast = new sfa_sign(SFA_OBJECT);
				_toksuper->push_back(_toklast);
				_toksuper = _toklast;
			}
			QTextBlock b2 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text2 = b2.text().toStdWString();
			//precondition of instance of a type
			if (text2.empty()){
				if (!text1.empty())
					return -1;
				// precondition of a member of a array
				else {
					if (i > 1 && !t->cellAt(i - 1, 1).firstCursorPosition().block().text().isEmpty()){
						_toksuper = _toklast;
						_toklast = new sfa_sign(*_toksuper);
						_toksuper->push_back(_toklast);
						_toksuper->set_type(SFA_ARRAY);
						_toklast->name_clear();
					}
				}
			}
			// precondition of a new name, value pair of a object
			else {
				_toklast = new sfa_sign(SFA_EMPTY);
				_toklast->set_name_str(text2);
				_toklast->set_name_start(b2.position());
				_toklast->set_name_last(b2.position() + b2.length());
			}			

			QTextBlock b3 = t->cellAt(i, 0).firstCursorPosition().block();
			wstring text3 = b3.text().toStdWString();
			if (text3.empty()) {
				if (text2.empty())
					break;
				else
					return -1;
			}
			else {
				_pos = 0;
				for (; _pos < text3.size(); ++_pos){
					wchar_t c = text3[_pos];
					switch (c){
						case 
					}
				}
			}
			//precondition of string			
			//precondition of bool
			//precondition of null
			//precondition of number
			//precondition of object
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

	int TableParser::parse_string(const wstring& syntax) {
		sfa_sign *token;
		int start = _pos;
		++_pos;
		for (; _pos < syntax.size(); ++_pos) {
			wchar_t c = syntax[_pos];

			/* Quote: end of string */
			if (c == '\"') {
				if (_toklast->type() == SFA_EMPTY) {
					/* is string value if the last token is NULL or ARRAY*/
					token = _toklast;
					token->set_type(SFA_STRING);
					token->set_value_start(start + 1);
					token->set_value_last(_pos);
					token->set_value_string(syntax.substr(start + 1, _pos - start - 1));
				}
				else {
					/* is the name if the type of last token is Object, NUMBER, BOOL*/
					_count++;
					token = new sfa_sign(SFA_EMPTY);
					token->set_name_start(start + 1);
					token->set_name_last(_pos);
					token->set_name_str(syntax.substr(start + 1, _pos - start - 1));
					_toksuper->push_back(token);
					_toklast = token;
				}
				return 0;
			}

			/* Backslash: Quoted symbol expected */
			if (c == '\\') {
				++_pos;
				switch (syntax[_pos]) {
					/* Allowed escaped symbols */
				case '\"': case '/': case '\\': case 'b':
				case 'f': case 'r': case 'n': case 't':
					break;
					/* Allows escaped symbol \uXXXX */
				case 'u':
					++_pos;
					for (int i = 0; i < 4 && syntax[_pos] != '\0'; ++i) {
						/* If it isn't a hex character we have an error */
						if (!((syntax[_pos] >= 48 && syntax[_pos] <= 57) || /* 0-9 */
							(syntax[_pos] >= 65 && syntax[_pos] <= 70) || /* A-F */
							(syntax[_pos] >= 97 && syntax[_pos] <= 102))) { /* a-f */
							_pos = start;
							return SFA_ERROR_INVAL;
						}
						++_pos;
					}
					--_pos;
					break;
					/* Unexpected symbol */
				default:
					_pos = start;
					return SFA_ERROR_INVAL;

				}
			}
		}
		_pos = start;
		return SFA_ERROR_PART_STRING;
	}
	int TableParser::parse_primitive(const wstring& syntax) {
		sfa_sign *token;
		int start = _pos;
		int type = SFA_NUMBER;
		for (; _pos < syntax.size(); ++_pos) {
			wchar_t c = syntax[_pos];
			switch (c) {
			case 't':
				if (syntax.substr(_pos, 4) == L"true")
					type = SFA_TRUE;
				break;
			case 'f':
				if (syntax.substr(_pos, 5) == L"false")
					type = SFA_FALSE;
				break;
			case 'n':
				if (syntax.substr(_pos, 4) == L"null")
					type = SFA_NULL;
				break;
			case ':':
			case '\t': case '\r': case '\n': case ' ':
			case ',': case ']': case '}':
				/* found */
				if (_toklast->type() == SFA_EMPTY && type != SFA_EMPTY) {
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
				_pos--;
				return 0;
				break;
			}
			if (syntax[_pos] < 32 || syntax[_pos] >= 127) {
				_pos = start;
				return SFA_ERROR_INVAL;
			}
		}
		/* In strict mode primitive must be followed by a comma/object/array */
		_pos = start;
		return SFA_ERROR_PART_PRIMITIVE;
	}
}
