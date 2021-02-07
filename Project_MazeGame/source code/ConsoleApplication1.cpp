#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "glew/glew.h"
#include "glut.h"
#include "GenerateMaze.h"
#include <istream>
#include <fstream>
#include<sstream>
#include "obj.h"
#include "daochu.h"
//#include "stanford_bunny.h"
using namespace std;

#define BITMAP_ID 0x4D42
#define Height 16
#define Width 16
#define omiga 0.8
#define MAX_COIN 100
int ID;
//screenshot
string screenShotPath = "ScreenShot/";
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
	return buf;
}
bool newScreenShot(int width, int height, const char *filePath);

/*
#pragma pack(1)
typedef struct bmpheader
{
	unsigned short bfType;              //UNIT        bfType;
	unsigned long   bfSize;              //DWORD       bfSize;
	unsigned short bfReserved1;         //UINT        bfReserved1;
	unsigned short bfReserved2;         //UINT        bfReserved2;
	unsigned long   bfOffBits;           //DWORD       bfOffBits;
}BITMAPFILEHEADER;
#pragma pack()

#pragma pack(1)
typedef struct bmpmapinfo
{
	unsigned long biSize;	          // DWORD        biSize;
	long         biWidth;                // LONG         biWidth;
	long         biHeight;               // LONG         biHeight;
	unsigned short biPlanes;               // WORD         biPlanes;
	unsigned short biBitCount;             // WORD         biBitCount;
	unsigned long biCompression;          // DWORD        biCompression;
	unsigned long biSizeImage;            // DWORD        biSizeImage;
	long         biXPelsPerMerer;        // LONG         biXPelsPerMerer;
	long     	  biYPelsPerMerer;        // LONG         biYPelsPerMerer;
	unsigned long biClrUsed;              // DWORD        biClrUsed;
	unsigned long biClrImportant;         // DWORD        biClrImportant;
}BITMAPINFOHEADER;
#pragma pack()
*/

// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
//定义纹理的函数
void texload(int i, char *filename);
void mix_spot_tex(int i, char *filename1);//与光泽混合
//初始化纹理
void init_tex();

int score = 0;

const float BLOCK_SIZE = 1.0;
const float BLOCK_HEIGHT = 2.0;
const int MAZE_SIZE = 21;
vector<vector<int>> maze_matrix;
int coin_matrix[MAZE_SIZE][MAZE_SIZE];

double place = -9;
double bei = 2;
double dx = 1;
double dy = 0;
double yidong = 0.01;

int ddx = 1;
int ddy = 0;
int sstop = 0;
int coinx = 1;
int coiny = 0;

float eye[] = { place / bei, 4, 4 - (place - 1) / bei };
float center[] = { place / bei, 0, -(place - 1) / bei };
float tplace[] = { place,place - 1, 1 };
float freecenter[] = { 0, 0, 0 };
float freeeyee[] = { 0, 0, 0 };

int mark = 0;
float tRotate = 0.0f;
float fTranslate;
float fRotate;
float fScale = 1.0f;	// set initial scale value to 1.0f

bool bPersp = true;
bool bAnim = false;
bool bWire = false;
bool tAnim = false;
bool freeeye = false;

int wHeight = 0;
int wWidth = 0;

int color_type = 0;//0-white, 1-green, 2-red, 3-blue
GLuint texture[12];//纹理标识符
GLubyte image_data[Height][Width][3];  // generate 的 纹理的图片数据
int ground_tex = 1;//地板的纹理，初始化为木头的纹理
int wall_tex = 0;//墙的纹理，初始化为油画的纹理
bool wall_mix = false;//墙纹理是否混合光泽
bool ground_mix = false;//地板纹理是否混合光泽
bool edit_Map = false;//是否进入地图编辑模式
bool global_eye = false;//是否进入全局视角模式

