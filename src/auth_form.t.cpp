#if defined(EPOXY_DUINO)
// #include <AUnit.h>
// #include <cstdint>
// #include "user_inputs.hpp"
// #include <Print.h>
//
// class ImitatedUserInputs : public UserInputs
// {
// public:
//     // For usage by code under test
//     virtual void set(Button btn, ButtonCb cb, void *ctx) override;
//     virtual void unset(Button btn) override;
//
//     virtual void set(Encoder, RotateCb cb, void *ctx) override;
//     virtual void unset(Encoder) override;
//
//     // For call from test
//     void click(Button btn);
//     void rotate(Encoder, const int direction);
//
//     ImitatedUserInputs();
//
// protected:
//     struct BtnHook
//     {
//         ButtonCb cb_;
//         void *ctx_;
//     };
//     struct EncoderHook
//     {
//         RotateCb cb_;
//         void *ctx_;
//     };
//
//     BtnHook btnHooks_[static_cast<uint8_t>(Button::num_of_buttons)];
//     EncoderHook encoderHooks_[static_cast<uint8_t>(Encoder::num_of_encoders)];
//
//     static void stub_btn_cb(void *);
//     static void stub_encoder_cb(void *, const int);
// };
//
// ////////////////////////////////////////////////////////////////////////////////
// ImitatedUserInputs::ImitatedUserInputs()
// {
//     memset(&btnHooks_, 0, sizeof(btnHooks_));
//     memset(&encoderHooks_, 0, sizeof(encoderHooks_));
// }
//
// void ImitatedUserInputs::set(Button btn, ButtonCb cb, void *ctx)
// {
//     btnHooks_[static_cast<uint8_t>(btn)].cb_ = cb;
//     btnHooks_[static_cast<uint8_t>(btn)].ctx_ = ctx;
// }
//
// void ImitatedUserInputs::unset(Button btn)
// {
//     set(btn, nullptr, nullptr);
// }
//
// void ImitatedUserInputs::set(Encoder encoder, RotateCb cb, void *ctx)
// {
//     encoderHooks_[static_cast<uint8_t>(encoder)].cb_ = cb;
//     encoderHooks_[static_cast<uint8_t>(encoder)].ctx_ = ctx;
// }
//
// void ImitatedUserInputs::unset(Encoder encoder)
// {
//     set(encoder, nullptr, nullptr);
// }
//
// void ImitatedUserInputs::click(Button btn)
// {
//     btnHooks_[static_cast<uint8_t>(btn)].cb_(btnHooks_[static_cast<uint8_t>(btn)].ctx_);
// }
//
// void ImitatedUserInputs::rotate(Encoder encoder, const int direction)
// {
//     encoderHooks_[static_cast<uint8_t>(encoder)].cb_(encoderHooks_[static_cast<uint8_t>(encoder)].ctx_, direction);
// }
//
// void ImitatedUserInputs::stub_btn_cb(void *) {}
// void ImitatedUserInputs::stub_encoder_cb(void *, const int) {}
//
// ////////////////////////////////////////////////////////////////////////////////
// // TODO: react on \n and jump to next k*width
// template<uint8_t height, uint8_t width>
// class InMemoryPrint: public Oled {
//   public:
//     size_t write(uint8_t c) override {
//       if (mIndex < height*width - 1) {
//         mBuf[mIndex] = c;
//         mIndex++;
//         return 1;
//       } else {
//         return 0;
//       }
//     }
//
//     size_t write(const uint8_t *buffer, size_t size) override {
//       if (buffer == nullptr) return 0;
//
//       while (size > 0 && mIndex < height*width - 1) {
//         write(*buffer++);
//         size--;
//       }
//       return size;
//     }
//
// // ESP32 and STM32duino do not provide a virtual Print::flush() method.
// #if defined(ESP32) || defined(ARDUINO_ARCH_STM32)
//     void flush() {
// #else
//     void flush() override {
// #endif
//       mIndex = 0;
//     }
//
//     /**
//      * Return the NUL terminated string buffer. After the buffer is no longer
//      * needed, the flush() method should be called to reset the internal buffer
//      * index to 0.
//      */
//     const char* getBuffer() const {
//       mBuf[mIndex] = '\0';
//       return mBuf;
//     }
//
//     uint16_t getBufSize() const {
//         return height*width;
//     }
//
//     void clear() {
//         memset(mBuf, 0, height*width);
//     }
//
//     void home() {
//         mIndex = 0;
//     }
//
//     void getLine(const uint8_t idx, char *buf, const uint8_t len) {
//         if (idx > height - 1) {
//             buf[0] = '\0';
//             return;
//         }
//         memcpy(buf, mBuf + idx*width, len < width ? len : width);
//     }
//
//   private:
//     mutable char mBuf[height*width];
//     uint8_t mIndex = 0;
// };
//
// ////////////////////////////////////////////////////////////////////////////////
// class AuthFormTestHelper
// {
// public:
//     AuthFormTestHelper(AuthForm &form) : form_(form) {}
//
//     void chooseNextSymbolGroup() {form_.chooseNextSymbolGroup();}
//     void eraseLastSymbol() {form_.eraseLastSymbol();}
//     void commitSymbol() {form_.commitSymbol()};
//     void commitPassword() {form_.commitPassword()};
//
//     char typingSymbol() const;
//
//     AuthForm form_;
// };
//
//
// ////////////////////////////////////////////////////////////////////////////////
// test(auth_form_ctor)
// {
//     InMemoryPrint<4, 20> oled;
//     {
//         const char *pBuf = oled.getBuffer();
//         for (const char *p = pBuf; p < pBuf + oled.getBufSize(); ++p) {
//             assertEqual(0, *p);
//         }
//     }
//
//     AuthForm authForm(oled);
//
//     const char *pBuf = oled.getBuffer();
//     {/// form should not print something before own initialization
//         const char *pBuf = oled.getBuffer();
//         for (const char *p = pBuf; p < pBuf + oled.getBufSize(); ++p) {
//             assertEqual(0, *p);
//         }
//     }
// };
//
// test(auth_form_init)
// {
//     InMemoryPrint<4, 20> oled;
//
//     AuthForm authForm(oled);
//     authForm.init(nullpth, nullpth);
// };
//
// test(auth_form_deactivate)
// {
//     InMemoryPrint<4, 20> oled;
//
//     struct FakeSwitchFormCb
//     {
//         uint8_t c {0};
//
//         void operator()(void *) {
//             ++c;
//         }
//     };
//
//     FakeSwitchFormCb cb;
//
//     AuthForm authForm(oled);
//     authForm.init(cb, nullptr);
//
//     assertEqual(0, cb.c);
//
//     authForm.deactivate();
//
//     assertEqual(1, cb.c);
// };
//
// test(auth_form_switch_letters_groups)
// {
//
// };
#endif // EPOXY_DUINO
