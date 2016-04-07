#include "mainwindow.h"

#include "Map/TrackGraphicsObject.h"
#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QGraphicsLineItem>
#include <QDesktopWidget>
#include <QApplication>

#include <QResizeEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //this->setFixedSize(QSize(600, 500));
    QRect desktop_screen = QApplication::desktop()->screen()->rect();
    QPoint center_pos = desktop_screen.center();
    int window_width = 0.992 * desktop_screen.width();
    int window_height = 0.922 * desktop_screen.height();
    this->setMinimumSize(400, 200);
    this->setGeometry(center_pos.x() - 0.5*window_width, center_pos.y() - 0.5*window_height,
                      window_width, window_height);
    this->setWindowTitle(tr("SCORP"));
    dlgEnterLogin = new QDialog(this, Qt::Dialog);
    dlgEnterLogin->setFixedSize(205, 120);
    dlgEnterLogin->setWindowTitle(tr("Enter Login"));
    dlgRegistration = new QDialog(this, Qt::Dialog);
    dlgRegistration->setFixedSize(205, 140);
    dlgRegistration->setWindowTitle(tr("Registration"));

    defineMainMenu();
    defineToolBar();
    defineStatusBar();
    defineLoginForm();
    defineRegistrationForm();
    defineMap();
    defineStationsList();
    defineAuthTurnOnForm();
    defineAuthTurnOffForm();

    //dlgEnterLogin->show();
    //dlgRegistration->show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QSize new_size = event->size();
    // move login form (passive)
    int frame_width = 120;
    frameAuthTurnOff->setGeometry(new_size.width() - frame_width - 2, 0, frame_width, 20);
    // move login form (active)
    frameAuthTurnOn->setGeometry(new_size.width() - frame_width - 2, 0, frame_width, 85);
    // move status bar
    m_statusBar->setGeometry(0, new_size.height()-24, new_size.width(), 24);
    // resize map
    int offset_y = m_toolBar->y() + m_toolBar->height();
    m_mapView->setGeometry(0, offset_y, new_size.width(),
                           new_size.height() - offset_y - m_statusBar->height());
    // move and resize station list
    bool hasHorizontalScrollBar = (m_mapView->width() <= m_mapScene->width());
    bool hasVerticalScrollBar = (m_mapView->height() <= m_mapScene->height());
    int list_width = 150;
    int list_x = new_size.width() - list_width - 2 - (hasHorizontalScrollBar ? 17 : 0);
    int list_height = (m_mapView->height() > (hasHorizontalScrollBar ? 417 : 400) ? 400 :
                       m_mapView->height() - 4 - (hasVerticalScrollBar ? 17 : 0));
    m_stationsListView->setGeometry(list_x, m_mapView->y() + 2,
                                    list_width, list_height);
}

