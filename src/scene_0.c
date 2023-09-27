#include "scene_0.h"
#include <GL/glu.h>
#include <math.h>
#include "config/config.h"
#include "fw/types.h"
#include "fw/render.h"
#include "fw/lut.h"
#include "fw/easing.h"
#include "fw/animation.h"
#include "fw/font.h"
#include "fw/projection.h"
#include "fw/broadcast.h"
#include "fw/sound.h"
#include "resource.h"
#include "fx/gradient.h"
#include "fx/glenz.h"
#include "fx/gradient_text.h"
#include "fx/starfield_2d.h"
#include "fx/gradient_border.h"
#include "fx/vector_logo.h"
#include "fx/vector_balls.h"
#include "fx/billboard.h"
#include "fx/hexagon.h"
#include "fx/texgen.h"


#define ANIM_VECTOR_LOGO 0
#define ANIM_CUBE 1
#define ANIM_COLOR_PLANES 2
#define ANIM_VECTOR_BALLS 3
#define ANIM_SCROLL_TEXT 4
#define ANIM_TITLE_LOGO 5
#define ANIM_GREETINGS 6
#define ANIM_HEXAGONS 7
#define ANIM_GURU 8

static BOOL _isTitleLogoAnimEnabled = FALSE;
static BOOL _isCubeAnimEnabled = FALSE;
static BOOL _isColorPlanesAnimEnabled = FALSE;
static BOOL _isVectorLogoAnimEnabled = FALSE;
static BOOL _isVectorBallsAnimEnabled = FALSE;
static BOOL _isGreetingsAnimEnabled = FALSE;
static BOOL _isHexagonsAnimEnabled = FALSE;
static BOOL _isGuruAnimEnabled = FALSE;

static fw_object_3d _vectorBallsMorph;
static fw_object_3d _vectorBallSrcObjects[4];

static fx_gradient _gradientRainbow = {
    .colors = (fw_vec4f[64]){},
    .numColors = 64,
};

static fx_gradient _gradientCredits = {
    .colors = (fw_vec4f[64]){},
    .numColors = 64,
};

static fx_gradient _gradientRed = {
    .colors = (fw_vec4f[64]){},
    .numColors = 64,
};


///////////////////////////////////////////////////////////////////////////////
// STRINGS
///////////////////////////////////////////////////////////////////////////////

static unsigned char _strScrollText[] =
    "gREETINGS, pROGRAMS!    "
    "aFTER 22 yEARS oF sILENCE wE aRE hAPPY tO pRESENT a 64K iNTRO cALLED . . .                     "
    "tHIS iS oLYMPIAN aT tHE kEYBOARD. oUR lAST pRODUCTION wAS iN 2001 fOR fROZAK aT bUENZLI pARTY 10 iN sWITZERLAND . . .                 "
    ". . . tIME fLIES . . .         sO aFTER a lITTLE hIATUS oF mORE tHAN tWO dECADES "
    "wE fELT tHAT tHE tIME hAS fINALLY cOME tO rELEASE sOMETHING nEW aGAIN ` ` `             "
    "tHIS iNTRO iS rENDERD iN gLORIOUS 320x200 pIXELS "
    "aND tHE mUSIC wAS tRACKED uSING sOME oF tHE oNCE uBIQUITOUS ST-XX sOUND sAMPLES.                 "
    ". . . aS yOU hAVE fIGURED bY nOW tHIS iNTRO wAS iNSPIRED bY vARIOUS oLD sCHOOL aMIGA cRACKTRO aND mEGADEMO cLASSICS "
    "fROM tHE lATE 80s aND eARLY 90s bY rED sECTOR, kEFRENS, fAIRLIGHT, nORTH sTAR, qUARTEX aND mANY mORE.            "
    "sPECIAL tHANKS gO tO jEFF / hXC2001 fOR tHE mOD pLAYER rOUTINE, nIGHTLIGHT / kEFRENS FOR SOME SAMPLES aND fERRIS / lOGICOMA fOR sQUISHY!                 "
    "~ ~ ~ gREETINGS gO oUT tO ~ ~ ~                      "
    "sADLY wE aRE aLREADY aPPROACHING tHE eND . . .                                                  "
    "sEE yOU nEXT tIME!           "
    ;
static int _strlenScrollText = sizeof(_strScrollText);


static unsigned char _strGreetingsText[] =
    "~ aLCATRAZ ~ aNARCHY ~ aNDROMEDA ~ aSD ~ cALODOX ~ cOMPLEX ~ fAIRLIGHT ~ fARBRAUSCH ~ fROZAK ~ hAUJOBB ~ kEFRENS "
    "~ mOODS pLATEAU ~ nORTH sTAR ~ pARADOX ~ qUARTEX ~ rAZOR 1911 ~ rEBELS ~ sANITY ~ sCOOPEX ~ sPACEBALLS ~ sCA ~ tHE sILENTS "
    "~ tRBL ~ tRSI ~ vANTAGE ~                  ... aND eVERYONE AT dEADLINE 2023!"
    ;
static int _strlenGreetingsText = sizeof(_strGreetingsText);

static unsigned char _creditsTexts[][6][256] = {
    {
        "cODE bY:",
        "       oLYMPIAN",
        "gRAPHICS bY:",
        "       oLYMPIAN",
        "mUSIC bY:",
        "       oLYMPIAN"
    },
    {
        "   sPECTROX",
        "     2023",
        "       ~",
        "   tHANK yOU",
        " fOR wATCHING!",
        ""
    }
};

///////////////////////////////////////////////////////////////////////////////
// INITAL KEYFRAME VARIABLES
///////////////////////////////////////////////////////////////////////////////

static const fw_vec4f _vectorLogoPosInit = {0};
static const fw_vec4f _vectorLogoRotInit = {0};
static const fw_vec4f _vectorLogoColorInit = {1,1,1,1};

static const fw_vec4f _titleLogoPosInit = {-260, 29};
static const fw_vec4f _titleLogoAmplitudeInit = {0};
static const fw_vec4f _titleLogoDistortInit = {0};
static const fw_vec4f _titleLogoDistortModeInit = {0};

static const fw_vec4f _cubePosInit = {-8,0,-8,0};
static const fw_vec4f _planePosInit = {8.5,0,-7.1,0};

static const fw_vec4f _colorPlanesPosInit = {-8,0,-7,0};

static const fw_vec4f _vectorBallsMorphObjIndexesInit = {0,1};
static const fw_vec4f _vectorBallsMorphPercentageInit = {0};
static const fw_vec4f _vectorBallsPosInit = {-9,-9};


static const fw_vec4f _greetingsTextPosXInit = {FW_RENDER_WIDTH + 10};
static const fw_vec4f _greetingsTextPosYInit = {20};
static const fw_vec4f _greetingsTextPosOffsetInit = {8, 24};
static const fw_vec4f _greetingsTextXSineInit = {0};
static const fw_vec4f _greetingsTextYSineInit = {0,4,.2};

static const fw_vec4f _hexagonsPosInit = {4,15,-30};
static const fw_vec4f _hexagonsDistanceInit = {0};

static const fw_vec4f _hexagonsRotYInit = {0};
static const fw_vec4f _hexagonsRotZInit = {0};
static const fw_vec4f _hexagonsRotSinXYZInit = {0};
static const fw_vec4f _hexagonsRotSinXInit = {0};
static const fw_vec4f _hexagonsPos2Init = {0};

