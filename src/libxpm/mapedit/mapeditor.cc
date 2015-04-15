#include "mapeditor.h"
#include "ui_mapeditor.h"
#include "XpmDocModel.h"
#include "addmap.h"
#include "removemap.h"


namespace xpm {
	//MapEditor::MapEditor(XpmTermDocControl* xpmControl, QWidget *parent) :QWidget(parent),
	MapEditor::MapEditor(XpmTableDocControl* xpmControl, QWidget *parent) :QWidget(parent),
		_xpmControl(xpmControl),
		model(0),
		ui(new Ui::MapEditor){
			ui->setupUi(this);
			ui->findMapButton->setEnabled(false);
			ui->addMapButton->setEnabled(false);
			ui->viewMapButton->setEnabled(false);
			ui->removeMapButton->setEnabled(false);
			connect(ui->findMapButton, SIGNAL(clicked()), this, SLOT(findMapByName()));
			connect(ui->viewMapButton, SIGNAL(clicked()), this, SLOT(viewDetailMap()));
			connect(ui->addMapButton, SIGNAL(clicked()), this, SLOT(addMap()));
			connect(ui->removeMapButton, SIGNAL(clicked()), this, SLOT(removeMap()));
	}

	MapEditor::~MapEditor(){
		delete ui;
	}

	void MapEditor::addMap() {
		//we need to refresh the maps
		AddMap addMap(_xpmControl, this);
		if(QDialog::Accepted == addMap.exec()) {
			viewMap();
			sfa_select range = _xpmControl->select();
			emit highlightSelection(range.word_ix, range.word.length());
			ui->addMapButton->setEnabled(false);
		}
	}

	void MapEditor::removeMap() {
		if(_range.word.size() > 0) {
			RemoveMap removeMap(_xpmControl, &_range, this);		
			if(QDialog::Accepted == removeMap.exec()) {
				cout << "remove map accepted" << endl;
				viewMap();
				emit disHighlightSelection(_range.word_ix, _range.word.length());
				ui->removeMapButton->setEnabled(false);
			}
		}
	}

	void MapEditor::findMapByName() {
	}

	void MapEditor::viewDetailMap() {
	}

	void MapEditor::viewMap() {
		if(this->isVisible()) {
			if(model) {
				delete model;
				model = 0;
			}
			sfa_blocks termblocks = _xpmControl->model()->blocks();
			if(termblocks.size() > 0) {
				model = new XpmDocModel(_xpmControl->model());
				ui->mapTreeView->setModel(model);
				ui->mapTreeView->expandAll();
				QItemSelectionModel *selectionModel = ui->mapTreeView->selectionModel();
				connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
					this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
				ui->lbTotal->setText(QString::number( _xpmControl->total_maps()));
			}else {
				ui->lbTotal->setText("0");
			}
		}
	}

	void MapEditor::setAddMapEnable(bool enable) {
		ui->addMapButton->setEnabled(enable);
	}

	/*void MapEditor::setupMapModel(TermBlocks& termblocks) {

	QStandardItem* rootNode = model->invisibleRootItem();
	for(TermBlocksIter it = termblocks.begin(); it != termblocks.end(); ++it) {
	QString pos; 
	pos.setNum((*it)->position());
	QStandardItem *blockitem = new QStandardItem(pos);
	blockitem->setEditable(false);
	rootNode->appendRow(blockitem);

	for(TermIter tit = (*it)->GetTerms().begin(); tit != (*it)->GetTerms().end(); ++tit) {
	QString text(tit->text().c_str());
	QString definition(tit->term()->GetInterpretation()->GetTextExplanation().c_str());
	QStandardItem *textitem = new QStandardItem(text + " - " + definition);
	textitem->setEditable(false);
	blockitem->appendRow(textitem);
	}
	}
	}*/

	void MapEditor::selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection) {
		const QModelIndex index = ui->mapTreeView->selectionModel()->currentIndex();
		if(!index.isValid())
			return ;

		TreeItem *currentItem = static_cast<TreeItem*>(index.internalPointer());
		bool enable = false;
		if(currentItem->parent() != model->GetRootItem()) {
			enable = true;
			_range = *(currentItem->range());
			emit selectMapRange(_range.word_ix, _range.word.length());
		}else {
			enable = false;
			_range.block_ix = 0;
			_range.word_ix = 0;
			_range.word.clear();
			_range.id.clear();
		}
		ui->removeMapButton->setEnabled(enable);

	}

	void MapEditor::showEvent(QShowEvent* event) {
		QWidget::showEvent(event);
		viewMap();
	}

}
