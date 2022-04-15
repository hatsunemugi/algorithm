#include "mainwindow.h"
#include "subwindow.h"
#include "ui_mainwindow.h"
#include "backtracking.h"
#include "generate.h"
#include "maxsum.h"
#include "analyser.h"
#include <QFileDialog>
#include <QRegion>
#include <QTimer>
#include <QMessageBox>
#include <QThreadPool>
#include <QTableWidget>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QHeaderView>
#include <QGridLayout>
#include <QPixmap>
#include <QSpacerItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->centralwidget->setMouseTracking(true);
    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->start(10);
    connect(t, &QTimer::timeout, this, [=](){Init();});
    connect(ui->adjSizeBtn, &QPushButton::clicked, this, [=](){controlWindowScale();});
}

MainWindow::~MainWindow()
{
    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->start(20);
    connect(t, &QTimer::timeout, this, [=](){delete w;delete ui;});

}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        mousePressed = true;
        lastPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::NoButton)
        mousePressed = false;
    if(!mousePressed){
        mouseState = 0;
        if(!maximized && abs(event->pos().x() - ui->mainWidget->pos().x()) < 5)
            mouseState |= AT_LEFT;
        if(!maximized && abs(event->pos().y() - ui->mainWidget->pos().y()) < 5)
            mouseState |= AT_TOP;
        if(!maximized && abs(event->pos().x() - ui->mainWidget->pos().x() - ui->mainWidget->width()) < 5)
            mouseState |= AT_RIGHT;
        if(!maximized && abs(event->pos().y() - ui->mainWidget->pos().y() - ui->mainWidget->height()) < 5)
            mouseState |= AT_BOTTOM;
        if(mouseState == AT_TOP_LEFT  || mouseState == AT_BOTTOM_RIGHT)
            setCursor(Qt::SizeFDiagCursor);
        else if(mouseState == AT_TOP_RIGHT || mouseState == AT_BOTTOM_LEFT)
            setCursor(Qt::SizeBDiagCursor);
        else if(mouseState & (AT_LEFT | AT_RIGHT))
            setCursor(Qt::SizeHorCursor);
        else if(mouseState & (AT_TOP | AT_BOTTOM))
            setCursor(Qt::SizeVerCursor);
        else
            unsetCursor();
    }
    else{
        if(mouseState == 0){
            if(maximized){
                qreal wRatio = (double)event->pos().x() / (double)ui->mainWidget->width();
                controlWindowScale();
                this->move(QPoint(event->globalPosition().x() - ui->mainWidget->width() * wRatio, -30));
                lastPos = QPoint(ui->mainWidget->width() * wRatio, event->pos().y());
            }
            else
                this->move(event->globalPosition().toPoint() - lastPos);
        }
        else{
            QPoint d = event->globalPosition().toPoint() - frameGeometry().topLeft() - lastPos;
            if(mouseState & AT_LEFT){
                if(this->width()>this->minimumWidth())
                this->move(this->frameGeometry().x() + d.x(), this->frameGeometry().y());
                this->resize(this->width() - d.x(), this->height());
            }
            if(mouseState & AT_RIGHT){
                this->resize(this->width() + d.x(), this->height());
            }
            if(mouseState & AT_TOP){
                if(this->height()>this->minimumHeight())
                this->move(this->frameGeometry().x(), this->frameGeometry().y() + d.y());
                this->resize(this->width(), this->height() - d.y());
            }
            if(mouseState & AT_BOTTOM){
                this->resize(this->width(), this->height() + d.y());
            }
        }
        lastPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
    //Resize border
    if(border)
        border->resize(ui->mainWidget->size() + QSize(2, 2));

    //Resize mask
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    //Resize all pages
    for(int i = 0; i < pageList.size(); i++){
        pageList[i]->resize(ui->mainWidget->width() * 0.3 < pageList[i]->preferWidth ? pageList[i]->preferWidth - 1 : ui->mainWidget->width() * 0.3 - 1, ui->mainWidget->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}

void MainWindow::Init()
{
    /* Create main widget and set mask, style sheet and shadow */
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    QString mainStyle;
    ui->mainWidget->setObjectName("mainWidget");
    mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius) + "}";
    ui->mainWidget->setStyleSheet(mainStyle);

    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    ui->mainWidget->setGraphicsEffect(windowShadow);
    /**********************************************************/

    /* Create border in order to cover the zigzag edge of the region */
    border = new QWidget(this);
    border->move(ui->mainWidget->pos() - QPoint(1, 1));
    border->resize(ui->mainWidget->size() + QSize(2, 2));
    QString borderStyle;
    borderStyle = "background-color:#00FFFFFF;border:1.5px solid #686868; border-radius:" + QString::asprintf("%d",cornerRadius) + "px";
    border->setStyleSheet(borderStyle);
    border->setAttribute(Qt::WA_TransparentForMouseEvents);
    border->show();
    /*****************************************************************/

    /* Create about page */
    defaultSettingsPage = new SlidePage(cornerRadius, "ABOUT", ui->mainWidget);
    textInputItem *version = new textInputItem("version", defaultSettingsPage);
    version->setValue("nullptr");
    version->setEnabled(false);
    textInputItem *updateDate = new textInputItem("lastest", defaultSettingsPage);
    updateDate->setValue("2022/4/1 16:44");
    updateDate->setEnabled(false);
    textInputItem *Author = new textInputItem("author", defaultSettingsPage);
    Author->setValue("made in heaven");
    Author->setEnabled(false);
    textInputItem *lic = new textInputItem("lic", defaultSettingsPage);
    lic->setValue("MIT License");
    lic->setEnabled(false);
    textInputItem *GitHub = new textInputItem("git", defaultSettingsPage);
    GitHub->setValue("github.com/");
    GitHub->setEnabled(false);
    defaultSettingsPage->AddContent(GitHub);
    defaultSettingsPage->AddContent(lic);
    defaultSettingsPage->AddContent(Author);
    defaultSettingsPage->AddContent(updateDate);
    defaultSettingsPage->AddContent(version);
    curSettingsPage = defaultSettingsPage;
    defaultSettingsPage->show();
    pageList.push_back(defaultSettingsPage);

    /************************/

    /* Initialize display area */
    QFont titleFont = QFont("Corbel Light", 24);
    QFontMetrics titleFm(titleFont);
    canvasTitle = new QLineEdit(this);
    canvasTitle->setFont(titleFont);
    canvasTitle->setText("START");
    canvasTitle->setMaxLength(20);
    canvasTitle->setReadOnly(true);
    canvasTitle->setMinimumHeight(titleFm.height());
    canvasTitle->setMaximumWidth(titleFm.size(Qt::TextSingleLine, "START").width() + 10);
    canvasTitle->setStyleSheet("background-color:#00000000;border-style:none;border-width:0px;margin-left:1px;");
    connect(canvasTitle, &QLineEdit::textEdited, canvasTitle, [=](QString text){canvasTitle->setMaximumWidth(titleFm.size(Qt::TextSingleLine, text).width());});

    QFont descFont = QFont("Corbel Light", 12);
    QFontMetrics descFm(descFont);
    canvasDesc = new QLineEdit(this);
    canvasDesc->setFont(descFont);
    canvasDesc->setText("to be continue");
    canvasDesc->setMaxLength(128);
    canvasDesc->setReadOnly(true);
    canvasDesc->setMinimumHeight(descFm.lineSpacing());
    canvasDesc->setStyleSheet("background-color:#00000000;border-style:none;border-width:0px;");

    settingsIcon = new customIcon(":/icons/icons/settings.svg", "settings", 5, this);
    settingsIcon->setMinimumHeight(canvasTitle->height() * 0.7);
    settingsIcon->setMaximumWidth(canvasTitle->height() * 0.7);
    connect(settingsIcon, &customIcon::clicked, this, [=](){
        QPropertyAnimation *rotate = new QPropertyAnimation(settingsIcon, "rotationAngle", this);
        rotate->setDuration(187);
        rotate->setStartValue(0);
        rotate->setEndValue(90);
        rotate->setEasingCurve(QEasingCurve::InOutExpo);
        rotate->start();
        curSettingsPage->slideIn();
    });



    layersIcon = new customIcon(":/icons/icons/layers.svg", "layers", 5, this);
    layersIcon->setMinimumHeight(canvasTitle->height() * 0.7);
    layersIcon->setMaximumWidth(canvasTitle->height() * 0.7);

    /* create title */
    QWidget *titleInnerWidget = new QWidget(this);
    titleInnerWidget->setFixedHeight(canvasTitle->height());
    QHBoxLayout *innerLayout = new QHBoxLayout(titleInnerWidget);
    titleInnerWidget->setLayout(innerLayout);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(10);
    innerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    innerLayout->addWidget(canvasTitle);
    innerLayout->addWidget(settingsIcon);
    innerLayout->addWidget(layersIcon);
    QWidget *titleWidget = new QWidget(this);
    titleWidget->setMaximumHeight(canvasTitle->height() + canvasDesc->height());
    QVBoxLayout *outerLayout = new QVBoxLayout(titleWidget);
    titleWidget->setLayout(outerLayout);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);
    outerLayout->addWidget(titleInnerWidget);
    outerLayout->addWidget(canvasDesc);



    /* create layers page */
    //for add new page
    textInputItem *rename = new textInputItem("Name:",createNewPage);
    rename->setValue("Layer_" + QString::asprintf("%d", canvasList.size()));
    textInputItem *redescribe = new textInputItem("Detail:",createNewPage);
    redescribe->setValue("No description");

    layersPage = new SlidePage(cornerRadius, "LAYERS", ui->mainWidget);
    layersPage->stackUnder(createNewPage);
    pageList.push_back(layersPage);
    connect(layersIcon, &customIcon::clicked,pageList[1], &SlidePage::slideIn);//..............................................
    generator=new Generate;
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});
    textButton *DivideAndConquer = new textButton("divide&&conquer", layersPage);
    textButton *DynamicProgramming = new textButton("dynamic programming", layersPage);
    textButton *Greedy = new textButton("greedy", layersPage);
    textButton *BackTracking = new textButton("backtracking", layersPage);
    textButton *generate = new textButton("generate", layersPage);
    textButton *analyser = new textButton("analyse", layersPage);
    createNewPage=layersPage;
    layersPage->AddContent(BackTracking);
    connect(BackTracking, &textButton::clicked, this, [=](){createNewPage->slideOut();backtracking();});
    layersPage->AddContent(Greedy);
    connect(Greedy, &textButton::clicked, this, [=](){createNewPage->slideOut();greedy();});
    layersPage->AddContent(DynamicProgramming);
    connect(DynamicProgramming, &textButton::clicked, this, [=](){createNewPage->slideOut();dynamicProgramming();});
    layersPage->AddContent(DivideAndConquer);
    connect(DivideAndConquer, &textButton::clicked, this, [=](){createNewPage->slideOut();divideAndConquer();});
    layersPage->AddContent(analyser);
    connect(analyser, &textButton::clicked, this, [=](){createNewPage->slideOut();analyse();});
    connect(generate,&textButton::clicked, this, [=](){
        emit start(((QLineEdit*)autoinput)->text());
        QThreadPool::globalInstance()->start(generator);
    });
    connect(this,&MainWindow::start,generator,&Generate::receive);
    connect(generator,&Generate::send,this,[=](QString o,QVector<int> p){
        ((QLineEdit*)autoinput)->setText(o);
    });
    connect(generator,&Generate::able,this,[=](bool t){
        if(t==true&&generated==false)
        {
            generated = true;
            pageList[1]->InsertContent(generate,0);
        }
        else if(t==true)
            generate->show();
        else if(t==false)
            generate->hide();
    });
    layersPage->show();
    generate->hide();
    ui->displayLayout->addWidget(titleWidget);
    ui->displayLayout->setAlignment(Qt::AlignTop);
    queueData=new QQueue<QVector<int>>;
    defaultPage = new QWidget(ui->mainWidget);
}

