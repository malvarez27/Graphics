/***
 Assignment-3: Shading via Lighting and Colors
 
 Name: Alvarez, Marissa
 
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: In this project I added color and implemented cross product, dot product, normals, and etc. This project is fun in my opinion because I love to see the finished product. Although, I did not implement shading yet, the image would have came out awesome. I added color to this code and saw my objects come to life. Implementing the functions helped me learn and get a better grasp on the concepts we learned in class...
    Also, over the weekend I implemented shading. It is definitely not perfect because my scene is flickering, but you can see some of the shading and light source from the implementation. I implemented and added two new function that subtract vectors and calculate h. I utilize those helper functions in my apply shading function. In my apply shading function I declared variable and used a for loop to implement.
 ***/



#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;



/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
float theta = 0.0;
vector<GLfloat> amb = {0.3, 0.3, 0.3};
vector<GLfloat> light_source = {0.0, 3.0, 0.0};
vector<GLfloat> camera = {2.0, 6.0, 5.0};
vector<GLfloat> spec = {0.25, 0.25, 0.25};
vector<GLfloat> diff = {0.25, 0.25, 0.25};

class ObjectModel {
    vector<GLfloat> _points;
    vector<GLfloat> _normals;
    vector<GLfloat> _base_colors;
    vector<GLfloat> _colors;
public:
    ObjectModel() { };
    vector<GLfloat> get_points() { return _points; };
    vector<GLfloat> get_normals() { return _normals; };
    vector<GLfloat> get_base_colors() { return _base_colors; };
    vector<GLfloat> get_colors() { return _colors; };
    void set_points(vector<GLfloat> points) { _points = points; };
    void set_normals(vector<GLfloat> normals) { _normals = normals; };
    void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
    void set_colors(vector<GLfloat> colors) { _colors = colors; };
};


/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  Using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene.*
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    
    for(int i = 0; i < (cartesian_coords.size()); i+=3) {
        //        cartesian_coords[i][3] = 1;
        result.push_back(cartesian_coords[0+i]);
        result.push_back(cartesian_coords[1+i]);
        result.push_back(cartesian_coords[2+i]);
        result.push_back(1.0f);
    }
    //    result.push_back(cartesian_coords[0]);
    //    result.push_back(cartesian_coords[1]);
    //    result.push_back(cartesian_coords[2]);
    //    result.push_back(1.0f);
    
    // Append the 1 in the 4th dimension to generate homogenous coordinates
    // have for loop instead and insert 1.0
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    
    int counter = 0;
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if (counter == 3) {
            counter = 0;
        }
        else{
            result.push_back(homogenous_coords[i]);
            counter++;
        }
        //    result.push_back(homogenous_coords[0]);
        //    result.push_back(homogenous_coords[1]);
        //    result.push_back(homogenous_coords[2]);
        
        // Remove the 1 in the 4th dimension to generate Cartesian coordinates
    }
    
    return result;
}
// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat = {
        1, 0, 0, dx,
        0, 1, 0, dy,
        0, 0, 1, dz,
        0, 0, 0, 1
    };
    
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat = {
    sx, 0, 0, 0,
    0, sy, 0, 0,
    0, 0, sz, 0,
    0, 0, 0,  1
};
return scale_mat;
}
// Definition of a rotation matrix along the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x = {
        1, 0,          0,           0,
        0, cos(theta), -sin(theta), 0,
        0, sin(theta), cos(theta),  0,
        0, 0,          0,           1
    };
    
    return rotate_mat_x;
}

// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y = {
        cos(theta), 0, sin(theta), 0,
        0,          1, 0,          0,
        -sin(theta),0, cos(theta), 0,
        0,          0, 0,          1
    };
    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z = {
        cos(theta), -sin(theta), 0, 0,
        sin(theta), cos(theta),  0, 0,
        0,          0,           1, 0,
        0,          0,           0, 1
    };
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    
    for(int i = 0; i < (B.size())/4; i++){
        result.push_back(A[0]*B[0+(i*4)] + A[1]*B[1+(i*4)] + A[2]*B[2+(i*4)] + A[3]*B[3+(i*4)]); //x
        result.push_back(A[4]*B[0+(i*4)] + A[5]*B[1+(i*4)] + A[6]*B[2+(i*4)] + A[7]*B[3+(i*4)]); //y
        result.push_back(A[8]*B[0+(i*4)] + A[9]*B[1+(i*4)] + A[10]*B[2+(i*4)] + A[11]*B[3+(i*4)]); //z
        result.push_back(A[12]*B[0+(i*4)] + A[13]*B[1+(i*4)] + A[14]*B[2+(i*4)] + A[15]*B[3+(i*4)]); //1
    }
    
    // to_cartesian
    return result;
}


// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    
    vector<GLfloat> init_plane_coords = to_homogenous_coord(init_plane());
    
    vector<GLfloat> a1 = mat_mult(translation_matrix(0, 0, 0.5), init_plane_coords);
    vector<GLfloat> a2 = mat_mult(translation_matrix(0, 0, -0.5), mat_mult(rotation_matrix_y(M_PI), init_plane_coords));
    vector<GLfloat> a3 = mat_mult(translation_matrix(-0.5, 0, 0), mat_mult(rotation_matrix_y(-(M_PI/2)), init_plane_coords));
    vector<GLfloat> a4 = mat_mult(translation_matrix(0.5, 0, 0), mat_mult(rotation_matrix_y(M_PI/2), init_plane_coords));
    vector<GLfloat> a5 = mat_mult(translation_matrix(0, -0.5, 0), mat_mult(rotation_matrix_x(M_PI/2), init_plane_coords));
    vector<GLfloat> a6 = mat_mult(translation_matrix(0, 0.5, 0), mat_mult(rotation_matrix_x(-(M_PI/2)), init_plane_coords));
    
    // a = a1 || a2 || ... || a6
    result.insert(result.end(), a1.begin(), a1.end());
    result.insert(result.end(), a2.begin(), a2.end());
    result.insert(result.end(), a3.begin(), a3.end());
    result.insert(result.end(), a4.begin(), a4.end());
    result.insert(result.end(), a5.begin(), a5.end());
    result.insert(result.end(), a6.begin(), a6.end());
    
    
    return result;
}
/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/
// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;
//     for(int i = 0; i < 12; i+=3){
//         C.push_back (A[i+1]*B[i+2] - A[i+2]*B[i+1]);
//         C.push_back (A[i]*B[i+2] - A[i+2]*B[i]);
//         C.push_back (A[i]*B[i+1] - A[i+1]*B[i]);
//     }
    C.push_back (A[1]*B[2] - A[2]*B[1]);
    C.push_back (A[0]*B[2] - A[2]*B[0]);
    C.push_back (A[0]*B[1] - A[1]*B[0]);
    
    return C;
 
}

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;
    
    int planeN = (points.size()/12);
    
    for(int i =0; i < planeN ; i++){
    vector<GLfloat> q0;
    vector<GLfloat> q1;
    vector<GLfloat> q3;
    vector<GLfloat> left;
    vector<GLfloat> down;
        
        q0.push_back(points[12*i] + 0);
        q0.push_back(points[12*i] + 1);
        q0.push_back(points[12*i] + 2);
        
        q1.push_back(points[12*i] + 3);
        q1.push_back(points[12*i] + 4);
        q1.push_back(points[12*i] + 5);
        
        q3.push_back(points[12*i] + 9);
        q3.push_back(points[12*i] + 10);
        q3.push_back(points[12*i] + 11);
        
        left.push_back(q1[0] - q0[0]);
        left.push_back(q1[1] - q0[1]);
        left.push_back(q1[2] - q0[2]);
        
        down.push_back(q1[0] - q0[0]);
        down.push_back(q1[1] - q0[1]);
        down.push_back(q1[2] - q0[2]);
       
        vector<GLfloat>cross = cross_product(left, down);
        
        for(int i=0; i < 3 ; i++){
            normals.push_back(cross[i]);
        }
        
    
    }
    
    // Note: each plane (quad) contains 4 points, choose the points
    // to generate your vectors such that the normals (given by the
    // cross product, point to the correct direction
    
    return normals;
}
/**************************************************
 *         Shading via Lighting and Color         *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/
vector<GLfloat> sub_vec(vector<GLfloat> first_vec, vector<GLfloat> second_vec ) {
    vector<GLfloat> result;
    for(int i = 0; i < first_vec.size(); i++) {
        result.push_back(first_vec[i] - second_vec[i]);
    }
        return result;
}

vector<GLfloat> calc_h(vector<GLfloat> v, vector<GLfloat> l) {
    vector<GLfloat> result;
    GLfloat vec_top = (v[0] + l[0]);
    GLfloat vec_top1 = v[1] + l[1];
    GLfloat vec_top2 = v[2] + l[2];
    GLfloat bottom1 = sqrt(pow(v[0],2) + pow(l[0], 2));
    GLfloat bottom2 = sqrt(pow(v[1],2) + pow(l[1], 2));
    GLfloat bottom3 = sqrt(pow(v[2],2) + pow(l[2], 2));
    
    result.push_back(vec_top/bottom1);
    result.push_back(vec_top1/bottom2);
    result.push_back(vec_top2/bottom3);
    
    return result;
}

//vector<GLfloat> color_object(GLfloat r, GLfloat g, GLfloat b) {
//    vector<GLfloat> result;
//    vector<GLfloat> base_color = {
//        r,   g,   b,
//        r,   g,   b,
//        r,   g,   b,
//        r,   g,   b
//    };
//    result = add_vector(result, base_color);
//    result = add_vector(result, base_color);
//    result = add_vector(result, base_color);
//
//    return color_object;
//}

// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}

// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
        r3,   g3,   b3
    };
    return base_color;
}

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {

   return ((A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]));
   
}
// Generates the colors of a set of surfaces based on the light source,
// surface normals, and base color of the surface
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera) {
    vector<GLfloat> colors;
    
    object_model.set_colors(colors);
    return object_model;
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 x n values) and specular (vector of 3 x n values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec) {
    vector<GLfloat> base_colors = object_model.get_base_colors();
    vector<GLfloat> normals = object_model.get_normals();
    vector<GLfloat> points = object_model.get_points();
    vector<GLfloat> colors;
    
    for(int i = 0; i < points.size()/3; i++){
        vector<GLfloat> normal = {normals[i*3+0],normals[i*3+1], normals[i*3+2]};
        vector<GLfloat> color_base = {base_colors[i*3+0],base_colors[i*3+1], base_colors[i*3+2]};
        vector<GLfloat> c_points = {points[i*3+0],points[i*3+1], points[i*3+2]};
        vector<GLfloat> v = sub_vec(c_points, camera);
        vector<GLfloat> h = calc_h(v, light_source);
        
         colors.push_back(color_base[0] * (amb[0] + diff[0] * dot_product(normal, light_source)) + (color_base[0] * spec[0] * dot_product(normal, h)));
         colors.push_back(color_base[1] * (amb[1] + diff[1] * dot_product(normal, light_source)) + (color_base[1] * spec[1] * dot_product(normal, h)));
         colors.push_back(color_base[2]* (amb[2] + diff[2] * dot_product(normal, light_source)) + (color_base[2] * spec[2] * dot_product(normal, h)));
        
        
    }
   
    object_model.set_colors(colors);
    return object_model;
}
//to apply shading I grab all of my points and I need to use the shading formula and I need normals, light source, and h which is the vector + the light source!
/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  Create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by using setting *
 *  the projection viewing matrices               *
 *                                                *
 *************************************************/


