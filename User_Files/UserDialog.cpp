#include "User_Files/UserDialog.h"
#include "User_Files/UserStorage.h"
#include "User_Files/User.h" 
#include "Qt_includes.h"

// Construct the dialog UI (name + age + OK/Cancel).
UserDialog::UserDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Enter User Info");
    setModal(true);

    m_nameEdit = new QLineEdit(this);
    m_ageSpin = new QSpinBox(this);
    m_ageSpin->setRange(0, 150); // reasonable age range

    m_okBtn = new QPushButton("OK", this);
    m_cancelBtn = new QPushButton("Cancel", this);

    QHBoxLayout *nameRow = new QHBoxLayout();
    nameRow->addWidget(new QLabel("Name:"));
    nameRow->addWidget(m_nameEdit);

    QHBoxLayout *ageRow = new QHBoxLayout();
    ageRow->addWidget(new QLabel("Age:"));
    ageRow->addWidget(m_ageSpin);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addStretch();
    buttons->addWidget(m_okBtn);
    buttons->addWidget(m_cancelBtn);

    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(nameRow);
    main->addLayout(ageRow);
    main->addLayout(buttons);

    connect(m_okBtn, &QPushButton::clicked, this, &UserDialog::onAccept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &UserDialog::reject);
}

// Validate inputs, add user to storage, then close with accept().
void UserDialog::onAccept() {
    QString name = m_nameEdit->text().trimmed();
    int age = m_ageSpin->value();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Please enter a name.");
        return; // don't close
    }

    User u;
    u.name = name;
    u.age = age;

    UserStorage::instance().addUser(u); // store user
    
    // Set the newly created user as the current user.
    UserStorage::instance().setCurrentUser(name); 

    accept();
}