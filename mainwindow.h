#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDateTime>
#include <QDebug>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSend_clicked();
    void recvdMessage();

private:
    Ui::MainWindow *ui;
    QUdpSocket m_sockRecv;
    QUdpSocket m_sockSend;
    QString    m_ipSend;
    int        m_portSend;
    int        m_portRecv;

    bool readConfig();
};

#endif