void MainWindow::analyse()
{
    layersPage = new SlidePage(cornerRadius, "ANALYSE", ui->mainWidget);
    layersPage->stackUnder(createNewPage);
    layerSel = new singleSelectGroup("Layers", layersPage);
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});

    textButton *lexicalAnalyse = new textButton("lexical", layersPage);
    textButton *grammarAnalyse = new textButton("grammar", layersPage);
    layersPage->AddContent(grammarAnalyse);
    layersPage->AddContent(lexicalAnalyse);
    connect(lexicalAnalyse, &textButton::clicked, this, [=](){pageList.back()->slideOut();lexical();});
    connect(grammarAnalyse, &textButton::clicked, this, [=](){pageList.back()->slideOut();grammar();});
    layersPage->AddContent(layerSel);
    layersPage->show();
    pageList.push_back(layersPage);
    layersPage->slideIn();
}

void MainWindow::lexical()
{
    ui->displayLayout->removeWidget(defaultPage);
    delete defaultPage;
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bigIconButton *forward = new bigIconButton(":/icons/icons/back.svg", "analyse", 10, this);
    forward->setScale(0.2);
    bigIconButton *openfile = new bigIconButton(":/icons/icons/open.png", "openfile", 10, this);
    openfile->setScale(0.5);
    QLabel *log=new QLabel;
    autolog=log;
    QTextEdit *input=new QTextEdit;
    QTableWidget *output=new QTableWidget;
    QHBoxLayout *button=new QHBoxLayout;
    button->addWidget(forward);
    button->addWidget(openfile);
    button->addWidget(input);
    QGridLayout *right=new QGridLayout;
    right->addWidget(input,0,0,1,4);
    right->addWidget(log,1,0,1,1);
    right->addWidget(forward,2,0,1,1);
    right->addWidget(openfile,2,2,1,1);
    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setContentsMargins(10, 6, 10, 16);
    defaultPageLayout->setSpacing(20);
    defaultPageLayout->addWidget(output);
    defaultPageLayout->addLayout(right);
    ui->displayLayout->addWidget(defaultPage);
    autoinput=input;
    autooutput=output;
    autoobject=new Lexical;
    connect(openfile,&bigIconButton::clicked,this,[=](){
        QTextEdit* i=((QTextEdit*)autoinput);
        i->clear();
        open();
        emitline(0);
    });
    connect(this,&MainWindow::read,this,[=](QString t){
        QTextEdit* i=((QTextEdit*)autoinput);
        i->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        i->append(t);
    });
    connect(forward,&bigIconButton::clicked,this,[=](){
        emit start(filepath);
        QThreadPool::globalInstance()->start((Lexical*)autoobject);
    });
    connect(this,&MainWindow::start,(Lexical*)autoobject,&Lexical::receive);
    connect((Lexical*)autoobject,&Lexical::send,this,[=](QVector<QVector<QString>> m){
        queueSize=m.size();
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize));
        QTableWidget *o=(QTableWidget*)autooutput;
        o->setRowCount(m.size());
        o->setColumnCount(m[0].size());
        o->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        o->resizeRowsToContents();
        for(int i=0;i<m.size();i++)
        {
            for(int j=0;j<m[0].size();j++){
                o->setItem(i,j,new QTableWidgetItem(m[i][j]));
                o->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
//            o->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }
    });
}

