#include "mainwindow.h"
#include <QTreeView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFrame>
#include <QWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QGraphicsLineItem>
#include <QDesktopWidget>
#include <QApplication>
#include <QIcon>
#include <QPixmap>
#include <QActionGroup>
#include <QDialog>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QMessageBox>

#include <QResizeEvent>
#include <QDebug>

#include "DB/ScorpDBSell.h"
#include "Map/MapScene.h"
#include "Map/TrackGraphicsObject.h"
#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"
#include "Map/GraphicsObjectsGroup.h"
#include "StationsList/TreeModel.h"
#include "StationsList/StationsListModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_db = new ScorpDBSell("../Scorp/DB/ScorpDB.db", false);
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

    //tableShowAllUsers->insertRow(tableShowAllUsers->rowCount());
    //tableShowAllUsers->setItem(tableShowAllUsers->rowCount()-1, 0, new QTableWidgetItem(tr("User1")));
    /*
    int count = 30;
    tableShowAllUsers->setRowCount(count);
    for (int i = 0; i < count; ++i)
    {
        tableShowAllUsers->setItem(i, 0, new QTableWidgetItem(tr("User%1").arg(i+1)));
    }
    */
    defineEditUserForm();
    defineEditStationForm();
    defineEditTrainForm();
    defineEditTourForm();
    defineTrainScheduleForm();
    defineStationScheduleForm();

    defineUsersListForm();
    defineStationsListForm();
    defineTrainsListForm();
    defineToursListForm();
    defineTrainScheduleForm();
    defineStationScheduleForm();
    defineFindTourDialog();
    defineEditDateTimeForm();
    defineAboutProgramForm();

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

    mnAccounts = new QMenu(tr("Accounts"));
    actUsersList = new QAction(tr("Users List"), mnAccounts);
    actEditProfile = new QAction(tr("Edit Profile"), mnAccounts);
    mnAccounts->addAction(actUsersList);
    mnAccounts->addAction(actEditProfile);

    mnMap = new QMenu(tr("Map"));
    mnSetMode = new QMenu(tr("Set Mode"));
    actSetViewMapMode = new QAction(tr("View"), mnSetMode);
    actSetMoveMapMode = new QAction(tr("Move"), mnSetMode);
    actSetAddStateMapMode = new QAction(tr("Add State"), mnSetMode);
    actSetAddLinkMapMode = new QAction(tr("Add Link"), mnSetMode);
    actSetDeleteMapMode = new QAction(tr("Delete"), mnSetMode);
    actStationsList = new QAction(tr("Stations List"), mnMap);
    actTrainsList = new QAction(tr("Trains List"), mnMap);
    actToursList = new QAction(tr("Tours List"), mnMap);
    actSetViewMapMode->setCheckable(true);
    actSetMoveMapMode->setCheckable(true);
    actSetAddStateMapMode->setCheckable(true);
    actSetAddLinkMapMode->setCheckable(true);
    actSetDeleteMapMode->setCheckable(true);
    QActionGroup* map_modes = new QActionGroup(mnSetMode);
    map_modes->addAction(actSetViewMapMode);
    map_modes->addAction(actSetMoveMapMode);
    map_modes->addAction(actSetAddStateMapMode);
    map_modes->addAction(actSetAddLinkMapMode);
    map_modes->addAction(actSetDeleteMapMode);
    mnSetMode->addActions(map_modes->actions());
    actSetViewMapMode->setChecked(true);
    mnMap->addMenu(mnSetMode);
    mnMap->addAction(actStationsList);
    mnMap->addAction(actTrainsList);
    mnMap->addAction(actToursList);

    actFindTour = new QAction(tr("Find Tour"), mnMap);
    actAbout = new QAction(tr("About"), mnFile);
    QList<QAction*> actions;
    actions << actFindTour;
    actions << actAbout;

    m_mainMenu = new QMenuBar(this);
    m_mainMenu->addMenu(mnFile);
    m_mainMenu->addMenu(mnAccounts);
    m_mainMenu->addMenu(mnMap);
    m_mainMenu->addActions(actions);
    this->setMenuBar(m_mainMenu);

    connect(actOpen, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(actExit, &QAction::triggered, this, &MainWindow::close);
    connect(actUsersList, &QAction::triggered, dlgUsersList, &QDialog::open);
    connect(actStationsList, &QAction::triggered, dlgStationsList, &QDialog::open);
    connect(actTrainsList, &QAction::triggered, dlgTrainsList, &QDialog::open);
    connect(actToursList, &QAction::triggered, dlgToursList, &QDialog::open);
    connect(actEditProfile, &QAction::triggered, this, &MainWindow::openEditProfileDialog);
    connect(actFindTour, &QAction::triggered, dlgFindTour, &QDialog::open);
    connect(actAbout, &QAction::triggered, dlgAboutProgram, &QDialog::open);
}

