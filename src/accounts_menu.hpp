#pragma once

#ifndef __ACCOUNTS_MENU_HPP__
#define __ACCOUNTS_MENU_HPP__

#include "model.hpp"
#include "menu.hpp"
#include "board.hpp"

void send_username(void *ctx);
void send_password(void *ctx);
void navigate_accounts(void *ctx, int direction);


class AccountsMenu : public Menu
{
public:
    AccountsMenu(Display &);
    ~AccountsMenu() {};

    virtual void init() override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    Account acc_;
    uint8_t acc_idx_{0};
    Display& oled_;

    void draw_acc();

    friend void send_username(void *ctx);
    friend void send_password(void *ctx);
    friend void navigate_accounts(void *ctx, int direction);
};

#endif // !__ACCOUNTS_MENU_HPP__

