/*
Ejemplo de relleno con patron, usando una imagen
como patron de relleno.

Wilson Javier Sarmiento, 2017
Universidad Militar Nueva Granada
Bogot‡ D.C. Colombia
*/

#include <stdlib.h>

#ifdef __APPLE__
    #include <GLUT/GLUT.h>
#else
    #include "glut.h"
#endif // __APPLE__

#include <string.h>
#include <math.h>
#include "readerPNMImageFile.h"


// Definiciones globales
enum FigureType
{
	FTCircle,
	FTRectangle,
};

// Definiciones globales
enum AppState
{
    ASInit,
    ASDraw,
    ASInspect,
};


// Variables globales
int     Xmi, Ymi;             // Punto inicial de click
int     Xme, Yme;             // Punto final de click
int     Xcu, Ycu;             // Coordenadas del cursor
int     WindowW, WindowH;	// Ancho y alto de la ventana
int     ImageW,  ImageH;	// Ancho y alto de la imagen

int***	Image;		// Matrix para almacenar la imagen

bool    Pattern;	// Rellenar o no con patr—n
bool    MouseMove;	// Si el mouse este en click

FigureType  Figure;         // Tipo de figura a ser graficada
AppState    CurrentState;   // Estado actual de la aplicaci—n

// Prototipo de funciones
void Init(void);

void ReadImage(char * FileName, int*** Matriz, int* Hi, int* Wi);
void DrawHorizontalLine(int Xi, int Xf, int Y);
void DrawCircle(int Xc, int Yc, float R);
void FillCircleWithImage(int Xc, int Yc, float R,int*** Img, int ImgH,int ImgW);
void DrawRectangle(int Xi, int Yi, int Wr, int Hr);
void FillRectangle(int Xi, int Yi, int Wr, int Hr);
void FillRectangleWithImage(int Xi, int Yi, int Wr, int Hr,int*** Img, int ImgH,int ImgW);

void ConfigureWindow(int newW, int newH);
void OnResizeWindow(int width, int height);
void OnRender(void);
void OnKeyboard(unsigned char key, int x, int y);


/*
Funci—n de inicializaci—n
*/
void Init(void)
{
    WindowW = 800;
    WindowH = 600;
    
    Xmi = 0;
    Xme = 0;
    Ymi = 0;
    Yme = 0;
    Xcu = 0;
    Ycu = 0;
    
	Figure          = FTCircle;
    CurrentState    = ASInit;
	Pattern         = true;
    MouseMove       = false;

    ImageW = 0;
    ImageH = 0;
    
    Image = 0; // Puntero a NULL

    // Es muy problable que se requiera usar la ruta completa de la imagen
    // Imagenes de prueba:
    //
    // lena512color.pnm
    // flower-2845903.pnm
    // colombia-2722716.pnm
    // DonaldIcon.pnm
    ReadImage((char*)"lena512color.pnm", Image,&ImageH,&ImageW);
}

/*
Funci—n para la lectura de una im‡gen

FileName:   Nombre del archivo
Matriz:     Matriz donde quedar‡ almacenada la im‡gen
Hi:         Variable donde se retornara el alto de la im‡gen
Wi:         Variable donde se retornara el ancho de la im‡gen
*/
void ReadImage(char * FileName, int*** Matriz, int* Hi, int* Wi)
{
    // Se verfica si la matriz esta vacia
    if(Image!=0)
    {
        // Si tiene datos se libera la memoria
    }
    
    int heigth;
    int width;
    
    // Se verfica el archivo
    if (checkPNMFile(FileName, &heigth, &width))
    {
        // Si el archivo es correcto de lee la imagen
        
        // Se inicializa una matriz para almacenar la imagen
        // Image[heigth][width][3]
        Image = new int**[heigth];  // Se inicilizan las filas de la imagen
        
        int h,w;
        for (h = 0; h < heigth; h++)
        {
            Image[h] = new int*[width]; // Cada fila dibe ser de width pixeles
            
            for (w = 0; w < width; w++)
            {
                Image[h][w] = new int[3]; // Cada pixel de 3 caneles (R,G,B)
            }
        }
        
        // Se lee la imagen
        readPNMFile(FileName, Image);
        
        // Se asignan los valores de alto y ancho a los par‡metros de la funci—n
        *Hi = heigth;
        *Wi = width;
    }
}

