#ifndef _ENCIPHER_H_
#define _ENCIPHER_H_

#include <comm/crypt.h>

//#include "core_obj/ByteArray.h"

#define PROTO_TEA_C2S "2f651cbf85539c977da645a90cbd6b62"
#define PROTO_TEA_S2C "335e0c133a6d7e254879d05612388210"
#define PROTO_MD5_KEY "40f09aa5ff525e8cc3a9d16bf1abc3c1"/*用于通讯加密*/

using core_obj::ByteArray;

namespace core_socket
{

//加密器
class encipher
{
public:
	encipher():md5_str_(PROTO_MD5_KEY)
	{
		init(PROTO_TEA_C2S,PROTO_TEA_S2C);
	}
	void md5_str(const char* s){ md5_str_ = s; }

	void init(const char *c2s_key,const char *s2c_key)
	{
		MD5CryptStr16(c2s_key,(char*)&tk_c2s_);
		MD5CryptStr16(s2c_key,(char*)&tk_s2c_);
	}

	//用所给的数据包密钥的重新生成
	void reset(ByteArray *pkt)
	{
		char tmp[2048];
		memset(tmp,0,sizeof(tmp));
		MD5CryptToStr32(pkt->data(), pkt->length(), tmp);
		strcat(tmp,md5_str_ ? md5_str_ : PROTO_MD5_KEY);
		init(tmp,tmp);
	}

	//S2C 发包前加密一下
	void encode(ByteArray *pkt)
	{
		int tea_v_size = (pkt->length() /*- pkt->pos*/) >> 3;
		tea_v *p_tv = (tea_v*)(pkt->data() /*+ pkt->pos*/);
		for (int i = 0; i < tea_v_size; i++)
			tean((TEAint32*)&tk_s2c_, (TEAint32*)(p_tv+i), 16);//加密
		tk_s2c_.k[3]--;
	}

	//C2S 解包前解密一下
	void decode(ByteArray *pkt)
	{
		int tea_v_size = (pkt->length() /*- pkt->pos*/) >> 3;//uint16 2个字节
		tea_v *p_tv = (tea_v*)(pkt->data() /*+ pkt->pos*/);
		for (int i = 0; i < tea_v_size; i++)
			tean((TEAint32*)&tk_c2s_, (TEAint32*)(p_tv+i), -16);//解密
		//跟客户端约定好,每解一次包就给key加上
		tk_c2s_.k[0]++;
	}

protected:
	tea_k tk_c2s_; 		/*用于C2S*/
	tea_k tk_s2c_; 		/*用于S2C*/
	const char* md5_str_; /*重置密码时加密字符串*/
};

//用于客户端往服务器发送时使用,测试专用
class c2sencipher : public encipher
{
public:
	void encode(ByteArray *pkt)
	{
		int tea_v_size = (pkt->length() /*- pkt->pos*/) >> 3;
		tea_v *p_tv = (tea_v*)(pkt->data() /*+ pkt->pos*/);
		for (int i = 0; i < tea_v_size; i++)
			tean((TEAint32*)&tk_c2s_, (TEAint32*)(p_tv+i), 16);//加密
		tk_c2s_.k[0]++;
	}

	void decode(ByteArray *pkt)
	{
		int tea_v_size = (pkt->length() /*- pkt->pos*/) >> 3;//uint16 2个字节
		tea_v *p_tv = (tea_v*)(pkt->data() /*+ pkt->pos*/);
		for (int i = 0; i < tea_v_size; i++)
			tean((TEAint32*)&tk_s2c_, (TEAint32*)(p_tv+i), -16);//解密
		//跟客户端约定好,每解一次包就给key加上
		tk_s2c_.k[3]--;
	}
};

}

#endif