GLfloat light_pos[] = { -25,8,0,1 };//光源位置
GLfloat light_angle = 45.0f;//光源角度
GLfloat light_Dir[] = { 1.0f, -1.0f,0.0f };//光源照射方向向量
GLfloat light_color[4][3] = { { 1, 1, 1 }, //white
							{ 0, 1, 0 }, //green
							{ 1, 0, 0 }, //red
							{ 0, 0, 1 } }; //blue

//纹理的顶点
GLfloat Tex_vertex[4][2] = { {1.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f},{0.0f,1.0f} };
extern ObjLoader Model_bunny;
void Draw_Scene()
{
	//glMatrixMode(GL_MODELVIEW);
	//draw teapoat
	glPushMatrix();
	//glTranslatef(0, 0, 4.5);
	glTranslatef(tplace[0], tplace[1], tplace[2]);
	glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
	glRotatef(-90, 0, 1, 0);
	glRotatef(tRotate, 0, 1, 0);//todo; hint: when operate the teapot, you may need to change some parameters
	glScalef(3, 3, 3);
	//glutSolidCube(1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_color[0]); // 漫反射
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_color[0]); // 镜面反射
	glMateriali(GL_FRONT, GL_SHININESS, 10);//设置镜面指数
	Model_bunny.Draw();
	glMateriali(GL_FRONT, GL_SHININESS, 0);//撤销镜面指数
	//DrawBunny();
	//tRotate = 0;
	glPopMatrix();

	//table
	/*
	glPushMatrix();
	glTranslatef(0, 0, -1);
	glScalef(10, 8, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();
	*/
	// four legs


}

