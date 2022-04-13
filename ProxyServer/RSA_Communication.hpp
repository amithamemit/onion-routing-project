#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <unistd.h>

#define PRIVATE_KEY_FILE "private.pem"
#define PUBLIC_KEY_FILE "public.pem"
#define BITS_SIZE 2048

class RSA_Communication{
    public:
        RSA_Communication(std::string);
        ~RSA_Communication();

        std::string getMyPublicKey() const;

        unsigned char* encrypt(unsigned char* message) const;
        unsigned char* decrypt(unsigned char* message) const;

        static void generateKeys();



    private:
        std::string getPrivateKeyFromFile() const;
        std::string getPublicKeyFromFile() const;

        std::string clientPublicKey;
        std::string m_myPrivateKey;
        std::string m_myPublicKey;
        RSA* m_rsa;
};