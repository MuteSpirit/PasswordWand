#pragma once
#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <RotaryEncoder.h>
#include "user_inputs.hpp"

////////////////////////////////////////////////////////////////////////////////
class DeviceUserInputs : public UserInputs
{
public:
    DeviceUserInputs();

    void setup(); /// call from sketch setup()
    void loop_step(); /// call from sketch loop()

    virtual void set(Button btn, BlindCall cb) override;
    virtual void unset(Button btn) override;

    virtual void set(Encoder, BlindCall cb) override;
    virtual void unset(Encoder) override;

protected:
    void checkButton(UserInputs::Button btn);
    void checkEncoder(UserInputs::Encoder enc);

protected:
    struct ButtonHook
    {
        BlindCall cb_     {BlindCall::stub()};
        bool push_happen_ {false};
    };

    ButtonHook btnHooks_[static_cast<uint8_t>(UserInputs::Button::num_of_buttons)];

    BlindCall encoderHooks_[static_cast<uint8_t>(UserInputs::Encoder::num_of_encoders)] {BlindCall::stub()};

    RotaryEncoder rotaryEncoder_;
};

#endif // !__BOARD_HPP__
