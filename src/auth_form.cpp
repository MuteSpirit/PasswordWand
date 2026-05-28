#include "auth_form.hpp"
#include "user_inputs.hpp"

AuthForm::AuthForm(Oled &oled, UserInputs &userInputs)
    : oled_(oled)
    , userInputs_(userInputs)
{
}

void AuthForm::init(BlindCall switchMenuCb)
{
    switchMenuCb_ = switchMenuCb;
}

void AuthForm::activate()
{
    oled_.home();

    userInputs_.set(UserInputs::Button::circle, BlindCall::make(this, &AuthForm::commitPassword));
    userInputs_.set(UserInputs::Button::cross, BlindCall::make(this, &AuthForm::commitSymbol));
    userInputs_.set(UserInputs::Button::square, BlindCall::make(this, &AuthForm::chooseNextSymbolGroup));
    userInputs_.set(UserInputs::Button::triangle, BlindCall::make(this, &AuthForm::eraseLastSymbol));
}

void AuthForm::deactivate()
{
    userInputs_.unset(UserInputs::Button::circle);
    userInputs_.unset(UserInputs::Button::cross);
    userInputs_.unset(UserInputs::Button::square);

    userInputs_.set(UserInputs::Button::triangle, switchMenuCb_);

    userInputs_.unset(UserInputs::Encoder::rotary);

    oled_.clear();
}
