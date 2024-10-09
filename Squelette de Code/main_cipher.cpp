#include <iostream>
#include <string>
using namespace std;

class Vigenere
{
public:
  string key;
 
  Vigenere(string key)
  {
    // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
    for(unsigned int i = 0; i < key.size(); ++i)
    {
      if(key[i] >= 'A' && key[i] <= 'Z')
        this->key += key[i];
      else if(key[i] >= 'a' && key[i] <= 'z')
        this->key += key[i] + 'A' - 'a';
    }
  }
 
  string encrypt(string text)
    {
        string out;
        unsigned int keyIndex = 0; // Pour garder la trace de la position dans la clé

        for (unsigned int i = 0; i < text.length(); ++i)
        {
            char currentChar = text[i];

            if (currentChar >= 'A' && currentChar <= 'Z')
            {
                out += (currentChar - 'A' + key[keyIndex] - 'A') % 26 + 'A'; // Chiffrement pour les majuscules
                keyIndex = (keyIndex + 1) % key.length(); // Passe à la lettre suivante de la clé
            }
            else if (currentChar >= 'a' && currentChar <= 'z')
            {
                out += (currentChar - 'a' + key[keyIndex] - 'A') % 26 + 'a'; // Chiffrement pour les minuscules
                keyIndex = (keyIndex + 1) % key.length(); // Passe à la lettre suivante de la clé
            }
            else
            {
                out += currentChar; // Ne change pas les caractères non alphabétiques
            }
        }

        return out;
    }

  string decrypt(string text)
{
    string out;
    unsigned int keyIndex = 0; // Pour garder la trace de la position dans la clé

    for (unsigned int i = 0; i < text.length(); ++i)
    {
        char currentChar = text[i];

        if (currentChar >= 'A' && currentChar <= 'Z')
        {
            out += (currentChar - 'A' - (key[keyIndex] - 'A') + 26) % 26 + 'A'; // Déchiffrement pour les majuscules
            keyIndex = (keyIndex + 1) % key.length(); // Passe à la lettre suivante de la clé
        }
        else if (currentChar >= 'a' && currentChar <= 'z')
        {
            out += (currentChar - 'a' - (key[keyIndex] - 'A') + 26) % 26 + 'a'; // Déchiffrement pour les minuscules
            keyIndex = (keyIndex + 1) % key.length(); // Passe à la lettre suivante de la clé
        }
        else
        {
            out += currentChar; // Ne change pas les caractères non alphabétiques
            // Ne pas incrémenter keyIndex ici
        }
    }

    return out;
}

};


// Fonction utilitaire pour trouver l'indice d'une lettre (A=0, B=1, ..., Z=25)
int char_to_index(char c) {
    return c - 'A';
}


// Fonction utilitaire pour convertir un index en caractère
char index_to_char(int index) {
    return 'A' + (index % ALPHABET_SIZE);
}


//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
  Vigenere cipher("MYKEY");
 
  string original_en  = "Kerckhoffs's principle - A cryptosystem should be secure even if everything about the system, except the key, is public knowledge.";
  string encrypted_en = cipher.encrypt(original_en);
  string decrypted_en = cipher.decrypt(encrypted_en);
 
  cout << original_en << endl;
  cout << "Encrypted: " << encrypted_en << endl;
  cout << "Decrypted: " << decrypted_en << endl;

  string original_fr  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";

  string encrypted_fr = cipher.encrypt(original_fr);
  string decrypted_fr = cipher.decrypt(encrypted_fr);
 
  //cout << original_fr << endl;
  //cout << "Encrypted: " << encrypted_fr << endl;
  //cout << "Decrypted: " << decrypted_fr << endl;

}

