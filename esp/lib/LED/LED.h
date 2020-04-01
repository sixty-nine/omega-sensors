#ifndef LED_h
#define LED_h

class LED
{
    public:
        LED(int pinNumber)
        {
            _pinNumber = pinNumber;
            pinMode(pinNumber, OUTPUT);
        }
        void blink(int count, int milli)
        {
            for (int i = 0; i < count; i++) {
                on(milli);
                off(milli);
            }
        }
        void on(int milli)
        {
            digitalWrite(_pinNumber, HIGH);
            delay(milli);
        }
        void off(int milli)
        {
            digitalWrite(_pinNumber, LOW);
            delay(milli);
        }
        void on() { on(0); }
        void off() { off(0); }
    private:
        int _pinNumber;
};

#endif