void MainWindow::grammar()
{
    createNewPage = new SlidePage(cornerRadius, "GRAMMARANALYSE ", ui->mainWidget);
    QLineEdit *canvasName = new QLineEdit(this);
    canvasName->setMaximumHeight(15);
    QLineEdit *canvasDesc = new QLineEdit(this);
    canvasDesc->setMaximumHeight(15);

    QWidget *whiteSpace = new QWidget(createNewPage);
    whiteSpace->setFixedHeight(30);
    textButton *ll1 = new textButton("LL(1)",createNewPage);
    textButton *oop = new textButton("OPP",createNewPage);
    createNewPage->AddContent(oop);
    createNewPage->AddContent(ll1);
    createNewPage->AddContent(whiteSpace);
    createNewPage->show();
    createNewPage->slideIn();
    pageList.push_back(createNewPage);

    connect(ll1, &textButton::clicked, this, [=](){pageList.back()->slideOut();grammar_ll1();});
    connect(oop, &textButton::clicked, this, [=](){pageList.back()->slideOut();});
}

void MainWindow::divideAndConquer()
{

}

void MainWindow::dynamicProgramming()
{
    layersPage = new SlidePage(cornerRadius, "DYNAMICPROGRAMMING", ui->mainWidget);
    layersPage->stackUnder(createNewPage);
    layerSel = new singleSelectGroup("Layers", layersPage);
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});

    textButton *MaxSum = new textButton("maxsum", layersPage);
    layersPage->AddContent(MaxSum);
    connect(MaxSum, &textButton::clicked, this, [=](){pageList.back()->slideOut();maxsum();});
    layersPage->AddContent(layerSel);
    layersPage->show();
    pageList.push_back(layersPage);
    layersPage->slideIn();
}

