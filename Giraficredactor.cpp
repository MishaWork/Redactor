
#include "TXLib.h"

class AbstractWindow
    {
    public:
    int x_, y_;
    int width_, height_;
    HDC Image_;
    const char* text_;

    COLORREF Color_;

    void Load ();
    void Draw ();

    int Manager ();
    int Checker (int MouseX, int MouseY);

    virtual void Action (int MouseX, int MouseY) = 0;

    AbstractWindow (int x, int y, int width, int heigh, const char* text = NULL, COLORREF Color = NULL);
    };

class CanvasWindow : public AbstractWindow
    {
    public:
    using AbstractWindow::AbstractWindow;

    virtual void Action (int MouseX, int MouseY);
    };

class ClearButWindow : public AbstractWindow
    {
    public:
    using AbstractWindow::AbstractWindow;

    virtual void Action (int MouseX, int MouseY);
    };

class ColorButWindow : public AbstractWindow
    {
    public:
    using AbstractWindow::AbstractWindow;

    virtual void Action (int MouseX, int MouseY);
    };

void Drawer ();

int main()
    {
    txCreateWindow (800, 600);

    //AbstractWindow Test (10, 10, 100, 100, "Ya.bmp");
    CanvasWindow Canvas  (110,   0, 690, 690, NULL);
    ClearButWindow Clear (  0,   0, 100, 100, "Clear.bmp");
    ColorButWindow Color (  0, 110, 100, 100, "Color.bmp");

    AbstractWindow* Mass [] = {&Canvas, &Clear, &Color, NULL};

    for (int Count = 0; Mass [Count] != NULL; Count++)
        {
        Mass [Count] -> Load ();
        Mass [Count] -> Draw ();
        }

    while (1<2)
        {
        int MouseX = txMouseX ();
        int MouseY = txMouseY ();

        for (int Count = 0; Mass [Count] != NULL; Count++)
            {
            if (Mass [Count]->Checker (MouseX, MouseY) == 1)
                {
                Mass [Count]->Action (MouseX, MouseY);
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
    txSetFillColor (TX_TRANSPARENT);
    txSetColor (TX_WHITE);

    if (Image_ != NULL)
        {
        txBitBlt (txDC(), x_, y_, width_, height_, Image_);
        }
    else txRectangle (x_, y_, x_ + width_, y_ + height_);
    }

int AbstractWindow::Manager ()
    {
    if (txMouseButtons () == 1)
        {
        return 1;
        }

    else return 0;
    }

void CanvasWindow::Action (int MouseX, int MouseY)
    {
    txSetFillColor (Color_);

    //txRectangle (100, 100, 200, 200);

    txCircle (MouseX, MouseY, 10);

    //printf ("я каякер ем говно\n");
    }

void ClearButWindow::Action (int MouseX, int MouseY)
    {
    txSetFillColor (TX_BLACK);
    txRectangle (110, 0, 800, 600);
    }

void ColorButWindow::Action (int MouseX, int MouseY)
    {

    }

int AbstractWindow::Checker (int MouseX, int MouseY)
    {
    if (x_ < MouseX && MouseX < x_ + width_ && y_ < MouseY && MouseY < y_ + height_ && Manager () == 1)
        {
        return 1;
        }
    else return 0;
    }

AbstractWindow::AbstractWindow (int x, int y, int width, int heigh, const char* imgTxt, COLORREF Color) :
    x_ (x),
    y_ (y),
    width_ (width),
    height_ (heigh),
    Image_ (NULL),
    text_ (imgTxt),
    Color_ (Color)
    {}