static const fw_vec4f _creditsPageNrInit = {0};
static const fw_vec4f _creditsPosLine1Init = {0};
static const fw_vec4f _creditsPosLine2Init = {0};
static const fw_vec4f _creditsPosLine3Init = {0};
static const fw_vec4f _creditsPosLine4Init = {0};
static const fw_vec4f _creditsPosLine5Init = {0};
static const fw_vec4f _creditsPosLine6Init = {0};

static const fw_vec4f _scrollTextPosInit = {FW_RENDER_WIDTH + 10};

static const fw_vec4f _guruPosInit = {0,19,-60.480545f};
static const fw_vec4f _guruRotInit = {0};
static const fw_vec4f _guruScaleInit = {1,1,1};

///////////////////////////////////////////////////////////////////////////////
// KEY FRAME VARIABLES
///////////////////////////////////////////////////////////////////////////////

static fw_vec4f _vectorLogoPos = _vectorLogoPosInit;
static fw_vec4f _vectorLogoRot = _vectorLogoRotInit;
static fw_vec4f _vectorLogoColor = _vectorLogoColorInit;

static fw_vec4f _titleLogoPos = _titleLogoPosInit;
static fw_vec4f _titleLogoAmplitude = _titleLogoAmplitudeInit;
static fw_vec4f _titleLogoDistort = _titleLogoDistortInit;
static fw_vec4f _titleLogoDistortMode = _titleLogoDistortModeInit;

static fw_vec4f _cubePos = _cubePosInit;
static fw_vec4f _planePos = _planePosInit;

static fw_vec4f _colorPlanesPos = _colorPlanesPosInit;

static fw_vec4f _vectorBallsMorphObjIndexes = _vectorBallsMorphObjIndexesInit;
static fw_vec4f _vectorBallsMorphPercentage = _vectorBallsMorphPercentageInit;
static fw_vec4f _vectorBallsPos = _vectorBallsPosInit;

static fw_vec4f _greetingsTextPosX = _greetingsTextPosXInit;
static fw_vec4f _greetingsTextPosY = _greetingsTextPosYInit;
static fw_vec4f _greetingsTextPosOffset = _greetingsTextPosOffsetInit;
static fw_vec4f _greetingsTextXSine = _greetingsTextXSineInit;
static fw_vec4f _greetingsTextYSine = _greetingsTextYSineInit;

static fw_vec4f _hexagonsPos = _hexagonsPosInit;
static fw_vec4f _hexagonsDistance = _hexagonsDistanceInit;
static fw_vec4f _hexagonsRotY = _hexagonsRotYInit;
static fw_vec4f _hexagonsRotZ = _hexagonsRotZInit;
static fw_vec4f _hexagonsRotSinXYZ = _hexagonsRotSinXYZInit;
static fw_vec4f _hexagonsRotSinX = _hexagonsRotSinXInit;
static fw_vec4f _hexagonsPos2 = _hexagonsPos2Init;

static fw_vec4f _creditsPageNr = _creditsPageNrInit;
static fw_vec4f _creditsPosLine1 = _creditsPosLine1Init;
static fw_vec4f _creditsPosLine2 = _creditsPosLine2Init;
static fw_vec4f _creditsPosLine3 = _creditsPosLine3Init;
static fw_vec4f _creditsPosLine4 = _creditsPosLine4Init;
static fw_vec4f _creditsPosLine5 = _creditsPosLine5Init;
static fw_vec4f _creditsPosLine6 = _creditsPosLine6Init;

static fw_vec4f _scrollTextPos = _scrollTextPosInit;

static fw_vec4f _guruPos = _guruPosInit;
static fw_vec4f _guruRot = _guruRotInit;
static fw_vec4f _guruScale = _guruScaleInit;

///////////////////////////////////////////////////////////////////////////////
// ANIM START / STOPP DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

static void animStartedVectorLogo(fw_animation *anim, fw_timer_data *time);
static void animCompletedVectorLogo(fw_animation *anim, fw_timer_data *time);

static void animStartedTitleLogo(fw_animation *anim, fw_timer_data *time);
static void animCompletedTitleLogo(fw_animation *anim, fw_timer_data *time);

static void animStartedCube(fw_animation *anim, fw_timer_data *time);
static void animCompletedCube(fw_animation *anim, fw_timer_data *time);

static void animStartedColorPlanes(fw_animation *anim, fw_timer_data *time);
static void animCompletedColorPlanes(fw_animation *anim, fw_timer_data *time);

static void animStartedVectorBalls(fw_animation *anim, fw_timer_data *time);
static void animCompletedVectorBalls(fw_animation *anim, fw_timer_data *time);

static void animStartedScrollText(fw_animation *anim, fw_timer_data *time);
static void animCompletedScrollText(fw_animation *anim, fw_timer_data *time);

static void animStartedGreetingsText(fw_animation *anim, fw_timer_data *time);
static void animCompletedGreetingsText(fw_animation *anim, fw_timer_data *time);

static void animStartedHexagons(fw_animation *anim, fw_timer_data *time);
static void animCompletedHexagons(fw_animation *anim, fw_timer_data *time);

static void animStartedGuru(fw_animation *anim, fw_timer_data *time);
static void animCompletedGuru(fw_animation *anim, fw_timer_data *time);

///////////////////////////////////////////////////////////////////////////////
// KEY FRAMES
///////////////////////////////////////////////////////////////////////////////

// Vector logo.
static fw_keyframe _keysVectorLogo[] = {
    {1, 4, _vectorLogoPosInit, {0,0,-2}, LinearInterpolation, &_vectorLogoPos},
    {13, 15, {0,0,-2}, {.5,0,-1.5}, QuadraticEaseInOut, &_vectorLogoPos},
    {16, 21, {.5,0,-1.5}, {-.5,0,-2.5}, QuadraticEaseInOut, &_vectorLogoPos},
    {22, 27, {-.5,0,-2.5}, {0,0,-2}, QuadraticEaseInOut, &_vectorLogoPos},
    {28.5, 30.5, {0,0,-2}, {10,0,-20}, QuadraticEaseIn, &_vectorLogoPos},

    {4, 9, _vectorLogoRotInit, {0,1,0, 360*2}, LinearInterpolation, &_vectorLogoRot},
    {9, 12, {0}, {-1,0,0, 360*2}, LinearInterpolation, &_vectorLogoRot},
    {12, 18, {0}, {-.7,.5,.8, 360*2}, LinearInterpolation, &_vectorLogoRot},
    {18, 24, {-.7,.5,.8, 0}, {-.2,.8,1.2, 360*2}, LinearInterpolation, &_vectorLogoRot},
    {24, 30.5, {-.2,.8,1.2, 0}, {1,0,0, 360*2}, LinearInterpolation, &_vectorLogoRot},

    {29.5, 30.5, _vectorLogoColorInit, {0,0,0,1}, LinearInterpolation, &_vectorLogoColor},
};

