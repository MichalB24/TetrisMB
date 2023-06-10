#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QGraphicsScene>
#include "game.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>

class QSoundEffect;
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject* parent = nullptr);
    void start();
    void stop();

private:

    Game game;
    QGraphicsPixmapItem* m_background, *m_tiles, *m_frame;

    const float timePerFrame;
    QTimer *timer;

    QSoundEffect* m_hitSFX;
    bool m_isMuted;

    void drawScore();
    void drawState();
    void gameOver();
    
    void drawActiveFigure();

    void moveFigure();


    void drawLevel();

    void drawBackground();
    void drawRecord();
    void rotateFigure();
    void drawInstruction();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void update();
signals:
    void goToMenuActivated();
};

#endif // INTERFACE_H
