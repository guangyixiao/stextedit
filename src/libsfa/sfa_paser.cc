#include "SDFSignControl.h"
#include <cstdlib>

namespace xpm {

	int SDFSignControl::parse(const wstring& syntax,   const XpmTermDoc& doc, SDFSign& root) {
		_count = 0;
		// reverse process
		_toksuper = &root;
		_toklast = &root;
		SDFSign* token = 0;
		int r ;
		for(; _pos < syntax.size(); ++_pos) {
			wchar_t c = syntax[_pos];
			int type = SDF_NULL; 
			switch (c) {
			case '{': case '[':
				type = c == '{' ? SDF_OBJECT : SDF_ARRAY;
				if(_toklast->type() == SDF_EMPTY) {
					/* is string reification if the last token is NULL or ARRAY*/ 				
					token = _toklast;
					token->set_type(type);
					token->set_v_start(_pos);
				}else {
					/* is the key if the type of last token is Object, NUMBER, BOOL*/				
					_count++;					
					token = new SDFSign(type);
					_toklast = token;
					if ( _toksuper != 0) {
						_toksuper->push_back(token);
					}
					token->set_v_start(_pos);
				}
				_toksuper = _toklast;
				break;
			case '}': case ']':
				type = c == '}' ? SDF_OBJECT : SDF_ARRAY;
				token = _toklast;
				// so this is a loop
				for (;;) {
					if (token->v_start() != -1 && token->v_end() == -1) {
						if ( token->type() != type) {
							return SDF_ERROR_INVAL;
						}
						token->set_v_end(_pos + 1);
						_toksuper = token->parent();
						break;
					}
					if ( token->parent() == &root) {
						break;
					}
					token = token->parent();
					// this is very important
				}
				break;
			case '\"':
				// r = parse_string(parse, js, len, tokens, num_tokens);
				r = parse_string(syntax);
				if (r < 0) return r;
				break;
			case '\t': case '\r': case '\n': case ' ':
				break;
			case ':':
				if (_toklast->type() != SDF_EMPTY)
					return SDF_ERROR_PART_KEY;
				break;
			case ',':
				if(_toklast->type() == SDF_EMPTY ) {
					_toklast->set_type(SDF_STRING);
					_toklast->set_rei_string(_toklast->de_str());
					_toklast->set_de_str(L"");
				}
				break;
			case '-': case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case 't': case 'f': case 'n':
				// r = parse_primitive(parse, js, len, tokens, num_tokens);
				r = parse_primitive(syntax);
				if (r<0) return r;
				break;
			default:
				return SDF_ERROR_INVAL;
			}
		}
		return parse_atom_signs(doc, root);
	}

	int SDFSignControl::parse_string(const wstring& syntax) {
		SDFSign *token;
		int start = _pos;
		++_pos;
		for(;_pos < syntax.size();++_pos) {
			wchar_t c = syntax[_pos];

			/* Quote: end of string */
			if (c == '\"') {
				if(_toklast->type() == SDF_EMPTY) {
					/* is string reification if the last token is NULL or ARRAY*/ 				
					token = _toklast;
					token->set_type(SDF_STRING);
					token->set_v_start(start+1);
					token->set_v_end(_pos);
					token->set_rei_string(syntax.substr(start+1, _pos-start-1));
				}else {
					/* is the key if the type of last token is Object, NUMBER, BOOL*/				
					_count++;
					token = new SDFSign(SDF_EMPTY);
					token->set_s_start(start+1);
					token->set_s_end(_pos);
					token->set_de_str(syntax.substr(start+1, _pos-start-1));
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
					for(int i = 0; i<4 && syntax[_pos] != '\0';++i) {
						/* If it isn't a hex character we have an error */
						if(!((syntax[_pos] >= 48 && syntax[_pos] <= 57) || /* 0-9 */
							(syntax[_pos] >= 65 && syntax[_pos] <= 70) || /* A-F */
							(syntax[_pos] >= 97 && syntax[_pos] <= 102))) { /* a-f */
								_pos = start;
								return SDF_ERROR_INVAL;
						}
						++_pos;
					}
					--_pos;
					break;
					/* Unexpected symbol */
				default:
					_pos = start;
					return SDF_ERROR_INVAL;
					
				}
			}
		}
		_pos = start;
		return SDF_ERROR_PART_STRING;
	}
	int SDFSignControl::parse_primitive(const wstring& syntax) {
		SDFSign *token;
		int start = _pos;
		int type = SDF_NUMBER;
		for(;_pos < syntax.size();++_pos) {
			wchar_t c = syntax[_pos];
			switch(c) {
			case 't':
				if (syntax.substr(_pos, 4) == L"true") 
					type = SDF_TRUE;
				break;
			case 'f':
				if (syntax.substr(_pos, 5) == L"false")
					type = SDF_FALSE;
				break;
			case 'n':
				if (syntax.substr(_pos, 4) == L"null")
					type = SDF_NULL;
				break;
			case ':':
			case '\t': case '\r': case '\n': case ' ':
			case ',': case ']': case '}':
				/* found */
				if(_toklast->type() == SDF_EMPTY && type != SDF_EMPTY) {
					token = _toklast;
					token->set_type(type);
					token->set_v_start(start);
					token->set_v_end(_pos);
				}else {
					++_count;
					token = new SDFSign(type);
					token->set_v_start(start);
					token->set_v_end(_pos);
					_toksuper->push_back(token);
					_toklast = token;
				}
				if (type == SDF_NUMBER) {
					token->set_rei_number(_wtof(syntax.substr(start, _pos - start).c_str()));
				}
				_pos--;
				return 0;
				break;
			}
			if (syntax[_pos] < 32 || syntax[_pos] >= 127) {
				_pos =start;
				return SDF_ERROR_INVAL;
			}
		}
		/* In strict mode primitive must be followed by a comma/object/array */
		_pos = start;
		return SDF_ERROR_PART_PRIMITIVE;
	}

	int SDFSignControl::parse_atom_signs(const XpmTermDoc& doc, SDFSign& r) {
		/* Check the error of the whole signs*/ 
		SDFSign* token = r.next_start();
		while(token != 0) {
			if ( token->v_start() != -1 && token->v_end() == -1) {
				return SDF_ERROR_PART_STRING;
			}
			if ( token->s_start() != -1 && token->s_end() == -1) {
				return SDF_ERROR_PART_OBJECT_ARRAY;
			}
			if ( token->de_str().size() > 0) {
				// process in denotation
				int start = token->s_start();
				int len = token->s_end() - start;
				vector<TermRange> trs = doc.findTermRanges(start, len);
				for(vector<TermRange>::iterator it = trs.begin() ; it != trs.end(); ++it) {
					token->de_append_map(it->posInBlock(), it->text(), it->term());
				}
				// needs to evaluate all words mapping in future
			}
			if ( token->type() == SDF_STRING && token->rei_as_string().size() > 0) {
				// process in reification
				int start = token->v_start();
				int len = token->v_end() - start;
				vector<TermRange> trs = doc.findTermRanges(start, len);
				for(vector<TermRange>::iterator it = trs.begin() ; it != trs.end(); ++it) {
					token->rei_append_map(it->posInBlock(), it->text(), it->term());
				}
			}
			token = r.next();

		};
		int count = _count;
		clear();
		return count;
	}
}