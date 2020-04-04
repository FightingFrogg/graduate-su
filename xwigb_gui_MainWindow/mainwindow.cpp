#include "mainwindow.h"
#include "disableeditor.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
//#include <windows.h>
#include <QTableWidgetItem>
#include <QMap>
//#include <QMapIterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mBar = menuBar();  //创建菜单栏

    //菜单栏
    File = mBar->addMenu("File");  //创建 file 菜单   
    Edit = mBar->addMenu("Edit");
    Paint = mBar->addMenu("Paint");
    Help = mBar->addMenu("Help");

    //菜单栏动作
    mOpen = File->addAction("Open");  //创建 open 菜单动作
    File->addSeparator();
    Xming = File->addMenu("Xming");
    mXming_launch = Xming->addAction("Xming");
    mXming_setting = Xming->addAction("Xming setting");
    mPaint = Paint->addAction("Paint");
    Help_2 =  Help->addMenu("Help");
    mXwigb = Help_2->addAction("Xwigb");
    mXimage = Help_2->addAction("Ximage");
    Help->addSeparator();
    mAbout = Help->addAction("About");

    //工具栏
    toolBar = addToolBar("toolBar");  //创建工具栏
    toolBar->addAction(mOpen);
    toolBar->addAction(mPaint);

    ui->textEdit->setWordWrapMode(QTextOption::NoWrap);  //设置文本编辑区水平滑块
    set_Parameter_Tree();  //设置参数表格
    ui->xwigb->setChecked(true);  //设置初选绘图模式


    //动作
    connect(mOpen, &QAction::triggered, this, &MainWindow::open_File);  //菜单栏 open
    connect(mPaint, &QAction::triggered, this, &MainWindow::paint);
    connect(mXwigb, &QAction::triggered, this, &MainWindow::xwigb_Help_doc);
    connect(mXimage, &QAction::triggered, this, &MainWindow::ximage_Help_doc);
    connect(mXming_launch, &QAction::triggered, this, &MainWindow::Launch_Xming);
    connect(mXming_setting, &QAction::triggered, this, &MainWindow::Xming_setting);

    //按钮
    connect(ui->open_button, &QPushButton::clicked, this, &MainWindow::open_File);  //按钮 open
    connect(ui->paint_button, &QPushButton::clicked, this, &MainWindow::paint);
    connect(ui->xwigb, &QAbstractButton::toggled,
            [=]()
            {
                ui->p_tree->topLevelItem(1)->child(0)->setText(1,"xwigb");
            });
    connect(ui->ximage, &QAbstractButton::toggled,
            [=]()
            {
                ui->p_tree->topLevelItem(1)->child(0)->setText(1,"ximage");
            });

    //进程
    xwigb = new QProcess(this);  //创建进程
    connect(xwigb, &QProcess::readyReadStandardOutput, this, &MainWindow::get_Output); // 准备进程的输出
    connect(xwigb, &QProcess::readyReadStandardError, this, &MainWindow::get_Error);  //准备进程的错误输出
    connect(xwigb, &QProcess::started, this, &MainWindow::start);
    connect(xwigb, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=]()
            {
                //ui->textEdit->append("finished");
                qDebug() << "finished";
            });

}


