#ifndef CORE_SINGLETON_H
#define CORE_SINGLETON_H


template <class T>
class Singleton
{
public:
    static T *instance() {
        static T obj;
        return &obj;
    }
protected:
    Singleton() = default;
    ~Singleton() = default;
};

#endif // CORE_SINGLETON_H
