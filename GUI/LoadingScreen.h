#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QDialog>
#include <QTimer>
#include <QPixmap> 
#include <QRect> 

class LoadingScreen : public QDialog {
    Q_OBJECT
public:
    explicit LoadingScreen(QWidget *parent = nullptr);
    void start(int durationMs);

signals:
    void finished(); // Emitted when the loading duration is over

protected:
    void paintEvent(QPaintEvent *event) override; // To draw the background image and progress bar

private slots:
    void updateProgress();

private:
    QTimer *m_timer;
    int m_duration; // Total duration of the loading screen
    int m_elapsed;  // Elapsed time
    QPixmap m_splashImage; // The image for the splash screen
    
    // Position and size for the progress bar rectangle
    QRect m_progressBarRect; 
};

#endif // LOADINGSCREEN_H