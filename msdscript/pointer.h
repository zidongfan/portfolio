//
// Created by Zidong Fan on 3/30/23.
//

#ifndef MSDSCRIPT_POINTER_H
#define MSDSCRIPT_POINTER_H

#include <memory>

#define USE_PLAIN_POINTERS 0 // 1 -> Normal pointers 0 -> Smart pointers
#if USE_PLAIN_POINTERS

# define NEW(T)    new T
# define PTR(T)    T*
# define CAST(T)   dynamic_cast<T*>
# define CLASS(T)  class T
# define THIS     this

#else

# define NEW(T)    std::make_shared<T>
# define PTR(T)    std::shared_ptr<T>
# define CAST(T)   std::dynamic_pointer_cast<T>
# define CLASS(T) : public std::enable_shared_from_this<T>
# define THIS      shared_from_this()


#endif

#endif //MSDSCRIPT_POINTER_H
