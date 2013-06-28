#include "popdialog.h"
#include <QApplication>

#include "tree.h"
#include "layouttree.h"
#include <iostream>
#include <QDebug>
#include <QTabWidget>
#include <QPushButton>

//#include "ModelLoader.h"

#include "tinyxml/tinyxml2.h"

using namespace std;



static void insert(void *, char *, int)
{
qDebug() << "pressed";
}

int main(int argc, char *argv[])
{

    tinyxml2::XMLDocument doc;
    cout<<doc.LoadFile("test.xml");

   // const char * title = doc.FirstChildElement()->GetText();

    QApplication a(argc, argv);

   tinyxml2::XMLText* textNode = doc.FirstChildElement()->ToText();
    //const char * title = textNode->Value();
//cout<<title<<endl;
   PopDialog w;
   w.SetWindowTitle("MY POP DIALOG");

   w.StartColumn();
      w.AddGLView("Glview");
   w.EndColumn();

   w.StartColumn();
   w.StartRow();
          /*w.StartColumn();
              for (int i = 0; i<8; i++){
                  w.StartRow();
                      w.AddButton("b1","Texty!");
                  w.EndRow();
              }
          w.EndColumn();
          w.StartColumn();
              w.AddButton("basda","New button");
          w.EndColumn();*/
          w.StartColumn();
              w.AddTabCtrl("Tabctrl");
              w.AddTab("Tabctrl","Tab1");
              w.AddTab("Tabctrl","Tab2");
              w.SetCurrentTab("Tabctrl",1);

              //w.AddButton("b2", "right side");
              w.StartColumn();
              for (int i = 0; i<8; i++){
                  w.StartRow();
                      w.AddButton("b1","Texty!");
                  w.EndRow();
              }
              w.EndColumn();

              w.SetCurrentTab("Tabctrl",0);

              w.StartRow();
                  w.AddPopList("mreh");
                  w.ListAdd("mreh","dadasd");
                  w.ListAdd("mreh","dadasd");
                  w.ListAdd("mreh","dadasd");
                  w.ListAdd("mreh","dadasd");
              w.EndRow();
              w.StartRow();
                  w.AddButton("newb","newbutton");
              w.EndRow();
          w.EndColumn();
   w.EndColumn();


   w.StartRow();/*
       w.StartColumn();
           w.AddSlider("new slider",20,20,1);
       w.EndColumn();
       w.StartColumn();
           w.AddEdit("editbox1",50,"Hello");
       w.EndColumn();*/

   w.EndRow();
   /*w.StartRow();
       w.StartColumn();
           for (int i = 0; i<8; i++){
               w.StartRow();
                   w.AddButton("b1","Texty!");
               w.EndRow();
           }
       w.EndColumn();
       w.StartColumn();
           w.AddButton("basda","New button");
       w.EndColumn();
       w.StartColumn();
           w.AddTabCtrl("Tabctrl");
           w.AddTab("Tabctrl","Tab1");
           w.AddTab("Tabctrl","Tab2");
           w.SetCurrentTab("Tabctrl",0);
           w.AddButton("b2", "right side");
           w.StartColumn();
               w.StartRow();
                   w.AddButton("b2", "right side");
               w.EndRow();
               w.StartRow();
                   w.AddButton("b2", "right side");
               w.EndRow();
               w.StartRow();
                   w.AddButton("b2", "right side");
               w.EndRow();
           w.EndColumn();
           w.SetCurrentTab("Tabctrl",1);
           w.StartRow();
               w.AddButton("newb","newbutton");
           w.EndRow();
       w.EndColumn();
   w.EndRow();
*/

   // w.printTree();
    //w.display();
    //w.show();

    return 0;//a.exec();
 /*
    w.StartColumn();
  // for (int i = 0; i<8; i++){
        w.StartRow();
           w.AddButton("b1","Texty!");
        w.EndRow();
 //   }
    w.EndColumn();



     */



  //
}




/*w.StartColumn();
   w.AddTabCtrl("Tabctrl");
  w.AddTab("Tabctrl","Tab1");
   w.AddTab("Tabctrl","Tab2");
   w.SetCurrentTab("Tabctrl",0);

      w.AddButton("b2", "right side");
       w.StartColumn();
           w.StartRow();
               w.AddButton("b2", "right side");
            w.EndRow();
           w.StartRow();
               w.AddButton("b2", "right side");
           w.EndRow();
       w.EndColumn();
  w.SetCurrentTab("Tabctrl",1);

  w.StartRow();
  w.AddButton("newb","newbutton");
   w.EndRow();

w.EndColumn();*/

//  w.display();
//w.show();

