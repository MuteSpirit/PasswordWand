#pragma once
#ifndef __ACCOUNTS_MENU_HPP__
#define __ACCOUNTS_MENU_HPP__

#include "menu.hpp"
#include "model.hpp"

class Settings;
class UserInputs;
class Oled;

template<typename T>
class ModelStorage;


class AccountsMenu : public Menu
{
public:
    AccountsMenu(Oled& oled, UserInputs& userInputs, const Settings&, ModelStorage<Account>&);
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

    Oled& oled_;
    UserInputs& userInputs_;

    const Settings& settings_;
    ModelStorage<Account>& modelStore_;

    Account acc_;
    uint16_t acc_idx_{0};
};

#endif // !__ACCOUNTS_MENU_HPP__