void MainWindow::greedy()
{
    layersPage = new SlidePage(cornerRadius, "GREEDY", ui->mainWidget);
    layersPage->stackUnder(createNewPage);
    layerSel = new singleSelectGroup("Layers", layersPage);
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});

    textButton *Graph = new textButton("graph", layersPage);
    layersPage->AddContent(Graph);
    autobutton=Graph;
    connect(Graph, &textButton::clicked, this, [=](){
        graph();
        QVector<QWidget*> de;
        de.push_back(((QWidget*)autobutton));
        de.push_back(((QWidget*)autolog));
        layersPage->RemoveContents(de);

    });
    layersPage->AddContent(layerSel);
    autolog=layerSel;
    layersPage->show();
    pageList.push_back(layersPage);
    layersPage ->slideIn();
}

void MainWindow::backtracking()
{
    layersPage = new SlidePage(cornerRadius, "DIVIDE&&CONQUER", ui->mainWidget);
    layersPage->stackUnder(createNewPage);
    layerSel = new singleSelectGroup("Layers", layersPage);
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});

    textButton *Queen = new textButton("queen", layersPage);
    textButton *SubGroup = new textButton("subgroup", layersPage);
    layersPage->AddContent(Queen);
    layersPage->AddContent(SubGroup);
    connect(Queen, &textButton::clicked, this, [=](){pageList.back()->slideOut();generator->disable();queen();});
    connect(SubGroup, &textButton::clicked, this, [=](){pageList.back()->slideOut();generator->enable();subgroup();});
    layersPage->AddContent(layerSel);
    layersPage->show();
    pageList.push_back(layersPage);
    layersPage->slideIn();
}

