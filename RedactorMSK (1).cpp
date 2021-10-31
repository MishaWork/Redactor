
#include "TXLib.h"

class AbstractWindow
    {
    public:
    int x_, y_;
    int width_, height_;
    HDC Image_;
    const char* text_;
    COLORREF Color;

    int Activity;

    void Load ();
    void Draw ();

    int Manager ();
    int Checker (int MouseX, int MouseY);

    virtual int Action (int MouseX, int MouseY) = 0;

    AbstractWindow (int x, int y, int width, int height, const char* text = NULL, COLORREF Color = NULL, int Activity = 1);
    };

class CanvasWindow : public AbstractWindow
    {
    public:

    using AbstractWindow::AbstractWindow;

    virtual int Action (int MouseX, int MouseY);
    };

class ColorChangerWindow : public AbstractWindow
    {
    public:

    ColorChangerWindow (int x, int y, int width, int height, const char* text, COLORREF Color, CanvasWindow* ColorWin);

    virtual int Action (int MouseX, int MouseY);

    CanvasWindow* ControlledCanvas_ = NULL;
    };

class ClearButWindow : public AbstractWindow
    {
    public:

    ClearButWindow (int x, int y, int width, int height, const char* text, COLORREF Color, CanvasWindow* Clear);

    virtual int Action (int MouseX, int MouseY);

    CanvasWindow* Clear_ = NULL;
    };

class ColorButWindow : public AbstractWindow
    {
    public:

    ColorButWindow (int x, int y, int width, int height, const char* text, COLORREF Color, ColorChangerWindow* ColorMass);

    virtual int Action (int MouseX, int MouseY);

    ColorChangerWindow* ColorMass_ = NULL;
    };

void Drawer ();

int main()
    {
    txCreateWindow (800, 600);

    //AbstractWindow     Test     ( 10,  10, 100, 100, "Ya.bmp");
    CanvasWindow       Canvas        (110,   0, 690, 690,   NULL);

    ColorChangerWindow ColorBut [5] {{110,  10,  20,  20,   NULL,  TX_WHITE, &Canvas},
                                     {110,  35,  20,  20,   NULL,    TX_RED, &Canvas},
                                     {110,  60,  20,  20,   NULL,  TX_GREEN, &Canvas},
                                     {135,  10,  20,  20,   NULL,   TX_BLUE, &Canvas},
                                     {135,  35,  20,  20,   NULL, TX_YELLOW, &Canvas} };

    ClearButWindow     Clearing      ( 10, 230, 100, 100, "Clear.bmp", NULL, &Canvas);
    ColorButWindow     ShowColorButs ( 10, 110, 100, 100, "Color.bmp", NULL, ColorBut);

    AbstractWindow* Mass [] = {&Canvas, &ColorBut [0], &ColorBut [1], &ColorBut [2], &ColorBut [3], &ColorBut [4], &Clearing, &ShowColorButs, NULL};

    for (int Count = 0; Mass [Count] != NULL; Count++)
        {
        Mass [Count] -> Load ();

        if (Mass [Count] -> Activity == 1)
            {
            Mass [Count] -> Draw ();
            }
        }

    while (1<2)
        {
        int MouseX = txMouseX ();
        int MouseY = txMouseY ();

        for (int Count = 7; Count >= 0; Count--)
            {
            if (Mass [Count]->Checker (MouseX, MouseY) == 1)
                {
                if (Mass [Count]->Activity == 1)
                    {
                    if (Mass [Count]->Action (MouseX, MouseY) == 1)
                        {
                        break;
                        }
                    }
                }
            //printf ("count = %d\n", Count);
            }

        txSleep ();
        }

    return 0;
    }

void Drawer ()
    {
    while (1 < 2)
        {
        int MouseX = txMouseX ();
        int MouseY = txMouseY ();

        if (txMouseButtons () == 1)
            {
            txCircle (MouseX, MouseY, 2);
            }
        txSleep ();
        //printf ("x = %d, y = %d\n", MouseX, MouseY);
        }
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
    if (Image_ != NULL)
        {
        txBitBlt (txDC(), x_, y_, width_, height_, Image_);
        }
    else
        {
        txSetFillColor (Color);
        txRectangle (x_, y_, x_ + width_, y_ + height_);
        }
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
    //txRectangle (100, 100, 200, 200);

    txSetFillColor (Color);

    txSetColor (Color);

    txCircle (MouseX, MouseY, 10);

    printf ("я каякер ем говно\n");

    return 1;
    }

int ColorChangerWindow::Action (int MouseX, int MouseY)
    {
    ControlledCanvas_->Color = Color;



    return 1;
    }

int ClearButWindow::Action (int MouseX, int MouseY)
    {
    txSetFillColor (TX_BLACK);
    txSetColor (TX_WHITE);

    txRectangle (Clear_->x_, Clear_->y_, Clear_->x_ + Clear_->height_, Clear_->y_ + Clear_->width_);

    return 1;
    }

int ColorButWindow::Action (int MouseX, int MouseY)
    {

    for (int Count = 0; Count < 5; Count++)
        {
        ColorMass_ [Count].Activity = 1;
        ColorMass_ [Count].Draw ();
        }
    }

int AbstractWindow::Checker (int MouseX, int MouseY)
    {
    if (x_ < MouseX && MouseX < x_ + width_ && y_ < MouseY && MouseY < y_ + height_ && Manager () == 1)
        {
        return 1;
        }
    else return 0;
    }

AbstractWindow::AbstractWindow (int x, int y, int width, int height, const char* imgTxt, COLORREF Color, int Activity) :
    x_ (x),
    y_ (y),
    width_ (width),
    height_ (height),
    Image_ (NULL),
    text_ (imgTxt),
    Color (Color),
    Activity (Activity)
    {}

ColorChangerWindow::ColorChangerWindow (int x, int y, int width, int height, const char* imgTxt, COLORREF Color, CanvasWindow* ColorWin) :
    AbstractWindow (x, y, width, height, imgTxt, Color, 0),
    ControlledCanvas_ (ColorWin)
    {}

ClearButWindow::ClearButWindow (int x, int y, int width, int height, const char* imgTxt, COLORREF Color, CanvasWindow* Clear) :
    AbstractWindow (x, y, width, height, imgTxt, Color, 1),
    Clear_ (Clear)
    {}

ColorButWindow::ColorButWindow (int x, int y, int width, int height, const char* imgTxt, COLORREF Color, ColorChangerWindow* ColorMass) :
    AbstractWindow (x, y, width, height, imgTxt, Color, 1),
    ColorMass_ (ColorMass)
    {}


/*ColorChanger::ColorChanger (int x, int y, int width, int height, const char* imgTxt) :
    AbstractWindow (x, y, width, heigh, imgTxt)
    {}*/
