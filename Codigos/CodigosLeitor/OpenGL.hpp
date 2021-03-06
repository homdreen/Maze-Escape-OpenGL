#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <omp.h>
#include <fstream>

double start, final;

GLfloat xRotated, yRotated, zRotated;
float angle=0.0;
/*float Ox = 7.5f, Oy = 7.5f, Oz = 15.0f;
float Cx = 7.5f, Cy = 4.0f, Cz = 7.5f;
float Ux = 0.0f, Uy = 0.0f, Uz = 90.0f;*/
/*
GLdouble Ox = 7.5f, Oy = 17.5f, Oz = 7.5f;
GLdouble Cx = 7.5f, Cy = 5.0f, Cz = -15.0f;
GLdouble Ux = 0.0f, Uy = 90.0f, Uz = 0.0f;
*/

//GLdouble Ux = 0.0f, Uy = 90.0f, Uz = 0.0f;
int **caminho;

float scale = 0.05f;
int x = 1, y = 1;
float auxOP = 6.0f;
struct Ponto{
	int x;
	int y;
	int z;
};
int rotX, rotY, obsZ;
Ponto **P1;
int altura;
int largura;

GLuint texture[1]; //the array for our texture

GLuint LoadTexture2( const char * filename, int width, int height )
{

    /* FUNCIONA APENAS COM PPM ASCII GERADO SEM COMENTARIOS, COMO NO CASO DO IMAGEMAGICK */

    GLuint texture;
    unsigned char * data;
    ifstream iFile(filename);
    char linha2[255];
    int r,g,b;

    //file = fopen( filename, "rb" );
    //if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( sizeof(char)*width * height * 3 );

    for (int i=0; i<(width*height*3); i+=3)
    {
        //fscanf(file,"%i %i %i",&r,&g,&b);
		iFile >> r;
		iFile >> g;
		iFile >> b;

        data[i]=(unsigned char)r;
        data[i+1]=(unsigned char)g;
        data[i+2]=(unsigned char)b;
		//cout << i << " " << endl;
    }

    glGenTextures( 1, &texture ); //gera a textura
    glBindTexture( GL_TEXTURE_2D, texture ); // faz o binding da texture com o array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

////

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	//cout << "a";
    free( data );
    return texture;
}

void init(void){
	// Define a cor de fundo da janela de visualização como branca
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 2
	glEnable(GL_LIGHT2);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Inicializa a variável que especifica o ângulo da projeção
	// perspectiva
	angle=50;

	// Inicializa as variáveis usadas para alterar a posição do
	// observador virtual
	rotX = 30;
	rotY = 0;
	obsZ = 180;
}

