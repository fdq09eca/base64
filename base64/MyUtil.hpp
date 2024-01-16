//
//  MyUtil.hpp
//  base64
//
//  Created by ChrisLam on 16/01/2024.
//

#ifndef MyUtil_hpp
#define MyUtil_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>

#define MY_ASSERT(x) my_assert(x, #x, __LINE__, __FILE__)

#define TEST(EXPR)                                                              \
do                                                                              \
{                                                                               \
printf("[%s] Line %3d: %s\n", ((EXPR) ? " OK " : "FAIL"), __LINE__, #EXPR);     \
} while (0) //-------------------

#define MY_DUMP(X)                        \
do                                        \
{                                         \
std::cout << #X ": " << X << "\n";        \
} while (false)


inline void my_assert(bool x, const char* msg, int lineNumber, const char* filename) {
    assert(x);
    if (!x) printf("%s", msg);
}

class MyError : public std::exception {
public:
    MyError(const char* msg){
        perror(msg);
        exit(errno);
    }
};

struct MyUtil {
    
    template<typename T>
    inline static size_t countOneBit(const T& o) {
        size_t n = 0;
        size_t totalBit = sizeof(T);
        
        for (auto i = 0; i < totalBit; i ++) {
            n += o & 0x01;
            o = o >> 1;
        }
        
        return n;
    }
    
    template<typename T>
    inline static size_t getSzLen(const T* sz) {
        size_t c = 0;
        auto* p = sz;
        while (*p) { c++; p++; };
        return c;
    }
    
    static void dump_hex(const char* c);
    static void dump_hex(const char16_t* c);
    static void dump_hex(const char32_t* c);
};


#endif /* MyUtil_hpp */
