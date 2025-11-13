#include "GUI/UserProfileWindow.h"
#include "User_Files/UserStorage.h"
#include "User_Files/User.h"
#include "GUI/WordDetailWindow.h" 
#include "Word_Files/Word_Storage.h" 
#include "Qt_includes.h"


// --- CalendarAndNotesWidget Implementation ---
// Constructor: Initializes the calendar and notes widget layout and connections.
CalendarAndNotesWidget::CalendarAndNotesWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 1. Calendar Button and Calendar Widget.
    m_calendarButton = new QPushButton(tr("Open Calendar"));
    
    m_calendarButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)); 
    m_calendarButton->setIconSize(QSize(24, 24));
    m_calendarButton->setStyleSheet("border: none; background-color: #f0f2f5; color: #4CAF50; font-weight: bold; text-align: left; padding: 5px;");

    m_calendar = new QCalendarWidget(this);
    m_calendar->setMaximumHeight(200); 

    // Styling for QSpinBox/text-field to ensure month/year visibility.
    m_calendar->setMinimumSize(250, 180); 
    // Calendar styles tuned to the app theme:
    // - navigation bar uses app green (#4CAF50)
    // - nav text (month/year) and controls are white for contrast
    // - day numbers use a dark color for readability
    // - selection highlight uses the app green with white text
    m_calendar->setStyleSheet(
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "    background-color: #4CAF50;"
        "    border-top-left-radius: 5px;"
        "    border-top-right-radius: 5px;"
        "    height: 30px;"
        "    color: white;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar QToolButton, QCalendarWidget QWidget#qt_calendar_navigationbar QSpinBox, QCalendarWidget QWidget#qt_calendar_navigationbar QSpinBox * {"
        "    color: white;"
        "    background-color: transparent;"
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: rgba(56,142,60,0.85);" /* subtle darker green */
        "}"
        "QCalendarWidget QSpinBox::text-field {"
        "    color: white;"
        "    background-color: transparent;"
        "    border: none;"
        "    margin: 0px;"
        "    padding: 0px;"
        "}"
        "QCalendarWidget QSpinBox::up-button, QCalendarWidget QSpinBox::down-button {"
        "    subcontrol-origin: border;"
        "}"
        "QCalendarWidget QMenu {"
        "    background-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView {"
        "    color: #333333;" 
        "    background-color: transparent;"
        "    selection-background-color: #4CAF50;" 
        "    selection-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView::item:hover {"
        "    background-color: rgba(76,175,80,0.12);" 
        "}"
        "QCalendarWidget QAbstractItemView::item:selected {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "}"
        "QCalendarWidget QTableView {"
        "    gridline-color: rgba(0,0,0,0.04);"
        "}"
    );
    
    m_calendar->hide(); 

    // Container for the calendar and its button
    QVBoxLayout *calendarLayout = new QVBoxLayout();
    calendarLayout->addWidget(m_calendarButton);
    calendarLayout->addWidget(m_calendar);
    calendarLayout->setContentsMargins(0, 0, 0, 0); 
    
    mainLayout->addLayout(calendarLayout); // Add the calendar container
    
    // 2. Notes Section. 
    QLabel *notesTitle = new QLabel(tr("Notes:")); 
    notesTitle->setStyleSheet("font-weight: bold; margin-top: 10px; color: #4CAF50;");
    
    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setPlaceholderText(tr("Type notes here..."));
    m_notesEdit->setFixedHeight(150); 
    
    m_notesSaveButton = new QPushButton(tr("Save Notes"));
    m_notesSaveButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px; padding: 5px;");

    mainLayout->addWidget(notesTitle);
    mainLayout->addWidget(m_notesEdit);
    mainLayout->addWidget(m_notesSaveButton);
    mainLayout->addStretch();

    connect(m_calendarButton, &QPushButton::clicked, this, &CalendarAndNotesWidget::on_calendarButton_clicked);
    connect(m_notesSaveButton, &QPushButton::clicked, this, &CalendarAndNotesWidget::on_notesSaveButton_clicked);

    loadNotes();
}

//  Toggles the visibility of the calendar widget.
void CalendarAndNotesWidget::on_calendarButton_clicked() {
    m_calendar->setVisible(!m_calendar->isVisible());
    m_calendarButton->setText(m_calendar->isVisible() ? tr("Close Calendar") : tr("Open Calendar"));
}

// Slot: Saves the notes content to user storage.
void CalendarAndNotesWidget::on_notesSaveButton_clicked() {
    saveNotes();
    QMessageBox::information(this, tr("Notes Saved"), tr("Notes content has been saved."));
}

// Loads the stored notes content for the current user into the text editor.
void CalendarAndNotesWidget::loadNotes() {
    QString username = UserStorage::instance().currentUser();
    if (username.isEmpty()) return;

    QJsonObject data = UserStorage::instance().currentUserData();
    QString notes = data.value("word_notes").toString();
    m_notesEdit->setText(notes);
}

