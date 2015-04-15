#include "XpmLineEdit.h"
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <QTextBlock>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QCompleter>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QModelIndex>
#include <qheaderview.h>
#include "model/Term.h"


namespace xpm {
	XpmLineEdit::XpmLineEdit(conex::TermControl* termControl, QWidget* parent)
		:QLineEdit(parent), 
		_termControl(termControl),
		_tableview(0),
		_c(0){
			//for complement
			setCompleter();
	}
	XpmLineEdit::~XpmLineEdit() {
		if(_c) {
			delete _c;
			_c = NULL;
			cout << "Normal delete _completer" << endl;
		}
	}
	void XpmLineEdit::setCompleter() {
		if(!_c) 
			setCompleter(new XpmCompleter(_termControl, this));
	}
	void XpmLineEdit::setCompleter(XpmCompleter *completer){
		if (_c)
			QObject::disconnect(_c, 0, this, 0);

		_c = completer;

		if (!_c)
			return;

		//_tableview.hidh
		_c->setWidget(this);
		if(!_tableview) {
			_tableview = new QTableView;
			_tableview->horizontalHeader()->hide();
			_c->setPopup(_tableview);
		}
		_c->setCompletionMode(QCompleter::PopupCompletion);
		//_c->setCaseSensitivity(Qt::CaseInsensitive);
		//QObject::connect(&_tableview,  SIGNAL(activated(const QModelIndex &)), _c, SLOT(selectedIndex(const QModelIndex&)));
		QObject::connect(_c, SIGNAL(activated(const QModelIndex &)), _c, SLOT(selectedIndex(const QModelIndex&)));
	}	

	XpmCompleter *XpmLineEdit::completer() const{
		return _c;
	}

	QString XpmLineEdit::textUnderCursor(){
		QString text = this->text();
		return text;
	}	

	void XpmLineEdit::keyPressEvent(QKeyEvent *e){
		if (_c && _c->popup()->isVisible()) {
			// The following keys are forwarded by the completer to the widget
			switch (e->key()) {
			case Qt::Key_Enter:
			case Qt::Key_Return:
			case Qt::Key_Escape:
			case Qt::Key_Tab:
			case Qt::Key_Backtab:
				e->ignore();
				return; // let the completer do default behavior
			default:
				break;
			}
		}

		bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Right); // CTRL+->
		if (!_c || !isShortcut) // do not process the shortcut when we have a completer
			QLineEdit::keyPressEvent(e);

		const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
		if (!_c || (ctrlOrShift && e->text().isEmpty()))
			return;

		static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
		bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
		QString completionPrefix = textUnderCursor();

		if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
			|| eow.contains(e->text().right(1)))) {
				_c->popup()->hide();
				return;
		}

		if (completionPrefix != _c->completionPrefix()) {
			_c->setCompletionPrefix(completionPrefix);
			if(_tableview)
				_tableview->resizeColumnsToContents();
			_c->popup()->setCurrentIndex(_c->completionModel()->index(0, 0));
		}
		QRect cr = cursorRect();
		cr.setWidth(_c->popup()->sizeHintForColumn(0)
			+ _c->popup()->sizeHintForColumn(1)
			+ _c->popup()->verticalScrollBar()->sizeHint().width());
		_c->complete(cr); // popup it up!
	}

	void XpmLineEdit::inputMethodEvent(QInputMethodEvent * e) {		
		QLineEdit::inputMethodEvent(e);	
		if (e->commitString().size() > 0) {
			cout << "input length: " << e->replacementLength() << endl;
			// begin the completer operation

			static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
			QString completionPrefix = textUnderCursor();

			if ( completionPrefix.length() < 1
				) {
					_c->popup()->hide();
					return;
			}

			if (completionPrefix != _c->completionPrefix()) {
				_c->setCompletionPrefix(completionPrefix);
				if(_tableview)
					_tableview->resizeColumnsToContents();
				_c->popup()->setCurrentIndex(_c->completionModel()->index(0, 0));
			}
			QRect cr = cursorRect();
			cr.setWidth(_c->popup()->sizeHintForColumn(0)
				+ _c->popup()->sizeHintForColumn(1)
				+ _c->popup()->verticalScrollBar()->sizeHint().width());
			_c->complete(cr); // popup it up!
		}
	}

	QVariant XpmLineEdit::inputMethodQuery( Qt::InputMethodQuery property ) const {
		return QLineEdit::inputMethodQuery(property);
	}
}