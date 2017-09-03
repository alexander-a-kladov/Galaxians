#include <qwidget.h>
#include <qimage.h>
#include <QMouseEvent>
#include <QTimer>

#ifndef __GALAXIANS_H__
#define __GALAXIANS_H__

const int WIDTH=640;
const int HEIGHT=640;

typedef struct {
    int x,y;
} TFire;

typedef struct {
    int x,y;
    int type;
    int alive;
} TAlien;

typedef struct {
    int x,y;
    QImage img;
} TArmor;

const int max_aliens=50;
const int max_alien_fire=10;

class Galaxians : public QWidget
{
    Q_OBJECT
    public:
	Galaxians(QWidget *parent=0) : QWidget(parent) {
	    setFixedSize(WIDTH,HEIGHT);
	    setWindowTitle("Space invaders!");
	    setMouseTracking(true);
	    armors[0].img.load("armor.png");
	    armors[1].img=armors[2].img=armors[0].img;
	    ship_img.load("ship.png");
	    alien_img.load("alien.png");
	    for (int i=0;i<3;i++) {
		armors[i].x=(i+1)*width()/4-armors[0].img.width()/2;
		armors[i].y=height()-armors[0].img.height()-80;
	    }
	    move_x = 0;
	    move_y = 0;
	    dx = 1;
	    dy = 0;
	    score = 0;
	    num_fires = 0;
	    lives = 10;
	    fillAliens();
	    ship_move=0;
	    ship_x = width()/2;
	    ship_y = height()-ship_img.height()-10;
	    ship_fl = false;
	    tmr = new QTimer(this);
	    connect(tmr,SIGNAL(timeout()),this,SLOT(timeOut()));
	    tmr->start(30);
	}
    protected:
	QImage ship_img,alien_img;
	QTimer *tmr;
	int ship_x,ship_y;
	int move_x,move_y;
	int dx,dy;
	TArmor  armors[3];
	TAlien  aliens[max_aliens];
	TFire   alien_fire[max_alien_fire];
	int num_fires;
	int num_aliens;
	int score;
	int lives;
	int ship_move;
	TFire ship_f;
	bool ship_fl;
	void moveFire();
	void fillAliens();
	void alienShoot();
	void hitAliens();
	void moveShip();
	void hitShip();
	void drawAliens(QPainter &p);
	void hitBlocks();
	bool clearBlock(int i, int x, int y);
	void moveAliens();
	void drawScore(QPainter &p);
	void paintEvent(QPaintEvent *ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void keyPressEvent(QKeyEvent *ev);
    protected slots:
	void timeOut();
};

#endif
