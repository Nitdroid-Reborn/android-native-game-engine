#ifndef __CHASH_H
#define __CHASH_H

#include "EngineTypes.h"

class Hash
{
public:
    Hash();
    Hash(U32 hashValue);
    Hash(const char *pString);
    Hash(const Hash &rhs);
    Hash &operator=(const Hash &rhs);

    void Set(const U32 hash);

	bool IsValid() const;
    operator U32 ()	const { return mHashValue; }
    U32 GetValue() const {return mHashValue;}

    const bool operator<(const Hash &rhs) const;
    const bool operator>(const Hash &rhs) const;
    const bool operator<=(const Hash &rhs) const;
    const bool operator>=(const Hash &rhs) const;
    const bool operator==(const Hash &rhs) const;
    const bool operator!=(const Hash &rhs) const;
private:
    U32 MakeHash(const char *pString);

    U32 mHashValue;
};
#endif //__CHASH_H
