#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ipSend("127.0.0.1"), //Default values
    m_portSend(44400),
    m_portRecv(44400)
{
    ui->setupUi(this);
    if(!readConfig())
        qDebug() << "Can't read config. Using default values..";

    m_sockRecv.bind(m_portRecv);
    connect(&m_sockRecv, SIGNAL(readyRead()),
            this,        SLOT(recvdMessage()), Qt::QueuedConnection);
    ui->line->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked()
{
    QByteArray buff;
    buff.append(ui->line->text());
    QString msg = "";
    msg.append(QDateTime::currentDateTime().toString(trUtf8("← [hh:mm:ss] ")));
    msg.append(buff);
    msg.append("\n");
    ui->dialog->append(msg);
    m_sockSend.writeDatagram(buff.data(),
                             buff.size(),
                             QHostAddress(m_ipSend),
                             m_portSend);
    ui->line->clear();
    ui->line->setFocus();
}

void MainWindow::recvdMessage()
{
    QByteArray buff;
    buff.resize(m_sockRecv.pendingDatagramSize());
    m_sockRecv.readDatagram(buff.data(), buff.size());
    QString msg = "";
    msg.append(QDateTime::currentDateTime().toString(trUtf8("→ [hh:mm:ss] ")));
    msg.append(buff);
    msg.append("\n");
    msg = msg.toUtf8();
    if(buff.size() != 0)
        ui->dialog->append(msg);
}

bool MainWindow::readConfig()
{
    QXmlStreamReader xmlReader;
    QFile file("config.xml");
    if(!file.open(QIODevice::ReadOnly))
        return false;

    xmlReader.setDevice(&file);
    QXmlStreamReader::TokenType token;

    while (!xmlReader.hasError() && !xmlReader.isEndDocument())
    {
        token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::StartElement)
        {
            if (xmlReader.name() == "main")
            {
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                {
                    if (attr.name().toString() == QLatin1String("ip_send"))
                        m_ipSend = attr.value().toString();
                    else if (attr.name().toString() == QLatin1String("port_send"))
                        m_portSend = attr.value().toString().toInt();
                    else if (attr.name().toString() == QLatin1String("port_recv"))
                        m_portRecv = attr.value().toString().toInt();
                }
            }
        }
    }
    file.close();
    return true;
}