void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1.0, 2.0, 20.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
vector<GLfloat> add_vector (vector<GLfloat> A, vector<GLfloat> B){
    for(int i = 0; i < B.size(); i++) {
        A.push_back(B[i]);
    }
    return A;
}

vector<GLfloat> add_vector (vector<GLfloat> A, ObjectModel B){
    for(int i = 0; i < B.get_points().size(); i++) {
        A.push_back(B.get_points()[i]);
    }
    return A;
}
vector<GLfloat> color_object(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> result;
    vector<GLfloat> init_base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    result = add_vector(result, init_base_color);
    result = add_vector(result, init_base_color);
    result = add_vector(result, init_base_color);
    result = add_vector(result, init_base_color);
    result = add_vector(result, init_base_color);
    result = add_vector(result, init_base_color);
    
    //color_object.set_points(result);
    
    return result;
}
ObjectModel build_bed(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    vector<GLfloat> bed_bottom = to_cartesian_coord(mat_mult(scaling_matrix(1.5,0.7,2),mat_mult(translation_matrix(1, 0, -0.5),(build_cube()))));
    vector<GLfloat> head_board = to_cartesian_coord(mat_mult(scaling_matrix(1.5,1.3,0.5),mat_mult(translation_matrix(1, 0.2, -4.5),(build_cube()))));
    vector<GLfloat> color_bed_bottom = color_object(0.000, 0.000, 0.545);
    vector<GLfloat> color_head_board = color_object(0.000, 0.749, 1.000);
    
    result = add_vector(result, bed_bottom);
    result = add_vector(result, head_board);
    result_c = add_vector(result_c, color_bed_bottom);
    result_c = add_vector(result_c, color_head_board);
    
    ObjectModel bed;
    bed.set_points(result);
    bed.set_base_colors(result_c);
    bed.set_normals(generate_normals(result));
    bed = apply_shading(bed, light_source, camera, amb, diff, spec);
    return bed;
    
    
}

