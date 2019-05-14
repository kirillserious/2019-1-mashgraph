#ifndef FS_H_
#define FS_H_

#include <string>

class Fs
{
        static std::string dir;
public:
        static void init (const std::string &str)
        {
                dir = str;
                dir = dir.erase(dir.rfind('/'));
        }
        static std::string make (const std::string &file)
        {
                return dir + "/" + file;
        }
};

#endif