// Title logo.
static fw_keyframe _keysTitleLogo[] = {
    {1, 2, _titleLogoPosInit, {36, 45}, QuadraticEaseOut, &_titleLogoPos},
    {20.2, 21.2, {36, 45}, {450, 39}, QuadraticEaseIn, &_titleLogoPos},

    {0, 4, _titleLogoAmplitudeInit, {50, 18}, QuadraticEaseOut, &_titleLogoAmplitude},
    {6, 10, {50, 18}, {20, 18}, QuadraticEaseOut, &_titleLogoAmplitude},
    {10, 18, {20, 18}, {50, 18}, QuadraticEaseOut, &_titleLogoAmplitude},

    {6, 11, {170,370, 4, 30}, {170-370+30,30, 4, 30}, LinearInterpolation, &_titleLogoDistort},
    {13, 18, {170,270, 2, 20}, {170-270+30,30, 2, 20}, LinearInterpolation, &_titleLogoDistort},

    {6, 6, {1}, {1}, LinearInterpolation, &_titleLogoDistortMode},
    {12, 12, {2}, {2}, LinearInterpolation, &_titleLogoDistortMode},
    {18, 18, {0}, {0}, LinearInterpolation, &_titleLogoDistortMode},
};

// Cube and plane.
static fw_keyframe _keysCube[] = {
    {1, 4, _cubePosInit, {0,0,-8,0}, BackEaseOut, &_cubePos},
    {5, 6, _planePosInit, {0,0,-7.1,0}, SineEaseOut, &_planePos},

    {30, 30.5, {0,0,-7.1,0}, {-1,0,-7.1,0}, SineEaseInOut, &_planePos},
    {30.5, 32, {-1,0,-7.1,0}, {18,0,-20,0}, SineEaseIn, &_planePos},
    {33, 35, {0,0,-8,0}, {8,0,-8,0}, BackEaseIn, &_cubePos},
};

// Color planes.
static fw_keyframe _keysColorPlanes[] = {
    {1, 4, _colorPlanesPosInit, {0,0,-7,0}, SineEaseOut, &_colorPlanesPos},
    {15, 15.5, {0,0,-7,0}, {0,-1,-7,0}, SineEaseInOut, &_colorPlanesPos},
    {15.5, 16.5, {0,-1,-7,0}, {0,9,-10,0}, SineEaseIn, &_colorPlanesPos},
    {17.5, 20, {0,9,-12,0}, {3,-9,-20,0}, LinearInterpolation, &_colorPlanesPos},
};

// Vector balls.
static fw_keyframe _keysVectorBalls[] = {
    {1, 4, _vectorBallsPosInit, {0}, QuadraticEaseOut, &_vectorBallsPos},
    {55, 59, {0}, {9,-12, 0}, QuadraticEaseIn, &_vectorBallsPos},

    {10, 13, {0}, {1}, ElasticEaseInOut, &_vectorBallsMorphPercentage},
    {23, 26, {0}, {1}, ElasticEaseInOut, &_vectorBallsMorphPercentage},
    {36, 39, {0}, {1}, ElasticEaseInOut, &_vectorBallsMorphPercentage},
    {49, 52, {0}, {1}, ElasticEaseInOut, &_vectorBallsMorphPercentage},

    {23, 23, _vectorBallsMorphObjIndexesInit, {1,2}, LinearInterpolation, &_vectorBallsMorphObjIndexes},
    {36, 36, _vectorBallsMorphObjIndexesInit, {2,3}, LinearInterpolation, &_vectorBallsMorphObjIndexes},
    {49, 49, _vectorBallsMorphObjIndexesInit, {3,0}, LinearInterpolation, &_vectorBallsMorphObjIndexes},
};

// Greetings.
static fw_keyframe _keysGreetingsText[] = {
    {4, 49, _greetingsTextPosXInit, {-5050}, LinearInterpolation, &_greetingsTextPosX},

    {20, 22, {0,4,.2}, {32,4,.2}, QuadraticEaseInOut, &_greetingsTextYSine},
    {23, 26, {32,4,.2}, {0,4,.2}, QuadraticEaseInOut, &_greetingsTextYSine},

    {29, 31, {0,4,.2}, {32,4,.2}, QuadraticEaseInOut, &_greetingsTextYSine},
    {32, 36, {32,4,.2}, {0,4,.2}, QuadraticEaseInOut, &_greetingsTextYSine},


    {28, 30, _greetingsTextPosOffsetInit, {-8,24}, QuadraticEaseInOut, &_greetingsTextPosOffset},
    {39, 44, {-8,24}, _greetingsTextPosOffsetInit, QuadraticEaseInOut, &_greetingsTextPosOffset},


    {20, 22, {0,3,.4}, {64,3,.4}, QuadraticEaseInOut, &_greetingsTextXSine},
    {23, 26, {64,3,.4}, {0,3,.4}, QuadraticEaseInOut, &_greetingsTextXSine},

    {28, 30, {0,3,.4}, {64,3,.4}, QuadraticEaseInOut, &_greetingsTextXSine},
    {32, 36, {64,3,.4}, {0,3,.4}, QuadraticEaseInOut, &_greetingsTextXSine},
};