void MainWindow::defineMainMenu()
{
    mnFile = new QMenu(tr("File"));
    actOpen = new QAction(tr("Open"), mnFile);
    actSave = new QAction(tr("Save"), mnFile);
    actSaveAs = new QAction(tr("Save As"), mnFile);
    actExit = new QAction(tr("Exit"), mnFile);
    mnFile->addAction(actOpen);
    mnFile->addAction(actSave);
    mnFile->addAction(actSaveAs);
    mnFile->addAction(actExit);

    m_mainMenu = new QMenuBar(this);
    m_mainMenu->addMenu(mnFile);
    this->setMenuBar(m_mainMenu);

    connect(actOpen, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(actExit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::defineToolBar()
{
    m_toolBar = addToolBar(tr("ToolBar"));
    m_toolBar->setGeometry(0, m_mainMenu->y() + m_mainMenu->height(), this->width(), 20);
    m_toolBar->setMovable(false);
    actSetViewMode = new QAction(tr("View"), m_toolBar);
    actSetMoveMode = new QAction(tr("Move"), m_toolBar);
    actSetAddStateMode = new QAction(tr("Add State"), m_toolBar);
    actSetAddTransitionMode = new QAction(tr("Add Transition"), m_toolBar);
    actSetAddTraceMode = new QAction(tr("Add Track"), m_toolBar);
    actSetDeleteMode = new QAction(tr("Delete"), m_toolBar);
    QList<QAction*> actions_list;
    actions_list << actSetViewMode
                 << actSetMoveMode
                 << actSetAddStateMode
                 << actSetAddTransitionMode
                 << actSetAddTraceMode
                 << actSetDeleteMode;
    m_toolBar->addActions(actions_list);
    //m_toolbar->addSeparator();
}

void MainWindow::defineStatusBar()
{
    m_statusBar = new QStatusBar(this);
    m_statusBar->setGeometry(0, this->height()-24, this->width(), 24);
    lbCurrentOperation = new QLabel(tr("Current Operation: View"), m_statusBar);
    lbCurrentOperation->setGeometry(20, 2, 270, 22);
    m_statusBar->setFixedHeight(26);
    this->setStatusBar(m_statusBar);
}

void MainWindow::defineAuthTurnOnForm()
{
    int frame_width = 120;
    frameAuthTurnOn = new QFrame(this);
    frameAuthTurnOn->setStyleSheet("background-color:#eeeeee;");
    frameAuthTurnOn->setFrameStyle(QFrame::Box);
    frameAuthTurnOn->setGeometry(this->width() - frame_width - 2, 0, frame_width, 85);
    QLabel* lbLogin = new QLabel(tr("Login:"), frameAuthTurnOn);
    lbLogin->setGeometry(10, 10, 50, 20);
    lbAuthTurnOnLogin = new QLabel(tr(""), frameAuthTurnOn);
    lbAuthTurnOnLogin->setGeometry(lbLogin->x() + lbLogin->width() + 5, lbLogin->y(), 80, 20);
    QLabel* lbGroup = new QLabel(tr("Group:"), frameAuthTurnOn);
    lbGroup->setGeometry(lbLogin->x(), lbLogin->y() + lbLogin->height() + 5, lbLogin->width(), 20);
    lbAuthTurnOnGroup = new QLabel(tr(""), frameAuthTurnOn);
    lbAuthTurnOnGroup->setGeometry(lbGroup->x() + lbGroup->width() + 5, lbGroup->y(),
                                   lbAuthTurnOnLogin->width(), 20);
    btnAuthLogin = new QPushButton(tr("Log In"), frameAuthTurnOn);
    int btn_width = 50;
    btnAuthLogin->setGeometry(frameAuthTurnOn->width() - btn_width - 1, frameAuthTurnOn->height() - 21,
                              btn_width, 20);
    btnAuthLogout = new QPushButton(tr("Log Out"), frameAuthTurnOn);
    btnAuthLogout->setGeometry(btnAuthLogin->geometry());
    btnAuthLogout->hide();
    btnAuthTurnOff = new QPushButton(tr("Turn Off"), frameAuthTurnOn);
    btnAuthTurnOff->setGeometry(frameAuthTurnOn->width() - 21, 1, 20, 20);
    frameAuthTurnOn->hide();

    connect(btnAuthLogin, &QPushButton::clicked, this, &MainWindow::btnAuthLoginClicked);
    connect(btnAuthLogout, &QPushButton::clicked, this, &MainWindow::btnAuthLogoutClicked);
    connect(btnAuthTurnOff, &QPushButton::clicked, this, &MainWindow::btnAuthTurnOffClicked);
}

void MainWindow::defineAuthTurnOffForm()
{
    int frame_width = 120;
    frameAuthTurnOff = new QWidget(this);
    frameAuthTurnOff->setGeometry(this->width() - frame_width - 2, 0, frame_width, 20);
    lbAuthTurnOffLogin = new QLabel(tr("Login:"), frameAuthTurnOff);
    lbAuthTurnOffLogin->setGeometry(0, 0, 95, 20);
    lbAuthTurnOffLogin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    btnAuthTurnOn = new QPushButton(tr("Turn On"), frameAuthTurnOff);
    btnAuthTurnOn->setGeometry(lbAuthTurnOffLogin->x() + lbAuthTurnOffLogin->width() + 5, 0, 20, 20);
    frameAuthTurnOff->show();

    connect(btnAuthTurnOn, &QPushButton::clicked, this, &MainWindow::btnAuthTurnOnClicked);
}

void MainWindow::defineLoginForm()
{
    QLabel* lbLogin = new QLabel(tr("Login"), dlgEnterLogin);
    lbLogin->setGeometry(12, 15, 50, 20);
    txtEnterLogin = new QLineEdit(dlgEnterLogin);
    txtEnterLogin->setGeometry(lbLogin->x() + lbLogin->width() + 5, lbLogin->y(), 125, 20);
    QLabel* lbPassword = new QLabel(tr("Password"), dlgEnterLogin);
    lbPassword->setGeometry(lbLogin->x(), lbLogin->y() + lbLogin->height() + 7, lbLogin->width(), 20);
    txtEnterPassword = new QLineEdit(dlgEnterLogin);
    txtEnterPassword->setGeometry(lbPassword->x() + lbPassword->width() + 5, lbPassword->y(),
                                  txtEnterLogin->width(), 20);
    btnEnterLoginClose = new QPushButton(tr("Cancel"), dlgEnterLogin);
    btnEnterLoginClose->setGeometry(dlgEnterLogin->width() - 70, dlgEnterLogin->height() - 30, 60, 20);
    btnEnterLoginOk = new QPushButton(tr("Ok"), dlgEnterLogin);
    btnEnterLoginOk->setGeometry(btnEnterLoginClose->x() - 55, btnEnterLoginClose->y(), 50, 20);

    connect(btnEnterLoginClose, &QPushButton::clicked, this, &MainWindow::btnEnterLoginCloseClicked);
    connect(btnEnterLoginOk, &QPushButton::clicked, this, &MainWindow::btnEnterLoginOkClicked);
}

void MainWindow::defineRegistrationForm()
{
    QLabel* lbLogin = new QLabel(tr("Login"), dlgRegistration);
    lbLogin->setGeometry(12, 15, 50, 20);
    txtRegistrationLogin = new QLineEdit(dlgRegistration);
    txtRegistrationLogin->setGeometry(lbLogin->x() + lbLogin->width() + 5, lbLogin->y(), 125, 20);
    QLabel* lbPassword = new QLabel(tr("Password"), dlgRegistration);
    lbPassword->setGeometry(lbLogin->x(), lbLogin->y() + lbLogin->height() + 7, lbLogin->width(), 20);
    txtRegistrationPassword = new QLineEdit(dlgRegistration);
    txtRegistrationPassword->setGeometry(lbPassword->x() + lbPassword->width() + 5, lbPassword->y(),
                                  txtRegistrationLogin->width(), 20);

    QLabel* lbGroup = new QLabel(tr("Group"), dlgRegistration);
    lbGroup->setGeometry(lbPassword->x(), lbPassword->y() + lbPassword->height() + 7, lbPassword->width(), 20);
    cmbRegistrationGroup = new QComboBox(dlgRegistration);
    cmbRegistrationGroup->setGeometry(lbGroup->x() + lbGroup->width() + 5, lbGroup->y(),
                                      txtRegistrationPassword->width(), 20);
    cmbRegistrationGroup->addItem(tr("Operator"));
    cmbRegistrationGroup->addItem(tr("Admin"));
    btnRegistrationClose = new QPushButton(tr("Cancel"), dlgRegistration);
    btnRegistrationClose->setGeometry(dlgRegistration->width() - 70, dlgRegistration->height() - 30, 60, 20);
    btnRegistrationOk = new QPushButton(tr("Ok"), dlgRegistration);
    btnRegistrationOk->setGeometry(btnRegistrationClose->x() - 55, btnRegistrationClose->y(), 50, 20);

    connect(btnRegistrationClose, &QPushButton::clicked, this, &MainWindow::btnRegistrationOkClicked);
    connect(btnRegistrationOk, &QPushButton::clicked, this, &MainWindow::btnRegistrationCloseClicked);
}

void MainWindow::defineMap()
{
    m_mapView = new QGraphicsView(this);
    int offset_y = m_toolBar->y() + m_toolBar->height() - 7;
    m_mapView->setGeometry(0, offset_y, this->width(), this->height() - offset_y - m_statusBar->height());
    m_mapScene = new MapScene(0, 0, m_mapView->width(), m_mapView->height(), this);
    m_mapView->setScene(m_mapScene);
    m_mapView->setDragMode(QGraphicsView::RubberBandDrag);

    connect(m_mapScene, &MapScene::itemsUpdated, [this](){m_mapView->repaint();});
    //connect(m_mapScene, &MapScene::itemsUpdated, [this](){m_mapView->update();});
    connect(actSetViewMode, &QAction::triggered, [this](){this->changeMode(MapMode::View);});
    connect(actSetMoveMode, &QAction::triggered, [this](){this->changeMode(MapMode::Move);});
    connect(actSetAddStateMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddState);});
    connect(actSetAddTransitionMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddTransition);});
    connect(actSetAddTraceMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddTrack);});
    connect(actSetDeleteMode, &QAction::triggered, [this](){this->changeMode(MapMode::Delete);});
}

