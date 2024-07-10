#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>
#include <string>
#include <cstddef>
#include <iostream>

class BasicPool {
    protected:
        size_t sz;
    
    public:
        BasicPool() {}
        virtual ~BasicPool() {}
        virtual size_t size() const = 0;
        virtual bool empty() const = 0;
        virtual int clear() = 0;
        virtual int load(std::istream& is) = 0;
        virtual int save(std::ostream& os) const = 0;
};
