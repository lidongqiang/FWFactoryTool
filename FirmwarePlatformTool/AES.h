#pragma once
#include <bitset>
#include <utility>

// AES
static const int KEY_SIZE = 16;    //    密钥长度为128位
static const int N_ROUND = 11;
class AES : public CWnd
{
	DECLARE_DYNAMIC(AES)

protected:
	DECLARE_MESSAGE_MAP()
public:
//    static const int KEY_SIZE = 16;    //    密钥长度为128位
//    static const int N_ROUND = 11;
    unsigned char plainText[16];    //    明文
    unsigned char state[16];    //    当前分组。
    unsigned char cipherKey[16];    //    密钥
    unsigned char roundKey[N_ROUND][16];    //轮密钥
    unsigned char cipherText[16];    //密文
    unsigned char SBox[16][16];    //    S盒
    unsigned char InvSBox[16][16];    //    逆S盒    
    void EncryptionProcess();    
    void DecryptionProcess();
    void Round(const int& round);
    void InvRound(const int& round);
    void FinalRound();
    void InvFinalRound();
    void KeyExpansion();
    void AddRoundKey(const int& round);    
    void SubBytes();    
    void InvSubBytes();
    void ShiftRows();    
    void InvShiftRows();
    void Mixcolumns();    
    void InvMixcolumns();
    void BuildSBox();
    void BuildInvSBox();
    void InitialState(const unsigned char* text);
    void InitialcipherText();    
    void InitialplainText();        
    unsigned char GFMultplyByte(const unsigned char& left, const unsigned char& right);
    const unsigned char* GFMultplyBytesMatrix(const unsigned char* left, const unsigned char* right);
	void swap(unsigned char* c1, unsigned char* c2);
public:
	AES();
	virtual ~AES();

    const unsigned char* cipher(const unsigned char* text, const unsigned char* key, const int& keySize);    
    const unsigned char* Invcipher(const unsigned char* text, const unsigned char* key, const int& keySize);
};