/*
Funci—n para dibujar una l’nea horizontal.
PRE: La funci—n debe ser llamada dentro de las instrucione glBegin(GL_POINTS),

ejemplo:
 
 ...
 glBegin(GL_POINTS);
 ...
    DrawVerticalLine(10,50,5);
 ...
 glEnd(GL_POINTS)
 ...
 
Xi:    coordenada x del punto unicial de la l’nea
Xe:    coordenada x del punto unicial de la l’nea
Y:     coordenada y de la l’nea horizontal
*/
void DrawHorizontalLine(int Xi, int Xe, int Y)
{
    int x;
    for (x = Xi; x <=  Xe; x++)
    {
        glVertex2i(x,Y);
    }
}

/*
Funci—n para dibujar una l’nea vertical.
PRE: La funci—n debe ser llamada dentro de las instrucione glBegin(GL_POINTS),

ejemplo:
 
 ...
 glBegin(GL_POINTS);
 ...
    DrawVerticalLine(10,50,5);
 ...
 glEnd(GL_POINTS)
 ...
 
Yi:    coordenada y del punto unicial de la l’nea
Ye:    coordenada y del punto unicial de la l’nea
X:     coordenada x de la l’nea horizontal
*/
void DrawVerticalLine(int Yi, int Ye, int X)
{
    int y;
    for (y = Yi; y <=  Ye; y++)
    {
        glVertex2i(X,y);
    }
}

/*
 Funci—n que dibuja el cursor del rat—n
 
 Xc:    coordenada x el cursor del rat—n
 Yc:    coordenada y el cursor del rat—n
 */

void DrawCursor(int Xc, int Yc)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    
    switch(CurrentState)
    {
        case ASInspect:
            glBegin(GL_QUADS);
                glVertex2i(Xc-5,Yc-5);
                glVertex2i(Xc-5,Yc+5);
                glVertex2i(Xc+5,Yc+5);
                glVertex2i(Xc+5,Yc-5);
            glEnd();
            glBegin(GL_QUADS);
                glVertex2i(Xc-1,Yc-1);
                glVertex2i(Xc-1,Yc+1);
                glVertex2i(Xc+1,Yc+1);
                glVertex2i(Xc+1,Yc-1);
            glEnd();
            break;
        case ASDraw:
            glBegin(GL_LINES);
                glVertex2i(Xc-5,Yc);
                glVertex2i(Xc+5,Yc);
                glVertex2i(Xc,Yc-5);
                glVertex2i(Xc,Yc+5);
            glEnd();
        break;
    }
}

/*
Funci—n que dibuja el contorno de un rect‡ngulo
 
Xi:    coordenada x del punto inicial del rect‡gulo
Yi:    coordenada y del punto inicial del rect‡gulo
Rw:    ancho del rect‡ngulo
Rh:    alto del rect‡guno
*/
void DrawRectangle(int Xi, int Yi, int Rw, int Rh)
{
	int ye = Yi+Rh;
	int xe = Xi+Rw;

	glBegin(GL_POINTS);
		DrawHorizontalLine(Xi,xe,Yi);
		DrawHorizontalLine(Xi,xe,ye);
		DrawVerticalLine(Yi,ye,Xi);
		DrawVerticalLine(Yi,ye,xe);
	glEnd();
}

/*
Funci—n que dibuja rect‡ngulo relleno
 
Xi:    coordenada x del punto inicial del rect‡gulo
Yi:    coordenada y del punto inicial del rect‡gulo
Rw:    ancho del rect‡ngulo
Rh:    alto del rect‡guno
*/
void FillRectangle(int Xi, int Yi, int Rw, int Rh)
{
	int y;
	int ye = Yi+Rh;
	int xe = Xi+Rw;
	
	glBegin(GL_POINTS);		 
		 for(y=Yi;y<=ye;y++)
		 {
			DrawHorizontalLine(Xi,xe,y);
		 }
	 glEnd();	
}

