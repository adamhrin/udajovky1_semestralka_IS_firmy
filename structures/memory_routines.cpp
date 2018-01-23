#include "memory_routines.h"
#include "ds_routines.h"

namespace structures {
	byte & MemoryRoutines::byteSet(byte & B)
	{
		B = 255;
		return B;
	}
	byte & MemoryRoutines::byteReset(byte & B)
	{
		B = 0;
		return B;
	}
	byte & MemoryRoutines::byteXOR(byte & B)
	{
		B ^= MAX_BYTE;
		return B;
	}
	byte & MemoryRoutines::byteSHL(byte & B)
	{
		B = B << 1;
		return B;
	}
	byte & MemoryRoutines::byteSHR(byte & B)
	{
		B = B >> 1;
		return B;
	}
	bool MemoryRoutines::byteNthBitGet(byte & B, int n)
	{
		DSRoutines::rangeCheckExcept(n, 8, "Invalid n!"); //priklad, nech n=3
		byte copyOfB = B;							      //copyOfB = 1010[0]111
		copyOfB >>= n;									  //10100111=>0001010[0] 
		return (copyOfB & 1);							  //		& 000000  1 = 0 (keby bolo na ntom mieste 1, AND vyhodi 1)
	}

	byte & MemoryRoutines::byteNthBitTo0(byte & B, int n)
	{
		DSRoutines::rangeCheckExcept(n, 8, "Invalid n!"); //priklad, nech  B = 10111010, n=4
		byte newB = 1;									  //newB      = 00000001
		newB <<= n;										  //shiftnute = 00010000
		newB ^= 255;									  //xornute   = 11101111											
		B &= newB;										  //(B & newB)= 10101010
		return B;
	}

	byte & MemoryRoutines::byteNthBitTo1(byte & B, int n)
	{
		DSRoutines::rangeCheckExcept(n, 8, "Invalid n!"); //priklad, nech  B = 10011010, nech n=5
		byte newB = 1;								      //newB      = 00000001
		newB <<= n;									      //shiftnute = 00100000
		B |= newB;										  //(B|=newB) = 10111010
		return B;
	}

	byte & MemoryRoutines::byteNthBitToggle(byte & B, int n)
	{
		DSRoutines::rangeCheckExcept(n, 8, "Invalid n!"); //priklad, nech B = 11111111, nech n=2
		byte newB = 1;									  //newB      = 00000001
		newB <<= n;										  //shiftnute = 00000100
		B ^= newB;										  //(B^=newB) = 11111011
		return B;
	}

	MemoryRoutines::MemoryRoutines()
	{
	}

}
