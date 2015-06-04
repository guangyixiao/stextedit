#ifndef XPM_DOC_GUI_STRING_COVERT_H
#define XPM_DOC_GUI_STRING_COVERT_H

#include <QString>
#include <string>
#include <iostream>
#include "control/TermFactory.h"
#include "control/TermControl.h"
//#include "model/XpmTermDoc.h"
#include "sfa.h"
#include <QTextEdit>

using namespace std;

namespace xpm {
	class StringCovert{
	public:
		//XpmTextEdit(XpmTermDocControl* control, QWidget* parent = 0);
		static std::wstring qToStdWString(const QString &str)
		{
#ifdef _MSC_VER
			return (std::wstring((wchar_t*)str.utf16(), str.length()));
#else
			return str.toStdWString();
#endif
		}
		static QString stdWToQString(const std::wstring &str)
		{
#ifdef _MSC_VER
			return QString::fromUtf16((const ushort *)str.c_str());
#else
			return QString::fromStdWString(str);
#endif
		}
		static void translateAll(QTextDocument& target, sfa_model* doc, sfa_model* _doc, conex::TermFactory* _termFactory) {
			if (_termFactory != 0) {
				string lang = doc->lang() == "en" ? "chs" : "en";
				cout << "translate to :" << lang << endl;
				conex::TermControl* _termControl = _termFactory->termControl(lang);
				//wcout << "from: " << syntax << endl;

				//copy syntax to output
				//copy doc to _doc
				*_doc = *doc;
				//chang the lang to the other
				_doc->set_lang(lang);
				sfa_blocks blocks = _doc->blocks();

				// reverse process
				for (sfa_blocks::reverse_iterator it = blocks.rbegin(); it != blocks.rend(); ++it) {
					int bp = (*it)->ix();
					for (sfa_maps::reverse_iterator tit = (*it)->maps().rbegin(); tit != (*it)->maps().rend(); ++tit) {
						int pos = bp + tit->word_ix;
						wstring signifier = tit->word;  // utf-16
						string iid = tit->id;
						int len = signifier.length();
						conex::Term* term = _termControl->GetTermByIid(iid);
						if (term != 0) {
							// begin to translate singe term
							wstring word = _termControl->TopWord(term);
							// wcout << L"lang on top word:" << word << endl;
							int _len = word.length();

							// replace the text		
							QTextCursor cursor(&target);
							cursor.setPosition(pos);
							cursor.setPosition(pos + len, QTextCursor::KeepAnchor);

							cursor.insertText(stdWToQString(word));

							//_syntax.replace(pos, len, _text);

							/*TermRange range = *tit;
							range.setText(L"11");
							range.setTerm(term);*/
							// replace termrange
							tit->word = word;
							// iid is the same in general, it is not needed to modify
							// but the term needs change from one lang to anthoer
							tit->id = term->id()->iid();
							// process Add chars or Remove chars
							//cout << "length of tern range :" << tit->word.length() << endl;
							int delt = _len - len;
							int startpos = delt < 0 ? pos + _len : pos + len;
							if (delt > 0) {
								(*it)->insert_before(startpos, delt);
								_doc->insert_before(startpos, delt);
							}
							else if (delt < 0) {
								(*it)->delete_befoer(startpos, -delt);
								_doc->delete_befoer(startpos, -delt);
							}
							//cout << "length of tern range :" << tit->word.length() << endl;
							//wcout << "text of term range :" << tit->word << endl;
						}
					}
				}
				//wcout << "to: " << _syntax << endl;
			}
		}
	};
}

#endif