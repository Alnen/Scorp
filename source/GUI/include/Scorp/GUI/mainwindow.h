#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include "Scorp/GUI/UserInfo.h"
#include "Scorp/DB/UserDBShell.h"
#include "Scorp/DB/RailwayNetDBShell.h"
#include "Scorp/GUI/Map/MapScene.h"

class QTreeView;
class QGraphicsView;
class QGraphicsScene;
class QToolBar;
class QMenuBar;
class QMenu;
class QAction;
class QLineEdit;
class QPushButton;
class QComboBox;
class QDialog;
class QLabel;
class QFrame;
class QWidget;
class QStatusBar;
class QTableWidget;
class QDateTime;
class QDateTimeEdit;
class QTimeEdit;
class QCalendarWidget;
class StationsListModel;
class QCompleter;
class QAbstractItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void mainWindowCreated();
    void userChanged();

private:
    void resizeEvent(QResizeEvent * event) override;

    void defineMainMenu();
    void defineToolBar();
    void defineStatusBar();

    void defineAuthorizationForm();
    void defineLoginForm();
    void defineMap();
    void defineStationsList();

    void defineUsersListForm();
    void defineStationsListForm();
    void defineTrainsListForm();
    void defineToursListForm();
    void defineFindTourDialog();
    void defineAboutProgramForm();

    void defineEditUserForm();
    void defineEditStationForm();
    void defineEditTrainForm();
    void defineEditTourForm();
    void defineTrainScheduleForm();
    void defineStationScheduleForm();

    void defineEditDateTimeForm();
    QAbstractItemModel* modelFromStationList();

    void updateStationListEditable(bool status);
    void updateTrainListEditable(bool status);
    void updateTourListEditable(bool status);

private slots:
    void connectToDatabase();
    void loadFromFile();
    void saveToFile();

    void btnEnterLoginOkClicked();
    void btnRegistrationOkClicked();
    void btnAuthLoginClicked();
    void btnAuthLogoutClicked();

    void changeMode(MapScene::MapMode mode);

    void openUsersListForm();
    void openEditProfileDialog();
    void openStationsListForm();
    void openTrainsListForm();
    void openToursListForm();
    void openTrainScheduleForm();
    void openStationScheduleForm();
    void openFindTourDialog();
    void showAboutProgramInfo();

    void changeUserGroupShowMode(int mode);
    void openEditUserForm(bool is_add_mode);
    void openUserEditForm();
    void openUserAddForm();
    void addUser();
    void acceptUserChanges();
    void clearUsersList();
    void removeUserFromList();

    void openStationAddForm();
    void openStationEditForm();
    void openEditStationForm(bool is_add_mode);
    void addStation();
    void acceptStationChanges();
    void clearStationsList();
    void removeStationFromList();

    void openTrainAddForm();
    void openTrainEditForm();
    void openEditTrainForm(bool is_add_mode);
    void addTrain();
    void acceptTrainChanges();
    void clearTrainsList();
    void removeTrainFromList();

    void openTourAddForm();
    void openTourEditForm();
    void openEditTourForm(bool is_add_mode);
    void addTour();
    void acceptTourChanges();
    void clearToursList();
    void removeTourFromList();

    void openEditDateTimeForm(bool is_arrival);
    void acceptDepartureDateTimeChanges();
    void acceptArrivalDateTimeChanges();
    void findTour();
    void updateUIbyUserGroup();
    void makeStep();

