#ifnde USER

#define USER

class CUserInfo
{
public:
    CUserInfo();
    ~CUserInfo();
    
    string m_name;
    string m_id;
    string m_msg;
//    string m_target_id;
}

class CUser
{
public:
    CUser();
    ~CUser();
    void insertUser();
    void delUser();
    CUserInfo * getUserInfo(string id);
    
private:
    list<CUserInfo *> m_UserList;
    map<string id, list<CUserInfo *>::iterator it> m_DicUserInfo;
}

#endif //user