void MainWindow::defineToolBar()
{
    m_toolBar = addToolBar(tr("ToolBar"));
    m_toolBar->setGeometry(0, m_mainMenu->y() + m_mainMenu->height(), this->width(), 20);
    m_toolBar->setMovable(false);
    actSetViewMode = new QAction(QIcon(QPixmap("../Scorp/images/view.png")), tr("View"), m_toolBar);
    actSetMoveMode = new QAction(QIcon(QPixmap("../Scorp/images/move.png")), tr("Move"), m_toolBar);
    actSetAddStateMode = new QAction(QIcon(QPixmap("../Scorp/images/state.png")), tr("Add State"), m_toolBar);
    actSetAddLinkMode = new QAction(QIcon(QPixmap("../Scorp/images/link.png")), tr("Add Link"), m_toolBar);
    actSetDeleteMode = new QAction(QIcon(QPixmap("../Scorp/images/delete.png")), tr("Delete"), m_toolBar);

    QList<QAction*> actions_list;
    actions_list << actSetViewMode
                 << actSetMoveMode
                 << actSetAddStateMode
                 << actSetAddLinkMode
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

    /*
    StateGraphicsObject* state = new StateGraphicsObject(0, 100, 100, 10);
    m_mapScene->addItem(state);

    StateGraphicsObject* state2 = new StateGraphicsObject(1, 300, 300, 10);
    m_mapScene->addItem(state2);
    LinkGraphicsObject link(2, state, state2);
    std::vector<PointGraphicsObject*> link_items(link.getLinkParts());
    for (size_t i = 0; i < link_items.size(); ++i)
    {
        m_mapScene->addItem(link_items[i]);
    }
    */

    /*

    GraphicsObjectsGroup group;
    group.addItem(state);
    //group.addItemsToScene();
    m_mapScene->addItem(group.getItems()[0]);
    */

    /*
    TrackGraphicsObject* track = new TrackGraphicsObject(state, state2);
    m_mapScene->addItem(track);
    */
    /*
    TransitionGraphicsObject* transition = new TransitionGraphicsObject(200, 200,
                                                                        6, 18, QColor::fromRgb(0, 200, 200));
    transition->setRotation(225);
    m_mapScene->addItem(transition);
    TrackGraphicsObject* track = new TrackGraphicsObject(state, transition);
    m_mapScene->addItem(track);
    */

    m_mapView->setScene(m_mapScene);
    m_mapView->setDragMode(QGraphicsView::RubberBandDrag);

    //connect(m_mapScene, &MapScene::itemsUpdated, [this](){m_mapView->repaint();});
    connect(actSetViewMode, &QAction::triggered, [this](){this->changeMode(MapMode::View);});
    connect(actSetMoveMode, &QAction::triggered, [this](){this->changeMode(MapMode::Move);});
    connect(actSetAddStateMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddState);});
    connect(actSetAddLinkMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddLink);});
    connect(actSetDeleteMode, &QAction::triggered, [this](){this->changeMode(MapMode::Delete);});

    connect(actSetViewMapMode, &QAction::triggered, [this](){this->changeMode(MapMode::View);});
    connect(actSetMoveMapMode, &QAction::triggered, [this](){this->changeMode(MapMode::Move);});
    connect(actSetAddStateMapMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddState);});
    connect(actSetAddLinkMapMode, &QAction::triggered, [this](){this->changeMode(MapMode::AddLink);});
    connect(actSetDeleteMapMode, &QAction::triggered, [this](){this->changeMode(MapMode::Delete);});
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

void MainWindow::defineUsersListForm()
{
    dlgUsersList = new QDialog(this, Qt::Dialog);
    dlgUsersList->setFixedSize(400, 400);
    dlgUsersList->setWindowTitle(tr("Users List"));

    int button_width = 74;
    QPushButton* btnBack = new QPushButton(tr("Back"), dlgUsersList);
    btnBack->setGeometry(dlgUsersList->width() - button_width - 11, dlgUsersList->height() - 30, button_width, 20);
    QPushButton* btnAdd = new QPushButton(tr("Add"), dlgUsersList);
    btnAdd->setGeometry(btnBack->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnEdit = new QPushButton(tr("Edit"), dlgUsersList);
    btnEdit->setGeometry(btnAdd->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnRemove = new QPushButton(tr("Remove"), dlgUsersList);
    btnRemove->setGeometry(btnEdit->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnClear = new QPushButton(tr("Clear"), dlgUsersList);
    btnClear->setGeometry(btnRemove->x() - button_width - 2, btnBack->y(), button_width, 20);

    tableUsers = new QTableWidget(dlgUsersList);
    tableUsers->setColumnCount(2);
    tableUsers->setHorizontalHeaderLabels(QStringList() << tr("User") << tr("Group"));
    tableUsers->setColumnWidth(0, 100);
    tableUsers->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableUsers->horizontalHeader()->setStretchLastSection(true);
    tableUsers->setGeometry(10, 10, dlgUsersList->width()-20, btnBack->y() - 45);

    QLabel* lbShowMode = new QLabel(tr("Show:"), dlgUsersList);
    lbShowMode->setGeometry(tableUsers->x(), tableUsers->y() + tableUsers->height() + 5, 40, 20);
    QComboBox* cmbShowMode = new QComboBox(dlgUsersList);
    cmbShowMode->addItem(tr("All"));
    cmbShowMode->addItem(tr("Only Operators"));
    cmbShowMode->addItem(tr("Only Admins"));
    cmbShowMode->setGeometry(lbShowMode->x() + lbShowMode->width() + 5, lbShowMode->y(), 100, 20);

    connect(btnBack, &QPushButton::clicked, dlgUsersList, &QDialog::close);
    connect(btnAdd, &QPushButton::clicked, [this](){this->openEditUserForm(true);});
    connect(btnEdit, &QPushButton::clicked, [this](){this->openEditUserForm(false);});
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeUserFromList);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearUsersList);
    connect(cmbShowMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeUserGroupShowMode(int)));
    //connect(cmbShowMode, &QComboBox::currentIndexChanged, this, &MainWindow::changeUserGroupShowMode);
}

