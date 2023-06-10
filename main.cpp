#include <QApplication>
#include <QCursor>
#include <QSoundEffect>

#include "interface.h"
#include <QGraphicsView>




class GameScene;
class MenuScene;
class SettingsScene;
class QSoundEffect;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//muzyka*****************
    QSoundEffect* mysicTheme;
    mysicTheme = new QSoundEffect();
    mysicTheme->setSource(QUrl("qrc:/pliki/Korobienki.wav"));
    mysicTheme->setLoopCount(QSoundEffect::Infinite);
    mysicTheme->setVolume(0.5f);
    mysicTheme->play();


    GameScene* gra;
    gra=new GameScene();
    gra->start();

    QGraphicsView w;
    w.setScene(gra);
    w.setCursor(QCursor(Qt::BlankCursor));


    w.show();

    return a.exec();
}
