/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;
	//Generating random petrol spawns
	petrolSpawn = { { 100, 80 }, { 300, 110 }, { 500, 100 }, { 700, 90 }, { 860, 85 },/**/{ 60, 260 }, { 327, 280 }, { 532, 220 }, { 680, 310 }, { 860, 290 },/**/
					{ 100, 450 }, { 250, 470 }, { 560, 540 }, { 740, 460 }, { 880, 520 },/**/{ 50, 650 }, { 260, 650 }, { 440, 640 }, { 713, 660 }, { 910, 620 } };

	theAreaClicked = {0, 0};
	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "bullet", "playerCar", "theBackground", "menuBackground", "endBackground" };
	texturesToUse = { "Images\\petrol.png", "Images\\petrol.png", "Images\\petrol.png", "Images\\petrol.png", "Images\\bullet.png", "Images\\playerCar.png", "Images\\carPark.png", "Images/Bkg/MainMenu.png", "Images/Bkg/End.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" , "fullPack" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" , "Fonts/Full Pack 2025.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Tyre Squeal", "theScore",};

	/*textureName = { "tile1", "tile2", "tile3", "tile4", "tile5", "tile6", "menuBackground" };
	texturesToUse = { "Images/Tiles/1.png", "Images/Tiles/2.png", "Images/Tiles/3.png", "Images/Tiles/4.png", "Images/Tiles/5.png", "Images/Tiles/6.png", "Images/Bkg/MainMenu.png" };*/
	//for (int tCount = 0; tCount < textureName.size(); tCount++)
	//{
	//	theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	//}
	//tempTextTexture = theTextureMgr->getTexture("tile1");
	//aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	//aColour = { 228, 213, 238, 255 };
	// Create text Textures

	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("fullPack")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/quit_btn.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/menu_btn.png", "Images/Buttons/startbtn.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 32, 280 }, { 400, 300 }, { 400, 300 }, { 400, 260 }, { 32, 186 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}


	// Load game sounds
	soundList = { "theme", "pop" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/theme.mp3", "Audio/pop.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteEbkgd.setSpritePos({ 0, 0 });
	spriteEbkgd.setTexture(theTextureMgr->getTexture("endBackground"));
	spriteEbkgd.setSpriteDimensions(theTextureMgr->getTexture("endBackground")->getTWidth(), theTextureMgr->getTexture("endBackground")->getTHeight());

	spriteMbkgd.setSpritePos({ 0, 0 });
	spriteMbkgd.setTexture(theTextureMgr->getTexture("menuBackground"));
	spriteMbkgd.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
	
	
	playerCar.setSpritePos({ 500, 350 });
	playerCar.setTexture(theTextureMgr->getTexture("playerCar"));
	playerCar.setSpriteDimensions(theTextureMgr->getTexture("playerCar")->getTWidth(), theTextureMgr->getTexture("playerCar")->getTHeight());
	playerCar.setPlayerVelocity({ 0, 0 });

	// Create vector array of textures

	/*for (int astro = 0; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ 200 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) });
		theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 40.0f, 40.0f });
		theAsteroids[astro]->setActive(true);
	}*/

	for (int astro = 0; astro < petrolSpawn.size(); astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(petrolSpawn[astro]);
		//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		//int randAsteroid = rand() % 4;-
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture("asteroid1"));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture("asteroid1")->getTWidth(), theTextureMgr->getTexture("asteroid1")->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 3.0f,3.0f });
		theAsteroids[astro]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true; 

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}
void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
				 spriteMbkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
				 // Render Button
				 theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
				 theButtonMgr->getBtn("exit_btn")->setSpritePos({ 32, 260 });
				 theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
		break;
	
	case PLAYING:
	{
				SDL_RenderClear(theRenderer);
				spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

				theAreaClicked = { 0, 0 };
				// Render each asteroid in the vector array
				for (int draw = 0; draw < theAsteroids.size(); draw++)
				{
					theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
				}
				// Render the Title
				cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
				SDL_Rect pos = { 14, 18, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
				FPoint scale = { 1, 1 };
				tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
				//Render the scoreboard
				theTextureMgr->addTexture("theScore", theFontMgr->getFont("fullPack")->createTextTexture(theRenderer, to_string(theScore).c_str(), SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
				tempTextTexture = theTextureMgr->getTexture("theScore");
				pos = { 700, 18, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
				scale = { 1, 1 };
				tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
				// render the playerCar
				playerCar.render(theRenderer, &playerCar.getSpriteDimensions(), &playerCar.getSpritePos(), playerCar.getSpriteRotAngle(), &playerCar.getSpriteCentre(), playerCar.getSpriteScale());
				SDL_RenderPresent(theRenderer);
	}
		break;

	case END:
	{
				spriteEbkgd.render(theRenderer, NULL, NULL, spriteEbkgd.getSpriteScale());
				theButtonMgr->getBtn("menu_btn")->setSpritePos({ 200, 260 });
				theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
				theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 260 });
				theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
		break;
	case QUIT:
	{
				 loop = false;
	}
		break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Check button clicked and change state]
	if (theGameState == MENU || theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}
	else if (theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	}
	theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);

	
	// Update the visibility and position of each asteriod
	vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
	while (asteroidIterator != theAsteroids.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theAsteroids.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			++asteroidIterator;
		}
	}

	//Player Movement
	int xPos = playerCar.getSpritePos().x;
	int yPos = playerCar.getSpritePos().y;
	if (xPos < 50)
	{
		playerCar.setSpriteTranslation({ 0, 0 });
		playerCar.setSpritePos({ 50, yPos });
	}
	if (xPos > 880)
	{
		playerCar.setSpriteTranslation({ 0, 0 });
		playerCar.setSpritePos({ 880, yPos });
	}
	if (yPos < 20)
	{
		playerCar.setSpriteTranslation({ 0, 0 });
		playerCar.setSpritePos({ xPos, 20 });
	}
	if (yPos > 600)
	{
		playerCar.setSpriteTranslation({ 0, 0 });
		playerCar.setSpritePos({ xPos, 600});
	}

	// Update the visibility and position of each bullet
	//vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	//while (bulletIterartor != theBullets.end())
	//{
	//	if ((*bulletIterartor)->isActive() == false)
	//	{
	//		bulletIterartor = theBullets.erase(bulletIterartor);
	//	}
	//	else
	//	{
	//		(*bulletIterartor)->update(deltaTime);
	//		++bulletIterartor;
	//	}
	//}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/

	//for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	//{
	//	//(*bulletIterartor)->update(deltaTime);
	//	for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
	//	{
	//		if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
	//		{
	//			// if a collision set the bullet and asteroid to false
	//			(*asteroidIterator)->setActive(false);
	//		/*	theSoundMgr->getSnd("explosion")->play(0);*/
	//			//increment the scoreboard by 1
	//			theScore += 100;
	//			theTextureMgr->deleteTexture("theScore");
	//		}
	//	}
	//}

