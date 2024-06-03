#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// WADS to move player.

// Ray caster code ......

#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174533 // one degree in radians

//-----------------------------MAP----------------------------------------------

float px, py, pdx, pdy, pa;

int mapX = 8,
    mapY = 8, mapS = 64;

int map[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,

};

float dist(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}
void drawRays2D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, disT;

    ra = pa - DR * 30;
    if (ra < 0)
    {
        ra += 2 * PI;
    }
    if (ra > 2 * PI)
    {
        ra -= 2 * PI;
    }

    for (r = 0; r < 60; r++)
    {
        // Check horizontal line
        dof = 0;
        float distH = 1000000, hx = px, hy = py;
        float aTan = -1 / tan(ra);
        if (ra > PI)
        {
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo -= yo * aTan;
        } // looking up
        if (ra < PI)
        {
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        } // looking down

        if (ra == 0 || ra == PI)
        {
            rx = px;
            ry = py;
            dof = 8;
        } // looking straight left or right

        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                hx = rx;
                hy = ry;
                distH = dist(px, py, hx, hy, ra);
                dof = 8;
            } // hit wall
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            } // next
        }

        // Check vertical line
        dof = 0;
        float distV = 1000000, vx = px, vy = py;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3)
        {
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
        } // looking left
        if (ra < P2 || ra > P3)
        {
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
        } // looking right

        if (ra == 0 || ra == PI)
        {
            rx = px;
            ry = py;
            dof = 8;
        } // looking straight up or down

        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                vx = rx;
                vy = ry;
                distV = dist(px, py, vx, vy, ra);
                dof = 8;
            } // hit wall
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            } // next
        }
        if (distV < distH)
        {
            rx = vx;
            ry = vy;
            disT = distV;
            glColor3f(0.9, 0, 0); // shading effect
        } // vertical wall hit
        if (distV > distH)
        {
            rx = hx;
            ry = hy;
            disT = distH;
            glColor3f(0.7, 0, 0); // shading effect
        } // horizontal wall hit

        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();

        // -- Draw 3d walls ----
        float ca = pa - ra;
        if (ca < 0)
        {
            ca += 2 * PI;
        }
        if (ca > 2 * PI)
        {
            ca -= 2 * PI;
        }
        disT = disT * cos(ca); // to overcome the curved effect because rays at angle are longer therefore height is longer, but it should be the same.
        float lineH = (mapS * 320) / disT;
        if (lineH > 320)
        {
            lineH = 320;
        } // line height

        float lineO = 160 - lineH / 2; // line offset
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r * 8 + 530, lineO);
        glVertex2i(r * 8 + 530, lineH + lineO);
        glEnd();
        ra += DR;
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        if (ra > 2 * PI)
        {
            ra -= 2 * PI;
        }
    }
}
void drawMap2d()
{
    int x, y, x0, y0;

    for (y = 0; y < mapY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y * mapY + x] == 1)
            {
                glColor3f(1, 1, 1);
            }
            else
            {
                glColor3f(0, 0, 0);
            }
            x0 = x * mapS, y0 = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(x0 + 1, y0 + 1);
            glVertex2i(x0 + 1, y0 + mapS - 1);
            glVertex2i(x0 + mapS - 1, y0 + mapS - 1);
            glVertex2i(x0 + mapS - 1, y0 + 1);
            glEnd();
        }
    }
}

void buttons(unsigned char key, int x, int u)
{
    if (key == 'a')
    {
        pa -= 0.1;
        if (pa < 0)
        {
            pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'd')
    {
        pa += 0.1;
        if (pa > 2 * PI)
        {
            pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'w')
    {
        px += pdx;
        py += pdy;
    }
    if (key == 's')
    {
        px -= pdx;
        py -= pdy;
    }
    glutPostRedisplay();
}

void drawPlayer()
{

    // point object
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();

    // Object line
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
    glEnd();
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    px = 300;
    py = 300;
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2d();

    drawRays2D();
    drawPlayer();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("3d game");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutMainLoop();
}