void drawCube(float size)
{
	float r = size / 2;
	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(-r, -r, -r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(-r, -r, r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(r, -r, r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(r, -r, -r);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(r, -r, r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(r, r, r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(r, r, -r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(r, -r, -r);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(-r, r, -r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(-r, r, r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(r, r, r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(r, r, -r);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(-r, -r, r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(-r, r, r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(-r, r, -r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(-r, -r, -r);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(-r, -r, r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(-r, r, r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(r, r, r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(r, -r, r);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2fv(Tex_vertex[0]); glVertex3f(-r, -r, -r);
	glTexCoord2fv(Tex_vertex[1]); glVertex3f(-r, r, -r);
	glTexCoord2fv(Tex_vertex[2]); glVertex3f(r, r, -r);
	glTexCoord2fv(Tex_vertex[3]); glVertex3f(r, -r, -r);
	glEnd();
}

//draw elements
void initCoin() {
	int coinX, coinY, score;
	for (int i = 0; i < MAX_COIN; i++)
	{
		coinX = 1 + rand() % (MAZE_SIZE - 1);
		coinY = 1 + rand() % (MAZE_SIZE - 1);
		score = 1 + rand() % 3; //1~3
		coin_matrix[coinX][coinY] = score;
	}
}

void drawCoins(GLenum mode) {
	int count = 0;
	int coin_x, coin_y;
	GLfloat coin_color[] = { 0.85f,0.65f,0.2f,1.0f }; // 金黄色
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coin_color); // 漫反射
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, coin_color); // 镜面反射
	glMateriali(GL_FRONT, GL_SHININESS, 50);//设置镜面指数
	for (int i = 0; i < MAZE_SIZE; i++)
		for (int j = 0; j < MAZE_SIZE; j++) {
			if (coin_matrix[i][j]) {
				if (maze_matrix[i][j] == 1) {
					coin_matrix[i][j] = 0;
				}
				else {
					glPushMatrix();
					glTranslatef(i * BLOCK_SIZE, 0, -j * BLOCK_SIZE);
					glTranslatef(BLOCK_SIZE / 2, BLOCK_HEIGHT * 2 / 3, -BLOCK_SIZE / 2);
					if (coin_matrix[i][j] == 1)
						glutSolidSphere(BLOCK_SIZE / 4, 40, 50); //sphere
					else if (coin_matrix[i][j] == 2) {
						glTranslatef(0, -BLOCK_HEIGHT / 8, 0);
						glRotatef(-90, 1, 0, 0);
						glutSolidCone(BLOCK_SIZE / 4, BLOCK_HEIGHT / 4, 40, 50);
					}
					else {
						glutSolidTorus(BLOCK_SIZE / 7, BLOCK_SIZE / 6, 40, 50);
					}
					glPopMatrix();
				}
			}
		}
	glMateriali(GL_FRONT, GL_SHININESS, 0);//撤销镜面指数
}

void drawMaze(vector<vector<int>>& matrix, int matrix_size)
{
	glEnable(GL_NORMALIZE);
	//wall
	//光照颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_color[0]); // 漫反射
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_color[0]); // 镜面反射
	//映射纹理
	glEnable(GL_TEXTURE_2D);
	if (wall_mix && wall_tex + 6 < 12)
		glBindTexture(GL_TEXTURE_2D, texture[wall_tex + 7]);  //选择纹理texture[wall_tex]
	else
		glBindTexture(GL_TEXTURE_2D, texture[wall_tex]);  //选择纹理texture[wall_tex]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理受光照影响
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++) {
			if (matrix[i][j] == 0 || matrix[i][j] == -1) continue;
			glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, 0, -j * BLOCK_SIZE);
			glTranslatef(BLOCK_SIZE / 2, BLOCK_HEIGHT / 2, -BLOCK_SIZE / 2);
			glScalef(BLOCK_SIZE, BLOCK_HEIGHT, BLOCK_SIZE);
			drawCube(1.0);
			glPopMatrix();
		}
	glDisable(GL_TEXTURE_2D);	//关闭纹理texture[wall_tex]

	//ground
	//映射纹理
	glEnable(GL_TEXTURE_2D);
	if (ground_mix && ground_tex + 6 < 12)
		glBindTexture(GL_TEXTURE_2D, texture[ground_tex + 7]);  //选择纹理texture[ground_tex]
	else
		glBindTexture(GL_TEXTURE_2D, texture[ground_tex]);  //选择纹理texture[ground_tex]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理受光照影响
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++) {
			glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, 0, -j * BLOCK_SIZE);
			glTranslatef(BLOCK_SIZE / 2, BLOCK_HEIGHT / 2 - 0.5, -BLOCK_SIZE / 2);
			glScalef(BLOCK_SIZE, 1.0, BLOCK_SIZE);
			drawCube(1.0);
			glPopMatrix();
		}
	glDisable(GL_TEXTURE_2D);	//关闭纹理texture[ground_tex]
	//glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	drawCoins(GL_RENDER);    //draw coin
	glPopMatrix();
}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
	}
	else {
		glOrtho(-3, 3, -3, 3, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	if (bAnim) fRotate += 0.5f;
	if (tAnim) tRotate += 0.5f;
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
		//case 27:
	case 'm':
		{
		    
			glutDestroyWindow(ID);
		    daochu::show_model();
			break;
		}
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth); break; }

	case ' ': {
		if (freeeye) {
			bAnim = !bAnim;
		}
		break;
	}
	case 'o': {bWire = !bWire; break; }
	case '0': { //screenshot
		screenShotPath = screenShotPath + currentDateTime();
		screenShotPath += ".bmp";
		printf("Sceenshot: %s\n", screenShotPath.c_str());
		newScreenShot(600, 600, screenShotPath.c_str());
		break;
	}
	case 'u': {
		freeeye = !freeeye;
		if (freeeye) {
			freecenter[0] = center[0];
			freecenter[1] = center[1];
			freecenter[2] = center[2];
			freeeyee[0] = eye[0];
			freeeyee[1] = eye[1];
			freeeyee[2] = eye[2];
		}
		if (!freeeye) {
			bAnim = false;
			fRotate = 0;
			center[0] = freecenter[0];
			center[0] = freecenter[0];
			center[1] = freecenter[1];
			center[2] = freecenter[2];
			eye[0] = freeeyee[0];
			eye[1] = freeeyee[1];
			eye[2] = freeeyee[2];
		}
		break;
	}


	case 'd': {
		if (freeeye) {
			if (mark == 0) {
				eye[0] += 0.08f;
				center[0] += 0.08f;
			}
			else if (mark == 1) {
				eye[2] += 0.08f;
				center[2] += 0.08f;
			}
			else if (mark == 2) {
				eye[0] -= 0.08f;
				center[0] -= 0.08f;
			}
			else if (mark == 3) {
				eye[2] -= 0.08f;
				center[2] -= 0.08f;
			}

		}
		if (!freeeye) {
			tRotate -= 90;
			mark++;

			if (mark == 4) {
				mark = 0;

			}
			if (mark == 1) {
				eye[2] = eye[2] - 4;
				eye[0] -= 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddy++;
					}
					else {
						ddy--;
					}

				}
			}
			if (mark == 2) {
				eye[2] = eye[2] - 4;
				eye[0] += 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddx++;
					}
					else {
						ddx--;
					}

				}
			}
			if (mark == 3) {
				eye[2] = eye[2] + 4;
				eye[0] += 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddy--;
					}
					else {
						ddy++;
					}

				}
			}
			if (mark == 0) {
				eye[2] = eye[2] + 4;
				eye[0] -= 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddx--;
					}
					else {
						ddx++;
					}

				}
			}
		}

		/*tplace[0] += 0.40f;
		eye[0] += 0.08f;
		center[0] += 0.08f;*/
		//if (tplace[0] >= 1.5f) tplace[0] = 1.5f;//todo, hint:use the ARRAY that you defined, and notice the teapot can NOT be moved out the range of the table.
		break;
	}
	case 'a': {//todo
		if (freeeye) {
			if (mark == 0) {
				eye[0] -= 0.08f;
				center[0] -= 0.08f;
			}
			else if (mark == 1) {
				eye[2] -= 0.08f;
				center[2] -= 0.08f;
			}
			else if (mark == 2) {
				eye[0] += 0.08f;
				center[0] += 0.08f;
			}
			else if (mark == 3) {
				eye[2] += 0.08f;
				center[2] += 0.08f;
			}

		}
		if (!freeeye) {
			tRotate += 90;
			mark--;
			if (mark == -1) {
				mark = 3;
			}
			if (mark == 3) {
				eye[2] = eye[2] - 4;
				eye[0] += 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddy++;
					}
					else {
						ddy--;
					}

				}
			}
			if (mark == 2) {
				eye[2] = eye[2] - 4;
				eye[0] -= 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddx--;
					}
					else {
						ddx++;
					}

				}
			}
			if (mark == 1) {
				eye[2] = eye[2] + 4;
				eye[0] -= 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddy--;
					}
					else {
						ddy++;
					}

				}
			}
			if (mark == 0) {
				eye[2] = eye[2] + 4;
				eye[0] += 4;
				if (maze_matrix[ddx][ddy] == 1) {
					if (sstop == 1) {
						ddx++;
					}
					else {
						ddx--;
					}

				}
			}
		}

		break;
	}
	case 'w': {
		if (freeeye) {
			if (mark == 0) {
				eye[2] -= 0.08f;
				center[2] -= 0.08f;
			}
			else if (mark == 1) {
				eye[0] += 0.08f;
				center[0] += 0.08f;
			}
			else if (mark == 2) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 1) {
					ddy--;

				}
				if (ddy == 0) {
					if (ddy - dy < 0.24) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						sstop = 0;
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
				}
				else if (maze_matrix[ddx][ddy] != 1) {
					if (ddy - dy < 0.24) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						ddy--;
						sstop = 0;

						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}

				}

			}
			else if (mark == 3) {
				eye[0] -= 0.08f;
				center[0] -= 0.08f;
			}
		}
		if (!freeeye) {
			
			if (mark == 0) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 1) {
					ddy++;
				}
				if (maze_matrix[ddx][ddy] ==0) {

					if (dy - ddy < 0.24) {
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
					else {
						ddy++;
						sstop = 0;
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}

				}



			}
			else if (mark == 1) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 1) {
					ddx++;

				}
				if (maze_matrix[ddx][ddy] == 0) {
					if (dx - ddx < 0.24) {
						dx = dx + 2 * yidong;
						tplace[0] += 2 * yidong;
						eye[0] += yidong;
						center[0] += yidong;
					}
					else {
						ddx++;
						sstop = 0;
						
						dx = dx - 2 * yidong;
						tplace[0] -= 2 * yidong;
						eye[0] -= yidong;
						center[0] -= yidong;
					}
				}

			}
			else if (mark == 2) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 1) {
					ddy--;

				}
				if (maze_matrix[ddx][ddy] == 0) {
					if (ddy - dy < 0.24) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						ddy--;
						sstop = 0;
						
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}

				}

			}
			else if (mark == 3) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 1) {
					ddx--;

				}
				if (maze_matrix[ddx][ddy] == 0) {
					if (ddx - dx < 0.24) {
						dx = dx - 2 * yidong;
						tplace[0] -= 2 * yidong;
						eye[0] -= yidong;
						center[0] -= yidong;
					}
					else {
						ddx--;
						sstop = 0;
						
						dx = dx + 2 * yidong;
						tplace[0] += 2 * yidong;
						eye[0] += yidong;
						center[0] += yidong;
					}
				}

			}
		}

		break;
	}
	case 's': {
		if (freeeye) {
			if (mark == 0) {
				eye[2] += 0.08f;
				center[2] += 0.08f;
			}
			else if (mark == 1) {
				eye[0] -= 0.08f;
				center[0] -= 0.08f;
			}
			else if (mark == 2) {
				eye[2] -= 0.08f;
				center[2] -= 0.08f;
			}
			else if (mark == 3) {
				eye[0] += 0.08f;
				center[0] += 0.08f;
			}

		}
		if (!freeeye) {
			if (mark == 0) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 0) {
					ddy--;
				}

				if (ddy == 0) {
					if (ddy - dy < 0.28) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {

						sstop = 1;
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
				}
				else if (maze_matrix[ddx][ddy] != 1) {

					if (ddy - dy < 0.28) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						ddy--;
						sstop = 1;
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}

				}

			}
			else if (mark == 2) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 0) {
					ddy++;
				}
				if (maze_matrix[ddx][ddy] == 0) {

					if (dy - ddy < 0.28) {
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
					else {
						ddy++;
						sstop = 1;
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}

				}

			}
			else if (mark == 3) {
				if (maze_matrix[ddx][ddy] == 1 && sstop == 0) {
					ddx++;
				}
				if (maze_matrix[ddx][ddy] == 0) {
					if (dx - ddx < 0.28) {
						dx = dx + 2 * yidong;
						tplace[0] += 2 * yidong;
						eye[0] += yidong;
						center[0] += yidong;
					}
					else {
						ddx++;
						sstop = 1;
						dx = dx - 2 * yidong;
						tplace[0] -= 2 * yidong;
						eye[0] -= yidong;
						center[0] -= yidong;
					}
				}
			}
		}


		break;
	}
	case 'z': {
		if (freeeye) {
			eye[1] += 0.08f;
			center[1] += 0.08f;
		}

		break;
	}
	case 'c': {
		if (freeeye) {
			eye[1] -= 0.08f;
			center[1] -= 0.08f;
		}

		break;
	}
	case 'e': {edit_Map = !edit_Map; global_eye = !global_eye; break; }
	}

	/*switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}
	case 'e': {edit_Map = !edit_Map; break; }//进入滴入编辑模式

	case 'a': { // 整体左移
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'd': { // 整体右移
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'w': { // 整体上移
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': { // 整体下移
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': { // 整体前移
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': { // 整体后移
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	}
	*/
	if (edit_Map) {//地图编辑模式：含纹理编辑和光照编辑
		switch (k)
		{
		case '+': {//混合光泽
			ground_mix = !ground_mix;
			wall_mix = !wall_mix;
			break;
		}
		case 'h': {//切换墙壁纹理
			if (wall_tex != 6) {
				wall_tex++;
			}
			else {
				wall_tex = 0;
			}
			break;
		}
		case 'g': {//切换地板纹理
			if (ground_tex != 6) {
				ground_tex++;
			}
			else {
				ground_tex = 0;
			}
			break;
		}// 裁剪拉伸纹理（即变化纹理顶点）
		//对0，1和 1，1
		case '1': { //0, 1循环下移
			if (Tex_vertex[3][1] > 0)
				Tex_vertex[3][1] -= 0.2f;
			else
				Tex_vertex[3][1] = 1.0f;
			break;
		}
		case '2': { //0, 1循环右移
			if (Tex_vertex[3][0] < 1)
				Tex_vertex[3][0] += 0.2f;
			else
				Tex_vertex[3][0] = 0.0f;
			break;
		}
		case '3': { //1，1循环下移
			if (Tex_vertex[0][1] > 0)
				Tex_vertex[0][1] -= 0.2f;
			else
				Tex_vertex[0][1] = 1.0f;
			break;
		}
		case '4': { //1，1循环左移
			if (Tex_vertex[0][0] > 0)
				Tex_vertex[0][0] -= 0.2f;
			else
				Tex_vertex[0][0] = 1.0f;
			break;
		}
				  //对1，0和 0，0
		case '6': { //1, 0循环上移
			if (Tex_vertex[1][1] < 1)
				Tex_vertex[1][1] += 0.2f;
			else
				Tex_vertex[1][1] = 0.0f;
			break;
		}
		case '7': { //1，0循环左移
			if (Tex_vertex[1][0] > 0)
				Tex_vertex[1][0] -= 0.2f;
			else
				Tex_vertex[1][0] = 1.0f;
			break;
		}
		case '8': { //0，0循环上移
			if (Tex_vertex[2][1] < 1)
				Tex_vertex[2][1] += 0.2f;
			else
				Tex_vertex[2][1] = 0.0f;
			break;
		}
		case '9': { //0，0循环右移
			if (Tex_vertex[2][0] < 1)
				Tex_vertex[2][0] += 0.2f;
			else
				Tex_vertex[2][0] = 0.0f;
			break;
		}
		case '5': {//还原
			for (int i = 0; i < 4; i++)
			{
				Tex_vertex[i][0] = (i < 2) ? 1.0f : 0.0f;//前2个
				Tex_vertex[i][1] = (i == 0 || i == 3) ? 1.0f : 0.0f;//第一个和最后一个
			}
			break;
		}
		case 'j': {//光源下
			light_pos[0] += 0.2f;
			break;
		}
		case 'l': {//光源上
			light_pos[0] -= 0.2f;
			break;
		}
		case 'i': {//光源远
			light_pos[1] += 0.2f;
			break;
		}
		case 'k': {//光源近
			light_pos[1] -= 0.2f;
			break;
		}
		case '\'': {//光源左
			light_pos[2] += 0.2f;
			break;
		}
		case ';': {//光源右
			light_pos[2] -= 0.2f;
			break;
		}
		case ',': {//角度变大
			if (light_angle <= 89.0f)
				light_angle += 0.2f;
			break;
		}
		case '.': {//角度变小
			if (light_angle >= 1.0f)
				light_angle -= 0.2f;
			break;
		}
		case 't': {//光源颜色切换
			if (color_type != 3)
				color_type++;
			else
				color_type = 0;
			break;
		}
		}
	}

	updateView(wHeight, wWidth);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	if (global_eye) {
		gluLookAt(0, 18, 0,
			-1, -18, 0,
			0, 1, 0);				// 场景（0，0，0）的视点中心随兔子移动，Y轴向上
	}
	else {
		gluLookAt(eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
	}

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST); // 开启深度测试
	glEnable(GL_LIGHTING); // 开启光照模式

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color[color_type]);   //设置环境光成分
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color[color_type]);  //设置镜面光成分
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[color_type]);   //设置漫射光成分
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_angle);           //裁减角度
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_Dir);          //光源方向
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.);                    //聚集度
	glEnable(GL_LIGHT0);//打开聚光灯

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	/*
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 1,0 );
	glScalef(0.2, 0.2, 0.2);

	glTranslatef(- BLOCK_SIZE * MAZE_SIZE / 2, 0, BLOCK_SIZE * MAZE_SIZE / 2);
	drawMaze(maze_matrix, MAZE_SIZE);
	*/
	// Draw maze
	glRotatef(fRotate, 0, 1.0f, 0);
	glPushMatrix();

	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-BLOCK_SIZE * MAZE_SIZE / 2, 0, BLOCK_SIZE * MAZE_SIZE / 2);
	drawMaze(maze_matrix, MAZE_SIZE);
	glPopMatrix();


	glRotatef(-90, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0, 0, 0);
	//glTranslatef(-BLOCK_SIZE * MAZE_SIZE / 2, BLOCK_SIZE * MAZE_SIZE / 2, 0);
	Draw_Scene();
	if (dx - coinx > 0.5) {
		coinx++;
	}
	if (coinx - dx > 0.5) {
		coinx--;
	}
	if (dy - coiny > 0.5) {
		coiny++;
	}
	if (coiny - dy > 0.5) {
		coiny--;
	}
	if (coin_matrix[coinx][coiny] != 0) {
		//daochu::show_model();
		score += coin_matrix[coinx][coiny];
		coin_matrix[coinx][coiny] = 0;
	}
	if (maze_matrix[ddx][ddy] == -1) {
		daochu::show_model();
	}
	//if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();

	char buffer[256];
	sprintf_s(buffer, "<Amazing Maze> Current Score:%d", score);
	glutSetWindowTitle(buffer);
}

