#include "GUI/LoadingScreen.h"
#include "Qt_includes.h"

LoadingScreen::LoadingScreen(QWidget *parent) :
    QDialog(parent, Qt::SplashScreen | Qt::FramelessWindowHint), // Frameless and Splash Screen
    m_timer(new QTimer(this)),
    m_duration(0),
    m_elapsed(0)
{
    // Load the splash image
    if (m_splashImage.load("deeplingo_logo_loading.png")) {
        setFixedSize(m_splashImage.size());
        
        int imgWidth = m_splashImage.width();
        int imgHeight = m_splashImage.height();
        
        // --- Calculate m_progressBarRect based on proportions ---
        // These proportions are based on the provided logo image:
        int barWidth = static_cast<int>(imgWidth * 0.40); 
        int barHeight = static_cast<int>(imgHeight * 0.06); 
        int barX = (imgWidth - barWidth) / 2;
        int barY = static_cast<int>(imgHeight * 0.52);
        
        m_progressBarRect = QRect(barX, barY, barWidth, barHeight);
    } else {
        // Fallback if image not loaded
        setFixedSize(600, 400); 
        m_progressBarRect = QRect(150, 300, 300, 30); // Default placeholder
        qWarning("LoadingScreen: Splash image 'deeplingo_logo_loading.png' not found or failed to load.");
    }

    // --- Transparency ---
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);

    // Connect timer for progress updates
    connect(m_timer, &QTimer::timeout, this, &LoadingScreen::updateProgress);

    // Center the splash screen
    const QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        move(screen->geometry().center() - frameGeometry().center());
    }
}

void LoadingScreen::start(int durationMs) {
    m_duration = durationMs;
    m_elapsed = 0;
    show();
    m_timer->start(20); // Update progress every 20ms
} 

void LoadingScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the splash image 
    if (!m_splashImage.isNull()) {
        painter.drawPixmap(0, 0, m_splashImage);
    } else {
        // Fallback: draw a simple box if the image is missing
        painter.fillRect(rect(), Qt::lightGray);
    }

    // Draw the custom progress bar
    if (m_duration > 0 && !m_splashImage.isNull()) {
        double progress = static_cast<double>(m_elapsed) / m_duration;
        int barWidth = static_cast<int>(m_progressBarRect.width() * progress);

        // 1. Draw the background/outline of the progress bar (the empty square)
        painter.setPen(QPen(QColor("#4CAF50"), 4));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(m_progressBarRect);

        // 2. Draw the filled part of the progress bar
        painter.setPen(Qt::NoPen); 
        painter.setBrush(QColor("#4CAF50")); // Green fill
        
        // Create a slightly inset rectangle for the fill to look clean inside the border
        QRect fillRect = m_progressBarRect.adjusted(3, 3, -3, -3); // Inset by 3 pixels
        fillRect.setWidth(barWidth - 6); // Adjust width for the inset
        
        painter.drawRect(fillRect);
    }
}

void LoadingScreen::updateProgress() {
    m_elapsed += m_timer->interval();
    if (m_elapsed >= m_duration) {
        m_timer->stop();
        emit finished();
        close();
    }
    update(); // Request a repaint to update the progress bar
}