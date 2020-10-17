#ifndef Prime_h
#define Prime_h

#include <stdbool.h>

bool isPrime(int number) {
  // Eliminate 0 and 1
  if (number <= 1) return false;
  // Eliminate all even numbers
  if (number % 2 == 0 && number > 2) return false;
  // Starting with 3, upon each odd number check for primality
  for(int i = 3; i < number / 2; i += 2)
    if (number % i == 0) return false;
  return true;
}

int nextPrime(int number) {
  int next_prime = number;
  bool found_prime = false;
  do {
    found_prime = isPrime(next_prime);
    ++next_prime;
  } while (!found_prime);
  return next_prime;
}

#endif
