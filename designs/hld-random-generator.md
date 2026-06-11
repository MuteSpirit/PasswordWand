# Random numbers generator

Random number generator is needed for generating master password salt, salts for each credential account.

## nRF52840 Pro Micro

Next board is planned to be used - https://ali.click/oycee1k.

It's based on nRF52840 chip and according to description has TRNG (True Random Number Generator).

Nordic Semiconductor "nRF52840 Objective Product specification 0.5.1" says:

```
25 RNG — Random number generator

The Random number generator (RNG) generates true non-deterministic random numbers based on internal
thermal noise that are suitable for cryptographic purposes. The RNG does not require a seed value.
```

Adafruit libraries stack for nRF52840 in Arduino IDE (see board manager https://adafruit.github.io/arduino-board-index/package_adafruit_index.json) contains library 
Adafruit_nRFCrypto which support work with TRNG.

## (Deprecated) Arduino Pro Micro

PasswordPump reads the voltage from ADC_READ_PIN floating pin to seed the random number generator. That pin should be not grounded or pulled up to VCC.

Such way of receiving random values depends on electro magneric fields close to the device.

To increase randomizing we may try next things:
* gather voltage several times and summarize them
* we may multiply and summarize them, e.g. multiple 4 and summarize with 3
* use another source of entrophy, i.e. light. Set photoresistor (and one more current limiting resistor) between another analog pin and ground and receive voltage on that pin

I've tested 3 entrophy sources on Arduino Uno:
1. unplugged pin
2. 10k Om + photoresistor
3. digital temperature sensor

Conclusion: all are similar to each other but "unplugged pin" will be less cost

```
21:05:16.035 -> Temp V=246
21:05:16.035 -> Unplugged pin V=246
21:05:16.234 -> Photodiode V=235
21:05:16.234 -> Temp V=237
21:05:16.234 -> Unplugged pin V=238
21:05:16.433 -> Photodiode V=227
21:05:16.433 -> Temp V=231
21:05:16.433 -> Unplugged pin V=232
21:05:16.632 -> Photodiode V=224
21:05:16.632 -> Temp V=228
21:05:16.632 -> Unplugged pin V=228
21:05:16.831 -> Photodiode V=224
21:05:16.831 -> Temp V=228
21:05:16.831 -> Unplugged pin V=228
21:05:17.063 -> Photodiode V=226
21:05:17.063 -> Temp V=233
21:05:17.063 -> Unplugged pin V=231
21:05:17.263 -> Photodiode V=235
21:05:17.263 -> Temp V=238
21:05:17.263 -> Unplugged pin V=236
21:05:17.462 -> Photodiode V=244
21:05:17.462 -> Temp V=246
21:05:17.462 -> Unplugged pin V=242
21:05:17.661 -> Photodiode V=256
21:05:17.661 -> Temp V=256
21:05:17.661 -> Unplugged pin V=250
21:05:17.860 -> Photodiode V=268
21:05:17.860 -> Temp V=265
21:05:17.860 -> Unplugged pin V=258
21:05:18.059 -> Photodiode V=279
21:05:18.059 -> Temp V=275
21:05:18.059 -> Unplugged pin V=267
21:05:18.258 -> Photodiode V=291
21:05:18.258 -> Temp V=284
```
