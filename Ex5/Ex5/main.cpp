// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdlib.h>
#include "glut.h"

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set initial scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;
bool lColor = false;// �Ƿ�ı价������ɫ

int wHeight = 0;
int wWidth = 0;

//������
GLfloat light_x = 0.0f;
GLfloat light_y = 0.0f;
GLfloat light_z = 0.0f;
GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 }; // ��������ɫ����ɫ

//�۹��
GLfloat spot_x = 0.0f;
GLfloat spot_y = 0.0f;
GLfloat spot_z = 0.0f;
GLfloat spot_angle = 5.0f;//�۹�ƽǶ�

void Draw_Leg();

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	// colors used (mat_diffuse)
	GLfloat color1[] = { 0.85f,0.65f,0.2f,1.0f }; // ���ɫ
	GLfloat color2[] = { 1.0f,0.0f,0.0f };
	GLfloat color3[] = { 0.0f,1.0f,0.0f };
	GLfloat color4[] = { 1.0f,1.0f,0.0f };
	GLfloat color5[] = { 0.0f,1.0f,1.0f };
	GLfloat color6[] = { 0.0f,0.0f,1.0f };

	//float mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };
	float mat_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	// ���
	// ͨ�����ò���ʹ�ò��Ϊ���ɫ
	// �޸Ĳ���ľ��淴��ϵ����ʹ֮�Թ�Դ���ָ߹�
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color1); // �����䣬���Ϊ���ɫ
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); // ���淴��
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50); // ���ȣ��޸Ĳ���ľ��淴��ϵ��
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	// ����
	// ͨ�����ò���ʹ������������ȵ���ɫ�ֱ�Ϊ��(1, 0, 0), (0, 1, 0), (1, 1, 0), (0, 1, 1), (0, 0, 1)
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color2); // ������
	glMaterialfv(GL_FRONT, GL_SPECULAR, color2); // ���淴��
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color3); // ������
	glMaterialfv(GL_FRONT, GL_SPECULAR, color3); // ���淴��
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color4); // ������
	glMaterialfv(GL_FRONT, GL_SPECULAR, color4); // ���淴��
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color5); // ������
	glMaterialfv(GL_FRONT, GL_SPECULAR, color5); // ���淴��
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, color6); // ������
	glMaterialfv(GL_FRONT, GL_SPECULAR, color6); // ���淴��
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
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
	
	// �ƶ������еĹ�Դ
	case 'j': { // ����������
		light_x  -= 0.2f;
		break;
			}
	case 'l': { // ����������
		light_x += 0.2f;
		break;
			}
	case 'i': { // ����������
		light_y += 0.2f;
		break;
			}
	case 'k': { // ����������
		light_y -= 0.2f;
		break;
			}
	case 'n': { // ������ǰ��
		light_z += 0.2f;
		break;
			}
	case 'm': { // ���������
		light_z -= 0.2f;
		break;
			}
	
	// �ı��Դ����ɫ����������ɫ���л�)
	case 'u': {
		lColor = !lColor;
		break;
			}

	// �����۹��Դ�ĳ���
	case 'f': { // �۹������
		spot_x -= 0.05f;
		break;
			}
	case 'h': { // �۹������
		spot_x += 0.05f;
		break;
			}
	case 't': { // �۹������
		spot_y -= 0.05f;
		break;
			}
	case 'g': { // �۹������
		spot_y += 0.05f;
		break;
			}
	case 'v': { // �۹��ǰ��
		spot_z += 0.05f;
		break;
			}
	case 'b': { // �۹�ƺ���
		spot_z -= 0.05f;
		break;
			}
	
	// �����۹��Դ������Ƕ�
	case 'r': { // �Ƕȱ��
		if (spot_angle <= 89.0f)
			spot_angle += 0.2f;
		break;
			}
	case 'y': { // �Ƕȱ�С
		if (spot_angle >= 1.0f)
			spot_angle -= 0.2f;
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
		0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST); // ������Ȳ���
	glEnable(GL_LIGHTING); // ��������ģʽ

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; // ��������ɫΪ��ɫ
	GLfloat light_pos[] = { 5 + light_x,5 + light_y,5 + light_z,1 }; // �������Դλ��												 
	GLfloat spot_pos[] = { 0.0,5.0,0.0,1.0 }; // �۹��λ��
	GLfloat spot_dir[] = { 0.0f + spot_x,-1.0f + spot_y,0.0f + spot_z }; // �۹�����䷽��
													   
	if (lColor) {// �Ƿ�ı������ɫ
		color[0] = 0.0f;
		color[2] = 0.0f; //
	}
	else {
		color[0] = 1.0f;
		color[2] = 1.0f; // ��ɫ
	}

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); // ��0�Ź�Դ��λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, white); // ���淴��
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white); //������  
	glLightfv(GL_LIGHT0, GL_AMBIENT, color); //���õ�0�Ź�Դ��η����Ĺ�����ɫ����������ɫ��
	glEnable(GL_LIGHT0); // ������0�Ź�Դ

	glLightfv(GL_LIGHT1, GL_POSITION, spot_pos); // ��1�Ź�Դ���۹�ƣ���λ��
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle); //�ü��Ƕ�
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir); //��Դ����
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.); //�ۼ���
	glLightfv(GL_LIGHT1, GL_SPECULAR, white); //���þ����ɷ�
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white); //���������ɷ�
	glLightfv(GL_LIGHT1, GL_AMBIENT, color); //���û�����ɷ�
	glEnable(GL_LIGHT1); // ������1�Ź�Դ

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

	glutMainLoop();
	return 0;
}