// Hexagons.
static fw_keyframe _keysHexagons[] = {
    {0, 3, _hexagonsPosInit, {0,-2,-20}, SineEaseOut, &_hexagonsPos},
    {6, 8, {0,-2,-20}, {0,0,-10}, SineEaseInOut, &_hexagonsPos},
    {37, 45, {0,0,-10}, {0,0,-20}, SineEaseInOut, &_hexagonsPos},
    {45, 48, {0,0,-20}, {-2,-4,-20}, SineEaseInOut, &_hexagonsPos},
    {48, 50, {-2,-4,-20}, {0,0,-20}, SineEaseInOut, &_hexagonsPos},
    {50, 53, {0,0,-20}, _hexagonsPosInit, SineEaseIn, &_hexagonsPos},

    {0, 53, {0,0,0,0}, {0,0,0,360 * 16}, LinearInterpolation, &_hexagonsRotY},

    {0, 10, {0,0,0,0}, {0,0,0,10}, QuadraticEaseInOut, &_hexagonsRotSinX},
    {26, 28, {0,0,0,10}, {0,0,0,30}, QuadraticEaseInOut, &_hexagonsRotSinX},
    {28, 30, {0,0,0,30}, {0,0,0,10}, QuadraticEaseInOut, &_hexagonsRotSinX},

    {0, 5, {1,1,1,50}, {1,1,1,50}, QuadraticEaseInOut, &_hexagonsRotSinXYZ},
    {5, 10, {1,1,1,50}, {.5,1,.7,50}, QuadraticEaseInOut, &_hexagonsRotSinXYZ},
    {13, 17, {.5,1,.7,50}, {0}, QuadraticEaseInOut, &_hexagonsRotSinXYZ},
    {37, 43, {0}, {1,0,0, 50}, QuadraticEaseInOut, &_hexagonsRotSinXYZ},
    {45, 52, {1,0,0, 50}, {.2,.6,0, 50}, QuadraticEaseInOut, &_hexagonsRotSinXYZ},

    {0, 5, {0,0,-2}, {0,0,-2}, QuadraticEaseInOut, &_hexagonsPos2},
    {5, 8, {0,0,-2}, {1,-1,-1}, QuadraticEaseInOut, &_hexagonsPos2},
    {12, 14, {1,-1,-1}, {0}, QuadraticEaseInOut, &_hexagonsPos2},
    {37, 41, {0}, {-2,1,2}, QuadraticEaseInOut, &_hexagonsPos2},
    {41, 45, {-2,1,2}, {-2,3,-1}, QuadraticEaseInOut, &_hexagonsPos2},

    {16, 20, {0,0,0,0}, {0,0,0,90}, QuadraticEaseInOut, &_hexagonsRotZ},
    {26, 30, {0,0,0,90}, {0,0,0,-90}, QuadraticEaseInOut, &_hexagonsRotZ},
    {37, 53, {0,0,0,-90}, {0,0,0,90}, QuadraticEaseInOut, &_hexagonsRotZ},

    {12, 14, _hexagonsDistanceInit, {0.2}, QuadraticEaseInOut, &_hexagonsDistance},
    {16, 21, {0.2}, {1.2}, QuadraticEaseInOut, &_hexagonsDistance},
    {26, 28, {1.2}, {0.2}, QuadraticEaseInOut, &_hexagonsDistance},
    {28, 30, {0.2}, {1.2}, QuadraticEaseInOut, &_hexagonsDistance},
    {37, 41, {1.2}, {-.6}, QuadraticEaseInOut, &_hexagonsDistance},
    {47, 49, {-.6}, {0}, QuadraticEaseInOut, &_hexagonsDistance},

    {17.5, 22.5, {100, -120}, {100, 56}, BounceEaseInOut, &_creditsPosLine1},
    {17.4, 22.4, {100, -120}, {100, 68}, BounceEaseInOut, &_creditsPosLine2},
    {17.3, 22.3, {100, -120}, {100, 92}, BounceEaseInOut, &_creditsPosLine3},
    {17.2, 22.2, {100, -120}, {100, 104}, BounceEaseInOut, &_creditsPosLine4},
    {17.1, 22.1, {100, -120}, {100, 128}, BounceEaseInOut, &_creditsPosLine5},
    {17.0, 22.0, {100, -120}, {100, 140}, BounceEaseInOut, &_creditsPosLine6},

    {26.0, 27.0, {100, 56}, {100, 334}, QuadraticEaseIn, &_creditsPosLine1},
    {25.8, 26.8, {100, 68}, {100, 322}, QuadraticEaseIn, &_creditsPosLine2},
    {25.6, 26.6, {100, 92}, {100, 298}, QuadraticEaseIn, &_creditsPosLine3},
    {25.4, 26.4, {100, 104}, {100, 286}, QuadraticEaseIn, &_creditsPosLine4},
    {25.2, 26.2, {100, 128}, {100, 262}, QuadraticEaseIn, &_creditsPosLine5},
    {25.0, 26.0, {100, 140}, {100, 250}, QuadraticEaseIn, &_creditsPosLine6},

    {27.0, 27.0, {1}, {1}, LinearInterpolation, &_creditsPageNr},

    {27.5, 32.5, {100, -120}, {100, 56}, BounceEaseInOut, &_creditsPosLine1},
    {27.4, 32.4, {100, -120}, {100, 68}, BounceEaseInOut, &_creditsPosLine2},
    {27.3, 32.3, {98, -120}, {98, 92}, BounceEaseInOut, &_creditsPosLine3},
    {27.2, 32.2, {100, -120}, {100, 116}, BounceEaseInOut, &_creditsPosLine4},
    {27.1, 32.1, {100, -120}, {100, 128}, BounceEaseInOut, &_creditsPosLine5},
    {27.0, 32.0, {100, -120}, {100, 140}, BounceEaseInOut, &_creditsPosLine6},

    {37.0, 38.0, {100, 56}, {100, 334}, QuadraticEaseIn, &_creditsPosLine1},
    {36.8, 37.8, {100, 68}, {100, 322}, QuadraticEaseIn, &_creditsPosLine2},
    {36.6, 37.6, {98, 92}, {98, 298}, QuadraticEaseIn, &_creditsPosLine3},
    {36.4, 37.4, {100, 116}, {100, 274}, QuadraticEaseIn, &_creditsPosLine4},
    {36.2, 37.2, {100, 128}, {100, 262}, QuadraticEaseIn, &_creditsPosLine5},
    {36.0, 37.0, {100, 140}, {100, 250}, QuadraticEaseIn, &_creditsPosLine6},
};

// Guru.
static fw_keyframe _keysGuru[] = {
    {3.75+3, 3.75+8, _guruPosInit, {0,-60, -70}, LinearInterpolation, &_guruPos},

    {3.75+3, 3.75+6, _guruScaleInit, {0.3f,.03f, .3f}, SineEaseIn, &_guruScale},

    {3.75+3, 3.75+4, {1,0,0,0}, {1,.2f,0,180}, LinearInterpolation, &_guruRot},
    {3.75+4, 3.75+7.5, {1,.2f,0,180}, {.2f,.7f,.3f,180*4}, LinearInterpolation, &_guruRot},
};

// Text scroller.
static fw_keyframe _keysScrollText[] = {
    {14, 17, _scrollTextPosInit, {4}, LinearInterpolation, &_scrollTextPos},
    {20, 32, {5}, {-1570}, LinearInterpolation, &_scrollTextPos},
    {61, 171, {-1570}, {-15700}, LinearInterpolation, &_scrollTextPos},
    {254, 266, {-15700}, {-18800}, LinearInterpolation, &_scrollTextPos},

    {215+10, 215+19, {-15700}, {-16800}, LinearInterpolation, &_scrollTextPos},
    {215+38, 215+52, {-16800}, {-18500}, LinearInterpolation, &_scrollTextPos},
};

///////////////////////////////////////////////////////////////////////////////
// ANIMS
///////////////////////////////////////////////////////////////////////////////

static fw_animation _anims[] = {
    [ANIM_VECTOR_LOGO] = {
        .isAutostart = TRUE,
        .keyframes = _keysVectorLogo,
        .numKeyframes = LEN(_keysVectorLogo),
        .started_proc = animStartedVectorLogo,
        .completed_proc = animCompletedVectorLogo,
    },
    [ANIM_TITLE_LOGO] = {
        .isAutostart = FALSE,
        .keyframes = _keysTitleLogo,
        .numKeyframes = LEN(_keysTitleLogo),
        .started_proc = animStartedTitleLogo,
        .completed_proc = animCompletedTitleLogo,
    },
    [ANIM_CUBE] = {
        .isAutostart = FALSE,
        .keyframes = _keysCube,
        .numKeyframes = LEN(_keysCube),
        .started_proc = animStartedCube,
        .completed_proc = animCompletedCube,
    },
    [ANIM_COLOR_PLANES] = {
        .isAutostart = FALSE,
        .keyframes = _keysColorPlanes,
        .numKeyframes = LEN(_keysColorPlanes),
        .started_proc = animStartedColorPlanes,
        .completed_proc = animCompletedColorPlanes,
    },
    [ANIM_VECTOR_BALLS] = {
        .isAutostart = FALSE,
        .keyframes = _keysVectorBalls,
        .numKeyframes = LEN(_keysVectorBalls),
        .started_proc = animStartedVectorBalls,
        .completed_proc = animCompletedVectorBalls,
    },
    [ANIM_GREETINGS] = {
        .isAutostart = FALSE,
        .keyframes = _keysGreetingsText,
        .numKeyframes = LEN(_keysGreetingsText),
        .started_proc = animStartedGreetingsText,
        .completed_proc = animCompletedGreetingsText,
    },
    [ANIM_HEXAGONS] = {
        .isAutostart = FALSE,
        .keyframes = _keysHexagons,
        .numKeyframes = LEN(_keysHexagons),
        .started_proc = animStartedHexagons,
        .completed_proc = animCompletedHexagons,
    },
    [ANIM_GURU] = {
        .isAutostart = FALSE,
        .keyframes = _keysGuru,
        .numKeyframes = LEN(_keysGuru),
        .started_proc = animStartedGuru,
        .completed_proc = animCompletedGuru,
    },
    [ANIM_SCROLL_TEXT] = {
        .isAutostart = FALSE,
        .keyframes = _keysScrollText,
        .numKeyframes = LEN(_keysScrollText),
        .started_proc = animStartedScrollText,
        .completed_proc = animCompletedScrollText,
    },
};

