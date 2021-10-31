
#include "TXLib.h"

class AbstractWindow
    {
    public:

    int id;
    int x_, y_;
    int width_, height_;
    HDC Image_;
    const char* text_;

    int Activity_;

    COLORREF Color_;

    void Load ();
    virtual void Draw ();

    int Manager ();
    int Checker (int MouseX, int MouseY);

    virtual int Action (int MouseX, int MouseY) = 0;

    AbstractWindow (int id, int x, int y, int width, int heigh, const char* text = NULL, COLORREF Color = NULL, int Activity = 1);
    };

class CanvasWindow : public AbstractWindow
    {
    public:
    using AbstractWindow::AbstractWindow;

    virtual void Draw ();

    virtual int Action (int MouseX, int MouseY);

    HDC Invis_;
    };

class ClearButWindow : public AbstractWindow
    {
    public:
    using AbstractWindow::AbstractWindow;

    virtual int Action (int MouseX, int MouseY);
    };

class ColorButWindow;

class PolitraWindow : public AbstractWindow
    {
    public:

    PolitraWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* ColorBut, ColorButWindow* Pivo);

    virtual int Action (int MouseX, int MouseY);

    void SetActivity (int Action);

    CanvasWindow* ColorBut = NULL;

    ColorButWindow* Pivo = NULL;
    };

class ColorButWindow : public AbstractWindow
    {
    public:

    using AbstractWindow::AbstractWindow;

    ColorButWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* Coloring, PolitraWindow* ColorSwitch);

    virtual int Action (int MouseX, int MouseY);

    CanvasWindow* Coloring_ = NULL;

    PolitraWindow* ColorSwitch_ = NULL;
    };

class CloseButWindow : public AbstractWindow
    {
    public:

    using AbstractWindow::AbstractWindow;

    virtual int Action (int MouseX, int MouseY);
    };

class FileOpenWindow : public AbstractWindow
    {
    public:

    using AbstractWindow::AbstractWindow;

    FileOpenWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* LDImage);

    char fileName_ [MAX_PATH] = {};

    virtual int Action (int MouseX, int MouseY);

    CanvasWindow* LDImage = NULL;
    };

class AbstractManager : public AbstractWindow
    {
    public:

    using AbstractWindow::AbstractWindow;

    virtual int Action (int MouseX, int MouseY);

    int Count = 0;

    void AddWin (AbstractWindow* Address);

    AbstractWindow* MangMass [10] = {};

    void LoadAll ();
    void DrawAll ();

    void ActionMang ();
    };

AbstractManager* MainManager = NULL;

HDC BrightnessChanger (HDC OpPhoto, int RCh, int GCh, int BCh);

int Prenimalka (int X, int Xmin, int Xmax);

int main()
    {
    txCreateWindow (800, 600);

    //AbstractWindow Test (10, 10, 100, 100, "Ya.bmp");
    CanvasWindow Canvas  (1, 110,   0, 690, 690, NULL);
    ClearButWindow Clear (2,   0,   0, 100, 100, "ClearNew.bmp");
    PolitraWindow Color (7,   0, 110, 100, 100, "ColorNew.bmp", NULL, &Canvas, NULL);

    ColorButWindow Pivo [4] {{3,  0, 220,  30,  30, NULL,    TX_RED, &Canvas, &Color},
                             {4,  0, 260,  30,  30, NULL,  TX_GREEN, &Canvas, &Color},
                             {5, 40, 220,  30,  30, NULL, TX_YELLOW, &Canvas, &Color},
                             {6, 40, 260,  30,  30, NULL,  TX_WHITE, &Canvas, &Color}};

    CloseButWindow Close   (8, 762, 562, 38, 38, "КРЕСТВЕЗДЕСУЩИЙ.bmp");

    FileOpenWindow FileOP (9, 0, 300, 100, 100, "OpenFileNew.bmp", TX_NULL, &Canvas);

    AbstractManager DrawMang (0,  0,   0, 800, 600, NULL, NULL);

    MainManager = &DrawMang;

    Color.Pivo = Pivo;

    DrawMang.AddWin (&Clear);
    DrawMang.AddWin (&Color);
    DrawMang.AddWin (&Pivo[0]);
    DrawMang.AddWin (&Pivo[1]);
    DrawMang.AddWin (&Pivo[2]);
    DrawMang.AddWin (&Pivo[3]);
    DrawMang.AddWin (&Close);
    DrawMang.AddWin (&FileOP);
    DrawMang.AddWin (&Canvas);

    for (int i = 0; i < DrawMang.Count; i++)
        {
        //printf ("id = %d\n", DrawMang.MangMass [i]->id);
        }

    DrawMang.LoadAll ();
    DrawMang.DrawAll ();

    printf ("file%s\n", FileOP.fileName_);

    DrawMang.ActionMang ();

    txSaveImage ("JopaSruchkoy.bmp");

    return 0;
    }

