#ifndef HardwareInfo_h
#define HardwareInfo_h

typedef struct {
    int ramFree;
    int ramUsed;
    float ramUsedPercent;
    float version;
} HardwareInfoRes;

class HardwareInfo {
    public:
        HardwareInfo();
        HardwareInfoRes getInfo();
    private:
        int freeRam();
};

#endif