//    w.printTree();
/*
QDialog * qd = new QDialog;



QBoxLayout * qbl = new QHBoxLayout;
QPushButton * qb = new QPushButton("sdad");
qbl->addWidget(qb);
QWidget * qw = new QWidget();
qw->setLayout(qbl);

QTabWidget * qtb = new QTabWidget();




qtb->addTab(qw,"tab1");


QVBoxLayout *qvb = new QVBoxLayout;

qvb->addWidget(qtb);

qd->setLayout(qvb);

qd->show();
MyDialog w;
char * buf=new char;
w.StartRow();
   w.StartColumn();
       w.StartRow();
       for (int i =0;i<6;i++){
           sprintf(buf,"bg%d",i);
           w.AddButton(buf,buf);
       }
       w.EndRow();
       w.StartRow();
           w.StartColumn();
               w.AddPopList("plis1");
               for (int i = 0; i<5; i++){
                   sprintf(buf,"%d",i);
                   //cout<<buf<<endl;
                   w.ListAdd("plis1",buf);
               }
           w.EndColumn();
           w.StartColumn();
               w.AddButton("b3");
           w.EndColumn();
           w.StartColumn();
               w.StartRow();
                   w.AddButton("b4");
               w.EndRow();
               w.StartRow();
                   w.AddButton("b5");
               w.EndRow();
           w.EndColumn();
       w.EndRow();
   w.EndColumn();
   w.StartColumn();
       w.AddToggle("t1");
       w.AddToggle("t2");
   w.EndColumn();
   w.StartColumn();
       w.StartRow();
           w.AddButton("b6");
           w.AddButton("b7");
       w.EndRow();
       w.StartRow();
           w.AddButton("b8");
           w.AddButton("b9");
       w.EndRow();
   w.EndColumn();
   w.StartColumn();
       w.AddButton("B1");
       w.AddButton("B2");
   w.EndColumn();
w.EndRow();
w.StartRow();
   w.AddButton("B3","Bottom Button");
   w.SetBitmap("B3");
   w.SetTileTip("B3","HOVER!!!!");
w.EndRow();
w.StartRow();
  // w.StartColumn();
    //   w.AddGLView("g1");
   //w.EndColumn();
   w.StartColumn();
       w.AddButton("but1","hey there");
     //  w.ActionTile("but1",&insert);
   w.EndColumn();
   w.StartColumn();
       w.AddText("L1","Baseball has been very good to me");
   w.EndColumn();
   w.StartColumn();
       w.AddToggle("t3");
   w.EndColumn();
w.EndRow();


w.printTree();

w.display();
w.show();*/
//  return 0;//a.exec();


/*
int buildDialog(int argc, char *argv[]){
QApplication a(argc, argv);
MyDialog w;
w.StartRow();
   w.StartColumn();
       w.StartRow();
       for (int i =0;i<6;i++){
           w.AddButton("b1");
       }
       w.EndRow();
       w.StartRow();
           w.StartColumn();
               w.AddPopList("b2");
               char * buf;
               for (int i = 0; i<5; i++){
                   sprintf(buf,"%d",i);
                   cout<<buf<<endl;
                   w.ListAdd("b2",buf);
               }
           w.EndColumn();
           w.StartColumn();
               w.AddButton("b3");
           w.EndColumn();
           w.StartColumn();
               w.StartRow();
                   w.AddButton("b4");
               w.EndRow();
               w.StartRow();
                   w.AddButton("b5");
               w.EndRow();
           w.EndColumn();
       w.EndRow();
   w.EndColumn();
   w.StartColumn();
       w.AddToggle("t1");
       w.AddToggle("t2");
   w.EndColumn();
   w.StartColumn();
       w.StartRow();
           w.AddButton("b6");
           w.AddButton("b7");
       w.EndRow();
       w.StartRow();
           w.AddButton("b8");
           w.AddButton("b9");
       w.EndRow();
   w.EndColumn();
   w.StartColumn();
       w.AddButton("B1");
       w.AddButton("B2");
   w.EndColumn();
w.EndRow();
w.StartRow();
   w.AddButton("B3","Bottom Button");
   w.SetBitmap("B3");
   w.SetTileTip("B3","HOVER!!!!");
w.EndRow();
w.StartRow();
   w.StartColumn();
   //    w.AddGLView("g1");
   w.EndColumn();
   w.StartColumn();
       w.AddButton("but1","hey there");
     //  w.ActionTile("but1",&insert);
   w.EndColumn();
   w.AddText("L1","Baseball has been very good to me");
w.EndRow();


w.printTree();

w.display();
w.show();*/


