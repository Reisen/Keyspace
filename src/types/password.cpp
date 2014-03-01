#include "password.hpp"
#include <algorithm>
#include <nettle/pbkdf2.h>

void
swap(Password &first, Password &second) {
    using std::swap;
    swap(first.length, second.length);
    swap(first.service, second.service);
    swap(first.username, second.username);
}


Password::Password() {
}


Password::Password(Password &&p) : Password() {
    swap(*this, p);
}


Password::Password(Password const& p) {
    this->length   = p.length;
    this->service  = p.service;
    this->username = p.username;
}


Password::Password(string service, string username, int length, int changes) :
    service{service},
    username{username},
    length{length},
    changes{changes} {
}


Password::~Password() {
}


string
Password::generate(string const& key) const {
    string slt = this->service + this->username;

    /* Generate a key using PBKDF2-HMAC-SHA256. Passwords are drawn from the
     * resulting key data. */
    uint8_t hash[128];
    pbkdf2_hmac_sha1(
        key.length(),
        (uint8_t const *)key.c_str(),
        1000 + this->changes,
        slt.length(),
        (uint8_t const *)slt.c_str(),
        128,
        hash
    );

    return (*this->encoder)(hash, 128).substr(0, this->length);
}


void
Password::set_encoder(Encoder e) {
    this->encoder = e;
}
