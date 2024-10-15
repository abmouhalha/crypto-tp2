#include <iostream>
#include <fstream>  // Bibliothèque pour gérer les fichiers
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
using namespace std;

class Vigenere
{
public:
    string key;

    Vigenere(string key)
    {
        // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
        for (unsigned int i = 0; i < key.size(); ++i)
        {
            if (key[i] >= 'A' && key[i] <= 'Z')
                this->key += key[i];
            else if (key[i] >= 'a' && key[i] <= 'z')
                this->key += key[i] + 'A' - 'a';
        }
    }

    string encrypt(string text)
    {
        string out;
        unsigned int keyIndex = 0;

        for (unsigned int i = 0; i < text.length(); ++i)
        {
            char currentChar = text[i];

            if (currentChar >= 'A' && currentChar <= 'Z')
            {
                out += (currentChar - 'A' + key[keyIndex] - 'A') % 26 + 'A';
                keyIndex = (keyIndex + 1) % key.length();
            }
            else if (currentChar >= 'a' && currentChar <= 'z')
            {
                out += (currentChar - 'a' + key[keyIndex] - 'A') % 26 + 'a';
                keyIndex = (keyIndex + 1) % key.length();
            }
            else
            {
                out += currentChar;
            }
        }

        return out;
    }

    string decrypt(string text)
    {
        string out;
        unsigned int keyIndex = 0;

        for (unsigned int i = 0; i < text.length(); ++i)
        {
            char currentChar = text[i];

            if (currentChar >= 'A' && currentChar <= 'Z')
            {
                out += (currentChar - 'A' - (key[keyIndex] - 'A') + 26) % 26 + 'A';
                keyIndex = (keyIndex + 1) % key.length();
            }
            else if (currentChar >= 'a' && currentChar <= 'z')
            {
                out += (currentChar - 'a' - (key[keyIndex] - 'A') + 26) % 26 + 'a';
                keyIndex = (keyIndex + 1) % key.length();
            }
            else
            {
                out += currentChar;
            }
        }

        return out;
    }

    // Cryptanalyse Vigenère
    string cryptanalyse(string ciphertext)
    {
        string filtered_text;
        for (char c : ciphertext)
        {
            if (c >= 'A' && c <= 'Z')
                filtered_text += c;
            else if (c >= 'a' && c <= 'z')
                filtered_text += c + 'A' - 'a';
        }

        int keyLength = findKeyLength(filtered_text);
        string estimatedKey = findKey(filtered_text, keyLength);

        cout << "Longueur estimée de la clé : " << keyLength << endl;
        cout << "Clé estimée : " << estimatedKey << endl;

        Vigenere tempCipher(estimatedKey);
        string decryptedText = tempCipher.decrypt(ciphertext);

        return decryptedText;
    }

private:
    double calculateIC(const string &text)
    {
        int counts[26] = {0};
        int total = 0;
        for (char c : text)
        {
            if (c >= 'A' && c <= 'Z')
            {
                counts[c - 'A']++;
                total++;
            }
        }

        double ic = 0.0;
        for (int i = 0; i < 26; ++i)
        {
            ic += counts[i] * (counts[i] - 1);
        }
        ic /= total * (total - 1);

        return ic;
    }

    int findKeyLength(const string &text)
    {
        int probableKeyLength = 1;
        double bestIC = 0.0;

        for (int keyLength = 1; keyLength <= 20; ++keyLength)
        {
            double averageIC = 0.0;
            for (int i = 0; i < keyLength; ++i)
            {
                string subText;
                for (unsigned int j = i; j < text.length(); j += keyLength)
                {
                    subText += text[j];
                }

                averageIC += calculateIC(subText);
            }
            averageIC /= keyLength;

            if (averageIC > bestIC)
            {
                bestIC = averageIC;
                probableKeyLength = keyLength;
            }
        }

        return probableKeyLength;
    }

    string findKey(const string &text, int keyLength)
    {
        string key;
        vector<double> englishFrequencies = {
            8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074};

        for (int i = 0; i < keyLength; ++i)
        {
            string subText;
            for (unsigned int j = i; j < text.length(); j += keyLength)
            {
                subText += text[j];
            }

            double minChiSquared = numeric_limits<double>::max();
            char bestShift = 'A';

            for (int shift = 0; shift < 26; ++shift)
            {
                int counts[26] = {0};
                for (char c : subText)
                {
                    int shifted = (c - 'A' - shift + 26) % 26;
                    counts[shifted]++;
                }

                double chiSquared = 0.0;
                int total = subText.length();
                for (int k = 0; k < 26; ++k)
                {
                    double expected = englishFrequencies[k] * total / 100.0;
                    chiSquared += pow(counts[k] - expected, 2) / expected;
                }

                if (chiSquared < minChiSquared)
                {
                    minChiSquared = chiSquared;
                    bestShift = 'A' + shift;
                }
            }

            key += bestShift;
        }

        return key;
    }
};

// Fonction pour lire un fichier
string lireFichier(const string &nomFichier)
{
    ifstream fichier(nomFichier);
    if (!fichier.is_open())
    {
        cerr << "Erreur lors de l'ouverture du fichier: " << nomFichier << endl;
        return "";
    }

    string contenu((istreambuf_iterator<char>(fichier)), istreambuf_iterator<char>());
    fichier.close();
    return contenu;
}

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
    // Analyser plusieurs fichiers de ciphertext
    vector<string> fichiers = {"ciphertext_1.txt", "ciphertext_2.txt", "ciphertext_3.txt", "ciphertext_4.txt"};

    for (const string &nomFichier : fichiers)
    {
        cout << "Analyse du fichier: " << nomFichier << endl;
        string contenu = lireFichier(nomFichier);

        if (contenu.empty())
            continue;

        Vigenere cipher("MYKEY");
        string decrypted = cipher.cryptanalyse(contenu);
        cout << "Texte déchiffré : " << decrypted << endl;
        cout << "-------------------------------" << endl;
    }

    return 0;
}
