//
//  main.cpp
//  base64
//
//  Created by ChrisLam on 16/01/2024.
//

#include <iostream>
#include "Base64.hpp"

void test_encodeBase64_p0(){
    std::string o;
    const char* sz = "Man";

    Base64::encode(o, (uint8_t*) sz, strlen(sz));
    MY_DUMP(o);
    TEST(o == "TWFu");
}

void test_encodeBase64_p1(){
    // 1 padding
    std::string o;
    const char* sz = "Ma";
    
    
    Base64::encode(o, (uint8_t*) sz, strlen(sz));
    MY_DUMP(o);
    TEST(o == "TWE=");
}

void test_encodeBase64_p2(){
    // 2 padding
    std::string o;
    const char* sz = "M";

    Base64::encode(o, (uint8_t*) sz, strlen(sz));
    MY_DUMP(o);
    TEST(o == "TQ==");
}


void test_encodeBase64_rand(){
    // 2 padding
    std::string o;
    const char* sz = "nullptr";
    
    
    Base64::encode(o, (uint8_t*) sz, strlen(sz));
    MY_DUMP(o);
    TEST(o == "bnVsbHB0cg==");
    
    sz = "Many hands make light work.";
    Base64::encode(o, (uint8_t*) sz, strlen(sz));
    MY_DUMP(o);
    TEST(o == "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    test_encodeBase64_p0();
    test_encodeBase64_p1();
    test_encodeBase64_p2();
    test_encodeBase64_rand();
    std::cout << "Hello, World!\n";
    return 0;
}
