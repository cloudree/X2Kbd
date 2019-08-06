//
// X2 Keyboard for PC
// by cloudree@naver.com
//

class CX2Keyboard
{
public:
    static CX2Keyboard*      GetInstance()     { return &m_instance; }
    
    void    init();
    void    process();

    BYTE    Get(int idx)    { return idx < sizeX ? m_readVal[ idx ] : 0; }

    void    SetLed_CAPS(bool isOn);
    void    SetLed_CODE(bool isOn);
    
protected:
    CX2Keyboard()   { };    
    static CX2Keyboard    m_instance;
    
    const int       m_pinCAPS = 2;
    const int       m_pinCODE = 3;
    const int       m_pinCLK = 4;
    const int       m_pinS[ sizeS ]   = { 7,6,5,8 };                // output (select column)
    const int       m_pinY[ sizeY ]   = { 7,6,5,8,  9,10,16,14 };   // input
    
    BYTE    m_readVal[ sizeX ];
};
