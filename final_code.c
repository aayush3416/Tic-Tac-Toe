//CONSTANTS
const int BOARD_HEIGHT_LENGTH = 3;
const int CENTERTILE = 4;
const int TILESONBOARD = 9;
//ADJUST TILE_LENGTH AND LENGTH_BETWEEN_TILES FOR BETTER ACCURACY
const float TILE_LENGTH = 9;
const float LENGTH_BETWEEN_TILES = 7;
const float TURN_ADJUSTMENT_DIST = 3.6;
const float TILE_AND_BETWEEN = TILE_LENGTH + LENGTH_BETWEEN_TILES;
const float DISPENSER_ADJUST = 14;
const float EDGE_TO_EDGE = 4 * TILE_LENGTH + 2 * LENGTH_BETWEEN_TILES;
const float REAL_BOARD_LENGTH = 49.5;
const float CORRECTION_FACTOR = 2.2;
const float CORRECTION_FACTOR_GYRO = 2.8;
const float DIST_BETWEEN_WHEELS = 9;
//This function initializes all the sensors
void sensorsetup()
{
#define GYRO_SENSOR_PORT S4
#define COLOR_SENSOR_PORT S3

	SensorType[GYRO_SENSOR_PORT] = sensorEV3_Gyro;
	SensorType[COLOR_SENSOR_PORT] = sensorEV3_Color;

	SensorMode[COLOR_SENSOR_PORT] = modeEV3Color_Color;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

void driveDist(float distance)
{
	resetGyro(S4);
	const int SPEED = 20;
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorD] = 0;
	float ENC_LIMIT =  distance * 180 / (PI * 2.75);
	int error_dist = 0;

	while(fabs(nMotorEncoder[motorA] + nMotorEncoder[motorD])/2 < ENC_LIMIT)
	{
		int read_gyro_value = getGyroDegrees(S4);
		motor[motorA] = SPEED-error_dist;
		motor[motorD] = SPEED+error_dist;
		error_dist = (nMotorEncoder[motorA] - nMotorEncoder[motorD])* CORRECTION_FACTOR;
		if(fabs(read_gyro_value) > 5)
			error_dist += read_gyro_value * CORRECTION_FACTOR_GYRO;
	}
	motor[motorA] = motor[motorD] = 0;
}

void driveToEdge()
{
	motor[motorA] = motor[motorD] = 10;
	while (SensorValue[COLOR_SENSOR_PORT] == 6)
	{}

	motor[motorA] = motor[motorD] = 0;
}
//Start up
char startup()
{
	displayString(5, "Press left button for Easy");
	displayString(6, "Press Center button for Medium");
	displayString(7, "Press Right button for Hard");
	char difficulty;

	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{

		if(getButtonPress(buttonLeft))
		{
			difficulty = 'E';
		} else if (getButtonPress(buttonEnter))
		{
			difficulty = 'M';
		} else if (getButtonPress(buttonRight))
		{
			difficulty = 'H';
		}
	}
	eraseDisplay();
	displayString(5, "Make sure the robot is aligned");
	displayString(6, "on the starting line and press");
	displayString(7, "the enter button to begin your");
	displayString(8, "turn.");

	while(!getButtonPress(buttonEnter))
	{}
	while (getButtonPress(buttonEnter))
	{}

	eraseDisplay();
	return difficulty;
}
// ShutDown

void shutDown(int outcome)
{
	if (outcome == 1)
	{
		//player wins
		displayString(5, "Congratulations, you have won!");
	}
	else if (outcome == 2)
	{
		//robot wins
		displayString(5, "The robot has beaten you!");
	}
	else if (outcome == 3)
	{
		//tie game
		displayString(5, "Tie Game!");
	}
	else if (outcome == 0)
	{
		//Cheating
		displayString(5, "The player has won");
	}
	displayString(6, "Press enter to end the program");
	while (!getButtonPress(buttonEnter))
	{}
	while (getButtonPress(buttonEnter))
	{}
}

//This function will drive the robot forwards based on the length of our board
void driveTile(float scalar)
{
	const float BOARD_LENGTH = 16;
	const float ENC_LIMIT = scalar * BOARD_LENGTH * 180 / (PI * 2.75);
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = 20;
	while(abs(nMotorEncoder[motorA]) < ENC_LIMIT)
	{}
	motor[motorA] = motor[motorD] = 0;
}

int getState()
{

	if (getColorName(COLOR_SENSOR_PORT) == 3)
		return 1;
	if (getColorName(COLOR_SENSOR_PORT) == 5)
		return 2;
	return 0;
}

//This will rotate our robot 90 degrees
void rotate90Clockwise()
{
	const float ANGLE = 88.5;
	resetGyro(S4);
	float motorSpeed = 10;

	while (getGyroDegrees(S4) < ANGLE)
	{
		motor[motorA] = -(motorSpeed * ((1.1*ANGLE - getGyroDegrees(S4))/ANGLE));
		motor[motorD] = (motorSpeed * ((1.1*ANGLE - getGyroDegrees(S4))/ANGLE));
	}
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);
}

