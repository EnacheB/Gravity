//==============================================================================
#include "View.h"
#include "Utilities/Color.h"
#include "Entities/Crate.h"

View::View(GameState * pGameState, bool fullscreen) {
	gameState = pGameState;

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
		throw 100;
	}

	uint32_t flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
	if ((Main_Window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, flags)) == NULL) {
		throw 100;
	}
	if ((Main_Renderer = SDL_CreateRenderer(Main_Window, -1, 0)) == NULL) {
		throw 100;
	}
	SDL_SetRenderDrawColor(Main_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Main_Renderer);
	SDL_RenderPresent(Main_Renderer);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(Main_Renderer, WIN_W, WIN_H);

	cameraCoords = std::make_pair(0, 0);

    distancePerPixel = zoomInLimit * 100;

	topBorder = 25;
	sideBorder = 25;
	guiHeight = 50;
    cameraVX = 0;
    cameraVY = 0;

	resetScreen();
}

View::~View() {
	SDL_DestroyRenderer(Main_Renderer);
	SDL_DestroyWindow(Main_Window);
}


void View::Render() {
	resetScreen();

    addressZooming();
    addressMoving();
	drawSystem();

	drawGUIBackground();
	drawGUI();

	SDL_RenderPresent(Main_Renderer);
}

