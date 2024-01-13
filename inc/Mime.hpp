#ifndef MIME_HPP
#define MIME_HPP

#include "Webserv.hpp"

class Mime
{
    public:
        Mime();
        STR             getMimeType(STR extension);
    private:
        MAP<STR, STR>   _mime_types;
        
};

#endif
