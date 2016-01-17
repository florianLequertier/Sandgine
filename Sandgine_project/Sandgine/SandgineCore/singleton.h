#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
private:
    Singleton();

public:
     Singleton(const Singleton<T>& other) = delete;
     void operator=(const Singleton<T>& other) = delete;

     static T& instance();
};

template<class T>
Singleton<T>::Singleton()
{

}

template<class T>
T& Singleton<T>::instance()
{
    static T instance;

    return instance;
}

#endif // SINGLETON_H