void MainWindow::set_Parameter_Tree()
{
    //构造一个 QTreeWidget 对象，并设置 2 列
    ui->p_tree->setColumnCount(2);
    QStringList header;
    header << "Parameter" << "Value";
    ui->p_tree->setHeaderLabels(header);

    QTreeWidgetItem *Basic = new QTreeWidgetItem(ui->p_tree,QStringList(QString("Basic")));  //第一个根节点,Basic
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"file")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"n1"<<"64")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"n2"<<"32")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"d1")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"d2")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"f1")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"f2")));
    Basic->addChild(new QTreeWidgetItem(Basic, QStringList(QStringList()<<"x2")));
    ui->p_tree->topLevelItem(0)->child(0)->setBackground(1, QBrush(QColor(228,228,228)));  //设置灰色

    QTreeWidgetItem *Label = new QTreeWidgetItem(ui->p_tree, QStringList("Label"));  //第二个根节点,Label
    Label->addChild(new QTreeWidgetItem(Label, QStringList(QStringList()<<"title"<<"xwigb")));
    Label->addChild(new QTreeWidgetItem(Label, QStringList(QStringList()<<"label2")));
    Label->addChild(new QTreeWidgetItem(Label, QStringList(QStringList()<<"windowtitle")));

    QTreeWidgetItem *Style = new QTreeWidgetItem(ui->p_tree,QStringList(QString("Style")));  //第三个根节点,Style
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"labelfont")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"titlefont")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"labelcolor")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"titlecolor")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"grdcolor")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"style")));
    Style->addChild(new QTreeWidgetItem(Style, QStringList(QStringList()<<"curvecolor")));

    QTreeWidgetItem *Paint = new QTreeWidgetItem(ui->p_tree, QStringList("Paint"));  //第四个根节点,Paint
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"verbose")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"xbox")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"ybox")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"wbox")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"hbox")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"x1beg")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"x1end")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"d1num")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"f1num")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"n1tic")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"grid1")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"x2beg")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"x2end")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"d2num")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"f2num")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"n2tic")));
    Paint->addChild(new QTreeWidgetItem(Paint, QStringList(QStringList()<<"grid2")));

    QTreeWidgetItem *Other = new QTreeWidgetItem(ui->p_tree,QStringList(QString("Style")));  //第五个根节点,Other
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"bias")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"clip")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"endian")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"interp")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"mpicks")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"perc")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"va")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"wigclip")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"wt")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"xcur")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"plotfile")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"curve")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"npair")));
    Other->addChild(new QTreeWidgetItem(Other, QStringList(QStringList()<<"curvewidth")));

    ui->p_tree->expandItem(ui->p_tree->topLevelItem(0));  //展开前两个节点
    ui->p_tree->expandItem(ui->p_tree->topLevelItem(1));

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<17; j++)
        {
            if(ui->p_tree->topLevelItem(i)->child(j) != 0)
            {
                ui->p_tree->topLevelItem(i)->child(j)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);  //设置可编辑
            }
            else
                break;
        }
    }

    ui->p_tree->topLevelItem(0)->child(0)->setFlags(ui->p_tree->topLevelItem(0)->child(0)->flags() & (~ Qt::ItemIsEditable));  //设置文件名不可编辑

    ui->p_tree->setItemDelegateForColumn(0, new DisableEditor(ui->p_tree));  //设置第一列不可编辑
}


void MainWindow::get_Parameter_Tree()
{
    for (int i=0; i<5; i++)
    {
        for(int j=0; j<17; j++)
        {
            if(ui->p_tree->topLevelItem(i)->child(j) != 0 && ui->p_tree->topLevelItem(i)->child(j)->text(1) != "")
            {
                map.insert(ui->p_tree->topLevelItem(i)->child(j)->text(0),
                           ui->p_tree->topLevelItem(i)->child(j)->text(1));
            }
        }
    }

    QMap<QString, QString>::const_iterator map_i;
    for(map_i=map.begin(); map_i!=map.end(); ++map_i) //迭代输出
        qDebug() << map_i.key() << map_i.value();
}

void MainWindow::paint()
{
    Launch_Xming();  //启动 Xming

    QString cmd;
    xwigb->setProgram("bash");  //open bash
    xwigb->start();
    ui->textEdit->append("bash is running...");

    if(true == ui->xwigb->isChecked())  //选择绘图模式
        cmd = "./xwigbTest1 <" + fName +" ";  //set cmd line parameter
    else if(true == ui->ximage->isChecked())
    {
        cmd = "./ximageTest1 <" + fName +" ";
//        ui->p_tree->topLevelItem(1)->child(0)->setText(1,"ximage");
    }

    get_Parameter_Tree();  //从参数表格获取参数
    QMap<QString, QString>::const_iterator map_i;
    for (map_i=map.begin(); map_i!=map.end(); ++map_i) {
        if(map_i.key() != "file")
            cmd += map_i.key() + "=" + map_i.value() + " ";
    }
    cmd += "\n";
    ui->textEdit->append(cmd);

    xwigb->write(cmd.toUtf8().data());

//    LPCWSTR class_Name = TEXT("Xming X");
//    LPCWSTR Caption = TEXT("xwigb");
//    HWND hwin = FindWindowEx(0, 0, class_Name, Caption);
//    WId wid = (WId)hwin;
//    if(hwin == 0){
//       qDebug() << "no window";
//    }
//    win = QWindow::fromWinId(wid);
//    widg = QWidget::createWindowContainer(win, this, Qt::FramelessWindowHint);
//    widg->setMinimumSize(800, 300);
//    widg->setGeometry(9, 285, 200, 200);
//    widg->show();

    xwigb->write("exit\n");  //退出
}


