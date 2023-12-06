#ifndef READONLYDELEGATE_H
#define READONLYDELEGATE_H

#include <QItemDelegate>
#include <QObject>

class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QWidget* parent=nullptr);
    QWidget* createEditor(QWidget*,const QStyleOptionViewItem&,const QModelIndex&) const override;
};

#endif // READONLYDELEGATE_H
