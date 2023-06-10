#include "game.h"
#include <QDebug>
#include <cstdlib>


//change to increase or decrease speed of game.
const float Game::SPEED = 500.0f;//in milisecond
const float Game::SPEED_UP = 100.0f;//in milisecond

const int Game::COUNT_OF_COLORS = 7;
const int Game::COUNT_OF_FIGURES = 7;
const int Game::COUNT_OF_BLOCKS = 4;

QSize Game::BLOCK_SIZE = QSize(18, 18);
QSize Game::RESOLUTION = QSize(520, 420);

Game::Game() : BOARD_HEIGHT(20), BOARD_WIDTH(10), m_dx(0), m_rotate(false),m_colorNum(1), m_timer(0), m_delay(SPEED), m_speedLev(1), m_state(State::Active), m_score(0), m_record(0), m_level(1), m_gameOver(false)
{
    for(int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for(int j = 0; j < BOARD_WIDTH; ++j)
        {
            m_field[i][j] = 0;
        }
    }
    //set zero for points
    for(int i = 0; i < 4; ++i)
    {
        m_a[i].x = 0;
        m_a[i].y = 0;
        m_b[i].x = 0;
        m_b[i].y = 0;
    }

    //       ****
    m_figures[0][0] = 1;          //  ****
    m_figures[0][1] = 3;          //  ****
    m_figures[0][2] = 5;          //  ****
    m_figures[0][3] = 7;          //  ****

    //      *
    //      **
    //       *
    m_figures[1][0] = 2;
    m_figures[1][1] = 4;
    m_figures[1][2] = 5;
    m_figures[1][3] = 7;

    //       *
    //      **
    //      *
    m_figures[2][0] = 3;
    m_figures[2][1] = 4;
    m_figures[2][2] = 5;
    m_figures[2][3] = 6;

    //      ***
    //       *
    m_figures[3][0] = 3;
    m_figures[3][1] = 4;
    m_figures[3][2] = 5;
    m_figures[3][3] = 7;

    //     **
    //      *
    //      *
    m_figures[4][0] = 2;
    m_figures[4][1] = 3;
    m_figures[4][2] = 5;
    m_figures[4][3] = 7;

    //      *
    //      *
    //     **
    m_figures[5][0] = 3;
    m_figures[5][1] = 5;
    m_figures[5][2] = 6;
    m_figures[5][3] = 7;


    //     **
    //     **
    m_figures[6][0] = 2;
    m_figures[6][1] = 3;
    m_figures[6][2] = 4;
    m_figures[6][3] = 5;



    //TEKSTURY
    if( !m_frame.load(":/pliki/plansza.png") )
    {
        qDebug() << "Nie znaleziono pliku: plansza.png";
    }
    if( !m_tile.load(":/pliki/klocki.png") )
    {
        qDebug() << "Nie znaleziono pliku:  klocki.png";
    }
}

bool Game::check()//sprawdza warunek czy obiekt moze sie przesunac
{
    for (int i=0;i<4;i++)
    {
        if (m_a[i].x<0 || m_a[i].x>=BOARD_WIDTH || m_a[i].y>=BOARD_HEIGHT)
        {
            return false;
        }
        else if (m_field[m_a[i].y][m_a[i].x])
        {
            return false;
        }
    }
    return true;
}

void Game::addScore(int val)
{
    m_score=m_score+val;
}

void Game::newRecord(int val)
{
    if(m_record<val)
        m_record=val;
}

void Game::levelUp()
{
    if(m_score%50==0&&m_score!=0)
    {
    m_level++;
    m_speedLev=m_speedLev+0.25;
    qDebug() <<"Poziom trudnosci zwiekrzono: "<<m_speedLev;
    }


}

void Game::reset()
{
    m_score=0;
    m_gameOver=false;
    m_speedLev=1;
    m_level=1;
    for(int i =0;i<BOARD_HEIGHT;++i)
    {
        for(int j=0;j<BOARD_WIDTH;++j)
        {
            m_field[i][j]= 0;
        }
    }

    //resetuje gre
    for(int i = 0; i < 4; ++i)
    {
        m_a[i].x = 0;
        m_a[i].y = 0;
        m_b[i].x = 0;
        m_b[i].y = 0;
    }


    srand(time(0));
    int n = rand() % Game::COUNT_OF_FIGURES;
    for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++)
    {
        m_a[i].x = (m_figures[n][i]%2)+BOARD_WIDTH/2-1;
        m_a[i].y = m_figures[n][i]/2;
    }

    m_timer = 0.0f;
    m_state = State::Active;
}
