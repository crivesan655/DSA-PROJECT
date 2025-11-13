#include "GUI/WordDetailWindow.h"
#include "Qt_includes.h"

// Constructor: Initializes the dialog window and sets up the UI.
WordDetailWindow::WordDetailWindow(const WordEntry &wordData, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Word Details: %1").arg(wordData.word));
    setMinimumSize(450, 600);
    setStyleSheet("background-color: #f8f8f8;");

    setupUI(wordData);
}

// Sets up the UI elements based on the provided word data.
void WordDetailWindow::setupUI(const WordEntry &wordData)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame); 

    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);
    contentLayout->setAlignment(Qt::AlignTop);

    // Word Title
    wordTitle = new QLabel(wordData.word);
    QFont titleFont = wordTitle->font();
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    wordTitle->setFont(titleFont);
    wordTitle->setStyleSheet("color: #4CAF50;");
    contentLayout->addWidget(wordTitle);

    // Definition (with small Tagalog toggle button on the right)
    QLabel *defLabel = new QLabel(tr("<b>Definition:</b>"));
    // Row for the label and the small toggle button
    QHBoxLayout *defRow = new QHBoxLayout();
    defRow->addWidget(defLabel);
    defRow->addStretch();

    toggleLangBtn = new QPushButton(this);
    toggleLangBtn->setToolTip(tr("Toggle Tagalog translation"));
    // Use embedded SVG icon for clearer UX; fall back to text if icon missing
    QIcon transIcon(QStringLiteral(":/icons/translate.svg"));
    if (!transIcon.isNull()) {
        toggleLangBtn->setIcon(transIcon);
        toggleLangBtn->setIconSize(QSize(18,18));
        toggleLangBtn->setText("");
    } else {
        toggleLangBtn->setText("TL");
    }
    // Make the button more visible: larger circular green button with white icon
    toggleLangBtn->setFixedSize(36, 36);
    toggleLangBtn->setFlat(true);
    toggleLangBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border-radius: 18px; }"
        "QPushButton:hover { background-color: #45a049; }"
    );
    defRow->addWidget(toggleLangBtn);
    contentLayout->addLayout(defRow);

    defText = new QTextEdit();
    defText->setPlainText(wordData.definition);
    defText->setReadOnly(true);
    defText->setFixedHeight(80);
    defText->setStyleSheet("background-color: white; border: 1px solid #d0d0d0; border-radius: 5px; padding: 5px;");
    contentLayout->addWidget(defText);

    // Synonyms
    QLabel *synLabel = new QLabel(tr("<b>Synonyms:</b>"));
    synText = new QLabel(wordData.synonyms.join(", "));
    synText->setWordWrap(true);
    contentLayout->addWidget(synLabel);
    contentLayout->addWidget(synText);

    // Antonyms
    QLabel *antLabel = new QLabel(tr("<b>Antonyms:</b>"));
    antText = new QLabel(wordData.antonyms.join(", "));
    antText->setWordWrap(true);
    contentLayout->addWidget(antLabel);
    contentLayout->addWidget(antText);

    // Background / Etymology
    QLabel *bgLabel = new QLabel(tr("<b>Background / Etymology:</b>"));
    bgText = new QTextEdit();
    bgText->setPlainText(wordData.background);
    bgText->setReadOnly(true);
    bgText->setFixedHeight(100);
    bgText->setStyleSheet("background-color: white; border: 1px solid #d0d0d0; border-radius: 5px; padding: 5px;");
    contentLayout->addWidget(bgLabel);
    contentLayout->addWidget(bgText);

    // Usage / Example
    QLabel *usageLabel = new QLabel(tr("<b>Usage / Example:</b>"));
    usageText = new QTextEdit();
    usageText->setPlainText(wordData.usage);
    usageText->setReadOnly(true);
    usageText->setFixedHeight(80);
    usageText->setStyleSheet("background-color: white; border: 1px solid #d0d0d0; border-radius: 5px; padding: 5px;");
    contentLayout->addWidget(usageLabel);
    contentLayout->addWidget(usageText);

    contentLayout->addStretch(1);
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    // Connect toggle button signal to switch between English and Tagalog
    // Prepare English originals so we can restore them when toggling back.
    m_engDef = wordData.definition;
    m_engSyn = wordData.synonyms.join(", ");
    m_engAnt = wordData.antonyms.join(", ");
    m_engBg = wordData.background;
    m_engUsage = wordData.usage;
    m_engWord = wordData.word;

    // Parse labeled Tagalog sections from the single `translation` field when possible.
    // Expected markers (in Tagalog data): "Kasingkahulugan:", "Kasalungat:", "Halimbawa:".
    auto parseTranslation = [](const QString &t) {
        QString defOut;
        QString synOut;
        QString antOut;
        QString bgOut;
        QString usageOut;

        if (t.isEmpty()) return std::tuple<QString,QString,QString,QString,QString>(defOut,synOut,antOut,bgOut,usageOut);

        // Split lines and look for markers
        QStringList lines = t.split('\n', Qt::SkipEmptyParts);
        QStringList pending;
        for (const QString &ln : lines) {
            QString L = ln.trimmed();
            if (L.startsWith("Kasingkahulugan", Qt::CaseInsensitive)) {
                // remove marker and collect
                QString v = L.section(':', 1).trimmed();
                synOut = v;
            } else if (L.startsWith("Kasalungat", Qt::CaseInsensitive)) {
                QString v = L.section(':', 1).trimmed();
                antOut = v;
            } else if (L.startsWith("Halimbawa", Qt::CaseInsensitive)) {
                QString v = L.section(':', 1).trimmed();
                usageOut = v;
            } else {
                pending.append(L);
            }
        }

        // If pending lines exist, treat the first as the Tagalog definition
        if (!pending.isEmpty()) {
            defOut = pending.takeFirst();
            // any remaining pending lines append to background if background empty
            if (!pending.isEmpty()) bgOut = pending.join("\n");
        }

        return std::tuple<QString,QString,QString,QString,QString>(defOut,synOut,antOut,bgOut,usageOut);
    };

    const QString tagall = wordData.translation;
    std::tie(m_tagDef, m_tagSyn, m_tagAnt, m_tagBg, m_tagUsage) = parseTranslation(tagall);
    // tagDef is typically the first pending line; use it as Tagalog title when present
    m_tagTitle = m_tagDef;

    connect(toggleLangBtn, &QPushButton::clicked, this, [this]() {
        static bool showingTagalog = false;
        if (showingTagalog) {
            defText->setPlainText(m_engDef);
            synText->setText(m_engSyn);
            antText->setText(m_engAnt);
            bgText->setPlainText(m_engBg);
            usageText->setPlainText(m_engUsage);
            // restore English title
            wordTitle->setText(m_engWord);
            setWindowTitle(tr("Word Details: %1").arg(m_engWord));
            toggleLangBtn->setToolTip(tr("Show Tagalog translation"));
            // If icon fallback text is used, show TL
            if (toggleLangBtn->text().isEmpty()) {
                // keep icon only
            } else {
                toggleLangBtn->setText("TL");
            }
        } else {
            // If there is no structured tagalog content, show message and return
            if (m_tagDef.isEmpty() && m_tagSyn.isEmpty() && m_tagAnt.isEmpty() && m_tagBg.isEmpty() && m_tagUsage.isEmpty()) {
                QMessageBox::information(this, tr("No translation"), tr("No Tagalog translation available for this word."));
                return;
            }
            // use Tagalog parts if present, otherwise keep English
            defText->setPlainText(!m_tagDef.isEmpty() ? m_tagDef : m_engDef);
            synText->setText(!m_tagSyn.isEmpty() ? m_tagSyn : m_engSyn);
            antText->setText(!m_tagAnt.isEmpty() ? m_tagAnt : m_engAnt);
            bgText->setPlainText(!m_tagBg.isEmpty() ? m_tagBg : m_engBg);
            usageText->setPlainText(!m_tagUsage.isEmpty() ? m_tagUsage : m_engUsage);
            // set Tagalog title if available
            if (!m_tagTitle.isEmpty()) {
                wordTitle->setText(m_tagTitle);
                setWindowTitle(tr("Word Details: %1").arg(m_tagTitle));
            }
            toggleLangBtn->setToolTip(tr("Show English definition"));
            if (toggleLangBtn->text().isEmpty()) {
                // keep icon only
            } else {
                toggleLangBtn->setText("EN");
            }
        }
        showingTagalog = !showingTagalog;
    });
}
