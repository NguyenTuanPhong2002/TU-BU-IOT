/*
 * Singleton.h
 *
 *  Created on: Jan 25, 2024
 *      Author: Nguyen Tuan Phong
 * 		gmail: ntphong011102@gmail.com
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

class Singleton
{
private:
    /* data */
    static Singleton *instancePtr;

    Singleton(/* args */);
    ~Singleton();

public:
    Singleton *getInstance(){
        if(instancePtr == nullptr){
            instancePtr = new Singleton();
        }
        return instancePtr;
    }
};

#endif /* SINGLETON_H_ */
