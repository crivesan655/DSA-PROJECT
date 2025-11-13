#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>

// Modal dialog that collects a user's name and age.
class QLineEdit;
class QSpinBox;
class QPushButton;

class UserDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserDialog(QWidget *parent = nullptr);

private slots:
    // Called when the OK button is pressed; validates and saves the user.
    void onAccept();

private:
    QLineEdit *m_nameEdit;   // input for name
    QSpinBox *m_ageSpin;     // input for age
    QPushButton *m_okBtn;    // OK button
    QPushButton *m_cancelBtn;// Cancel button
};

#endif // USERDIALOG_H