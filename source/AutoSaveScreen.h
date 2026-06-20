#pragma once
#include <citro2d.h>


class AutoSaveScreen {
public:
    void init();
    void update();
    void draw(C3D_RenderTarget* bottom);
    void exit();
    bool isFinished() const;

private:
C2D_SpriteSheet panelSheet;
C2D_Sprite panelSpr;
C2D_SpriteSheet ButtonSheet;
C2D_Sprite ButtonSpr;
C2D_SpriteSheet SaveChestSheet;
C2D_Sprite SaveChestSpr;
C2D_SpriteSheet SaveArrowSheet;
C2D_Sprite SaveArrowSpr;

    C2D_Font myMojangFont;
    C2D_Font myDFGFont;
    C2D_Text autosavetext;
    C2D_Text oktext;
    C2D_TextBuf dynamicBuf;

bool finished;
float startY;

};