#include "Hash.h"

#define INVALID_HASH 0xffffffff
#define HASH_INIT	0x811c9dc5
#define HASH_PRIME	0x01000193
#define CONVERT_BACKSLASH
#define CASE_INSENSITIVE

Hash::Hash() : mHashValue(INVALID_HASH)
{
}

Hash::Hash(U32 hashValue) : mHashValue(hashValue)
{
}

Hash::Hash(const Hash &rhs) : mHashValue(rhs.mHashValue)
{

}

Hash::Hash(const char *pString)
{
	mHashValue = MakeHash(pString);
}

Hash &Hash::operator=(const Hash &rhs)
{
	if (&rhs != this)
	{
		mHashValue = rhs.mHashValue;
	}
	return *this;
}

bool Hash::IsValid() const
{
	return (mHashValue != INVALID_HASH);
}

U32 Hash::MakeHash(const char *hashString)
{
	if (!hashString || !hashString[0])
		return INVALID_HASH;

	const unsigned char* string = (const unsigned char*)hashString;
    U32 hash = HASH_INIT;

	while (*string)
	{
		hash *= HASH_PRIME;

		char c = *string++;

#ifdef CONVERT_BACKSLASH
		if (c == '\\')
		{
			c = '/';
		}
#endif

#ifdef CASE_INSENSITIVE
		if ((c >= 'a') && (c <= 'z'))
		{
			c -= 'a' - 'A';
		}
#endif
        hash ^= (U32)c;
	}
	return hash;
}

void Hash::Set(const U32 hash)
{
	mHashValue = hash;
}

const bool Hash::operator<(const Hash &rhs) const
{
	return mHashValue < rhs.mHashValue;
}

const bool Hash::operator>(const Hash &rhs) const
{
	return mHashValue > rhs.mHashValue;
}

const bool Hash::operator<=(const Hash &rhs) const
{
	return mHashValue <= rhs.mHashValue;
}

const bool Hash::operator>=(const Hash &rhs) const
{
	return mHashValue >= rhs.mHashValue;
}

const bool Hash::operator==(const Hash &rhs) const
{
	return mHashValue == rhs.mHashValue;
}

const bool Hash::operator!=(const Hash &rhs) const
{
	return (!(*this == rhs));
}