void AbstractWindow::Load ()
    {
    if (text_ != NULL)
        {
        Image_ = txLoadImage (text_);
        }
    }

void AbstractWindow::Draw ()
    {
    //COLORREF ColorHSL = txRGB2HSL (Color_);

    txSetFillColor (Color_);
    //txSetColor (Color_, 5);

    if (Image_ != NULL)
        {
        //printf ("фиолетовая индоутка %d\n", id);

        txBitBlt (txDC(), x_, y_, width_, height_, Image_);
        }
    else txRectangle (x_, y_, x_ + width_, y_ + height_);

    char Actv [3] = "";

    sprintf (Actv, "%d\n", Activity_);

    txTextOut (x_, y_, Actv);
    }

void CanvasWindow::Draw ()
    {
    }

int AbstractWindow::Manager ()
    {
    if (txMouseButtons () == 1)
        {
        return 1;
        }

    else return 0;
    }

int CanvasWindow::Action (int MouseX, int MouseY)
    {
    COLORREF ColorHSL = txRGB2HSL (Color_);

    int H = txExtractColor (ColorHSL, TX_HUE);
    int S = txExtractColor (ColorHSL, TX_SATURATION);
    int L = txExtractColor (ColorHSL, TX_LIGHTNESS);

    S = S - 30;

    L = L + 30;

    if (L > 255)
        {
        L = 255;
        }

    if (S < 0)
        {
        S = 0;
        }

    COLORREF ColorNew = txHSL2RGB (RGB (H, S, L));

    txSetFillColor (Color_);
    txSetColor (ColorNew);

    txEllipse (MouseX-5, MouseY-5, MouseX+5, MouseY+5, Invis_);

    return 0;
    }

int ClearButWindow::Action (int MouseX, int MouseY)
    {
    txSetFillColor (TX_BLACK);
    txRectangle (110, 0, 800, 600);

    MainManager->DrawAll ();

    return 0;
    }

int PolitraWindow::Action (int MouseX, int MouseY)
    {
    SetActivity (1);

    return 0;
    }

int ColorButWindow::Action (int MouseX, int MouseY)
    {
    Coloring_->Color_ = Color_;

    ColorSwitch_->SetActivity (0);

    MainManager->DrawAll ();

    return 0;
    }

int AbstractManager::Action (int MouseX, int MouseY)
    {
    printf ("меня вызвали в мусарню");

    return 0;
    }

int CloseButWindow::Action (int MouseX, int MouseY)
    {
    return 1;
    }

namespace ComDlg32
    {
    TX_DLLIMPORT (false, "ComDlg32.dll", bool, GetOpenFileNameA, (OPENFILENAMEA* ofnStruct), WINAPI);
    TX_DLLIMPORT (false, "ComDlg32.dll", bool, GetSaveFileNameA, (OPENFILENAMEA* ofnStruct), WINAPI);
    }

int FileOpenWindow::Action (int MouseX, int MouseY)
    {
    OPENFILENAME ofn    = { sizeof (ofn), txWindow() };

    ofn.lpstrTitle      = "\xcd\xe5\x20\xea\xee\xef\xe8\xef\xe0\xf1\xf2\xfc\x2c\x20\xe0\x20\xf0\xe0\xe7\xe1\xe5\xf0\xe8"
                                 "\xf1\xfc\x20\xe8\x20\xf1\xe4\xe5\xeb\xe0\xe9\x20\xf4\xf3\xed\xea\xf6\xe8\xfe\x21\x21\x21";
    ofn.lpstrFile       = fileName_;

    ofn.nMaxFile        = sizeof (fileName_);

    ofn.lpstrFilter     = "Bmp Files\0" "*.bmp\0"  // ofn.nFilterIndex = 1
                          "All Files\0" "*.*\0";   // ofn.nFilterIndex = 2

    ofn.nFilterIndex    = 1;

    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    bool oldPSW = _txProcessSystemWarnings;
    _txProcessSystemWarnings = false;            // Just do it. (C) NAN - Not a Nike :)

    if ($( ComDlg32::GetOpenFileNameA ))
        $( ComDlg32::GetOpenFileNameA (&ofn) );  // Весьма полезная функция, отображает диалог выбора файла.

    _txProcessSystemWarnings = oldPSW;           // Just do it too.

    //printf ("\n" "GetOpenFileName() returned: fileName = \"%s\"\n", fileName);

    HDC OpPhoto = txLoadImage (fileName_);
    if (OpPhoto == NULL) txMessageBox ("Долбит атмос");

    HDC NewPH = BrightnessChanger (OpPhoto, -10, -20, -30);

    txBitBlt (txDC(), 110, 0, 0, 0, NewPH);

    return 0;
    }

