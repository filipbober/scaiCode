// Extension to UAlbertaBot (University of Alberta - AIIDE StarCraft Competition)
// by David Churchill <dave.churchill@gmail.com>  
// Author: Filip C. Bober <filip.bober@gmail.com>

#pragma once

#include <stdio.h>
#include "Common.h"

#include <bitset>

///////////////////////////////////////////////////////////////////////////////
//
// BitSetExt
//
///////////////////////////////////////////////////////////////////////////////

#define __lz(a) countLeadingZeros(a)
#define __tz(a) countTrailingZeros(a)


#define S_ONE std::bitset<BITSET_SIZE>(1)
#define S_ZERO std::bitset<BITSET_SIZE>(0)
#define S_LSHIFT(VAL, N) ((VAL) << ((N-1)))
#define S_SINGLE_BIT(N) (S_LSHIFT(S_ONE, N))

#define __ONE S_ONE
#define __ZERO S_ZERO

//#define REVERSE_ACTION_ITERATOR

namespace BuildOrderSearch
{
	typedef unsigned char Action;
	const unsigned int BITSET_SIZE = 64;

	class BitSetExt
	{
		std::bitset<BITSET_SIZE> setExt;

	public:
		BitSetExt() : setExt(0) {}							// default constructor sets to zero
		BitSetExt(std::bitset<BITSET_SIZE> s) : setExt(s) {}

		// pops the next action (bit from the right)
		Action popAction()
		{
#ifdef REVERSE_ACTION_ITERATOR
			// get the number of trailing zeros
			int nextAction = (BITSET_SIZE - 1) - __lz(set);
#else
			// get the number of trailing zeros
			int nextAction = __tz(setExt);
#endif

			// set that bit to a zero
			subtract(nextAction);

			return (Action)nextAction;
		}

		// peeks at the next action
		int nextAction()
		{
#ifdef REVERSE_ACTION_ITERATOR
			// get the number of trailing zeros
			int nextAction = (BITSET_SIZE - 1) - __lz(set);
#else
			// get the number of trailing zeros
			int nextAction = __tz(setExt);
#endif

			return nextAction;
		}

		int  		operator [] 	(const int bit) 	const { return setExt[bit]; }	// get the bit at i
		BitSetExt 		operator +  	(const int bit) 	const { return BitSetExt(setExt | S_SINGLE_BIT(bit)); }	// member addition // setExt | std::bitset<BITSET_SIZ(bit  
		BitSetExt 		operator -  	(const int bit) 	const { return BitSetExt(setExt & ~S_SINGLE_BIT(bit)); }	// member subtraction
		BitSetExt 		operator |  	(const BitSetExt & a) 	const { return BitSetExt(setExt | a.setExt); }				// set union
		BitSetExt 		operator +  	(const BitSetExt & a)	const { return BitSetExt(setExt | a.setExt); }				// set union
		BitSetExt 		operator -  	(const BitSetExt & a)	const { return BitSetExt(setExt & ~a.setExt); }			// set subtraction
		BitSetExt 		operator &  	(const BitSetExt & a)	const { return BitSetExt(setExt & a.setExt); }				// set intersection
		BitSetExt 		operator ~  	()					const { return BitSetExt(~setExt); }					// set negation

		// new (std::bitset)
		bool 		isEmpty() 					const { return setExt == S_ZERO; }					// the set is all zeros
		bool 		contains(const BitSetExt a) 	const { return (setExt & a.setExt) == a.setExt; }			// completely contain another set
		bool		contains(const int bit) 	const { return setExt[bit]; }// contains a bit set to 1	
		bool		containsAny(const BitSetExt a)	const { return (setExt & ~a.setExt) != setExt; }			// does set contain any of
		bool		containsNone(const BitSetExt a)	const { return (setExt & ~a.setExt) == setExt; }			// does set contain none of 
		bool 		isSubsetOf(const BitSetExt a) 	const { return (a.setExt & setExt) == setExt; }			// is a subset of another set
		int 		getBit(const int bit)		const { return setExt[bit]; }	// identical to contains
		void 		add(const int bit) { setExt.set(bit); }					// sets a bit to 1
		void 		add(const BitSetExt a) { setExt |= a.setExt; }							// sets all of input set to 1
		void		subtract(const int bit) { setExt.set(bit, 0); }				// set bit to zero
		void 		subtract(const BitSetExt a) { setExt &= ~a.setExt; }							// sets all of input set to 0

		int countTrailingZeros(std::bitset<BITSET_SIZE> s) const
		{
			int zeros = 0;
			std::bitset<BITSET_SIZE> sBs = std::bitset<BITSET_SIZE>(s);

			if (sBs == S_ZERO)
			{
				return 0;
			}
			
			while ((sBs & S_ONE) != S_ONE)
			{
				zeros++;
				sBs >>= 1;
			}

			return zeros;
		}

		int countLeadingZerosLegacy(std::bitset<BITSET_SIZE> s) const
		{
			std::bitset<BITSET_SIZE> sBs = std::bitset<BITSET_SIZE>(s);		
			std::bitset<BITSET_SIZE> sLeading = S_LSHIFT(S_ONE, (BITSET_SIZE/2));

			int zeros = 0;

			if (sBs == S_ZERO)
			{
				return 0;
			}

			while ((sBs & sLeading) != sLeading)
			{
				++zeros;
				sBs <<= 1;
			}
			

			return zeros;
		}

		int countLeadingZeros(std::bitset<BITSET_SIZE> s) const
		{
			std::bitset<BITSET_SIZE> sBs = std::bitset<BITSET_SIZE>(s);
			std::bitset<BITSET_SIZE> sLeading = (S_ONE << (BITSET_SIZE - 1));

			int zeros = 0;

			while ((sBs & sLeading) != sLeading)
			{
				++zeros;
				sBs <<= 1;
			}


			return zeros;
		}

		int numActions() const
		{
			return setExt.count();
		}

		Action randomAction() const
		{
			BitSetExt s(setExt);
			int num = s.numActions();
			if (num == 1)
			{
				return s.popAction();
			}

			int r = (rand() % (s.numActions() - 1));

			Action a = s.popAction();

			for (int i = 0; i < r; i++)
			{
				a = s.popAction();
			}

			return a;
		}

		void print() const
		{
			// not fully implemented
			for (int i = 0; i < BITSET_SIZE; i++) { printf("%d", (BITSET_SIZE - i) % 10); }
			printf("\n");
			for (int i = 0; i < BITSET_SIZE; i++) { printf("%d", (setExt[(BITSET_SIZE - i)])); }

		}
	};

	typedef BitSetExt ActionSetExt;
	typedef BitSetExt ActionSet;

}