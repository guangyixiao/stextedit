#include "XpmTextEdit.h"
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <QTextBlock>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QCompleter>
#include <QClipboard>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QModelIndex>
#include <QMenu>
#include <QToolTip>
#include <QTime>
#include <QString>
#include <qheaderview.h>
#include "model/Term.h"
#include "StringCovert.h"


namespace xpm {
	//XpmTextEdit::XpmTextEdit(XpmTermDocControl* control, QWidget* parent)
	XpmTextEdit::XpmTextEdit(XpmTableDocControl* control, QWidget* parent)
		:QTextEdit(parent), 
		_xpmControl(control),
		_tableview(0),
		_blockPosition(0),
		_startPos(0),
		_c(0){
			//setup hightlight format
			//setup hightlight format
			//_highlightFormat.setUnderlineColor(Qt::blue);
			//_highlightFormat.setUnderlineStyle(QTextCharFormat::DashUnderline);
			//_dishighlightFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
			QFont font(tr("Times New Roman"), 12);
			setFont(font);
			//setup hightlight format
		    _highlightFormat.setUnderlineColor(Qt::black);
		    _highlightFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
			_disHighlightFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
		//_highlightFormat.setForeground(Qt::blue);
		//_dishighlightFormat.setForeground(Qt::black);
			connect(document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(termsChange(int, int, int)));
			//for complement
			setCompleter();
			setAttribute(Qt::WA_InputMethodEnabled, true);
			// construct the iid copy action
			_copyiid = new QAction(tr("copy iid"), this);
			connect(_copyiid, SIGNAL(triggered()), this, SLOT(copyIid()));
	}
	XpmTextEdit::~XpmTextEdit() {
		if(_c) {
			delete _c;
			_c = NULL;
			cout << "Normal delete _completer" << endl;
		}
	}
	void XpmTextEdit::setCompleter() {
		if(!_c)  {
			TermControl* control = _xpmControl->termControl();
			setCompleter(new XpmCompleter(control, this));
		}
	}
	void XpmTextEdit::setCompleter(XpmCompleter *completer){
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
		QObject::connect(_c, SIGNAL(activatedTerm(conex::Term*)), this, SLOT(insertTerm(conex::Term*)));
	}	

	XpmCompleter *XpmTextEdit::completer() const{
		return _c;
	}