static fw_animation_schedule _animSchedule = {
    _anims,
    LEN(_anims)
};

///////////////////////////////////////////////////////////////////////////////
// ANIM START / STOP IMPLEMENTATIONS
///////////////////////////////////////////////////////////////////////////////

static void animStartedVectorLogo(fw_animation *anim, fw_timer_data *time) {
    _isVectorLogoAnimEnabled = TRUE;
    _vectorLogoPos = _vectorLogoPosInit;
    _vectorLogoRot = _vectorLogoRotInit;
    _vectorLogoColor = _vectorLogoColorInit;
    fw_animation_start(&_animSchedule.animations[ANIM_SCROLL_TEXT], time);
}

static void animCompletedVectorLogo(fw_animation *anim, fw_timer_data *time) {
    _isVectorLogoAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_TITLE_LOGO], time);
}

static void animStartedTitleLogo(fw_animation *anim, fw_timer_data *time) {
    _isTitleLogoAnimEnabled = TRUE;
    _titleLogoDistortMode = _titleLogoDistortModeInit;
}

static void animCompletedTitleLogo(fw_animation *anim, fw_timer_data *time) {
    _isTitleLogoAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_CUBE], time);
}

static void animStartedCube(fw_animation *anim, fw_timer_data *time) {
    _isCubeAnimEnabled = TRUE;
}

static void animCompletedCube(fw_animation *anim, fw_timer_data *time) {
    _isCubeAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_COLOR_PLANES], time);
}

static void animStartedColorPlanes(fw_animation *anim, fw_timer_data *time) {
    _isColorPlanesAnimEnabled = TRUE;
}

static void animCompletedColorPlanes(fw_animation *anim, fw_timer_data *time) {
    _isColorPlanesAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_VECTOR_BALLS], time);
}

static void animStartedVectorBalls(fw_animation *anim, fw_timer_data *time) {
    _isVectorBallsAnimEnabled = TRUE;
    _vectorBallsMorphPercentage = _vectorBallsMorphPercentageInit;
    _vectorBallsMorphObjIndexes = _vectorBallsMorphObjIndexesInit;
}

static void animCompletedVectorBalls(fw_animation *anim, fw_timer_data *time) {
    _isVectorBallsAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_GREETINGS], time);
}

static void animStartedGreetingsText(fw_animation *anim, fw_timer_data *time) {
    _isGreetingsAnimEnabled = TRUE;
}

static void animCompletedGreetingsText(fw_animation *anim, fw_timer_data *time) {
    _isGreetingsAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_HEXAGONS], time);
}

static void animStartedHexagons(fw_animation *anim, fw_timer_data *time) {
    _isHexagonsAnimEnabled = TRUE;

    _hexagonsPos = _hexagonsPosInit;
    _hexagonsDistance = _hexagonsDistanceInit;
    _hexagonsRotY = _hexagonsRotYInit;
    _hexagonsRotZ = _hexagonsRotZInit;
    _hexagonsRotSinXYZ = _hexagonsRotSinXYZInit;
    _hexagonsRotSinX = _hexagonsRotSinXInit;
    _hexagonsPos2 = _hexagonsPos2Init;

    _creditsPageNr = _creditsPageNrInit;
    _creditsPosLine1 = _creditsPosLine1Init;
    _creditsPosLine2 = _creditsPosLine2Init;
    _creditsPosLine3 = _creditsPosLine3Init;
    _creditsPosLine4 = _creditsPosLine4Init;
    _creditsPosLine5 = _creditsPosLine5Init;
    _creditsPosLine6 = _creditsPosLine6Init;
}

static void animCompletedHexagons(fw_animation *anim, fw_timer_data *time) {
    _isHexagonsAnimEnabled = FALSE;
    fw_animation_start(&_animSchedule.animations[ANIM_GURU], time);
}

static void animStartedGuru(fw_animation *anim, fw_timer_data *time) {
    _isGuruAnimEnabled = TRUE;
    if (FW_IS_MUSIC_ENABLED) {
        fw_sound_shutdown();
    }
}

static void animCompletedGuru(fw_animation *anim, fw_timer_data *time) {
    fw_broadcast_exit();
}

static void animStartedScrollText(fw_animation *anim, fw_timer_data *time) {

}

static void animCompletedScrollText(fw_animation *anim, fw_timer_data *time) {
}

///////////////////////////////////////////////////////////////////////////////
// INIT
///////////////////////////////////////////////////////////////////////////////

void scene_0_init() {
    fw_animation_resetAnimationSchedule(&_animSchedule);

    fx_gradient_spread(fx_gradient_getRainbow(), &_gradientRainbow, TRUE);

    fw_vec4f creditsGradientTemplateColors[] = {
        {.4,.4,.4},
        {1,1,1},
        {1,1,1},
        {.75,.75,.75},
        {1,1,1},
    };
    fx_gradient creditsGradientTemplate = {
        .colors = creditsGradientTemplateColors,
        .numColors = LEN(creditsGradientTemplateColors),
    };
    fx_gradient_spread(&creditsGradientTemplate, &_gradientCredits, TRUE);


    fw_vec4f redGradientTemplateColors[] = {
        {1,0,0},
        {1,.8,.8},
        {1,0,0},
        {1,0,0},
        {1,.8,.8},
    };
    fx_gradient redGradientTemplate = {
        .colors = redGradientTemplateColors,
        .numColors = LEN(redGradientTemplateColors),
    };
    fx_gradient_spread(&redGradientTemplate, &_gradientRed, TRUE);

    fx_starfield_2d_init();

    int spriteIndexes[] = {RES_SPRITE_INDEX_VEC_BALL_BLUE_NORMAL, RES_SPRITE_INDEX_VEC_BALL_YELLOW_SMALL};
    _vectorBallSrcObjects[0] = fx_vector_balls_createCube(5,5,5, 0.5f, spriteIndexes, LEN(spriteIndexes));
    _vectorBallSrcObjects[1] = fx_vector_balls_createSphere(13,10,1.4, spriteIndexes, LEN(spriteIndexes));
    _vectorBallSrcObjects[2] = fx_vector_balls_createPyramid(7, .5f, .6f, spriteIndexes, LEN(spriteIndexes));
    _vectorBallSrcObjects[3] = fx_vector_balls_createCube(12,12,1, 0.5f, spriteIndexes, LEN(spriteIndexes));

    _vectorBallsMorph = fx_vector_balls_createMorphTarget(_vectorBallSrcObjects, LEN(_vectorBallSrcObjects), spriteIndexes, LEN(spriteIndexes));

    fx_texgen_eye_linear_init();

    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);

}

