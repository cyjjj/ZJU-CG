#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "glew.h"
#include "glut.h"
#include "GenerateMaze.h"

//#include "stanford_bunny.h"
using namespace std;

#define BITMAP_ID 0x4D42
#define Height 16
#define Width 16
#define omiga 0.8
#define MAX_COIN 100

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

// �����ʾ�����飬������������ı�ʾ��
// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
//��������ĺ���
void texload(int i, char *filename);
void mix_spot_tex(int i, char *filename1);//�������
//��ʼ������
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

float eye[] = { place/ bei, 4, 4- (place-1)/ bei };
float center[] = { place/ bei, 0, -(place-1)/ bei };
float tplace[] = { place,place-1, 1.3 };
float freecenter[] = { 0, 0, 0 };
float freeeyee[] = { 0, 0, 0 };

int mark = 0;
float tRotate = 0.0f;
float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set initial scale value to 1.0f

bool bPersp = true;
bool bAnim = false;
bool bWire = false;
bool tAnim = false;
bool freeeye = false;

int wHeight = 0;
int wWidth = 0;

int color_type = 0;//0-white, 1-green, 2-red, 3-blue
GLuint texture[12];//�����ʶ��
GLubyte image_data[Height][Width][3];  // generate �� �����ͼƬ����
int ground_tex = 1;//�ذ��������ʼ��Ϊľͷ������
int wall_tex = 0;//ǽ��������ʼ��Ϊ�ͻ�������
bool wall_mix = false;//ǽ�����Ƿ��Ϲ���
bool ground_mix = false;//�ذ������Ƿ��Ϲ���
bool edit_Map = false;//�Ƿ�����ͼ�༭ģʽ
bool global_eye = false;//�Ƿ����ȫ���ӽ�ģʽ

GLfloat light_pos[] = { -20,15,0,1 };//��Դλ��
GLfloat light_angle = 45.0f;//��Դ�Ƕ�
GLfloat light_Dir[] = { 1.0f, -1.0f,0.0f };//��Դ���䷽������
GLfloat light_color[4][3] = { { 1, 1, 1 }, //white
							{ 0, 1, 0 }, //green
							{ 1, 0, 0 }, //red
							{ 0, 0, 1 } }; //blue

