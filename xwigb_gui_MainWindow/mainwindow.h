#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QProcess>
#include <QWindow>
#include <QWidget>
#include <QToolBar>
#include <QLayout>
#include <QTreeWidget>
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open_File();
    void paint();
    void xwigb_Help_doc();
    void ximage_Help_doc();
    void set_Parameter_Tree();
    void get_Parameter_Tree();
    void Launch_Xming();
    void Xming_setting();
    void get_Output();
    void get_Error();
    void start();

private slots:



private:
    Ui::MainWindow *ui;

    QMenuBar *mBar;  //菜单栏
    QMenu *File, *Edit, *Paint, *Help, *Help_2, *Xming;  //菜单列表
    QAction *mOpen, *mPaint, *mXwigb, *mXimage, *mXming_setting, *mXming_launch, *mAbout;  //菜单动作
    QMap<QString, QString> map;  //创建参数表

    int open_editor_C;
    QString fName;
    QProcess *xwigb;
    QWindow *win;
    QWidget *widg;
    QToolBar *toolBar;
    QTreeWidget *p_Tree;
    QTreeWidgetItem *open_editor_I;

};
#endif // MAINWINDOW_H
