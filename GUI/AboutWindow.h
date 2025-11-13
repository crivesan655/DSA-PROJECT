#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor. Takes the parent widget for proper modal behavior.
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    QLabel *titleLabel;
    QTextEdit *contentDisplay;

    // Method to set up all widgets and layout.
    void setupUI();
    // Method to populate the editable content.
    void populateContent();
};

#endif // ABOUTWINDOW_H