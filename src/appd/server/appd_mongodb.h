#ifndef _APPD_MONGODB_H_
#define _APPD_MONGODB_H_

#include <db_wrap/mongo_db.h>

class AppdMongoDB:public MongoDB
{
public:
	AppdMongoDB(log_holder_t *log);
	virtual ~AppdMongoDB();

};
#endif // !_LOGIND_MONGODB_H_
