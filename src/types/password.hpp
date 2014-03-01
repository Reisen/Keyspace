#ifndef TYPE_PASSWORD
#define TYPE_PASSWORD

#include <string>

using std::string;

/* Define some default password encoders. */
string hex_encode(uint8_t *, size_t);
string ascii85_encode(uint8_t *, size_t);

class Password {
    public:
        typedef string (*Encoder)(uint8_t *data, size_t data_len);

        friend void swap(Password &, Password &);

        Password();
        Password(Password &&);
        Password(Password const&);
        Password(string, string, int, int);
        virtual ~Password();

        string generate(string const&) const;
        void set_encoder(Encoder e);

    private:
        string service  = "";
        string username = "";
        int length      = 0;
        int changes     = 0;
        Encoder encoder = ascii85_encode;
};

#endif