	void XpmTextEdit::insertTerm(conex::Term* term) {	
		if (_c->widget() != this)
			return;
		//get the completion string
		wstring completion_str = term->indexWord(); 
		wcout << L"completion :" << completion_str << endl;
		QString completion(StringCovert::stdWToQString(completion_str));
		//merge the format of prefix chars

		/*cursor.movePosition(QTextCursor::Left);
		cursor.select(QTextCursor::WordUnderCursor);
		cursor.mergeCharFormat(_highlightFormat);
		cursor.movePosition(QTextCursor::Right);*/

		//insert the remain of the completion with the term format
		// complete
		//int extra = completion.length() - _c->completionPrefix().length();
		//textCursor().insertText(completion.right(extra), _highlightFormat);	

		//// highlight selection string
		//QTextCursor cursor(textCursor());
		//cursor.setPosition(_startPos);
		//cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, completion_str.size());
		//cursor.setCharFormat(_highlightFormat);
		// replace complete
        QTextCursor cursor(textCursor());
		int position = cursor.position();
		cursor.setPosition(_startPos);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, (position - _startPos));
		cursor.insertText(completion);
		// call back the addchars method
		// textChange(addchars)
		// merge the last format
		cursor.setPosition(_startPos);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, completion_str.size());
		cursor.setCharFormat(_highlightFormat);
		//need to get the current block and block position
		//int length = completion.length();
		//maintain the term consistence of two document models
		//RemainRangePair pair =  _xpmdoc.GetXpmTermDoc().InsertTermRange(_startPos, _blockPosition, term);
		sfa_ranges pair = _xpmControl->add_map(_startPos, completion_str, term->id()->iid(), _blockPosition);

		//need to activ remain range
		dishighlightRemainRangePair(_startPos, pair);
		//we are going to insert a space for the format consistence.
		//textCursor().insertText(tr(" "), _disHighlightFormat);
		//textCursor().setCharFormat(_disHighlightFormat);

		//refreseh the map
		setCurrentCharFormat(_disHighlightFormat);
		emit refreshMap();
	}

	void XpmTextEdit::clear() {
		setCurrentCharFormat(_disHighlightFormat);		
	}

	void XpmTextEdit::formatMaps() {
		for( sfa_blocks::iterator it = _xpmControl->model()->blocks().begin(); it != _xpmControl->model()->blocks().end(); ++it)  {
			for( sfa_maps::iterator tit = (*it)->maps().begin(); tit != (*it)->maps().end(); ++tit) {
				QTextCursor cursor(textCursor());
				cursor.setPosition((*it)->ix() + tit->word_ix);
				cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, tit->word.length());
				cursor.setCharFormat(_highlightFormat);
			}
		}
	}

	QString XpmTextEdit::textUnderCursor(){
		QTextCursor cursor(textCursor());
		cursor.select(QTextCursor::WordUnderCursor);
		QString text = cursor.selectedText();
		_startPos = cursor.selectionStart();
		_blockPosition = cursor.block().position();
		return text;
	}

	void XpmTextEdit::termsChange(int position, int charsRemoved, int charsAdded) {
		if(charsRemoved == charsAdded) {
			return;
		}else {
			sfa_ranges pair;
			int delt = charsAdded - charsRemoved;
			if(delt < 0) {
				//pair = _xpmdoc.GetXpmTermDoc().RemoveCharsInPosRange(position, charsRemoved);
				QTime tm;
		        tm.start();
				position += charsAdded; 
				pair = _xpmControl->delete_chars(position, -delt);
				int millseconds = tm.elapsed();
				cout << "Remove Chars to Model, chars:" << -delt << ",time:" << millseconds << endl;
			}else if(delt > 0) {
				//pair =  _xpmdoc.GetXpmTermDoc().AddCharsInPosRange(position, charsAdded);				
				QTime tm;
		        tm.start();
				position += charsRemoved;
				dishighlightAddedChars(position, charsAdded);
				pair = _xpmControl->insert_chars(position, delt);
				int millseconds = tm.elapsed();
				cout << "Add Chars to Model, chars :" << delt << ",time:" << millseconds << endl;
			}
			dishighlightRemainRangePair(position, pair);
			emit refreshMap();
		}
	}

	void XpmTextEdit::dishighlightAddedChars(int position, int charsAdded) {
		QTextCursor cursor(textCursor());
		cursor.setPosition(position);
		cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, charsAdded);
		cursor.mergeCharFormat(_disHighlightFormat);
	}

	void XpmTextEdit::dishighlightRemainRangePair(int position, sfa_ranges& pair) {		
		sfa_range left = pair.first;
		sfa_range right = pair.second;
		QTextCursor cursor(textCursor());
		if(left.len > 0 || right.len > 0) {
			if(left.len > 0) {		
				cursor.setPosition(left.pos);
				cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, left.len);
				cursor.mergeCharFormat(_disHighlightFormat);
				//_cursor->
			}
			if(right.len > 0) {
				cursor.setPosition(right.pos);
				cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, right.len);;
				cursor.mergeCharFormat(_disHighlightFormat);
			}	
			//fuck , what's the wrong of the problem!
		}
	}

	QMimeData * XpmTextEdit::createMimeDataFromSelection() const {
		return QTextEdit::createMimeDataFromSelection();		
	}

	void XpmTextEdit::insertFromMimeData ( const QMimeData * source ) {
		QTextCursor cursor(textCursor());
		int start = cursor.selectionStart();
		int blockpos = cursor.block().position();
		QTextEdit::insertFromMimeData(source);
		/*if ( source->text().length() == _xpmclipboard.length())
		    insertXpmDoc(start, blockpos);*/
	}

	bool XpmTextEdit::event(QEvent* event) {
		if (event->type() == QEvent::ToolTip) {
			QHelpEvent* helpEvent = static_cast <QHelpEvent*>(event);
			QTextCursor cursor = cursorForPosition(helpEvent->pos());
			//TermRange termRange = _xpmdoc.GetXpmTermDoc().GetTermRangeFromPos(cursor.position());
			sfa_map* map = _xpmControl->current_map(cursor.position());
			cout << "current position :" << cursor.position() << endl;
			if (map && map->id.length() > 0) {
				TermControl* control = _xpmControl->termControl();
				Term* t = control->GetTermByIid(map->id);
				if(t) {					
					QString text  = StringCovert::stdWToQString(t->text());
					/*sfa s;
					string source = text.toUtf8();
					text = QString::fromUtf8(s.parse_source(source).content.c_str());*/
					QToolTip::showText(helpEvent->globalPos(), text);
				}
			}else {
				QToolTip::hideText();
			}
		}
		return QTextEdit::event(event);
	}

	void XpmTextEdit::focusInEvent(QFocusEvent *e){
		if (_c)
			_c->setWidget(this);
		QTextEdit::focusInEvent(e);
	}

	void XpmTextEdit::copyXpmDoc() {
		//_xpmclipboard.clear();
		QTextCursor cursor(textCursor());
		int start = cursor.selectionStart();
		int end = cursor.selectionEnd();
		//TermBlocks blocks = _xpmdoc.GetXpmTermDoc().GetXpmBlocksFromSelection(start, end);
		//_xpmControl->GetXpmBlocksFromSelection(start, end, &_xpmclipboard);
	}

	void XpmTextEdit::insertXpmDoc(int start, int blockpos) {		
		//_xpmdoc.GetXpmTermDoc().InsertXpmBlocks(start, _xpmclipboard.length(), blocks);
		//_xpmControl->InsertXpmBlocks(blockpos, start,  &_xpmclipboard);
		emit refreshMap();
	}

	void XpmTextEdit::insertSpacesAsBellow() {
		QTextCursor cursor(textCursor());
		string block_str = cursor.block().text().toStdString();
		std::size_t found = block_str.find_first_not_of("\t ");
		string text = "\n";
		if(found != std::string::npos || block_str.find_first_of("\t ") != std::string::npos) {
			text += block_str.substr(0, found);
		}
		cursor.insertText(QString::fromStdString(text.c_str()));
	}

	void XpmTextEdit::keyPressEvent(QKeyEvent *e){
		
		// begin the completer operation
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

		// begin the copy operation		
		if(e->type() == QKeyEvent::KeyPress) {
			if( e->matches(QKeySequence::Copy)) {
				copyXpmDoc();
				QTextEdit::keyPressEvent(e);
				return;
			}else if(e->key() == Qt::Key_Return) {
				insertSpacesAsBellow();
				return;
			}
		}

		bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Right); // CTRL+->
		if (!_c || !isShortcut) // do not process the shortcut when we have a completer
			QTextEdit::keyPressEvent(e);

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

	void XpmTextEdit::inputMethodEvent(QInputMethodEvent * e) {		
		QTextEdit::inputMethodEvent(e);	
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

	QVariant XpmTextEdit::inputMethodQuery( Qt::InputMethodQuery property ) const {
		return QTextEdit::inputMethodQuery(property);
	}

	void XpmTextEdit::contextMenuEvent(QContextMenuEvent* e) {
		QMenu *menu = createStandardContextMenu();
		QTextCursor cursor = cursorForPosition(e->pos());
		//TermRange termRange = _xpmdoc.GetXpmTermDoc().GetTermRangeFromPos(cursor.position());
		sfa_map* map = _xpmControl->current_map(cursor.position());
		if (map && map->id.length() > 0) {
			menu->addSeparator();
			menu->addAction(_copyiid);
			_iid = map->id;
		}		
		menu->exec(e->globalPos());
		delete menu;
	}

	void XpmTextEdit::copyIid() {
		QClipboard* clipboard = QApplication::clipboard();
		clipboard->setText(QString::fromStdString (_iid));
	}
}