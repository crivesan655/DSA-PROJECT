#include "Qt_includes.h"
#include "GUI/Gui_Holder.h"
#include "User_Files/UserStorage.h"
#include "User_Files/UserDialog.h"
#include "GUI/LoadingScreen.h" 

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Load existing users (file is users.json in cwd).
    // If a previous user was saved, they will be set as the current user here.
    UserStorage::instance().load("users.json");

    // Show the custom loading screen with title and animated bar.
    LoadingScreen loader;
    
    QEventLoop loop;
    QObject::connect(&loader, &LoadingScreen::finished, &loop, &QEventLoop::quit);
    loader.start(1400); 
    loop.exec();        

    // Show a modal dialog to collect user name and age (user can cancel).
    // Only show the UserDialog if no user is currently set.
    if (UserStorage::instance().currentUser().isEmpty()) {
        UserDialog dlg;
        // The dialog handles saving the new user and setting them as the current user on accept.
        dlg.exec(); 
    }

    // Show main application window.
    Gui_Holder w;
    w.show();

    return a.exec();
}