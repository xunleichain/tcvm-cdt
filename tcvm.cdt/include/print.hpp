#ifndef _PRINT_H_
#define _PRINT_H_

#ifdef DEBUG

#include "tcstring.hpp"
#include "tcapi.hpp"
#include "tcmethod.hpp"
#include "reflect.hpp"
#include "types.hpp"


namespace debug {

using namespace tc;

class StringBuffer {
public:
    StringBuffer(int size=1024) {
        if (size <= 1024)
            this->buf = _cache;
        else
            this->buf = new char[size]();
        this->pos = 0;
    }

    ~StringBuffer(){
        if (this->buf != _cache)
            delete[] this->buf;
    }

    void append(const char *s) {
        if (s != NULL) {
            strcpy(this->buf+this->pos, s);
            this->pos += strlen(s);
        }
    }

    inline void append(const char c) {
        this->buf[this->pos] = c;
        this->pos += 1;
    }

    inline void append(const tc::string& s) { append(s.c_str()); }

    inline void append(const int n) {
        char *s = itoa(n);
        append(s);
    }
    inline void append(const int64& n) {
        char *s = i64toa(n, 10);
        append(s);
    }
    inline void append(const uint64& n) { append(int64(n)); }
    inline void append(const tc::Address& addr) { append(addr.toString()); }
    inline void append(const tc::BInt& n) { append((const char*)(n.toString())); }
    inline void append(const tc::Hash& hash) { append(hash.toString()); }
    inline void append(const tc::Token& token) {
        append("Token-");
        append(token.id());
        append(':');
        append(token.value());
    }
    
    template<typename... Args>
    void append(const std::tuple<Args...>& val) {
        int index = 0;
        StringBuffer *buf = this;
        reflect::ForEachTuple(val, [&buf, &index](const auto& item) {
            if (index > 0)
                buf->append(", ");
            buf->append(item);
            index++;
        });
        append(']');
    }
    
    template<typename T>
    void append(const std::vector<T>& vt) {
        append('[');
        StringBuffer *buf = this;
        int index = 0;
        std::for_each(vt.begin(), vt.end(), [&buf, &index](const auto& item) {
            if (index > 0)
                buf->append(", ");
            buf->append(item);
            index++;
        });
        append(']');
    }
    
    template<typename K, typename V>
    void append(const std::map<K, V>& m) {
        append('{');
        StringBuffer *buf = this;
        int index = 0;

        std::for_each(m.begin(), m.end(), [&buf, &index](const auto& item) {
            if (index > 0)
                buf->append(", ");
            buf->append(item.first);
            buf->append(':');
            buf->append(item.second);
            index++;
        });
        append('}');
    }

    const char *toString() {
        this->buf[this->pos+1] = '\0';
        return this->buf;
    }

private:
    char *buf;
    int pos;

private:
    static char *_cache;
};

char *StringBuffer::_cache = (char *)calloc(1, 1024);


template<typename... Args>
void TCPrint(char *head, const std::tuple<Args...>&& value) {
    StringBuffer buf(1024);
    buf.append(head);
    buf.append("  "); //buf.append('\t');
    bool flag = false;

    reflect::ForEachTuple(value, [&buf, &flag](auto&& arg) {
        buf.append(arg);
        if (flag) {
            buf.append("  ");
            flag = false;
        } else {
            buf.append('=');
            flag = true;
        }
    });

    TC_Prints(buf.toString());
}

} // namespace debug

#define Print(head, ...) \
  debug::TCPrint((char *)head, std::make_tuple(__VA_ARGS__));


#else

#define Print(head, ...) 

#endif // DEBUG
 
#endif // _PRINT_H_