static void renderColorPlanes(fw_timer_data *time) {
    glDisable(GL_DEPTH_TEST);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPushMatrix();
    {

        glTranslatef(_colorPlanesPos.x,_colorPlanesPos.y,_colorPlanesPos.z);
        glRotatef(time->elapsed*80.f, -2.f * sin(time->elapsed), -1.5f, 2.0f);
        glRotatef(50 + 30 * fw_lut_sin(time->elapsed * .5f), 1,1,0);

        float spacing = 1;

        glPushMatrix();
        glTranslatef(0,0,-2 * spacing);
        glScalef(.5,.5,.5);
        glBegin(GL_QUADS);
        {
            glColor4f(1.0f, 0.0f, 0.0f, .8f);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,0,-1 * spacing);
        glScalef(.75,.75,.75);
        glBegin(GL_QUADS);
        {
            glColor4f(1.0f, 0.5f, 0.0f, .8f);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,0,0);
        glScalef(1,1,1);
        glBegin(GL_QUADS);
        {

            glColor4f(1.0f, 1.0f, 0.0f, .8f);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,0,1 * spacing);
        glScalef(.75,.75,.75);
        glBegin(GL_QUADS);
        {

            glColor4f(0.0f, 1.0f, 0.0f, .8f);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,0,2 * spacing);
        glScalef(.5,.5,.5);
        glBegin(GL_QUADS);
        {
            glColor4f(0.0f, 0.0f, 1.0f, .8f);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

static void renderPlane(fw_timer_data *time) {
    glPushMatrix();

    glTranslatef(_planePos.x,_planePos.y,_planePos.z);
    glScalef(2,2,2);
    glRotatef(time->elapsed*80.f, -2.f * sin(time->elapsed), -1.5f, 2.0f);
    glBegin(GL_QUADS);
    glColor4f(0,0,1,.8f);
    glVertex3f(-1, 1, 0);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(1, 1, 0);
    glEnd();

    glPopMatrix();
}

static void renderGlenzCube(fw_timer_data *time) {
    glPushAttrib(GL_LIGHTING_BIT);
    glShadeModel(GL_FLAT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[]) {
        -2, 2, 5, 0
        });

    glPushMatrix();
    glTranslatef(_cubePos.x, _cubePos.y, _cubePos.z);
    glRotatef(time->elapsed*50.f, 4.f * sin(time->elapsed), 2.0f, 3.0f);
    fw_object_3d_put(&getGlenzCubes()[0]);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glPopAttrib();
}


static void renderGreetingsText(fw_timer_data *time) {
    const float t = time->elapsed - _animSchedule.animations[ANIM_GREETINGS].timeStartedAt;

    fw_image *srcImage = getRGBABuffer2();
    fw_font_face *face = getDefaultFontFace();

    fx_gradient_text_properties props = {
        .target = srcImage,
        .text = _strGreetingsText,
        .textLength = _strlenGreetingsText,
        .fontFace = face,
        .fontSize = 2,
        .gradient = &_gradientRainbow,
        .gradientOffset = 32.f * fw_lut_sin(4.f * t),
        .outputXMin = -40,
        .outputXMax = FW_RENDER_WIDTH + 40
    };

    props.xSineAmplitude = _greetingsTextXSine.x;
    props.xSineFrequency = _greetingsTextXSine.y;
    props.xSineCharacterOffsetFactor = _greetingsTextXSine.z;

    props.ySineAmplitude = _greetingsTextYSine.x;
    props.ySineFrequency = _greetingsTextYSine.y;
    props.ySineCharacterOffsetFactor = _greetingsTextYSine.z;

    props.xStart = _greetingsTextPosX.x;
    props.yStart = _greetingsTextPosY.x;

    fx_gradient_text_writeBegin(srcImage);

    for (int i=0; i<5; i++) {
        if (i%2) {
            props.gradient = &_gradientCredits;
        } else {
            props.gradient = &_gradientRainbow;
        }
        props.yStart += _greetingsTextPosOffset.y;
        props.xStart += _greetingsTextPosOffset.x;

        fx_gradient_text_write(&props, t);
        props.xStart += 1;
        fx_gradient_text_write(&props, t);
    }

    fw_image *dstImage = getRGBABuffer();

    for (int x=0; x<dstImage->width; x++) {
        int yoff = (int)(32.f * fw_lut_sin(t * 4.f + x *.01f) + .5f);
        for (int y=0; y<dstImage->height; y++) {
            int dstIdx = dstImage->bytesPerPixel * (x + y * dstImage->width);

            const int dstY = (y + yoff);
            if (x < 0 || x >= dstImage->width || dstY < 0 || dstY >= dstImage->height) {
                dstImage->pixels[dstIdx] = 0;
                dstImage->pixels[dstIdx+1] = 0;
                dstImage->pixels[dstIdx+2] = 0;
                dstImage->pixels[dstIdx+3] = 0;
                continue;
            }
            int srcdx = srcImage->bytesPerPixel * (x + dstY * srcImage->width);

            dstImage->pixels[dstIdx] = srcImage->pixels[srcdx];
            dstImage->pixels[dstIdx+1] = srcImage->pixels[srcdx+1];
            dstImage->pixels[dstIdx+2] = srcImage->pixels[srcdx+2];
            dstImage->pixels[dstIdx+3] = srcImage->pixels[srcdx+3];
        }
    }

    fx_gradient_text_writeEnd(dstImage);
}

static void renderScrollText(fw_timer_data *time) {
    fw_image *image = getRGBABuffer();
    fw_font_face *face = getDefaultFontFace();

    fx_gradient_text_properties props = {
        .target = image,
        .text = _strScrollText,
        .textLength = _strlenScrollText,
        .fontFace = face,
        .fontSize = 2,
        .gradient = &_gradientRainbow,
        .gradientOffset = 24,
    };

    props.xStart = _scrollTextPos.x;
    props.yStart = 174.f;

    props.ySineAmplitude = 8;
    props.ySineFrequency = 4;
    props.ySineCharacterOffsetFactor = 0.2f;

    fx_gradient_text_writeBegin(image);
    fx_gradient_text_write(&props, time->elapsed);
    fx_gradient_text_writeEnd(image);
}

static void renderSpectrox(fw_timer_data *time) {
    glColor3f(_vectorLogoColor.x,_vectorLogoColor.y,_vectorLogoColor.z);
    glTranslatef(_vectorLogoPos.x, _vectorLogoPos.y, _vectorLogoPos.z);
    glRotatef(_vectorLogoRot.w, _vectorLogoRot.x,_vectorLogoRot.y,_vectorLogoRot.z);
    glScalef(3,3,3);
    fx_vector_logo_put();
}


static void renderTitle(fw_timer_data *time) {
    fw_image *srcImage = getImage();
    fw_sprite_image *srcSprite = &getImage()->sprites[RES_SPRITE_INDEX_TITLE_LOGO];
    fw_sprite_image *gradientSprite = &getImage()->sprites[RES_SPRITE_INDEX_TITLE_GRADIENT];
    fw_image *dstImage = getRGBABuffer();
    fw_image_clear(dstImage);

    unsigned char *srcPixels = srcImage->pixels;
    unsigned char *dstPixels = dstImage->pixels;

    const float srcXStart = srcSprite->topLeft.x;
    const float srcYStart = srcSprite->topLeft.y;

    const float t = time->elapsed - _animSchedule.animations[ANIM_TITLE_LOGO].timeStartedAt;

    const float yAdd = _titleLogoAmplitude.y * sinf(t * 2.f);
    const float dstXStart = _titleLogoPos.x + _titleLogoAmplitude.x * sinf(t * 4.f);
    const float dstYStart = _titleLogoPos.y + yAdd;

    for (int y = 0; y < srcSprite->height; y++) {
        float srcY = y + srcYStart;
        float dstY = y + dstYStart;

        int gradStartX = gradientSprite->topLeft.x;
        int gradStartY = gradientSprite->topLeft.y;
        int gradIdx = srcImage->bytesPerPixel * (gradStartX + ( gradStartY + (int)(dstY + .5f)%gradientSprite->height ) * srcImage->width);

        for (int x = 0; x < srcSprite->width; x++) {
            float srcX = x + srcXStart;
            float dstX = x + dstXStart;

            srcX = (int)(srcX +.5f);
            srcY = (int)(srcY + .5f);
            dstX = (int)(dstX +.5f);
            dstY = (int)(dstY + .5f);

            if (dstX <= 0 || dstX >= dstImage->width || dstY <= 0 || dstY >= dstImage->height) {
                continue;
            }

            int srcIdx = srcImage->bytesPerPixel * (srcX + srcY * srcImage->width);
            int dstIdx = dstImage->bytesPerPixel * (dstX + dstY * dstImage->width);

            unsigned char srcR = srcPixels[srcIdx];
            unsigned char srcG = srcPixels[srcIdx+1];
            unsigned char srcB = srcPixels[srcIdx+2];
            unsigned char srcA = srcPixels[srcIdx+3];

            if (srcR == 255 && srcG == 255 && srcB == 255 && srcA == 255) {
                dstPixels[dstIdx] = srcPixels[gradIdx];
                dstPixels[dstIdx+1] = srcPixels[gradIdx+1];
                dstPixels[dstIdx+2] = srcPixels[gradIdx+2];
                dstPixels[dstIdx+3] = 255;

            } else {
                dstPixels[dstIdx] = srcPixels[srcIdx];
                dstPixels[dstIdx+1] = srcPixels[srcIdx+1];
                dstPixels[dstIdx+2] = srcPixels[srcIdx+2];
                dstPixels[dstIdx+3] = srcPixels[srcIdx+3];
            }
        }
    }


    fw_image *finalImage = dstImage;
    if (_titleLogoDistortMode.x > 0) {

        fw_image *dstImage2 = getRGBABuffer2();

        const int u = yAdd + _titleLogoDistort.x; // Upper
        const int b = yAdd + _titleLogoDistort.y; // Bottom
        const float h = b-u; // Höhe

        const float c = _titleLogoDistort.z; // Anzahl volle Sinus-Schwingungen. Erhöhen, wenn die Height `h` erhöht wird.
        const float n = c * M_PI / h;

        const float a = _titleLogoDistort.w; // Amplitude einer Sinus-Schwingung.
        float xoff = 0;

        for (int y=0; y<dstImage2->height; y++) {

            if (y >= u && y <= b) {
                xoff = a * fw_lut_sin((y-u) * n);
            }

            if (_titleLogoDistortMode.x == 2 && y%2) {
                xoff = -xoff;
            }

            for (int x=0; x<dstImage2->width; x++) {
                int dstIdx2 = dstImage2->bytesPerPixel * (x + y * dstImage2->width);

                const int dstX2 = (int)(x + xoff + .5f);
                if (dstX2 < 0 || dstX2 >= dstImage2->width || y < 0 || y >= dstImage2->height) {
                    dstImage2->pixels[dstIdx2] = 0 ;
                    dstImage2->pixels[dstIdx2+1] = 0 ;
                    dstImage2->pixels[dstIdx2+2] = 0 ;
                    dstImage2->pixels[dstIdx2+3] = 0 ;
                    continue;
                }

                int srcdx = dstImage->bytesPerPixel * (dstX2 + y * dstImage->width);
                dstImage2->pixels[dstIdx2] = dstPixels[srcdx];
                dstImage2->pixels[dstIdx2+1] = dstPixels[srcdx+1];
                dstImage2->pixels[dstIdx2+2] = dstPixels[srcdx+2];
                dstImage2->pixels[dstIdx2+3] = dstPixels[srcdx+3];
            }
        }
        finalImage = dstImage2;
    }

    fw_image_renderBegin(finalImage, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, finalImage->width, finalImage->height, GL_RGBA, GL_UNSIGNED_BYTE, finalImage->pixels);
    fw_image_putImage(finalImage);
    fw_image_renderEnd();
}

static void renderLogo() {
    fw_image_renderBegin(getImage(), 1);
    glTranslatef(285,150,0);
    fw_image_putSpriteImage(&getImage()->sprites[RES_SPRITE_INDEX_MINI_LOGO]);
    fw_image_renderEnd();
}

static void morph(fw_object_3d *tgtObj, fw_object_3d *srcObj1, fw_object_3d *srcObj2, float p, fw_image *image, fw_timer_data *time) {
    const float t = 2 + time->elapsed - _animSchedule.animations[ANIM_VECTOR_BALLS].timeStartedAt;
    fx_vector_balls_morph(tgtObj, srcObj1, srcObj2, p);

    fw_projection_perspective();

    glTranslatef(_vectorBallsPos.x + 2 * fw_lut_sin(-1.5 + t * .5f), _vectorBallsPos.y + 1 * fw_lut_sin(-1.5 + t * .5f), _vectorBallsPos.z - 10);
    glRotatef(t * 60.f, 1,.2,0);
    glTranslatef(1.f * fw_lut_sin(-1.5+2.f + t*.4), .5f *fw_lut_sin(-1.5+1.f + t * .2f),0);
    glRotatef(t * 80.f, 0,1,0.1);
    glTranslatef(1.5f * fw_lut_sin(-1.5 + t*.2), fw_lut_sin(-1.5 + t * .1f),0);

    fx_billboard_render(tgtObj, image);
}

static void billboardMorph(fw_timer_data *time) {
    int idx1 = _vectorBallsMorphObjIndexes.x;
    int idx2 = _vectorBallsMorphObjIndexes.y;
    morph(&_vectorBallsMorph, &_vectorBallSrcObjects[idx1], &_vectorBallSrcObjects[idx2], _vectorBallsMorphPercentage.x, getImage(), time);
}

static void renderHexagons(fw_timer_data *time) {

    const float t = time->elapsed - _animSchedule.animations[ANIM_HEXAGONS].timeStartedAt;

    glEnable(GL_DEPTH_TEST);

    float distance = _hexagonsDistance.x;

    glPushMatrix();
    {
        glTranslatef(_hexagonsPos.x, _hexagonsPos.y, _hexagonsPos.z);
        glRotatef(_hexagonsRotZ.w, 0,0,1);
        glRotatef(_hexagonsRotSinX.w * fw_lut_sin(t * 2), 1,0,0);
        glRotatef(_hexagonsRotY.w, 0,1,0);
        glRotatef(_hexagonsRotSinXYZ.w, _hexagonsRotSinXYZ.x,_hexagonsRotSinXYZ.y,_hexagonsRotSinXYZ.z * fw_lut_sin(t));

        glTranslatef(_hexagonsPos2.x,_hexagonsPos2.y,_hexagonsPos2.z);
        glScalef(3,3,3);

        glPushMatrix();
        {
            glTranslatef(0,-distance,0);
            glRotatef(90, 1,0,0);
            fw_object_3d_putFaces(&getHexagons()[0]);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(0,distance,0);
            glRotatef(270, 1,0,0);
            glRotatef(60, 0,0,1);
            fw_object_3d_putFaces(&getHexagons()[0]);
        }
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, getHexagonImage2()->textureID);
        fx_texgen_eye_linear_begin(.5f);

        float scaleHack = .9995;

        glPushMatrix();
        {
            glTranslatef(0,-distance,0);
            glRotatef(90, 1,0,0);
            glScalef(scaleHack,scaleHack,scaleHack);
            fw_object_3d_putFaces(&getHexagons()[1]);
        }
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, getHexagonImage1()->textureID);
        fx_texgen_eye_linear_changeScale(.5f);

        glPushMatrix();
        {
            glTranslatef(0,distance,0);
            glRotatef(270, 1,0,0);
            glRotatef(60, 0,0,1);
            glScalef(scaleHack,scaleHack,scaleHack);
            fw_object_3d_putFaces(&getHexagons()[1]);
        }
        glPopMatrix();

        fx_texgen_eye_linear_end();

    }
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
}

