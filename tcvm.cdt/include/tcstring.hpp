#ifndef _TC_STRING_HPP_
#define _TC_STRING_HPP_

#include <cassert>
#include <cstring>
#include <cstdlib>

#ifndef TCSTRING

#include <string>
namespace tc {
    typedef std::string string;
}

#else

namespace tc {

class string {

private:
    const char *mData;
    size_t mLen;
    bool mIsMalloc;

    inline void reset() {
        if (mIsMalloc) {
            if (mData != NULL) {
                free((char *)mData);
                mData = NULL;
            }
            mLen = 0;
            mIsMalloc = false;
        } 
    }

public:
    string(const char *str="", bool isMalloc=false) : mData(str), mLen(strlen(str)), mIsMalloc(isMalloc) {}
    ~string() { reset(); }

    string(const string& rhs) {
        if (rhs.mIsMalloc) {
            mData = (char *)calloc(1, rhs.mLen+1);
            memcpy((char *)mData, (char *)rhs.mData, rhs.mLen+1);
        } else {
            mData = rhs.mData;
        }
        mIsMalloc = rhs.mIsMalloc;
        mLen = rhs.mLen;
    }
    inline size_t length() const { return mLen; }
    inline const char* c_str() const { return mData; }

    string& operator =(const string& rhs) {
        if (this != &rhs) {
            reset();
            if (rhs.mIsMalloc) {
                mData = (char *)calloc(1, rhs.mLen+1);
                memcpy((char *)mData, (char *)rhs.mData, rhs.mLen+1);
            } else {
                mData = rhs.mData;
            }
            mIsMalloc = rhs.mIsMalloc;
            mLen = rhs.mLen;
        }
        return *this;
    }
    string& operator =(const char *s) {
        reset();
        mData = s;
        mLen = strlen(s);
        return *this;
    }

    inline void assign(const char *s) {
        reset();
        mData = s;
        mLen = strlen(s);
    }

    inline int compare(const string& s) { return strcmp(mData, s.mData); }
    inline bool operator ==(const string& s) { return (compare(s) == 0); }
    inline bool operator <(const string& s) { return (compare(s) < 0); }

    string& append(const string& s) {
        if (s.mLen > 0) {
            size_t len = mLen + s.mLen + 1;
            char *data = (char *)calloc(1, len);
            if (mData != NULL)
                memcpy(data, mData, mLen);
            memcpy(data+mLen, s.mData, s.mLen);
            
            reset();
            mData = data;
            mLen =  len - 1;
            mIsMalloc = true;
        }
        return *this;
    }
    inline string& append(const char *s) { return append(string(s)); }
    inline string& operator +(const string& s) { return append(s); }
    inline string& operator +(const char *s) { return append(s); }
    inline string& operator +=(const string& s) { return append(s); }
};

}; // namespace tc

#endif

#endif // _TC_STRING_HPP_
