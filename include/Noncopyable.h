#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace mynet{

/*
*设计该类的目的是防止用该派生类去构造另一个派生类，或者为另一个派生类赋值
*用户不能构造该类，只能由派生类构造
*/
class Noncopyable{

public:
	Noncopyable(const Noncopyable &a) = delete;
	Noncopyable & operator=(const Noncopyable &a) = delete;
protected:
	Noncopyable() = default;
	~Noncopyable() = default;
	
};
	
	
}







#endif
