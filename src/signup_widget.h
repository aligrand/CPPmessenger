#ifndef SIGNUP_WIDGET_H
#define SIGNUP_WIDGET_H

#include <QWidget>
#include <cstdlib>
#include <ctime>
#include <QLineEdit>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

namespace Ui {
class signup_widget;
}

class signup_widget : public QWidget
{
    Q_OBJECT

public:
    explicit signup_widget(QWidget *parent = nullptr);
    ~signup_widget();

signals:
    void goto_si();
    void su(QString username, QString password, QString pn, QString email);
    void is_username_avalable(QString text, bool &result);

private slots:
    void on_username_lineEdit_textChanged(const QString &arg1);

    void on_password_lineEdit_textChanged(const QString &arg1);

    void on_c_password_lineEdit_textChanged(const QString &arg1);

    void on_p_visible_OF_clicked();

    void on_cp_visible_OF_clicked();

    void on_renew_captcha_pushButton_clicked();

    void on_signup_pushButton_clicked();

    void on_goto_signin_link_linkActivated(const QString &link);

    void on_email_lineEdit_textChanged(const QString &arg1);

    void on_pn_lineEdit_textChanged(const QString &arg1);

private:
    Ui::signup_widget *ui;
    bool is_password_visible = false;
    bool is_Cpassword_visible = false;
    bool is_password_val = false, is_username_val = false, is_Cpassword_val = false, is_pn_val = false,
            is_email_val = false;
    QString captcha_code;
    QRegularExpression username_regex, password_regex, Cpassword_regex, pn_regex, email_regex;
    QRegularExpressionValidator username_regex_val, password_regex_val, Cpassword_regex_val, pn_regex_val,
                                 email_regex_val;
    const QString captcha_name_arr[10] = {"12hello", "aA5Bb", "F50vs", "h23A",
                                    "hP439", "Pa33p", "Q49mn", "sEv07", "wHy32", "yU168"};

    void captcha_gen();
    inline int random(int min, int max);
};

#endif // SIGNUP_WIDGET_H