// Once it sees the black color, it will keep moving forward until it no longer senses the black
// it will come to a stop
void onceBlack()
{
	motor[motorA] = motor[motorD] = 10;
	while (SensorValue[COLOR_SENSOR_PORT] == 1)
	{}
	motor[motorA] = motor[motorD] = 0;
}

//This will dipense the tiles using our third motor

void dispenseTile()
{
	float card_rad = 4;
	float card_circumference = 25.13;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 0;

	float ENC_LIMIT = card_circumference * 360 / (2 * PI * card_rad);

	while (nMotorEncoder[motorB] > -ENC_LIMIT)
	{
		motor[motorB] = -18;
	}
	motor[motorB] = 0;
}



// ----------------------------- NON_TRIVIAL + REQUIRED 6 FUNCTIONS -----------------------------------

// This function will choose any EMPTY random tile, and return the index of it
int chooseTile(int*board) {
	const int TILESONBOARD = 9;
	int emptyTiles = 0;
	for (int index = 0; index < TILESONBOARD; index++) {
		if (board[index] == 0) {
			emptyTiles++;
		}
	}
	int rand = random(emptyTiles-1);
	int count = 0;
	while (rand >= 0) {
		if (board[count] == 0) {
			rand--;
		}
		count++;
	}
	count--;
	if (count == 4 && emptyTiles != 1) {
		return chooseTile(board);
	}
	return count;
}

//After the robot scans the board, the robot will determine whether the player has plced the tile in the center or as made an invalid move.
// It returns true if the move was valid
// Returns false if the move is invalid.
bool compareBoards(int * newBoard, int * oldBoard)
{
	int differences = 0;

	for(int index = 0; index<TILESONBOARD; index++)
		if (newBoard[index]!= oldBoard[index])
		differences++;

	// if there are no difference, check center tile
	if(differences ==0)
	{
		if(oldBoard[CENTERTILE] ==0)
		{
			//the center tile was initially empty, so the player placed a tile there
			newBoard[CENTERTILE] = 1;
		}
		else
		{
			// The center line was already fiilled and there were no differences so the player did not place a tile		return false;
			return false;
		}
	}
	if(differences > 1)
	{
		return false;
	}
	return true;
}


//This function will display the player's turn timer on the screen and make sure it finishes according to difficulty
bool timePlayer(char difficulty)
{
	const int EASY = 20;
	const int MEDIUM = 10;
	const int HARD = 5;

	int max_time = 0;

	if (difficulty == 'E')
	{
		max_time = EASY;
	}
	if (difficulty == 'M')
	{
		max_time = MEDIUM;
	}

	if (difficulty == 'H')
	{
		max_time = HARD;
	}

	clearTimer(T1);
	eraseDisplay();
	displayString(5, "Press the Down button when");
	displayString(6, "you are done your turn");

	while (time1[T1] / 1000 < max_time)
	{
		displayString(7, "You Have %d Seconds Left", max_time - time1[T1] / 1000);
		if (getButtonPress(buttonDown))
		{
			eraseDisplay();
			return true;
		}

	}
	eraseDisplay();
	return false;
}


//This fucntion will determine whether the game is tied, if an player has won, or computer has won
// and will return an integer value accordingly

int evaluateBoard(int* board) {
	//check to see which player won the game, or tied, or incomplete.

	//first check columns
	for (int column = 0; column < BOARD_HEIGHT_LENGTH; column++) {
		if (board[column] != 0 && board[column] == board[column + BOARD_HEIGHT_LENGTH] && board[column] == board[column + BOARD_HEIGHT_LENGTH*2]) {
			return board[column];
		}
	}
	//now check rows
	for (int row = 0; row < BOARD_HEIGHT_LENGTH; row++) {
		if (board[BOARD_HEIGHT_LENGTH*row] != 0 && board[BOARD_HEIGHT_LENGTH*row] == board[BOARD_HEIGHT_LENGTH*row + 1] && board[BOARD_HEIGHT_LENGTH*row] == board[BOARD_HEIGHT_LENGTH*row + 2]) {
			return board[BOARD_HEIGHT_LENGTH*row];
		}
	}
	//special case: check both diagonals
	if (board[0] != 0 && board[0] == board[4] && board[0] == board[8]) {
		return board[0];
	}
	if (board[2] != 0 && board[2] == board[4] && board[2] == board[6]) {
		return board[2];
	}
	//If none of those were true, return 0 for inclomplete game
	return 0;
}

void driveAndScan(int * board, int index) {
	board[index] = getState();
	driveDist(TILE_AND_BETWEEN);
}


