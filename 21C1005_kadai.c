#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include"drawlib.h"

#define ZSCALE 500
#define MAXMNG 7
#define MAXPOLI 1000

//配列の先頭はポリゴンの数
double six[12][3] = {{11,0,0},{0.0,0.0,34.0},{-20.0,-17.0,0.0},{0.0,17.0,0.0},{0.0,0.0,-34.0},{-20.0,-17.0,0.0},{20.0,-17.0,0.0},{0.0,0.0,34.0},{0.0,17.0,0.0},{20.0,-17.0,0.0},{0.0,0.0,-34.0},{-20.0,-17.0,0.0}};

double stage[44][3] = {{43.0,0.0,0.0},{-50.0,0.0,-50.0},{50.0,0.0,-50.0},{50.0,0.0,-60.0},{-50.0,0.0,-60.0},{-50.0,0.0,-70.0},{50.0,0.0,-70.0},{50.0,0.0,-80.0},{-50.0,0.0,-80.0},{-50.0,0.0,-90.0},{50.0,0.0,-90.0},{50.0,0.0,-100.0},{-50.0,0.0,-100.0},{-50.0,0.0,-110.0},{50.0,0.0,-110.0},{50.0,0.0,-120.0},{-50.0,0.0,-120.0},{-50.0,0.0,-130.0},{50.0,0.0,-130.0},{50.0,0.0,-140.0},{-50.0,0.0,-140.0},{-50.0,0.0,-150.0},{50.0,0.0,-150.0},{50.0,0.0,-50.0},{40.0,0.0,-50.0},{40.0,0.0,-150.0},{30.0,0.0,-150.0},{30.0,0.0,-50.0},{20.0,0.0,-50.0},{20.0,0.0,-150.0},{10.0,0.0,-150.0},{10.0,0.0,-50.0},{0.0,0.0,-50.0},{0.0,0.0,-150.0},{-10.0,0.0,-150.0},{-10.0,0.0,-50.0},{-20.0,0.0,-50.0},{-20.0,0.0,-150.0},{-30.0,0.0,-150.0},{-30.0,0.0,-50.0},{-40.0,0.0,-50.0},{-40.0,0.0,-150.0},{-50.0,0.0,-150.0},{-50.0,0.0,-50.0}};
//X,Y,Z,pitch,yaw,roll,type,time,scale
//0~3:壁 4:奥 5:ボール 6:手前
double mng[MAXMNG][9] = {{0,50,150,0,0,0,1,0,4},{0,-50,150,0,0,0,1,0,4},
			{-50,0,150,0,0,M_PI/2,1,0,4},{50,0,150,0,0,M_PI/2,1,0,4},
			{0,-100,400,M_PI/2,0,0,1,0,1.0},{50,-50,200,0,M_PI/90,0,0,0,1.0},
			{0,-100,0,M_PI/2,0,0,1,0,1.0}};
double vx=0,vy=0,vz=0;

void Move(double *,double,double,double);
void Rotate(double *,double *,double,double,double);
void Scale(double *,double *,double);
double *GetPoliadrs(int,int);
void RestartGame();