void View::resetScreen() {
	// Make background grey
	SDL_SetRenderDrawColor(Main_Renderer, 70, 70, 70, 255);
	SDL_RenderClear(Main_Renderer);

	// Set size variables
	areaW = WIN_W - sideBorder * 2;
	areaH = WIN_H - guiHeight - 2 * topBorder;

	// Make playing-box
	SDL_Rect rect = { sideBorder,topBorder,areaW,areaH };
	SDL_SetRenderDrawColor(Main_Renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(Main_Renderer, &rect);
}

void View::drawGUIBackground() {
	// Make background grey
	SDL_SetRenderDrawColor(Main_Renderer, 70, 70, 70, 255);

	// Make playing-box
	SDL_Rect rect1 = { 0, 0, WIN_W, topBorder };
	SDL_Rect rect2 = { 0, WIN_H - topBorder - guiHeight, WIN_W, topBorder + guiHeight };
	SDL_Rect rect3 = { 0, 0, sideBorder, WIN_H };
	SDL_Rect rect4 = { WIN_W - sideBorder, 0, sideBorder, WIN_H };
	SDL_RenderFillRect(Main_Renderer, &rect1);
	SDL_RenderFillRect(Main_Renderer, &rect2);
	SDL_RenderFillRect(Main_Renderer, &rect3);
	SDL_RenderFillRect(Main_Renderer, &rect4);
}


void View::drawGUI() {
	  const System * system = gameState->lockMostRecentSystem();
	double hullFraction = system->playerShip.getHullFraction();
	double shieldFraction = system->playerShip.getShieldFraction();
    double heatFraction = system->playerShip.getHeatFraction();


    gameState->unlockMostRecentSystem();

	drawBar(sideBorder + HEALTH_BAR_H_PADDING, topBorder + areaH + HEALTH_BAR_V_PADDING,
        HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT , Color(25,50,50), Color(0,200,200),shieldFraction);

    drawBar( sideBorder - HEALTH_BAR_H_PADDING + areaW - HEALTH_BAR_WIDTH,
        topBorder + areaH + HEALTH_BAR_V_PADDING, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT ,
			 Color(50,20,20), Color(200,32,32) ,hullFraction);
	drawBar(sideBorder + areaW/2 - HEALTH_BAR_WIDTH/2, topBorder + areaH + HEALTH_BAR_V_PADDING,
        HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT , Color(100,20,0), Color(255,69,0),heatFraction);


}

void View::drawSystem() {
	const System * system = gameState->lockMostRecentSystem();


	for (const Ship &ship : system->ships) {
		SDL_SetRenderDrawColor(Main_Renderer, 200, 200, 0, SDL_ALPHA_OPAQUE);
		drawShip(ship,*system);
        drawShipBars(ship, *system);
	}

	SDL_SetRenderDrawColor(Main_Renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	drawShip(system->playerShip,*system);

	for (const Projectile &projectile : system->projectiles) {
		SDL_SetRenderDrawColor(Main_Renderer, 0, 0, 200, SDL_ALPHA_OPAQUE);
		drawProjectile(projectile,*system);
	}

    for (const Projectile &projectile : system->AIprojectiles) {
        SDL_SetRenderDrawColor(Main_Renderer, 0, 100, 200, SDL_ALPHA_OPAQUE);
        drawProjectile(projectile,*system);

    }

	for (const Crate &crate : system->crates) {
		SDL_SetRenderDrawColor(Main_Renderer, 100, 0, 250, SDL_ALPHA_OPAQUE);
		drawCrate(crate,*system);

	}

    SDL_SetRenderDrawColor(Main_Renderer, 200, 200, 0, SDL_ALPHA_OPAQUE);
	for(double angle = 0; angle < 2*M_PI; angle += 2*M_PI/90 ) {
        	std::pair<short, short> screenCoords = spaceToScreen(system->getKillRadius() * cos(angle)
                    , system->getKillRadius() * sin(angle), system);
	    if(screenCoordsAreWithinCameraView(screenCoords.first,screenCoords.second))
        drawCircle(screenCoords.first, screenCoords.second, 10);
    }



	gameState->unlockMostRecentSystem();
}




std::pair<float, float> View::getCameraOffset(const System *system) {
	// If there are ships and the camera mode is set to follow the player, this will center the view; otherwise they're 0
    return std::make_pair(system->playerShip.getPosition().x, system->playerShip.getPosition().y);

}

std::pair<long long,long long> View::screenToSpace(int x, int y,const System * system) {
	double dimension = areaH < areaW ? areaH : areaW;
	auto offset = getCameraOffset(system);
    return std::make_pair(
    	(x - cameraCoords.first  - areaW / 2) * distancePerPixel  / dimension + offset.first, //Very wrong, but compiles
    	(y - cameraCoords.second - areaH / 2) * distancePerPixel  / dimension + offset.second
    );
}

std::pair<Sint16,Sint16> View::spaceToScreen(double x, double y,const System * system) {
	auto offset = getCameraOffset(system);
	return std::make_pair(
		// position                    [fit to screen                                 ]  [free cammera offset            ]
		((x - offset.first) / distancePerPixel + areaW / 2 + cameraCoords.first ),
		((y - offset.second) /distancePerPixel + areaH / 2 + cameraCoords.second)
	);
}


void View::drawCircle(int xCenter, int yCenter, int radius, int lineWidth) {
	for (int i = 0; i < lineWidth; i++) {
		// draws better than using +- i
		drawCircle(xCenter, yCenter, radius - i);
	}
}

void View::drawCircle(int xCenter, int yCenter, int radius) {
	if (radius < 1) {
		SDL_RenderDrawPoint(Main_Renderer, xCenter, yCenter);
	}

	int x = radius;
	int y = 0;
	int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

	while (y <= x) {
		SDL_RenderDrawPoint(Main_Renderer, x + xCenter, y + yCenter); // Octant 1
		SDL_RenderDrawPoint(Main_Renderer, y + xCenter, x + yCenter); // Octant 2
		SDL_RenderDrawPoint(Main_Renderer, -x + xCenter, y + yCenter); // Octant 4
		SDL_RenderDrawPoint(Main_Renderer, -y + xCenter, x + yCenter); // Octant 3
		SDL_RenderDrawPoint(Main_Renderer, -x + xCenter, -y + yCenter); // Octant 5
		SDL_RenderDrawPoint(Main_Renderer, -y + xCenter, -x + yCenter); // Octant 6
		SDL_RenderDrawPoint(Main_Renderer, x + xCenter, -y + yCenter); // Octant 8
		SDL_RenderDrawPoint(Main_Renderer, y + xCenter, -x + yCenter); // Octant 7
		y++;
		if (decisionOver2 <= 0) {
			decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
		} else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
		}
	}
}

void View::drawCircleFilled(int xCenter, int yCenter, int radius) {
	if (radius < 1) {
		SDL_RenderDrawPoint(Main_Renderer, xCenter, yCenter);
	}

	int x = radius;
	int y = 0;
	int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

	while (y <= x) {

		SDL_RenderDrawLine(Main_Renderer, x + xCenter, y + yCenter, -x + xCenter, y + yCenter);
		SDL_RenderDrawLine(Main_Renderer, y + xCenter, x + yCenter, -y + xCenter, x + yCenter);
		SDL_RenderDrawLine(Main_Renderer, -x + xCenter, -y + yCenter, x + xCenter, -y + yCenter);
		SDL_RenderDrawLine(Main_Renderer, -y + xCenter, -x + yCenter, y + xCenter, -x + yCenter);

		y++;
		if (decisionOver2 <= 0) {
			decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
		} else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
		}
	}
}

