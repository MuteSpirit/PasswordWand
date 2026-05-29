#pragma once
#include "auth.hpp"
#ifndef __AUTH_FORM__
#define __AUTH_FORM__

#include "menu.hpp"
#include "layout.hpp"


class AuthFormTestHelper;
class UserInputs;
class Oled;
class Authenticator;

////////////////////////////////////////////////////////////////////////////////
/// * Rotate encoder will scroll current symbol from available symbolic group
/// * Button "square" will change current group of symbols - alpha lowercase, alpha uppercase, digits, specials
/// * Button "cross" will commit current symbol
/// * Button "triangle" will erase last symbol (=== Backspace) to decrease amount of login failures due to one mistake in the middle of long password
/// * Button "circle" will commit password and trigger authentication procedure
/// * Master password will be visible during typing
class AuthForm : public Menu
{
public:
    enum class SymbolGroup : uint8_t
    {
        alphaLowerCase,
        alphaUpperCase,
        digit,
        specials,
        COUNT
    };

    AuthForm(Oled &oled, UserInputs &inputs, Authenticator& auth);

    virtual void init(BlindCall switchMenuCb) override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    friend AuthFormTestHelper;

    void chooseNextSymbolGroup();

    void chooseNextSymbol();
    void choosePrevSymbol();

    void eraseLastSymbol();
    void commitSymbol();
    void commitPassword();

    char typingSymbol() const;
    uint8_t typingGroupLen() const;

protected:
    Oled &oled_;
    UserInputs &userInputs_;
    Authenticator& auth_;

    BlindCall switchMenuCb_ {BlindCall::stub()};

    uint8_t typingSymbolIdx_ {0};
    const char *typingChars_ {nullptr};
    SymbolGroup typingSymbolGroup_ {AuthForm::SymbolGroup::alphaLowerCase};

    char password_[MASTER_PASSWORD_SIZE];
};

#endif // !__AUTH_FORM__
