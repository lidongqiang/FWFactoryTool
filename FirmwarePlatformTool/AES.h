#pragma once
#include <bitset>
#include <utility>

// AES
static const int KEY_SIZE = 16;    //    ��Կ����Ϊ128λ
static const int N_ROUND = 11;
class AES : public CWnd
{
	DECLARE_DYNAMIC(AES)

protected:
	DECLARE_MESSAGE_MAP()
public:
//    static const int KEY_SIZE = 16;    //    ��Կ����Ϊ128λ
//    static const int N_ROUND = 11;
    unsigned char plainText[16];    //    ����
    unsigned char state[16];    //    ��ǰ���顣
    unsigned char cipherKey[16];    //    ��Կ
    unsigned char roundKey[N_ROUND][16];    //����Կ
    unsigned char cipherText[16];    //����
    unsigned char SBox[16][16];    //    S��
    unsigned char InvSBox[16][16];    //    ��S��    
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


