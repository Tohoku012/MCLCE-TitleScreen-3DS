#pragma once
#include <citro2d.h>


class Button {
public:
    void init();
    void update();
    void draw(C3D_RenderTarget* top, int state);
    void exit();
    bool isFinished() const;
    

private:
C2D_SpriteSheet graphicSheet;
C2D_SpriteSheet buttonSheet;

C2D_Sprite aSpr;
C2D_Sprite bSpr;
C2D_Sprite xSpr;
C2D_Sprite ySpr;

C2D_Sprite RedSpr;
C2D_Sprite BlueSpr;
C2D_Sprite YelloSpr;
C2D_Sprite GreenSpr;

    C2D_Font myMojangFont;
    C2D_Font myDFGFont;

    C2D_Text Selecttext;

    C2D_TextBuf dynamicBuf;
    bool isJapanese;

struct ButtonState {
    u32 key;            
    C2D_Sprite* sprite;   
    float originalY;    
    u64 pressedTime;     
};

ButtonState buttons[4];

bool finished;
};

struct ButtonMapping {
    u32 key;
    C2D_Sprite* sprite;
};


