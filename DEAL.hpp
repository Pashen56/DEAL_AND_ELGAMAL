//
//  DEAL.hpp
//  DEAL_and_ELGAMAL
//
//  Created by Murad Daudov on 30.10.2023.
//

#ifndef DEAL_hpp
#define DEAL_hpp

#include <stdio.h>

//
//#include "DEAL.hpp"
#include <fstream>
#include <iostream>

enum class Mode { ENCRYPT, DECRYPT };
enum class CipherMode { ECB, CBC, PCBC, CFB, OFB, CTR, RandomDelta };
static bool SubKey[16][48];
static bool SubKeyDeal[8][64];
const int countbits = 8, dkey = 8, countOUT = 32, countIN = 48, kilength = 48, inLength = 32, keyl = 64, blength = 64, halflength = 32, dblocklength = 128;
const int snum = 8, srow = 4, scol = 16;

const static char IP[64] = {
        58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};

const static char IPR[64] = {
        40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25
};

static const char E[48] = {
        32,1,2,3,4,5,4,5,6,7,8,9,
        8,9,10,11,12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,22,23,24,25,
        24,25,26,27,28,29,28,29,30,31,32,1
};

const static char P[32] = {
        16,7,20,21,
        29,12,28,17,
        1,15,23,26,
        5,18,31,10,
        2,8,24,14,
        32,27,3,9,
        19,13,30,6,
        22,11,4,25
};

const static char S_BLOCK[8][4][16] = {
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
    0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
    4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
    15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,

    15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
    3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
    0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
    13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,

    10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
    13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
    13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
    1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,

    7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
    13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
    10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
    3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,

    2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
    14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
    4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
    11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,

    12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
    10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
    9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
    4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,

    4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
    13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
    1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
    6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,

    13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
    1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
    7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
    2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

const static char PC1[56] = {
        57,49,41,33,25,17,9,1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,21,13,5,28,20,12,4
};

const static char PC2[48] = {
        14,17,11,24,1,5,3,28,15,6,21,10,
        23,19,12,4,26,8,16,7,27,20,13,2,
        41,52,31,37,47,55,30,40,51,45,33,48,
        44,49,39,56,34,53,46,42,50,36,29,32
};

const static char LOOP[16] = {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};



void CreateFile(std::string filename, const char* bytes, int size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(bytes, size);
        file.close();
    }
    else {
        throw "File output error!";
    }
}

void XOR(bool* InA, const bool* InB, int len) {
    for (int i = 0; i < len; i++) {
        InA[i] ^= InB[i];
    }
}

void CShift(bool* In, int len, int loop) {
    bool* Tmp = new bool[len];
    memcpy(Tmp, In, loop);
    memcpy(In, In + loop, len - loop);
    memcpy(In + len - loop, Tmp, loop);
    delete[] Tmp;
}

void Permutation(bool* Out, bool* In, const char* Table, int len) {
    bool* Tmp = new bool[len];
    for (int i = 0; i < len; i++) {
        Tmp[i] = In[Table[i] - 1];
    }
    memcpy(Out, Tmp, len);
    delete[] Tmp;
}

void ByteToBit(bool* Out, const char* In, int bits) {
    for (int i = 0; i < bits; i++) {
        Out[i] = (In[i / 8] >> (i % 8)) & 1;
    }
}

void BitToByte(char* Out, const bool* In, int bits) {
    memset(Out, 0, (bits + 7) / 8);
    for (int i = 0; i < bits; i++) {
        Out[i / 8] |= In[i] << (i % 8);
    }
}

void Substitution(bool Out[countOUT], const bool In[countIN], const char S_box[snum][srow][scol]) {
    for (char i = 0, j, k; i < 8; i++, In += 6, Out += 4) {
        j = (In[0] << 1) + In[5];
        k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
        ByteToBit(Out, &S_box[i][j][k], 4);
    }
}



class Key {
public:
    virtual void SetKey(const char Key[dkey], char* KeyDeal, int keylength) = 0;
};

class SubKeys final : public Key {
public:
    void SetKey(const char Key[dkey], char* Keydeal = nullptr, int keylength = 0) override {

        static bool K[keyl], * KL = K, * KR = K + 28;
        ByteToBit(K, Key, keyl);
        Permutation(K, K, PC1, 56);
        for (int i = 0; i < 16; i++) {
            CShift(KL, 28, LOOP[i]);
            CShift(KR, 28, LOOP[i]);
            Permutation(SubKey[i], K, PC2, 48);
        }
    }

    virtual ~SubKeys() {};
};



class Conversion {
public:
    virtual void F(bool In[inLength], const bool Ki[kilength]) = 0;
};

class EncryptConversion final : public Conversion {
public:
    void F(bool In[inLength], const bool Ki[kilength]) override {
        static bool MR[kilength];
        Permutation(MR, In, E, kilength);
        XOR(MR, Ki, kilength);
        Substitution(In, MR, S_BLOCK);
        Permutation(In, In, P, inLength);
    }

    virtual ~EncryptConversion() {}
};



class EncryptDecrypt {
public:
    virtual void Run(char Out[countbits], char In[countbits], Mode, int keylength, int round) = 0;
};

