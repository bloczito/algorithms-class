#include "browser.h"
#include <QtWebEngineCore>
#include <QWebEngineView>
#include <QApplication>
#include <iostream>
#include <cstdlib>

int basic(int argc, char *argv[]) {
    QUrl starturl("https://www.duckduckgo.com");
    if (argc == 2) starturl = argv[1];

    QApplication app(argc, argv);
    Browser browser;
    browser.show();
    return app.exec();
}



int main(int argc, char *argv[]) {
    return basic(argc, argv);
}