void renderCreditsLine(float x, float y, unsigned char *str, int str2len, fx_gradient *gradient, fw_timer_data *time) {
    fw_image *image = getRGBABuffer();
    fw_font_face *face = getDefaultFontFace();

    fx_gradient_text_properties props = {
        .target = image,
        .text = str,
        .textLength = str2len,
        .fontFace = face,
        .gradient = gradient,
        .gradientOffset = (int)(time->elapsed * 24.f + .5f),
    };

    props.xStart = x;
    props.yStart = y;

    fx_gradient_text_writeBegin(image);
    fx_gradient_text_write(&props, time->elapsed);
    fx_gradient_text_writeEnd(image);
}

void renderCredits(fw_timer_data *time) {
    int p = _creditsPageNr.x;
    fx_gradient *gradientLine3;

    if (p == 0) {
        gradientLine3 = &_gradientCredits;
    } else {
        gradientLine3 = &_gradientRed;
    }

    renderCreditsLine(_creditsPosLine1.x, _creditsPosLine1.y, _creditsTexts[p][0], strlen((char*)_creditsTexts[p][0]), &_gradientCredits, time);
    renderCreditsLine(_creditsPosLine2.x, _creditsPosLine2.y, _creditsTexts[p][1], strlen((char*)_creditsTexts[p][1]), &_gradientCredits, time);
    renderCreditsLine(_creditsPosLine3.x, _creditsPosLine3.y, _creditsTexts[p][2], strlen((char*)_creditsTexts[p][2]), gradientLine3, time);
    renderCreditsLine(_creditsPosLine4.x, _creditsPosLine4.y, _creditsTexts[p][3], strlen((char*)_creditsTexts[p][3]), &_gradientCredits, time);
    renderCreditsLine(_creditsPosLine5.x, _creditsPosLine5.y, _creditsTexts[p][4], strlen((char*)_creditsTexts[p][4]), &_gradientCredits, time);
    renderCreditsLine(_creditsPosLine6.x, _creditsPosLine6.y, _creditsTexts[p][5], strlen((char*)_creditsTexts[p][5]), &_gradientCredits, time);
}

