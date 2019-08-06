//
// Translate
// by cloudree@naver.com
//

class CTranslate
{
public:
    static CTranslate*      GetInstance()     { return &m_instance; }

    void    init();
    void    process();

protected:
    CTranslate()   { };
    void    CheckToggleCAPS( int x, int y );
    void    CheckToggleHANGUL( int x, int y );
    bool    IsPressed( int x, int y_ );

    static CTranslate    m_instance;

    bool    m_isCAPS;
    bool    m_isHANGUL;
    bool    m_oldShifted;
};
