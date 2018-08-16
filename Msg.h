#ifndef MSG

#define MSG
#include <string>
#include <list>
#include <map>
//#include <unordered_map>
#include "libjson/libjson.h"

using namespace std;



class CMsg
{
public:
    CMsg();
    ~CMsg();
    void addUser();
    
private:
    list<CUserInfo *> m_UserList;
    map<string id, list<CUserInfo *>::iterator it> m_DicUserInfo;
};

#endif //Msg.h