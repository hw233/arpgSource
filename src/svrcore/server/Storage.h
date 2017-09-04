#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <core_obj/internal.h>

namespace core_obj {

class GuidObject;
class GuidObjectTable;

#define BINLOG_EXT_NAME ".blg"

class Storage
{
public:
	typedef std::function<GuidObject*(string)> CreateFunction;
	typedef std::function<bool(const string &)> ReadFunction;
	Storage(CreateFunction f);
	virtual ~Storage();
protected:
	string work_directory_;		//存储的文件夹路径
	CreateFunction create_func_;//GuidObject对象创建方法
public:
	static bool CreatDir(string path);
	static void RemoveDir(string path);
	static bool IsHaveFile(string path);
public:
	bool Goto(string f);	
	
	bool Load(const string& file_name,vector<GuidObject*> &vec);		//载入某个文件下面的所有的对象
	bool AddContent(const string &file_name, const string &content);	//追加文件内容
	bool ReadFile(const string &file_name, ReadFunction fun);			//读某文件的内容
	bool Remove(const string& file_name);								//删除一个文件
	void ForEachDir(std::function<void(string , string )>);				//遍历路径下的所有文件（不遍历文件夹）
	void GetDirs(const string &dir,std::function<void(const string&)>);	//获得路径下的所有文件夹名字
	bool CopyDirFilse(string& src, string& dst);						//拷贝源路径下的所有文件到目的路径	
	bool CopyAFile(string& src, string& dst);							//拷贝文件
	bool SaveFile(const string& file_name, const string& file_content);	//保存一个文件到硬盘
};

}
#endif
