#include "MemoryDB.h"

template<>
int MemDB_Compare_Handle<char *>(void *v1,void *v2, size_t )
{
	char *_v1 = static_cast<char *>(v1);
	char *_v2 = static_cast<char *>(v2);
	return strcmp(_v1, _v2);
}
