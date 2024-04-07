#ifndef GMX_TOOLS_DUMP_DIRECTOR_H
#define GMX_TOOLS_DUMP_DIRECTOR_H

class DumpDirector {
private:
    DumpBuilder mainBuilder;

public:
    DumpDirector(DumpBuilder &builder) : mainBuilder(builder) {}

    void interpret(std::string selector)
    {
        
    }
};

#endif
