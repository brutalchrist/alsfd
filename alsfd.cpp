#include "alsfd.h"
#include "ui_alsfd.h"

#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QTextCodec>

ALSFD::ALSFD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ALSFD)
{
    ui->setupUi(this);

    /*Codec UTF-8*/
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(linuxCodec);

    //Inicializar flags
    conectado = false;
    maxBytes = 1;

    /*Inicializar el Tray*/
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->setIcon(QIcon("imagenes/icon.png"));
    trayIcon->show();

    //Inicializar puerto
    port = new QextSerialPort(QextSerialPort::EventDriven);
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    //Buscar dispositivos
    cargarListaDisp();

    //inicializar fuente OpenCV
    cvInitFont( &font, CV_FONT_VECTOR0, 0.5, 0.5, 0, 2.0, CV_AA);

    startTimer(100);
}

ALSFD::~ALSFD()
{
    delete ui;
}

/* Eventos */
void ALSFD::timerEvent(QTimerEvent *event)
{
  if(conectado){
    msg = "";
    port->flush();
    //if(numBytes > 1024) numBytes = 1024;
    i = port->read(buff, maxBytes);
    buff[i] = '\0';
    msg = buff;
    if(!msg.compare("1")){
      QString nombre = QDateTime::currentDateTime().toString();
      nombre.replace(" ", "-");
      qDebug()<< nombre;
      tomarFoto(nombre);
      ui->listaEventos->addItem(nombre);
      ui->listaEventos->item(ui->listaEventos->count()-1)->setForeground(Qt::blue);
      trayIcon->showMessage("Nueva interrupcion", "Nueva interrupcion - " + nombre + "!", QSystemTrayIcon::Warning, 6000);
      trayIcon->setIcon(QIcon("imagenes/icon_alert.png"));
    }
  }
}

void ALSFD::closeEvent( QCloseEvent *event )
{
  trayIcon->showMessage("Mensaje", "La aplicacion no a sido cerrada, solo minimizada al tray", QSystemTrayIcon::Information, 3000);
}

void ALSFD::on_conectarBoton_clicked()
{
  QString portname = ui->puertoSelector->itemText(ui->puertoSelector->currentIndex());
  if(!portname.isEmpty()){
      port->setPortName(portname.toLatin1());
      port->open(QIODevice::ReadWrite);
      if(port->isOpen())
      {
        ui->desconectarBoton->setEnabled(true);
        ui->conectarBoton->setEnabled(false);
        conectado = true;
      }
      else
        error("Imposible realizar conexión al dispositivo.");
  }
}

void ALSFD::on_desconectarBoton_clicked()
{
  port->close();
  ui->conectarBoton->setEnabled(true);
  ui->desconectarBoton->setEnabled(false);
  conectado = false;
}

void ALSFD::on_reload_clicked()
{
  cargarListaDisp();
}

void ALSFD::on_listaEventos_itemClicked(QListWidgetItem* item)
{
  QString imagen = item->text();
  imagen.prepend("capturas/");
  imagen.append(".jpg");

  ui->lblImagen->setPixmap(QPixmap(imagen));
}

void ALSFD::on_actionSalir_triggered()
{
  exit(0);
}

void ALSFD::on_actionAcerca_de_triggered()
{
  //A.move(((this->width()/2 +this->x()) - A.width()/2), ((this->height()/2 + this->y()) - this->height()/2));
  //A.show();
}

/*Metodos tray*/
void ALSFD::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
    case QSystemTrayIcon::Trigger:
      if(this->isVisible())
        this->hide();
      else if(this->isHidden())
        this->showNormal();
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
    default:
    ;
  }
}

void ALSFD::createTrayIcon()
{
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}

void ALSFD::createActions()
{
  minimizeAction = new QAction(tr("Mi&nimizar"), this);
  connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

  maximizeAction = new QAction(tr("Ma&ximizar"), this);
  connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

  restoreAction = new QAction(tr("&Restaurar"), this);
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

  quitAction = new QAction(tr("&Salir"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

/*Metodos*/
void ALSFD::tomarFoto(QString nombre)
{
  cam = cvCreateCameraCapture(1);
  frame = cvQueryFrame(cam);
  IplImage *final = cvCreateImage(cvSize(540, 380), frame->depth, frame->nChannels);
  cvResize(frame, final);
  cvPutText(final, nombre.toLatin1(), cvPoint(10, 20), &font,CV_RGB(0,255,0));
  nombre.prepend("capturas/");
  nombre.append(".jpg");
  cvSaveImage(nombre.toLatin1(), final);
  cvReleaseCapture(&cam);
}

void ALSFD::cargarListaDisp()
{
  devicename="/dev/ttyACMX";
  for (int i=0; i<8; i++)
  {
    devicename.chop(1);
    if (devices.exists(devicename.append(i+48)))
      ui->puertoSelector->addItem(devicename.toLatin1());
  }
}

void ALSFD::error(QString mensaje)
{
  QMessageBox msgBox(QMessageBox::Warning, tr("Error"), mensaje, 0, this);
  msgBox.exec();
}
