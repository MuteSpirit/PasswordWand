#pragma once

#include "blind_call.hpp"
#include "layout.hpp"
#ifndef __AUTH_FORM__
#define __AUTH_FORM__

#include "menu.hpp"
#include "oled.hpp"
#include "user_inputs.hpp"

class AuthFormTestHelper;


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
        specials
    };

    static constexpr char alphaLowerChars[] {"abcdefghijklmnopqrstuvwxyz"}; 
    static constexpr char alphaUpperChars[] {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"}; 
    static constexpr char digitChars[] {"0123456789"}; 
    static constexpr char specialChars[] {" /?><,:';|}{][+_)(*&^%$#!~=-@. "};

    AuthForm(Oled &oled, UserInputs &inputs);

    virtual void init(BlindCall switchMenuCb) override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    friend AuthFormTestHelper;

    void chooseNextSymbolGroup();
    void eraseLastSymbol();
    void commitSymbol();
    void commitPassword();

    char typingSymbol() const;

protected:
    Oled &oled_;
    UserInputs &userInputs_;

    BlindCall switchMenuCb_ {BlindCall::stub()};

    uint8_t typingSymbolIdx_ {0};
    SymbolGroup typingSymbolGroup_ {AuthForm::SymbolGroup::alphaLowerCase};

    char password[MASTER_PASSWORD_SIZE];
    // TODO: where to take salt? shoud this form do that?
};

#endif // !__AUTH_FORM__
