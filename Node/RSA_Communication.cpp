#include "RSA_Communication.hpp"


/*
* default constructor for rsa communication
* input: public key of target and port of destination
* output: none
*/
RSA_Communication::RSA_Communication(std::string decryptPublicKey, int port){
	this->generateKeys(port);
	this->port = port;
    this->m_myPublicKey = this->getPublicKeyFromFile();
    this->m_myPrivateKey = this->getPrivateKeyFromFile();
    this->m_decryptPublicKey = decryptPublicKey;

    // init the rsa object and add to him the user public key (for encrypt) and the server private key (for decrypt)
    BIO* bio = BIO_new( BIO_s_mem() );
	// adding the public key to the rsa
	BIO_write(bio, this->m_decryptPublicKey.c_str(), this->m_decryptPublicKey.length());
	this->m_rsa = PEM_read_bio_RSAPublicKey( bio, NULL, 0, 0 ) ;
    
	// adding the private key to the rsa
	//BIO_write(bio, this->m_myPrivateKey.c_str(), this->m_myPrivateKey.length());
	//PEM_read_bio_RSAPrivateKey( bio, &this->m_rsa, 0, 0 );

    // clean memory
    BIO_free(bio);
}
 
/*
* default contructor for rsa communication
* input: port of destination
* output: none
*/
RSA_Communication::RSA_Communication(int port){
	this->generateKeys(port);
	this->port = port;
	this->m_myPublicKey = this->getPublicKeyFromFile();
	this->m_myPrivateKey = this->getPrivateKeyFromFile();

	// init the rsa object and add to him the user public key (for encrypt) and the server private key (for decrypt)
    BIO* bio = BIO_new( BIO_s_mem() );
	// adding the public key to the rsa
	BIO_write(bio, this->m_myPublicKey.c_str(), this->m_myPublicKey.length());
	this->m_rsa = PEM_read_bio_RSAPublicKey( bio, NULL, 0, 0 ) ;
    
	// adding the private key to the rsa
	BIO_write(bio, this->m_myPrivateKey.c_str(), this->m_myPrivateKey.length());
	PEM_read_bio_RSAPrivateKey( bio, &this->m_rsa, 0, 0 );

	BIO_free(bio);
}

/*
* default destructor for rsa communication
* input: none
* output: none
*/
RSA_Communication::~RSA_Communication(){
    RSA_free(this->m_rsa);
}

/*
Method that return the server public key.
Input: None.
Output: the public key of the server
*/
std::string RSA_Communication::getMyPublicKey() const{
    return this->m_myPublicKey;
}

/*
Method that encrypt a message. The encryption is using the public key of the user.
Input: message to encrypt.
Output: encrypted message with the public key of the user.
*/
unsigned char* RSA_Communication::encrypt(unsigned char* message) const{
    unsigned char* encrypted = new unsigned char[RSA_size(this->m_rsa)];
    RSA_public_encrypt(RSA_size(this->m_rsa), message, encrypted, this->m_rsa, RSA_NO_PADDING);
	//std::cout << "encrypted: " << encrypted << ", decrypted: " << message << std::endl;
    return encrypted;
}

/*
Method that decrypt a message. The decryption is using the private key of the server.
Input: message to decrypt.
Output: decrypted message.
*/
unsigned char* RSA_Communication::decrypt(unsigned char* message) const{
    unsigned char* decrypted = new unsigned char[RSA_size(this->m_rsa)];
	RSA_private_decrypt(RSA_size(this->m_rsa), message, decrypted, this->m_rsa, RSA_NO_PADDING);
	//std::cout << "encrypted: " << message << ", decrypted: " << decrypted << std::endl;

    return decrypted;
}


/*
Method that read the private key file and return the private key.
Input: None.
Output: the server private key
*/
std::string RSA_Communication::getPrivateKeyFromFile() const{
	std::ifstream privateKeyFile (std::to_string(this->port) + PRIVATE_KEY_FILE);
	std::string privateKey = "";
	// reading the file content to get the private key
	while(privateKeyFile){
		std::string line = "";
        std::getline(privateKeyFile, line);
		privateKey += line + "\n";
	}
	return privateKey;
}

/*
Method that read the public key file and return the public key.
Input: None.
Output: the server public key.
*/
std::string RSA_Communication::getPublicKeyFromFile() const{
	std::ifstream publicKeyFile (std::to_string(this->port) + PUBLIC_KEY_FILE);
	std::string publicKey = "";
	// reading the file content to get the public key
	while(publicKeyFile){
		std::string line = "";
        std::getline(publicKeyFile, line);
		publicKey += line + "\n";
	}
	return publicKey;
}

/*
Method that generate keys for the server.
The method will generate a public key and a private key.
The keys will be save in 2 different files.
Input: None.
Output: None.
*/
void RSA_Communication::generateKeys(int port){
	std::string currentNodePublicKeyFile = std::to_string(port) + PUBLIC_KEY_FILE; // std::to_string(port) + PUBLIC_KEY_FILE;
	std::string currentNodePrivateKeyFile =  std::to_string(port) + PRIVATE_KEY_FILE; // std::to_string(port) + PRIVATE_KEY_FILE;

	std::ifstream privateKeyStream(currentNodePrivateKeyFile);
	std::ifstream publicKeyStream(currentNodePublicKeyFile);


    bool privateKeyFileExist = privateKeyStream.good();
    bool publicKeyFileExist = publicKeyStream.good();

    // if both of the files exist need to generate new keys.
    if (privateKeyFileExist && publicKeyFileExist){
        return;
    }

    int ret = 0;
	RSA	*rsa = NULL;
	BIGNUM *bne = NULL;
	BIO	*bp_public = NULL, *bp_private = NULL;

	int	bits = BITS_SIZE;
	unsigned long e = RSA_F4;

	std::cout << "generate keys and save for node with port " << port << std::endl;
	// 1. generate rsa key
	bne = BN_new();
	ret = BN_set_word(bne, e);
	
	if(ret != 1){
		std::cout << "Error with generating keys" << std::endl;
		goto free_all;
	}

	rsa = RSA_new();
	ret = RSA_generate_key_ex(rsa, bits, bne, NULL);

	if(ret != 1){
		std::cout << "Error with generating keys" << std::endl;
		goto free_all;
	}

	// 2. save public key
	bp_public = BIO_new_file(currentNodePublicKeyFile.c_str(), "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, rsa);
	if(ret != 1){
		std::cout << "Error with generating keys" << std::endl;
		goto free_all;
	}

	// 3. save private key
	bp_private = BIO_new_file(currentNodePrivateKeyFile.c_str(), "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);

	// 4. free
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(rsa);
	BN_free(bne);

}