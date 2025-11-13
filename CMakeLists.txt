cmake_minimum_required(VERSION 3.16)
project(DSA_Dictionary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    Main.cpp
    
    # GUI Files
    GUI/Gui_Holder.cpp
    GUI/LoadingScreen.cpp
    GUI/UserProfileWindow.cpp
    GUI/WordDetailWindow.cpp
    GUI/AboutWindow.cpp
    
    # Resources
    resources/icons.qrc
    
    # Function Files
    Function_Files/Function.cpp
    
    # Word Files
    Word_Files/Word_Storage.cpp
    
    # User Files
    User_Files/UserStorage.cpp
    User_Files/UserDialog.cpp
)

# subdirectories to the include path 
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/GUI
    ${CMAKE_SOURCE_DIR}/User_Files
    ${CMAKE_SOURCE_DIR}/Word_Files
    ${CMAKE_SOURCE_DIR}/Function_Files
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)