void MainWindow::open_File()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "open", "./");
    ui->textEdit->append(path);
    QFileInfo finfo = QFileInfo(path);
    fName = finfo.fileName();
    ui->p_tree->topLevelItem(0)->child(0)->setText(1,fName);  //在参数树中添加文件名

    get_Parameter_Tree();
}


void MainWindow::get_Output()
{
    if (false == QString(xwigb->readAllStandardOutput()).startsWith(""))
    {
        ui->textEdit->append(QString(xwigb->readAllStandardOutput()));
    }

}


void MainWindow::Launch_Xming()
{
    QProcess taskList;
    QProcess *XmingL = new QProcess(this);
    taskList.start("tasklist");  //启动 tasklist 命令，确定 Xming 是否正在运行
    taskList.waitForFinished();
    QByteArray result = taskList.readAllStandardOutput();
    if(-1==result.indexOf("Xming.exe"))  //Xming 没有运行
    {
        QStringList arg;
        arg << ":0" << "-clipboard" << "-multiwindow";
        XmingL->setProgram("C:/Program Files (x86)/Xming/Xming.exe");
        XmingL->setArguments(arg);
        XmingL->start();
        XmingL->waitForStarted();
        ui->textEdit->append("Xming...");
    }
    else
        ui->textEdit->append("Xming is running.");

    taskList.kill();  //关闭 tasklist
}


void MainWindow::Xming_setting()
{
    QProcess taskList;
    QProcess *XmingL = new QProcess(this);
    taskList.start("tasklist");  //启动 tasklist 命令，确定 Xming 是否正在运行
    taskList.waitForFinished();
    QByteArray result = taskList.readAllStandardOutput();
    if(-1==result.indexOf("XLaunch.exe"))  //Xming 没有运行
    {
        XmingL->setProgram("C:/Program Files (x86)/Xming/XLaunch.exe");
        XmingL->start();
        XmingL->waitForStarted();
    }
    else
        ui->textEdit->append("XLaunch is running.");

    taskList.kill();  //关闭 tasklist
}


void MainWindow::get_Error()
{
    qDebug() << QString(xwigb->readAllStandardError());
}


void MainWindow::start()
{
    ui->textEdit->append("running...");
}


