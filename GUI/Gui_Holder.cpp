#include "Qt_includes.h" 
#include "GUI/Gui_Holder.h"
#include "GUI/UserProfileWindow.h" 
#include "GUI/AboutWindow.h" 
#include "GUI/WordDetailWindow.h" 
#include "Function_Files/Function.h" 
#include "User_Files/UserStorage.h" 
#include "Word_Files/Word_Storage.h" 
#include "User_Files/User.h" 
#include <QListWidget>


// Helper function to extract initials from a full name.
static QString initialsFromName(const QString &name) {
    QStringList parts = name.simplified().split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return QString();
    QString out;
    out.append(parts.value(0).left(1).toUpper());
    if (parts.size() > 1) out.append(parts.value(1).left(1).toUpper());
    return out;
}

// Constructor: Sets up the main window UI and initializes the function pointer.
Gui_Holder::Gui_Holder(QWidget *parent)
    : QMainWindow(parent),
      m_appFunction(new Function) // Initialize the Function pointer
{
    setWindowTitle("DeepLingo"); 
    setMinimumSize(700, 520);

    // Global UI styling (Green Theme).
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f2f5; 
            font-family: 'Segoe UI', sans-serif; 
        }
        QLabel {
            color: #333333; 
        }
        QLineEdit, QTextEdit, QComboBox {
            border: 1px solid #d0d0d0;
            border-radius: 8px; 
            padding: 8px 10px;
            background-color: #ffffff;
            selection-background-color: #a8e6cf; 
        }
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
            border: 1px solid #4CAF50; 
            outline: none;
        }
        QPushButton {
            background-color: #4CAF50; 
            color: white;
            border: none;
            border-radius: 8px; 
            padding: 10px 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #45a049; 
        }
        QPushButton:pressed {
            background-color: #3e8e41; 
        }
        QTabWidget::pane { 
            border: 1px solid #d0d0d0;
            border-radius: 8px; 
            background-color: #ffffff;
            margin-top: -1px; 
        }
        QTabBar::tab {
            background: #e0e0e0; 
            color: #555555;
            padding: 10px 15px;
            border: 1px solid #d0d0d0;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background: #ffffff; 
            color: #4CAF50; 
            border-bottom-color: #ffffff; 
        }
        QTabBar::tab:hover {
            background: #f0f0f0; 
        }
    )" );


    setupUI();

    // Ensure words are loaded from storage.
    WordStorage::instance().load();

    // Update the profile button's appearance.
    updateProfileView();
}

// Destructor.
Gui_Holder::~Gui_Holder() {
    delete m_appFunction; // Delete the Function object
}