ObjectModel build_table(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    vector<GLfloat> leg = to_cartesian_coord(mat_mult(scaling_matrix(0.5, 0.7, -0.5),mat_mult(translation_matrix(-3, 0, 0),(build_cube()))));
    vector<GLfloat> leg1 = to_cartesian_coord(mat_mult(scaling_matrix(0.5, 0.7, -0.5),mat_mult(translation_matrix(-5, 0, 0),(build_cube()))));
    vector<GLfloat> top = to_cartesian_coord(mat_mult(translation_matrix(-2,0.7,0), mat_mult(scaling_matrix(2, 0.2, 1), (build_cube()))));
    vector<GLfloat> color_leg = color_object(0.729, 0.333, 0.827);
    vector<GLfloat> color_leg1 = color_object(0.729, 0.333, 0.827);
    vector<GLfloat> color_top = color_object(1.000, 0.000, 1.000);
    
    result = add_vector(result, leg);
    result = add_vector(result, leg1);
    result = add_vector(result, top);
    result_c = add_vector(result_c, color_leg);
    result_c = add_vector(result_c, color_leg1);
    result_c = add_vector(result_c, color_top);
    
    ObjectModel table;
    table.set_points(result);
    table.set_base_colors(result_c);
    table.set_normals(generate_normals(result));
    table = apply_shading(table, light_source, camera, amb, diff, spec);
    
    return table;
    
    
}

