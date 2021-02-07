// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "glut.h"

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

//纹理
#define BITMAP_ID 0x4D42
#define Height 32
#define Width 32
#define omiga 0.5

GLuint texture[4];//纹理标识符
GLubyte tex[Height][Width][3];  // 生成的红黑棋盘格纹理的图片数据
int tea_tex = 0; //茶壶的纹理，初始化为Crack.bmp的纹理
bool mix = false; //是否混合两纹理

//读纹理函数
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
//加载纹理的函数
void texload(int i, char *filename);
//定义纹理的函数
void init();

//画Cube并处理纹理顶点
void Draw_Cube(GLfloat size)
{
	//法向
	static GLfloat n[6][3] =
	{
		{ -1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, -1.0 }
	};
	//面的四个顶点对应坐标数组下标
	static GLint faces[6][4] =
	{
		{ 0, 1, 2, 3 },
		{ 3, 2, 6, 7 },
		{ 7, 6, 5, 4 },
		{ 4, 5, 1, 0 },
		{ 5, 6, 2, 1 },
		{ 7, 4, 0, 3 }
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(GL_QUADS);
		glNormal3fv(&n[i][0]);
		glTexCoord2i(1, 1); glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2i(1, 0); glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2i(0, 0); glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2i(0, 1); glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	Draw_Cube(1.0);
}

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	//GLint x0 = 1, x1 = 1, x2 = 1, x3 = 1, y0 = 1, y1 = 1, y2 = 1, y3 = 1;

	//茶壶
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[tea_tex]);  //选择纹理texture[0]
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理受光照影响
	glutSolidTeapot(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//关闭纹理texture[0]

	//桌子	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //选择纹理texture[1]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//设置纹理不受光照影响
	if (mix) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
	}

	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	Draw_Cube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);	//关闭纹理texture[1]
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
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
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'd': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	case 't': {
		tea_tex = (tea_tex == 0) ? 3 : 0; // 0:Monet.bmp, 3:black-red chessbord
		break;
			  }
	case 'm': {
		mix = !mix; // mix texture
		break;
			   }
	}

	updateView(wHeight, wWidth);
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	init();//初始化纹理

	glutMainLoop();
	return 0;
}

//纹理
//------------------------------------------------------------------------------------
//读纹理函数
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

//加载纹理的函数
void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
	unsigned char*   bitmapData;                                       // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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

//混合纹理
void mix_tex(int i, char *filename1, char *filename2) {
	BITMAPINFOHEADER bitmapInfoHeader1, bitmapInfoHeader2;  // bitmap信息头
	unsigned char*   bitmapData1;
	unsigned char*  bitmapData2;   // 纹理数据

	bitmapData1 = LoadBitmapFile(filename1, &bitmapInfoHeader1);
	bitmapData2 = LoadBitmapFile(filename2, &bitmapInfoHeader2);
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
	//生成红黑相间的棋盘格图像
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			tex[i][j][0] = (GLubyte)((i & 4) ^ (j & 4)) * 255;;
			tex[i][j][1] = 0;
			tex[i][j][2] = 0;
		}
	}
}

//定义纹理的函数
void init()
{
	glGenTextures(4, texture);  // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texload(0, "Monet.bmp");
	texload(1, "Crack.bmp");
	mix_tex(2, "Crack.bmp", "Spot.bmp");
	//下面生成自定义纹理
	generate();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}