void MainWindow::defineStationsListForm()
{
    dlgStationsList = new QDialog(this, Qt::Dialog);
    dlgStationsList->setFixedSize(400, 400);
    dlgStationsList->setWindowTitle(tr("Stations List"));

    int button_width = 74;
    QPushButton* btnBack = new QPushButton(tr("Back"), dlgStationsList);
    btnBack->setGeometry(dlgStationsList->width() - button_width - 11, dlgStationsList->height() - 30, button_width, 20);
    QPushButton* btnAdd = new QPushButton(tr("Add"), dlgStationsList);
    btnAdd->setGeometry(btnBack->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnEdit = new QPushButton(tr("Edit"), dlgStationsList);
    btnEdit->setGeometry(btnAdd->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnRemove = new QPushButton(tr("Remove"), dlgStationsList);
    btnRemove->setGeometry(btnEdit->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnClear = new QPushButton(tr("Clear"), dlgStationsList);
    btnClear->setGeometry(btnRemove->x() - button_width - 2, btnBack->y(), button_width, 20);

    QPushButton* btnShowSchedule = new QPushButton(tr("Schedule"), dlgStationsList);
    btnShowSchedule->setGeometry(10, btnClear->y() - 25, dlgStationsList->width()-20, 20);

    tableStations = new QTableWidget(dlgStationsList);
    tableStations->setColumnCount(1);
    tableStations->setHorizontalHeaderLabels(QStringList() << tr("Station"));
    tableStations->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableStations->setGeometry(10, 10, dlgStationsList->width()-20, btnShowSchedule->y() - 15);
    tableStations->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnBack, &QPushButton::clicked, dlgStationsList, &QDialog::close);
    connect(btnAdd, &QPushButton::clicked, [this](){this->openEditStationForm(true);});
    connect(btnEdit, &QPushButton::clicked, [this](){this->openEditStationForm(false);});
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeStationFromList);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearStationsList);
    connect(btnShowSchedule, &QPushButton::clicked, this, &MainWindow::openStationScheduleForm);
}

void MainWindow::defineTrainsListForm()
{
    dlgTrainsList = new QDialog(this, Qt::Dialog);
    dlgTrainsList->setFixedSize(400, 400);
    dlgTrainsList->setWindowTitle(tr("Trains List"));

    int button_width = 74;
    QPushButton* btnBack = new QPushButton(tr("Back"), dlgTrainsList);
    btnBack->setGeometry(dlgTrainsList->width() - button_width - 11, dlgTrainsList->height() - 30, button_width, 20);
    QPushButton* btnAdd = new QPushButton(tr("Add"), dlgTrainsList);
    btnAdd->setGeometry(btnBack->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnEdit = new QPushButton(tr("Edit"), dlgTrainsList);
    btnEdit->setGeometry(btnAdd->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnRemove = new QPushButton(tr("Remove"), dlgTrainsList);
    btnRemove->setGeometry(btnEdit->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnClear = new QPushButton(tr("Clear"), dlgTrainsList);
    btnClear->setGeometry(btnRemove->x() - button_width - 2, btnBack->y(), button_width, 20);

    QPushButton* btnShowSchedule = new QPushButton(tr("Schedule"), dlgTrainsList);
    btnShowSchedule->setGeometry(10, btnClear->y() - 25, dlgTrainsList->width()-20, 20);

    tableTrains = new QTableWidget(dlgTrainsList);
    tableTrains->setColumnCount(2);
    tableTrains->setHorizontalHeaderLabels(QStringList() << tr("Train") << tr("Route"));
    tableTrains->setColumnWidth(0, 100);
    tableTrains->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableTrains->horizontalHeader()->setStretchLastSection(true);
    tableTrains->setGeometry(10, 10, dlgTrainsList->width()-20, btnShowSchedule->y() - 15);
    tableTrains->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnBack, &QPushButton::clicked, dlgTrainsList, &QDialog::close);
    connect(btnAdd, &QPushButton::clicked, [this](){this->openEditTrainForm(true);});
    connect(btnEdit, &QPushButton::clicked, [this](){this->openEditTrainForm(false);});
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeTrainFromList);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearTrainsList);
    connect(btnShowSchedule, &QPushButton::clicked, this, &MainWindow::openTrainScheduleForm);
}

void MainWindow::defineToursListForm()
{
    dlgToursList = new QDialog(this, Qt::Dialog);
    dlgToursList->setFixedSize(400, 400);
    dlgToursList->setWindowTitle(tr("Tours List"));

    int button_width = 74;
    QPushButton* btnBack = new QPushButton(tr("Back"), dlgToursList);
    btnBack->setGeometry(dlgToursList->width() - button_width - 11, dlgToursList->height() - 30, button_width, 20);
    QPushButton* btnAdd = new QPushButton(tr("Add"), dlgToursList);
    btnAdd->setGeometry(btnBack->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnEdit = new QPushButton(tr("Edit"), dlgToursList);
    btnEdit->setGeometry(btnAdd->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnRemove = new QPushButton(tr("Remove"), dlgToursList);
    btnRemove->setGeometry(btnEdit->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnClear = new QPushButton(tr("Clear"), dlgToursList);
    btnClear->setGeometry(btnRemove->x() - button_width - 2, btnBack->y(), button_width, 20);

    tableTours = new QTableWidget(dlgToursList);
    tableTours->setColumnCount(1);
    tableTours->setHorizontalHeaderLabels(QStringList() << tr("Tour"));
    tableTours->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableTours->setGeometry(10, 10, dlgToursList->width()-20, btnBack->y() - 15);
    tableTours->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnBack, &QPushButton::clicked, dlgToursList, &QDialog::close);
    connect(btnAdd, &QPushButton::clicked, [this](){this->openEditTourForm(true);});
    connect(btnEdit, &QPushButton::clicked, [this](){this->openEditTourForm(false);});
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeTourFromList);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearToursList);
}

