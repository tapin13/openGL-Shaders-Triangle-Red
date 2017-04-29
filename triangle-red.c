/* 
 * File:   triangle.c
 * Author: tapin13
 *
 * Created on April 27, 2017, 1:32 AM
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

GLuint program;
GLint attribute_coord2d;

int init_resources(void) {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source = 
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
#else
    "#version 120\n"
#endif
    "attribute vec2 coord2d;"
    "void main(void) {"
    "   gl_Position = vec4(coord2d, 0.0, 1.0);"
    "}";
    
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if(compile_ok == 0) {
        fprintf(stderr, "Error in vertex shader\n");
        return 0;
    }
    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source = ""
    "#version 120\n"
    "void main(void) {"
    "   gl_FragColor[0] = 1.0;"
    "   gl_FragColor[1] = 0.0;"
    "   gl_FragColor[2] = 0.0;"
    "}";
    
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if(!compile_ok) {
        fprintf(stderr, "Error in fragment shader\n");
        return 0;
    }
    
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if(!link_ok) {
        fprintf(stderr, "Error in linking program\n");
        return 0;
    }
    
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if(attribute_coord2d == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }
    
    return 1;
}

void onDisplay() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_verteces[] = {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8
    };
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, triangle_verteces);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);
    
    glutSwapBuffers();
}

void free_resources(void) {
    glDeleteProgram(program);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangle");
    
    // probable this part can be skip
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        return EXIT_FAILURE;
    }
    
    if(init_resources() == 1) {
        glutDisplayFunc(onDisplay);
        glutMainLoop();
    }
    
    free_resources();
    
    return (EXIT_SUCCESS);
}

