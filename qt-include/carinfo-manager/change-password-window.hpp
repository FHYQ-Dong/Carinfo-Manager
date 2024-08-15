/**
 * @file qt-include/carinfo-manager/change-password-window.hpp
 * @brief Declaration of class ChangePasswordWindow
 * 
 * @details
 * This file contains the declaration of the ChangePasswordWindow class.
 * The ChangePasswordWindow class provides a window to change the password of an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include <QWidget>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ChangePasswordWindow : public QWidgetWithSubWindows {
    Q_OBJECT
  public:
    ChangePasswordWindow(QWidget *parent,
                        const std::string &ip,
                        const int port,
                        const Account &acc,
                        QWidgetWithSubWindows *parentWindow = nullptr);
    ~ChangePasswordWindow();

  private:
    std::string ip;
    int port;
    Account acc;
    QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
    QLabel *oldPasswordLabel, *newPasswordLabel, *confirmPasswordLabel;
    QLineEdit *oldPasswordEdit, *newPasswordEdit, *confirmPasswordEdit;
    QPushButton *changePasswordButton;

  private slots:
    void changePassword();
};
