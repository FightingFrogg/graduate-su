#ifndef DISABLEEDITOR_H
#define DISABLEEDITOR_H

#include <QStyledItemDelegate>

class DisableEditor : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DisableEditor(QObject *parent = nullptr);

    //设置不可编辑
    virtual QWidget * createEditor(QWidget *parent,	const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const override
    {
        return 0;
    }

signals:

};

#endif // DISABLEEDITOR_H
