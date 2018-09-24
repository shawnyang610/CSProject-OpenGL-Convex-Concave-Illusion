//
//  main.cpp
//  project_disks
//
//  Created by Shawn Yang on 9/24/18.
//  Copyright © 2018 Shawn Yang. All rights reserved.
//
#include <GLUT/glut.h>
#include <cmath>
#include <iostream>


void draw_sphere(int pos_h=100, int pos_v=100, int r=75){
  double t, p = 3.141593/180;
  int count=0;
  for (int i=r; i>=-r;i--){
    float color_density = (count++)/(double)(2*r+1);
    glColor3f(color_density, color_density, color_density);
    t = i * p;
    float pt_l_x = sqrt(r*r-i*i);
    float pt_l_y = i;
    float pt_r_x = - pt_l_x;
    float pt_r_y = i;
    glVertex2f(pt_l_x+pos_h, pt_l_y+pos_v);
    glVertex2f(pt_r_x+pos_h, pt_r_y+pos_v);
  }
}

void display (void){
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINES);
    draw_sphere();
  glEnd();
  glFlush();
}

int main(int argc, char ** argv) {
  
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(1024, 768);
  glutInitWindowPosition(200, 100);
  glutCreateWindow("project1");
  glClearColor(0.2, 0.2, 0.2, 0.2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1024.0, 0, 768.0);
  glutDisplayFunc(display);
  glutMainLoop();
  
  
  std::cout << "Hello, World!\n";
  return 0;
}
