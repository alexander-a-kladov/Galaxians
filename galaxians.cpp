#include "galaxians.h"
#include <qpainter.h>

void Galaxians::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.setBrush(Qt::black);
    p.drawRect(0,0,width(),height());
    for (int i=0;i<3;i++)
	p.drawImage(armors[i].x,armors[i].y,armors[i].img);
    p.drawImage(ship_x-ship_img.width()/2,ship_y,ship_img);
    drawAliens(p);
    p.setPen(Qt::white);
    if (ship_fl) p.drawLine(ship_f.x, ship_f.y, ship_f.x, ship_f.y-10);
    for (int i=0;i<num_fires;i++)
	p.drawLine(alien_fire[i].x, alien_fire[i].y, 
		    alien_fire[i].x, alien_fire[i].y-10);
    p.setFont(QFont("Arial", 14));
    p.setPen(Qt::white);
    p.drawText(width()-150,20,QString("Score:%1").arg(score));
    p.drawText(50,20,QString("Hit points:%1").arg(lives));
    if (!lives) p.drawText(width()/2-50,height()/2,"Game over");
    else
	if (!num_aliens) p.drawText(width()/2-50,height()/2,"You win");
    return;
}

void Galaxians::mouseMoveEvent(QMouseEvent *ev)
{
    ship_x = ev->x();
    return;
}

void Galaxians::mousePressEvent(QMouseEvent *ev)
{
    if (!ship_fl) {
	ship_f.x = ship_x;
	ship_f.y = ship_y;
	ship_fl = true;
    }
    return;
}

void Galaxians::moveShip()
{
    if (ship_move<0) {
	if (ship_x>0) ship_x+=ship_move;
	ship_move++;
	return;
    }
    if (ship_move>0) {
	if (ship_x<width()) ship_x+=ship_move;
	ship_move--;
	return;
    }
    return;
}

void Galaxians::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
	case Qt::Key_Left:
	    //if (ship_x>0) ship_x-=10;
	    ship_move=-10;
	    break;
	case Qt::Key_Right:
	    //if (ship_x<width()) ship_x+=10;
	    ship_move=10;
	    break;
	case Qt::Key_Space:
	    if (!ship_fl) {
		ship_f.x = ship_x;
		ship_f.y = ship_y;
		ship_fl = true;
	    }
	    break;
    }
//    repaint();
}

void Galaxians::moveFire()
{
    if (ship_fl) {
	if (ship_f.y>0) {
	    ship_f.y-=10;
	}
	else ship_fl=false;
    }
    
    for (int i=0;i<num_fires;i++)
	if (alien_fire[i].y<height()) alien_fire[i].y+=10;
	    else num_fires=0;
    return;
}

void Galaxians::timeOut()
{
    if (!lives||!num_aliens) {
	tmr->stop();
	repaint();
    }
    moveShip();
    alienShoot();
    moveFire();
    hitBlocks();
    hitAliens();
    hitShip();
    moveAliens();
    repaint();
    return;
}

void Galaxians::hitAliens()
{
    if (!ship_fl) return;
    for (int i=0;i<max_aliens;i++) {
	if (aliens[i].alive) {
	    QRect rect(aliens[i].x-alien_img.width()/2,aliens[i].y,
			alien_img.width(),alien_img.height());
	    if (rect.contains(ship_f.x,ship_f.y)) {
		ship_fl = false;
		num_aliens--;
		aliens[i].alive = 0;
		score += 10;
		break;
	    }
	}
    }
    return;
}

void Galaxians::hitShip()
{
    for (int i=0;i<num_fires;i++)
	if (QRect(ship_x-ship_img.width()/2,ship_y,
	    ship_img.width(),ship_img.height())
	    .contains(alien_fire[i].x,alien_fire[i].y)) {
	    lives--;
	    num_fires=0;
	    }
    return;
}

void Galaxians::fillAliens()
{
    num_aliens = max_aliens;
    for (int i=0;i<5;i++)
    for (int j=0;j<10;j++) {
	aliens[i*10+j].x = ((j+3)*width())/15;
	aliens[i*10+j].y = i*30+20;
	aliens[i*10+j].alive = true;
    }
    return;
}


void Galaxians::drawAliens(QPainter &p)
{
    static int phase=0;
    for (int i=0;i<max_aliens;i++) {
	if (aliens[i].alive) 
	    p.drawImage(aliens[i].x-alien_img.width()/2,
			aliens[i].y, alien_img,
			0, 30*((phase>=10)?1:0), 30,29);
    }
    phase++;
    phase%=20;
    return;
}

void Galaxians::alienShoot()
{
    if (num_fires) return;
    for (int i=0;i<max_aliens;i++)
	if (aliens[i].alive) {
	    if ((aliens[i].x>ship_x-ship_img.width()/2)&&
		(aliens[i].x<ship_x+ship_img.width()/2)) {
        if //(random()%10>5) {
        (1) {
		num_fires=1;
		alien_fire[0].x = aliens[i].x;
		alien_fire[0].y = aliens[i].y+alien_img.height()-10;
		}
		}
	}
    return;
}

void Galaxians::hitBlocks()
{
    int x,y;
    for (int i=0;i<3;i++) {
	if (ship_fl) {
	    x = ship_f.x - armors[i].x;
	    y = ship_f.y - armors[i].y;
	    if (clearBlock(i,x,y)) ship_fl = false;
	}
	for (int j=0;j<num_fires;j++) {
	    x = alien_fire[j].x - armors[i].x;
	    y = alien_fire[j].y - armors[i].y;
	    if (clearBlock(i,x,y)) num_fires = 0;
	} 
    }
    return;
}

bool Galaxians::clearBlock(int i, int x, int y)
{
    bool res = false;
    for (int j=0;j<10;j++)
	    if (QRect(0,0,armors[i].img.width(),armors[i].img.height()).
		    contains(x,y-j))
	    if (armors[i].img.pixel(x,y-j)!=qRgb(0,0,0))
		{
		    res = true;
		    QPainter p(&armors[i].img);
		    p.drawLine(x,y,x,y-10);
		    break;
		}
    return res;
}

void Galaxians::moveAliens()
{
	static int a=0;
	a++;a%=5;
	if (a) return;
	if (dy) {
	    for (int i=0;i<max_aliens;i++) aliens[i].y+=10;
	    dy = 0;
	    return;
	}
	if (dx>0) 
	{
	    if (move_x+1<10) {
		move_x++;
		for (int i=0;i<max_aliens;i++) aliens[i].x+=10;
	    }
		else {dx=-1*dx;dy=1;}
	    return;
	}
	
	if (dx<0) {
	    if (move_x-1>-10) {
		move_x--;
		for (int i=0;i<max_aliens;i++) aliens[i].x-=10;
	    }
		else {dx=-1*dx;dy=1;}
	    return;
	}
    return;
}
