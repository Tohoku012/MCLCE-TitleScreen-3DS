#pragma once
#include <citro2d.h>


class MainMenuScreen {
public:
    void init();
    void update();
    void draw(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
    bool isFinished() const;
    ~MainMenuScreen();

private:
    C2D_Font myMojangFont;
    C2D_Font myDFGFont;
    C2D_Text splashtext;

    C2D_Text PlayGametext;
    C2D_Text Achievementstext;
    C2D_Text HelpOptionstext;
    C2D_Text Storetext;

    C2D_TextBuf dynamicBuf;
    bool finished;
    bool isJapanese;

    C2D_SpriteSheet ButtonSheet;
    C2D_Sprite ButtonSpr;
    C2D_Sprite ButtonOverSpr;
    int selectedIndex = 0;

    float textScale = 0.0f; 
    float animCounter = 0.0f; 
};