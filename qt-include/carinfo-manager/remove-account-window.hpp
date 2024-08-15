/**
 * @file qt-include/carinfo-manager/remove-account-window.hpp
 * @brief Declaration of class RemoveAccountWindow
 * 
 * @details
 * This file contains the declaration of the RemoveAccountWindow class.
 * The RemoveAccountWindow class provides a window to remove an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class RemoveAccountWindow : public QWidgetWithSubWindows {
    Q_OBJECT
  public:
    RemoveAccountWindow(QWidget *parent,
                        const std::string &ip,
                        const int port,
                        const Account &acc,
                        QWidgetWithSubWindows *parentWindow = nullptr);
    ~RemoveAccountWindow();

  private:
    std::string ip;
    int port;
    Account acc;
    QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
    QLabel *targetUsernameLabel;
    QLineEdit *targetUsernameEdit;
    QPushButton *removeButton;

  private slots:
    void removeAccount();
};
