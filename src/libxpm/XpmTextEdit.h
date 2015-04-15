#ifndef XPM_DOC_GUI_XPM_EDIT_H
#define XPM_DOC_GUI_XPM_EDIT_H

#include <QTextEdit>
#include <QTableView>
#include <QApplication>
#include "XpmTableDocControl.h"
#include "XpmCompleter.h"
//#include "model/XpmClipboard.h"


using namespace std;

namespace xpm {
	class XpmTextEdit: public QTextEdit {
		Q_OBJECT
	public:
		//XpmTextEdit(XpmTermDocControl* control, QWidget* parent = 0);
		XpmTextEdit(XpmTableDocControl* control,  QWidget* parent = 0);
		virtual ~XpmTextEdit() ;		

		void setCompleter();
		void setCompleter(XpmCompleter *c);
		XpmCompleter* completer() const;
		//XpmClipboard& xpmclipboard();
		//XpmTermDocControl* docControl();
		//XpmDocControl* docControl();
		//void setDocControl(XpmTermDocControl* control);
		//void setDocControl(XpmDocControl* control);
		void copyXpmDoc();
		void insertXpmDoc(int start, int blockpos);
		void updateCompleter();
		void clear();
	    void formatMaps();
	protected:
		virtual void keyPressEvent( QKeyEvent *e);
		virtual void focusInEvent(QFocusEvent *e);
		// override copy operation
		virtual QMimeData* createMimeDataFromSelection() const;
		// override paste operation
		virtual void insertFromMimeData ( const QMimeData * source );
		virtual bool event(QEvent* event);
		// override input method
		virtual void inputMethodEvent(QInputMethodEvent *e);
		virtual QVariant inputMethodQuery ( Qt::InputMethodQuery property ) const;
	private slots:
		void insertTerm(conex::Term* term);
		void termsChange(int position, int charsRemoved, int charsAdded);
    signals:
		void refreshMap();
	public:
		QString textUnderCursor();
		void dishighlightRemainRangePair(int position, sfa_ranges& pair);
		void dishighlightAddedChars(int position, int charsAdded);
	private:
		// insert spaces as bellow
		void insertSpacesAsBellow();
		QTableView* _tableview;
		//XpmTermDocControl* _xpmControl;
		XpmTableDocControl* _xpmControl;
		//XpmClipboard _xpmclipboard;
		XpmCompleter *_c;
		QTextCharFormat _highlightFormat;
		QTextCharFormat _disHighlightFormat;
		int _blockPosition;
		int _startPos;	
		int _endPos;
	};

	/*inline XpmClipboard& XpmTextEdit::xpmclipboard() {
		return _xpmclipboard;
	}*/

	inline void XpmTextEdit::updateCompleter() {
		TermControl* control = _xpmControl->termControl();
		completer()->setTermControl(control);
	}
}

#endif