void MainWindow::graph()
{

    bigIconButton *createNew = new bigIconButton(":/icons/icons/create.png", "Create new", 10, this);
    createNew->setScale(0.9);

    /* create layers page */
    //for add new page
    textInputItem *rename = new textInputItem("Name:",createNewPage);
    rename->setValue("Layer_" + QString::asprintf("%d", canvasList.size()));
    textInputItem *redescribe = new textInputItem("Detail:",createNewPage);
    redescribe->setValue("No description");

    layerSel = new singleSelectGroup("Layers", layersPage);
    connect(layerSel, &singleSelectGroup::itemChange, layersPage, [=](){layersPage->UpdateContents();});
    textButton *openFileBtn = new textButton("Open file", layersPage);
    connect(openFileBtn, &textButton::clicked, this, [=](){
        QString inputPath = QFileDialog::getOpenFileName(this, tr("Open map"), " ",  tr("Map File(*.map)"));
        if(!inputPath.isEmpty()){
            MyCanvas *newCanvas = loadCanvas(inputPath);
            if(newCanvas != nullptr){
                canvasList.push_back(newCanvas);
                selectionItem *newLayer = new selectionItem(newCanvas->name(), newCanvas->description(), layersPage);
                layerSel->AddItem(newLayer);
                layerSel->SetSelection(newLayer);
                pageList.push_back(newCanvas->settingPage());
                connect(newLayer, &selectionItem::selected, this, [=](){selectCanvas(newCanvas);});
                selectCanvas(newCanvas);
                connect(newCanvas, &MyCanvas::nameChanged, this, [=](QString text){
                    canvasTitle->setText(text);
                    canvasTitle->setMaximumWidth(QFontMetrics(QFont("Corbel Light", 24)).size(Qt::TextSingleLine, canvasTitle->text()).width() + 10);
                    newLayer->setTitle(text);
                });
                connect(newCanvas, &MyCanvas::descChanged, this, [=](QString text){this->canvasDesc->setText(text);newLayer->setDescription(text);});
                connect(newCanvas, &MyCanvas::setDel, this, [=](MyCanvas *c){curSettingsPage->slideOut();deleteCanvas(c);layerSel->RemoveItem(newLayer);});
                createNewPage->slideOut();
            }
        }
    });
    textButton *addNewBtn = new textButton("Create new", layersPage);
    layersPage->AddContent(addNewBtn);
    layersPage->AddContent(openFileBtn);

    layersPage->AddContent(layerSel);
    connect(addNewBtn, &textButton::clicked, this, [=](){layersPage->slideOut();rename->setValue("Layer_" + QString::asprintf("%d", canvasList.size()));redescribe->setValue("No description");createNewPage->slideIn();});
    layersPage->show();
    pageList.push_back(layersPage);

    /* create add new slide page */
    createNewPage = new SlidePage(cornerRadius, "CREATE CANVAS", ui->mainWidget);
    QLineEdit *canvasName = new QLineEdit(this);
    canvasName->setMaximumHeight(20);
    QLineEdit *canvasDesc = new QLineEdit(this);
    canvasDesc->setMaximumHeight(20);

    QWidget *whiteSpace = new QWidget(createNewPage);
    whiteSpace->setFixedHeight(30);
    singleSelectGroup *structureSel = new singleSelectGroup("Structure",createNewPage);
    selectionItem *item_1 = new selectionItem("AL", "Use adjacent list for canvas", createNewPage);
    selectionItem *item_2 = new selectionItem("AML", "Use multiple adjacent list for canvas", createNewPage);
    structureSel->AddItem(item_1);
    structureSel->AddItem(item_2);
    singleSelectGroup *dirSel = new singleSelectGroup("Mode", createNewPage);
    selectionItem *item_3 = new selectionItem("DG", "Directed graph", createNewPage);
    selectionItem *item_4 = new selectionItem("UDG", "Undirected graph", createNewPage);
    dirSel->AddItem(item_3);
    dirSel->AddItem(item_4);
    textButton *submit = new textButton("Create!", createNewPage);
    connect(submit, &textButton::clicked, this, [=](){
        MyCanvas *newCanvas = new MyCanvas(cornerRadius,
                                           rename->value(),
                                           redescribe->value(),
                                           structureSel->value() == 0 ? MyCanvas::AL : MyCanvas::AML,
                                           dirSel->value() == 0 ? MyCanvas::DG : MyCanvas::UDG, ui->mainWidget);
        canvasList.push_back(newCanvas);
        selectionItem *newLayer = new selectionItem(newCanvas->name(), newCanvas->description(), layersPage);
        layerSel->AddItem(newLayer);
        layerSel->SetSelection(newLayer);
        pageList.push_back(newCanvas->settingPage());
        connect(newLayer, &selectionItem::selected, this, [=](){selectCanvas(newCanvas);});
        selectCanvas(newCanvas);
        connect(newCanvas, &MyCanvas::nameChanged, this, [=](QString text){
            canvasTitle->setText(text);
            canvasTitle->setMaximumWidth(QFontMetrics(QFont("Corbel Light", 24)).size(Qt::TextSingleLine, canvasTitle->text()).width() + 10);
            newLayer->setTitle(text);
        });
        connect(newCanvas, &MyCanvas::descChanged, this, [=](QString text){this->canvasDesc->setText(text);newLayer->setDescription(text);});
        connect(newCanvas, &MyCanvas::setDel, this, [=](MyCanvas *c){curSettingsPage->slideOut();deleteCanvas(c);layerSel->RemoveItem(newLayer);});
        autobutton=newCanvas;
        connect(layersIcon, &customIcon::clicked, newCanvas, [=](){emit ((MyCanvas*)autobutton)->setDel((MyCanvas*)autobutton);});
        createNewPage->slideOut();
    });
    createNewPage->AddContent(submit);
    createNewPage->AddContent(dirSel);
    createNewPage->AddContent(structureSel);
    createNewPage->AddContent(whiteSpace);
    createNewPage->AddContent(redescribe);
    createNewPage->AddContent(rename);
    connect(createNew, &bigIconButton::clicked, createNewPage, [=](){rename->setValue("Layer_" + QString::asprintf("%d", canvasList.size()));redescribe->setValue("No description");createNewPage->slideIn();});
    createNewPage->show();
    pageList.push_back(createNewPage);
}

