#pragma once

#include <iostream>
#include <vector>

using namespace std;



/*	HMAC			Hashed Message Authenfication Code
 *	data			Donn�es � hacher
 *	hashFunction	Fonction de hachage utilis�e
 *	hashSize		Taille du bloc retourn� par la fonction de hachage (SHA256: 64,...)
 *	secreteKey		Clef utilis�e pour authentifi� les donn�es
 */
vector<uint8_t> HMAC(vector<uint8_t> data, vector<uint8_t> hashFunction(vector<uint8_t>), int blckSize, vector<uint8_t> secreteKey) {

	/*
	HMAC(m, k) = hash( (K ^ opad) || hash( (k ^ ipad) || m ) )
	m, donn�es,					^	XOR
	k, clef secr�te				||	Concat�nation
	opad, 0x5c5c5c5c...
	ipad, 0x36363636...
	*/


	//1) Padding sur la clef, on la compl�te de 0, afin qu'elle soit un multiple de la taille de bloc
	//Si la clef est plus grande que le bloc, alors on la hash pr�alablement
	if (secreteKey.size() > blckSize)
		secreteKey = hashFunction(secreteKey);

	while (secreteKey.size() % blckSize)
		secreteKey.push_back(0);


	//2) k ^ ipad	-> secreteKey
	for (int i = 0; i < secreteKey.size(); i++)
		secreteKey[i] ^= 0x36;

	//3) (k ^ ipad) || m ) -> m
	data.insert(data.begin(), secreteKey.begin(), secreteKey.end());

	//4)  hash( (k ^ ipad) || m )
	vector<uint8_t> firstHash = hashFunction(data);

	//5) K ^ 0x5c5c5c
	//Nous avons d�j� k ^ 0x363636... dans secreteKey, en faisant donc 
	//secreteKey ^ (0x3636... ^ 0x5c5c...) = secreteKey ^ 0x6a6a6a...

	for (int i = 0; i < secreteKey.size(); i++)
		secreteKey[i] ^= 0x5c ^ 0x36;

	secreteKey.insert(secreteKey.end(), firstHash.begin(), firstHash.end());

	return hashFunction(secreteKey);
}
