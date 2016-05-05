#include "mainwindow.h"

#include "Map/TrackGraphicsObject.h"
#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"
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

#include <QResizeEvent>
#include <QDebug>

#include "Map/GraphicsObjectsGroup.h"

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

    dlgShowNameList = new QDialog(this, Qt::Dialog);
    dlgShowNameList->setFixedSize(300, 500);
    dlgShowNameList->setWindowTitle(tr(""));
    tableShowNameList = new QTableWidget(dlgShowNameList);
    QHeaderView* hv = new QHeaderView(Qt::Horizontal);
    tableShowNameList->setHorizontalHeader(hv);
    hv->hide();
    tableShowNameList->setColumnCount(1);
    hv->setSectionResizeMode(0, QHeaderView::Stretch);
    tableShowNameList->setGeometry(10, 10, dlgShowNameList->width()-20, dlgShowNameList->height()-20);
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
    defineUsersListForm();
    defineStationsListForm();
    defineTrainsListForm();
    defineToursListForm();
    defineTrainScheduleForm();
    defineStationScheduleForm();
    defineFindTourDialog();
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

    mnSchedule = new QMenu(tr("Schedule"));
    actTrainSchedule = new QAction(tr("Train Schedule"), mnSchedule);
    actStationSchedule = new QAction(tr("Station Schedule"), mnSchedule);
    actFindTour = new QAction(tr("Find Tour"), mnSchedule);
    mnSchedule->addAction(actTrainSchedule);
    mnSchedule->addAction(actStationSchedule);
    mnSchedule->addAction(actFindTour);

    actAbout = new QAction(tr("About"), mnAccounts);
    QList<QAction*> actions;
    actions << actAbout;

    m_mainMenu = new QMenuBar(this);
    m_mainMenu->addMenu(mnFile);
    m_mainMenu->addMenu(mnAccounts);
    m_mainMenu->addMenu(mnMap);
    m_mainMenu->addMenu(mnSchedule);
    m_mainMenu->addActions(actions);
    this->setMenuBar(m_mainMenu);

    connect(actOpen, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(actExit, &QAction::triggered, this, &MainWindow::close);
    connect(actUsersList, &QAction::triggered, dlgUsersList, &QDialog::open);
    connect(actEditProfile, &QAction::triggered, this, &MainWindow::openEditProfileDialog);
    connect(actFindTour, &QAction::triggered, dlgFindTour, &QDialog::open);
    connect(actAbout, &QAction::triggered, this, &MainWindow::showAboutProgramInfo);
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
    //ObjectItem* obj_item = new ObjectItem(state, state2, m_mapScene);
    //obj_item.item1 = state;
    //obj_item.item2 = state2;
    //m_mapScene->addItem(obj_item.m_item1);
    //m_mapScene->addItem(obj_item.item2);
    //obj_item->addToScene();

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
    QPushButton* btnAddUser = new QPushButton(tr("Add"), dlgUsersList);
    btnAddUser->setGeometry(btnBack->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnEditUser = new QPushButton(tr("Edit"), dlgUsersList);
    btnEditUser->setGeometry(btnAddUser->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnRemoveUser = new QPushButton(tr("Remove"), dlgUsersList);
    btnRemoveUser->setGeometry(btnEditUser->x() - button_width - 2, btnBack->y(), button_width, 20);
    QPushButton* btnClearUser = new QPushButton(tr("Clear"), dlgUsersList);
    btnClearUser->setGeometry(btnRemoveUser->x() - button_width - 2, btnBack->y(), button_width, 20);

    tableUsers = new QTableWidget(dlgUsersList);
    tableUsers->setColumnCount(2);
    tableUsers->setHorizontalHeaderLabels(QStringList() << tr("User") << tr("Group"));
    tableUsers->setColumnWidth(0, 100);
    tableUsers->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tableUsers->horizontalHeader()->setStretchLastSection(true);
    tableUsers->setGeometry(10, 10, dlgUsersList->width()-20, btnBack->y() - 15);
}

void MainWindow::defineStationsListForm()
{
    dlgStationsList = new QDialog(this, Qt::Dialog);
    dlgStationsList->setFixedSize(300, 400);
    dlgStationsList->setWindowTitle(tr("Stations List"));
}

void MainWindow::defineTrainsListForm()
{
    dlgTrainsList = new QDialog(this, Qt::Dialog);
    dlgTrainsList->setFixedSize(300, 400);
    dlgTrainsList->setWindowTitle(tr("Trains List"));
}

void MainWindow::defineToursListForm()
{
    dlgToursList = new QDialog(this, Qt::Dialog);
    dlgToursList->setFixedSize(300, 400);
    dlgToursList->setWindowTitle(tr("Tours List"));
}

void MainWindow::defineTrainScheduleForm()
{
    dlgTrainSchedule = new QDialog(this, Qt::Dialog);
    dlgTrainSchedule->setFixedSize(300, 400);
    dlgTrainSchedule->setWindowTitle(tr("Train Schedule"));
}

void MainWindow::defineStationScheduleForm()
{
    dlgStationSchedule = new QDialog(this, Qt::Dialog);
    dlgStationSchedule->setFixedSize(300, 400);
    dlgStationSchedule->setWindowTitle(tr("Station Schedule"));
}

void MainWindow::defineFindTourDialog()
{
    dlgFindTour = new QDialog(this, Qt::Dialog);
    dlgFindTour->setFixedSize(300, 400);
    dlgFindTour->setWindowTitle(tr("Find Tour"));
    //
    QLineEdit* txtDeparturePlace = new QLineEdit(dlgFindTour);
    txtDeparturePlace->setPlaceholderText(tr("Departure Place"));
    txtDeparturePlace->setGeometry(10, 10, 200, 20);
    QLineEdit* txtDepartureTime = new QLineEdit(dlgFindTour);
    txtDepartureTime->setPlaceholderText(tr("Departure Time"));
    txtDepartureTime->setGeometry(txtDeparturePlace->x(),
                                  txtDeparturePlace->y() + txtDeparturePlace->height() + 5, 200, 20);
    QLineEdit* txtArrivalPlace = new QLineEdit(dlgFindTour);
    txtArrivalPlace->setPlaceholderText(tr("Arrival Place"));
    txtArrivalPlace->setGeometry(txtDepartureTime->x(),
                                 txtDepartureTime->y() + txtDepartureTime->height() + 5, 200, 20);
    QLineEdit* txtArrivalTime = new QLineEdit(dlgFindTour);
    txtArrivalTime->setPlaceholderText(tr("Arrival Time"));
    txtArrivalTime->setGeometry(txtArrivalPlace->x(), txtArrivalPlace->y() + txtArrivalPlace->height() + 5,
                                200, 20);

    QPushButton* btnBack = new QPushButton(tr("Cancel"), dlgFindTour);
    btnBack->setGeometry(dlgFindTour->width() - 90, dlgFindTour->height() - 30, 80, 20);
    QPushButton* btnFind = new QPushButton(tr("Find"), dlgFindTour);
    btnFind->setGeometry(btnBack->x() - 90, btnBack->y(), 80, 20);
}

void MainWindow::defineAboutProgramForm()
{
    dlgAboutProgram = new QDialog(this, Qt::Dialog);
    dlgAboutProgram->setFixedSize(300, 400);
    dlgAboutProgram->setWindowTitle(tr("About Program"));
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
    //
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
    //
}

void MainWindow::openStationScheduleForm()
{
    //
}

void MainWindow::openFindTourDialog()
{
    //
}

void MainWindow::showAboutProgramInfo()
{
    //
}
