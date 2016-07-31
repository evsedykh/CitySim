#ifndef SINGLETON_H
#define SINGLETON_H

template< class T > class Singleton
{
public:
    static const Singleton< T >& GetInstance()
    {
        static Singleton< T > theSingleton;
        return theSingleton;
    }
private:
    Singleton(){}
    Singleton(const Singleton& );
    Singleton& operator=(const Singleton&);

};

#endif // SINGLETON_H
