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
#include <memory>
#include "StationsList/StationsListModel.h"
#include "Map/MapScene.h"

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

private:
    QDialog* dlgEnterLogin;
    QDialog* dlgRegistration;
    QWidget* m_authForm;

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

    // ToolBar
    QToolBar* m_toolBar;
    QAction* actSetViewMode;
    QAction* actSetMoveMode;
    QAction* actSetAddStateMode;
    QAction* actSetAddTransitionMode;
    QAction* actSetAddTraceMode;
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
