#ifndef TYPE_OPTIONS
#define TYPE_OPTIONS

struct Options {
    int argc           = 0;
    int length         = 20;
    int changes        = 0;
    char *const *argv  = 0;
    bool alphanumeric  = false;
    bool help          = false;
    const char *master = nullptr;
};

#endif