void MainWindow::defineFindTourDialog()
{
    dlgFindTour = new QDialog(this, Qt::Dialog);
    dlgFindTour->setFixedSize(532, 400);
    dlgFindTour->setWindowTitle(tr("Find Tour"));
    QLabel* lbDeparturePlace = new QLabel(tr("Departure Place:"), dlgFindTour);
    lbDeparturePlace->setGeometry(10, 10, 80, 20);
    int offset_x = lbDeparturePlace->x() + lbDeparturePlace->width() + 10;
    cmbDeparturePlace = new QComboBox(dlgFindTour);
    cmbDeparturePlace->setGeometry(offset_x, lbDeparturePlace->y(), 300, 20);
    QLabel* lbDepartureTime = new QLabel(tr("Departure Time:"), dlgFindTour);
    lbDepartureTime->setGeometry(lbDeparturePlace->x(), lbDeparturePlace->y() + lbDeparturePlace->height() + 5,
                                  lbDeparturePlace->width(), 20);
    dateDeparture = new QDateTimeEdit(dlgFindTour);
    dateDeparture->setGeometry(cmbDeparturePlace->x(), lbDepartureTime->y(), cmbDeparturePlace->width(), 20);
    QPushButton* btnDepartureTime = new QPushButton(tr("..."), dlgFindTour);
    btnDepartureTime->setGeometry(dateDeparture->x() + dateDeparture->width() - 20, dateDeparture->y(), 20, 20);
    QLabel* lbArrivalPlace = new QLabel(tr("Arrival Place:"), dlgFindTour);
    lbArrivalPlace->setGeometry(lbDepartureTime->x(), lbDepartureTime->y() + lbDepartureTime->height() + 5,
                                  lbDepartureTime->width(), 20);
    cmbArrivalPlace = new QComboBox(dlgFindTour);
    cmbArrivalPlace->setGeometry(cmbDeparturePlace->x(), lbArrivalPlace->y(), cmbDeparturePlace->width(), 20);
    QLabel* lbArrivalTime = new QLabel(tr("Arrival Time:"), dlgFindTour);
    lbArrivalTime->setGeometry(lbArrivalPlace->x(), lbArrivalPlace->y() + lbArrivalPlace->height() + 5,
                                  lbArrivalPlace->width(), 20);
    dateArrival = new QDateTimeEdit(dlgFindTour);
    dateArrival->setGeometry(cmbDeparturePlace->x(), lbArrivalTime->y(), cmbDeparturePlace->width(), 20);
    QPushButton* btnArrivalTime = new QPushButton(tr("..."), dlgFindTour);
    btnArrivalTime->setGeometry(dateArrival->x() + dateArrival->width() - 20, dateArrival->y(), 20, 20);

    QPushButton* btnBack = new QPushButton(tr("Cancel"), dlgFindTour);
    btnBack->setGeometry(dlgFindTour->width() - 90, dlgFindTour->height() - 30, 80, 20);
    QPushButton* btnFind = new QPushButton(tr("Find"), dlgFindTour);
    btnFind->setGeometry(btnBack->x() - 90, btnBack->y(), 80, 20);

    tableFoundTours = new QTableWidget(dlgFindTour);
    tableFoundTours->setColumnCount(6);
    tableFoundTours->setHorizontalHeaderLabels(QStringList() << tr("Tour") << tr("Train")
                                               << tr("Departure\nstation") << tr("Departure\ntime")
                                               << tr("Arrival\nstation") << tr("Arrival\ntime"));
    tableFoundTours->setColumnWidth(0, 100);
    tableFoundTours->setColumnWidth(1, 50);
    tableFoundTours->setColumnWidth(2, 100);
    tableFoundTours->setColumnWidth(3, 80);
    tableFoundTours->setColumnWidth(4, 100);
    tableFoundTours->setColumnWidth(5, 80);

    for (int i = 0; i < 6; ++i)
    {
        tableFoundTours->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
    }
    int offset_y = lbArrivalTime->y() + lbArrivalTime->height() + 5;
    tableFoundTours->setGeometry(10, offset_y, dlgFindTour->width()-20, btnFind->y() - offset_y - 10);
    tableFoundTours->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnDepartureTime, &QPushButton::clicked, [this](){this->openEditDateTimeForm(false);});
    connect(btnArrivalTime, &QPushButton::clicked, [this](){this->openEditDateTimeForm(true);});
    connect(btnBack, &QPushButton::clicked, dlgFindTour, &QDialog::close);
}

void MainWindow::defineAboutProgramForm()
{
    dlgAboutProgram = new QDialog(this, Qt::Dialog);
    dlgAboutProgram->setFixedSize(250, 200);
    dlgAboutProgram->setWindowTitle(tr("About SCORP"));
    QLabel* lbInfo = new QLabel(tr("<center><h2>SCORP</h2><center>"), dlgAboutProgram);
    lbInfo->setGeometry(0, 5, dlgAboutProgram->width(), 20);
    QLabel* lbInfo2 = new QLabel(tr("<center>Created with Qt 5.5<center>"), dlgAboutProgram);
    lbInfo2->setGeometry(0, lbInfo->y() + lbInfo->height() + 5, dlgAboutProgram->width(), 20);
    QPushButton* btnOk = new QPushButton(tr("Ok"), dlgAboutProgram);
    int button_width = 70;
    btnOk->setGeometry(dlgAboutProgram->width() - button_width - 10, dlgAboutProgram->height() - 30,
                       button_width, 20);

    connect(btnOk, &QPushButton::clicked, dlgAboutProgram, &QDialog::close);
}