void init()
{
	getNewMaze(10, 10, maze_matrix);
	init_tex();
	initCoin();
	//
	for (int i = 0; i < maze_matrix.size(); i++) {
		for (int j = 0; j < maze_matrix[0].size(); j++) {
			if (maze_matrix[i][j] == 0 && coin_matrix[i][j] != 0)
				cout << coin_matrix[i][j] << ".";
			else
				cout << maze_matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	int windowHandle = glutCreateWindow("Maze Game");
	ID = windowHandle;
	init();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(key);

	glutMainLoop();

	return 0;
}

// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	unsigned char	*bitmapImage;		// bitmap图像数据
	int	imageIdx = 0;		// 图像位置索引
	unsigned char	tempRGB;	// 交换变量

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

void texload(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
	unsigned char*   bitmapData;                                       // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);	//bitmap数据指针  
}

void mix_spot_tex(int i, char *filename1) {
	BITMAPINFOHEADER bitmapInfoHeader1, bitmapInfoHeader2;                                 // bitmap信息头
	unsigned char*   bitmapData1;
	unsigned char*  bitmapData2;   // 纹理数据

	bitmapData1 = LoadBitmapFile(filename1, &bitmapInfoHeader1);
	bitmapData2 = LoadBitmapFile("Spot.bmp", &bitmapInfoHeader2);
	int w = bitmapInfoHeader1.biWidth, h = bitmapInfoHeader1.biHeight;
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			bitmapData1[j*w * 3 + k * 3 + 0] =
				bitmapData1[j*w * 3 + k * 3 + 0] * omiga +
				bitmapData2[j*w * 3 + k * 3 + 0] * (1 - omiga);
			bitmapData1[j*w * 3 + k * 3 + 1] =
				bitmapData1[j*w * 3 + k * 3 + 1] * omiga +
				bitmapData2[j*w * 3 + k * 3 + 1] * (1 - omiga);
			bitmapData1[j*w * 3 + k * 3 + 2] =
				bitmapData1[j*w * 3 + k * 3 + 2] * omiga +
				bitmapData2[j*w * 3 + k * 3 + 2] * (1 - omiga);
		}
	}

	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader1.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader1.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData1);	//bitmap数据指针  
}

