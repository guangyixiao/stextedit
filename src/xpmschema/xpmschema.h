#ifndef XPMDOC_GUI_XPM_SCHEMA_H
#define XPMDOC_GUI_XPM_SCHEMA_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMap>
#include <QPointer>
#include <qprinter.h>
#include "XpmTextEdit.h"
#include "mapedit/mapeditor.h"
//#include "XpmTermDocControl.h"
#include "XpmTableDocControl.h"
#include "sfa_parser.h"
#include "TableParser.h"

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QMenu)

namespace xpm {
	
	using namespace std;

	class XpmSchema : public QMainWindow
	{
		Q_OBJECT

	public:
		XpmSchema(QWidget *parent = 0);
		~XpmSchema();
		// for experiments


	protected:
		virtual void closeEvent(QCloseEvent *e);
		
	private:
		void setupFileActions();
		void setupEditActions();
		void setupTextActions();
		void setupTableActions();
		void setupViewActions();
		void setupSimuActions();
		void createMapDockWindow();
		bool load(const QString &f);
		// load json
		bool loadTable(const QString &f);	
		bool loadInitTable(string lang);
		//void loadVoc(QString filename = QApplication::applicationDirPath() + tr("/data/Vocabulary_zh_1.0.xml"));
		bool maybeSave();
		void setCurrentFileName(const QString &fileName);
		void loadSemantic(const QString &f);
		bool saveSemantic();

		private slots:
			void fileNewChs();
			void fileNewEn();			
			void fileOpen();
			//open json
			void fileOpenTable();
			bool fileSave();
			// file save json
			bool fileSaveTable();
			bool fileSaveAs();
			// file save as json
			bool fileSaveAsTable();
			void filePrint();
			void filePrintPreview();
			void filePrintPdf();

			void textBold();
			void textUnderline();
			void textItalic();
			void textFamily(const QString &f);
			void textSize(const QString &p);
			void textStyle(int styleIndex);
			void textColor();
			void textAlign(QAction *a);

			//maps format
			void formatMaps();
			void clearMapsFormats();

			void currentCharFormatChanged(const QTextCharFormat &format);
			void cursorPositionChanged();

			void clipboardDataChanged();
			void about();
			void printPreview(QPrinter *);
						
			//set colors
			void highlightSelection(int pos, int length);
			void disHighlightSelection(int pos, int length);
			void selectMapRange(int pos, int length);

			// for translate the doc
			void translate();
			// for parse the syntax to signnode
			void parse();
			//to do experiments
			// 1 add 1000 chars in the middle, return the middle position
			void addCharsInMiddle();
			// 2 remove 1000 chars in the middle
			void removeCharsInMiddle();
			// table operations
			void addRow();
			void removeRow();

	private:
		void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
		void fontChanged(const QFont &f);
		void colorChanged(const QColor &c);
		void alignmentChanged(Qt::Alignment a);

		QAction *actionSave,
			*actionTextBold,
			*actionTextUnderline,
			*actionTextItalic,
			*actionTextColor,
			*actionFormatMaps,
			*actionClearFormatMaps,
			*actionAlignLeft,
			*actionAlignCenter,
			*actionAlignRight,
			*actionAlignJustify,
			*actionTableInsert,
			*actionViewMap,
			*actionUndo,
			*actionRedo,
			*actionCut,
			*actionCopy,
			*actionPaste,
			*actionTranslate,
			*actionParse,
			*actionSimuAddChars,
			*actionSimuRemoveChars,
			*actionAddRow,
			*actionRemoveRow;

		QDockWidget *mapDock;

		QComboBox *comboStyle;
		QFontComboBox *comboFont;
		QComboBox *comboSize;

		QToolBar *tb;
		QString fileName;
		XpmTextEdit *textEdit;
		MapEditor *mapEdit;
		
		//XpmTermDocControl *xpmControl;


		XpmTableDocControl *xpmControl;		
		conex::TermFactory _termFactory;
		sfa_parser _signparser;
		TableParser _tableparser;
		QTextCharFormat _highlightFormat;
		QTextCharFormat _dishighlightFormat;
	};
}

#endif
