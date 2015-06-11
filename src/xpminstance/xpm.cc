#include "Xpm.h"

#include <iostream>
#include <string>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QVector>

#include "StringCovert.h"
#include "sfatree/sfatree.h"

namespace xpm {
#ifdef Q_WS_MAC
	const QString rsrcPath = ":/images/mac";
#else
	const QString rsrcPath = ":/images/win";
#endif

#ifdef NDEBUG
	const QString enPath = "/data/Vocabulary_en_1.0.0.xml";
	const QString chsPath = "/data/Vocabulary_zh_1.0.0.xml";
#else
	const QString enPath = "/data/Vocabulary_en_0.1.0.xml";
	const QString chsPath = "/data/Vocabulary_zh_0.1.0.xml";
#endif

	Xpm::Xpm(QWidget *parent)
		: QMainWindow(parent),
		_termFactory((QApplication::applicationDirPath() + enPath).toStdString(), (QApplication::applicationDirPath() + chsPath).toStdString()),
		//_translator(&_termFactory),
		textEdit(0)
	{
		setToolButtonStyle(Qt::ToolButtonFollowStyle);
		setupFileActions();
		setupTemplateActions();
		setupEditActions();		
		setupTextActions();		
				
		//Xpm = new QTextEdit(this);
		//we need the xpm control right now and make use the loaded vocabulary
		xpmControl = new XpmTableDocControl(&_termFactory);
		//xpmControl = new sfa();
		//loadVoc();
		//create dock widget
		createMapDockWindow();
		setupViewActions();
		setupTableActions();	
		setupSimuActions();
		

		{
			QMenu *helpMenu = new QMenu(tr("Help"), this);
			menuBar()->addMenu(helpMenu);
			helpMenu->addAction(tr("About"), this, SLOT(about()));
			helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
		}
		//we need 2 second to load the vocabulary
		textEdit = new XpmTextEdit(xpmControl);
		connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
			this, SLOT(currentCharFormatChanged(QTextCharFormat)));
		connect(textEdit, SIGNAL(cursorPositionChanged()),
			this, SLOT(cursorPositionChanged()));
		connect(textEdit, SIGNAL(refreshMap()), mapEdit, SLOT(viewMap()));
		connect(mapEdit, SIGNAL(highlightSelection(int, int)), this, SLOT(highlightSelection(int,int)));
		connect(mapEdit, SIGNAL(disHighlightSelection(int, int)), this, SLOT(disHighlightSelection(int, int)));
		connect(mapEdit, SIGNAL(selectMapRange(int, int)), this, SLOT(selectMapRange(int, int)));

		setCentralWidget(textEdit);
		textEdit->setFocus();
		setCurrentFileName(QString());

		fontChanged(textEdit->font());
		colorChanged(textEdit->textColor());
		alignmentChanged(textEdit->alignment());

		connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
			actionSave, SLOT(setEnabled(bool)));
		connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
			this, SLOT(setWindowModified(bool)));
		connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
			actionUndo, SLOT(setEnabled(bool)));
		connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
			actionRedo, SLOT(setEnabled(bool)));

		setWindowModified(textEdit->document()->isModified());
		actionSave->setEnabled(textEdit->document()->isModified());
		actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
		actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

		connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
		connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

		actionCut->setEnabled(true);
		actionCopy->setEnabled(true);

		connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
		connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
		connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

		connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
		connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
		connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
