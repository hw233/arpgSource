/****************************************************************************
 Copyright (c) 2011-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "Cocos2dxLuaLoader.h"
#include <string>
#include <algorithm>

#include "CCLuaStack.h"

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif // WIN32

#include <stdio.h>
#include <zlib/zlib.h>
#include <assert.h>

struct unzip_tool
{
	unzip_tool()
	{
		uncompress_size_ = 256;
		buff_ = malloc(uncompress_size_);

		zlib_stream_ = (struct z_stream_s *)malloc(sizeof(struct z_stream_s));

		zlib_stream_->zalloc = (alloc_func)0;
		zlib_stream_->zfree = (free_func)0;
		zlib_stream_->opaque = (voidpf)0;

		//速度取Z_BEST_SPEED到Z_BEST_COMPRESSION之间（1到9）
		//Z_DEFAULT_COMPRESSION压缩率与速度的平衡（相当于6）
		int z_res = deflateInit(zlib_stream_, Z_DEFAULT_COMPRESSION);
		assert(z_res == Z_OK);
	}
	~unzip_tool()
	{
		if(zlib_stream_){
			deflateEnd(zlib_stream_);	
			free(zlib_stream_);
			zlib_stream_ = NULL;
		}
		
		if(buff_){
			free(buff_);
			buff_ = NULL;
		}
	}

	int unCompress(const char* inputf,int len,char **out)
	{

		int err = 0;		
		uLongf unc_size = 0;
		Bytef *ptr = (Bytef*)malloc(uncompress_size_);


		//从压缩包中取得解压后在大小，如果返回包的大小不一致，则取大的为准		
		do
		{
			//扩展空间
			unc_size = uncompress_size_;
			err = uncompress(ptr, &unc_size, (Bytef *)inputf, len);
			if(err != Z_OK)
			{
				if(err == Z_BUF_ERROR){
					uncompress_size_ <<= 1;
					ptr = (Bytef*)realloc(ptr,uncompress_size_);
				}else {
					free(ptr);
					return -1;
				}
			}
		}while(err != Z_OK);
		*out = (char*)ptr;
		return unc_size;		
	}

	struct z_stream_s *zlib_stream_;				//zlib压缩流
	void *buff_;
	int uncompress_size_;
};

extern "C"
{	
	static int read_file(const char* inputf,char **out) 
	{
		char *p = NULL;	
		int total = 0;
		char temp[8096];

		FILE *f = fopen(inputf, "rb");
		if(!f) {
			printf("open file:%s failed!",inputf);
			return -1;
		}

		p = (char*)malloc(1);
		while(!feof(f)){
			int n = fread(temp,1,sizeof(temp),f);
			total += n;
			p = (char*)realloc(p,total + 1);
			memcpy(p + total - n,temp, n);
		}

		fclose(f);

		*out = p;
		return total;
	}

    int cocos2dx_lua_loader(lua_State *L)
    {
        static const std::string BYTECODE_FILE_EXT    = ".luac";
        static const std::string NOT_BYTECODE_FILE_EXT = ".lua";
		static const std::string LUA_ZIP_EXT = ".luaz";
        
        std::string filename(luaL_checkstring(L, 1));
        size_t pos = filename.rfind(NOT_BYTECODE_FILE_EXT);
        if (pos != std::string::npos)
        {
            filename = filename.substr(0, pos);
        }
        
        pos = filename.find_first_of(".");
        while (pos != std::string::npos)
        {
            filename.replace(pos, 1, "/");
            pos = filename.find_first_of(".");
        }
        
        // search file in package.path
        unsigned char* chunk = nullptr;
        uint32_t chunkSize = 0;
        std::string chunkName;        
        
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        std::string searchpath(lua_tostring(L, -1));
        lua_pop(L, 1);
        size_t begin = 0;
        size_t next = searchpath.find_first_of(";", 0);
        
        do
        {
            if (next == std::string::npos)
                next = searchpath.length();
            std::string prefix = searchpath.substr(begin, next);
            if (prefix[0] == '.' && prefix[1] == '/')
            {
                prefix = prefix.substr(2);
            }
            
            pos = prefix.find("?.lua");
            chunkName = prefix.substr(0, pos) + filename + BYTECODE_FILE_EXT;
            if (access(chunkName.c_str(), 4/*"rb"*/) == 0)
            {                
				chunkSize = read_file(chunkName.c_str(),(char**)&chunk);
                break;
            }
            else
            {
                chunkName = prefix.substr(0, pos) + filename + NOT_BYTECODE_FILE_EXT;
                if (access(chunkName.c_str(), 4/*"rb"*/) == 0)
                {
                    chunkSize = read_file(chunkName.c_str(),(char**)&chunk);
                    break;
                }
				else
				{
					chunkName = prefix.substr(0, pos) + filename + LUA_ZIP_EXT;
					if (access(chunkName.c_str(), 4/*"rb"*/) == 0)
					{
						char *file_content = NULL;
						int content_len = 0;
						//先把文件内容读出来
						content_len = read_file(chunkName.c_str(),(char**)&file_content);
						//调用zip解压缩	
						static unzip_tool _unzip_tool;
						chunkSize = _unzip_tool.unCompress(file_content,content_len,(char**)&chunk);
						if(file_content) 
						{
							free(file_content);
							file_content = 0;
						}
						break;
					}
				}
            }
            
            begin = next + 1;
            next = searchpath.find_first_of(";", begin);
        } while (begin < (int)searchpath.length());
        
        if (chunk)
        {
            LuaStack* stack = LuaStack::getInstance(L);
            stack->luaLoadBuffer(L, (char*)chunk, (int)chunkSize, chunkName.c_str());
            free(chunk);
        }
        else
        {
            printf("can not get file data of %s\n", chunkName.c_str());
            return 0;
        }
        
        return 1;
    }
}
