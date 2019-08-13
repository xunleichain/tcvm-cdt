#pragma once

/**
 * Define specific things for the thunderchain
 */

extern "C" {
   void TC_RequireWithMsg(bool, const char*);
   void tc_assert(unsigned int i, const char* str){
            TC_RequireWithMsg(i, str);
    }
   void __cxa_pure_virtual() { tc_assert(false, "pure virtual method called"); }
}