void MainWindow::defineEditUserForm()
{
    dlgEditUser = new QDialog(this, Qt::Dialog);
    dlgEditUser->setFixedSize(200, 150);
    QLabel* lbLogin = new QLabel(tr("Login:"), dlgEditUser);
    lbLogin->setGeometry(10, 10, 55, 20);
    int offset_x = lbLogin->x() + lbLogin->width() + 5;
    txtUserLogin = new QLineEdit(dlgEditUser);    
    txtUserLogin->setGeometry(offset_x, lbLogin->y(), dlgEditUser->width() - 10 - offset_x, 20);
    QLabel* lbPassword = new QLabel(tr("Password:"), dlgEditUser);
    lbPassword->setGeometry(lbLogin->x(), lbLogin->y() + lbLogin->height() + 5, lbLogin->width(), 20);
    txtUserPassword = new QLineEdit(dlgEditUser);
    txtUserPassword->setGeometry(offset_x, lbPassword->y(), txtUserLogin->width(), 20);
    QLabel* lbGroup = new QLabel(tr("Group:"), dlgEditUser);
    lbGroup->setGeometry(lbPassword->x(), lbPassword->y() + lbPassword->height() + 5, lbLogin->width(), 20);
    cmbUserGroup = new QComboBox(dlgEditUser);
    cmbUserGroup->addItem(tr("User"));
    cmbUserGroup->addItem(tr("Operator"));
    cmbUserGroup->addItem(tr("Admin"));
    cmbUserGroup->setGeometry(offset_x, lbGroup->y(), txtUserLogin->width(), 20);

    QPushButton* btnCancel = new QPushButton(tr("Cancel"), dlgEditUser);
    btnCancel->setGeometry(dlgEditUser->width() - 90, dlgEditUser->height() - 30, 80, 20);
    btnAcceptUserChanges = new QPushButton(tr("Accept"), dlgEditUser);
    btnAcceptUserChanges->setGeometry(btnCancel->x() - 85, btnCancel->y(), 80, 20);
    btnAddUser = new QPushButton(tr("Add"), dlgEditUser);
    btnAddUser->setGeometry(btnAcceptUserChanges->geometry());

    connect(btnCancel, &QPushButton::clicked, dlgEditUser, &QDialog::close);
    connect(btnAcceptUserChanges, &QPushButton::clicked, this, &MainWindow::acceptUserChanges);
    connect(btnAddUser, &QPushButton::clicked, this, &MainWindow::addUser);
}

void MainWindow::defineEditStationForm()
{
    dlgEditStation = new QDialog(this, Qt::Dialog);
    dlgEditStation->setFixedSize(200, 150);
    QLabel* lbStationName = new QLabel(tr("Name:"), dlgEditStation);
    lbStationName->setGeometry(10, 10, 40, 20);
    int offset_x = lbStationName->x() + lbStationName->width() + 5;
    txtEditedStationName = new QLineEdit(dlgEditStation);
    txtEditedStationName->setGeometry(offset_x, lbStationName->y(), dlgEditStation->width() - 10 - offset_x, 20);
    QLabel* lbStationCoords = new QLabel(tr("Coords:"), dlgEditStation);
    lbStationCoords->setGeometry(lbStationName->x(), lbStationName->y() + lbStationName->height() + 5,
                                 lbStationName->width(), 20);
    txtEditedStationX = new QLineEdit(dlgEditStation);
    txtEditedStationX->setPlaceholderText(tr("X"));
    txtEditedStationX->setGeometry(lbStationCoords->x() + lbStationCoords->width() + 5, lbStationCoords->y(),
                                   (txtEditedStationName->width() - 5)/2, 20);
    txtEditedStationY = new QLineEdit(dlgEditStation);
    txtEditedStationY->setPlaceholderText(tr("Y"));
    txtEditedStationY->setGeometry(txtEditedStationX->x() + txtEditedStationX->width() + 5, txtEditedStationX->y(),
                                   txtEditedStationX->width(), 20);
    /*
    QLabel* lbTrains = new QLabel(tr("Trains"), dlgEditStation);
    lbTrains->setGeometry(dlgEditStation->width() * 0.5 - 50, lbStationCoords->y() + lbStationCoords->height() + 5, 100, 20);
    QTableWidget* tableStationTrains = new QTableWidget(dlgEditStation);
    tableStationTrains->setGeometry(10, lbTrains->y() + lbTrains->height() + 5, dlgEditStation->width()-20, 100);
    */
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), dlgEditStation);
    btnCancel->setGeometry(dlgEditStation->width() - 90, dlgEditStation->height() - 30, 80, 20);
    btnAcceptStationChanges = new QPushButton(tr("Accept"), dlgEditStation);
    btnAcceptStationChanges->setGeometry(btnCancel->x() - 85, btnCancel->y(), 80, 20);
    btnAddStation = new QPushButton(tr("Add"), dlgEditStation);
    btnAddStation->setGeometry(btnAcceptStationChanges->geometry());

    connect(btnCancel, &QPushButton::clicked, dlgEditStation, &QDialog::close);
    connect(btnAcceptStationChanges, &QPushButton::clicked, this, &MainWindow::acceptStationChanges);
    connect(btnAddStation, &QPushButton::clicked, this, &MainWindow::addStation);
}