ObjectModel build_carpet1(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    
    vector<GLfloat> carpet = to_cartesian_coord(mat_mult(scaling_matrix(5,.1,2.5),mat_mult(translation_matrix(0, -4, .5),(build_cube()))));
    vector<GLfloat> color_carpet = color_object(0.000, 0.808, 0.820);
    
    result = add_vector(result, carpet);
    result_c = add_vector(result_c, color_carpet);
    
    ObjectModel carpet1;
    carpet1.set_points(result);
    carpet1.set_base_colors(result_c);
    carpet1.set_normals(generate_normals(result));
    carpet1 = apply_shading(carpet1, light_source, camera, amb, diff, spec);
    
    return carpet1;
    
    
}
ObjectModel build_chair1(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    vector<GLfloat> chair = to_cartesian_coord(mat_mult(scaling_matrix(0.7,0.3,0.7),mat_mult(translation_matrix(-3, 0.3, -2),(build_cube()))));
    vector<GLfloat> back_chair = to_cartesian_coord(mat_mult(scaling_matrix(0.7,1,0.1),mat_mult(translation_matrix(-3, 0.75, -18),(build_cube()))));
    vector<GLfloat> chair_base = to_cartesian_coord(mat_mult(scaling_matrix(0.1,1,0.1),mat_mult(translation_matrix(-21, -.5, -14),(build_cube()))));
    vector<GLfloat> color_chair = color_object(0.545, 0.271, 0.075);
    vector<GLfloat> color_back_chair = color_object(0.545, 0.271, 0.075);
    vector<GLfloat> color_chair_base = color_object(0.000, 0.000, 0.000);
    
    result = add_vector(result, chair);
    result = add_vector(result, back_chair);
    result = add_vector(result, chair_base);
    result_c = add_vector(result_c, color_chair);
    result_c = add_vector(result_c, color_back_chair);
    result_c = add_vector(result_c, color_chair_base);
    ObjectModel chair1;
    chair1.set_points(result);
    chair1.set_base_colors(result_c);
    chair1.set_normals(generate_normals(result));
    chair1 = apply_shading(chair1, light_source, camera, amb, diff, spec);
    
    return chair1;
    
    
}
ObjectModel build_laptop1(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    
    
    vector<GLfloat> laptop_base = to_cartesian_coord(mat_mult(scaling_matrix(0.5,0.2,0.5),mat_mult(translation_matrix(-4, 4, 0),(build_cube()))));
    vector<GLfloat> laptop_top = to_cartesian_coord(mat_mult(scaling_matrix(0.5 , 0.6, 0.1),mat_mult(translation_matrix(-4, 2, 3),(build_cube()))));
    vector<GLfloat> color_base = color_object(0.502, 0.502, 0.502);
    vector<GLfloat> color_top = color_object(0.502, 0.502, 0.502);
    
    result = add_vector(result, laptop_base);
    result = add_vector(result, laptop_top);
    result_c = add_vector(result_c, color_base);
    result_c = add_vector(result_c, color_top);
    
    //result = add_vector(color_object, 0.502, 0.502, 0.502)
    
    ObjectModel laptop1;
    laptop1.set_base_colors(result_c);
    laptop1.set_points(result);
    laptop1.set_normals(generate_normals(result));
    laptop1 = apply_shading(laptop1, light_source, camera, amb, diff, spec);
    
    return laptop1;
    
    
}
ObjectModel build_pillow1(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    
    vector<GLfloat> pillow = to_cartesian_coord(mat_mult(scaling_matrix(.5,0.25,.11),mat_mult(translation_matrix(3, 2, -17.5),(build_cube()))));
    vector<GLfloat> color_pillow = color_object(1.000, 1.000, 1.000);
    
    result = add_vector(result, pillow);
    result_c = add_vector(result_c, color_pillow);
    
    ObjectModel pillow1;
    pillow1.set_points(result);
    pillow1.set_base_colors(result_c);
    pillow1.set_normals(generate_normals(result));
    pillow1 = apply_shading(pillow1, light_source, camera, amb, diff, spec);
    
    return pillow1;
    
    
}
ObjectModel build_lamp1(){
    vector<GLfloat> result;
    vector<GLfloat> result_c;
    
    vector<GLfloat> lamp = to_cartesian_coord(mat_mult(scaling_matrix(.1,1.5,.1),mat_mult(translation_matrix(-9, 0.5, 6),(build_cube()))));
     vector<GLfloat> lamp_top = to_cartesian_coord(mat_mult(scaling_matrix(.4,0.4,.4),mat_mult(translation_matrix(-2.25,3.75,1.5),(build_cube()))));
    vector<GLfloat> color_lamp = color_object(0.824, 0.706, 0.549);
    vector<GLfloat> color_lamp_top = color_object(1.000, 1.000, 0.000);
    
    result = add_vector(result, lamp);
    result = add_vector(result, lamp_top);
    result_c = add_vector(result_c, color_lamp);
    result_c = add_vector(result_c, color_lamp_top);
    
    ObjectModel lamp1;
    lamp1.set_points(result);
    lamp1.set_base_colors(result_c);
    lamp1.set_normals(generate_normals(result));
    lamp1 = apply_shading(lamp1, light_source, camera, amb, diff, spec);
    
    
    return lamp1;
    
    
}
// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    vector<GLfloat> scene;
    
    //vector<GLfloat> init_cart_coords = to_cartesian_coord(build_cube());
    // return vector2array(build_cube());
    //vector<GLfloat> cube1 =  build_cube();
    
