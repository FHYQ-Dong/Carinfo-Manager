/**
 * @file qt-include/carinfo-manager/add-account-window.hpp
 * @brief Declaration of class AddAccountWindow
 * 
 * @details
 * This file contains the declaration of the AddAccountWindow class.
 * The AddAccountWindow class provides a window to add a new account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"
#include "carinfo-manager/httphandler-client.hpp"

class AddAccountWindow : public QWidgetWithSubWindows {
    Q_OBJECT
    public:
        AddAccountWindow(QWidget *parent,
                         const std::string &ip,
                         const int port,
                         const Account &acc,
                         QWidgetWithSubWindows *parentWindow = nullptr);
        ~AddAccountWindow();

    private:
        std::string ip;
        int port;
        Account acc;
        QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
        QLabel *newUsernameLabel, *newPasswordLabel, *confirmPasswordLabel, *newAccounttypeLabel;
        QLineEdit *newUsernameEdit, *newPasswordEdit, *confirmPasswordEdit;
        QComboBox *newAccounttypeComboBox;
        QPushButton *addAccountButton;

    private slots:
        void addAccount();
};