/*
Funci—n que dibuja rect‡ngulo relleno usando una im‡gen como patr—n
 
Xi:    coordenada x del punto inicial del rect‡gulo
Yi:    coordenada y del punto inicial del rect‡gulo
Rw:    ancho del rect‡ngulo
Rh:    alto del rect‡guno
Img:   matrix que tiene la informaci—n de la im‡gen
ImgH:  alto de la im‡gen
ImgH:  ancho de la im‡gen
 */
void FillRectangleWithImage(int Xi, int Yi, int Rw, int Rh,int*** Img, int ImgH,int ImgW)
{
    int x,y;
    int xImg,yImg;
    int ye = Yi+Rh;
    int xe = Xi+Rw;
    
    float rf,gf,bf;
    int ri,gi,bi;
    
    float sx,sy;
    
    if(Rw<1 || Rh < 1)
        return;
    
    // Valores de escalamiento para la convertir de coordenada de Bounding Box a Image
    sx = (float)(ImgW-1)/Rw;
    sy = (float)(ImgH-1)/Rh;
    
    glBegin(GL_POINTS);
    for(y=Yi;y<=ye;y++)
    {
        for(x=Xi;x<=xe;x++)
        {
            // Se convierte la coordenada de Bounding Box a Image
            xImg = (float)(x-Xi)*sx;
            yImg = (float)(y-Yi)*sy;
            
            // Se lee el color de pixel de la im‡gen
            ri = Img[yImg][xImg][0];
            gi = Img[yImg][xImg][1];
            bi = Img[yImg][xImg][2];
            
            rf = (float)ri/255;
            gf = (float)gi/255;
            bf = (float)bi/255;
            
            // Se pinta el pixel de la figura
            glColor3f(rf,gf,bf);
            glVertex2i(x,y);
        }
    }
    glEnd();	
}

/*
Funci—n que dibuja el contorno de un c’rculo

Xc:    coordenada x del centro del c’rculo
Yc:    coordenada y del centro del c’rculo
R:     radio del c’rculo
*/
void DrawCircle(int Xc, int Yc, float R)
{
    int x, y;
    int p,dp1,dp2;
    
    y = R;
    x = 0;
    p = 1 - R;
    dp1 = 3;
    dp2 = 5 - 2*R;
    
    glBegin(GL_POINTS);
    
    //DrawCirclePointBySymmetry
    glVertex2i( x + Xc, y + Yc);
    glVertex2i( x + Xc,-y + Yc);
    glVertex2i(-x + Xc, y + Yc);
    glVertex2i(-x + Xc,-y + Yc);
    glVertex2i( y + Xc, x + Yc);
    glVertex2i( y + Xc,-x + Yc);
    glVertex2i(-y + Xc, x + Yc);
    glVertex2i(-y + Xc,-x + Yc);
    // END - DrawCirclePointBySymmetry
    
    while (x <= y)
    {
        if (p < 0)
        {
            p += dp1;
            dp1 += 2;
            dp2 += 2;
            x++;
        }
        else
        {
            p += dp2;
            dp1 += 2;
            dp2 += 4;
            x++;
            y--;
        }
        
        //DrawCirclePointBySymmetry
        glVertex2i( x + Xc, y + Yc);
        glVertex2i( x + Xc,-y + Yc);
        glVertex2i(-x + Xc, y + Yc);
        glVertex2i(-x + Xc,-y + Yc);
        glVertex2i( y + Xc, x + Yc);
        glVertex2i( y + Xc,-x + Yc);
        glVertex2i(-y + Xc, x + Yc);
        glVertex2i(-y + Xc,-x + Yc);
        // END - DrawCirclePointBySymmetry
    }
    glEnd();
}

/*
Funci—n que dibuja un c’rculo relleno

 Xc:    coordenada x del centro del c’rculo
 Yc:    coordenada y del centro del c’rculo
 R:     radio del c’rculo
*/
void FillCircle(int Xc, int Yc, float R)
{
    int x, y;
    int p,dp1,dp2;
    
    y = R;
    x = 0;
    p = 1 - R;
    dp1 = 3;
    dp2 = 5 - 2*R;
    
    glBegin(GL_POINTS);
    
    while (x <= y)
    {
        if (p < 0)
        {
            p += dp1;
            dp1 += 2;
            dp2 += 2;
            x++;
        }
        else
        {
            p += dp2;
            dp1 += 2;
            dp2 += 4;
            x++;
            y--;
        }
        
        
        //DrawCirclePointBySymmetry
		DrawHorizontalLine(-x + Xc,x + Xc, y + Yc);
		DrawHorizontalLine(-x + Xc,x + Xc,-y + Yc);
		DrawHorizontalLine(-y + Xc,y + Xc, x + Yc);
		DrawHorizontalLine(-y + Xc,y + Xc,-x + Yc);        
        // END - DrawCirclePointBySymmetry
    }

    // se deibuja la linea central del c’rculo
	DrawHorizontalLine(-R+ Xc,R + Xc, Yc);
	
    glEnd();
}

