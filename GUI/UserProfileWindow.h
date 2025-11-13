#ifndef USERPROFILEWINDOW_H
#define USERPROFILEWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget> 
#include <QListWidgetItem> 
#include <QHBoxLayout>
#include <QVBoxLayout>

// Forward declarations
class QCalendarWidget; 
class QSplitter;
struct User; 

// Widget containing the Calendar and Notes sections for the sidebar.
class CalendarAndNotesWidget : public QWidget {
    Q_OBJECT
public:
    CalendarAndNotesWidget(QWidget *parent = nullptr);

private slots:
    void on_calendarButton_clicked(); 
    void on_notesSaveButton_clicked(); 

private:
    QPushButton *m_calendarButton;
    QCalendarWidget *m_calendar;
    QTextEdit *m_notesEdit;
    QPushButton *m_notesSaveButton;
    
    // Loads notes from the user data storage.
    void loadNotes();
    // Saves the current text editor content to user data storage.
    void saveNotes();
};


// Main dialog window for displaying the User Profile details.
class UserProfileWindow : public QDialog {
    Q_OBJECT
public:
    UserProfileWindow(const User &user, QWidget *parent = nullptr);

private slots:
    // Slot to handle clicks on added words in the list.
    void on_addedWordClicked(QListWidgetItem *item); 

private:
    QLabel *m_nameLabel;
    QLabel *m_ageLabel;
    QLabel *m_addedWordsLabel;
    QListWidget *m_addedWordsList; 
    QTextEdit *m_recentSearchesText; 
    
    // Sets up the user interface elements based on the provided User struct.
    void setupUI(const User &user);
};

#endif // USERPROFILEWINDOW_H