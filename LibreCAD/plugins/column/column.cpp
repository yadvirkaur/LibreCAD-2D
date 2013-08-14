/*****************************************************************************/
/*  column.cpp - plugin example for LibreCAD                                 */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QDoubleValidator>
#include <math.h>
#include "document_interface.h"
#include "column.h"
#include<QPainter>

QString LC_Sample::name() const
 {
     return (tr("column"));
 }

PluginCapabilities LC_Sample::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("column"));
    return pluginCapabilities;
}

void LC_Sample::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_Sampledlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_Sampledlg::lc_Sampledlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("Column"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

 
    label = new QLabel(tr("Outer Radius"));
    mainLayout->addWidget(label, 0, 0);
    outerradius = new QLineEdit();
    outerradius->setValidator(val);
    mainLayout->addWidget(outerradius, 0, 1);

    label = new QLabel(tr("Spacing Between Outer and Stirup"));
    mainLayout->addWidget(label, 1, 0);
    spaceedit = new QLineEdit();
    spaceedit->setValidator(val);
    mainLayout->addWidget(spaceedit, 1, 1);

    label = new QLabel(tr("No. of reinforcing bar:"));
    mainLayout->addWidget(label, 2, 0);
    baredit = new QLineEdit();
    baredit->setValidator(val);
    mainLayout->addWidget(baredit, 2, 1);


    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 4, 0);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 4, 1);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
}


bool lc_Sampledlg::failGUI(QString *msg)
{
    
    if (outerradius->text().isEmpty()) {msg->insert(0, tr("outer radius field is empty")); return true;}
    if (spaceedit->text().isEmpty()) {msg->insert(0, tr("Spacing field is empty")); return true;}


    return false;
}


void lc_Sampledlg::procesAction(Document_Interface *doc)
{
    Q_UNUSED(doc);
    double sp; //spacing b/w stirrup and outer
    double radius1; // outer radius
    int angle=360,bars; // steel bars
    radius1= outerradius->text().toDouble();
    sp= spaceedit->text().toDouble();
    bars=baredit->text().toInt();
    double radius2= radius1-sp;
    double radiusin=radius1-sp-8;
    double radius3= radius1-sp-16;
    double radius4 = 8;
    //double radius5 = radius1-sp-40;

    QPointF Coord_a(20.0,0.0),Coord_b(0,3*radius1),Coord_c(2*radius1,0),Coord_d(0,0.0-3*radius1),mid_1(radius1,0);
    QPointF circum_1,circum_pt,start(0,radius1+40),end_pt;
    QPointF Coord_e, Coord_f,Coord_g,Coord_h;
            Coord_b += Coord_a;
            Coord_c += Coord_b;
            Coord_d +=Coord_c;
            mid_1 += Coord_b;
            start += mid_1;
            end_pt +=Coord_a;
            Coord_e += Coord_a;
            Coord_f +=Coord_d;
            Coord_g +=Coord_a;
            Coord_h +=Coord_d;

    doc->setLayer("lay1");
    doc->setCurrentLayerProperties(QColor(255,0,255, 127),DPI::Width15  , DPI::SolidLine);
    doc->addLine(&Coord_a,&Coord_b);
    doc->addCircle(&start,radius1);
    doc->addLine(&Coord_b,&Coord_c);
    doc->addLine(&Coord_c,&Coord_d);

    doc->setLayer("lay2");
    doc->setCurrentLayerProperties(QColor(0,0,255, 127),DPI::Width20  , DPI::SolidLine);
    doc->addCircle(&start, radius2);
    doc->addCircle(&start, radiusin);
    
    int ds= angle/bars;
    if(ds>=(2*radius4))
    {

       for (int i=0; i!=angle; i = i + ds )
       {
           circum_1.setY(start.ry()+ radius3*cos(i*.017455));
           circum_1.setX(start.rx()+radius3*sin(i*.017455));
           doc->addCircle(&circum_1,radius4);

           for (int j=90; j<angle; j= j+180)
           {
               circum_pt.setY(3*radius1);
               circum_pt.setX(circum_1.rx() + radius4*sin(j*0.017455));
               end_pt.setX(mid_1.rx()+radius3*sin(i*.017455)+radius4*sin(j*.017455));
               doc->addLine (&circum_pt,&end_pt);
           }

       }

       for (int k=0; k<(3*radius1-3*radius1/(.5*bars)); k= k+3*radius1/(.5*bars+1))
       {
           Coord_e.setY((3*radius1/(.5*bars+1)+k));
           Coord_e.setX(Coord_a.rx()+sp);
           Coord_f.setY((3*radius1/(.5*bars+1)+k));
           Coord_f.setX(Coord_d.rx()-sp);
           Coord_g.setY(Coord_e.ry()+8);
           Coord_g.setX(Coord_e.rx());
           Coord_h.setY(Coord_f.ry()+8);
           Coord_h.setX(Coord_f.rx());
           doc->addLine (&Coord_e,&Coord_f);
           doc->addLine (&Coord_g,&Coord_h);
           doc->addLine (&Coord_e,&Coord_g);
           doc->addLine (&Coord_f,&Coord_h);


       }
    }
   
}


void lc_Sampledlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "Sample plugin", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_Sampledlg::~lc_Sampledlg()
{
}
void lc_Sampledlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_Sampledlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "column_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

 outerradius->setText( settings.value("outer radius",300).toString() );
    spaceedit->setText( settings.value("spacing", 40).toString() );
    baredit->setText( settings.value("reinforcing bars", 6).toString() );


    resize(size);
    move(pos);
 }

void lc_Sampledlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "column_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
 settings.setValue("Outer radius", outerradius->text());
    settings.setValue("endy", spaceedit->text());
    settings.setValue("Reinforcing Bars", baredit->text());
 }


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_column, LC_Sample);
#endif
