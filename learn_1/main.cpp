
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>

#include <shaders.h>

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

int WIDTH = 800;
int HEIGHT = 800;

vec3 eye; vec3 up;
mat4 projection, modelview;

GLuint vertex_shader, fragment_shader, shader_program;
GLuint projection_pos, modelview_pos, color_pos;

// Cube attributes
GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// back
	-1.0, -1.0, -1.0,
	 1.0, -1.0, -1.0,
	 1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0
};
GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0
};
GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};
GLuint cubeVAO;

float angle = 15.f;

void init(void);
void init_cube_buffers();
void keyboard(unsigned char, int, int);
void special_keyboard(int, int, int);
mat4 translate(float, float, float); 
mat4 rotateX(float);
mat4 rotateY(float);
mat4 rotateZ(float);
void draw_cube();
void changeViewport(int, int);
void render();

// Main function
int main(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Create window
	glutCreateWindow("HELLO GL");
	glutInitWindowSize(WIDTH, HEIGHT);
	glutReshapeWindow(WIDTH, HEIGHT);

	// Bind functions
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);

	// Initialize Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Unable to initalize Glew ! " << std::endl;
		return 1;
	}

	// Start program
	init();
	
	glutMainLoop();

	return 0;
}

void changeViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Returns a homogenious translation matrix
mat4 translate(float x, float y, float z)
{
	// Translation matrix (column-majored)
	mat4 matrix = mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	);

	return matrix;
}

// Returns a rotate matrix
mat4 rotateX(float angle)
{
	float c = glm::cos(angle);
	float s = glm::sin(angle);

	mat4 rx = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, c, s, 0.0,
		0.0, -s, c, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return rx;
}

// Returns a rotate matrix
mat4 rotateY(float angle)
{
	float c = glm::cos(angle);
	float s = glm::sin(angle);

	mat4 ry = mat4(
		c, 0.0, -s, 0.0,
		0.0, 1.0, 0.0, 0.0,
		s, 0.0, c, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return ry;
}

// Returns a rotate matrix
mat4 rotateZ(float angle)
{
	float c = glm::cos(angle);
	float s = glm::sin(angle);

	mat4 rz = mat4(
		c, s, 0.0, 0.0,
		-s, c, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return rz;
}

// Keyboard inputs
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w': // Press 'w' to move forward with camera
			modelview *= translate(0.f, 0.0f, 0.1f);
			break;
		case 's': // Press 's' to move backward with camera
			modelview *= translate(0.f, 0.0f, -0.1f);
			break;
		case 'a': // Press 'e' to move left with camera
			modelview *= translate(0.1f, 0.f, 0.f);
			break;
		case 'd': // Press 'd' to move right with camera
			modelview *= translate(-0.1f, 0.f, 0.f);
			break;
		case 'r': // Press 'r' to move up with camera
			modelview *= translate(0.f, -0.1f, 0.f);
			break;
		case 'f': // Press 'f' to move down with camera
			modelview *= translate(0.f, 0.1f, 0.f);
			break;
	};
	glutPostRedisplay();
}

// Special keyboard inputs
void special_keyboard(int key, int x, int y)
{
	switch (key)
	{
		// Rotate camera
		case GLUT_KEY_UP:
			modelview *= rotateX(glm::radians(angle));
			break;
		case GLUT_KEY_DOWN:
			modelview *= rotateX(glm::radians(-angle));
			break;
		case GLUT_KEY_LEFT:
			modelview *= rotateY(glm::radians(angle));
			break;
		case GLUT_KEY_RIGHT:
			modelview *= rotateY(glm::radians(-angle));
			break;
		case GLUT_KEY_PAGE_UP:
			modelview *= rotateZ(glm::radians(angle));
			break;
		case GLUT_KEY_PAGE_DOWN:
			modelview *= rotateZ(glm::radians(-angle));
			break;
	};
	glutPostRedisplay();
}

// Initialize the program and start
void init(void)
{
	// Initialize viewing values
	eye = vec3(0.f, 0.f, 5.f);
	up = vec3(0.f, 1.f, 0.f);
	projection = glm::perspective(glm::radians(60.0f), (float)WIDTH /HEIGHT, 0.1f, 1000.0f);
	modelview = glm::lookAt(eye, vec3(0.0f, 0.0f, 0.0f), up);

	// Initialize shaders
	vertex_shader = initshaders(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl");
	fragment_shader = initshaders(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl");

	//GLuint program = glCreateProgram();
	shader_program = initprogram(vertex_shader, fragment_shader);
	//GLint linked;
	//glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);

	// Get the positions of other uniform variables
	projection_pos = glGetUniformLocation(shader_program, "projection");
	modelview_pos = glGetUniformLocation(shader_program, "modelview");

	init_cube_buffers();

	glEnable(GL_DEPTH_TEST);
}

// Initialize cube buffers and return
void init_cube_buffers()
{
	glGenVertexArrays(1, &cubeVAO);

	glBindVertexArray(cubeVAO);

	// Vertices buffer
	GLuint vbo_cube_vertices;
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Colors buffer
	GLuint vbo_cube_colors;
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Element buffer
	GLuint ibo_cube_elements;
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	
	// Unbind for prevent further modification
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cube()
{
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0, 1);
	
	// Send camera uniforms
	glUniformMatrix4fv(modelview_pos, 1, GL_FALSE, &(modelview)[0][0]);
	glUniformMatrix4fv(projection_pos, 1, GL_FALSE, &(projection)[0][0]);

	// Draw cube
	draw_cube();


	glutSwapBuffers();
}