void MainWindow::maxsum()
{
    ui->displayLayout->removeWidget(defaultPage);
    delete defaultPage;
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bigIconButton *calculate = new bigIconButton(":/icons/icons/create.png", "calculate", 10, this);
    calculate->setScale(0.5);
    QLabel *log=new QLabel;
    autolog=log;
    QLineEdit *input=new QLineEdit;
    QTableWidget *output=new QTableWidget;
    QHBoxLayout *button=new QHBoxLayout;
    button->addWidget(calculate);
    button->addWidget(input);
    output->setMinimumWidth(400);
    output->setMinimumHeight(400);
    QGridLayout *right=new QGridLayout;
    right->addWidget(input,0,0,1,4);
    right->addWidget(log,1,0,1,1);
    right->addWidget(calculate,2,2,1,1);
    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setContentsMargins(10, 6, 10, 16);
    defaultPageLayout->setSpacing(20);
    defaultPageLayout->addWidget(output);
    defaultPageLayout->addLayout(right);
    ui->displayLayout->addWidget(defaultPage);
    autoinput=input;
    autooutput=output;
    autobutton=calculate;
    autoobject=new MaxSum;
    connect(calculate,&bigIconButton::clicked,this,[=](){
        QTableWidget *o=(QTableWidget*)autooutput;
        o->setRowCount(0);
        o->setColumnCount(0);
        QString text=((QLineEdit*)autoinput)->text();
        queueSize=0;
        emit start(text);
        QThreadPool::globalInstance()->start((MaxSum*)autoobject);
    });
    connect(this,&MainWindow::start,(MaxSum*)autoobject,&MaxSum::receive);
    connect((MaxSum*)autoobject,&MaxSum::send,this,[=](QVector<QString> q,QVector<int> a,QVector<QVector<QString>> p){
        QTableWidget *o=(QTableWidget*)autooutput;
        o->setColumnCount(p.size());
        o->setRowCount(p[0].size());
        o->resizeRowsToContents();
        QStringList headers;
        headers<<"j"<<"b[j]"<<"i"<<"j"<<"besti"<<"bestj"<<"sum";
        for(int i=0;i<p.size();i++)
        {
            for(int j=0;j<p[0].size();j++){
                o->setItem(i,j,new QTableWidgetItem(p[i][j]));
                o->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
            o->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }
        o->setHorizontalHeaderLabels(headers);
    });


}

void MainWindow::queen()
{
    ui->displayLayout->removeWidget(defaultPage);
    delete defaultPage;
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bigIconButton *forward = new bigIconButton(":/icons/icons/back.svg", "forward", 10, this);
    forward->setScale(0.2);
    bigIconButton *calculate = new bigIconButton(":/icons/icons/create.png", "calculate", 10, this);
    calculate->setScale(0.5);
    QLabel *log=new QLabel;
    autolog=log;
    QLineEdit *input=new QLineEdit;
    QTableWidget *output=new QTableWidget;
    QHBoxLayout *button=new QHBoxLayout;
    button->addWidget(forward);
    button->addWidget(calculate);
    button->addWidget(input);
    output->setMinimumWidth(400);
    output->setMinimumHeight(400);
    QGridLayout *right=new QGridLayout;
    right->addWidget(input,0,0,1,4);
    right->addWidget(log,1,0,1,1);
    right->addWidget(forward,2,0,1,1);
    right->addWidget(calculate,2,2,1,1);
    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setContentsMargins(10, 6, 10, 16);
    defaultPageLayout->setSpacing(20);
    defaultPageLayout->addWidget(output);
    defaultPageLayout->addLayout(right);
    ui->displayLayout->addWidget(defaultPage);
    autoinput=input;
    autooutput=output;
    autobutton=calculate;
    autoobject=new backtracking::Queen;
    connect(autoobject,&QObject::destroyed,this,[=](){autobutton->deleteLater();});
    connect(calculate,&bigIconButton::clicked,this,[=](){
        int n=((QLineEdit*)autoinput)->text().toInt();
        QTableWidget* o=(QTableWidget*)autooutput;
        o->setRowCount(n);
        o->setColumnCount(n);
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if((i+j)%2==0){
                    QLabel *black = new QLabel();     //创建lable
                    black->setPixmap(QPixmap(":/icons/icons/black.png").scaled(QSize(50,50)));    //加载图片
                    black->setAlignment(Qt::AlignHCenter);      //设置居中
                    black->setScaledContents(false);
                    o->setCellWidget(i,j,black);
                }
                else{
                    QLabel *white = new QLabel();     //创建lable
                    white->setPixmap(QPixmap(":/icons/icons/white.png").scaled(QSize(50,50)));    //加载图片
                    white->setAlignment(Qt::AlignHCenter);      //设置居中
                    white->setScaledContents(false);
                    o->setCellWidget(i,j,white);
                }
        o->resizeColumnsToContents();
        o->resizeRowsToContents();
        emit start(((QLineEdit*)autoinput)->text());
        QThreadPool::globalInstance()->start((backtracking::Queen*)autoobject);
        queueSize=0;
    });
    connect(this,&MainWindow::start,(backtracking::Queen*)autoobject,&backtracking::Queen::receive);
    connect((backtracking::Queen*)autoobject,&backtracking::Queen::send,this,[=](QQueue<QVector<int>> m){
        delete queueData;
        queueData=new QQueue<QVector<int>>(m);
        queueSize=queueData->size();
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize));
    });
    connect((backtracking::Queen*)autoobject,&backtracking::Queen::update,this,[=](int i){
        queueSize+=i;
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize));
    });
    connect(forward,&bigIconButton::clicked,this,[=](){
        if(queueData->size()==0)
            return;
        queueSize=queueData->size();
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize-1));
        QVector<int> solution;
        solution=queueData->dequeue();
        QTableWidget* o=(QTableWidget*)autooutput;
        int n=solution.size()-1;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if((i+j)%2==0){
                    QLabel *black = new QLabel();     //创建lable
                    black->setPixmap(QPixmap(":/icons/icons/black.png").scaled(QSize(50,50)));    //加载图片
                    black->setAlignment(Qt::AlignHCenter);      //设置居中
                    black->setScaledContents(false);
                    o->setCellWidget(i,j,black);
                }
                else{
                    QLabel *white = new QLabel();     //创建lable
                    white->setPixmap(QPixmap(":/icons/icons/white.png").scaled(QSize(50,50)));    //加载图片
                    white->setAlignment(Qt::AlignHCenter);      //设置居中
                    white->setScaledContents(false);
                    o->setCellWidget(i,j,white);
                }
        for(int i=0;i<n;i++)
        {
            QLabel *chess = new QLabel();     //创建lable
            chess->setPixmap(QPixmap(":/icons/icons/chess-queen.png").scaled(QSize(50,50)));    //加载图片
            chess->setAlignment(Qt::AlignHCenter);      //设置居中
            chess->setScaledContents(false);
            o->setCellWidget(i,solution[i+1]-1,chess);
        }
    });
}

