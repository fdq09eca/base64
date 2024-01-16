//
//  Base64.hpp
//  base64
//
//  Created by ChrisLam on 16/01/2024.
//

#ifndef Base64_hpp
#define Base64_hpp

#include <stdio.h>
#include "MyUtil.hpp"

struct Base64 {
    // ref: https://en.wikipedia.org/wiki/Base64
    
    static uint8_t toBase64Byte(char c) {
        if (c >= 'A' && c <= 'Z') { return c - 'A'; } // 0  - 25
        if (c >= 'a' && c <= 'z') { return c -  71; } // 26 - 51
        if (c >= '0' && c <= '9') { return c +   4; } // 52 - 61
        if (c == '+'            ) { return 62;      }
        if (c == '/'            ) { return 63;      }
        return '='; // "padding"
    }
    
    
    
    static char toChar(uint8_t v) {
        // from base64 val to char
        // ref: https://www.ascii-code.com/
        
        if (v <  26) { return v + 'A'; } // A - Z
        if (v <  52) { return v + 71 ; } // a - z
        if (v <  62) { return v -  4 ; } // 0 - 9
        if (v == 62) { return '+'    ; }
        if (v == 63) { return '/'    ; }
        
        return '='; // padding
    }
    
    static void decode(std::string& o, const char* src, size_t srcLen) {
        if (!src) return;
        o.clear();
    
        auto p = src;
        auto e = src + srcLen;
        while (p < e) {
            
            char c1 = *p; p++;
            char c2 = *p; p++;
            
            uint8_t bc1 = toBase64Byte(c1);
            uint8_t bc2 = toBase64Byte(c2);
            
            uint8_t b1 = ((bc1 & 0x3F) << 2) | ((bc2 & 0x30) >> 4);
            o.push_back(b1);
            
            char c3 = *p; p++;
            if (c3 == '=') return;
            
            
            uint8_t bc3 = toBase64Byte(c3);
            uint8_t b2_1 = ((bc2 & 0x0F) << 4);
            uint8_t b2_2 = ((bc3 & 0x3C) >> 2);
            uint8_t b2 = b2_1 | b2_2;
            o.push_back(b2);
            
            char c4 = *p; p++;
            if (c4 == '=') return;
                   
            uint8_t bc4 = toBase64Byte(c4);
            uint8_t b3_1 = ((bc3 & 0x03) << 6);
            uint8_t b3_2 = ((bc4 & 0x3F)     );
            uint8_t b3 = b3_1 | b3_2;
            o.push_back(b3);
            
        }
    }
    
    static void encode(std::string& o, const uint8_t* src, size_t srcLen) {
        if (!src) return;
        
        o.clear();
        
        auto p = src;
        auto e = src + srcLen;
        size_t r = srcLen % 3;
        size_t npad = r? 3 - r : 0;
        
        while (p < e) {
            if (npad == 1 && p + 2 >= e)
            {   // 1 padding
                uint8_t c1 = *p; p++;
                uint8_t c2 = *p; p++;
                
                uint8_t b1 =  (c1 >> 2);
                uint8_t b2 = ((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4);
                uint8_t b3 = ((c2 & 0x0F) << 2);
            
                o.push_back(toChar(b1 & 0x3F));
                o.push_back(toChar(b2 & 0x3F));
                o.push_back(toChar(b3 & 0x3C));
                o.push_back('=');
            }
            else if (npad == 2 && p + 1 >= e)
            {   // 2 padding
                uint8_t c1 = *p; p++;
                
                uint8_t b1 =  (c1 >> 2);
                uint8_t b2 = ((c1 & 0x03) << 4);
                
                o.push_back(toChar(b1 & 0x3F));
                o.push_back(toChar(b2 & 0x30));
                o.push_back('=');
                o.push_back('=');
            }
            else
            {
                uint8_t c1 = *p; p++;
                uint8_t c2 = *p; p++;
                uint8_t c3 = *p; p++;
                
                char b1 =  (c1 >> 2);
                char b2 = ((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4);
                char b3 = ((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6);
                char b4 =  (c3 & 0x3F);
                
                
                o.push_back(Base64::toChar(b1 & 0x3F));
                o.push_back(Base64::toChar(b2 & 0x3F));
                o.push_back(Base64::toChar(b3 & 0x3F));
                o.push_back(Base64::toChar(b4 & 0x3F));
            }
        }
    }
    
    static void encode_urlsafe(std::string& o, const uint8_t* src, size_t srcLen) {
        // omits the padding and replaces +/ with -_
        // ref: https://developer.mozilla.org/en-US/docs/Glossary/Base64
        
        encode(o, src, srcLen);
        size_t npad = 0;
        
        for (char& c : o) {
            if (c == '+' ) c = '-';
            if (c == '/' ) c = '_';
            if (c == '=' ) npad++;
        }
        
        if (npad)
            o.resize(o.size() - npad);
    }
    
    static void decode_urlsafe(std::string& o, const char* src, size_t srcLen) {
        std::string src_copy = src;
        
        for (auto& c : src_copy) {
            if (c == '+' || c == '/') {
                throw MyError("[ERR] contain '+' or '/', Url unsafe ecoding.");
            }
            if (c == '-') c = '+';
            if (c == '_') c = '/';
        }
        
        // add padding.
        size_t r = src_copy.size() % 4;
        size_t npad = r? 4 - r: 0;
        
        for (size_t i = 0; i < npad; i++) {
            src_copy.append("=");
        }
        
        decode(o, src_copy.data(), srcLen);
    }
};

#endif /* Base64_hpp */