void MainWindow::xwigb_Help_doc()
{
    QString Help_doc =
    "----------------------------------------------------------------------------------\n"
    " XWIGB - X WIGgle-trace plot of f(x1,x2) via Bitmap\n"
    " \n"
    " xwigb n1= [optional parameters] <binaryfile\n"
    " \n"
    " X Functionality:\n"
    " Button 1              Zoom with rubberband box\n"
    " Button 2              Show mouse (x1,x2) coordinates while pressed\n"
    " q or Q key            Quit\n"
    " s key                 Save current mouse (x1,x2) location to file\n"
    " p or P key            Plot current window with pswigb (only from disk files)\n"
    " a or page up keys     enhance clipping by 10%\n"
    " c or page down keys   reduce clipping by 10%\n"
    " i or +(keypad)        zoom in by factor 2\n"
    " o or -(keypad)        zoom out by factor 2\n"
    " l                     lock the zoom while moving the coursor\n"
    " u                     unlock the zoom\n"
    " 1,2,...,9             Zoom/Move factor of the window size\n"
    " up,down,left,right keys       move zoom window by half width/height\n"
    "\n"
    " Notes:\n"
    " Reaching the window limits while moving within changes the zoom\n"
    " factor in this direction. The use of zoom locking(l) disables it\n"
    "\n"
    " Required Parameters:\n"
    " n1                    number of samples in 1st (fast) dimension\n"
    "\n"
    " Optional Parameters:\n"
    " d1=1.0                sampling interval in 1st dimension\n"
    " f1=0.0                first sample in 1st dimension\n"
    " n2=all                number of samples in 2nd (slow) dimension\n"
    " d2=1.0                sampling interval in 2nd dimension\n"
    " f2=0.0                first sample in 2nd dimension\n"
    " x2=f2,f2+d2,...       array of sampled values in 2nd dimension\n"
    " mpicks=/dev/tty       file to save mouse picks in\n"
    " bias=0.0              data value corresponding to location along axis 2\n"
    " perc=100.0            percentile for determining clip\n"
    " clip=(perc percentile)        data values < bias+clip and > bias-clip are clipped\n"
    " xcur=1.0              wiggle excursion in traces corresponding to clip\n"
    " wt=1                  =0 for no wiggle-trace; =1 for wiggle-trace\n"
    " va=1                  =0 for no variable-area; =1 for variable-area fill\n"
    "                       =2 for variable area, solid/grey fill\n"
    "                       SHADING: 2<=va<=5  va=2 light grey, va=5 black\n"
    " verbose=0             =1 for info printed on stderr (0 for no info)\n"
    " xbox=50               x in pixels of upper left corner of window\n"
    " ybox=50               y in pixels of upper left corner of window\n"
    " wbox=550              width in pixels of window\n"
    " hbox=700              height in pixels of window\n"
    " x1beg=x1min           value at which axis 1 begins\n"
    " x1end=x1max           value at which axis 1 ends\n"
    " d1num=0.0             numbered tic interval on axis 1 (0.0 for automatic)\n"
    " f1num=x1min           first numbered tic on axis 1 (used if d1num not 0.0)\n"
    " n1tic=1               number of tics per numbered tic on axis 1\n"
    " grid1=none            grid lines on axis 1 - none, dot, dash, or solid\n"
    " x2beg=x2min           value at which axis 2 begins\n"
    " x2end=x2max           value at which axis 2 ends	\n"
    " d2num=0.0             numbered tic interval on axis 2 (0.0 for automatic)\n"
    " f2num=x2min           first numbered tic on axis 2 (used if d2num not 0.0)\n"
    " n2tic=1               number of tics per numbered tic on axis 2\n"
    " grid2=none            grid lines on axis 2 - none, dot, dash, or solid\n"
    " label2=               label on axis 2\n"
    " labelfont=Erg14       font name for axes labels\n"
    " title=                title of plot\n"
    " titlefont=Rom22       font name for title\n"
    " windowtitle=xwigb     title on window\n"
    " labelcolor=blue       color for axes labels\n"
    " titlecolor=red        color for title\n"
    " gridcolor=blue        color for grid lines\n"
    " style=seismic         normal (axis 1 horizontal, axis 2 vertical) or \n"
    "                       seismic (axis 1 vertical, axis 2 horizontal)\n"
    " endian=               =0 little endian =1 big endian	\n"
    " interp=0              no interpolation in display\n"
    "                       =1 use 8 point sinc interpolation\n"
    " wigclip=0             If 0, the plot box is expanded to accommodate\n"
    "                       the larger wiggles created by xcur>1.	If this\n"
    "                       flag is non-zero, the extra-large wiggles are\n"
    "                       are clipped at the boundary of the plot box.\n"
    " plotfile=plotfile.ps  filename for interactive ploting (P)\n"
    " curve=curve1,curve2,...       file(s) containing points to draw curve(s)\n"
    " npair=n1,n2,n2,...            number(s) of pairs in each file\n"
    " curvecolor=color1,color2,...  color(s) for curve(s)\n"
    " curvewidth=w1,w2,...          line width(s) for curve(s), in pixels\n"
    "----------------------------------------------------------------------------------\n";

    ui->textEdit->append(Help_doc);

}