void MainWindow::defineEditTrainForm()
{
    dlgEditTrain = new QDialog(this, Qt::Dialog);
    dlgEditTrain->setFixedSize(250, 150);
    QLabel* lbTrainNumber = new QLabel(tr("Number:"), dlgEditTrain);
    lbTrainNumber->setGeometry(10, 10, 80, 20);
    int offset_x = lbTrainNumber->x() + lbTrainNumber->width() + 5;
    txtEditedTrainNumber = new QLineEdit(dlgEditTrain);
    txtEditedTrainNumber->setGeometry(offset_x, lbTrainNumber->y(),
                                      dlgEditTrain->width() - 10 - offset_x, 20);
    QLabel* lbTrainStation = new QLabel(tr("Current Station:"), dlgEditTrain);
    lbTrainStation->setGeometry(lbTrainNumber->x(), lbTrainNumber->y() + lbTrainNumber->height() + 5,
                                lbTrainNumber->width(), 20);
    txtEditedTrainStation = new QLineEdit(dlgEditTrain);
    txtEditedTrainStation->setGeometry(offset_x, lbTrainStation->y(),
                                       txtEditedTrainNumber->width(), 20);
    QLabel* lbTrainRoute = new QLabel(tr("Route:"), dlgEditTrain);
    lbTrainRoute->setGeometry(lbTrainStation->x(), lbTrainStation->y() + lbTrainStation->height() + 5,
                              lbTrainNumber->width(), 20);
    txtEditedTrainRoute = new QLineEdit(dlgEditTrain);
    txtEditedTrainRoute->setGeometry(offset_x, lbTrainRoute->y(), txtEditedTrainNumber->width(), 20);
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), dlgEditTrain);
    btnCancel->setGeometry(dlgEditTrain->width() - 90, dlgEditTrain->height() - 30, 80, 20);
    btnAcceptTrainChanges = new QPushButton(tr("Accept"), dlgEditTrain);
    btnAcceptTrainChanges->setGeometry(btnCancel->x() - 85, btnCancel->y(), 80, 20);
    btnAddTrain = new QPushButton(tr("Add"), dlgEditTrain);
    btnAddTrain->setGeometry(btnAcceptTrainChanges->geometry());

    connect(btnCancel, &QPushButton::clicked, dlgEditTrain, &QDialog::close);
    connect(btnAcceptTrainChanges, &QPushButton::clicked, this, &MainWindow::acceptTrainChanges);
    connect(btnAddTrain, &QPushButton::clicked, this, &MainWindow::addTrain);
}

void MainWindow::defineEditTourForm()
{
    dlgEditTour = new QDialog(this, Qt::Dialog);
    dlgEditTour->setFixedSize(250, 300);
    QLabel* lbTourName = new QLabel(tr("Name:"), dlgEditTour);
    lbTourName->setGeometry(10, 10, 35, 20);
    int offset_x = lbTourName->x() + lbTourName->width() + 5;
    txtEditedTourName = new QLineEdit(dlgEditTour);
    txtEditedTourName->setGeometry(offset_x, lbTourName->y(), dlgEditTour->width() -10 - offset_x, 20);
    QLabel* lbStations = new QLabel(tr("Stations"), dlgEditTour);
    lbStations->setAlignment(Qt::AlignHCenter);
    lbStations->setGeometry(10, lbTourName->y() + lbTourName->height() + 10,
                            dlgEditTour->width() - 20, 20);
    tableEditedTourStations = new QTableWidget(dlgEditTour);
    tableEditedTourStations->setGeometry(10, lbStations->y() + lbStations->height(),
                                         dlgEditTour->width() - 20, 200);
    tableEditedTourStations->setEditTriggers(QTableWidget::NoEditTriggers);
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), dlgEditTour);
    btnCancel->setGeometry(dlgEditTour->width() - 90, dlgEditTour->height() - 30, 80, 20);
    btnAcceptTourChanges = new QPushButton(tr("Accept"), dlgEditTour);
    btnAcceptTourChanges->setGeometry(btnCancel->x() - 85, btnCancel->y(), 80, 20);
    btnAddTour = new QPushButton(tr("Add"), dlgEditTour);
    btnAddTour->setGeometry(btnAcceptTourChanges->geometry());

    connect(btnCancel, &QPushButton::clicked, dlgEditTour, &QDialog::close);
    connect(btnAcceptTourChanges, &QPushButton::clicked, this, &MainWindow::acceptTourChanges);
    connect(btnAddTour, &QPushButton::clicked, this, &MainWindow::addTour);
}

void MainWindow::defineTrainScheduleForm()
{
    dlgTrainSchedule = new QDialog(this, Qt::Dialog);
    dlgTrainSchedule->setFixedSize(320, 400);
    dlgTrainSchedule->setWindowTitle(tr("Train Schedule"));
    QLabel* lbTrainNumber = new QLabel(tr("Number:"), dlgTrainSchedule);
    lbTrainNumber->setGeometry(10, 10, 45, 20);
    txtSelectedTrainNumber = new QLineEdit(dlgTrainSchedule);
    int offset_x = lbTrainNumber->x() + lbTrainNumber->width() + 5;
    txtSelectedTrainNumber->setGeometry(offset_x, lbTrainNumber->y(),
                                        dlgTrainSchedule->width() - 10 - offset_x, 20);
    txtSelectedTrainNumber->setEnabled(false);
    int button_width = 74;
    QPushButton* btnOk = new QPushButton(tr("Ok"), dlgTrainSchedule);
    btnOk->setGeometry(dlgTrainSchedule->width() - button_width - 11, dlgTrainSchedule->height() - 30, button_width, 20);

    tableTrainSchedule = new QTableWidget(dlgTrainSchedule);
    tableTrainSchedule->setColumnCount(3);
    tableTrainSchedule->setHorizontalHeaderLabels(QStringList() << tr("Arrival Time") << tr("WaitTime") << tr("Station"));
    tableTrainSchedule->setColumnWidth(0, 80);
    tableTrainSchedule->setColumnWidth(1, 80);
    QHeaderView* header = tableTrainSchedule->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setStretchLastSection(true);
    int offset_y = txtSelectedTrainNumber->y() + txtSelectedTrainNumber->height() + 5;
    tableTrainSchedule->setGeometry(10, offset_y, dlgTrainSchedule->width()-20, btnOk->y() - offset_y - 5);
    tableTrainSchedule->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnOk, &QPushButton::clicked, dlgTrainSchedule, &QDialog::close);
    /*
    for (int i = 0; i < 100; ++i)
    {
        tableTrainSchedule->insertRow(tableTrainSchedule->rowCount());
        tableTrainSchedule->setItem(tableTrainSchedule->rowCount() - 1, 0, new QTableWidgetItem(tr("a_%1").arg(i)));
        tableTrainSchedule->setItem(tableTrainSchedule->rowCount() - 1, 1, new QTableWidgetItem(tr("b_%1").arg(i)));
        tableTrainSchedule->setItem(tableTrainSchedule->rowCount() - 1, 2, new QTableWidgetItem(tr("station_%1").arg(i)));
        tableTrainSchedule->resizeColumnToContents(2);
    }
    */
}

