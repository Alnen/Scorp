#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <memory>
#include "StationsList/StationsListModel.h"
#include "Map/MapScene.h"

class ObjectItem
{
public:
    ObjectItem(PointGraphicsObject* item1, PointGraphicsObject* item2, MapScene* scene)
        : m_item1(item1), m_item2(item2), m_scene(scene)
    {
        //
    }

    void addToScene()
    {
        if (m_item1 && m_scene)
        {
            m_scene->addItem((QGraphicsItem*)m_item1);
        }
    }

public:
    PointGraphicsObject* m_item1;
    PointGraphicsObject* m_item2;
    MapScene* m_scene;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void resizeEvent(QResizeEvent * event);

    void defineMainMenu();
    void defineToolBar();
    void defineStatusBar();
    void defineAuthTurnOnForm();
    void defineAuthTurnOffForm();
    void defineLoginForm();
    void defineRegistrationForm();
    void defineMap();
    void defineStationsList();

    void defineUsersListForm();
    void defineStationsListForm();
    void defineTrainsListForm();
    void defineToursListForm();
    void defineTrainScheduleForm();
    void defineStationScheduleForm();
    void defineFindTourDialog();
    void defineAboutProgramForm();

private slots:
    void loadFromFile();
    void saveToFile();
    void saveAs();

    void btnEnterLoginOkClicked();
    void btnEnterLoginCloseClicked();
    void btnRegistrationOkClicked();
    void btnRegistrationCloseClicked();
    void btnAuthLoginClicked();
    void btnAuthLogoutClicked();
    void btnAuthTurnOnClicked();
    void btnAuthTurnOffClicked();

    void changeMode(MapMode mode);

    void openUsersListForm();
    void openEditProfileDialog();
    void openStationsListForm();
    void openTrainsListForm();
    void openToursListForm();
    void openTrainScheduleForm();
    void openStationScheduleForm();
    void openFindTourDialog();
    void showAboutProgramInfo();


private:
    QDialog* dlgEnterLogin;
    QDialog* dlgRegistration;
    QDialog* dlgShowNameList;

    QDialog* dlgUsersList;
    QDialog* dlgStationsList;
    QDialog* dlgTrainsList;
    QDialog* dlgToursList;
    QDialog* dlgTrainSchedule;
    QDialog* dlgStationSchedule;
    QDialog* dlgFindTour;
    QDialog* dlgAboutProgram;

    QTableWidget* tableUsers;
    QTableWidget* tableStations;
    QTableWidget* tableTrains;
    QTableWidget* tableTours;
    QTableWidget* tableTrainSchedule;
    QTableWidget* tableStationSchedule;

    QWidget* m_authForm;
    QTableWidget* tableShowNameList;

    //QGraphicsScene* m_mapScene;
    MapScene* m_mapScene;
    QGraphicsView* m_mapView;
    std::unique_ptr<StationsListModel> m_stationsListModel;
    QTreeView* m_stationsListView;
    // main window menu
    QMenuBar* m_mainMenu;
    QMenu* mnFile;
    QAction* actOpen;
    QAction* actSave;
    QAction* actSaveAs;
    QAction* actExit;

    QMenu* mnAccounts;
    QAction* actUsersList;
    QAction* actEditProfile;

    QMenu* mnMap;
    QMenu* mnSetMode;
    QAction* actSetViewMapMode;
    QAction* actSetMoveMapMode;
    QAction* actSetAddStateMapMode;
    QAction* actSetAddLinkMapMode;
    QAction* actSetDeleteMapMode;
    QAction* actStationsList;
    QAction* actTrainsList;
    QAction* actToursList;

    QMenu* mnSchedule;
    QAction* actTrainSchedule;
    QAction* actStationSchedule;
    QAction* actFindTour;

    QAction* actAbout;

    // ToolBar
    QToolBar* m_toolBar;
    QAction* actSetViewMode;
    QAction* actSetMoveMode;
    QAction* actSetAddStateMode;
    QAction* actSetAddLinkMode;
    QAction* actSetDeleteMode;

    // StatusBar
    QStatusBar* m_statusBar;
    QLabel* lbCurrentOperation;

    // auth form (TurnOn)
    QFrame* frameAuthTurnOn;
    QLabel* lbAuthTurnOnLogin;
    QLabel* lbAuthTurnOnGroup;
    QPushButton* btnAuthLogin;
    QPushButton* btnAuthLogout;
    QPushButton* btnAuthTurnOff;

    // auth form (TurnOff)
    QWidget* frameAuthTurnOff;
    QLabel* lbAuthTurnOffLogin;
    QPushButton* btnAuthTurnOn;

    // login enter form
    QLineEdit* txtEnterLogin;
    QLineEdit* txtEnterPassword;
    QPushButton* btnEnterLoginOk;
    QPushButton* btnEnterLoginClose;

    // registration form
    QLineEdit* txtRegistrationLogin;
    QLineEdit* txtRegistrationPassword;
    QComboBox* cmbRegistrationGroup;
    QPushButton* btnRegistrationOk;
    QPushButton* btnRegistrationClose;
};

#endif // MAINWINDOW_H
