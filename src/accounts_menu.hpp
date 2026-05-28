#pragma once
#ifndef __ACCOUNTS_MENU_HPP__
#define __ACCOUNTS_MENU_HPP__

#include <inttypes.h>
#include "model.hpp"
#include "menu.hpp"
#include "board.hpp"

class Settings;
class UserInputs;


class AccountsMenu : public Menu
{
public:
    AccountsMenu(Display &oled, UserInputs &userInputs, const Settings&);
    ~AccountsMenu() = default;

    virtual void init(BlindCall switchMenuCb) override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    void sendUsername();
    void sendPassword();
    void sendTab();
    void navigateAccounts(int direction);

    void draw();

    Display& oled_;
    UserInputs &userInputs_;

    const Settings& settings_;

    Account acc_;
    uint8_t acc_idx_{0};
};

#endif // !__ACCOUNTS_MENU_HPP__

