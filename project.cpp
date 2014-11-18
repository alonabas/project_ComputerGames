//
//  cgp-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//  Tweaked by HUJI Computer Games Programming staff 2014


#include "Defs.h"
#include "Model.h"
#include "Camera.h"
#include "ModelFlags.h"
#include "MainModel.h"
#include "Instructions.h"
#include "Torch.h"
#include "Globals.h"
#include "FinalScene.h"
/** Internal Definitions */

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //
#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //
/** Key definitions */

#define KEY_ANIMATE         ('n') // Key used to start animation              //
#define KEY_ESC            ('\e') // Key used to terminate the program - ESC  //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
#define KEY_UP              ('w') // Key used to move Forward                 //
#define KEY_DOWN            ('s') // Key used to move backward                //
#define KEY_LEFT            ('a') // Key used to rotate left                  //
#define KEY_RIGHT           ('d') // Key used to rotate right                 //
#define KEY_INIT_VIEW       ('i') // Key used to turn counterclockwise      //
#define BACK_FLAGS           ('b') // Key used to turn counterclockwise      //
#define SWITCH_MODE           ('m') // Key used to turn counterclockwise      //
#define MAIN 0
#define SUB 1

extern int _global_width;
extern int _global_high;

/** display callback */
void displayItems(void);

/** display callback */
void displayGame(void);

/** main display callback */
void mainDisplay(void);

/** main window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y);

/** mouse dragging callback  */
void motion(int x, int y);

/** timer callback */
void timer(int value);

/** Keyboard release key function */
void keyUp(unsigned char key, int x, int y);

void menu(int);

void pasive_motion(int x, int y);

/** Global variables */

int g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int g_dwLastFPS = 0;                        // Last FPS Check Time
bool g_startAnimation = false;
bool g_duringAnimation = false;

bool cameraUp = false;
bool cameraDown = false;
bool cameraLeft = false;
bool cameraRight = false;

int main_window, items_window, game_window;
int scene_id;
// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model* _model[2];
FinalScene * _final;
Instructions _text;
Camera* _camera;
Torch * _torch;
ALCdevice* _alDevice;
ALCcontext* _alContext;
Sound* _wind;
/** main function */
int main(int argc, char* argv[]) {
	std::cout << "Starting Hidden Object Game..." << std::endl;

	// Initialize GLUT
	glutInit(&argc, argv);
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif

    // init openAL
	{
		_alDevice = alcOpenDevice(NULL);
		if (!_alDevice)
			std::cout << "Error to access device" << std::endl;
		_alContext = alcCreateContext(_alDevice, NULL);
		if (!_alContext)
			std::cout << "Error to create context" << std::endl;
		alcMakeContextCurrent(_alContext);
	}
    _wind = Sound::readWave("Sound/Windy.wav");


	glutInitWindowSize(WINDOW_SIZE + WINDOW_SIZE_LIST, WINDOW_SIZE);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	// main window
	main_window = glutCreateWindow("CGP Project: Hidden object game");
	glutDisplayFunc(mainDisplay);
	glutReshapeFunc(windowResize);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	int glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(1);
	}

#ifdef __APPLE__
	GLint sync = 1;
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

	// subwindow 1
	{
		items_window = glutCreateSubWindow(main_window, 0, 0, WINDOW_SIZE_LIST,
				WINDOW_SIZE);
		glutDisplayFunc(displayItems);
		// Create a menu
		{
			glutCreateMenu(menu);
			// Add menu items
			glutAddMenuEntry("Switch mode", SWITCH_MODE);
			glutAddMenuEntry("Exit", KEY_ESC);
			// Associate a mouse button with menu
			glutAttachMenu(GLUT_RIGHT_BUTTON);
		}
		_text.init();
	}

	// subwindow 2
	{
		game_window = glutCreateSubWindow(main_window, WINDOW_SIZE_LIST, 0,
				WINDOW_SIZE_LIST + WINDOW_SIZE, WINDOW_SIZE);
		glutDisplayFunc(displayGame);
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyUp);
		glutMouseFunc(mouse);
		glutPassiveMotionFunc(pasive_motion);
		glutMotionFunc(motion);
		glutTimerFunc(100, timer, 0);   // uint millis int value
		glutIgnoreKeyRepeat(0);
		glClearColor(0, 0, 0, 0);
		glClearDepth(1);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	_camera = Camera::instance();
	_model[0] = new MainModel();
	_model[1] = new ModelFlags();
	_final = new FinalScene();
	_final->init();
	_model[0]->init();
	_model[1]->init();
	_torch = Torch::instance();
	_torch->init();
	// Set clear color to black:
	glClearColor(0.0, 0.0, 0.0, 0.0);
	g_startAnimation = true;
	scene_id = 0;

    // Start events/drawing loop
	glutMainLoop();
	alcDestroyContext(_alContext);
	alcCloseDevice(_alDevice);

	return 0;
}