/*
Funci—n que dibuja un c’rculo relleno usando una im‡gen como patr—n
 
Xc:    coordenada x del centro del c’rculo
Yc:    coordenada y del centro del c’rculo
R:     radio del c’rculo
Img:   matrix que tiene la informaci—n de la im‡gen
ImgH:  alto de la im‡gen
ImgH:  ancho de la im‡gen
*/
void FillCircleWithImage(int Xc, int Yc, float R,int*** Img, int ImgH,int ImgW)
{
    int x, y;
    int xi, yi, xe, ye;
    int p,dp1,dp2;
    
    float sx,sy;
    
    float rf,gf,bf;
    int ri,gi,bi;
    
    int xImg,yImg;
    
    if(R==0)
        return;
    
    y = R;
    x = 0;
    p = 1 - R;
    dp1 = 3;
    dp2 = 5 - 2*R;
    
    // Calcula el Bonding Box asociado al c’rculo
    xi = Xc - R;
    yi = Yc - R;
    
    xe = Xc + R;
    ye = Yc + R;
    
    // Valores de escalamiento para la convertir de coordenada de Bounding Box a Image
    sx = (float)(ImgW-1)/(xe-xi);
    sy = (float)(ImgH-1)/(ye-yi);
    // OJO, (xe-xi)= (ye-yi) = 2+R
    
    // Variables para el relleno de las lineas horizontales que rellenan el c’rculo
    int lxi[4];
    int lxe[4];
    int ly[4];
    int lj,xj,yj;
    
    glBegin(GL_POINTS);
    
    while (x <= y)
    {
        if (p < 0)
        {
            p += dp1;
            dp1 += 2;
            dp2 += 2;
            x++;
        }
        else
        {
            p += dp2;
            dp1 += 2;
            dp2 += 4;
            x++;
            y--;
        }
        
        
        // Asigno lo valores de las lineas horizontales que rellenan el c’rculo
        lxi[0] = -x + Xc;
        lxe[0] =  x + Xc;
        ly[0]  =  y + Yc;

        lxi[1] = -x + Xc;
        lxe[1] =  x + Xc;
        ly[1]  = -y + Yc;
        
        lxi[2] = -y + Xc;
        lxe[2] =  y + Xc;
        ly[2]  =  x + Yc;
        
        lxi[3] = -y + Xc;
        lxe[3] =  y + Xc;
        ly[3]  = -x + Yc;

        for(lj=0;lj<4;lj++)
        {
            yj = ly[lj];
            for(xj=lxi[lj];xj<=lxe[lj];xj++)
            {
                // Se convierte la coordenada de Bounding Box a Image
                xImg = (float)(xj-xi)*sx;
                yImg = (float)(yj-yi)*sy;
                    
                // Se lee el color de pixel de la im‡gen
                ri = Img[yImg][xImg][0];
                gi = Img[yImg][xImg][1];
                bi = Img[yImg][xImg][2];
                    
                rf = (float)ri/255;
                gf = (float)gi/255;
                bf = (float)bi/255;
                    
                // Se pinta el pixel de la figura
                glColor3f(rf,gf,bf);
                glVertex2i(xj,yj);
            }
        }
        
    }
    
	// se deibuja la linea central del c’rculo
    
    yj = Yc;
    for(xj=-R+ Xc;xj<=R + Xc;xj++)
    {
        // Se convierte la coordenada de Bounding Box a Image
        xImg = (float)(xj-xi)*sx;
        yImg = (float)(yj-yi)*sy;
        
        // Se lee el color de pixel de la im‡gen
        ri = Img[yImg][xImg][0];
        gi = Img[yImg][xImg][1];
        bi = Img[yImg][xImg][2];
        
        rf = (float)ri/255;
        gf = (float)gi/255;
        bf = (float)bi/255;
        
        // Se pinta el pixel de la figura
        glColor3f(rf,gf,bf);
        glVertex2i(xj,yj);
    }
	
    glEnd();
}