//����Ķ���
GLfloat Tex_vertex[4][2] = { {1.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f},{0.0f,1.0f} };

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
	glScalef(0.5, 0.5, 0.5);
	glutSolidCube(1.0);
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
	//GLfloat coin_color[] = { 0.85f,0.65f,0.2f,1.0f }; // ���ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_color[0]); // ������
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_color[0]); // ���淴��
	for (int i = 0; i < MAZE_SIZE; i++)
		for (int j = 0; j < MAZE_SIZE; j++) {
			if (coin_matrix[i][j]) {
				if (maze_matrix[i][j]) {
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
}

void drawMaze(vector<vector<int>>& matrix, int matrix_size)
{
	glEnable(GL_NORMALIZE);
	//wall
	//������ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_color[0]); // ������
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_color[0]); // ���淴��
	glMateriali(GL_FRONT, GL_SHININESS, 50);//���þ���ָ��
	//ӳ������
	glEnable(GL_TEXTURE_2D);
	if(wall_mix && wall_tex + 6 < 12)
		glBindTexture(GL_TEXTURE_2D, texture[wall_tex + 7]);  //ѡ������texture[wall_tex]
	else
		glBindTexture(GL_TEXTURE_2D, texture[wall_tex]);  //ѡ������texture[wall_tex]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//���������ܹ���Ӱ��
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++) {
			if (matrix[i][j] == 0) continue;
			glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, 0, -j * BLOCK_SIZE);
			glTranslatef(BLOCK_SIZE / 2, BLOCK_HEIGHT / 2, -BLOCK_SIZE / 2);
			glScalef(BLOCK_SIZE, BLOCK_HEIGHT, BLOCK_SIZE);
			drawCube(1.0);
			glPopMatrix();
		}
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[wall_tex]

	//ground
	//ӳ������
	glEnable(GL_TEXTURE_2D);
	if (ground_mix && ground_tex + 6 < 12)
		glBindTexture(GL_TEXTURE_2D, texture[ground_tex + 7]);  //ѡ������texture[ground_tex]
	else
		glBindTexture(GL_TEXTURE_2D, texture[ground_tex]);  //ѡ������texture[ground_tex]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//���������ܹ���Ӱ��
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++) {
			glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, 0, -j * BLOCK_SIZE);
			glTranslatef(BLOCK_SIZE / 2, BLOCK_HEIGHT / 2 - 0.5, -BLOCK_SIZE / 2);
			glScalef(BLOCK_SIZE, 1.0, BLOCK_SIZE);
			drawCube(1.0);
			glPopMatrix();
		}
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[ground_tex]
	//glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	drawCoins(GL_RENDER);    //draw coin
	glPopMatrix();
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
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
		newScreenShot(700, 700, screenShotPath.c_str());
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
				eye[2] += 0.08f;
				center[2] += 0.08f;
			}
			else if (mark == 3) {
				eye[0] -= 0.08f;
				center[0] -= 0.08f;
			}
		}
		if (!freeeye) {
			if (mark == 0) {
				if (maze_matrix[ddx][ddy] != 1) {
					
					if (dy - ddy < 0.2) {
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
					else {
						ddy++;
						sstop = 0;
					}
					
				}
				

				
			}
			else if (mark == 1) {
				if (maze_matrix[ddx][ddy] != 1) {
					if (dx - ddx < 0.2) {
						dx = dx + 2 * yidong;
						tplace[0] += 2 * yidong;
						eye[0] += yidong;
						center[0] += yidong;
					}
					else {
						ddx++;
						sstop = 0;
					}
				}
				
			}
			else if (mark == 2) {
				if (maze_matrix[ddx][ddy] != 1) {
					if (ddy - dy < 0.2) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						ddy--;
						sstop = 0;
					}
					
				}
				
			}
			else if (mark == 3) {
				if (maze_matrix[ddx][ddy] != 1) {
					if (ddx - dx < 0.2) {
						dx = dx - 2 * yidong;
						tplace[0] -= 2 * yidong;
						eye[0] -= yidong;
						center[0] -= yidong;
					}
					else {
						ddx--;
						sstop = 0;
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
				if (maze_matrix[ddx][ddy] != 1) {

					if (ddy - dy < 0.2) {
						dy = dy - 2 * yidong;
						tplace[1] -= 2 * yidong;
						eye[2] += yidong;
						center[2] += yidong;
					}
					else {
						ddy--;
						sstop = 1;
					}

				}
				
			}
			else if (mark == 1) {
				if (maze_matrix[ddx][ddy] != 1) {

					if (ddx - dx < 0.2) {
						dx = dx - 2 * yidong;
						tplace[0] -= 2 * yidong;
						eye[0] -= yidong;
						center[0] -= yidong;
					}
					else {
						ddx--;
						sstop = 1;
					}

				}
				
			}
			else if (mark == 2) {
				if (maze_matrix[ddx][ddy] != 1) {

					if (dy - ddy < 0.2) {
						dy = dy + 2 * yidong;
						tplace[1] += 2 * yidong;
						eye[2] -= yidong;
						center[2] -= yidong;
					}
					else {
						ddy++;
						sstop = 1;
					}

				}
				
			}
			else if (mark == 3) {
				if (maze_matrix[ddx][ddy] != 1) {
					if (dx - ddx < 0.2) {
						dx = dx + 2 * yidong;
						tplace[0] += 2 * yidong;
						eye[0] += yidong;
						center[0] += yidong;
					}
					else {
						ddx++;
						sstop = 1;
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
	case 'e': {edit_Map = !edit_Map; break; }//�������༭ģʽ

	case 'a': { // ��������
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'd': { // ��������
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'w': { // ��������
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': { // ��������
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': { // ����ǰ��
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': { // �������
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	}
	*/
	if (edit_Map) {//��ͼ�༭ģʽ��������༭�͹��ձ༭
		switch (k)
		{
		case 'u': {//ȫ���ӽ�
			global_eye = !global_eye;
			break;
		}
		case 'm': {//��Ϲ���
			ground_mix = !ground_mix;
			wall_mix = !wall_mix;
			break;
		}
		case 'h': {//�л�ǽ������
			if (wall_tex != 6) {
				wall_tex++;
			}
			else {
				wall_tex = 0;
			}
			break;
		}
		case 'g': {//�л��ذ�����
			if (ground_tex != 6) {
				ground_tex++;
			}
			else {
				ground_tex = 0;
			}
			break;
		}// �ü������������仯�����㣩
		//��0��1�� 1��1
		case '1': { //0, 1ѭ������
			if (Tex_vertex[3][1] > 0)
				Tex_vertex[3][1] -= 0.2f;
			else
				Tex_vertex[3][1] = 1.0f;
			break;
		}
		case '2': { //0, 1ѭ������
			if (Tex_vertex[3][0] < 1)
				Tex_vertex[3][0] += 0.2f;
			else
				Tex_vertex[3][0] = 0.0f;
			break;
		}
		case '3': { //1��1ѭ������
			if (Tex_vertex[0][1] > 0)
				Tex_vertex[0][1] -= 0.2f;
			else
				Tex_vertex[0][1] = 1.0f;
			break;
		}
		case '4': { //1��1ѭ������
			if (Tex_vertex[0][0] > 0)
				Tex_vertex[0][0] -= 0.2f;
			else
				Tex_vertex[0][0] = 1.0f;
			break;
		}
				  //��1��0�� 0��0
		case '6': { //1, 0ѭ������
			if (Tex_vertex[1][1] < 1)
				Tex_vertex[1][1] += 0.2f;
			else
				Tex_vertex[1][1] = 0.0f;
			break;
		}
		case '7': { //1��0ѭ������
			if (Tex_vertex[1][0] > 0)
				Tex_vertex[1][0] -= 0.2f;
			else
				Tex_vertex[1][0] = 1.0f;
			break;
		}
		case '8': { //0��0ѭ������
			if (Tex_vertex[2][1] < 1)
				Tex_vertex[2][1] += 0.2f;
			else
				Tex_vertex[2][1] = 0.0f;
			break;
		}
		case '9': { //0��0ѭ������
			if (Tex_vertex[2][0] < 1)
				Tex_vertex[2][0] += 0.2f;
			else
				Tex_vertex[2][0] = 0.0f;
			break;
		}
		case '5': {//��ԭ
			for (int i = 0; i < 4; i++)
			{
				Tex_vertex[i][0] = (i < 2) ? 1.0f : 0.0f;//ǰ2��
				Tex_vertex[i][1] = (i == 0 || i == 3) ? 1.0f : 0.0f;//��һ�������һ��
			}
			break;
		}
		case 'j': {//��������
			light_pos[0] += 0.2f;
			break;
		}
		case 'l': {//��������
			light_pos[0] -= 0.2f;
			break;
		}
		case 'i': {//��Դ��
			light_pos[1] += 0.2f;
			break;
		}
		case 'k': {//��Դ��
			light_pos[1] -= 0.2f;
			break;
		}
		case '\'': {//��Դǰ
			light_pos[2] += 0.2f;
			break;
		}
		case ';': {//��Դ��
			light_pos[2] -= 0.2f;
			break;
		}
		case ',': {//�Ƕȱ��
			if (light_angle <= 89.0f)
				light_angle += 0.2f;
			break;
		}
		case '.': {//�Ƕȱ�С
			if (light_angle >= 1.0f)
				light_angle -= 0.2f;
			break;
		}
		case 't': {//��Դ��ɫ�л�
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
			0, 1, 0);				// ������0��0��0�����ӵ������������ƶ���Y������
	}
	else {
		gluLookAt(eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������
	}

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST); // ������Ȳ���
	glEnable(GL_LIGHTING); // ��������ģʽ

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color[color_type]);   //���û�����ɷ�
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color[color_type]);  //���þ����ɷ�
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[color_type]);   //���������ɷ�
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_angle);           //�ü��Ƕ�
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_Dir);          //��Դ����
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.);                    //�ۼ���
	glEnable(GL_LIGHT0);//�򿪾۹��

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
		score += coin_matrix[coinx][coiny];
		coin_matrix[coinx][coiny] = 0;
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
			if(maze_matrix[i][j] == 0 && coin_matrix[i][j] != 0)
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

	init();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(key);

	glutMainLoop();

	return 0;
}

// �����ʾ�����飬������������ı�ʾ��
// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char	*bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}

void texload(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char*   bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��  
}

void mix_spot_tex(int i, char *filename1) {
	BITMAPINFOHEADER bitmapInfoHeader1, bitmapInfoHeader2;                                 // bitmap��Ϣͷ
	unsigned char*   bitmapData1;
	unsigned char*  bitmapData2;   // ��������

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
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader1.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader1.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData1);	//bitmap����ָ��  
}

/* ���ͺ����Ĳ���:
   �����ĸ�����ΪΪ4*4��С��rgbͼƬ����
   �����ĸ�����
   ����i����λΪ1����λ���ϰ�ߣ��൱�������и�
   ����j����λΪ1����λ�����ߣ��൱�������и�
   ��˵�ͬʱΪ1ʱ��λ������
   ͬʱΪ0ʱλ������
   ��Ҫ����ɫ������rͨ����Ϊ255
   �ƹ㵽�ܶ������Ҳ����
*/
void generate() {
	//���ɺ������ͼ��
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
	glGenTextures(12, texture);                                         // ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
	texload(0, "Monet.bmp");//�ͻ�
	texload(1, "Crack.bmp");//ľ��
	texload(2, "Stone.bmp");//ʯͷ
	texload(3, "Grass.bmp");//�ݴ�
	texload(4, "Box.bmp");//����
	//���������Զ�������
	generate();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�������ش洢ģʽ��������ȡ��ͼ�����ݵ��ж��뷽ʽ.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	texload(6, "Spot.bmp");//����

	mix_spot_tex(7, "Monet.bmp");//�ͻ���Ϲ���
	mix_spot_tex(8, "Crack.bmp");//ľ�ƻ�Ϲ���
	mix_spot_tex(9, "Stone.bmp");//ʯͷ��Ϲ���
	mix_spot_tex(10, "Grass.bmp");//�ݴԻ�Ϲ���
	mix_spot_tex(11, "Box.bmp");//���ӻ�Ϲ���
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
	FileHeader.bfSize = height*width * 24 + FileHeader.bfOffBits; // file size
	InfoHeader.biXPelsPerMeter = InfoHeader.biYPelsPerMeter = 0; // resolution
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;
	InfoHeader.biPlanes = 1;
	InfoHeader.biCompression = 0;
	InfoHeader.biBitCount = 24;
	InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	InfoHeader.biHeight = height;
	InfoHeader.biWidth = width;
	InfoHeader.biSizeImage = height*width * 3;

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