void View::drawTriangle(int xCenter, int yCenter, int width, int height, double angle, int lineWidth) {
	for (int i = 0; i < lineWidth; i++) {
		// Generate the vertices
		std::pair<int, int> point = rotateAround(xCenter + height / 2 - i, yCenter, xCenter, yCenter, angle);
		std::pair<int, int> left = rotateAround(xCenter - height / 2 + i, yCenter + width / 2 - i, xCenter, yCenter, angle);
		std::pair<int, int> right = rotateAround(xCenter - height / 2 + i, yCenter - width / 2 + i, xCenter, yCenter, angle);

		// Draw the lines
		SDL_RenderDrawLine(Main_Renderer, point.first, point.second, left.first, left.second);
		SDL_RenderDrawLine(Main_Renderer, point.first, point.second, right.first, right.second);
		SDL_RenderDrawLine(Main_Renderer, right.first, right.second, left.first, left.second);
	}
}

// TODO: filled triangle?

void View::drawRectUnfilled(int xCenter, int yCenter, int width, int height, int lineWidth) {
	for (int i = 0; i < lineWidth; i++) {
		SDL_RenderDrawLine(Main_Renderer, xCenter - width / 2 + i, yCenter - height / 2 + i, xCenter + width / 2 - i, yCenter - height / 2 + i);
		SDL_RenderDrawLine(Main_Renderer, xCenter + width / 2 - i, yCenter - height / 2 + i, xCenter + width / 2 - i, yCenter + height / 2 - i);
		SDL_RenderDrawLine(Main_Renderer, xCenter + width / 2 - i, yCenter + height / 2 - i, xCenter - width / 2 + i, yCenter + height / 2 - i);
		SDL_RenderDrawLine(Main_Renderer, xCenter - width / 2 + i, yCenter + height / 2 - i, xCenter - width / 2 + i, yCenter - height / 2 + i);
	}
}

std::pair<int, int> View::rotateAround(int pointX, int pointY, int originX, int originY, double angle) {
	int x = cos(angle) * (pointX - originX) - sin(angle) * (pointY - originY) + originX;
	int y = sin(angle) * (pointX - originX) + cos(angle) * (pointY - originY) + originY;
	return std::make_pair(x, y);
}

void View::moveCameraLeft() {
	cameraVX = -2;
}
void View::moveCameraRight() {
	cameraVX = 2;
}
void View::moveCameraDown() {
	cameraVY = -2;
}
void View::moveCameraUp() {
	cameraVY = 2;
}

void View::stopCameraLeft() {
	cameraVX = 0;
}
void View::stopCameraRight() {
	cameraVX = 0;
}
void View::stopCameraDown() {
	cameraVY = 0;
}
void View::stopCameraUp() {
	cameraVY = 0;
}

/*
void View::switchCameraMode() {
	freeCamera = !freeCamera;
	if (freeCamera) {
		cameraCoords = std::make_pair(0, 0);
	}
}
*/

void View::zoomIn() {
	if (distancePerPixel + zoomDelta < zoomOutLimit && distancePerPixel + zoomDelta > zoomInLimit)
		distancePerPixel += zoomDelta;
}

void View::zoomOut() {
	if (distancePerPixel + zoomDelta < zoomOutLimit && distancePerPixel + zoomDelta > zoomInLimit)
		distancePerPixel += zoomDelta;
}

bool View::screenCoordsAreWithinCameraView(int x, int y) {
	return (y <= topBorder + areaH && y >= topBorder && x >= sideBorder && x <= areaW + sideBorder);
}

void View::startZoomingIn()
{
    zoomDelta = -defaultZoomChange;
}

void View::startZoomingOut()
{
    zoomDelta = defaultZoomChange;
}

void View::stopZooming()
{
    zoomDelta = 0.0;
}

void View::addressZooming()
{

	if (distancePerPixel + zoomDelta < zoomOutLimit && distancePerPixel + zoomDelta > zoomInLimit)
		distancePerPixel += zoomDelta * distancePerPixel/1e6;
    else
    {
        stopZooming();
        if(distancePerPixel >= zoomOutLimit)
            distancePerPixel = zoomOutLimit - defaultZoomChange;
        if(distancePerPixel <= zoomOutLimit)
            distancePerPixel = zoomOutLimit + defaultZoomChange;
            distancePerPixel = zoomOutLimit - defaultZoomChange;

    }

}

