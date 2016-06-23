// http://qt.developpez.com/doc/4.6/itemviews-frozencolumn/

 #ifndef CUSTOM_TABLE_VIEW_H
 #define CUSTOM_TABLE_VIEW_H


//--- include files -------------------------------------------------------------
#include "Defines.hpp"

#pragma warning_qt_push
#include <QtGui/QTableView>
#include <QtCore/QObject>
#pragma warning_qt_pop

namespace rtt {
namespace plugin {

// Forward-declaration as GUI invokes backend
namespace backend {


} // namespace backend

namespace data
{

} // namespace data

namespace ui {

class CustomTableView : public QTableView 
{
  Q_OBJECT

public:
	CustomTableView(QWidget *parent = 0);
	~CustomTableView();

	void setDataModel(QAbstractItemModel * model);

	public slots:
		void onSortComplete();

	private slots:
		void updateSectionWidth(int logicalIndex,int, int newSize);
		void updateSectionHeight(int logicalIndex, int, int newSize);


	signals:
		void onFrozenRowClicked(QModelIndex index);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	// virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
	void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);

private:

	void init();
	void updateFrozenTableGeometry();

	QTableView *m_frozenTvLast;
	QTableView * m_frozenTvFirst;

};

}
}
}
 #endif