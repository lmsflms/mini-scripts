#include <iostream>
#include <vector>

//Returns the modular exponentiation : base^exponent % modulo, based on https://fr.wikipedia.org/wiki/Exponentiation_modulaire
unsigned long long modular_exponentiation(unsigned long long base, unsigned long long exponent, unsigned long long modulo) {
    unsigned long long result = 1;
    //while there are still bits in the exponent
    while (exponent > 0) {
        //we check if the lsb of the exponent is 1
        if ((exponent & 1) > 0) {
            //if it's 1, we multiply result by the current base
            result = (result * base) % modulo;
        }
        //we move the exponent by 1 bit to the right
        exponent = exponent >> 1;
        //and we calculate the square of the base for the next exponent
        base = (base * base) % modulo;
    }
    return result;
}

int diffie_hellman(int private_key, int public_key, int modulo) {
    return modular_exponentiation(public_key, private_key, modulo);
}

int main()
{
    std::cout << "============== Diffie-Hellman protocol implementation ==============\n";
    //We define a base. It should produce as many remainder values for the shared secret key
    unsigned long long base = 123456789; 

    //We also define a modulo that should be a big prime number.
    unsigned long long modulo = 999999937;

    //The private keys of Alice and Bob (must be secret!) :
    unsigned long long alice_key = 987654321;
    unsigned long long bob_key = 123456789;

    //Public exchange step : We calculate the public keys by calculating the base^private_key % modulo
    unsigned long long base_alice_key_modulo = modular_exponentiation(base, alice_key, modulo);
    unsigned long long base_bob_key_modulo = modular_exponentiation(base, bob_key, modulo);

    //Then we use Diffie-Hellman to get their secret. 
    unsigned long long secret_alice = diffie_hellman(alice_key, base_bob_key_modulo, modulo);
    unsigned long long secret_bob = diffie_hellman(bob_key, base_alice_key_modulo, modulo);

    //Both should be the same
    std::cout << "ALICE: " << secret_alice << "\nBOB: " << secret_bob << "\n====================================================================\n";
}