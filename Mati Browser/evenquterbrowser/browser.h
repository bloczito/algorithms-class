#pragma once

#include <QMainWindow>
#include <QUrl>
#include <QHash>
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class Browser; }
QT_END_NAMESPACE

class Browser : public QMainWindow
{
    Q_OBJECT

public:
    Browser(QWidget *parent = nullptr);
    Browser(const QUrl &url, QWidget *parent = nullptr);
    void loadPage(const QUrl &url);
    void loadPageFromLine();
    QUrl getUrlFromLine();
    QUrl getUrlFromPage();
    void updateLineUrl(const QUrl &url);
    void updatePage();
    void searchWith(const QString &searchEngine, const QString &query);
    void showHistory();
    ~Browser();

private:
    QUrl currentUrl;
    QString currentSearchEngine;
    Ui::Browser *ui;
    static const QUrl defaultUrl;
    static const QString defaultSearchEngine;
    static const QUrl homePage;

    QHash<QString, QString> searchEngines{};


private:
    bool isUrl(const QUrl& url);
};