// Initializes all widgets and layouts.
void Gui_Holder::setupUI()
{
    QWidget *cent = new QWidget(this);
    QVBoxLayout *mainLay = new QVBoxLayout(cent);
    mainLay->setContentsMargins(15, 15, 15, 15); 

    // Top Bar: Logo and Button Container
    QWidget *topBar = new QWidget(cent);
    QHBoxLayout *topLay = new QHBoxLayout(topBar);
    topLay->setContentsMargins(0, 0, 0, 0);
    
    
    // Invisible spacer/stretch on the left to push the logo right
    topLay->addStretch(); 
    
    // Application title / Logo 
    m_logoLabel = new QLabel(cent);
    if (m_logoImage.load("DeepLingo_Logo.png")) { 
        m_logoLabel->setPixmap(m_logoImage.scaled(650, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_logoLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Fallback to text if image not found
        m_logoLabel->setText("<h1><b style='color:#4CAF50;'>DeepLingo</b></h1>");
        m_logoLabel->setAlignment(Qt::AlignCenter);
        qWarning("Gui_Holder: Title logo 'DeepLingo_Logo.png' not found. Falling back to text.");
    }
    m_logoLabel->setMaximumWidth(650); 
    topLay->addWidget(m_logoLabel);
    
    // Invisible spacer/stretch in the middle to push the right buttons far right
    topLay->addStretch();
    
    // Vertical Container for Profile and Settings Buttons (Right-aligned)
    QWidget *rightButtonContainer = new QWidget(topBar);
    QVBoxLayout *rightButtonLay = new QVBoxLayout(rightButtonContainer);
    rightButtonLay->setContentsMargins(0, 0, 0, 0);
    rightButtonLay->setSpacing(5); 

    // Profile Button (Top Right)
    profileButton = new QPushButton(rightButtonContainer);
    profileButton->setFixedSize(40, 40); 
    profileButton->setIcon(this->style()->standardIcon(QStyle::SP_MessageBoxInformation)); 
    profileButton->setIconSize(QSize(24, 24)); 
    profileButton->setText(""); 
    profileButton->setToolTip(tr("Profile"));
    // Styling for the profile button.
    profileButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            border-radius: 20px; 
            padding: 0px; 
        }
        QPushButton:hover { background-color: #45a049; }
        QPushButton:pressed { background-color: #3e8e41; }
    )");
    rightButtonLay->addWidget(profileButton);
    connect(profileButton, &QPushButton::clicked, this, &Gui_Holder::on_profileButton_clicked);

    // Settings Button (Below Profile)
    settingsButton = new QPushButton(rightButtonContainer);
    settingsButton->setFixedSize(40, 40); 
    settingsButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    settingsButton->setIconSize(QSize(24, 24));
    settingsButton->setText("");
    settingsButton->setToolTip(tr("Settings / About"));
    // Styling for the settings button (same style).
    settingsButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4CAF50;
            border-radius: 20px; 
            padding: 0px; 
        }
        QPushButton:hover { background-color: #45a049; }
        QPushButton:pressed { background-color: #3e8e41; }
    )");
    rightButtonLay->addWidget(settingsButton);
    connect(settingsButton, &QPushButton::clicked, this, &Gui_Holder::on_settingsButton_clicked);
    
    // Add stretch to push buttons to the top of the container
    rightButtonLay->addStretch();
    
    // Add the button container to the main top bar layout
    topLay->addWidget(rightButtonContainer);
    
    mainLay->addWidget(topBar);
    mainLay->addSpacing(10); 
    
    // QTabWidget setup
    QTabWidget *tabs = new QTabWidget(cent);

    // Setup the "Add Word" tab.
    QWidget *addTab = new QWidget;
    QVBoxLayout *addLay = new QVBoxLayout(addTab);
    addLay->setContentsMargins(15, 15, 15, 15); 

    addLay->addWidget(new QLabel(tr("Word:"), addTab));
    wordInputAdd = new QLineEdit(addTab);
    addLay->addWidget(wordInputAdd);

    addLay->addWidget(new QLabel(tr("Definition:"), addTab));
    definitionInputAdd = new QTextEdit(addTab);
    definitionInputAdd->setMinimumHeight(80); 
    addLay->addWidget(definitionInputAdd);

    addLay->addWidget(new QLabel(tr("Synonyms (comma separated):"), addTab));
    synonymsInput = new QLineEdit(addTab);
    addLay->addWidget(synonymsInput);

    addLay->addWidget(new QLabel(tr("Antonyms (comma separated):"), addTab));
    antonymsInput = new QLineEdit(addTab); 
    addLay->addWidget(antonymsInput);

    addLay->addWidget(new QLabel(tr("Background / Etymology:"), addTab));
    backgroundInput = new QTextEdit(addTab);
    backgroundInput->setMinimumHeight(80);
    addLay->addWidget(backgroundInput);

    addLay->addWidget(new QLabel(tr("Usage / Example:"), addTab));
    usageInput = new QTextEdit(addTab);
    usageInput->setMinimumHeight(60);
    addLay->addWidget(usageInput);
    addLay->addStretch(); 

    addWordButton = new QPushButton(tr("Add Word"), addTab);
    connect(addWordButton, &QPushButton::clicked, this, &Gui_Holder::on_addWordButton_clicked);
    addLay->addWidget(addWordButton);


    tabs->addTab(addTab, tr("Add Word"));

    // Setup the "Search" tab.
    QWidget *searchTab = new QWidget;
    QVBoxLayout *sLay = new QVBoxLayout(searchTab);
    sLay->setContentsMargins(15, 15, 15, 15); 
    sLay->addWidget(new QLabel(tr("Search Word:"), searchTab));
    wordInputSearch = new QLineEdit(searchTab);
    sLay->addWidget(wordInputSearch);
    searchWordButton = new QPushButton(tr("Search"), searchTab);
    sLay->addWidget(searchWordButton);
    resultOutputSearch = new QTextEdit(searchTab);
    resultOutputSearch->setReadOnly(true);
    resultOutputSearch->setStyleSheet("background-color: #f8f8f8;"); 
    resultOutputSearch->setMinimumHeight(250);
    sLay->addWidget(resultOutputSearch);
    sLay->addStretch();
    connect(searchWordButton, &QPushButton::clicked, this, &Gui_Holder::on_searchWordButton_clicked);
    tabs->addTab(searchTab, tr("Search"));

    // Setup the "Browse" tab.
    QWidget *browseTab = new QWidget;
    QVBoxLayout *bLay = new QVBoxLayout(browseTab);
    bLay->setContentsMargins(15, 15, 15, 15); 

    letterComboBox = new QComboBox(browseTab);
    // Populate combo box with letters A-Z.
    for (char c='A'; c<='Z'; ++c) letterComboBox->addItem(QString(c));
    bLay->addWidget(letterComboBox);
    browseOutput = new QListWidget(browseTab);
    browseOutput->setStyleSheet("background-color: #f8f8f8;"); 
    browseOutput->setMinimumHeight(300);
    browseOutput->setSelectionMode(QAbstractItemView::SingleSelection);
    bLay->addWidget(browseOutput);
    bLay->addStretch();
    // Connect combo box signal to slot to update list.
    connect(letterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Gui_Holder::on_letterComboBox_currentIndexChanged);
    
    // Connect list item click to open detail window
    connect(browseOutput, &QListWidget::itemClicked, this, &Gui_Holder::on_browseItem_clicked);

    // Trigger the initial display of words starting with 'A'
    on_letterComboBox_currentIndexChanged(0);

    tabs->addTab(browseTab, tr("Browse"));

    mainLay->addWidget(tabs);

    setCentralWidget(cent);

    // Set initial avatar text.
    updateProfileAvatar();
}

// Refreshes the profile view.
void Gui_Holder::updateProfileView()
{
    updateProfileAvatar();
}

// Updates avatar appearance based on current user.
void Gui_Holder::updateProfileAvatar()
{
    QString user = UserStorage::instance().currentUser();
    if (user.isEmpty()) {
        profileButton->setToolTip(tr("No user selected"));
        // Icon for 'no user/add user'.
        profileButton->setIcon(this->style()->standardIcon(QStyle::SP_MessageBoxQuestion)); 
    } else {
        profileButton->setToolTip(user);
        // Icon for 'user profile'.
        profileButton->setIcon(this->style()->standardIcon(QStyle::SP_MessageBoxInformation)); 
    }
}

// Handles the Settings/About button click event.
void Gui_Holder::on_settingsButton_clicked()
{
    // Launch the AboutWindow modally.
    AboutWindow aboutDlg(this);
    aboutDlg.exec();
}
// -----------------------------------------------------------------

// Handles the Add Word button click event: processes input and saves to storage.
void Gui_Holder::on_addWordButton_clicked()
{
    QString w = wordInputAdd->text().trimmed();
    QString def = (static_cast<QTextEdit*>(definitionInputAdd))->toPlainText().trimmed();
    QString syn = synonymsInput->text().trimmed();
    QString ant = antonymsInput->text().trimmed();
    QString bg = backgroundInput->toPlainText().trimmed();
    QString usage = usageInput->toPlainText().trimmed();

    if (w.isEmpty()) {
        QMessageBox::information(this, tr("Missing"), tr("Please enter a word."));
        return;
    }

    bool ok = m_appFunction->addWordFromInputs(w, def, QString(), syn, ant, bg, usage); 
    if (ok) {
        QMessageBox::information(this, tr("Added"), tr("Word added successfully."));
        
        // Clear input fields after successful addition.
        wordInputAdd->clear();
        definitionInputAdd->clear();
        synonymsInput->clear();
        antonymsInput->clear();
        backgroundInput->clear();
        usageInput->clear();
    } else {
        QMessageBox::warning(this, tr("Failed"), tr("Could not add word (maybe duplicate)."));
    }
}

// Handles the Search Definition button click event: retrieves and displays word details.
void Gui_Holder::on_searchWordButton_clicked()
{
    QString key = wordInputSearch->text().trimmed();
    if (key.isEmpty()) return;
    
    // Search for the word in storage.
    auto words = WordStorage::instance().allWords();
    for (const auto &e : words) {
        if (e.word.compare(key, Qt::CaseInsensitive) == 0) {
            QString out;
            out += "Word: " + e.word + "\n\n";
            out += "Definition: " + e.definition + "\n\n";
            out += "Synonyms: " + e.synonyms.join(", ") + "\n";
            out += "Antonyms: " + e.antonyms.join(", ") + "\n\n";
            out += "Background: " + e.background + "\n\n";
            out += "Usage: " + e.usage + "\n";
            resultOutputSearch->setPlainText(out);
            return;
        }
    }
    resultOutputSearch->setPlainText(tr("Not found"));
}

// Updates the browse list when a new letter is selected in the combo box.
void Gui_Holder::on_letterComboBox_currentIndexChanged(int index)
{
    if (index < 0) return;
    QChar letter = letterComboBox->itemText(index).at(0);
    // Get all words starting with that letter.
    auto list = WordStorage::instance().wordsForLetter(letter);
    browseOutput->clear();
    for (const auto &e : list) {
        QListWidgetItem *it = new QListWidgetItem(e.word + " - " + e.definition, browseOutput);
        it->setData(Qt::UserRole, e.word);
        browseOutput->addItem(it);
    }
}

// Slot: open WordDetailWindow when a browse item is clicked
void Gui_Holder::on_browseItem_clicked(QListWidgetItem *item)
{
    if (!item) return;
    QString key = item->data(Qt::UserRole).toString();
    if (key.isEmpty()) return;

    // Find the full WordEntry for the selected word
    auto all = WordStorage::instance().allWords();
    for (const auto &e : all) {
        if (e.word.compare(key, Qt::CaseInsensitive) == 0) {
            WordDetailWindow dlg(e, this);
            dlg.exec();
            return;
        }
    }
}

// Displays the current user's profile details in a new modal window.
void Gui_Holder::on_profileButton_clicked()
{
    QString username = UserStorage::instance().currentUser();
    if (username.isEmpty()) {
        QMessageBox::information(this, tr("Profile"), tr("No user is currently selected. Please add a user first."));
        return;
    }

    // Force the UserStorage cache to reload data from the user's file. 
    UserStorage::instance().loadUserData(username);
    
    // Retrieve the detailed user data and deserialize it into a User struct.
    QJsonObject data = UserStorage::instance().currentUserData();
    User u = User::fromJson(data);

    // Launch the new modal profile window.
    UserProfileWindow profileDlg(u, this);
    profileDlg.exec();
}

// Handles closing event: confirms exit and saves data.
void Gui_Holder::closeEvent(QCloseEvent *event)
{
    auto res = QMessageBox::question(this,
                                      tr("Confirm exit"),
                                      tr("Are you sure you want to exit the application?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);
    if (res == QMessageBox::Yes) {
        // Ensure words are persisted to file storage.
        WordStorage::instance().save();
        event->accept();
    } else {
        event->ignore();
    }
}