/*
 Funci—n que verifica si un punto este dentro de un c’rculo
 
 X:    coordenada x del punto a ser evaluado
 Y:    coordenada x del punto unicial del rect‡gulo
 Xc:    coordenada x del centro del c’rculo
 Yc:    coordenada y del centro del c’rculo
 R:     radio del c’rculo
 */
bool IsInsideCircle(int X,int Y, int Xc, int Yc, float R)
{
    X = X - Xc;
    Y = Y - Yc;
    
    float d;
    
    d = sqrtf(X*X + Y*Y);
    
    return d<=R;
}

/*
 Funci—n que verifica si un punto este dentro de un rectangulo
 
 X:    coordenada x del punto a ser evaluado
 Y:    coordenada x del punto unicial del rect‡gulo
 Xi:    coordenada x del punto inicial del rect‡gulo
 Yi:    coordenada y del punto inicial del rect‡gulo
 Rw:    ancho del rect‡ngulo
 Rh:    alto del rect‡guno
*/
bool IsInsideRectangle(int X,int Y, int Xi, int Yi, int Rw, int Rh)
{
    return X>= Xi && Y>= Yi && X<= (Xi+Rw) && Y <= (Yi+Rh);
}

// Captura de clicks del mouse
void OnMouse(int Button, int State, int X, int Y)
{
    if(CurrentState == ASInit)
        CurrentState = ASDraw;
    
    if (Button == GLUT_LEFT_BUTTON)
    {
        if (State == GLUT_DOWN)
        {
            Xcu = X;
            Ycu = WindowH-Y;

			if(CurrentState == ASDraw)
			{
				Xmi = X;
				Ymi = WindowH-Y;

				Xme = Xmi;
				Yme = Ymi;
			}
            
            MouseMove = true;
        }
        else if (State == GLUT_UP)
        {
            MouseMove = false;
        }
        
        glutPostRedisplay();
    }
}

// Captura de posición del mouse durante el moviÖmiento
void OnMouseMove(int X, int Y)
{
    Xcu = X;
    Ycu = WindowH-Y;
    
    int tmp;
    switch(CurrentState)
    {
        case ASInspect:
            break;
        case ASDraw:
            Xme = Xcu;                                     
            Yme = Ycu;                        
            break;
    }
    
    glutPostRedisplay();
}

// Se configura el modo de graficación en el teclado
void OnKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 'X':
		exit(0);
		break;
	case 'f':
	case 'F':
		Pattern = !Pattern;
		break;
	case 'c':
	case 'C':
		Figure = FTCircle;
		break;
	case 'r':
	case 'R':
		Figure = FTRectangle;
		break;
	case 'd':
	case 'D':
		CurrentState = ASDraw;
		break;
	case 'i':
	case 'I':
		CurrentState = ASInspect;
		break;
	}
    
	glutPostRedisplay();
}

/*
Funci—n de configuraci—n de la ventana de graficaci—n el puerto de vista
PRE:    1) Se crea una ventana de graficaci—n del mismo tama–o
        del puerto de vista.
        2) El punto inicial del puerto de vista y de la ventana de graficaci—n
        se ubican en el origen de coordenadas.

Ww: ancho de la ventana de graficaci—n y del puerto de vista
Wh: alto de la entana de graficaci—n y del puerto de vista
 */
void ConfigureWindow(int Ww, int Wh)
{
    glViewport(0, 0, Ww, Wh);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(1, Ww, 1, Wh);
}

/*
Evento incovado cuando la ventana de la GUI cambia de ta–ano
Ver documentaci—n GLUT https://www.opengl.org/resources/libraries/glut/spec3/node48.html#SECTION00083000000000000000
*/
void OnResizeWindow(int Ww, int Wh)
{
    WindowW = Ww;
    WindowH = Wh;
    
    glutPostRedisplay();
}

