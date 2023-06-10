#include "interface.h"

#include <QKeyEvent>
#include <QDir>
#include <QPainter>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent), game(), timePerFrame(1000.0f/60.0f)
{
    setSceneRect(0,0, Game::RESOLUTION.width(), Game::RESOLUTION.height() );
    srand(time(0));

    m_frame = new QGraphicsPixmapItem(game.m_frame);
    m_tiles = new QGraphicsPixmapItem(game.m_tile);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScene::update);


}

void GameScene::start()
{
    game.reset();
    timer->start(timePerFrame);

}

void GameScene::stop()
{
    timer->stop();
}


void GameScene::keyPressEvent(QKeyEvent *event)
{
    if( !event->isAutoRepeat() )
    {
        switch(event->key())
        {
        case Qt::Key_Left:
            if(game.m_state == Game::State::Active)
            {
                game.m_dx = -1;
            }
            break;

        case Qt::Key_Right:
            if(game.m_state == Game::State::Active)
            {
                game.m_dx = 1;
            }
            break;

        case Qt::Key_Up:
            if(game.m_state == Game::State::Active)
            {
                game.m_rotate = true;
            }
            break;

        case Qt::Key_Down:
            if(game.m_state == Game::State::Active)
            {
                game.m_delay = Game::SPEED_UP;
            }

            break;
        case Qt::Key_Space:
            if(game.m_state == Game::State::Active)
            {
                game.m_state = Game::State::Paused;
            }
            else if( game.m_state == Game::State::Paused )
            {
                game.m_state = Game::State::Active;
            }
            break;



        default:
            break;
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if( !event->isAutoRepeat() )
    {

        switch(event->key())
        {
        case Qt::Key_Left:
        case Qt::Key_Right:
            game.m_dx = 0;
            break;

        case Qt::Key_Up:
            game.m_rotate = false;
            break;

        case Qt::Key_Down:
            game.m_delay = Game::SPEED;
        default:
            break;
        }
    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::drawScore()  //wynik
{
    QString scoreString = QString("Twój wynik:\n%1").arg(game.m_score);

    QGraphicsTextItem* scoreTextItem = new QGraphicsTextItem(scoreString, nullptr);
    QFont font("Arial", 12, QFont::Bold);
    scoreTextItem->setFont(font);
    scoreTextItem->setDefaultTextColor(Qt::black);
    scoreTextItem->setPos(0, 320);
    addItem(scoreTextItem);

}

void GameScene::drawRecord()  //wynik
{
    QString scoreString = QString("Rekord:\n%1").arg(game.m_record);

    QGraphicsTextItem* scoreTextItem = new QGraphicsTextItem(scoreString, nullptr);
    QFont font("Arial", 12, QFont::Bold);
    scoreTextItem->setFont(font);
    scoreTextItem->setDefaultTextColor(Qt::black);
    scoreTextItem->setPos(0, 240);
    addItem(scoreTextItem);


}


void GameScene::drawLevel()  //wynik
{
    QString scoreString = QString("Level: %1").arg(game.m_level);

    QGraphicsTextItem* scoreTextItem = new QGraphicsTextItem(scoreString, nullptr);
    QFont font("Arial", 20, QFont::Bold);
    scoreTextItem->setFont(font);
    scoreTextItem->setDefaultTextColor(Qt::black);
    scoreTextItem->setPos(0, 10);
    addItem(scoreTextItem);
}

void GameScene::drawState()
{
    QString stan;

    if (game.m_state == Game::State::Paused)
    {
        stan = "Pauza";
    }
    else if (game.m_state == Game::State::Active)
    {
        stan = "Aktywna";
    }
    else if (game.m_state == Game::State::Game_Over)
    {
        stan = "Game Over";
    }

    QString stanGry = QString("Stan gry: %1").arg(stan);
    QGraphicsTextItem* stateTextItem = new QGraphicsTextItem(stanGry, nullptr);
    QFont font("Arial", 10, QFont::Bold);
    stateTextItem->setFont(font);
    stateTextItem->setDefaultTextColor(Qt::black);
    stateTextItem->setPos(160, 390);
    addItem(stateTextItem);
}

void GameScene::drawInstruction()  //wynik
{
    QString instruction = QString("Sterowanie:\n");
    QGraphicsTextItem* textInstrucji = new QGraphicsTextItem(instruction, nullptr);
    QFont font1("Arial", 16, QFont::Bold);
    textInstrucji->setFont(font1);
    textInstrucji->setDefaultTextColor(Qt::black);
    textInstrucji->setPos(360, 30);
    addItem(textInstrucji);

    QString keyssys = QString("<---         -Lewo \n  --->      -Prawo \n ^      Obróć figurę\n \\/  - Przyspiesz opadanie \n (SPACJA) -Pauza gry \n");
    QGraphicsTextItem* textKeys = new QGraphicsTextItem(keyssys, nullptr);
    QFont font("Arial", 10);
    textKeys->setFont(font);
    textKeys->setDefaultTextColor(Qt::black);
    textKeys->setPos(360, 60);
    addItem(textKeys);

}

void GameScene::gameOver()
{
    if (game.m_gameOver)
    {
        QMessageBox gameOverBox;
        gameOverBox.setText("Game Over");
        gameOverBox.addButton("Kontynuj", QMessageBox::AcceptRole);
        gameOverBox.exec();
         game.m_delay = Game::SPEED;
        game.newRecord(game.m_score);
        game.reset();
    }
}

void GameScene::drawActiveFigure()//ulorzenie spadajacych klockow
{
    for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++)
    {
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(game.m_tile.copy(game.m_colorNum * Game::BLOCK_SIZE.width(), 0,
         Game::BLOCK_SIZE.width(), Game::BLOCK_SIZE.height()));
        addItem(pixmapItem);
        pixmapItem->setPos(game.m_a[i].x * Game::BLOCK_SIZE.width(), game.m_a[i].y * Game::BLOCK_SIZE.height());

        pixmapItem->moveBy(m_frame->pos().x(), m_frame->pos().y());
        //pixmapItem->moveBy(m_frame->pos().x(), m_frame->pos().y());
    }
}

void GameScene::moveFigure()
{
    for (int i=0;i<Game::COUNT_OF_BLOCKS;i++)
    {
        game.m_b[i]=game.m_a[i];
        game.m_a[i].x=game.m_a[i].x+game.m_dx;
    }

    if (!game.check())//sprawdz czy nie wystepuje kolizja z sciana
    {
        for (int i=0;i<Game::COUNT_OF_BLOCKS;i++)
        {
            game.m_a[i]=game.m_b[i];
        }
    }
}


void GameScene::drawBackground()
{

    QColor jasnoSzary(220, 220, 220); // Jasnoszary kolor tła

    // Tworzenie prostokątnego kształtu o rozmiarach sceny
    QGraphicsRectItem* backgroundItem = new QGraphicsRectItem(sceneRect());
    backgroundItem->setBrush(QBrush(jasnoSzary)); // Ustawienie koloru tła
    addItem(backgroundItem);

    m_frame = new QGraphicsPixmapItem(game.m_frame);
    addItem(m_frame);
    m_frame->moveBy(160, 31);

}

void GameScene::rotateFigure()
{
    if (game.m_rotate)
    {
        Point p = game.m_a[1]; //center of rotation
        for (int i = 0; i< Game::COUNT_OF_BLOCKS; i++)
        {
            int x=game.m_a[i].y-p.y;
            int y=game.m_a[i].x-p.x;
            game.m_a[i].x = p.x-x;
            game.m_a[i].y = p.y+y;
        }
        if (!game.check())//warunek aby figury nie wypadly z mapy
        {
            for (int i = 0; i<Game::COUNT_OF_BLOCKS; i++)
            {
                game.m_a[i]=game.m_b[i];//dopuki bloki sie nie przetna
            }
        }
        game.m_rotate = false;
    }
}

void GameScene::update()//zamienic na sterowanie
{
    if(game.m_state == Game::State::Active)
    {
        game.m_timer=game.m_timer+(timePerFrame)*game.m_speedLev;
    }

    moveFigure();

    //////Rotate//////
   rotateFigure();

    ///////Tick//////
    if ( game.m_timer > game.m_delay)
    {
        for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++)
        {
            game.m_b[i]=game.m_a[i];
            game.m_a[i].y++;
        }

        if (!game.check())
        {
            for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++)
            {
                game.m_field[game.m_b[i].y][game.m_b[i].x] = game.m_colorNum;
            }

            game.m_colorNum=(rand()%(Game::COUNT_OF_COLORS-1))+1;//zmiana kolorow
            int n= rand() % Game::COUNT_OF_FIGURES;
            for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++)
            {
                game.m_a[i].x = (game.m_figures[n][i] % 2) + game.BOARD_WIDTH/2-1;
                game.m_a[i].y = game.m_figures[n][i] / 2;



                if(game.m_field[game.m_a[i].y][game.m_a[i].x])//linijka odpowiadajaca za koniec gry
                {
                    qDebug() << "Game Over";//zmien to
                    game.m_gameOver = true;
                }
            }
        }

        game.m_timer=0;//zmienia predkosc gry na 0
    }

    //***************kasowanie lini
    for (int i=0; i<game.BOARD_HEIGHT;i++)
    {
        bool full=1;

        // Czy linia pelna
        for (int j=0;j<game.BOARD_WIDTH;j++)
        {
            if (!game.m_field[i][j])
            {
                full=0;
                break;
            }
        }

        if (full)
        {
            // usun ja
            for (int k=i;k>0;k--)
            {
                for (int j=0;j<game.BOARD_WIDTH; j++)
                {
                    game.m_field[k][j]=game.m_field[k-1][j];
                }
            }
            game.addScore(10);
            game.levelUp();


        }
    }

    game.m_dx=0;

    //drawing
    clear();

    drawBackground();

    for (int i=0;i<game.BOARD_HEIGHT;i++)//jezeli klocek spadnie na ziemie ma sie zatrzymac
    {
        for (int j=0;j<game.BOARD_WIDTH;j++)
        {
            if (game.m_field[i][j]==0) continue;
            QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(game.m_tile.copy(game.m_field[i][j]*18, 0, 18, 18));//tekstura brylek
            addItem(pixmapItem);
            pixmapItem->setPos(j*Game::BLOCK_SIZE.width(), i*Game::BLOCK_SIZE.height());
            pixmapItem->moveBy(m_frame->pos().x(), m_frame->pos().y());
        }


    }


    drawActiveFigure();
    drawLevel();
    drawScore();
    drawRecord();
    drawState();
    drawInstruction();
    gameOver();
}
