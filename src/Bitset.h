#ifndef BITSET_H
#define BITSET_H

#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

const int ULL_SIZE = sizeof(unsigned long long);
const int ULL_BITS = ULL_SIZE * 8;

class Bitset
{
	public:
		
		unsigned long long *bits;
		int size;
		int n_bits;

		Bitset(int n_bits=1)
		{
			this->n_bits = n_bits;
			this->size = (n_bits / ULL_BITS);
			if( n_bits % ULL_BITS > 0) this->size++;
			this->bits = new unsigned long long[size];
			memset(this->bits, 0, ULL_SIZE * size);
		}

		~Bitset()
		{
			delete[] bits;
		}

		void set()
		{
			for (int i = 0; i < this->size; i++){
				this->bits[i] = ~0ULL;
			}
		}

		void set( int pos)
		{
			this->bits[pos/ULL_BITS] |= 1ULL << pos%ULL_BITS;
		}

		void reset()
		{
			for (int i = 0; i < this->size; i++){
				this->bits[i] = 0ULL;
			}	
		}

		Bitset& operator=(const Bitset& other) // copy assignment
		{
			if (this != &other) { // self-assignment check expected
				if (other.size != this->size)
				{ 
					delete[] this->bits;            // destroy storage in this
					/* reset size to zero and mArray to null, in case allocation throws */
					this->bits = new unsigned long long[other.size]; // create storage in this
				}
				this->size = other.size;
				this->n_bits = other.n_bits;
				memcpy(this->bits, other.bits, ULL_SIZE*this->size);
				/* copy data from other's storage to this storage */
			}
			return *this;
		}

		Bitset operator<< (const int &val)
		{

			// TODO: reuso de codigo
			Bitset ret(this->n_bits);

			if (val  >= this->n_bits){
				ret.reset();
				return ret;
			}

			memcpy(ret.bits, this->bits, ULL_SIZE*this->size);
			
			// great shift
			if (val > ULL_BITS){
				int jump = val / ULL_BITS;

				for (int i = ret.size-1; i >= jump; i--){
					ret.bits[i] = this->bits[i-jump];
					ret.bits[i-jump] = 0ULL;
				}
			}

			// small shift
			int shift_val = val % ULL_BITS;

			if (shift_val == 0){
				return ret;
			}

			for (int i = ret.size-1; i >= 1; i--){
				ret.bits[i] <<= shift_val;
				ret.bits[i] |= ret.bits[i-1] >> (ULL_BITS - shift_val);
			}
			ret.bits[0] <<= shift_val;

			return ret;
		}

		Bitset operator| (const Bitset &arg)
		{
			Bitset ret(this->n_bits);
			memcpy(ret.bits, this->bits, ULL_SIZE*this->size);

			int min_size = min(ret.size, arg.size);

			for (int i = 0; i < min_size; i++){
				ret.bits[i] |= arg.bits[i];
			}

			return ret;
		}

		Bitset operator& (const Bitset &arg)
		{
			Bitset ret(this->n_bits);
			memcpy(ret.bits, this->bits, ULL_SIZE*this->size);

			int min_size = min(ret.size, arg.size);

			for (int i = 0; i < min_size; i++){
				ret.bits[i] &= arg.bits[i];
			}

			return ret;
		}

		Bitset operator~ ()
		{
			Bitset ret(this->n_bits);

			for (int i = 0; i < ret.size; i++){
				ret.bits[i] = ~(this->bits[i]);
			}

			return ret;
		}

		string toString()
		{
			string ret;
			for(int i =  0; i < this->size; i++)
			{
				unsigned long long temp = this->bits[i];
				for(int j  = 0; j < ULL_BITS; j++)
				{
					ret += '0' + (temp & 1ULL);
					temp >>= 1;
				}
			}

			string str = ret.substr(0, this->n_bits);
			reverse(str.begin(), str.end());
			return str;
		}
		bool operator< (const Bitset &arg)
		{
			Bitset ret;
			ret = (*this);

			ret.bits[(this->size)-1] &= (1ULL << (this->n_bits%ULL_BITS)) - 1ULL;
			arg.bits[(arg.size)-1] &= (1ULL << (arg.n_bits%ULL_BITS)) - 1ULL;

			int max_size = max(ret.size, arg.size);

			for (int i = max_size-1; i >= 0; i--){
				unsigned long long a, b;
				a = i >= ret.size ? 0 : ret.bits[i];
				b = i >= arg.size ? 0 : arg.bits[i];

				if (a < b) return true;
				else if( a > b) return false;  
			}

			return false;
		}

};

#endif