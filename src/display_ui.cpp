#include "display_ui.hpp"
#include "user_inputs.hpp"
#include "accounts_menu.hpp"
#include "settings_menu.hpp"


DisplayUI::DisplayUI(Display &oled,
                     UserInputs &userInputs,
                     AccountsMenu &accMenu,
                     SettingsMenu &settingsMenu)
    : oled_(oled)
    , userInputs_(userInputs)
    , accMenu_(accMenu)
    , settingsMenu_(settingsMenu)
{}

void
DisplayUI::ui_setup(void)
{
    oled_.clear();
    oled_.home();

    accMenu_.init(BlindCall::make(this, &DisplayUI::switch2settingsMenu));
    settingsMenu_.init(BlindCall::make(this, &DisplayUI::switch2accountsMenu));

    switch2accountsMenu();
}

void
DisplayUI::switch2settingsMenu()
{
    accMenu_.deactivate();
    settingsMenu_.activate();

    userInputs_.set(UserInputs::Button::triangle, BlindCall::make(this, &DisplayUI::switch2accountsMenu));
}

void
DisplayUI::switch2accountsMenu()
{
    settingsMenu_.deactivate();
    accMenu_.activate();

    userInputs_.set(UserInputs::Button::triangle, BlindCall::make(this, &DisplayUI::switch2settingsMenu));
}