/* 解释函数的步骤:
   制作四个网格为为4*4大小的rgb图片数据
   对于四个网格：
   当行i第三位为1，行位于上半边（相当于奇数行格）
   当列j第三位为1，列位于左半边（相当于奇数列格）
   因此当同时为1时，位于左上
   同时为0时位于右下
   需要填充红色，即将r通道设为255
   推广到很多个网格也适用
*/
void generate() {
	//生成红黑相间的图像
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			int x = ((i & 4) ^ (j & 4)) * 255;
			image_data[i][j][0] = (GLubyte)x;
			image_data[i][j][1] = 0;
			image_data[i][j][2] = 0;
		}
	}
}

void init_tex()
{
	glGenTextures(12, texture);                                         // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texload(0, "Monet.bmp");//油画
	texload(1, "Crack.bmp");//木材
	texload(2, "Stone.bmp");//石头
	texload(3, "Grass.bmp");//草丛
	texload(4, "Box.bmp");//箱子
	//下面生成自定义纹理
	generate();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texload(6, "Spot.bmp");//光泽

	mix_spot_tex(7, "Monet.bmp");//油画混合光泽
	mix_spot_tex(8, "Crack.bmp");//木制混合光泽
	mix_spot_tex(9, "Stone.bmp");//石头混合光泽
	mix_spot_tex(10, "Grass.bmp");//草丛混合光泽
	mix_spot_tex(11, "Box.bmp");//箱子混合光泽
}

