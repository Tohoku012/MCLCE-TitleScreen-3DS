#pragma once
#include <citro2d.h>


class LogoScreen {
public:
    void init();
    void update();
    void draw(C3D_RenderTarget* bottom);
    bool isFinished();

private:
C2D_SpriteSheet mojangSheet;
C2D_SpriteSheet fourJSheet;
C2D_ImageTint tint;
C2D_Sprite mojangSpr;
C2D_Sprite fourJSpr;

float timer;
float alpha;
u8 fadeAlpha;
int stage; // 0=mojang,1=4j,2=end
};