private:
    QString m_userDBPath;
    QString m_railwayNetDBPath;
    QPushButton* btnMakeStep;
    UserInfo m_currentUser;
    RailwayNetDBShell m_railwayNetDBManager;
    UserDBShell m_userDBManager;

    QDialog* dlgEnterLogin;
    QDialog* dlgUsersList;
    QDialog* dlgStationsList;
    QDialog* dlgTrainsList;
    QDialog* dlgToursList;
    QDialog* dlgFindTour;
    QDialog* dlgAboutProgram;

    QDialog* dlgTrainSchedule;
    QDialog* dlgStationSchedule;
    QDialog* dlgEditUser;
    QDialog* dlgEditStation;
    QDialog* dlgEditTrain;
    QDialog* dlgEditTour;
    QDialog* dlgEditDateTime;

    QTableWidget* tableUsers;
    QTableWidget* tableStations;
    QTableWidget* tableTrains;
    QTableWidget* tableTours;
    QTableWidget* tableTrainSchedule;
    QTableWidget* tableStationSchedule;
    QTableWidget* tableFoundTours;

    QLineEdit* txtUserLogin;
    QLineEdit* txtUserPassword;
    QComboBox* cmbUserGroup;
    QPushButton* btnAcceptUserChanges;
    QPushButton* btnAddUser;

    QPushButton* btnStationsListAdd;
    QPushButton* btnStationsListEdit;
    QPushButton* btnStationsListRemove;
    QPushButton* btnStationsListClear;
    QLineEdit* txtEditedStationName;
    QLineEdit* txtEditedStationX;
    QLineEdit* txtEditedStationY;
    QPushButton* btnAcceptStationChanges;
    QPushButton* btnAddStation;

    QPushButton* btnTrainsListAdd;
    QPushButton* btnTrainsListEdit;
    QPushButton* btnTrainsListRemove;
    QPushButton* btnTrainsListClear;
    QLineEdit* txtEditedTrainNumber;
    QLineEdit* txtEditedTrainRoute;
    QPushButton* btnAddTrain;
    QPushButton* btnAcceptTrainChanges;

    QPushButton* btnToursListAdd;
    QPushButton* btnToursListEdit;
    QPushButton* btnToursListRemove;
    QPushButton* btnToursListClear;
    QLineEdit* txtEditedTourName;
    QTableWidget* tableEditedTourStations;
    QPushButton* btnAddTour;
    QPushButton* btnAcceptTourChanges;

    QLineEdit* txtDeparturePlace;
    QLineEdit* txtArrivalPlace;
    QCompleter* m_completer;
    QDateTimeEdit* dateDeparture;
    QDateTimeEdit* dateArrival;
    QComboBox* cmbUserShowMode;

    QPushButton* btnAcceptDepartureTime;
    QPushButton* btnAcceptArrivalTime;
    QTimeEdit* m_timeEdit;
    QCalendarWidget* m_calendar;

    QLineEdit* txtSelectedStationName;
    QLineEdit* txtSelectedTrainNumber;
    QWidget* m_authForm;
    MapScene* m_mapScene;
    QGraphicsView* m_mapView;
    std::unique_ptr<StationsListModel> m_stationsListModel;
    QTreeView* m_stationsListView;
    // main window menu
    QMenuBar* m_mainMenu;
    QMenu* mnFile;
    QAction* actLoad;
    QAction* actSave;
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
	QAction* actSetAddTrainMapMode;
    QAction* actSetAddAccessTokenMapMode;
    QAction* actSetDeleteMapMode;
    QAction* actSetDeleteTrainMapMode;
    QAction* actStationsList;
    QAction* actTrainsList;
    QAction* actToursList;

    QAction* actFindTour;
    QAction* actAbout;

    // ToolBar
    QToolBar* m_toolBar;
    QAction* actSetViewMode;
    QAction* actSetMoveMode;
    QAction* actSetAddStateMode;
    QAction* actSetAddLinkMode;
	QAction* actSetAddTrainMode;
    QAction* actSetAddAccessTokenMode;
    QAction* actSetDeleteMode;
    QAction* actSetDeleteTrainMode;

    // StatusBar
    QStatusBar* m_statusBar;
    QLabel* lbCurrentOperation;

    // Authorization Form
    QWidget* frameAuthorization;
    QLabel* lbCurrentUserGroup;
    QLabel* lbCurrentUserLogin;
    QPushButton* btnAuthLogin;
    QPushButton* btnAuthLogout;

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
