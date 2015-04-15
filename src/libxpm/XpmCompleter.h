#ifndef XPM_GUI_XPM_COMPLETER_H
#define XPM_GUI_XPM_COMPLETER_H

#include <string>
#include <QCompleter>
#include <QApplication>
#include <QString>
#include <QModelIndex>
#include "control/TermControl.h"
#include "control/VocabularyXmlControl.h"
#include "model/Vocabulary.h"
#include "model/Term.h"
#include "VocabularyTableModel.h"

namespace xpm {
	using namespace std;

	class XpmCompleter : public QCompleter{
		Q_OBJECT
	public:
		XpmCompleter(conex::TermControl* termControl, QObject * parent = 0);
		virtual ~XpmCompleter();
		QString completionPrefix() const;
		void setCompletionPrefix(const QString & prefix);
		void setTermControl(conex::TermControl* termControl);
	public slots:
		void selectedIndex(const QModelIndex& index );
	signals:
		void activatedTerm(conex::Term* term);	
	private:
		QString _prefix;
		conex::TermControl* _termControl;
		VocabularyTableModel* _model;
	};

	inline QString XpmCompleter::completionPrefix() const {
		return _prefix;
	}	
	inline void XpmCompleter::setTermControl(conex::TermControl* termControl) {
		_termControl = termControl;
	}
}
#endif