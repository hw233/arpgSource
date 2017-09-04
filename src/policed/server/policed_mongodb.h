#ifndef _POLICED_MONGODB_H_
#define _POLICED_MONGODB_H_

#include "policed_app.h"
#include <db_wrap/mongo_db.h>
#include "policed_context.h"
#include "policed_app.h"
#include "policed_object_manager.h"

class PolicedMongoDB:public MongoDB
{
	friend class GuidManager;
public:
	PolicedMongoDB(log_holder_t *log);
	virtual ~PolicedMongoDB();

	account_info *LoadAccountByUserName(const char *username);
	account_info *LoadAccountByUid(const char *uid);

	string GetAccountFromGuid(string str);
private:
	string server_name_;
};


#endif // !_POLICED_MONGODB_H_