void MainWindow::open()
{
        filepath = QFileDialog::getOpenFileName(this,tr("open a file."),"",tr("text(*.txt *.cpp);;All files(*.*)"));
        if (filepath.isEmpty())
                QMessageBox::warning(this, "warning!", "empty filepath!");
        else
        {
            QFile file(filepath);
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)||filepath.isEmpty())//只读
            {
                qDebug()<<"error filepath!";
            }
            QTextStream readStream(&file);
            while(!readStream.atEnd())	//没有读到文件末尾
            {
                //readLine 读取一行
                buffer+=readStream.readLine()+"\n";
            }
        }
}

void MainWindow::emitline(int i)
{
    QStringList lines = buffer.split('\n');
    if(i==lines.size())
        return;
    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->start(10);
    connect(t, &QTimer::timeout,this,[=](){
        emit read(lines[i]);
        emitline(i+1);
    });
}

void MainWindow::subgroup()
{
    ui->displayLayout->removeWidget(defaultPage);
    delete defaultPage;
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bigIconButton *forward = new bigIconButton(":/icons/icons/back.svg", "forward", 10, this);
    forward->setScale(0.2);
    bigIconButton *calculate = new bigIconButton(":/icons/icons/create.png", "calculate", 10, this);
    calculate->setScale(0.5);
    QLabel *log=new QLabel;
    autolog=log;
    QLineEdit *input=new QLineEdit;
    QTableWidget *output=new QTableWidget;
    QHBoxLayout *button=new QHBoxLayout;
    button->addWidget(forward);
    button->addWidget(calculate);
    button->addWidget(input);
    QGridLayout *right=new QGridLayout;
    right->addWidget(input,0,0,1,4);
    right->addWidget(log,1,0,1,1);
    right->addWidget(forward,2,0,1,1);
    right->addWidget(calculate,2,2,1,1);
    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setContentsMargins(10, 6, 10, 16);
    defaultPageLayout->setSpacing(20);
    defaultPageLayout->addWidget(output);
    defaultPageLayout->addLayout(right);
    ui->displayLayout->addWidget(defaultPage);
    autoinput=input;
    autooutput=output;
    backtracking::SubGroup *s=new backtracking::SubGroup;
    connect(calculate,&bigIconButton::clicked,this,[=](){
        QTableWidget *o=(QTableWidget*)autooutput;
        o->setRowCount(0);
        o->setColumnCount(0);
        QString text=((QLineEdit*)autoinput)->text();
        queueSize=0;
        emit start(text);
        QThreadPool::globalInstance()->start(s);
    });
    connect(this,&MainWindow::start,s,&backtracking::SubGroup::receive);
    connect(s,&backtracking::SubGroup::send,this,[=](QQueue<QVector<int>> m){
        delete queueData;
        queueData=new QQueue<QVector<int>>(m);
        queueSize=queueData->size();
        QLabel *log=(QLabel*)autolog;
        QTableWidget *o=(QTableWidget*)autooutput;
        o->setColumnCount(queueData->front().size());
        o->resizeColumnsToContents();
        log->setText(QString::asprintf("%d lefted",queueSize));
    });
    connect(s,&backtracking::SubGroup::update,this,[=](int i){
        queueSize+=i;
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize));
    });
    connect(forward,&bigIconButton::clicked,this,[=](){
        if(queueData->size()==0)
            return;
        queueSize=queueData->size();
        QLabel *log=(QLabel*)autolog;
        log->setText(QString::asprintf("%d lefted",queueSize-1));
        QVector<int> solution;
        solution=queueData->dequeue();
        int n=solution.size();
        QTableWidget* o=(QTableWidget*)autooutput;
        o->setRowCount(o->rowCount()+1);
        o->resizeRowsToContents();
        for(int i=0;i<n;i++)
        {
            if(solution[i]>0)
            o->setItem(o->rowCount()-1,i,new QTableWidgetItem(QString::number(solution[i])));
//            else
//            o->setItem(o->rowCount()-1,i,new QTableWidgetItem("0"));
        }
    });
}

