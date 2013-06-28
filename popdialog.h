#ifndef POPDIALOG_H
#define POPDIALOG_H

#include <QDialog>
#include "layouttree.h"


typedef void* (*aFunc)(void*,char*,int);


class PopDialog : public QDialog, public LayoutTree
{
    Q_OBJECT
public:
    struct funSt{
        void (* fun)(void*, char*,int);
       void * uno;
       char * dos;
       int * tres;
    };
    explicit PopDialog(QWidget *parent = 0);

    void SetWindowTitle(const char * text);

    void AddTabCtrl(const char * tabControl, int length=0, int height=0);

    void SetCurrentTab(const char * tabControl, unsigned int tab=0);

    void AddTab(const char * tabControl, char * name);

    void display();

    void ActionTile(const char * handle);

    void AddEdit(const char * tile, int length, const char * value = (const char * )0 );

    void AddText(const char * handle,
                 const char * text);

    void AddButton(const char * handle,
                   const char * caption="",
                   int length=150);

    //void AddRadioGroup(const char * handle);

    void AddToggle(const char * handle,
                   int length=0,
                   const char * caption=NULL);

    void AddSlider(const char * handle,
                   int length=100,
                   int height=30,
                   int style=0,
                   int min=-180,
                   int max=180);

    void AddGLView(const char * handle, int width=200, int height=500);

    void AddPopList(const char * handle,
                    int len=8);


    void ListAdd(const char * handle,
                 const char * line);

    void SetTileTip(const char * handle,
                    const char * value);

    void SetBitmap(const char * handle,
                   int id=0,
                   const char * fil="C:\\Sandbox\\bitm.bmp");

   // void ActionTile(const char * handle/*actionfunction*/);
signals:
   // void (*ActionFunct)(void *, char*, int);
private slots:
   // void mousePressEvent(QMouseEvent *e){
 //       if(e->button()==Qt::LeftButton)
 //               emit ActionFunct();
 //   }

public slots:
    
};

#endif // POPDIALOG_H