///////////////////////////////////////////////////////////////////////////////
// EXECUTE
///////////////////////////////////////////////////////////////////////////////

static void renderDemo(fw_timer_data *time) {
    fw_render_viewportLowRes(0,12);

    // 40: lower border = yStart at bottom, -15-40: -top border - lower border = height
    glScissor(0,40,FW_RENDER_WIDTH, FW_RENDER_HEIGHT-15-40);

    glEnable(GL_SCISSOR_TEST);

    fx_starfield_2d_render(time);

    fw_projection_perspective();

    glEnable(GL_DEPTH_TEST);

    if (_isCubeAnimEnabled) {
        renderGlenzCube(time);
        renderPlane(time);
    }

    if (_isColorPlanesAnimEnabled) {
        renderColorPlanes(time);
    }

    if (_isVectorLogoAnimEnabled) {
        renderSpectrox(time);
    }


    if (_isVectorBallsAnimEnabled) {
        billboardMorph(time);
    }

    if (_isTitleLogoAnimEnabled) {
        renderTitle(time);
    }

    if (_isGreetingsAnimEnabled) {
        renderGreetingsText(time);
    }

    if (_isHexagonsAnimEnabled) {
        renderHexagons(time);
        renderCredits(time);
    }

    glDisable(GL_SCISSOR_TEST);

    fw_render_viewportLowRes(0,0);
    renderLogo();
    fx_gradient_border_render(time, 15, -40);
    renderScrollText(time);
}

static void renderGuru(fw_timer_data *time) {
    const float t = time->elapsed - _animSchedule.animations[ANIM_GURU].timeStartedAt;

    glColor4f(1,1,1,1);
    fw_projection_perspective();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, getImage()->textureID);


    fw_sprite_image *sprite;
    if (fw_lut_sin(t * 4) > 0) {
        sprite = &getImage()->sprites[RES_SPRITE_INDEX_GURU_BORDER];
    } else {
        sprite = &getImage()->sprites[RES_SPRITE_INDEX_GURU_PLAIN];
    }

    const float w = sprite->width/4.f;
    const float h = sprite->height/4.f;

    glPushMatrix();

    glTranslatef(_guruPos.x, _guruPos.y, _guruPos.z);
    glRotatef(_guruRot.w, _guruRot.x,_guruRot.y,_guruRot.z);
    glScalef(_guruScale.x, _guruScale.y, _guruScale.z);

    glBegin(GL_QUADS);

    glTexCoord2f(sprite->texTopLeft.x, sprite->texTopLeft.y);
    glVertex3f(-w/2.f, h/2.f, 0);

    glTexCoord2f(sprite->texTopLeft.x, sprite->texBottomRight.y);
    glVertex3f(-w/2.f, -h/2.f, 0);

    glTexCoord2f(sprite->texBottomRight.x, sprite->texBottomRight.y);
    glVertex3f(w/2.f, -h/2.f, 0);

    glTexCoord2f(sprite->texBottomRight.x, sprite->texTopLeft.y);
    glVertex3f(w/2.f, h/2.f, 0);
    glEnd();


    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void scene_0_execute(fw_timer_data *time) {
    fw_animation_processAnimationSchedule(&_animSchedule, time);

    fw_render_bindTextureBufferBegin();

    if (_isGuruAnimEnabled) {
        renderGuru(time);
    } else {
        renderDemo(time);
    }

    fw_render_bindTextureBufferEnd();
    fw_render_renderTextureBuffer();
}