void MainWindow::grammar_ll1()
{
    ui->displayLayout->removeWidget(defaultPage);
    delete defaultPage;
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bigIconButton *forward = new bigIconButton(":/icons/icons/back.svg", "analyse", 10, this);
    forward->setScale(0.2);
    bigIconButton *openfile = new bigIconButton(":/icons/icons/open.png", "openfile", 10, this);
    openfile->setScale(0.5);
    QLabel *log=new QLabel;
    autolog=log;
    QTextEdit *input=new QTextEdit;
    QTableWidget *output=new QTableWidget;
    QHBoxLayout *button=new QHBoxLayout;
    button->addWidget(forward);
    button->addWidget(openfile);
    button->addWidget(input);
    QGridLayout *right=new QGridLayout;
    right->addWidget(input,0,0,1,4);
    right->addWidget(log,1,0,1,1);
    right->addWidget(forward,2,0,1,1);
    right->addWidget(openfile,2,2,1,1);
    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setContentsMargins(10, 6, 10, 16);
    defaultPageLayout->setSpacing(20);
    defaultPageLayout->addWidget(output);
    defaultPageLayout->addLayout(right);
    ui->displayLayout->addWidget(defaultPage);
    autoinput=input;
    autooutput=output;
    connect(openfile,&bigIconButton::clicked,this,[=](){
        QTextEdit* i=((QTextEdit*)autoinput);
        i->clear();
        open();
        emitline(0);
    });
    connect(this,&MainWindow::read,this,[=](QString t){
        QTextEdit* i=((QTextEdit*)autoinput);
        i->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        i->append(t);


    });
    connect(forward,&bigIconButton::clicked,this,[=](){
        QString text=((QTextEdit*)autoinput)->toPlainText();
        emit start(text);
//        QThreadPool::globalInstance()->start();
    });
//    connect(this,&MainWindow::start,s,&backtracking::SubGroup::receive);
//    connect(s,&backtracking::SubGroup::send,this,[=](QQueue<QVector<int>> m){
//        delete queueData;
//        queueData=new QQueue<QVector<int>>(m);
//        queueSize=queueData->size();
//        QLabel *log=(QLabel*)autolog;
//        QTableWidget *o=(QTableWidget*)autooutput;
//        o->setColumnCount(queueData->front().size());
//        o->resizeColumnsToContents();
//        log->setText(QString::asprintf("%d lefted",queueSize));
//    });
}
void MainWindow::selectCanvas(MyCanvas *canvas){
    if(!curCanvas){
        ui->displayLayout->removeWidget(defaultPage);
        defaultPage->hide();
        ui->displayLayout->addWidget(canvas);
        canvas->show();
    }
    else{
        ui->displayLayout->removeWidget(curCanvas);
        curCanvas->hide();
        ui->displayLayout->addWidget(canvas);
        canvas->show();
    }
    curCanvas = canvas;
    canvas->settingPage()->setParent(ui->mainWidget);
    curSettingsPage = canvas->settingPage();
    canvasTitle->setText(curCanvas->name());
    canvasTitle->setMaximumWidth(QFontMetrics(QFont("Corbel Light", 24)).size(Qt::TextSingleLine, canvasTitle->text()).width() + 10);
    canvasDesc->setText(curCanvas->description());
}

void MainWindow::deleteCanvas(MyCanvas *canvas){
    int index = canvasList.indexOf(canvas);
    if(index < 0)
        return;
    canvasList.erase(canvasList.begin() + index);
    ui->displayLayout->removeWidget(curCanvas);
    curCanvas->hide();
    if(canvasList.size() > 0){
        selectCanvas(canvasList[0]);
    }
    else{
        ui->displayLayout->addWidget(defaultPage);
        defaultPage->show();
        curCanvas = nullptr;
        canvasTitle->setText("START");
        canvasTitle->setMaximumWidth(QFontMetrics(QFont("Corbel Light", 24)).size(Qt::TextSingleLine, "START").width() + 10);
        canvasDesc->setText("Add your first canvas to start");
        curSettingsPage = defaultSettingsPage;
    }
    pageList.erase(pageList.begin() + pageList.indexOf(canvas->settingPage()));
    delete canvas;
    ui->mainWidget->update();
}

MyCanvas *MainWindow::loadCanvas(const QString &path)
{
    QFile input(path);
    input.open(QIODevice::ReadOnly);
    QTextStream ts(&input);
    QString magicString = ts.readLine();
    if(magicString != "VFdGeWFXUnZaekl3TURJd05ESTE=")   return nullptr;
    MyCanvas *newCanvas = new MyCanvas(ts, cornerRadius, ui->mainWidget);
    input.close();
    return newCanvas;
}
void MainWindow::controlWindowScale(){
    if(!maximized){
        lastGeometry = this->frameGeometry();
        windowShadow->setEnabled(false);
        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
        border->hide();
        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + ";border-radius:0px;}";
        ui->mainWidget->setStyleSheet(mainStyle);
        this->showMaximized();
        maximized = true;
        QPainterPath path;
        path.addRect(ui->mainWidget->rect());
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);
    }
    else{
        ui->verticalLayout->setContentsMargins(1, 1, 1, 1);
        this->showNormal();
        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius) + "}";
        ui->mainWidget->setStyleSheet(mainStyle);
        QPainterPath path;
        path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);
        border->show();
        windowShadow->setEnabled(true);
        this->resize(lastGeometry.width(), lastGeometry.height());
        this->move(lastGeometry.x(), lastGeometry.y());
        maximized = false;
    }
}

