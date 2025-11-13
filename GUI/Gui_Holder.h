#ifndef GUI_HOLDER_H
#define GUI_HOLDER_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QTabWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QCloseEvent>

// Forward Declarations
class Function; 
class AboutWindow;

//The main window class for the DeepLingo application.
//Manages the layout, main tabs, and top-bar actions (Profile and Settings).
class Gui_Holder : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui_Holder(QWidget *parent = nullptr);
    ~Gui_Holder();

private slots:
    // Slots for main application buttons/interactions
    void on_profileButton_clicked();
    void on_settingsButton_clicked(); 
    void on_addWordButton_clicked();
    void on_searchWordButton_clicked();
    void on_letterComboBox_currentIndexChanged(int index);
    void on_browseItem_clicked(QListWidgetItem *item);

private:
    // UI Setup & Maintenance
    void setupUI();
    void updateProfileView();
    void updateProfileAvatar();
    void closeEvent(QCloseEvent *event) override;

    // Top Bar Widgets
    QLabel *m_logoLabel;
    QPixmap m_logoImage;
    QPushButton *profileButton;
    QPushButton *settingsButton; 

    // Add Word Tab Widgets
    QLineEdit *wordInputAdd;
    QTextEdit *definitionInputAdd;
    QLineEdit *synonymsInput;
    QLineEdit *antonymsInput;
    QTextEdit *backgroundInput;
    QTextEdit *usageInput;
    QPushButton *addWordButton;

    // Search Tab Widgets
    QLineEdit *wordInputSearch;
    QPushButton *searchWordButton;
    QTextEdit *resultOutputSearch;

    // Browse Tab Widgets
    QComboBox *letterComboBox;
    QListWidget *browseOutput;

    // Application Functionality Holder
    Function *m_appFunction; 
};

#endif // GUI_HOLDER_H