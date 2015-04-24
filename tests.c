#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <assert.h>
#include "geometry.h"
#include "array.h"
#include <math.h>
#include "triangulation_tools.h"
#include "tiny_gc.h"
#include "tests.h"
#define NB_SUBDIV 140


half_edge test_tetra() {
    size_t n = 4;
    gl_vertex *v = GC_malloc(n * sizeof (gl_vertex));

    v[0] = *(GLvertex3d(-1.0, -1.0, -1.0));
    v[1] = *(GLvertex3d(1.0, 0.0, 0.0));
    v[2] = *(GLvertex3d(0.0, 1.0, 0.0));
    v[3] = *(GLvertex3d(0.0, 0.0, 1.0));

    return tetraedron(&v[1], &v[0], &v[2], &v[3]);
}

void copiePoint(point3d un, const point3d point) {
    un->x = point->x;
    un->y = point->y;
    un->z = point->z;

}


void checkPoint(vecteur3d un, const vecteur3d vecteur) {


    if ((abs(vecteur->z) < abs(vecteur->x)) && (abs(vecteur->z) < abs(vecteur->y))) {
        un->x = vecteur->y;
        un->y = -(vecteur->x);
        un->z = 0;

    } else if (abs(vecteur->x) < abs(vecteur->y)) {
        un->x = 0;
        un->y = vecteur->z;
        un->z = -(vecteur->y);
    } else {
        un->x = vecteur->z;
        un->y = 0;
        un->z = -(vecteur->x);
    }

}

half_edge drawn_cylindre(point3d un, point3d deux, double rayon) {
   
    point3d cercle1 = (point3d) GC_malloc(NB_SUBDIV * sizeof (point3d));
    point3d cercle2 = (point3d) GC_malloc(NB_SUBDIV * sizeof (point3d));
   
    vecteur3d i = (vecteur3d) GC_malloc(sizeof (vecteur3d));
    vecteur3d k = (vecteur3d) GC_malloc(sizeof (vecteur3d));
    vecteur3d j = (vecteur3d) GC_malloc(sizeof (vecteur3d_cell));
   
    gl_vertex ** g_cercle1 = (gl_vertex**) GC_malloc(NB_SUBDIV * sizeof (gl_vertex*));
    gl_vertex ** g_cercle2 = (gl_vertex**) GC_malloc(NB_SUBDIV * sizeof (gl_vertex*));
    
    // half_edge* tab_half_edge = (half_edge*) GC_malloc(NB_SUBDIV * sizeof (half_edge));

    vec3d(i, un, deux);

    checkPoint(j, i);
    normalize3d(j);

    vec_prod3d(k, i, j);
    normalize3d(k);

    double u = 2 * 3.14 / NB_SUBDIV;
    int n;

    if (j != NULL) {

        for (n = 0; n < NB_SUBDIV; n++) {

            copiePoint(&cercle1[n], un);
            translate3d(&cercle1[n], rayon * cos(u * n), k);
            translate3d(&cercle1[n], rayon * sin(u * n), j);
            copiePoint(&cercle2[n], &cercle1[n]);

            cercle2[n].x = cercle2[n].x + i->x;
            cercle2[n].y = cercle2[n].y + i->y;
            cercle2[n].z = cercle2[n].z + i->z;

        }
    }

    for (n = 0; n < NB_SUBDIV; n++) {
        g_cercle1[n] = GLvertex3d(cercle1[n].x, cercle1[n].y, cercle1[n].z);
        g_cercle2[n] = GLvertex3d(cercle2[n].x, cercle2[n].y, cercle2[n].z);
    }
/*
    half_edge edge;

    for (n = 1; n < NB_SUBDIV; n++) {
          edge = create_triangle(g_cercle1[n - 1], g_cercle1[n], g_cercle2[n - 1]);
          half_edge e21 = edge -> opp;
          half_edge e13 = edge -> next;
          half_edge e31 = e13 -> opp;
          half_edge e32 = e31 -> next;
          add_vertex_to_edge(e21,i);
        // tab_half_edge[n - 1] = edge;
        // edge = create_triangle(g_cercle2[n - 1], g_cercle2[n], g_cercle1[n]);
        // tab_half_edge[n] = edge;
    }
*/

    // first case
    half_edge e1 = create_triangle(g_cercle1[0], g_cercle2[0], g_cercle2[1]);
    add_vertex_to_edge(e1 -> next, g_cercle1[1]);
    half_edge e2 = e1 -> next -> next -> opp -> next;

    for (n = 1; n < NB_SUBDIV - 1; n++) {
        add_vertex_to_edge(e2, g_cercle2[n + 1]);
        add_vertex_to_edge(e2 -> next, g_cercle1[n + 1]);
        e2 = e2 -> next -> next -> opp -> next;
    }

    // last case
    add_vertex_to_edge(e2, g_cercle2[0]);
    close_triangle(e2 -> next, e1 -> opp);


    return e1 -> opp -> prev;
    
}