//    vector<GLfloat> leg = to_cartesian_coord(mat_mult(scaling_matrix(0.5, 0.7, -0.5),mat_mult(translation_matrix(-3, 0, 0),(build_cube()))));
//    vector<GLfloat> leg1 = to_cartesian_coord(mat_mult(scaling_matrix(0.5, 0.7, -0.5),mat_mult(translation_matrix(-5, 0, 0),(build_cube()))));
//    vector<GLfloat> top = to_cartesian_coord(mat_mult(translation_matrix(-2,0.7,0), mat_mult(scaling_matrix(2, 0.2, 1), (build_cube()))));
//    vector<GLfloat> carpet = to_cartesian_coord(mat_mult(scaling_matrix(5,.1,2.5),mat_mult(translation_matrix(0, -4, .5),(build_cube()))));
//    vector<GLfloat> bed_bottom = to_cartesian_coord(mat_mult(scaling_matrix(1.5,0.7,2),mat_mult(translation_matrix(1, 0, -0.5),(build_cube()))));
//    vector<GLfloat> head_board = to_cartesian_coord(mat_mult(scaling_matrix(1.5,1.3,0.5),mat_mult(translation_matrix(1, 0.2, -4.5),(build_cube()))));
//    vector<GLfloat> chair = to_cartesian_coord(mat_mult(scaling_matrix(0.7,0.3,0.7),mat_mult(translation_matrix(-3, 0.3, -2),(build_cube()))));
//    vector<GLfloat> back_chair = to_cartesian_coord(mat_mult(scaling_matrix(0.7,1,0.1),mat_mult(translation_matrix(-3, 0.75, -18),(build_cube()))));
//    vector<GLfloat> chair_base = to_cartesian_coord(mat_mult(scaling_matrix(0.1,1,0.1),mat_mult(translation_matrix(-21, -.5, -14),(build_cube()))));
//    vector<GLfloat> laptop_base = to_cartesian_coord(mat_mult(scaling_matrix(0.5,0.2,0.5),mat_mult(translation_matrix(-4, 4, 0),(build_cube()))));
//    vector<GLfloat> laptop_top = to_cartesian_coord(mat_mult(scaling_matrix(0.5 , 0.6, 0.1),mat_mult(translation_matrix(-4, 2, 3),(build_cube()))));
//
//
//    result.insert(result.end(), top.begin(), top.end());
//    result.insert(result.end(), leg.begin(), leg.end());
//    result.insert(result.end(), leg1.begin(), leg1.end());
//    result.insert(result.end(), carpet.begin(), carpet.end());
//    result.insert(result.end(), bed_bottom.begin(), bed_bottom.end());
//    result.insert(result.end(), head_board.begin(), head_board.end());
//    result.insert(result.end(), chair.begin(), chair.end());
//    result.insert(result.end(), back_chair.begin(), back_chair.end());
//    result.insert(result.end(), chair_base.begin(), chair_base.end());
//    result.insert(result.end(), laptop_base.begin(), laptop_base.end());
//    result.insert(result.end(), laptop_top.begin(), laptop_top.end());
//
//    return vector2array(result);
//
    //    leg = to_cartesian_coord(leg);
    //    GLfloat* leg_arr = vector2array(leg);
    //    return leg_arr;
    //    cube1 = to_cartesian_coord(cube1);
    //    GLfloat* cube1_arr = vector2array(cube1);
    //    return cube1_arr;
    scene = add_vector(scene,build_bed().get_points());
    scene = add_vector(scene,build_table().get_points());
    scene = add_vector(scene,build_carpet1().get_points());
    scene = add_vector(scene,build_chair1().get_points());
    scene = add_vector(scene,build_laptop1().get_points());
    scene = add_vector(scene,build_pillow1().get_points());
    scene = add_vector(scene,build_lamp1().get_points());
    return vector2array(scene);
    
}
// Construct the color mapping of the scene
GLfloat* init_color() {
    vector<GLfloat> colors;
    colors = add_vector(colors, build_bed().get_colors());
    colors = add_vector(colors, build_table().get_colors());
    colors = add_vector(colors, build_carpet1().get_colors());
    colors = add_vector(colors, build_chair1().get_colors());
    colors = add_vector(colors, build_laptop1().get_colors());
    colors = add_vector(colors, build_pillow1().get_colors());
    colors = add_vector(colors, build_lamp1().get_colors());
//      for(int i = 0; i <72; i++) {
//        colors.push_back(0.5);
//}
    //cout << colors.size();
     return vector2array(colors);
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glRotatef(theta, 0.0, 1.0, 0.0);
    //glRotatef(theta, 1.0, 0.0, 0.0);
    
    GLfloat* colors = init_color();
    GLfloat* cube = init_scene();
    //GLfloat cube_arr = vector2array(cube);
    //return cube_arr;
    //GLfloat cube_arr = init_scene();
    //vector2array(init_scene());
    // vector<GLfloat> leg = mat_mult(scale_matrix(1, 5, 1), build_cube())
    // vector<GLfloat> top = mat_mult(translation_matrix(0,3,0), mat_mult(scale_matrix(5, 1, 5), build_cube()));
    
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    cube);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4*6*14);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
    delete colors;
    delete cube;
}

void idle_func() {
    theta = theta+0.3;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");
    idle_func();
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

