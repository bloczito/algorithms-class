#include "browser.h"
#include "ui_browser.h"
#include <QStringBuilder>
#include <QDebug>
#include <QUrlQuery>
#include <QListWidget>
#include <QWebEngineHistory>

const QUrl Browser::defaultUrl = QUrl("https://start.duckduckgo.com");
const QString Browser::defaultSearchEngine = QString("duckduckgo");
const QUrl Browser::homePage = QUrl("https://reddit.com");

Browser::Browser(QWidget *parent)
    : QMainWindow(parent)
    , currentUrl(this->defaultUrl)
    , currentSearchEngine(this->defaultSearchEngine)
    , ui(new Ui::Browser)
{
    ui->setupUi(this);
    searchEngines.insert("duckduckgo", "https://www.duckduckgo.com");

    connect(ui->lineEdit,
            &QLineEdit::editingFinished,
            this,
            &Browser::updatePage);

    connect(ui->webEngineView,
            &QWebEngineView::urlChanged,
            this,
            &Browser::updateLineUrl);

    connect(ui->homeButton,
            &QPushButton::clicked,
            this,
            [this]{this->loadPage(this->homePage);});

    connect(ui->reloadButton,
            &QPushButton::clicked,
            ui->webEngineView,
            &QWebEngineView::reload);

    connect(ui->webEngineView,
            &QWebEngineView::loadProgress,
            ui->pageProgressBar,
            &QProgressBar::setValue);

    connect(ui->backButton,
            &QPushButton::clicked,
            ui->webEngineView,
            &QWebEngineView::back);

    connect(ui->forwardButton,
            &QPushButton::clicked,
            ui->webEngineView,
            &QWebEngineView::forward);

    connect(ui->actionHistory,
            &QAction::triggered,
            this,
            &Browser::showHistory);


    loadPage(getUrlFromLine());
}

void Browser::loadPage(const QUrl &url)
{
    ui->webEngineView->load(url);
}

void Browser::loadPageFromLine()
{
    loadPage(getUrlFromLine());
}

QUrl Browser::getUrlFromLine()
{
    return QUrl::fromUserInput(ui->lineEdit->text());
}

QUrl Browser::getUrlFromPage()
{
    return ui->webEngineView->url();
}

void Browser::updateLineUrl(const QUrl &url)
{
    ui->lineEdit->setText(url.url());
}

void Browser::searchWith(const QString& searchEngine, const QString& question)
{
    QString suffix("?q=");
    QUrl url(searchEngines.value(searchEngine, defaultUrl.url()) %
             suffix %
             question);
    loadPage(url);
}

void Browser::updatePage()
{
    QUrl request = getUrlFromLine();

    if (isUrl(request)) loadPage(request);
    else searchWith("", request.url());
}

bool Browser::isUrl(const QUrl& url)
{
    qDebug() << url.url();
    return !url.scheme().isEmpty();
}

void Browser::showHistory()
{
    //QListWidget history(this);

    //for (auto elem : ui->webEngineView->history()->items())
    //    qDebug() << elem.title() << ":  " << elem.url();
}

Browser::~Browser()
{
    delete ui;
}