#endif
		
		_highlightFormat.setUnderlineColor(Qt::black);
		_highlightFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		_dishighlightFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);

		QString initialFile = QApplication::applicationDirPath() + "/data/example_en.sebd";
		const QStringList args = QCoreApplication::arguments();
		if (args.count() == 2) {
			initialFile = args.at(1);			
		}
		if (!loadTable(initialFile))
			fileNewEn();

		//setup hightlight format
		
		//_highlightFormat.setForeground(Qt::blue);
		//_dishighlightFormat.setForeground(Qt::black);
	}

	Xpm::~Xpm() {
	}

	void Xpm::closeEvent(QCloseEvent *e)
	{
		if (maybeSave())
			e->accept();
		else
			e->ignore();
	}

	void Xpm::setupFileActions()
	{
		QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("File Actions"));
		addToolBar(tb);

		QMenu *menu = new QMenu(tr("&File"), this);
		menuBar()->addMenu(menu);

		QAction *a;

		QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
		a = new QAction( newIcon, tr("&New En"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(QKeySequence::New);
		connect(a, SIGNAL(triggered()), this, SLOT(fileNewEn()));
		tb->addAction(a);
		menu->addAction(a);

		QIcon chsIcon = QIcon::fromTheme("document-new-chs", QIcon(rsrcPath + "/filenew_chs.png"));
		a = new QAction( chsIcon, tr("&New Chs"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(Qt::CTRL + Qt::Key_E);
		connect(a, SIGNAL(triggered()), this, SLOT(fileNewChs()));
		tb->addAction(a);
		menu->addAction(a);

		a = new QAction(QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png")),
			tr("&Open..."), this);
		a->setShortcut(QKeySequence::Open);
		//connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
		connect(a, SIGNAL(triggered()), this, SLOT(fileOpenTable()));
		tb->addAction(a);
		menu->addAction(a);

		menu->addSeparator();

		actionSave = a = new QAction(QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png")),
			tr("&Save"), this);
		a->setShortcut(QKeySequence::Save);
		//connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
		connect(a, SIGNAL(triggered()), this, SLOT(fileSaveTable()));
		a->setEnabled(false);
		tb->addAction(a);
		menu->addAction(a);

		a = new QAction(tr("Save &As..."), this);
		a->setPriority(QAction::LowPriority);
		//connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
		connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAsTable()));
		menu->addAction(a);
		menu->addSeparator();

#ifndef QT_NO_PRINTER
		a = new QAction(QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png")),
			tr("&Print..."), this);
		a->setPriority(QAction::LowPriority);    
		a->setShortcut(QKeySequence::Print);
		connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
		tb->addAction(a);
		menu->addAction(a);

		a = new QAction(QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png")),
			tr("Print Preview..."), this);
		connect(a, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
		menu->addAction(a);

		a = new QAction(QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png")),
			tr("&Export PDF..."), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(Qt::CTRL + Qt::Key_D);
		connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
		tb->addAction(a);
		menu->addAction(a);

		menu->addSeparator();
#endif

		a = new QAction(tr("&Quit"), this);
		a->setShortcut(Qt::CTRL + Qt::Key_Q);
		connect(a, SIGNAL(triggered()), this, SLOT(close()));
		menu->addAction(a);
	}

	void Xpm::setupTemplateActions()
	{
		/*QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("Template Actions"));
		addToolBar(tb);*/

		QMenu *menu = new QMenu(tr("&Template"), this);
		menuBar()->addMenu(menu);

		QAction *a;
		

		a = new QAction(QIcon::fromTheme("template-open", QIcon(rsrcPath + "/fileopen.png")),
			tr("Open &Template..."), this);
		a->setShortcut(QKeySequence::Open);
		//connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
		connect(a, SIGNAL(triggered()), this, SLOT(fileOpenTemplate()));
		//tb->addAction(a);
		menu->addAction(a);

		a = new QAction(QIcon::fromTheme("reification-open", QIcon(rsrcPath + "/fileopen.png")),
			tr("Open &Reification..."), this);
		a->setShortcut(QKeySequence::Open);
		//connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
		connect(a, SIGNAL(triggered()), this, SLOT(fileOpenReification()));
		//tb->addAction(a);
		menu->addAction(a);

		menu->addSeparator();
	}

	void Xpm::setupEditActions()
	{
		QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("Edit Actions"));
		addToolBar(tb);
		QMenu *menu = new QMenu(tr("&Edit"), this);
		menuBar()->addMenu(menu);

		QAction *a;
		a = actionUndo = new QAction(QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png")),
			tr("&Undo"), this);
		a->setShortcut(QKeySequence::Undo);
		tb->addAction(a);
		menu->addAction(a);
		a = actionRedo = new QAction(QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png")),
			tr("&Redo"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(QKeySequence::Redo);
		tb->addAction(a);
		menu->addAction(a);
		menu->addSeparator();
		a = actionCut = new QAction(QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png")),
			tr("Cu&t"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(QKeySequence::Cut);
		tb->addAction(a);
		menu->addAction(a);
		a = actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png")),
			tr("&Copy"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(QKeySequence::Copy);
		tb->addAction(a);
		menu->addAction(a);
		a = actionPaste = new QAction(QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png")),
			tr("&Paste"), this);
		a->setPriority(QAction::LowPriority);
		a->setShortcut(QKeySequence::Paste);
		tb->addAction(a);
		menu->addAction(a);
#ifndef QT_NO_CLIPBOARD
		actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
	}

	void Xpm::setupTextActions()
	{
		QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("Format Actions"));
		addToolBar(tb);

		QMenu *menu = new QMenu(tr("F&ormat"), this);
		menuBar()->addMenu(menu);

		actionTextBold = new QAction(QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png")),
			tr("&Bold"), this);
		actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
		actionTextBold->setPriority(QAction::LowPriority);
		QFont bold;
		bold.setBold(true);
		actionTextBold->setFont(bold);
		connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
		tb->addAction(actionTextBold);
		menu->addAction(actionTextBold);
		actionTextBold->setCheckable(true);

		actionTextItalic = new QAction(QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png")),
			tr("&Italic"), this);
		actionTextItalic->setPriority(QAction::LowPriority);
		actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
		QFont italic;
		italic.setItalic(true);
		actionTextItalic->setFont(italic);
		connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
		tb->addAction(actionTextItalic);
		menu->addAction(actionTextItalic);
		actionTextItalic->setCheckable(true);

		actionTextUnderline = new QAction(QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png")),
			tr("&Underline"), this);
		actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
		actionTextUnderline->setPriority(QAction::LowPriority);
		QFont underline;
		underline.setUnderline(true);
		actionTextUnderline->setFont(underline);
		connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
		tb->addAction(actionTextUnderline);
		menu->addAction(actionTextUnderline);
		actionTextUnderline->setCheckable(true);

		menu->addSeparator();

		QActionGroup *grp = new QActionGroup(this);
		connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

		// Make sure the alignLeft  is always left of the alignRight
		if (QApplication::isLeftToRight()) {
			actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")),
				tr("&Left"), grp);
			actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), tr("C&enter"), grp);
			actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), tr("&Right"), grp);
		} else {
			actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), tr("&Right"), grp);
			actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), tr("C&enter"), grp);
			actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")), tr("&Left"), grp);
		}
		actionAlignJustify = new QAction(QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png")), tr("&Justify"), grp);

		actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
		actionAlignLeft->setCheckable(true);
		actionAlignLeft->setPriority(QAction::LowPriority);
		actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
		actionAlignCenter->setCheckable(true);
		actionAlignCenter->setPriority(QAction::LowPriority);
		actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
		actionAlignRight->setCheckable(true);
		actionAlignRight->setPriority(QAction::LowPriority);
		actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
		actionAlignJustify->setCheckable(true);
		actionAlignJustify->setPriority(QAction::LowPriority);

		tb->addActions(grp->actions());
		menu->addActions(grp->actions());

		menu->addSeparator();

		QPixmap pix(16, 16);
		pix.fill(Qt::black);
		actionTextColor = new QAction(pix, tr("&Color..."), this);
		connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
		tb->addAction(actionTextColor);
		menu->addAction(actionTextColor);

		// maps format
		menu->addSeparator();

		// add format maps action
		QPixmap pix2(16, 16);
		pix2.fill(Qt::blue);
		actionFormatMaps = new QAction(pix2, tr("&Format Maps..."), this);
		connect(actionFormatMaps, SIGNAL(triggered()), this, SLOT(formatMaps()));
		//tb->addAction(actionFormatMaps);
		menu->addAction(actionFormatMaps);		
		// clear maps format
		// add format maps action
		QPixmap pix3(16, 16);
		pix3.fill(Qt::white);
		actionClearFormatMaps = new QAction(pix3, tr("&Clear Format Maps..."), this);
		connect(actionClearFormatMaps, SIGNAL(triggered()), this, SLOT(clearMapsFormats()));
		//tb->addAction(actionClearFormatMaps);
		menu->addAction(actionClearFormatMaps);	

		tb = new QToolBar(this);
		tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
		tb->setWindowTitle(tr("Format Actions"));
		addToolBarBreak(Qt::TopToolBarArea);
		addToolBar(tb);

		comboStyle = new QComboBox(tb);
		tb->addWidget(comboStyle);
		comboStyle->addItem("Standard");
		comboStyle->addItem("Bullet List (Disc)");
		comboStyle->addItem("Bullet List (Circle)");
		comboStyle->addItem("Bullet List (Square)");
		comboStyle->addItem("Ordered List (Decimal)");
		comboStyle->addItem("Ordered List (Alpha lower)");
		comboStyle->addItem("Ordered List (Alpha upper)");
		comboStyle->addItem("Ordered List (Roman lower)");
		comboStyle->addItem("Ordered List (Roman upper)");
		connect(comboStyle, SIGNAL(activated(int)),
			this, SLOT(textStyle(int)));

		comboFont = new QFontComboBox(tb);
		tb->addWidget(comboFont);
		connect(comboFont, SIGNAL(activated(QString)),
			this, SLOT(textFamily(QString)));

		comboSize = new QComboBox(tb);
		comboSize->setObjectName("comboSize");
		tb->addWidget(comboSize);
		comboSize->setEditable(true);

		QFontDatabase db;
		foreach(int size, db.standardSizes())
			comboSize->addItem(QString::number(size));

		connect(comboSize, SIGNAL(activated(QString)),
			this, SLOT(textSize(QString)));
		comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
			.pointSize())));

		
	}

	void Xpm::createMapDockWindow() {
		mapDock = new QDockWidget(tr("Map Edit"), this);
		mapDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		mapEdit = new MapEditor(xpmControl, mapDock);		
		mapDock->setWidget(mapEdit);
		addDockWidget(Qt::RightDockWidgetArea, mapDock);		
	}

	void Xpm::setupViewActions() {
		QMenu *menu = new QMenu(tr("&View"), this);
		menuBar()->addMenu(menu);

		menu->addAction(mapDock->toggleViewAction());
	}

	void Xpm::setupTableActions()
	{
		QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("Table Actions"));
		addToolBar(tb);

		QMenu *menu = new QMenu(tr("&Table"), this);
		menuBar()->addMenu(menu);
		
		/*actionTableInsert = new QAction(QIcon::fromTheme("table-insert", QIcon(rsrcPath + "/tableinsert.png")),
			tr("&Table"), this);
		actionTextBold->setShortcut(Qt::CTRL + Qt::Key_T);
		actionTextBold->setPriority(QAction::LowPriority);
		QFont bold;
		bold.setBold(true);
		actionTableInsert->setFont(bold);
		connect(actionTableInsert, SIGNAL(triggered()), this, SLOT(tableInsert()));
		tb->addAction(actionTableInsert);
		menu->addAction(actionTableInsert);

		menu->addSeparator();		
		tb->addSeparator();*/

		// for insert chars and remove chare experiments
		actionAddRow = new QAction(QIcon::fromTheme("row-add", QIcon(rsrcPath + "/window_add.png")),
			tr("&Add Row"), this);
		connect(actionAddRow, SIGNAL(triggered()), this, SLOT(addRow()));
		tb->addAction(actionAddRow);
		menu->addAction(actionAddRow);

		actionRemoveRow = new QAction(QIcon::fromTheme("row-remove", QIcon(rsrcPath + "/window_remove.png")),
			tr("&Remove Row"), this);
		connect(actionRemoveRow, SIGNAL(triggered()), this, SLOT(removeRow()));
		tb->addAction(actionRemoveRow);
		menu->addAction(actionRemoveRow);

		menu->addSeparator();	

	}

	void Xpm::setupSimuActions()
	{
		QToolBar *tb = new QToolBar(this);
		tb->setWindowTitle(tr("Simulation Actions"));
		addToolBar(tb);

		QMenu *menu = new QMenu(tr("&Simulation"), this);
		menuBar()->addMenu(menu);

		// for translate doc from one lang to anther
		actionTranslate = new QAction(QIcon::fromTheme("translate", QIcon(rsrcPath + "/translate.png")),
			tr("&Translate"), this);
		connect(actionTranslate, SIGNAL(triggered()), this, SLOT(translate()));
		tb->addAction(actionTranslate);
		menu->addAction(actionTranslate);

		// for translate doc from one lang to anther
		actionParse = new QAction(QIcon::fromTheme("parse", QIcon(rsrcPath + "/parse.png")),
			tr("&Parse"), this);
		connect(actionParse, SIGNAL(triggered()), this, SLOT(parse()));
		tb->addAction(actionParse);
		menu->addAction(actionParse);

		menu->addSeparator();
		//tb->addSeparator();

		// for insert chars and remove chare experiments
		actionSimuAddChars = new QAction(QIcon::fromTheme("chars-add", QIcon(rsrcPath + "/window_add.png")),
			tr("&Add Chars"), this);
		connect(actionSimuAddChars, SIGNAL(triggered()), this, SLOT(addCharsInMiddle()));
		//tb->addAction(actionSimuAddChars);
		menu->addAction(actionSimuAddChars);

		actionSimuRemoveChars = new QAction(QIcon::fromTheme("chars-remove", QIcon(rsrcPath + "/window_remove.png")),
			tr("&Remove Chars"), this);
		connect(actionSimuRemoveChars, SIGNAL(triggered()), this, SLOT(removeCharsInMiddle()));
		//tb->addAction(actionSimuRemoveChars);
		menu->addAction(actionSimuRemoveChars);

		menu->addSeparator();		
	}

	bool Xpm::load(const QString &f)
	{
		if (!QFile::exists(f))
			return false;
		QFile file(f);
		if (!file.open(QFile::ReadOnly))
			return false;

		QByteArray data = file.readAll();
		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		QString str = codec->toUnicode(data);
		if (Qt::mightBeRichText(str)) {
			textEdit->setHtml(str);
		} else {
			str = QString::fromLocal8Bit(data);
			textEdit->setPlainText(str);
		}

		setCurrentFileName(f);
		//we need to load the semantic file with the same filename
		loadSemantic(f);
		mapEdit->viewMap();
		return true;
	}

	bool Xpm::loadInitTable(string lang) {
		QString initialFile = lang == "en" ? "data/xpm_en.sebd" : "data/xpm_chs.sebd";		
		bool result = loadTable(initialFile);
		setCurrentFileName(tr(""));
		return result;
	}

	bool Xpm::loadTable(const QString &f)
	{
		if (!QFile::exists(f))
			return false;
		QFile file(f);
		if (!file.open(QFile::ReadOnly))
			return false;

		setCurrentFileName(f);

		//string syntax = xpmControl->OpenDoc(f.toAscii().constData());

		QByteArray data = file.readAll();
		//QTextCodec *codec = Qt::codecForHtml(data);
		//QString str = codec->toUnicode(data);
		QString str = QString::fromUtf8(data);
		string source = str.toUtf8(); //StringCovert::qToStdWString( str );
		// get the syntax
		sfa_pair pair = xpmControl->parse_source(source);
		string content = pair.content;
		QString qcontent = QString::fromUtf8(content.c_str());     //(StringCovert::stdWToQString( syntax));
		//cout << "content :" << content << endl;
		// clear the text eidt and update the completer
		textEdit->clear();		

		if (Qt::mightBeRichText(qcontent)) {
			textEdit->setHtml(qcontent);
		} else {
			//str = QString::fromLocal8Bit(data);
			textEdit->setPlainText(qcontent);
		}

		setCurrentFileName(f);
		// get the signs
		if ( content.size() != source.size() ) {
			//we need to load the semantic file with the same filename
			// load the signs model
			cout << "parsing model." << endl;
			xpmControl->parse_model(pair.maps);
			// this is a very import process!!!! Don't forget it.
			textEdit->updateCompleter();
			cout << "parsed model." << endl;
			mapEdit->viewMap();
			formatMaps();
		}
		return true;
	}

	//void Xpm::loadVoc(QString filename) {
	//	string __filename = filename.toAscii().constData();
	//	if ( _vocControl.GetFile() != __filename) {
	//		//cout << "filename :" << __filename << endl;
	//		if ( _voc != 0 ) {
	//			delete _voc;
	//			_voc = 0;
	//		}
	//		_voc = new conex::Vocabulary();
	//		_vocControl.SetFile(__filename);
	//		_vocControl.Load(*_voc);
	//		_termControl.SetVocabulary(_voc);
	//		cout << "Load Finished. " << endl;
	//	}
	//}

	void Xpm::loadSemantic(const QString &f) {
		//f.sp
		string str_filename(f.toStdString());
		unsigned int len = str_filename.find_last_of(".");
		str_filename = str_filename.substr(0, len);
		str_filename.append( ".xpm");
		//xpmControl->OpenDoc(str_filename);
	}

	bool Xpm::maybeSave()
	{
		if (!textEdit->document()->isModified())
			return true;
		if (fileName.startsWith(QLatin1String(":/")))
			return true;
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Application"),
			tr("The document has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return fileSaveTable();
		else if (ret == QMessageBox::Cancel)
			return false;
		return true;
	}

	void Xpm::setCurrentFileName(const QString &fileName)
	{
		this->fileName = fileName;
		textEdit->document()->setModified(false);

		QString shownName;
		if (fileName.isEmpty())
			shownName = "untitled.txt";
		else
			shownName = QFileInfo(fileName).fileName();

		setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));
		setWindowModified(false);
	}

	void Xpm::fileNewChs()
	{
		if (maybeSave() && !loadInitTable("chs")) {
			xpmControl->model()->clear();
			xpmControl->model()->set_lang("chs");
			textEdit->clear();
			textEdit->updateCompleter();
			mapEdit->viewMap();
			setCurrentFileName(QString());
			
			// setup a table
			QVector<QTextLength> columnWidth;
			int rows = 5;
			int cols = 3;
			columnWidth.append(QTextLength(QTextLength::FixedLength, 200));
			columnWidth.append(QTextLength(QTextLength::FixedLength, 300));
			columnWidth.append(QTextLength(QTextLength::FixedLength, 300));
			QTextTableFormat tableformat;
			tableformat.setColumnWidthConstraints(columnWidth);
			tableformat.setAlignment(Qt::AlignCenter);
			tableformat.setWidth(textEdit->lineWidth());
			tableformat.setCellPadding(0);
			tableformat.setMargin(0);

			//tableformat.setwi
			QTextCursor cursor(textEdit->textCursor());
			cursor.insertText("This is the context of the instance document:");
			QTextTable* table = cursor.insertTable(rows, cols, tableformat);
			int position = table->cellAt(0, 0).firstPosition();
			cursor.setPosition(position);
			int blockpos = cursor.block().position();
			cursor.insertText(tr("context"));
			xpmControl->add_map(position, L"context", "201204251104431590", blockpos);
			highlightSelection(position, 7);

			position = table->cellAt(0, 1).firstPosition();
			cursor.setPosition(position);
			blockpos = cursor.block().position();
			cursor.insertText(tr("schema"));
			xpmControl->add_map(position, L"schema", "201204251104492678", blockpos);
			highlightSelection(position, 6);

			cursor.setPosition(table->cellAt(0, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/example.xpms"));
			
			position = table->cellAt(1, 1).firstPosition();
			cursor.setPosition(position);
			blockpos = cursor.block().position();
			cursor.insertText(tr("dependent object"));
			xpmControl->add_map(position,L"dependent", "201204251104436431", blockpos);
			highlightSelection(position, 9);

			position += 10; 
			xpmControl->add_map(position, L"object", "201204251104225947", blockpos);
			highlightSelection(position, 6);

			cursor.setPosition(table->cellAt(1, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/example.xpm01"));
			cursor.setPosition(table->cellAt(2, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/example.xpm02"));
			cursor.movePosition(QTextCursor::End);
			textEdit->setTextCursor(cursor);

			//tableformat.setwi
			rows = 50;
			//QTextCursor cursor(textEdit->textCursor());
			cursor.insertText("This is the content of the instance document:");
			table = cursor.insertTable(rows, cols, tableformat);
			cursor.setPosition(table->cellAt(0, 0).firstPosition());
			textEdit->setTextCursor(cursor);
		}
	}

	void Xpm::fileNewEn()
	{
		if (maybeSave() && !loadInitTable("en")) {			
			xpmControl->model()->clear();
			xpmControl->model()->set_lang("en");
			textEdit->clear();
			textEdit->updateCompleter();
			mapEdit->viewMap();
			setCurrentFileName(QString());
			// setup a table
			QVector<QTextLength> columnWidth;
			int rows = 5;
			int cols = 3;
			columnWidth.append(QTextLength(QTextLength::FixedLength, 200));
			columnWidth.append(QTextLength(QTextLength::FixedLength, 300));
			columnWidth.append(QTextLength(QTextLength::FixedLength, 300));
			QTextTableFormat tableformat;
			tableformat.setColumnWidthConstraints(columnWidth);
			tableformat.setAlignment(Qt::AlignCenter);
			tableformat.setWidth(textEdit->lineWidth());
			tableformat.setCellPadding(0);
			tableformat.setMargin(0);

			//tableformat.setwi
			QTextCursor cursor(textEdit->textCursor());
			cursor.insertText("This is the context of the instance document:");
			QTextTable* table = cursor.insertTable(rows, cols, tableformat);
			int position = table->cellAt(0, 0).firstPosition();
			cursor.setPosition(position);
			int blockpos = cursor.block().position();
			cursor.insertText(tr("context"));
			xpmControl->add_map(position,L"context", "201204251104431590", blockpos);
			highlightSelection(position, 7);

			position = table->cellAt(0, 1).firstPosition();
			cursor.setPosition(position);
			blockpos = cursor.block().position();
			cursor.insertText(tr("schema"));
			xpmControl->add_map(position, L"schema", "201204251104492678", blockpos);
			highlightSelection(position, 6);

			cursor.setPosition(table->cellAt(0, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/examples.xpms"));
			
			position = table->cellAt(1, 1).firstPosition();
			cursor.setPosition(position);
			blockpos = cursor.block().position();
			cursor.insertText(tr("dependent object"));
			xpmControl->add_map(position, L"dependent", "201204251104436431", blockpos);
			highlightSelection(position, 9);

			position += 10; 
			xpmControl->add_map(position, L"object", "201204251104225947", blockpos);
			highlightSelection(position, 6);

			cursor.setPosition(table->cellAt(1, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/example01.xpm"));
			cursor.setPosition(table->cellAt(2, 2).firstPosition());
			cursor.insertText(tr("http://xpmdoc.com/example01.xpm"));
			cursor.movePosition(QTextCursor::End);
			textEdit->setTextCursor(cursor);

			//tableformat.setwi
			rows = 50;
			//QTextCursor cursor(textEdit->textCursor());
			cursor.insertText("This is the content of the instance document:");
			table = cursor.insertTable(rows, cols, tableformat);
			cursor.setPosition(table->cellAt(0, 0).firstPosition());
			textEdit->setTextCursor(cursor);
		}
	}

	void Xpm::fileOpen()
	{
		QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
			QString(), tr("HTML-Files (*.htm *.html);;Text-Files (*.txt);;All Files (*)"));
		if (!fn.isEmpty())
			load(fn);
	}

	void Xpm::fileOpenTable()
	{
		QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
			QString(), tr("SEBD_Files(*.sebd);;HTML_Files (*.htm *.html);;Json-Files (*.json);;Text-Files (*.txt);;All Files (*)"));
		if (!fn.isEmpty())
			loadTable(fn);
	}

	void Xpm::fileOpenTemplate() {
		QString tempFile = QApplication::applicationDirPath() + "/data/templateExample.sebd";
		loadTable(tempFile);
	}

	void Xpm::fileOpenReification() {
		QString tempFile = QApplication::applicationDirPath() + "/data / reificationExample.sebd";
		loadTable(tempFile);
	}

	bool Xpm::saveSemantic() {
		string str_filename(fileName.toStdString());
		unsigned int len = str_filename.find_last_of(".");
		str_filename =str_filename.substr(0, len);
		str_filename.append( ".xpm");
		//xpmControl->SaveDoc(str_filename);
		return true;
	}

	bool Xpm::fileSave()
	{
		if (fileName.isEmpty())
			return fileSaveAs();

		QTextDocumentWriter writer(fileName);
		bool success = writer.write(textEdit->document());
		if (success)
			textEdit->document()->setModified(false);
		success = saveSemantic();
		return success;
	}

	bool Xpm::fileSaveTable()
	{
		if (fileName.isEmpty())
			return fileSaveAsTable();

		QString qsyntax = textEdit->toHtml();
		//qsyntax.replace("p, li { white-space: pre-wrap; }", "");
		string syntax = qsyntax.toUtf8();  // StringCovert::qToStdWString(qsyntax);
		bool success = xpmControl->write_file(fileName.toStdString(),syntax);
		if (success)
			textEdit->document()->setModified(false);
		return success;
	}

	bool Xpm::fileSaveAs()
	{
		QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
			QString(), tr("ODF files (*.odt);;Text-Files (*.txt);;HTML-Files (*.htm *.html);;All Files (*)"));
		if (fn.isEmpty())
			return false;
		if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".txt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
			fn += ".odt"; // default
		setCurrentFileName(fn);
		return fileSave();
	}

	bool Xpm::fileSaveAsTable()
	{
		QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
			QString(), tr("SEBD-Files(*.sebd);;HTML-Files(*.htm *.html);;Json files (*.json);;Text-Files (*.txt);;All Files (*)"));
		if (fn.isEmpty())
			return false;
		if (! (fn.endsWith(".sebd", Qt::CaseInsensitive)) || (fn.endsWith(".html", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".json", Qt::CaseInsensitive) || fn.endsWith(".txt", Qt::CaseInsensitive)) )
			fn += ".html"; // default
		setCurrentFileName(fn);
		return fileSaveTable();
	}

	void Xpm::filePrint()
	{
#ifndef QT_NO_PRINTER
		QPrinter printer(QPrinter::HighResolution);
		QPrintDialog *dlg = new QPrintDialog(&printer, this);
		if (textEdit->textCursor().hasSelection())
			dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
		dlg->setWindowTitle(tr("Print Document"));
		if (dlg->exec() == QDialog::Accepted) {
			textEdit->print(&printer);
		}
		delete dlg;
#endif
	}

	void Xpm::filePrintPreview()
	{
#ifndef QT_NO_PRINTER
		QPrinter printer(QPrinter::HighResolution);
		QPrintPreviewDialog preview(&printer, this);
		connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
		preview.exec();
#endif
	}

	void Xpm::printPreview(QPrinter *printer)
	{
#ifdef QT_NO_PRINTER
		Q_UNUSED(printer);
#else
		textEdit->print(printer);
#endif
	}


	void Xpm::filePrintPdf()
	{
#ifndef QT_NO_PRINTER
		//! [0]
		QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
			QString(), "*.pdf");
		if (!fileName.isEmpty()) {
			if (QFileInfo(fileName).suffix().isEmpty())
				fileName.append(".pdf");
			QPrinter printer(QPrinter::HighResolution);
			printer.setOutputFormat(QPrinter::PdfFormat);
			printer.setOutputFileName(fileName);
			textEdit->document()->print(&printer);
		}
		//! [0]
#endif
	}

	void Xpm::textBold()
	{
		QTextCharFormat fmt;
		fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
		mergeFormatOnWordOrSelection(fmt);
	}

	void Xpm::textUnderline()
	{
		QTextCharFormat fmt;
		fmt.setFontUnderline(actionTextUnderline->isChecked());
		mergeFormatOnWordOrSelection(fmt);
	}

	void Xpm::textItalic()
	{
		QTextCharFormat fmt;
		fmt.setFontItalic(actionTextItalic->isChecked());
		mergeFormatOnWordOrSelection(fmt);
	}

	void Xpm::textFamily(const QString &f)
	{
		QTextCharFormat fmt;
		fmt.setFontFamily(f);
		mergeFormatOnWordOrSelection(fmt);
	}

	void Xpm::textSize(const QString &p)
	{
		qreal pointSize = p.toFloat();
		if (p.toFloat() > 0) {
			QTextCharFormat fmt;
			fmt.setFontPointSize(pointSize);
			mergeFormatOnWordOrSelection(fmt);
		}
	}

	void Xpm::textStyle(int styleIndex)
	{
		QTextCursor cursor = textEdit->textCursor();

		if (styleIndex != 0) {
			QTextListFormat::Style style = QTextListFormat::ListDisc;

			switch (styleIndex) {
			default:
			case 1:
				style = QTextListFormat::ListDisc;
				break;
			case 2:
				style = QTextListFormat::ListCircle;
				break;
			case 3:
				style = QTextListFormat::ListSquare;
				break;
			case 4:
				style = QTextListFormat::ListDecimal;
				break;
			case 5:
				style = QTextListFormat::ListLowerAlpha;
				break;
			case 6:
				style = QTextListFormat::ListUpperAlpha;
				break;
			case 7:
				style = QTextListFormat::ListLowerRoman;
				break;
			case 8:
				style = QTextListFormat::ListUpperRoman;
				break;
			}

			cursor.beginEditBlock();

			QTextBlockFormat blockFmt = cursor.blockFormat();

			QTextListFormat listFmt;

			if (cursor.currentList()) {
				listFmt = cursor.currentList()->format();
			} else {
				listFmt.setIndent(blockFmt.indent() + 1);
				blockFmt.setIndent(0);
				cursor.setBlockFormat(blockFmt);
			}

			listFmt.setStyle(style);

			cursor.createList(listFmt);

			cursor.endEditBlock();
		} else {
			// ####
			QTextBlockFormat bfmt;
			bfmt.setObjectIndex(-1);
			cursor.mergeBlockFormat(bfmt);
		}
	}

	void Xpm::textColor()
	{
		QColor col = QColorDialog::getColor(textEdit->textColor(), this);
		if (!col.isValid())
			return;
		QTextCharFormat fmt;
		fmt.setForeground(col);
		mergeFormatOnWordOrSelection(fmt);
		colorChanged(col);
	}

	void Xpm::textAlign(QAction *a)
	{
		if (a == actionAlignLeft)
			textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
		else if (a == actionAlignCenter)
			textEdit->setAlignment(Qt::AlignHCenter);
		else if (a == actionAlignRight)
			textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
		else if (a == actionAlignJustify)
			textEdit->setAlignment(Qt::AlignJustify);
	}

	void Xpm::currentCharFormatChanged(const QTextCharFormat &format)
	{
		fontChanged(format.font());
		colorChanged(format.foreground().color());
	}

	void Xpm::cursorPositionChanged()
	{
		alignmentChanged(textEdit->alignment());
	}

	void Xpm::clipboardDataChanged()
	{
#ifndef QT_NO_CLIPBOARD
		actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
	}

	void Xpm::about()
	{
		QMessageBox::about(this, tr("About"), tr("This example demonstrates Qt's "
			"rich text editing facilities in action, providing an example "
			"document for you to experiment with."));
	}

	void Xpm::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
	{
		QTextCursor cursor = textEdit->textCursor();
		if (!cursor.hasSelection())
			cursor.select(QTextCursor::WordUnderCursor);
		cursor.mergeCharFormat(format);
		textEdit->mergeCurrentCharFormat(format);
	}

	void Xpm::fontChanged(const QFont &f)
	{
		comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
		comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
		actionTextBold->setChecked(f.bold());
		actionTextItalic->setChecked(f.italic());
		actionTextUnderline->setChecked(f.underline());
	}

	void Xpm::colorChanged(const QColor &c)
	{
		QPixmap pix(16, 16);
		pix.fill(c);
		actionTextColor->setIcon(pix);
	}

	void Xpm::alignmentChanged(Qt::Alignment a)
	{
		if (a & Qt::AlignLeft) {
			actionAlignLeft->setChecked(true);
		} else if (a & Qt::AlignHCenter) {
			actionAlignCenter->setChecked(true);
		} else if (a & Qt::AlignRight) {
			actionAlignRight->setChecked(true);
		} else if (a & Qt::AlignJustify) {
			actionAlignJustify->setChecked(true);
		}
	}
	
	void Xpm::highlightSelection(int pos, int length) {
		QTextCursor cursor(textEdit->textCursor());
		cursor.setPosition(pos);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
		cursor.setCharFormat(_highlightFormat);
	}

	void Xpm::disHighlightSelection(int pos, int length) {
		QTextCursor cursor(textEdit->textCursor());
		cursor.setPosition(pos);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
		cursor.setCharFormat(_dishighlightFormat);
	}

	void Xpm::selectMapRange(int pos, int length) {
		QTextCursor cursor = textEdit->textCursor();
		cursor.setPosition(pos);
		cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
		textEdit->setTextCursor(cursor);
	}

	void Xpm::formatMaps() {
		for( sfa_blocks::iterator it = xpmControl->model()->blocks().begin(); it != xpmControl->model()->blocks().end(); ++it)  {
			for( sfa_maps::iterator tit = (*it)->maps().begin(); tit != (*it)->maps().end(); ++tit) {
				highlightSelection((*it)->ix() + tit->word_ix, tit->word.length());
			}
		}
	}

	void Xpm::clearMapsFormats() {
		for( sfa_blocks::iterator it = xpmControl->model()->blocks().begin(); it != xpmControl->model()->blocks().end(); ++it)  {
			for( sfa_maps::iterator tit = (*it)->maps().begin(); tit != (*it)->maps().end(); ++tit) {
				disHighlightSelection((*it)->ix() + tit->word_ix, tit->word.length());
			}
		}
	}

	void Xpm::addCharsInMiddle() {
		QString text = tr("Peirce himself, the main transitional figure in this area from a modern to a postmodern intellectual culture in philosophy,");
		text.append( "would likely have had little use in his own semiotic development for provincial narrowness in trying to eliminate or belittle");
		text.append( "the oldest name for semiotic study, even as he most emphatically rejected for semiotics the understanding of “doctrine” in the");
		text.append( "latter modern sense of “dogma” (as Bergman points out). Peirce’s whole idea for semiotics as the doctrine of signs was that semiosis");
		text.append( "would become the focal point for a community of inquirers, who would investigate the perfusion of signs throughout the universe for");
		text.append( "its own sake and according to its full requirements. (He would not likely have looked with admiration upon the development of a");
		text.append( "scholarly circle closed upon his personal work as something to be isolated from or within the larger semiotic community of inquiry.");
		QTextCursor cursor(textEdit->textCursor());
		int pos = textEdit->document()->characterCount()/2;
		cursor.setPosition(pos);
		QTime tm;
		int total = 0;
		for( int i = 0; i < 10; ++i) {
			tm.start();
			cursor.insertText(text);
			int millseconds = tm.elapsed();
			total += millseconds;
			cout << "Origin Add 1000 Chars in Middle -" << i << "th: " << millseconds << endl;
		}
		cout << "Origin Add 1000 Chars in Middle in total: " << total << endl;
	}

	void Xpm::removeCharsInMiddle() {
		QTextCursor cursor(textEdit->textCursor());		
		QTime tm;
		int total = 0;
		for( int i = 0; i < 10; ++i) {
			int pos = textEdit->document()->characterCount()/2;
		    cursor.setPosition(pos);
		    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1000);
			tm.start();
			cursor.removeSelectedText();
			int millseconds = tm.elapsed();
			total += millseconds;
			cout << "Origin Remove 1000 Chars in Middle -" << i << "th: " << millseconds << endl;
		}
		cout << "Origin Remove 1000 Chars in Middle in total: " << total << endl;
	}

	void Xpm::translate() {
		if(maybeSave()) {
			string out_filename(fileName.toStdString());
			unsigned int len = out_filename.find_last_of(".");
			out_filename = out_filename.substr(0, len);
			out_filename.append( "_2");
			// new a xpmdoc control
			sfa outControl;
			//wstring syntax = StringCovert::qToStdWString( textEdit->toHtml());
			QString str_source = textEdit->toHtml();
			QTextDocument target;
			target.setHtml(str_source);
			string initstr = target.toHtml().toUtf8();
			//cout << "copyed text document :" << initstr << endl;
			// translate to new _syntax and new xpmdoc of outcontrol
			//_translator.translateAll(syntax, xpmControl->xpmdoc(), _syntax, outControl.xpmdoc());
			StringCovert::translateAll(target, xpmControl->model(), outControl.model(), &_termFactory);
			string out_syntax = target.toHtml().toUtf8();
			// outcontrol will save its xpmdoc in the out_file
			out_filename += outControl.lang() + ".sebd";
			outControl.write_file(out_filename, out_syntax);
			QMessageBox mbox;
			string message = "Sucess translated to the file" + out_filename + "!";
			mbox.setText(message.c_str());
			mbox.setInformativeText("Do you want to open the translated document?");
			mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			mbox.setDefaultButton(QMessageBox::Yes);
			int ret = mbox.exec();
			switch (ret) {
			case QMessageBox::Yes:
				loadTable(tr(out_filename.c_str()));
				break;
			default:
				break;
			}
		}
	}

	void Xpm::parse() {
		if(maybeSave()) {
			//string out_filename(fileName.toAscii().constData());
			//unsigned int len = out_filename.find_last_of(".");
			//out_filename = out_filename.substr(0, len);
			// new a xpmdoc control
			// encoding is utf-16
			wstring syntax = StringCovert::qToStdWString( textEdit->toPlainText());
			// translate to new _syntax and new xpmdoc of outcontrol
			//_translator.translateAll(syntax, xpmControl->xpmdoc(), _syntax, outControl.xpmdoc());
			sfa_sign s(SFA_OBJECT) ;
			//int err = _signparser.parse(syntax, xpmControl->xpmdoc(), s);
			int err = _signparser.parse(syntax, *xpmControl->model(), s);
			if (err <= 0) {
				s.clear();
				err = _tableparser.parse(textEdit->document(), *xpmControl->model(), s);
			}
			// outcontrol will save its xpmdoc in the out_file
			if ( err > 0) {
				QMessageBox mbox;
				mbox.setText("Parsed sucess!");
				mbox.exec();
				xpm::SFATree tree(xpmControl->termControl(),&s, this);
				tree.exec();
			}else {
				QMessageBox mbox;
				char buffers[4];
				itoa(err, buffers, 10);
				string message = "Failed to parse since error code :";
				message.append(buffers);
				message.append(", message: ");
				switch(err) {
				case -1:
					message.append("Incorrect of Syntax");
					break;
				case -2:
					message.append("Part of Object or Array");
					break;
				case -3:
					message.append("Part of STRING");
					break;
				case -4:
					message.append("Part of PRIMITIVE");
					break;
				case -5:
					message.append("Without Denotation Error");
					break;
				default:
					message = "The syntax is empty";
				}
				mbox.setText(message.c_str());
				mbox.exec();
			}
		}
	}

	void Xpm::addRow() {
		QTextTable *tt = textEdit->textCursor().currentTable();    
		if (!tt)
			return;    
		QTextCursor cursor = textEdit->textCursor();    
		QTextTableCell cell = tt->cellAt(cursor); 
		tt->insertRows(cell.row()+1, 1);
	}

	void Xpm::removeRow() {
		QTextTable *tt = textEdit->textCursor().currentTable();    
		if (!tt)
			return;    
		QTextCursor cursor = textEdit->textCursor();    
		QTextTableCell cell = tt->cellAt(cursor); 
		tt->removeRows(cell.row(), 1);
	}
}//namespace xpm