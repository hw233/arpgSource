#ifndef _OFFSET_PATH_H_
#define _OFFSET_PATH_H_

#include <iostream>
#include <stdint.h>
#include <vector>
#include <cassert>

struct offset_path{
	int origin_x_;
	int origin_y_;

	int last_x_;
	int last_y_;

	std::vector<char> path_;

	void set_origin(int x, int y){
		origin_x_ = x;
		origin_y_ = y;

		last_x_ = origin_x_;
		last_y_ = origin_y_;
	}	
	void _add(int x,int y){
		//新坐标-原点坐标得到偏移量
		int nx = x - last_x_;
		int ny = y - last_y_;

		//std::cout << "last:" << last_x_ << "," << last_y_ << "    cur:" << x << "," << y <<  " dist:" << nx << "," << ny << std::endl;

		last_x_ = x;
		last_y_ = y;

		//
		ASSERT(nx > -127 && nx < 128);
		ASSERT(ny > -127 && ny < 128);
		path_.push_back((int8_t)nx);
		path_.push_back((int8_t)ny);
	}

	void add(int size, int8 *path){
		path_.resize(size);
		memcpy(&path_[0], path, size);
	}

	void set(const std::vector<int>& path){
		assert(path.size() >= 0);
		assert(path.size()%2 == 0);
		path_.clear();

		origin_x_ = path[0];
		origin_y_ = path[1];

		last_x_ = origin_x_;
		last_y_ = origin_y_;

		for(int i = 2; i < (int)path.size(); i = i + 2)
			_add(path[i], path[i+1]);
	}

	//从绝对坐标路径转化成
	template<typename T>
	void get(std::vector<T>& path) {
		path.clear();
		last_x_ = origin_x_;
		last_y_ = origin_y_;

		////插入原点
		//path.push_back((T)last_x_);
		//path.push_back((T)last_y_);

		//产生绝对坐标
		for(int i = 0; i < (int)path_.size(); i = i + 2) {
			last_x_ += path_[i];
			last_y_ += path_[i+1];

			path.push_back((T)last_x_);
			path.push_back((T)last_y_);
		}
	}
};

static bool CreateOffsetPath(int x,int y,std::deque<float> &path,vector<char>& ofpath)
{	
	ofpath.clear();
	int last_x = x;
	int last_y = y;

	int totalpoint = 0;
	for (int i = 0; i < (int)path.size(); i = i + 2)
	{
		//如果超过100个点则直接返回
		if(totalpoint++ >= 1000)
			return false;

		if(path[i] != path[i] || path[i+1] != path[i+1])
			return false;

		int nx = int(path[i]) - last_x;
		int ny = int(path[i+1]) - last_y;

		if(nx <= -127 || nx >= 128 || ny <= -127 || ny >= 128){
			path.insert(path.begin()+i,(last_x + path[i])/2);
			path.insert(path.begin()+i+1,(last_y + path[i+2])/2);
			i -= 2;
			continue;
		}

		ASSERT(nx > -127 && nx < 128);
		ASSERT(ny > -127 && ny < 128);		

		ofpath.push_back(nx);
		ofpath.push_back(ny);

		last_x = (int)path[i];
		last_y = (int)path[i+1];
	}
	return true;
}


#endif