void MainWindow::defineStationsList()
{
    m_stationsListModel = std::move(std::unique_ptr<StationsListModel>(new StationsListModel()));//new StationsListModel();
    m_stationsListModel->appendStation(tr("St. 1"));
    m_stationsListModel->appendStation(tr("St. 2"));
    m_stationsListModel->appendStation(tr("St. 3"));
    m_stationsListModel->appendTrain(0, 1);
    m_stationsListModel->appendTrain(0, 5);
    m_stationsListModel->appendTrain(0, 6);
    m_stationsListModel->appendTrain(1, 2);
    m_stationsListModel->appendTrain(1, 4);
    m_stationsListModel->appendTrain(2, 3);

    m_stationsListView = new QTreeView(this);
    m_stationsListView->setModel(m_stationsListModel->model());

    bool hasHorizontalScrollBar = (m_mapView->width() <= m_mapScene->width());
    bool hasVerticalScrollBar = (m_mapView->height() <= m_mapScene->height());
    int list_width = 150;
    int list_x = this->width() - list_width - 2 - (hasHorizontalScrollBar ? 17 : 0);
    int list_height = (m_mapView->height() > (hasHorizontalScrollBar ? 417 : 400) ? 400 :
                       m_mapView->height() - 4 - (hasVerticalScrollBar ? 17 : 0));

    m_stationsListView->setGeometry(list_x, m_mapView->y() + 2, list_width, list_height);
    m_stationsListView->setHeaderHidden(true);
    m_stationsListView->setIndentation(20);
}

