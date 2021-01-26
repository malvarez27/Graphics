//
//  assignment1.cpp
//  assignment1
//  Name: Alvarez, Marissa
//  Copyright © 2018 Marissa Alvarez. All rights reserved.
/* Project Summary: The approach I took for this assignment was the iterative approach. It was definetly easier to visualize once I decided to take that approach. Also, I created two functions one that would generate midpoints and generate points. In addition, I made sure to put the control points, mid points, and mid-mid points in a vector then I would interleave each of the mid points and mid-mid points and use the first control point then interleave midpoints and put the last control points when generating my line. When drawing the curve I iterarate to each of the points. The way I chose my points, I inputted the image in Desmos and created the points with the help of a grid. Lastly, after picking all of my points I call all of the draw curve functions and it prints out the image.
*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <vector>
#include <iostream>
using namespace std;
class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};
Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y; }
void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}
vector<Vertex> generate_midpoints(vector<Vertex> control_points) {
    vector<Vertex> midpoints;
    // Generate points for a given Bezier curve iteration
    for (int p = 0; p < control_points.size()-1; p++) {
        Vertex v_0 = control_points[p];
        Vertex v_1 = control_points[p+1];
        // Midpoint along the line from v_0 and v_1
        GLfloat y1 = (v_0.get_y() + v_1.get_y()) / 2;
        GLfloat x1 = (v_0.get_x() + v_1.get_x()) / 2;
        // push_back simply adds a new element to the back of your vector (list)
        midpoints.push_back(Vertex(x1, y1));
    }
    return midpoints;
}
vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    vector<Vertex> midpoints = generate_midpoints(control_points);
    vector<Vertex> midpoints2 = generate_midpoints(midpoints);

    points.push_back(control_points[0]);
    for (int i = 0; i < midpoints.size()-1; i++){
        points.push_back(midpoints[i]);
        points.push_back(midpoints2[i]);
    }
    points.push_back(midpoints[midpoints.size()-1]);
    points.push_back(control_points[control_points.size()-1]);

    return points;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {
    glPointSize(2.0f);
    // Draw a Bezier curve based on the given control points
    for (int i = 0; i < n_iter; i++) {
        control_points = generate_points(control_points);
    }

    glBegin(GL_POINTS);
    for (int i = 0; i< control_points.size()-1; i++){
        glVertex2f(control_points[i].get_x(), control_points[i].get_y());
    }
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for(int i = 0; i< control_points.size()-2; i++){
        glVertex2f(control_points[i].get_x(), control_points[i].get_y());
        glVertex2f(control_points[i+1].get_x(), control_points[i+1].get_y());
    }
    glEnd();
}
//    glLineWidth(2.0f);
//    glBegin(GL_LINES);
//    for(int i=0; i< generated_points.size()-1; i++){
//        glVertex2f(generated_points[i].get_x(), generated_points[i].get_y());
//        glVertex2f(generated_points[i+1].get_x(), generated_points[i+1].get_y());
//    }
//      glEnd();

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);

        vector<Vertex> head_r;
        head_r.push_back(Vertex(-0.5f, 0.0f));
        head_r.push_back(Vertex(-0.5f, 0.1f));
        head_r.push_back(Vertex(-0.5f, 0.5f));
        head_r.push_back(Vertex(-0.3f, 0.9f));
        head_r.push_back(Vertex(0.0f, 1.0f));
        head_r.push_back(Vertex(0.2f, 0.94f));
        head_r.push_back(Vertex(0.4f, 0.75f));
        head_r.push_back(Vertex(0.5f, 0.5f));
        head_r.push_back(Vertex(0.45f, 0.2f));
        head_r.push_back(Vertex(0.4f, 0.0f));
        head_r.push_back(Vertex(0.4f, -0.2f));
        head_r.push_back(Vertex(0.3f, -0.4f));
        head_r.push_back(Vertex(0.2f, -0.5f));
        head_r.push_back(Vertex(0.0f, -0.6f));
        head_r.push_back(Vertex(-0.2f, -0.6f));
        head_r.push_back(Vertex(-0.4f, -0.4f));
        head_r.push_back(Vertex(-0.45f, -0.2f));
        head_r.push_back(Vertex(-0.5f, 0.0f));

        vector<Vertex> hair_r;
        hair_r.push_back(Vertex(-0.5f, 0.2f));
        hair_r.push_back(Vertex(-0.45f, 0.4f));
        hair_r.push_back(Vertex(-0.3f, 0.7f));
        hair_r.push_back(Vertex(0.0f, 0.64f));
        hair_r.push_back(Vertex(0.2f, 0.5f));
        hair_r.push_back(Vertex(0.3f, 0.55f));
        hair_r.push_back(Vertex(0.4f, 0.2f));
        hair_r.push_back(Vertex(0.4f, 0.1f));
        hair_r.push_back(Vertex(0.4f, 0.0f));

        vector<Vertex> eye_r;
        eye_r.push_back(Vertex(0.04f, 0.2f));
        eye_r.push_back(Vertex(0.1f, 0.25f));
        eye_r.push_back(Vertex(0.15f, 0.25f));
        eye_r.push_back(Vertex(0.2f, 0.23f));
        eye_r.push_back(Vertex(0.23f, 0.2f));
        eye_r.push_back(Vertex(0.15f, 0.19f));
        eye_r.push_back(Vertex(0.04f, 0.2f));

    vector<Vertex> eye_l;
    eye_l.push_back(Vertex(-0.37f, 0.22f));
    eye_l.push_back(Vertex(-0.3f, 0.25f));
    eye_l.push_back(Vertex(-0.25f, 0.25f));
    eye_l.push_back(Vertex(-0.2f, 0.2f));
    eye_l.push_back(Vertex(-0.3f, 0.2f));
    eye_l.push_back(Vertex(-0.37f, 0.22f));

    vector<Vertex> eyebrow_l;
    eyebrow_l.push_back(Vertex(-0.4f, 0.3f));
    eyebrow_l.push_back(Vertex(-0.3f, 0.31f));
    eyebrow_l.push_back(Vertex(-0.2f, 0.3f));

    vector<Vertex> eyebrow_r;
    eyebrow_r.push_back(Vertex(0.04f, 0.3f));
    eyebrow_r.push_back(Vertex(0.1f, 0.32f));
    eyebrow_r.push_back(Vertex(0.2f, 0.3f));
    eyebrow_r.push_back(Vertex(0.3f, 0.28f));

    vector<Vertex> nose_r;
    nose_r.push_back(Vertex(-0.22f, -0.0f));
    nose_r.push_back(Vertex(-0.1f, -0.08f));
    nose_r.push_back(Vertex(0.04f, 0.0f));

    vector<Vertex> mouth_r;
    mouth_r.push_back(Vertex(-0.3f, -0.2f));
    mouth_r.push_back(Vertex(-0.2f, -0.16f));
    mouth_r.push_back(Vertex(-0.1f, -0.16f));
    mouth_r.push_back(Vertex(0.0f, -0.16f));
    mouth_r.push_back(Vertex(0.12f, -0.19f));
    mouth_r.push_back(Vertex(0.0f, -0.3f));
    mouth_r.push_back(Vertex(-0.1f, -0.33f));
    mouth_r.push_back(Vertex(-0.2f, -0.3f));
    mouth_r.push_back(Vertex(-0.3f, -0.2f));

    vector<Vertex> ear_r;
    ear_r.push_back(Vertex(0.4f, 0.0f));
    ear_r.push_back(Vertex(0.4f, 0.1f));
    ear_r.push_back(Vertex(0.45f, 0.2f));
    ear_r.push_back(Vertex(0.53f, 0.3f));
    ear_r.push_back(Vertex(0.56f, 0.2f));
    ear_r.push_back(Vertex(0.5f, 0.0f));
    ear_r.push_back(Vertex(0.45f, -0.1f));
    ear_r.push_back(Vertex(0.4f, -0.1f));

    vector<Vertex> ear_l;
    ear_l.push_back(Vertex(-0.5f, 0.2f));
    ear_l.push_back(Vertex(-0.55f, 0.33f));
    ear_l.push_back(Vertex(-0.56f, 0.2f));
    ear_l.push_back(Vertex(-0.56f, 0.1f));
    ear_l.push_back(Vertex(-0.5f, 0.0f));

        draw_curve(head_r, 8);
        draw_curve(hair_r, 8);
        draw_curve(eye_r, 8);
        draw_curve(eye_l, 8);
        draw_curve(eyebrow_l, 8);
        draw_curve(eyebrow_r, 8);
        draw_curve(nose_r, 8);
        draw_curve(mouth_r, 8);
        draw_curve(ear_r, 8);
        draw_curve(ear_l, 8);

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(700,700); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
