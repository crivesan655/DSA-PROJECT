#ifndef WORDDETAILWINDOW_H
#define WORDDETAILWINDOW_H

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

// Forward declaration
struct WordEntry; 

// Dialog window to display the full details of a specific word entry.
class WordDetailWindow : public QDialog {
    Q_OBJECT
public:
    // Constructor: Takes a constant reference to a WordEntry struct.
    explicit WordDetailWindow(const WordEntry &wordData, QWidget *parent = nullptr);

private:
    // Sets up the UI elements based on the provided WordEntry data.
    void setupUI(const WordEntry &wordData);

    // Member variables for toggling translation
    // Section widgets that may be swapped to Tagalog equivalents
    QLabel *wordTitle; 
    QTextEdit *defText;
    QLabel *synText;
    QLabel *antText;
    QTextEdit *bgText;
    QTextEdit *usageText;
    QPushButton *toggleLangBtn;

    // Stored originals and parsed translations to avoid capturing local variables
    QString m_engWord;
    QString m_engDef;
    QString m_engSyn;
    QString m_engAnt;
    QString m_engBg;
    QString m_engUsage;

    QString m_tagDef;
    QString m_tagSyn;
    QString m_tagAnt;
    QString m_tagBg;
    QString m_tagUsage;
    QString m_tagTitle;
};

#endif // WORDDETAILWINDOW_H