void DefineIluminacao (void)
{
        GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
        GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};          // "cor"
        GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
        GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

        // Capacidade de brilho do material
        GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
        GLint especMaterial = 60;

        // Define a refletância do material
        glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
        // Define a concentração do brilho
        glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

        // Ativa o uso da luz ambiente
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

        // Define os parâmetros da luz de número 0
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
        glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void DrawCube(void){
    xRotated = 80.0f;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//gluLookAt(	Ox, Oy, Oz,
		//		Cx, Cy, Cz,
			//	Ux, Uy, Uz);
    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	DefineIluminacao();

    glTranslatef(0.0,0.0,-10.5);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    //glRotatef(zRotated,0.0,0.0,1.0);

	//gluLookAt(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	//glColor3f(117.0f,50.0f,17.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-auxOP, -auxOP, -auxOP);
	glTexCoord2f(0.0, 1.0); glVertex3f(-auxOP, -auxOP, (205*scale)-auxOP);
	glTexCoord2f(1.0, 0.0); glVertex3f((205*scale)-auxOP, -auxOP, (205*scale)-auxOP);
	glTexCoord2f(1.0, 1.0); glVertex3f((205*scale)-auxOP, -auxOP, -auxOP);
	glEnd();
	glDisable(GL_TEXTURE_2D);



	//glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_POLYGON);

	glColor3f(255.0f,0.0f,0.0f);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexCoord2f(0.0, 0.0); glVertex3f((5*x*scale)-auxOP, -auxOP, (5*y*scale)-auxOP);
	glTexCoord2f(0.0, 1.0); glVertex3f((5*x*scale)-auxOP, -auxOP, (((5*y)+5)*scale)-auxOP);
	glTexCoord2f(1.0, 0.0); glVertex3f((((5*x)+5)*scale)-auxOP, -auxOP, (((5*y)+5)*scale)-auxOP);
	glTexCoord2f(1.0, 1.0); glVertex3f((((5*x)+5)*scale)-auxOP, -auxOP, (5*y*scale)-auxOP);
	glEnd();

    for(int i = 0; i < altura; i++){
		glBegin (GL_POLYGON);
        glColor3f(0.33f,0.66f,0.17f);

        glTexCoord2f(0.0, 0.0); glVertex3f((P1[i][0].x*scale)-auxOP, (P1[i][0].y*scale)-auxOP, (P1[i][0].z*scale)-auxOP);
        glTexCoord2f(0.0, 1.0); glVertex3f((P1[i][1].x*scale)-auxOP, (P1[i][1].y*scale)-auxOP, (P1[i][1].z*scale)-auxOP);
        glTexCoord2f(1.0, 0.0); glVertex3f((P1[i][2].x*scale)-auxOP, (P1[i][2].y*scale)-auxOP, (P1[i][2].z*scale)-auxOP);
        glTexCoord2f(1.0, 1.0); glVertex3f((P1[i][3].x*scale)-auxOP, (P1[i][3].y*scale)-auxOP, (P1[i][3].z*scale)-auxOP);
		glEnd();
    }

	for(int i = 0; i < altura; i++){

	if((P1[i][0].y == 10)&&(P1[i][1].y == 10)&&(P1[i][2].y == 10)&&(P1[i][3].y == 10)){
		glBegin (GL_POLYGON);
		glColor3f(0.33f,1.0f,0.65f);
		glTexCoord2f(0.0, 0.0); glVertex3f((P1[i][0].x*scale)-auxOP, (P1[i][0].y*scale)-auxOP, (P1[i][0].z*scale)-auxOP);
		glTexCoord2f(0.0, 1.0); glVertex3f((P1[i][1].x*scale)-auxOP, (P1[i][1].y*scale)-auxOP, (P1[i][1].z*scale)-auxOP);
		glTexCoord2f(1.0, 0.0); glVertex3f((P1[i][2].x*scale)-auxOP, (P1[i][2].y*scale)-auxOP, (P1[i][2].z*scale)-auxOP);
		glTexCoord2f(1.0, 1.0); glVertex3f((P1[i][3].x*scale)-auxOP, (P1[i][3].y*scale)-auxOP, (P1[i][3].z*scale)-auxOP);
		glEnd();
	}
}

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void reshape(int x, int y){
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();



    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}

void processSpecialKeys(int key, int xx, int yy) {
	float ponto = 0.0;

	switch (key) {
		case GLUT_KEY_LEFT:
			if(caminho[x-1][y] == 1)
			x--;
		break;
		case GLUT_KEY_RIGHT:
			if(caminho[x+1][y] == 1)
			x++;
		break;
		case GLUT_KEY_UP:
			if(caminho[x][y-1] == 1)
			y--;
		break;
		case GLUT_KEY_DOWN:
			if(caminho[x][y+1] == 1)
			y++;
		break;
	}
	if(y == 40){
		final = omp_get_wtime();
		cout << "Tempo => " << final - start << " segundos"<<endl;
		exit(1);
	}
	DrawCube();
}

void processNormalKeys(unsigned char key, int x, int y){
	switch (key) {
		case 27:
			exit(0);
		break;
	}
}

void ImprimeP1(){
	int cont = 1;
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < 4; j++){
			cout << "v " << P1[i][j].x << " " << P1[i][j].y << " " << P1[i][j].z << endl;
		}
		cout << "f " << cont << " " << cont+1 << " " << cont+2 << " " << cont+3 << endl;
		cont+=4;
	}
}

#endif
