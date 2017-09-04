/************************************************************************
 * file	:svrcore.h                                                                     
 * desc	:该文件引用了svrcore所有相关的头文件
 *		如 通讯协议 对象实体定义 对象更新协议
 *
 *
 * author:	linbc
 * date:	20110429
 * version:
 ************************************************************************/
#ifndef _SVRCORE_H_
#define _SVRCORE_H_

#include <svrcore-internal.h>

#ifdef __cplusplus
#  include <shared/util.h>
//#  include <database/DatabaseEnv.h>
#  include <server/svrcore_app.h>
#  include <protocol/WorldPacket.h>
#  include <shared/MemoryDB.h>
#endif

//#include <protocol/external.h>
//#include <protocol/internal.h>


/*
*	svrcore_init - 初始化
*/
void svrcore_init();

/*
*	svrcore_free - 释放
*/
void svrcore_free();

#endif