/*
Evento de graficaci—n de la aplicaci—n
Ver documentaci—n GLUT https://www.opengl.org/resources/libraries/glut/spec3/node46.html#SECTION00081000000000000000
*/
void OnRender(void)
{
    glClearColor(0.5f,0.5f,0.5f,0.0f);
    
    // Se limpia la ventana
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Se configura una ventana de graficaci—n y puerto de vista
    // del mismo ta–amo de la ventana de la GUI
    ConfigureWindow(WindowW, WindowH);
    
    glColor3f(1.0f,1.0f,1.0f);
    
    int dx,dy;
    int xc,yc;
	int x0,y0;
    float r;
    	    
    dx = abs(Xme - Xmi);
    dy = abs(Yme - Ymi);

	if(Xmi<Xme)
		x0 = Xmi; 
	else
		x0 = Xme;

	if(Ymi<Yme)
		y0 = Ymi; 
	else
		y0 = Yme;
            
	if (Figure == FTCircle)
	{                                
		r = sqrtf(dx*dx + dy*dy);
		xc = Xmi; 
		yc = Ymi; ;
                                
		if (Pattern)
        {
            if(Image)
              FillCircleWithImage(xc,yc,r,Image,ImageH,ImageW);
        }
        else
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            FillCircle(xc,yc,r);
        }
		glColor3f(0.0f, 0.0f, 0.0f);
		DrawCircle(xc,yc,r);
	}
    else if (Figure == FTRectangle)
    {                 				 
        if (Pattern)
        {
            if(Image)
              FillRectangleWithImage(x0,y0,dx,dy,Image,ImageH,ImageW);
        }
        else
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            FillRectangle(x0,y0,dx,dy);
        }
		glColor3f(0.0f, 0.0f, 0.0f);
		DrawRectangle(x0,y0,dx,dy);
	}   
    
    
    if(CurrentState == ASInspect)
    {
        int bxi, bxe;
        int byi, bye;
        bool inside=false;
        
        int xImg,yImg;
        float sx,sy;
        
        float rf,gf,bf;
        int ri,gi,bi;
        
        rf=rf=rf=-1.0;
        
        if (Figure == FTCircle)
        {
            inside = IsInsideCircle(Xcu,Ycu,xc,yc,r);
            bxi = xc-r;
            bxe = xc+r;
            byi = yc-r;
            bye = yc+r;
        }
        else if (Figure == FTRectangle)
        {
            inside = IsInsideRectangle(Xcu,Ycu,x0,y0,dx,dy);
            bxi = x0;
            bxe = x0+dx;
            byi = y0;
            bye = y0+dy;
        }
        
        sx = (float)(ImageW-1)/(bxe-bxi);
        sy = (float)(ImageH-1)/(bye-byi);
        
        if(inside)
        {
            rf=1.0f;
            gf=1.0f;
            bf=1.0f;
            
            if(Image)
            {
                // Se convierte la coordenada de Bounding Box a Image
                xImg = (float)(Xcu-bxi)*sx;
                yImg = (float)(Ycu-byi)*sy;
                
                // Se lee el color de pixel de la im‡gen
                ri = Image[yImg][xImg][0];
                gi = Image[yImg][xImg][1];
                bi = Image[yImg][xImg][2];
                
                rf = (float)ri/255;
                gf = (float)gi/255;
                bf = (float)bi/255;
            }
            
            glColor3f(rf,gf,bf);
            FillRectangle(10,10,50,50);
        }
        else
        {
            glBegin(GL_LINES);
                glVertex2i(10,10);
                glVertex2i(60,60);
                glVertex2i(10,60);
                glVertex2i(60,10);
            glEnd();
        }
        
        glColor3f(1.0f,0.0f,0.0f);
        DrawRectangle(10,10,50,50);
    }
    
    if(MouseMove)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        DrawCursor(Xcu,Ycu);
    }
    
    // Se intercambia el buffer fontal y el posterior
    glutSwapBuffers();
}

// main de la aplicación
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    Init();
    
    glutCreateWindow("Mi ventana");
    glutReshapeWindow(WindowW, WindowH);
    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnResizeWindow);
    glutMouseFunc(OnMouse);
    glutMotionFunc(OnMouseMove);
	glutKeyboardFunc(OnKeyboard);
    glutMainLoop();
    
    
    return 0;
}


