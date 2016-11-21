//==============================================================================
// SDL Tutorial 1
//==============================================================================
#ifndef _VIEW_H_
#define _VIEW_H_

#include "SDL2/SDL.h"

#include "GameState.h"
#include "Utilities/Color.h"
#include "Entities/Crate.h"
#include <utility>
#include <iostream>


#define WIN_H 900
#define WIN_W 1600
#define BODY_DIAMETER 4

#define DEFAULT_X_DISTANCE 3e10

// GUI parameters
#define HEALTH_BAR_HEIGHT 50
#define HEALTH_BAR_WIDTH 250
#define HEALTH_BAR_V_PADDING 10
#define HEALTH_BAR_H_PADDING 10


/**
 * Handles the GUI and representation of the System
 */
class View  {
public:
    View(GameState * pGameState, bool fullscreen);
    ~View();

    void Render();

    void drawShip(const Ship &ship, const System &system);
	void drawShipBars(const Ship &ship, const System &system);
    void drawProjectile(const Projectile &projectile, const System &system);
	void drawCrate(const Crate &crate, const System &system);
    void drawBar(Sint16 pTopLeftX, Sint16 pTopLeftY, Sint16 pWidth, Sint16 pHeight,
				   Color backgroundColor, Color foregroundColor,double fractionFull);

    void setColor(Color pColor);

    void drawCircle(int xCenter, int yCenter, int radius);
    void drawCircle(int xCenter, int yCenter, int radius, int lineWidth);
    void drawCircleFilled(int xCenter, int yCenter, int radius);
    void drawTriangle(int xCenter, int yCenter, int width, int height, double angle, int lineWidth);
    void drawRectUnfilled(int xCenter, int yCenter, int width, int height, int lineWidth);


    std::pair<int, int> rotateAround(int px, int py, int ox, int oy, double angle);

    std::pair<long long,long long> screenToSpace(int x, int y,const System * system);
    std::pair<Sint16,Sint16> spaceToScreen(double x, double y,const System * system);
    bool screenCoordsAreWithinCameraView(int x, int y);

    void moveCameraUp();
    void moveCameraDown();
    void moveCameraLeft();
    void moveCameraRight();
    void stopCameraUp();
    void stopCameraDown();
    void stopCameraLeft();
    void stopCameraRight();

    void switchCameraMode();
    void zoomIn();
    void zoomOut();
    void startZoomingIn();
    void startZoomingOut();
    void stopZooming();

private:
    // TODO: set star seed based on some unique server parameter

    void resetScreen();
    void drawGUI();
    void drawGUIBackground();
    void drawSystem();
    void addressZooming();
    void addressMoving();

    int hashFunction(unsigned int a, unsigned int b, unsigned int c);
    void setupStarfield();

    std::pair<float, float> getCameraOffset(const System* system);

    SDL_Window*    Main_Window;
    SDL_Renderer* Main_Renderer;

    std::pair<double, double> cameraCoords;
    GameState * gameState;
    Sint16 areaH;
    Sint16 areaW;
    Sint16 topBorder;
    Sint16 sideBorder;
    Sint16 guiHeight;

    float distancePerPixel;

    float cameraVX;
    float cameraVY;

    int shipScale = 2;

    float zoomDelta = 0;
    float defaultZoomChange = 5e2;
    float zoomInLimit = 10;
    float zoomOutLimit = 1e5;


};

// Color defines
#define SHIP_1 {10,  200, 10 };
#define SHIP_2 {10,  100, 200};
#define SHIP_3 {200, 75,  25 };
#define SHIP_4 {150, 10,  200};
#define SHIP_5 {150, 150, 150};

#define SUN_YELLOW {255, 255, 0};

#endif
