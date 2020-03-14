typedef void (*TimerCallback)(void);

class Timer {
    public:
        Timer(unsigned long interval, TimerCallback callback);
        void setInterval(unsigned long interval);
        void update();
    private:
        TimerCallback _callback;
        unsigned long _prevMillis = 0;
        unsigned long _interval = 500;
};
