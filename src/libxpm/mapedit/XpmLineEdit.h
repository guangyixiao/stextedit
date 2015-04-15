#ifndef XPM_DOC_GUI_XPM_LINE_EDIT_H
#define XPM_DOC_GUI_XPM_LINE_EDIT_H

#include <QLineEdit>
#include <QApplication>
#include <QTableView>
#include "XpmCompleter.h"
#include "control/TermControl.h"


using namespace std;

namespace xpm {
	class XpmLineEdit: public QLineEdit {
		Q_OBJECT
	public:
		XpmLineEdit(conex::TermControl *_termControl, QWidget* parent = 0);
		virtual ~XpmLineEdit() ;		

		void setCompleter();
		void setCompleter(XpmCompleter *c);
		XpmCompleter* completer() const;
	protected:
		virtual void keyPressEvent( QKeyEvent *e);
		// override input method
		virtual void inputMethodEvent(QInputMethodEvent *e);
		virtual QVariant inputMethodQuery ( Qt::InputMethodQuery property ) const;
	public:
		QString textUnderCursor();
	private:
		XpmCompleter *_c;
		QTableView* _tableview;
		conex::TermControl *_termControl;
	};
}

#endif