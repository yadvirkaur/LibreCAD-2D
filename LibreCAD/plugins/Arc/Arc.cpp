/*****************************************************************************/
/*  Arc.cpp - plugin example for LibreCAD                                 */
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
#include <QGroupBox>
#include "document_interface.h"
#include "Arc.h"

QString LC_Arc::name() const
 {
     return (tr("Arc plugin"));
 }

PluginCapabilities LC_Arc::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("Arc plugin"));
    return pluginCapabilities;
}

void LC_Arc::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_Arcdlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_Arcdlg::lc_Arcdlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("Draw Arc"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

    label = new QLabel(tr("Start X:"));
    mainLayout->addWidget(label, 1, 1);
    startxedit = new QLineEdit();
    startxedit->setValidator(val);
    mainLayout->addWidget(startxedit, 2, 1);

    label = new QLabel(tr("Start Y:"));
    mainLayout->addWidget(label, 3, 1);
    startyedit = new QLineEdit();
    startyedit->setValidator(val);
    mainLayout->addWidget(startyedit, 4, 1);

    label = new QLabel(tr("Enter radius:"));
    mainLayout->addWidget(label, 1, 4);
    radiusedit = new QLineEdit();
    radiusedit->setValidator(val);
    mainLayout->addWidget(radiusedit, 2, 4);
    
    label = new QLabel(tr("Enter a1x:"));
    mainLayout->addWidget(label, 1, 2);
    a1xedit = new QLineEdit();
    a1xedit->setValidator(val);
    mainLayout->addWidget(a1xedit, 2, 2);

    label = new QLabel(tr("Enter a1y:"));
    mainLayout->addWidget(label, 3, 2);
    a1yedit = new QLineEdit();
    a1yedit->setValidator(val);
    mainLayout->addWidget(a1yedit, 4, 2);

       label = new QLabel(tr("Enter a2x:"));
    mainLayout->addWidget(label, 1, 3);
    a2xedit = new QLineEdit();
    a2xedit->setValidator(val);
    mainLayout->addWidget(a2xedit, 2, 3);

     label = new QLabel(tr("Enter a2y:"));
    mainLayout->addWidget(label, 3, 3);
    a2yedit = new QLineEdit();
    a2yedit->setValidator(val);
    mainLayout->addWidget(a2yedit, 4, 3);

  
    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 1, 5);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 3, 5);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
}


bool lc_Arcdlg::failGUI(QString *msg)
{
    if (startxedit->text().isEmpty()) {msg->insert(0, tr("Start X is empty")); return true;}
    if (startyedit->text().isEmpty()) {msg->insert(0, tr("Start Y is empty")); return true;}
  
    if (radiusedit->text().isEmpty()) {msg->insert(0, tr(" radius field is empty")); return true;}

    if (a1xedit->text().isEmpty()) {msg->insert(0, tr("a1x is empty")); return true;}
    if (a1yedit->text().isEmpty()) {msg->insert(0, tr("a1y is empty")); return true;}
    if (a2xedit->text().isEmpty()) {msg->insert(0, tr("a2x is empty")); return true;}
    if (a2yedit->text().isEmpty()) {msg->insert(0, tr("a2y is empty")); return true;}
  
  
  
   return false;
  
}







  



void lc_Arcdlg::procesAction(Document_Interface *doc)
{
    Q_UNUSED(doc);
    QPointF start;
    qreal radius, a1, a2;
    
    start.setX(startxedit->text().toDouble());
    start.setY(startyedit->text().toDouble());
    radius=radiusedit->text().toDouble();
   a1=a1xedit->text().toDouble();
   a1=a1yedit->text().toDouble();
    a2=a2xedit->text().toDouble();
    a2=a2yedit->text().toDouble();
     

    doc->addArc(&start,radius,a1,a2);
}

void lc_Arcdlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "Arc plugin", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_Arcdlg::~lc_Arcdlg()
{
}
void lc_Arcdlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_Arcdlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "Arc_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

    startxedit->setText( settings.value("startx", 30).toString() );
    startyedit->setText( settings.value("starty", 60).toString() );
    radiusedit->setText( settings.value("radius", 40).toString() );

    a1xedit->setText( settings.value("a1x", 40).toString() );

  a1yedit->setText( settings.value("a1y", 40).toString() );


 a2xedit->setText( settings.value("a2x", 80).toString() );

  a2yedit->setText( settings.value("a2y", 80).toString() );
  

  
    resize(size);
    move(pos);
 }

void lc_Arcdlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "Arc_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

    settings.setValue("startx", startxedit->text());
    settings.setValue("starty", startyedit->text());
     settings.setValue("radius", radiusedit->text());
      settings.setValue("a1x", a1xedit->text());
        settings.setValue("a1y",a1yedit->text());
       settings.setValue("a2x", a2xedit->text());
      settings.setValue("a2y", a2yedit->text());

 }

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_Arc, LC_Arc);
#endif
