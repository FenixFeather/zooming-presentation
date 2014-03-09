#ifndef ABSTRACTMODE_HPP
#define ABSTRACTMODE_HPP

class AbstractMode{
public:
     virtual bool init() = 0;
     virtual void run() = 0;
     virtual void close() = 0;
     //virtual ~AbstractMode();
};


#endif //ABSTRACTMODE_HPP
