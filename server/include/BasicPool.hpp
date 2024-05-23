#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>

class BasicPool {
    protected:
        size_t sz;
    
    public:
        BasicPool() {}
        ~BasicPool() {}
        virtual size_t size() const = 0;
        virtual bool empty() const = 0;
};