void View::addressMoving()
{
    cameraCoords.first += cameraVX;
    cameraCoords.second += cameraVY;
}

void View::drawShip(const Ship &ship, const System &system) {
	std::pair<short, short> screenCoords = spaceToScreen(ship.getPosition().x, ship.getPosition().y, &system);

	if(screenCoordsAreWithinCameraView(screenCoords.first,screenCoords.second))
	{
		screenCoords = std::make_pair(screenCoords.first + sideBorder, screenCoords.second + topBorder);
		drawTriangle(static_cast<Sint16>(screenCoords.first), static_cast<Sint16>(screenCoords.second), 5 * shipScale, 10 * shipScale, ship.getAngle(), 3);

		// draw an engine effects if they're on
		if (ship.getEnginesOn()) {
			std::pair<short, short> exhaustPoint = std::make_pair(screenCoords.first, screenCoords.second + 10 * shipScale);
			exhaustPoint = rotateAround(exhaustPoint.first, exhaustPoint.second, screenCoords.first, screenCoords.second, ship.getAngle() + M_PI / 2.0);

			SDL_SetRenderDrawColor(Main_Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			drawTriangle(exhaustPoint.first, exhaustPoint.second, 3 * shipScale, 6 * shipScale, ship.getAngle() + M_PI, 2);
		}
	}



}

void View::drawShipBars(const Ship &ship, const System &system) {
    std::pair<short, short> screenCoords = spaceToScreen(ship.getPosition().x, ship.getPosition().y, &system);

    if (screenCoordsAreWithinCameraView(screenCoords.first, screenCoords.second)) {

        drawBar(static_cast<Sint16> (screenCoords.first),
                static_cast<Sint16>(screenCoords.second - 5.0 * shipScale - 5), 30, 5,
                Color(25, 50, 50), Color(100, 200, 200), ship.getShieldFraction());


        drawBar(static_cast<Sint16>(screenCoords.first),
                static_cast<Sint16>(screenCoords.second - 5.0 * shipScale),
                30, 5, Color(50, 50, 20), Color(200, 200, 100), ship.getHullFraction());
    }
}



void View::drawProjectile(const Projectile &projectile, const System &system) {
	  std::pair<short, short> screenCoords = spaceToScreen(projectile.getPosition().x, projectile.getPosition().y, &system);
        float projectileDrawSize = projectile.getCollisionRange() / distancePerPixel;
        if (projectileDrawSize > 3) {
            drawCircle(static_cast<Sint16>(screenCoords.first + sideBorder), static_cast<Sint16>(screenCoords.second + topBorder), projectileDrawSize, 2);
        } else {
            drawRectUnfilled(static_cast<Sint16>(screenCoords.first + sideBorder), static_cast<Sint16>(screenCoords.second + topBorder), 10, 10, 3);
        }
}

void View::drawCrate(const Crate &crate, const System &system) {
	std::pair<short, short> screenCoords = spaceToScreen(crate.getPosition().x, crate.getPosition().y, &system);
	int  crateSideLength = crate.getCollisionRange() / distancePerPixel;
    crateSideLength /= 2;
	if(crateSideLength < 10)
		crateSideLength = 10;
	drawRectUnfilled(static_cast<Sint16>(screenCoords.first + sideBorder), static_cast<Sint16>(screenCoords.second + topBorder)
			,crateSideLength, crateSideLength, 3);
}

void View::drawBar(Sint16 pTopLeftX, Sint16 pTopLeftY, Sint16 pWidth, Sint16 pHeight,
				   Color backgroundColor, Color foregroundColor,double fractionFull) {
	SDL_Rect barFull = { pTopLeftX, pTopLeftY, pWidth, pHeight};
	setColor(backgroundColor);
	SDL_RenderFillRect(Main_Renderer, &barFull);

	SDL_Rect barPartial = { pTopLeftX, pTopLeftY, static_cast<int>(fractionFull * pWidth), pHeight};
	setColor(foregroundColor);
	SDL_RenderFillRect(Main_Renderer, &barPartial);
}

void View::setColor(Color pColor) {
	SDL_SetRenderDrawColor(Main_Renderer, pColor.r, pColor.g, pColor.b, SDL_ALPHA_OPAQUE);

}

