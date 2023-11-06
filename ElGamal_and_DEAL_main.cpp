#include <iostream>
#include "largeClass.h"
#include "stdlib.h"
#include "time.h"
#include <string>
#include <fstream>
#include "DEAL.hpp"
#include "ElGamal.h"

using namespace std;

int main()
{
    int bitlen = 300;

    largeClass p = GenPrime(bitlen);
    largeClass g = GenRandomByRangeC(p);
    largeClass x = GenRandomByRangeC(p);
    largeClass y = powModClass(g, x, p);

    largeClass m, a, b;
    m.readFromBinFile("file.txt");
    encrypt(m, p, g, y, a, b);

    ofstream ofst("encrypted_file.txt");
    ofst << a.str() << endl << b.str();
    ofst.close();

    ifstream ifst("encrypted_file.txt");
    string a_str, b_str;
    getline(ifst, a_str);
    getline(ifst, b_str);

    a = largeClass(a_str.c_str());
    b = largeClass(b_str.c_str());
    m = decrypt(p, x, a, b);

    m.writeToBinFile("decrypted_file.txt");



    //DEAL

    char key[8] = { 2, 2, 1, 0, 9, 3, 1, 4 };
    SubKeys K;
    K.SetKey(key);

    //char key2[128] = { 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4 };
    //char key2[192] = { 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4 };
    char key2[256] = { 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4, 2, 2, 1, 0, 9, 3, 1, 4 };
    int keylength = 256;
    SubKeysDeal K2;
    K2.SetKey(nullptr, key2, keylength);

    try {
            Encryption("sound.wav", keylength);
        }
        catch (const char* errormsg) {
            std::cout << errormsg << std::endl;
        }

    try {
            Encryption("img.jpg", keylength);
        }
        catch (const char* errormsg) {
            std::cout << errormsg << std::endl;
        }

    try {
            Encryption("text.txt", keylength);
        }
        catch (const char* errormsg) {
            std::cout << errormsg << std::endl;
        }

    try {
            Encryption("video.mp4", keylength);
        }
        catch (const char* errormsg) {
            std::cout << errormsg << std::endl;
        }


    return 0;
}