int main(){
	double cam[6] = {0,-1000,1,0,M_PI/2,0};
	int pscore=0,cscore=0,timer=0;

	dl_initialize(1.5);
	
	dl_clear(DL_C("black"));
	dl_text("3D-PONG",75,200,3.0,DL_C("white"),3);
	dl_text("Use WASD keys to move",100,300,1.0,DL_C("white"),1);
	dl_text("First to 13 points wins",100,330,1.0,DL_C("white"),1);
	dl_text("Press Enter key to start.",100,360,1.0,DL_C("white"),1);

	int t,k=0,x,y;
	while(k!=DL_KEY_ENTER){
		dl_get_event(&t,&k,&x,&y);
	}

	srand((unsigned) time(NULL));

	while(1){
		double draw[MAXPOLI][3];
		int slot = 0;
		for(int i=0;i<MAXMNG;i++){
			double *tmp = GetPoliadrs((int)mng[i][6],0);
			int poli = (int)tmp[0];

			mng[i][2]+=0;
			if(mng[i][2] >= 5000) mng[i][2] = 0;
			mng[i][7]++;

			for(int j=0;j<poli;j++){
				double *base = GetPoliadrs(mng[i][6],j+1);
				int adrs = j + slot;
				Rotate(draw[adrs],base,mng[i][3],mng[i][4],mng[i][5]);
				Move(draw[adrs],mng[i][0],mng[i][1],mng[i][2]);
				Scale(draw[adrs],draw[adrs],mng[i][8]);
			}
			slot+=poli;
		}
		slot = 0;
		
		if(dl_get_event(&t,&k,&x,&y)){
			if(t == DL_EVENT_KEY){
				if(k == 'a' && mng[6][0] > -150) mng[6][0]-=30;
				if(k == 'd' && mng[6][0] < 150) mng[6][0]+=30;
				if(k == 'w' && mng[6][1] > -250) mng[6][1]-=30;
				if(k == 's' && mng[6][1] < 50) mng[6][1]+=30;
			}
		}

		double tx=mng[5][0],ty=mng[5][1],tz=mng[5][2];
		//敵AI
		if(tz >= 350){
			if(mng[4][0] > tx && mng[4][0] > -150) mng[4][0]-=5;
			if(mng[4][0] < tx && mng[4][0] < 150) mng[4][0]+=5;
			if(mng[4][1] > ty-100 && mng[4][1] > -250) mng[4][1]-=5;
			if(mng[4][1] < ty-100 && mng[4][1] < 50) mng[4][1]+=5;
		}
		mng[5][0]+=vx,mng[5][1]+=vy,mng[5][2]+=vz;
		mng[5][3]+=vx/30,mng[5][4]+=vy/30,mng[5][5]+=vz/30;
		//当たり判定
		if(tx > 200-vx || tx < -200-vx) vx*=-1;
		if(ty > 200-vy || ty < -200-vy) vy*=-1;
		if(tz > 400-vz || tz < 0-vz){
			int ck;
			if(tz > 200) ck = 4;
			else ck = 6;
			double cx=mng[ck][0],cy=mng[ck][1];
			if(tx >= cx-50 && tx <= cx+50 && ty >= cy+50 && ty <= cy+150){
		       		vz*=-1;
				mng[5][2]+=vz*3;
			}
		}
		//ゲーム判定
		if(tz > 410.0){
			pscore++;
			RestartGame();
		}else if(tz < -10.0){
			cscore++;
			RestartGame();
		}
		if(pscore >= 13 || cscore >= 13) break;

		//オープニング
		if(timer < 500){
			if(timer <= 250) cam[1]+=4;
			if(timer <=500 && timer > 250) cam[4]-=M_PI/500;
		}else if(timer == 500){
			RestartGame();
			cam[4] = 0;
		}

		//スピードアップ
		if(timer % 1000 == 0){
			vx+=0.5;
			vy+=0.5;
		}
		
		//描画
		dl_stop();
		dl_clear(DL_RGB(0,0,0));

		for(int g=0;g<MAXMNG;g++){
			double *tmp = GetPoliadrs((int)mng[g][6],0);
			int poli = (int)tmp[0];

			for(int i=0;i<poli;i++){
				int adrs = i + slot;
				Move(draw[adrs],cam[0],cam[1],cam[2]);
				Rotate(draw[adrs],draw[adrs],cam[3],cam[4],cam[5]);
			}

			for(int i=0;i<poli;i++){
				int j = i+1;
				if(j>=poli) j = 0;
				int adrs = i + slot;
				int adrs2 = j + slot;
				double sz = draw[adrs][2]/ZSCALE+1,ez = draw[adrs2][2]/ZSCALE+1;
				double sx = draw[adrs][0]/sz+320.0,ex = draw[adrs2][0]/ez+320.0;
				double sy = draw[adrs][1]/sz+240.0,ey = draw[adrs2][1]/ez+240.0;
				if((isfinite(sx) &&isfinite(ex) && isfinite(sy) && isfinite(ey)) && (sz >= 1 || ez >= 1)){
					unsigned long cl = DL_RGB(255,255,255);
					switch(g){
						case 4:
							cl = DL_RGB(256,128,0);
							break;
						case 5:
							cl = DL_RGB(255,0,0);
							break;
						case 6:
							cl = DL_RGB(0,128,256);
							break;
					}
					dl_line(sx,sy,ex,ey,cl,1);
				}
			}
			slot+=poli;
		}
		char ps[3],cs[3];
		sprintf(ps,"%02d",pscore);
		sprintf(cs,"%02d",cscore);
		dl_text("YOU",50,290,0.6,DL_C("white"),1);
		dl_text("CPU",550,290,0.6,DL_C("white"),1);
		dl_text(ps, 50, 320, 1.0, DL_C("white"), 1);
		dl_text(cs, 550, 320, 1.0, DL_C("white"), 1);
		dl_resume();
		timer++;
	}
	//ゲームオーバー
	dl_text("GAMEOVER",75,200,3.0,DL_C("white"),3);
	dl_wait(3);
}


void Move(double *draw,double x,double y,double z){
	draw[0] += x;
	draw[1] += y;
	draw[2] += z;
}

void Rotate(double *draw,double *base,double p,double y,double r){
	double pos[3] = {base[0],base[1],base[2]};
	draw[0] = pos[0]*cos(r)*cos(y) + pos[1]*(cos(r)*sin(y)*sin(p) - sin(r)*cos(p)) 
		+ pos[2]*(sin(p)*sin(r)+cos(p)*sin(y)*cos(r));
	draw[1] = pos[0]*sin(r)*cos(y) + pos[1]*(sin(p)*sin(y)*sin(r) + cos(p)*cos(r))
	       	+ pos[2]*(sin(r)*sin(y)*cos(p)-cos(r)*sin(p));
	draw[2] = pos[1]*sin(p)*cos(y) + pos[2]*cos(p)*cos(y) - pos[0]*sin(y);
}

void Scale(double *draw,double *base,double scale){
	for(int i = 0;i<3;i++) draw[i]=base[i]*scale;
}

double *GetPoliadrs(int type,int adrs){
	switch(type){
		case 0:
			return six[adrs];
		case 1:
			return stage[adrs];
		default:
			return NULL;
	}
}

void RestartGame(){
	dl_wait(0.5);
	vx=0.5*(rand()%10);
	vy=0.5*(rand()%10);
	vz=0.5*(rand()%4)+1;
	mng[5][0] = 0;
	mng[5][1] = 0;
	mng[5][2] = 200;
}
