#include "GUI/AboutWindow.h"
#include "Qt_includes.h"

// Constructor initializes the UI.
AboutWindow::AboutWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About DeepLingo"));
    setMinimumSize(450, 450);
    setModal(true); 

    // Apply the same green styling for consistency
    setStyleSheet(R"(
        QDialog {
            background-color: #f0f2f5;
        }
        QLabel {
            color: #4CAF50; /* Green title */
            font-size: 24px;
            font-weight: bold;
        }
        QTextEdit {
            border: 1px solid #d0d0d0;
            border-radius: 8px;
            padding: 10px;
            background-color: #ffffff;
        }
    )");

    setupUI();
    populateContent();
}

AboutWindow::~AboutWindow() {}

// Sets up the visual structure (labels, layout).
void AboutWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title Label
    titleLabel = new QLabel(tr("Project DeepLingo"), this);
    // No explicit alignment on the label itself, let the layout center it.
    
    // Create a horizontal layout to truly center the titleLabel
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addStretch(); 
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch(); 
    
    mainLayout->addLayout(titleLayout); // Add the horizontal layout to the main vertical layout
    mainLayout->addSpacing(15);

    // Content Display Area (where members/credits will go)
    contentDisplay = new QTextEdit(this);
    contentDisplay->setReadOnly(true);
    mainLayout->addWidget(contentDisplay);
}

// Populates the QTextEdit with the About content.
void AboutWindow::populateContent()
{
    // *** Text that you'll see in the about window ***
    QString content = R"(
        <p style="font-size: 14px; color: #333;">
        Welcome to DeepLingo, your personalized dictionary and vocabulary builder.
        </p>

        <h3 style="color: #4CAF50;">Project Development Team</h3>
        <p>
        This application was proudly created as a collaborative effort by the following members:
        </p>

        <ul>
            <li><strong>Anastacio, Lester Arvid P.:</strong> [Role - e.g., Lead Developer]</li>
            <li><strong>Catungal, Jan Kerwin B. 2:</strong> [Role - e.g., Documentation/UI/UX Design]</li>
            <li><strong>Avila, Vince Gabriel V:</strong> [Role - e.g., Documentation/Words For English and Tagalog]</li>
            <li><strong>Alcantara, Jason P:</strong> [Role - e.g., Documentation/Words For English and Tagalog]</li>
        </ul>
        <!-- -->)";
    // ****************************************************

    contentDisplay->setHtml(content);
}