HDC BrightnessChanger (HDC OpPhoto, int RCh, int  GCh, int BCh)
    {
    int Width = txGetExtentX (OpPhoto);
    int Height = txGetExtentY (OpPhoto);

    COLORREF Color = 0;

    for (int x = 0; x < Width; x ++)
        {
        for (int y = 0; y < Height; y ++)
            {
            Color = txGetPixel (x, y, OpPhoto);
            int R = txExtractColor (Color, TX_RED);
            int G = txExtractColor (Color, TX_GREEN);
            int B = txExtractColor (Color, TX_BLUE);

            R = R + RCh;
            G = G + GCh;
            B = B + BCh;

            R = Prenimalka (R, 0, 255);
            G = Prenimalka (G, 0, 255);
            B = Prenimalka (B, 0, 255);

            COLORREF ColorNew (RGB (R, G, B));

            txSetPixel (x, y, ColorNew, OpPhoto);
            }
        }
    return OpPhoto;
    }

int Prenimalka (int X, int Xmin, int Xmax)
    {
    if (X > Xmax)
        {
        return Xmax;
        }

    if (X < Xmin)
        {
        return Xmin;
        }

    return X;
    }

int AbstractWindow::Checker (int MouseX, int MouseY)
    {
    if (x_ < MouseX && MouseX < x_ + width_ && y_ < MouseY && MouseY < y_ + height_ && Manager () == 1)
        {
        return 1;
        }
    else return 0;
    }

void PolitraWindow::SetActivity (int Activity)
    {
    for (int Count = 0; Count < 4; Count ++)
        {
        Pivo[Count].Activity_ = Activity;

        Pivo[Count].Draw ();
        }
    }

void AbstractManager::AddWin (AbstractWindow* Address)
    {
    MangMass [Count] = Address;

    Count = Count + 1;
    }

void AbstractManager::LoadAll ()
    {
    for (int i = 0; i < Count; i++)
        {
        MangMass [i] -> Load ();
        }
    }

void AbstractManager::DrawAll ()
    {
    Draw ();

    for (int i = 0; i < Count; i ++)
        {
        if (MangMass [i] -> Activity_ == 1)
            {
            MangMass [i] -> Draw ();
            }
        }
    }

void AbstractManager::ActionMang ()
    {
    while (1<2)
        {
        int MouseX = txMouseX ();
        int MouseY = txMouseY ();

        for (int i = 0; i < Count; i++)
            {
            if (MangMass [i]->Checker (MouseX, MouseY) == 1)
                {
                if (MangMass [i] -> Activity_ == 1)
                    {
                    if (MangMass [i]->Action (MouseX, MouseY) == 1)
                        {
                        txDisableAutoPause ();
                        return;
                        }
                    }
                }
            }
        txSleep ();
        }
    }

AbstractWindow::AbstractWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, int Activity) :
    id (id),
    x_ (x),
    y_ (y),
    width_ (width),
    height_ (heigh),
    Image_ (NULL),
    text_ (imgTxt),
    Color_ (Color),
    Activity_ (Activity)
    {}

CanvasWindow::CanvasWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, txCreateDIBSection) :
    AbstractWindow (id, x, y, width, heigh, imgTxt, Color, 1),
    Invis_ (txCreateDIBSection (690, 690))
    {}

PolitraWindow::PolitraWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* ColorBut, ColorButWindow* Pivo) :
    AbstractWindow (id, x, y, width, heigh, imgTxt, Color, 1),
    ColorBut (ColorBut),
    Pivo (Pivo)
    {}

ColorButWindow::ColorButWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* Coloring, PolitraWindow* ColorSwitch) :
    AbstractWindow (id, x, y, width, heigh, imgTxt, Color, 0),
    Coloring_ (Coloring),
    ColorSwitch_ (ColorSwitch)
    {}

FileOpenWindow::FileOpenWindow (int id, int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color, CanvasWindow* LDImage) :
    AbstractWindow (id, x, y, width, heigh, imgTxt, Color, 1)
    {}