void mainDisplay(void) { // just clear the screen
	glClearColor(0.05, 0.05, 0.15, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void displayGame(void) { // just clear the screen
	glutSetWindow(game_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (scene_id == 1){
        if (_model[scene_id]->getStatus() == ANIMATION){
            _model[scene_id]->draw();
        }
        else if(_text.isEndFlags()){
            _camera->reset();
            scene_id = 0;
            _model[scene_id]->draw();
            _torch->draw();
        }
        else if(_text.isWin()){
            glutSetCursor(GLUT_CURSOR_INFO);
            _camera->reset();
            _wind->stop();
            _final->draw();
        }
        else{
            _model[scene_id]->draw();
            _torch->draw();
        }
    }
    else if(scene_id == 0){
        if (_model[scene_id]->getStatus() == ANIMATION){
            _model[scene_id]->draw();
        }
        else if(_text.isWin()){
            _camera->reset();
            _wind->stop();
            _final->draw();
        }
        else{
            _model[scene_id]->draw();
            _torch->draw();
        }
    }
	glutSwapBuffers();
}

void displayItems(void) {
	glutSetWindow(items_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05, 0.05, 0.15, 1.0f);
	_text.draw();
	glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h) {
	glViewport(0, 0, w, h);
	if (w < WINDOW_SIZE_LIST) {
		// some error
	} else {
		int rest = w - WINDOW_SIZE_LIST;
		if (rest > h)
			rest = h;
		glutSetWindow(game_window);

		_global_width = rest;
		_global_high = rest;
		glutPositionWindow(WINDOW_SIZE_LIST, 0);
		glutReshapeWindow(rest, rest);

		glutSetWindow(items_window);
		_text.resize(WINDOW_SIZE_LIST, h);
		glutPositionWindow(0, 0);
		glutReshapeWindow(WINDOW_SIZE_LIST, h);

	}
}

// Menu handling function definition
void menu(int item) {
	switch (item) {
	case SWITCH_MODE:
		_torch->switchMode();
		break;
    case KEY_ESC:
        exit(RC_OK);
        break;
	default: { /* Nothing */
	}
		break;
	}

	glutPostRedisplay();

	return;
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y) {
	unsigned int lower_key = tolower(key);
	if (_model[scene_id]->getStatus() == ANIMATION || _text.isWin() == true) {
		if(lower_key == KEY_ESC){
			exit(RC_OK);
		}
		return;
	}
	switch (lower_key) {
	case KEY_UP:
		if (scene_id == 0) {
			_camera->turnUp();
			cameraUp = true;
			_torch->update(x, y);
			glutPostRedisplay();
		}
		break;
	case KEY_LEFT:
		if (scene_id == 0) {
			_camera->turnLeft();
			cameraLeft = true;
		} else {
			_camera->moveLeft();
		}
		_torch->update(x, y);
		glutPostRedisplay();
		break;
	case KEY_RIGHT:
		if (scene_id == 0) {
			_camera->turnRight();
			cameraRight = true;

		} else {
			_camera->moveRight();
		}
		_torch->update(x, y);
		glutPostRedisplay();
		break;
	case KEY_DOWN:
		if (scene_id == 0) {
			_camera->turnDown();
			cameraDown = true;
			_torch->update(x, y);
			glutPostRedisplay();
		}
		break;
	case KEY_RESET:
		// reset to initial view of the object
		// For use in a future exercise
		break;
	case KEY_RELOAD:
		// Reload the shading programs of the object
		// For use in a future exercise
		break;
	case KEY_ESC:
		// Terminate the program:
		exit(RC_OK);
		break;
    case SWITCH_MODE:
        _torch->switchMode();
        glutPostRedisplay();
        break;

	case KEY_INIT_VIEW:
		_camera->reset();
		_torch->update(x, y);
        glutPostRedisplay();
		break;
    case BACK_FLAGS:
        if (scene_id == 1){
            _camera->reset();
            _torch->update(x, y);
            scene_id = 0;
            glutPostRedisplay();
        }
        break;
	default:
		std::cerr << "Key " << lower_key << " undefined\n";
		break;
	}
	return;
}

/********************************************************************
 * Function  :	keyUp
 * Arguments :	key : the key that was relesed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/

void keyUp(unsigned char key, int x, int y) {
	unsigned int lower_key = tolower(key);
	switch (lower_key) {
	case KEY_UP:
		cameraUp = false;
		break;
	case KEY_DOWN:
		cameraDown = false;
		break;
	case KEY_LEFT:
		cameraLeft = false;
		break;
	case KEY_RIGHT:
		cameraRight = false;
		break;
	}
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y) {
	if (_model[scene_id]->getStatus() == ANIMATION) {
		return;
	}
	int found;
	if (state == 1 && button == GLUT_LEFT_BUTTON) {
		if( _text.isWin() == true){
			_torch->update(x, y);
			glutPostRedisplay();
		}
		if (_model[scene_id]->isHit(x, y)) {
			found = _model[scene_id]->getFound();
			if (found == 100 && !_text.isEndFlags()) {
				scene_id = 1;
				_camera->reset();
				_text.switchMode();
				_torch->update(x, y);
				displayItems();
			} else {
				_text.setFound(found);
				_model[scene_id]->remove();
				displayItems();
			}
			glutPostRedisplay();
		}
	}
	return;
}

/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x value of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y) {

	return;
}
void pasive_motion(int x, int y) {
	if (x > 0 && x < _global_width && y > 0 && x < _global_high) {
		_torch->update(x, y);
		glutPostRedisplay();
	}
	return;
}
void timer(int value) {
	/* Set the timer to be called again in X milli - seconds. */
	if (g_startAnimation) {
		value = 0;
		g_duringAnimation = true;
		g_startAnimation = false;
	}

	glutTimerFunc(FPS, timer, ++value);   // uint millis int value

	if (g_duringAnimation) {
		if (cameraUp)
			_camera->turnUp();
		if (cameraDown)
			_camera->turnDown();
		if (cameraLeft)
			_camera->turnLeft();
		if (cameraRight)
			_camera->turnRight();
		displayGame();
        if(!_text.isWin()) _wind->play();
	}
}