void MainWindow::defineStationScheduleForm()
{
    dlgStationSchedule = new QDialog(this, Qt::Dialog);
    dlgStationSchedule->setFixedSize(320, 400);
    dlgStationSchedule->setWindowTitle(tr("Station Schedule"));
    QLabel* lbStationName = new QLabel(tr("Name:"), dlgStationSchedule);
    lbStationName->setGeometry(10, 10, 35, 20);
    int offset_x = lbStationName->x() + lbStationName->width() + 5;
    txtSelectedStationName = new QLineEdit(dlgStationSchedule);
    txtSelectedStationName->setGeometry(offset_x, lbStationName->y(),
                                        dlgStationSchedule->width() - 10 - offset_x, 20);
    txtSelectedStationName->setEnabled(false);
    int button_width = 74;
    QPushButton* btnOk = new QPushButton(tr("Ok"), dlgStationSchedule);
    btnOk->setGeometry(dlgStationSchedule->width() - button_width - 11, dlgStationSchedule->height() - 30, button_width, 20);

    tableStationSchedule = new QTableWidget(dlgStationSchedule);
    tableStationSchedule->setColumnCount(3);
    tableStationSchedule->setHorizontalHeaderLabels(QStringList() << tr("Arrival Time") << tr("WaitTime") << tr("Train"));
    tableStationSchedule->setColumnWidth(0, 80);
    tableStationSchedule->setColumnWidth(1, 80);
    QHeaderView* header = tableStationSchedule->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setStretchLastSection(true);
    int offset_y = txtSelectedStationName->y() + txtSelectedStationName->height() + 5;
    tableStationSchedule->setGeometry(10, offset_y, dlgStationSchedule->width()-20,
                                      btnOk->y() - offset_y - 5);
    tableStationSchedule->setEditTriggers(QTableWidget::NoEditTriggers);

    connect(btnOk, &QPushButton::clicked, dlgStationSchedule, &QDialog::close);
}

void MainWindow::defineEditDateTimeForm()
{
    dlgEditDateTime = new QDialog(this, Qt::Dialog);
    dlgEditDateTime->setFixedSize(400, 350);
    dlgEditDateTime->setWindowTitle(tr("Choose Date and Time"));
    /*
    QLabel* lbDate = new QLabel(tr("Date"), dlgEditDateTime);
    lbDate->setAlignment(Qt::AlignHCenter);
    lbDate->setGeometry(10, 10, dlgEditDateTime->width() - 20, 20);
    */
    m_calendar = new QCalendarWidget(dlgEditDateTime);
    //m_calendar->setGeometry(10, lbDate->y() + lbDate->height(), dlgEditDateTime->width() - 20, 200);
    m_calendar->setGeometry(10, 10, dlgEditDateTime->width() - 20, 200);
    QLabel* lbTime = new QLabel(tr("Time:"), dlgEditDateTime);
    lbTime->setGeometry(m_calendar->x(), m_calendar->y() + m_calendar->height() + 5, 35, 20);
    int offset_x = lbTime->x() + lbTime->width() + 5;
    m_timeEdit = new QTimeEdit(dlgEditDateTime);
    m_timeEdit->setGeometry(offset_x, lbTime->y(), dlgEditDateTime->width() - 10 - offset_x, 20);

    QPushButton* btnCancel = new QPushButton(tr("Cancel"), dlgEditDateTime);
    btnCancel->setGeometry(dlgEditDateTime->width() - 80, dlgEditDateTime->height() - 30, 70, 20);
    btnAcceptDepartureTime = new QPushButton(tr("Ok"), dlgEditDateTime);
    btnAcceptDepartureTime->setGeometry(btnCancel->x() - 55, btnCancel->y(), 50, 20);
    btnAcceptArrivalTime = new QPushButton(tr("Ok"), dlgEditDateTime);
    btnAcceptArrivalTime->setGeometry(btnAcceptDepartureTime->geometry());

    connect(btnAcceptDepartureTime, &QPushButton::clicked, this, &MainWindow::acceptDepartureDateTimeChanges);
    connect(btnAcceptArrivalTime, &QPushButton::clicked, this, &MainWindow::acceptArrivalDateTimeChanges);
    connect(btnCancel, &QPushButton::clicked, dlgEditDateTime, &QDialog::close);
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
    //std::string s1 = txtEnterLogin->text().toStdString();
    //std::string s2 = txtEnterPassword->text().toStdString();
    //bool is_auth = m_db->authenticate("admin", "adminpasss");
    //"SELECT * FROM User WHERE Login=:login AND Password=:pass""
    bool is_auth = m_db->authenticate(txtEnterLogin->text().toStdString(),
                                     txtEnterPassword->text().toStdString());
    if (is_auth)
    {
        lbAuthTurnOnLogin->setText(txtEnterLogin->text());
        std::string user_group = m_db->getUserGroup(txtEnterLogin->text().toStdString());
        lbAuthTurnOnGroup->setText(QString::fromStdString(user_group));
        emit userChanged();
        dlgEnterLogin->close();
    }
    else
    {
        QMessageBox::warning(this, tr("Authenticate"), tr("Wrong login or password!"));
    }
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
    case MapMode::AddLink:
        lbCurrentOperation->setText(tr("Current Operation: Add Link"));
        break;
    case MapMode::Delete:
        lbCurrentOperation->setText(tr("Current Operation: Delete"));
        break;
    }
}

