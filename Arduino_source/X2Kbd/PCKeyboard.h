//
// PC Keyboard
// by cloudree@naver.com
//

class CPCKeyboard
{
public:
    static CPCKeyboard*      GetInstance()     { return &m_instance; }

    void    init();
    void    process();

    void    presskey( BYTE ch );
    void    releasekey( BYTE ch );
    
protected:
    CPCKeyboard()   { };
    static CPCKeyboard    m_instance;
};