// Saves the current text editor content back to the user's data storage.
void CalendarAndNotesWidget::saveNotes() {
    QString username = UserStorage::instance().currentUser();
    if (username.isEmpty()) return;

    QJsonObject data = UserStorage::instance().currentUserData();
    data["word_notes"] = m_notesEdit->toPlainText();
    UserStorage::instance().updateCurrentUserData(data);
    
    UserStorage::instance().saveCurrentUserData();
}


// --- UserProfileWindow Implementation ---
// Constructor: Sets window properties and calls setupUI.
UserProfileWindow::UserProfileWindow(const User &user, QWidget *parent) 
    : QDialog(parent) 
{
    setWindowTitle(tr("Profile: %1").arg(user.name));
    setMinimumSize(600, 450);
    setStyleSheet("background-color: #ffffff;"); 

    setupUI(user);
}

// Sets up the main layout and widgets for the profile window.
void UserProfileWindow::setupUI(const User &user) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // --- Left Pane: User Details ---
    QWidget *detailsPane = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout(detailsPane);
    vLayout->setAlignment(Qt::AlignTop);

    QLabel *title = new QLabel(tr("User Details"));
    QFont titleFont = title->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #4CAF50; margin-bottom: 15px;");
    vLayout->addWidget(title);

    m_nameLabel = new QLabel(tr("Name: <b>%1</b>").arg(user.name));
    m_ageLabel = new QLabel(tr("Age: <b>%1</b>").arg(user.age));
    vLayout->addWidget(m_nameLabel);
    vLayout->addWidget(m_ageLabel);
    vLayout->addSpacing(20);

    // Added Words section.
    m_addedWordsLabel = new QLabel(tr("<b>Words Added (%1):</b>").arg(user.addedWords.size()));
    
    // Use QListWidget for interactable words.
    m_addedWordsList = new QListWidget();
    m_addedWordsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_addedWordsList->setFixedHeight(100);
    m_addedWordsList->setStyleSheet("border: 1px solid #d0d0d0; border-radius: 5px;");
    
    if (user.addedWords.isEmpty()) {
        QListWidgetItem *placeholderItem = new QListWidgetItem(tr("None recorded."), m_addedWordsList);
        placeholderItem->setFlags(placeholderItem->flags() & ~Qt::ItemIsSelectable); // Disable selection
    } else {
        // Add words to the list, ensuring uniqueness.
        QStringList uniqueWords = user.addedWords;
        uniqueWords.removeDuplicates();
        m_addedWordsList->addItems(uniqueWords);
    }

    // Connect list click signal to the slot.
    connect(m_addedWordsList, &QListWidget::itemClicked, this, &UserProfileWindow::on_addedWordClicked);


    // Recent Searches section.
    QLabel *recentSearchesTitle = new QLabel(tr("<b>Recent Searches (%1):</b>").arg(user.recentSearches.size()));
    m_recentSearchesText = new QTextEdit();
    m_recentSearchesText->setReadOnly(true);
    m_recentSearchesText->setFixedHeight(120);
    m_recentSearchesText->setStyleSheet("background-color: #f8f8f8; border: 1px solid #d0d0d0; border-radius: 5px;");
    
    QString searchesText = user.recentSearches.isEmpty() ? tr("No recent searches recorded.") : user.recentSearches.join("\n");
    m_recentSearchesText->setText(searchesText);

    vLayout->addWidget(m_addedWordsLabel);
    vLayout->addWidget(m_addedWordsList); 
    vLayout->addSpacing(15);
    vLayout->addWidget(recentSearchesTitle);
    vLayout->addWidget(m_recentSearchesText);
    vLayout->addStretch(1); 
    
    detailsPane->setStyleSheet("padding-right: 20px; border-right: 1px solid #e0e0e0;");


    // --- Right Pane: Calendar and Notes ---
    CalendarAndNotesWidget *sidebar = new CalendarAndNotesWidget();
    sidebar->setFixedWidth(250); 

    // Add both panes to the main layout.
    mainLayout->addWidget(detailsPane, 2); 
    mainLayout->addWidget(sidebar, 1); 
}

// Slot: Handles clicks on words in the list to open the detail window.
void UserProfileWindow::on_addedWordClicked(QListWidgetItem *item) {
    QString wordKey = item->text();

    // Prevent clicking the placeholder item.
    if (wordKey == tr("None recorded.")) return; 

    // Look up the word details in WordStorage.
    auto words = WordStorage::instance().allWords();
    for (const auto &e : words) {
        if (e.word.compare(wordKey, Qt::CaseInsensitive) == 0) {
            // Found the word, open the detail window using the WordEntry struct.
            WordDetailWindow detailDlg(e, this); 
            detailDlg.exec();
            return;
        }
    }
    QMessageBox::warning(this, tr("Error"), tr("Word details not found in storage."));
}