//This function will drive around the board and scan the board for empty tiles
void scanBoard(int * board, int *emptyBoard)
{
	//Since the center cannot be scanned, we will set it to its previous state
	emptyBoard[4] = board[4];

	//SCAN IN ORDER: 6, 3, 0, 1, 2, 5, 8, 7
	driveToEdge();
	driveDist(0.5*TILE_LENGTH);
	driveAndScan(emptyBoard, 6);
	driveAndScan(emptyBoard, 3);

	board[0] = getState();
	driveDist(0.5*TILE_LENGTH);

	//turn
	driveDist(TILE_LENGTH);
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
	driveToEdge();
	driveDist(0.5*TILE_LENGTH);
	driveDist(TILE_AND_BETWEEN);

	driveAndScan(emptyBoard, 1);
	board[2] = getState();
	driveDist(0.5*TILE_LENGTH);

	//turn
	driveDist(TILE_LENGTH);
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
	driveToEdge();
	driveDist(0.5*TILE_LENGTH);
	driveDist(TILE_AND_BETWEEN);

	driveAndScan(emptyBoard, 5);
	board[8] = getState();
	driveDist(0.5*TILE_LENGTH);

	//turn
	driveDist(TILE_LENGTH);
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
	driveToEdge();
	driveDist(0.5*TILE_LENGTH);
	driveDist(TILE_AND_BETWEEN);

	driveAndScan(emptyBoard, 7);
	driveDist(0.5*TILE_LENGTH);

	//turn
	driveDist(TILE_LENGTH);
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
	driveToEdge();

}


void driveAndPlace(int rowPos)
{
	driveToEdge();
	driveDist(TILE_AND_BETWEEN * rowPos);
	//Adjust this driveDist length so the dispenser lines up with the slot

	driveDist(DISPENSER_ADJUST);
	dispenseTile();
	driveDist(TILE_AND_BETWEEN * (BOARD_HEIGHT_LENGTH-(rowPos)) - DISPENSER_ADJUST + TILE_LENGTH - LENGTH_BETWEEN_TILES);
	//at this point, the front of the robot is dispenserAdjust in front of the line.
	//Drive further if necessary so that the robot turns and ends up at the edge of the tile
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
}

void driveRow()
{
	driveToEdge();
	driveDist(EDGE_TO_EDGE);
	//Drive adjustment distance here
	driveDist(TURN_ADJUSTMENT_DIST);
	rotate90Clockwise();
}

// This function will drive over to appropriate position and call the dispense tile function and then drive to original spot
void placeTile(int tileIndex, int * board)
{
	driveToEdge();
	board[tileIndex] = 2;
	//rowPos is the position of a tile on a side, clockwise. For example on the left side, rowPos 0 is the bottom left corner and 2 is the top left corner.
	int rowPos = 0;

	// Center tile special case
	if (tileIndex == 4)
	{
		driveDist(TILE_AND_BETWEEN + TILE_LENGTH);
		driveDist(TURN_ADJUSTMENT_DIST);
		rotate90Clockwise();
		driveDist(TILE_AND_BETWEEN);
		driveDist(DISPENSER_ADJUST);
		dispenseTile();
		driveDist(TILE_AND_BETWEEN + TILE_LENGTH);
		driveDist(TURN_ADJUSTMENT_DIST - DISPENSER_ADJUST);
		rotate90Clockwise();
		driveDist(TILE_AND_BETWEEN + TILE_LENGTH);
		driveDist(TURN_ADJUSTMENT_DIST);
		rotate90Clockwise();
		driveRow();
	}

	else
	{

		//determine what side the of the board the robot is on
		//Left Side
		if(tileIndex % 3 == 0)
		{
			rowPos = 2 - (tileIndex/3);
			driveAndPlace(rowPos);
			driveRow();
			driveRow();
			driveRow();
		}

		//Top row
		else if (tileIndex <= 2)
		{
			rowPos = tileIndex;
			driveRow();
			driveAndPlace(rowPos);
			driveRow();
			driveRow();
		}

		//Right Side Column
		else if (tileIndex % 3 == 2)
		{
			rowPos = (tileIndex + 1)/ 3 - 1;
			driveRow();
			driveRow();
			driveAndPlace(rowPos);
			driveRow();
		}

		//Tile 7s
		else
		{
			rowPos = 1;
			driveRow();
			driveRow();
			driveRow();
			driveAndPlace(rowPos);
		}
	}
}
task main()
{
	sensorsetup();
	driveToEdge();


	char difficulty = startup();
	bool is_game_on = true;
	int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	bool cheating = false;
	int outcome = 0;

	while (is_game_on)
	{
		if (!timePlayer(difficulty))
		{
			is_game_on = false;
		}
		if (is_game_on)
		{
			int tempBoard[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
			scanBoard(board, tempBoard);
			wait1Msec(6000);
			cheating = !compareBoards(tempBoard,board);
			board = tempBoard;
			outcome = evaluateBoard(board);
		}

		if (outcome != 0 || cheating == true)
		{
			is_game_on = false;
		}
		if (is_game_on)
		{
			int tile = (chooseTile(board));
			placeTile(tile, board);
		}
		outcome = evaluateBoard(board);
		if (outcome != 0)
		{
			is_game_on = false;
		}
	}

	shutDown(outcome);

}
