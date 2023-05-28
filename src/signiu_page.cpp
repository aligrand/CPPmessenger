#include "signiu_page.h"
#include "ui_signiu_page.h"

signIU_page::signIU_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::signIU_page),
    ap_window(new auth_page()),
    userinfo_w(new UserInfoWindow())
{
    ui->setupUi(this);

    si_w = new signin_widget(this);
    si_w->setVisible(false);
    si_w->move(293, 85);

    su_w = new signup_widget(this);
    su_w->setVisible(false);
    su_w->move(163, 30);

    si_w->setVisible(true);

    connect(si_w, &signin_widget::si, this, &signIU_page::signin);
    connect(si_w, &signin_widget::goto_su, this, &signIU_page::goto_signup);
    connect(server, &ServerMan::loginResult, this, &signIU_page::signin_result);
    connect(su_w, &signup_widget::su, this, &signIU_page::signup);
    connect(su_w, &signup_widget::goto_si, this, &signIU_page::goto_signin);
    connect(ap_window, &auth_page::result_ready, this, &signIU_page::auth_result);
}

signIU_page::~signIU_page()
{         
    delete ui;
    delete si_w;
    delete su_w;
}

void signIU_page::goto_signin()
{
    su_w->setVisible(false);

    si_w->setVisible(true);
}

void signIU_page::goto_signup()
{
    si_w->setVisible(false);

    su_w->setVisible(true);
}

void signIU_page::signin(QString username, QString password)
{
    sign_info[0] = username;
    sign_info[1] = password;

    emit server->command("LOGIN " + username + " " + password);
}

void signIU_page::signin_result(qint8 result)
{
    if (result == 0)
    {
        QMessageBox::critical(this, "Error", "User not found");
    }
    else if (result == 2)
    {
        QMessageBox::critical(this, "Error", "Password is incorect");
    }
    else
    {
        myUsername = sign_info[0];

        QFile u_info("userinfo.txt");
        u_info.open(QIODevice::WriteOnly | QIODevice::Text);
        u_info.write((sign_info[0] + "\n").toStdString().c_str());
        u_info.write((sign_info[1] + "\n").toStdString().c_str());
        u_info.close();

        emit server->command("UPDATE-DB");
        emit server->command("MESSAGE-INDEX 0");

        go_next_window = true;
    }
}

void signIU_page::signup(QString username, QString password, QString pn, QString email)
{
    sign_info[0] = username;
    sign_info[1] = password;
    sign_info[2] = pn;
    sign_info[3] = email;

    ap_window->show();
    this->hide();
}

void signIU_page::auth_result(qint8 res)
{
    if (res == 1)
    {
        SqlRecordQString record;

        record << sign_info[0] << sign_info[3] << sign_info[2] << "" << "" << "" << "1";
        record.end();

        emit server->command(QString("ADD-USER ") + record);
        emit server->command("SET-PASS " + sign_info[0] + " " + sign_info[1]);

        emit server->command("LOGIN " + sign_info[0] + " " + sign_info[1]);

        myUsername = sign_info[0];

        QFile u_info("userinfo.txt");
        u_info.open(QIODevice::WriteOnly | QIODevice::Text);
        u_info.write((sign_info[0] + "\n").toStdString().c_str());
        u_info.write((sign_info[1] + "\n").toStdString().c_str());
        u_info.close();

        QFile m_index("message-index.txt");
        m_index.open(QIODevice::WriteOnly | QIODevice::Text);
        m_index.write("0");
        m_index.close();

        QSqlQuery sqlQuery;

        sqlQuery.prepare("INSERT INTO users (username, emailAddress, phoneNumber, isOnline)"
                         "VALUES (?, ?, ?, 1)");
        sqlQuery.addBindValue(sign_info[0]);
        sqlQuery.addBindValue(sign_info[3]);
        sqlQuery.addBindValue(sign_info[2]);
        sqlQuery.exec();
    }
    else
    {
        ap_window->hide();
        this->show();
    }
}