void MainWindow::ximage_Help_doc()
{
    QString Help_doc =
    "----------------------------------------------------------------------------------\n"
    " XIMAGE - X IMAGE plot of a uniformly-sampled function f(x1,x2)\n"
    "\n"
    " ximage n1= [optional parameters] <binaryfile\n"
    "\n"
    " X Functionality:\n"
    " Button 1              Zoom with rubberband box\n"
    " Button 2              Show mouse (x1,x2) coordinates while pressed\n"
    " q or Q key            Quit\n"
    " s key                 Save current mouse (x1,x2) location to file\n"
    " p or P key            Plot current window with pswigb (only from disk files)\n"
    "\n"
    " ... change colormap interactively\n"
    " r                     install next RGB - colormap\n"
    " R                     install previous RGB - colormap\n"
    " h                     install next HSV - colormap\n"
    " H                     install previous HSV - colormap\n"
    " H                     install previous HSV - colormap\n"
    " (Move mouse cursor out and back into window for r,R,h,H to take effect)\n"
    "\n"
    " Required Parameters:\n"
    " n1                    number of samples in 1st (fast) dimension\n"
    "\n"
    " Optional Parameters:\n"
    " d1=1.0                sampling interval in 1st dimension\n"
    " f1=0.0                first sample in 1st dimension\n"
    " n2=all                number of samples in 2nd (slow) dimension\n"
    " d2=1.0                sampling interval in 2nd dimension	\n"
    " f2=0.0                first sample in 2nd dimension\n"
    " mpicks=/dev/tty       file to save mouse picks in\n"
    " perc=100.0            percentile used to determine clip\n"
    " clip=(perc percentile) clip used to determine bclip and wclip\n"
    " bperc=perc            percentile for determining black clip value\n"
    " wperc=100.0-perc      percentile for determining white clip value\n"
    " bclip=clip            data values outside of [bclip,wclip] are clipped\n"
    " wclip=-clip           data values outside of [bclip,wclip] are clipped\n"
    " cmap=hsv\'n\' or rgb\'m\'         \'n\' is a number from 0 to 13\n"
    "                               \'m\' is a number from 0 to 11\n"
    "                               cmap=rgb0 is equal to cmap=gray\n"
    "                               cmap=hsv1 is equal to cmap=hue	\n"
    "                               (compatibility to older versions)\n"
    " legend=0              =1 display the color scale\n"
    " units=                unit label for legend\n"
    " legendfont=times_roman10      font name for title	\n"
    " verbose=1             =1 for info printed on stderr (0 for no info)\n"
    " xbox=50               x in pixels of upper left corner of window\n"
    " ybox=50               y in pixels of upper left corner of window\n"
    " wbox=550              width in pixels of window\n"
    " hbox=700              height in pixels of window\n"
    " lwidth=16             colorscale (legend) width in pixels\n"
    " lheight=hbox/3        colorscale (legend) height in pixels\n"
    " lx=3                  colorscale (legend) x-position in pixels\n"
    " ly=(hbox-lheight)/3   colorscale (legend) y-position in pixels\n"
    " x1beg=x1min           value at which axis 1 begins\n"
    " x1end=x1max           value at which axis 1 ends	\n"
    " d1num=0.0             numbered tic interval on axis 1 (0.0 for automatic)\n"
    " f1num=x1min           first numbered tic on axis 1 (used if d1num not 0.0)\n"
    " n1tic=1               number of tics per numbered tic on axis 1\n"
    " grid1=none            grid lines on axis 1 - none, dot, dash, or solid\n"
    " label1=               label on axis 1\n"
    " x2beg=x2min           value at which axis 2 begins\n"
    " x2end=x2max           value at which axis 2 ends	\n"
    " d2num=0.0             numbered tic interval on axis 2 (0.0 for automatic)\n"
    " f2num=x2min           first numbered tic on axis 2 (used if d2num not 0.0)\n"
    " n2tic=1               number of tics per numbered tic on axis 2\n"
    " grid2=none            grid lines on axis 2 - none, dot, dash, or solid\n"
    " label2=               label on axis 2\n"
    " labelfont=Erg14       font name for axes labels\n"
    " title=                title of plot\n"
    " titlefont=Rom22       font name for title\n"
    " windowtitle=ximage    title on window\n"
    " labelcolor=blue       color for axes labels\n"
    " titlecolor=red        color for title\n"
    " gridcolor=blue        color for grid lines\n"
    " style=seismic         normal (axis 1 horizontal, axis 2 vertical) or\n"
    "                       seismic (axis 1 vertical, axis 2 horizontal)\n"
    " blank=0               This indicates what portion of the lower range \n"
    "                       to blank out (make the background color).  The \n"
    "                       value should range from 0 to 1.\n"
    " curve=curve1,curve2,...       file(s) containing points to draw curve(s)\n"
    " npair=n1,n2,n2,...            number(s) of pairs in each file \n"
    " curvecolor=color1,color2,...  color(s) for curve(s) \n"
    " blockinterp=0         whether to use block interpolation (0=no, 1=yes)\n"
    "----------------------------------------------------------------------------------\n";

    ui->textEdit->append(Help_doc);
}


MainWindow::~MainWindow()
{
    delete ui;
}

