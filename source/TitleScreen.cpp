#include "TitleScreen.h"
#include <3ds.h>

void TitleScreen::init()
{
    topSheet =
    C2D_SpriteSheetLoad(
        "romfs:/gfx/BackgroundTop.t3x"
    );

    bottomSheet =
    C2D_SpriteSheetLoad(
        "romfs:/gfx/BackgroundBottom.t3x"
    );

    scrollX = 0.0f;

    if (topSheet && bottomSheet)
    {
        C2D_SpriteFromSheet(&topBg, topSheet, 0);
        C2D_SpriteFromSheet(&bottomBg, bottomSheet, 0);
        C2D_SpriteSetPos(&topBg, 0, 0);
        C2D_SpriteSetPos(&bottomBg, 0, 0);
        C2D_SpriteSetScale(&topBg, 3.3333f, 3.3333f);
        C2D_SpriteSetScale(&bottomBg, 3.3333f, 3.3333f);

    }

    editionSheet =
    C2D_SpriteSheetLoad(
        "romfs:/gfx/New_Nintendo_3DS_Edition.t3x"
    );

if (editionSheet)
{
    C2D_SpriteFromSheet(
        &editionLogo,
        editionSheet,
        0
    );

    C2D_SpriteSetCenter(
        &editionLogo,
        0.5f,
        0.0f
    );

    C2D_SpriteSetPos(
        &editionLogo,
        200.0f,
        80.0f
    );

    C2D_SpriteSetScale(
        &editionLogo,
        0.375f,
        0.375f
    );
}
}

void TitleScreen::update()
{
        scrollX += 0.5f;

    if (scrollX > 2733.306f)
        scrollX = 0.0f;
}

void TitleScreen::draw(C3D_RenderTarget* top,C3D_RenderTarget* bottom)
{
    C2D_TargetClear(top, C2D_Color32(0,0,0,255));
    C2D_TargetClear(bottom, C2D_Color32(0,0,0,255));
    

    C2D_SceneBegin(top);

    if (topSheet)
    {
        C2D_SpriteSetPos(&topBg, -scrollX, 0);
        C2D_DrawSprite(&topBg);

        C2D_SpriteSetPos(&topBg, -scrollX + 2733.306, 0);
        C2D_DrawSprite(&topBg);
    }

    if (editionSheet)
{
    C2D_DrawSprite(&editionLogo);
}
    
    C2D_SceneBegin(bottom);

    if (bottomSheet)
    {
        C2D_SpriteSetPos(&bottomBg, -scrollX-40, 0);
        C2D_DrawSprite(&bottomBg);

        C2D_SpriteSetPos(&bottomBg, -scrollX-40 + 2733.306, 0);
        C2D_DrawSprite(&bottomBg);
    }


}