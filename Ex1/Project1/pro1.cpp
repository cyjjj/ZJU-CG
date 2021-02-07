#include <glut.h>

void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); //±³¾°É«Îª°×É«
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 1.0);//°×
	glVertex2f(0, 0);
	glColor3f(1.0, 1.0, 0); //»Æ
	glVertex2f(-0.4 / 3, 0.3);
	glColor3f(1.0, 0, 0); //ºì
	glVertex2f(0, 0.5);
	glVertex2f(0.4 / 3, 0.3);
	glColor3f(1.0, 0, 1.0); //Æ·ºì
	glVertex2f(0.4, 0.3);
	glVertex2f(0.8 / 3, 0);
	glColor3f(0, 0, 1.0); //À¶
	glVertex2f(0.4, -0.3);
	glVertex2f(0.4 / 3, -0.3);
	glColor3f(0, 1.0, 1.0); //Çà
	glVertex2f(0, -0.5);
	glVertex2f(-0.4 / 3, -0.3);
	glColor3f(0, 1.0, 0); //ÂÌ
	glVertex2f(-0.4, -0.3);
	glVertex2f(-0.8 / 3, 0);
	glColor3f(1.0, 1.0, 0); //»Æ
	glVertex2f(-0.4, 0.3);
	glVertex2f(-0.4 / 3, 0.3);
	glEnd;
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}