//--- include files -------------------------------------------------------------
#include "Defines.hpp"

#pragma warning_qt_push
#include <QtGui/QtGui>
#include "UI/CustomTableView.h"
#include "CustomTableView.moc"
#pragma warning_qt_pop


namespace rtt {
namespace plugin {
namespace ui {



CustomTableView::CustomTableView(QWidget *parent/* = 0*/)
{
	this->setParent(parent);
	m_frozenTvLast = new QTableView(this);
	m_frozenTvLast->setSelectionMode(QAbstractItemView::SingleSelection);
	
	m_frozenTvFirst = new QTableView(this);
	m_frozenTvFirst->setSelectionMode(QAbstractItemView::SingleSelection);

	//connect the headers and scrollbars of both tableviews together
	connect(horizontalHeader()
	   , SIGNAL(sectionResized(int,int,int))
	   , this
	   , SLOT(updateSectionWidth(int,int,int)));

   connect(verticalHeader()
	   , SIGNAL(sectionResized(int,int,int))
	   , this
	   , SLOT(updateSectionHeight(int,int,int)));

   connect(m_frozenTvLast->verticalScrollBar()
	   , SIGNAL(valueChanged(int))
	   , verticalScrollBar()
	   , SLOT(setValue(int)));

   connect(verticalScrollBar()
	   , SIGNAL(valueChanged(int))
	   , m_frozenTvLast->verticalScrollBar()
	   , SLOT(setValue(int)));

   connect(m_frozenTvLast
	   , SIGNAL(clicked( const QModelIndex &))
	   , this
	   , SIGNAL(onFrozenRowClicked( const QModelIndex &) ) );

   connect(m_frozenTvFirst->verticalScrollBar()
	   , SIGNAL(valueChanged(int))
	   , verticalScrollBar()
	   , SLOT(setValue(int)));

   connect(verticalScrollBar()
	   , SIGNAL(valueChanged(int))
	   , m_frozenTvFirst->verticalScrollBar()
	   , SLOT(setValue(int)));

   connect(m_frozenTvFirst
	   , SIGNAL(clicked( const QModelIndex &))
	   , this
	   , SIGNAL(onFrozenRowClicked( const QModelIndex &) ) );

}

CustomTableView::~CustomTableView()
{
	delete m_frozenTvLast;
	delete m_frozenTvFirst;
}

void CustomTableView::setDataModel(QAbstractItemModel * model)
{
	setModel(model);
	init();
}

void CustomTableView::init()
{
	m_frozenTvLast->setModel(model());
	m_frozenTvFirst->setModel(model());

	m_frozenTvLast->setFocusPolicy(Qt::NoFocus);
	m_frozenTvLast->verticalHeader()->hide();
	m_frozenTvLast->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	m_frozenTvFirst->setFocusPolicy(Qt::NoFocus);
	m_frozenTvFirst->verticalHeader()->hide();
	m_frozenTvFirst->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	viewport()->stackUnder(m_frozenTvLast);
	//viewport()->stackUnder(m_frozenTvFirst);

	m_frozenTvLast->setStyleSheet("QTableView { border: none;"
		 /*  "background-color: #8EDE21;"*/
		   "selection-background-color: #999}");

	m_frozenTvFirst->setStyleSheet("QTableView { border: none;"
		/*  "background-color: #8EDE21;"*/
		"selection-background-color: #999}");

	m_frozenTvLast->setSelectionModel(selectionModel());
	m_frozenTvFirst->setSelectionModel(selectionModel());

	for(int col=0; col<8; col++)
		m_frozenTvLast->setColumnHidden(col, true);

	for(int col=2; col<=8; col++)
		m_frozenTvFirst->setColumnHidden(col, true);

	for( int i = 0 ; i+2 <= model()->rowCount() ; i = i+2)
		m_frozenTvFirst->setSpan(i,0,2,1);	

		
	   //horizontalHeader()->setResizeMode(8, QHeaderView::Fixed);
	   //horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
      // frozenTableView->setColumnWidth(0, columnWidth(0) );
	m_frozenTvLast->setColumnWidth(8, columnWidth(8) );
	m_frozenTvFirst->setColumnWidth(0, columnWidth(0) );
	m_frozenTvFirst->setColumnWidth(1, columnWidth(1) );
	//frozenTableView->setColumnWidth(0, columnWidth(0) );

	m_frozenTvLast->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_frozenTvLast->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_frozenTvLast->show();


	m_frozenTvFirst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_frozenTvFirst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_frozenTvFirst->show();


	updateFrozenTableGeometry();

	setHorizontalScrollMode(ScrollPerPixel);
	setVerticalScrollMode(ScrollPerPixel);
	m_frozenTvLast->setVerticalScrollMode(ScrollPerPixel);
	m_frozenTvFirst->setVerticalScrollMode(ScrollPerPixel);
 }

void CustomTableView::updateSectionWidth(int logicalIndex, int, int newSize)
{

	m_frozenTvLast->setColumnWidth(logicalIndex,newSize);
	m_frozenTvFirst->setColumnWidth(logicalIndex,newSize);
	updateFrozenTableGeometry();

}

void CustomTableView::updateSectionHeight(int logicalIndex, int, int newSize)
{
	m_frozenTvLast->setRowHeight(logicalIndex, newSize);
	m_frozenTvFirst->setRowHeight(logicalIndex, newSize);
}

void CustomTableView::resizeEvent(QResizeEvent * event)
{
	QTableView::resizeEvent(event);
	updateFrozenTableGeometry();
}

/*
 QModelIndex FreezeTableWidget::moveCursor(CursorAction cursorAction,
                                           Qt::KeyboardModifiers modifiers)
 {
       QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);
		
       if(cursorAction == MoveRight 
		   && current.column()>0 
		   && visualRect(current).topLeft().x() < frozenTableView->columnWidth(8) )
	   {

             const int newValue = horizontalScrollBar()->value() 
				 + visualRect(current).topLeft().x()
				 - frozenTableView->columnWidth(8);
             
			 horizontalScrollBar()->setValue(newValue);
       }
       return current;
 }*/

void CustomTableView::scrollTo (const QModelIndex & index, ScrollHint hint)
{
	if(index.column()>0)
		QTableView::scrollTo(index, hint);
}

void CustomTableView::updateFrozenTableGeometry()
{
	int widthToAdd = verticalHeader()->width()+frameWidth()+viewport()->width() - columnWidth(8);

	int allColumnWidth = verticalHeader()->width() + frameWidth();
	for(int col=0; col<8; col++)
		allColumnWidth += columnWidth(col);

	int toSet = 0;
	if( widthToAdd < allColumnWidth )
		toSet= widthToAdd;
	else
		toSet = allColumnWidth;

	/*verticalHeader()->width()+frameWidth()+viewport()->width() - columnWidth(8)*/
	m_frozenTvLast->setGeometry(toSet
		, frameWidth()
		, columnWidth(8) +0.1
		, viewport()->height()+horizontalHeader()->height());

	m_frozenTvFirst->setGeometry(verticalHeader()->width()+frameWidth()
		, frameWidth()
		, columnWidth(0) + columnWidth(1)+0.1
		, viewport()->height()+horizontalHeader()->height());
}

void CustomTableView::onSortComplete()
{
	for( int i = 0 ; i+2 <= model()->rowCount() ; i = i+2)
		m_frozenTvFirst->setSpan(i,0,2,1);	
}

}
}
}