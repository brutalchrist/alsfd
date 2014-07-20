#ifndef ALSFD_H
#define ALSFD_H

#include <QMainWindow>
#include <QFile>
#include <QSystemTrayIcon>
#include <QListWidgetItem>

#include <cv.h>
#include <highgui.h>

#include "qextserialport.h"

namespace Ui {
class ALSFD;
}

class ALSFD : public QMainWindow
{
    Q_OBJECT

public:
    explicit ALSFD(QWidget *parent = 0);
    ~ALSFD();
    QString devicename, msg;
    QFile devices;
    QextSerialPort *port;
    bool conectado;
    char buff[1024];
    int numBytes, maxBytes, i;
    //acercaDe A;

private:
    Ui::ALSFD *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    CvCapture *cam;
    IplImage *frame;
    CvFont font;

private slots:
    void timerEvent(QTimerEvent *event);
    void closeEvent( QCloseEvent *event );
    void on_conectarBoton_clicked();
    void on_desconectarBoton_clicked();
    void on_reload_clicked();
    void on_listaEventos_itemClicked(QListWidgetItem* item);
    void on_actionSalir_triggered();
    void on_actionAcerca_de_triggered();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void createTrayIcon();
    void createActions();

    void tomarFoto(QString nombre);
    void cargarListaDisp();
    void error(QString mensaje);

};

#endif // ALSFD_H
