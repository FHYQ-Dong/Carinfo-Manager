#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>
#include <string>
#include <cstddef>

class BasicPool {
    protected:
        size_t sz;
    
    public:
        BasicPool() {}
        ~BasicPool() {}
        virtual size_t size() const = 0;
        virtual bool empty() const = 0;
        virtual int clear() = 0;
        virtual int load(std::string filename) = 0;
        virtual int save(std::string filename) const = 0;
};