#define BITMAP_ID 0x4D42

bool newScreenShot(int width, int height, const char *filePath) {
	byte *image;
	FILE *fp;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;

	FileHeader.bfType = BITMAP_ID; // BMP identity
	FileHeader.bfOffBits = 14 + sizeof(BITMAPINFOHEADER); // data location
	FileHeader.bfReserved1 = FileHeader.bfReserved2 = 0;
	FileHeader.bfSize = height * width * 24 + FileHeader.bfOffBits; // file size
	InfoHeader.biXPelsPerMeter = InfoHeader.biYPelsPerMeter = 0; // resolution
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;
	InfoHeader.biPlanes = 1;
	InfoHeader.biCompression = 0;
	InfoHeader.biBitCount = 24;
	InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	InfoHeader.biHeight = height;
	InfoHeader.biWidth = width;
	InfoHeader.biSizeImage = height * width * 3;

	image = (byte *)malloc(sizeof(byte)*InfoHeader.biSizeImage);
	if (image == NULL) {
		free(image);
		printf("Exception: No enough space!\n");
		return false;
	}

	// get output image data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image);

	fopen_s(&fp, filePath, "wb");
	if (fp == NULL) {
		printf("Exception: Fail to open file!\n");
		return false;
	}
	fwrite(&FileHeader.bfType, 2, 1, fp);
	fwrite(&FileHeader.bfSize, 4, 1, fp);
	fwrite(&FileHeader.bfReserved1, 2, 1, fp);
	fwrite(&FileHeader.bfReserved2, 2, 1, fp);
	fwrite(&FileHeader.bfOffBits, 4, 1, fp);
	fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(image, InfoHeader.biSizeImage, 1, fp);
	free(image);
	fclose(fp);
	return true;
}