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
#define BITS_SIZE 1024

class RSA_Communication{
    public:
        RSA_Communication(std::string, int);
        RSA_Communication(int);
        ~RSA_Communication();

        std::string getMyPublicKey() const;

        unsigned char* encrypt(unsigned char* message) const;
        unsigned char* decrypt(unsigned char* message) const;

        static void generateKeys(int);



    private:
        std::string getPrivateKeyFromFile() const;
        std::string getPublicKeyFromFile() const;

        std::string m_decryptPublicKey; //destination public key
        std::string m_myPrivateKey; //relay's private key
        std::string m_myPublicKey; //relay's public key
        RSA* m_rsa; //rsa variable to encrypt and decrypt
        int port; //port of target in the conversation
};