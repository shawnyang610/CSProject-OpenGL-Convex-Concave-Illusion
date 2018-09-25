//  CS718 Computer Graphics
//  project1_random generated disks (5~10 with diff lighting patterns)
//  Author: Shawn Yang
//  9/24/2018

#include <GLUT/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

int WINDOW_HEIGHT=600;
int WINDOW_WIDTH=800;
int MIN_NUM_OF_DISKS = 5;
int MAX_NUM_OF_DISKS = 10;
int TRY_LIMIT=50; // max num of times trying to generate non-overlaping disks

struct Disk{
  // disk
  int center_x;
  int center_y;
  int pattern;
  int radius;
  // bounding box coordinates of the disk, for overlap check
  int bb_ul_x;
  int bb_ul_y;
  int bb_ur_x;
  int bb_ur_y;
  int bb_ll_x;
  int bb_ll_y;
  int bb_lr_x;
  int bb_lr_y;
  Disk(const int window_width, const int window_height, const int rad){
    radius = rad;
    // center is randomly generated within object space
    center_x = radius + rand() % (window_width - radius * 2);
    center_y = radius + rand() % (window_height - radius * 2);
    pattern = rand()%2; // 1 = reversed  pattern
    // calculates its bounding box
    bb_ul_x = center_x - radius;
    bb_ul_y = center_y + radius;
    bb_ur_x = center_x + radius;
    bb_ur_y = center_y + radius;
    bb_ll_x = center_x - radius;
    bb_ll_y = center_y - radius;
    bb_lr_x = center_x + radius;
    bb_lr_y = center_y - radius;
  }
};

bool is_overlap(const Disk &disk_1, const Disk &disk_2){
  // find overlapping bounding box
  int ovlp_bb_ll_x = disk_1.bb_ll_x > disk_2.bb_ll_x ? disk_1.bb_ll_x : disk_2.bb_ll_x;
  int ovlp_bb_ll_y = disk_1.bb_ll_y > disk_2.bb_ll_y ? disk_1.bb_ll_y : disk_2.bb_ll_y;
  int ovlp_bb_ur_x = disk_1.bb_ur_x < disk_2.bb_ur_x ? disk_1.bb_ur_x : disk_2.bb_ur_x;
  int ovlp_bb_ur_y = disk_1.bb_ur_y < disk_2.bb_ur_y ? disk_1.bb_ur_y : disk_2.bb_ur_y;
  
  // whether ovlp bounding box is valid, if yes, return false
  if (ovlp_bb_ur_x < ovlp_bb_ll_x || ovlp_bb_ur_y < ovlp_bb_ll_y){
    return false;
  }
  return true;
}

bool is_disk_valid(const Disk &disk,const vector<Disk> &disks){
  // check if sphere overlaps with prior spheres already in the list
  for (Disk d : disks){
    if (is_overlap(d, disk)){
      return false;
    }
  }
  return true;
}

vector<Disk>& generate_disks(int window_height, int window_width){
  srand(time(NULL));
  rand();
  int try_limit = TRY_LIMIT;
  int try_count=0;
  int num_disks =MIN_NUM_OF_DISKS + rand() % (MAX_NUM_OF_DISKS-MIN_NUM_OF_DISKS+1);
  vector<Disk> *valid_disks = new vector<Disk>;
  int shortest_axis = fmin (window_height, window_width);
  int disk_diameter = fmin((sqrt(window_height*window_width/(MAX_NUM_OF_DISKS*2))), shortest_axis);
  Disk * disk;
  // randomly generate a disk within object space
  // check for overlap, if no overlap then add to list.
  // if overlap, repeat until reaching number of TRY_LIMIT
  // stops when num_disks is reached in the list
  while (try_count++ < try_limit){
    disk = new Disk(WINDOW_WIDTH, WINDOW_HEIGHT, disk_diameter/2);
    if (is_disk_valid(*disk, *valid_disks)){
      valid_disks->push_back(*disk);
    }
    else {
      delete disk;
    }
    if (valid_disks->size()>=num_disks){
      break;
    }
  }
  return *valid_disks;
}


void draw_disk(const Disk &disk){
  double t, p = 3.141593/180;
  // count handles 2 diff patterns
  int count;
  if (disk.pattern==0){
    count = 0;
  }
  else {
    count = -2 * disk.radius -1;
  }
  
  for (int i=disk.radius; i>=-disk.radius;i--){
    float color_density = (abs(count++))/(double)(2*disk.radius+1); // handles 2 diff patterns
    glColor3f(color_density, color_density, color_density);
    t = i * p;
    float pt_l_x = sqrt(disk.radius * disk.radius - i * i);
    float pt_l_y = i;
    float pt_r_x = - pt_l_x;
    float pt_r_y = i;
    glVertex2f(pt_l_x + disk.center_x, pt_l_y + disk.center_y);
    glVertex2f(pt_r_x + disk.center_x, pt_r_y + disk.center_y);
  }
}

void display (void){
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINES);
    // start drawing all the disks in the list
    vector<Disk> disks = generate_disks(WINDOW_HEIGHT, WINDOW_WIDTH);
    for (Disk disk: disks){
      draw_disk(disk);
    }
    //
  glEnd();
  glFlush();
}

int main(int argc, char ** argv) {
  
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(200, 100);
  glutCreateWindow("Shawn Yang's 718-Project1");
  glClearColor(0.1, 0.1, 0.1, 0.1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT );
  glutDisplayFunc(display);
  glutMainLoop();
  
  return 0;
}