/*
void MainWindow::showAllOperators()
{
    dlgShowNameList->setWindowTitle(tr("All Operators"));
    if (tableShowNameList->rowCount() > 0)
    {
        tableShowNameList->clearContents();
    }
    // Make Request to DB
    dlgShowNameList->open();
}
*/


void MainWindow::openUsersListForm()
{
    dlgUsersList->open();
}

void MainWindow::openEditProfileDialog()
{
    openEditUserForm(false);
    dlgEditUser->setWindowTitle(tr("Edit Profile"));
}

void MainWindow::openStationsListForm()
{
    //
}

void MainWindow::openTrainsListForm()
{
    //
}

void MainWindow::openToursListForm()
{
    //
}

void MainWindow::openTrainScheduleForm()
{
    dlgTrainSchedule->open();
}

void MainWindow::openStationScheduleForm()
{
    dlgStationSchedule->open();
}

void MainWindow::openFindTourDialog()
{
    //
}

void MainWindow::showAboutProgramInfo()
{
    //
}

void MainWindow::changeUserGroupShowMode(int mode)
{
    if (mode == 0)
    {
        //
    }
    else if (mode == 1)
    {
        //
    }
    else
    {
        //
    }
}

void MainWindow::openEditUserForm(bool is_add_mode)
{
    if (is_add_mode)
    {
        dlgEditUser->setWindowTitle(tr("Add User"));
        txtUserLogin->setEnabled(true);
        btnAddUser->show();
        btnAcceptUserChanges->hide();
    }
    else
    {
        dlgEditUser->setWindowTitle(tr("Edit User"));
        txtUserLogin->setEnabled(false);
        btnAddUser->hide();
        btnAcceptUserChanges->show();
    }
    dlgEditUser->open();
}

void MainWindow::addUser()
{
    //
}

void MainWindow::acceptUserChanges()
{
    //
}

void MainWindow::clearUsersList()
{
    //
}

void MainWindow::removeUserFromList()
{
    //
}

void MainWindow::openEditStationForm(bool is_add_mode)
{
    if (is_add_mode)
    {
        dlgEditStation->setWindowTitle(tr("Add Station"));
        btnAddStation->show();
        btnAcceptStationChanges->hide();
    }
    else
    {
        dlgEditStation->setWindowTitle(tr("Edit Station"));
        btnAddStation->hide();
        btnAcceptStationChanges->show();
    }
    dlgEditStation->open();
}

void MainWindow::addStation()
{
    dlgEditStation->close();
}

void MainWindow::acceptStationChanges()
{
    dlgEditStation->close();
}

void MainWindow::clearStationsList()
{
    //
}

void MainWindow::removeStationFromList()
{
    //
}

void MainWindow::openEditTrainForm(bool is_add_mode)
{
    if (is_add_mode)
    {
        dlgEditTrain->setWindowTitle(tr("Add Train"));
        btnAddTrain->show();
        btnAcceptTrainChanges->hide();
    }
    else
    {
        dlgEditTrain->setWindowTitle(tr("Edit Train"));
        btnAddTrain->hide();
        btnAcceptTrainChanges->show();
    }
    dlgEditTrain->open();
}

void MainWindow::addTrain()
{
    dlgEditTrain->close();
}

void MainWindow::acceptTrainChanges()
{
    dlgEditTrain->close();
}

void MainWindow::clearTrainsList()
{
    //
}

void MainWindow::removeTrainFromList()
{
    //
}

void MainWindow::openEditTourForm(bool is_add_mode)
{
    if (is_add_mode)
    {
        dlgEditTour->setWindowTitle(tr("Add Tour"));
        btnAddTour->show();
        btnAcceptTourChanges->hide();
    }
    else
    {
        dlgEditTour->setWindowTitle(tr("Edit Tour"));
        btnAddTour->hide();
        btnAcceptTourChanges->show();
    }
    dlgEditTour->open();
}

void MainWindow::addTour()
{
    dlgEditTour->close();
}

void MainWindow::acceptTourChanges()
{
    dlgEditTour->close();
}

void MainWindow::clearToursList()
{
    //
}

void MainWindow::removeTourFromList()
{
    //
}

void MainWindow::openEditDateTimeForm(bool is_arrival)
{
    if (is_arrival)
    {
        m_calendar->setSelectedDate(dateArrival->date());
        m_timeEdit->setTime(dateArrival->time());
        btnAcceptDepartureTime->hide();
        btnAcceptArrivalTime->show();
    }
    else
    {
        m_calendar->setSelectedDate(dateDeparture->date());
        m_timeEdit->setTime(dateDeparture->time());
        btnAcceptDepartureTime->show();
        btnAcceptArrivalTime->hide();
    }
    dlgEditDateTime->open();
}

void MainWindow::acceptDepartureDateTimeChanges()
{
    dateDeparture->setDate(m_calendar->selectedDate());
    dateDeparture->setTime(m_timeEdit->time());
    dlgEditDateTime->close();
}

void MainWindow::acceptArrivalDateTimeChanges()
{
    dateArrival->setDate(m_calendar->selectedDate());
    dateArrival->setTime(m_timeEdit->time());
    dlgEditDateTime->close();
}
