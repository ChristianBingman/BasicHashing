/*hashmap.h*/

//
// Christian Bingman
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing
//
// References: original non-collision code written by Prof. Hummel (UIC)
//
// hashmap implements a hash table assuming a perfect hash function;
// the class is not designed to handle collisions.  The class provides
// the underlying hash table along with insert() and search() functions;
// the user of this class must provided a Hash(key, N) function that
// hashes the given key and returns an integer in the range 0..N-1,
// inclusive, if the key is valid.  If the key is invalid, the Hash()
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
//

#pragma once

#include <iostream>
#include <functional>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
  struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;

    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N;   // capacity of hash table (# of locations or array size)

public:
  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
    this->HT = new HashEntry[N];
  }
  
  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }

  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }
  //
  // Copy Constructor
  //
  // Empties the current hashtable then copies the data from the other
  // hashtable
  //
    hashmap(const hashmap& other){
        this->HT = new HashEntry[other.N];
        
        this->N = other.N;
        for (int i = 0; i < other.N; i++) {
            this->HT[i].ContainsData = other.HT[i].ContainsData;
            this->HT[i].Key = other.HT[i].Key;
            this->HT[i].Value = other.HT[i].Value;
        }
        
    }
    
  //
  // Equals Operator
  //
  // Calls the copy constructor
  //
    hashmap& operator=(const hashmap& other){
        delete[] this->HT;
        this->HT = new HashEntry[other.N];
        
        this->N = other.N;
        for (int i = 0; i < other.N; i++) {
            this->HT[i].ContainsData = other.HT[i].ContainsData;
            this->HT[i].Key = other.HT[i].Key;
            this->HT[i].Value = other.HT[i].Value;
        }
        return *this;
    }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, probing for
  // empty space if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
      int index = Hash(key, N);
      int bucketsProbed = 0;
      

    //
    // insert:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
    else  // valid array index, store into hash table:
    {
        while (bucketsProbed < N) {
            if (!HT[index].ContainsData || HT[index].Key == key) {
                HT[index].ContainsData = true;
                HT[index].Key = key;
                HT[index].Value = value;
                return true;
            }
            
            index = (index + 1) % N;
            
            bucketsProbed++;
        }
      return true;
    }
  }

  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash
  // function returned an invalid value (e.g. due to an improperly
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, N);
      int bucketsProbed = 0;

    //
    // search:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
    else  // Probe for the hash
    {
        while(HT[index].ContainsData && bucketsProbed < N){
            if(HT[index].Key == key){
                value = HT[index].Value;
                return true;
            }
            
            index = (index + 1) % N;
            
            bucketsProbed++;
        }
        return false;
    }
  }

};