//Checks for collisions between the player and the petrol tanks
	for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
	{
		if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &playerCar.getBoundingRect()))
		{
			// if a collision set the petrol tank to false
			(*asteroidIterator)->setActive(false);
			theSoundMgr->getSnd("pop")->play(0);
			//increment the scoreboard by 100
			theScore += 100;
			theTextureMgr->deleteTexture("theScore");
		}
		if (theScore >= 2000)
		{
			theGameState = END;
		}

	}

	// Update the Rockets position
	playerCar.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;

				case SDLK_UP:
				{
								int xTrans = playerCar.getSpriteTranslation().x;
								int yTrans = playerCar.getSpriteTranslation().y;
								playerCar.setSpriteTranslation({ xTrans + 100, yTrans+ 100 });
				}
					break;
				case SDLK_RIGHT:
				{
								   playerCar.setSpriteRotAngle(playerCar.getSpriteRotAngle() + 10);
				}
					break;

				case SDLK_LEFT:
				{
								  playerCar.setSpriteRotAngle(playerCar.getSpriteRotAngle() - 10);
				}
					break;
				case SDLK_DOWN:
				{
								int xTrans = playerCar.getSpriteTranslation().x;
								int yTrans = playerCar.getSpriteTranslation().y;
								playerCar.setSpriteTranslation({ xTrans - 100, yTrans -100 });
				}
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

