#ifndef TRINITYCORE_PROGRESSBAR_H
#define TRINITYCORE_PROGRESSBAR_H

#include <svrcore-internal.h>

class TEA_SVRCORE_SPEC barGoLink
{
	static char const * const empty;
	static char const * const full;

	int rec_no;
	int rec_pos;
	int num_rec;
	int indic_len;

public:

	void step( void );
	barGoLink( int );
	~barGoLink();
};
#endif

