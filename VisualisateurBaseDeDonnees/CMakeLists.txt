cmake_minimum_required(VERSION 3.5)

project(VisualisateurBaseDeDonnees VERSION 0.1 LANGUAGES CXX)



# Configuration de base de CMake
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclure les chemins de recherche pour AUTOUIC pour trouver les fichiers .ui dans le dossier 'ui'
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Trouver les packages Qt nécessaires, y compris Widgets et Sql pour le support de la base de données
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Définir les fichiers sources et d'en-têtes du projet
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    headers/mainwindow.h
    ui/mainwindow.ui

)

# Inclure les répertoires d'en-têtes pour que le compilateur puisse trouver les fichiers .h
include_directories(headers)

# Définir la cible exécutable du projet et spécifier les fichiers sources
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VisualisateurBaseDeDonnees
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        headers/jsonmanager.h
        headers/usercreationwindow.h
        src/usercreationwindow.cpp


        src/jsonmanager.cpp
        headers/loginwindow.h
        src/loginwindow.cpp


    )
else()
    if(ANDROID)
        add_library(VisualisateurBaseDeDonnees SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(VisualisateurBaseDeDonnees
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Lier les bibliothèques Qt (Widgets et Sql) à l'application
target_link_libraries(VisualisateurBaseDeDonnees PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Configuration supplémentaire pour les cibles, si nécessaire
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VisualisateurDB)
endif()
set_target_properties(VisualisateurBaseDeDonnees PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VisualisateurBaseDeDonnees
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finaliser l'exécutable, nécessaire pour les projets Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VisualisateurBaseDeDonnees)
endif()