void MainWindow::loadFromFile()
{
    //
}

void MainWindow::saveToFile()
{
    //
}
void MainWindow::saveAs()
{
    //
}

void MainWindow::btnEnterLoginOkClicked()
{
    dlgEnterLogin->close();
}

void MainWindow::btnEnterLoginCloseClicked()
{
    dlgEnterLogin->close();
}

void MainWindow::btnRegistrationOkClicked()
{
    //
}

void MainWindow::btnRegistrationCloseClicked()
{
    //
}

void MainWindow::btnAuthLoginClicked()
{
    dlgEnterLogin->show();
}

void MainWindow::btnAuthLogoutClicked()
{
    //
}

void MainWindow::btnAuthTurnOnClicked()
{
    frameAuthTurnOff->hide();
    frameAuthTurnOn->show();
}

void MainWindow::btnAuthTurnOffClicked()
{
    frameAuthTurnOn->hide();
    frameAuthTurnOff->show();
}

void MainWindow::changeMode(MapMode mode)
{
    m_mapScene->setMode(mode);
    switch (mode)
    {
    case MapMode::View:
        lbCurrentOperation->setText(tr("Current Operation: View"));
        break;
    case MapMode::Move:
        lbCurrentOperation->setText(tr("Current Operation: Move"));
        break;
    case MapMode::AddState:
        lbCurrentOperation->setText(tr("Current Operation: Add State"));
        break;
    case MapMode::AddTransition:
        lbCurrentOperation->setText(tr("Current Operation: Add Transition"));
        break;
    case MapMode::AddTrack:
        lbCurrentOperation->setText(tr("Current Operation: Add Track"));
        break;
    case MapMode::Delete:
        lbCurrentOperation->setText(tr("Current Operation: Delete"));
        break;
    }
}
