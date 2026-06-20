#pragma once
#include <citro2d.h>

class TitleScreen {
public:
    void init();
    void update();
    void draw(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
    float scrollX;

private:
    C2D_SpriteSheet topSheet;
    C2D_SpriteSheet bottomSheet;

    C2D_Sprite topBg;
    C2D_Sprite bottomBg;

    C2D_SpriteSheet editionSheet;
    C2D_Sprite editionLogo;
};