class DES final : public EncryptDecrypt {
public:
    void Run(char Out[countbits], char In[countbits], Mode mode, int keylength = 0, int round = 0) override {

        int size = sizeof(In);
        int paddingBytes = size % 8;
        if (paddingBytes != 0) {
            for (int i = 0; i < paddingBytes; i++) {
                Out[size + i] = static_cast<char>(paddingBytes);
            }
            size += paddingBytes;
        }
        static bool M[blength], Tmp[halflength], * Li = M, * Ri = M + halflength;
        EncryptConversion Round;
        ByteToBit(M, In, 64);
        Permutation(M, M, IP, 64);

        if (mode == Mode::DECRYPT) { Li = M + halflength, Ri = M; }
        for (int i = 0, j = 15; i < 16; i++, j--) {
            memcpy(Tmp, Ri, 32);
            if (keylength == 128 || keylength == 192 || keylength == 256) {
                Round.F(Ri, SubKeyDeal[round]);
            }
            else {
                if (mode == Mode::ENCRYPT) { Round.F(Ri, SubKey[i]); }
                else if (mode == Mode::DECRYPT) { Round.F(Ri, SubKey[j]); }
            }
            XOR(Ri, Li, 32);
            memcpy(Li, Tmp, 32);
        }
        if (mode == Mode::DECRYPT) { size -= static_cast<int>(Out[size - 1]); }

        Permutation(M, M, IPR, 64);
        BitToByte(Out, M, 64);
    }

    virtual ~DES() {}
};



class DEAL final : public EncryptDecrypt {
public:
    void Run(char Out[countbits], char In[countbits], Mode mode, int keylength, int round = 0) override {

        static bool M[dblocklength], Tmp[blength];
        int numberofrounds = 0;
        ByteToBit(M, In, dblocklength);
        bool* Li = M, * Ri = M + blength;
        char p[blength];

        if (keylength == 128 || keylength == 192) { numberofrounds = 6; }
        else if (keylength == 256) { numberofrounds = 8; }

        if (mode == Mode::DECRYPT) { Li = M + blength, Ri = M; }

        for (int i = 0, j = numberofrounds - 1; i < numberofrounds; i++, j--) {
            memcpy(Tmp, Ri, 64);
            DES f;
            BitToByte(p, Ri, 64);
            if (mode == Mode::ENCRYPT) { f.Run(p, p, Mode::ENCRYPT, 128, i); }
            else if (mode == Mode::DECRYPT) { f.Run(p, p, Mode::ENCRYPT, 128, j); }
            ByteToBit(Ri, p, 64);
            XOR(Li, Ri, 64);
            memcpy(Ri, Li, 64);
            memcpy(Li, Tmp, 64);
        }

        BitToByte(Out, M, 128);
    }

    virtual ~DEAL() {}
};

void GetRoundKeyDeal(int numberofround, char* K, int i = 0) {
    char cTmp[blength];
    bool bTmp[blength], I[blength];
    std::fill_n(I, 64, false);
    DES f;

    if (numberofround != 1) {
        memcpy(bTmp, K, 64);
        if (i != 0) {
            I[i] = 1;
            XOR(I, bTmp, 64);
            XOR(I, SubKeyDeal[numberofround - 2], 64);
            BitToByte(cTmp, I, 64);
        }
        else {
            XOR(bTmp, SubKeyDeal[numberofround - 2], 64);
            BitToByte(cTmp, bTmp, 64);
        }
        f.Run(cTmp, cTmp, Mode::ENCRYPT);
    }
    else {
        f.Run(cTmp, K, Mode::ENCRYPT);
    }
    ByteToBit(SubKeyDeal[numberofround - 1], cTmp, 64);
}

class SubKeysDeal final : public Key {
public:
    void SetKey(const char Key[dkey], char* Keydeal, int keylength) override {

        if (keylength == 128) {
            static char* K1 = Keydeal, * K2 = Keydeal + 64;
            GetRoundKeyDeal(1, K1);
            GetRoundKeyDeal(2, K2);
            GetRoundKeyDeal(3, K1, 1);
            GetRoundKeyDeal(4, K2, 2);
            GetRoundKeyDeal(5, K1, 4);
            GetRoundKeyDeal(6, K2, 8);
        }
        else if (keylength == 192) {
            static char* K1 = Keydeal, * K2 = Keydeal + 64, * K3 = Keydeal + 128;
            GetRoundKeyDeal(1, K1);
            GetRoundKeyDeal(2, K2);
            GetRoundKeyDeal(3, K3);
            GetRoundKeyDeal(4, K1, 1);
            GetRoundKeyDeal(5, K2, 2);
            GetRoundKeyDeal(6, K3, 4);
        }
        else if (keylength == 256) {
            static char* K1 = Keydeal, * K2 = Keydeal + 64, * K3 = Keydeal + 128, * K4 = Keydeal + 192;
            GetRoundKeyDeal(1, K1);
            GetRoundKeyDeal(2, K2);
            GetRoundKeyDeal(3, K3);
            GetRoundKeyDeal(4, K4);
            GetRoundKeyDeal(5, K1, 1);
            GetRoundKeyDeal(6, K2, 2);
            GetRoundKeyDeal(7, K3, 4);
            GetRoundKeyDeal(8, K4, 8);
        }

    }

    virtual ~SubKeysDeal() {};
};


void Encryption(const char* filename, int keylength) {

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw "File input error!";
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string fsname = filename;

    char* buffer = new char[size];

    file.read(buffer, size);
    file.close();

    DEAL deal;

    deal.Run(buffer, buffer, Mode::ENCRYPT, keylength);
    CreateFile("Encrypted_" + fsname, buffer, size);
    std::cout << filename << " encrypted" << std::endl;

    deal.Run(buffer, buffer, Mode::DECRYPT, keylength);
    CreateFile("Decrypted_" + fsname, buffer, size);
    std::cout << filename << " decrypted" << std::endl;

    delete[] buffer;
}


#endif /* DEAL_hpp */
