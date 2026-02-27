#include <raylib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>


#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "TicTacToe"

#define NUM_OF_SQUARES (9)
#define SQUARE_SIZE (90.0f)
#define SQUARE_GAP (SQUARE_SIZE + 10.0f)
#define MAX_PER_ROW (3)


typedef enum TileValue
{
    DEFAULT_VALUE,
    CIRCLE,
    CROSS
} TileValue;

typedef enum PlayerTurn
{
    CIRCLE_TURN,
    CROSS_TURN
} PlayerTurn;

typedef struct Tile
{
    Rectangle tile;
    TileValue value;
} Tile;


typedef enum GameResult
{
    PLAYING,
    DRAW,
    WIN_CIRCLE,
    WIN_CROSS
} GameResult;

typedef struct GameState
{
    PlayerTurn playerTurn;
    GameResult gameResult;
} GameState;

GameState gameState = {CROSS_TURN, PLAYING};

Tile tiles[NUM_OF_SQUARES];

static void initTiles(void)
{
    for (int i = 0; i < NUM_OF_SQUARES; i++)
    {
        tiles[i].tile = (Rectangle) {0,0, SQUARE_SIZE, SQUARE_SIZE};
        tiles[i].value = DEFAULT_VALUE;

        // Align buttons in the grid
        const float currentSquareX = SQUARE_GAP * ((float) (i % MAX_PER_ROW) - (float) MAX_PER_ROW/2.0f);
        const float centerSquareX =  ((float) GetScreenWidth() / 2.0f) + currentSquareX;
        const float currentSquareY = floorf((float) i / (float) MAX_PER_ROW) * SQUARE_GAP;
        const float centerSquareY = ((float) GetScreenHeight() / 2.0f) + currentSquareY - (SQUARE_SIZE / 2.0f * (float) MAX_PER_ROW);

        tiles[i].tile.x = centerSquareX;
        tiles[i].tile.y = centerSquareY;
    }
}

static void resetTiles(void)
{
    for (int i = 0; i < NUM_OF_SQUARES; i++)
    {
        tiles[i].value = DEFAULT_VALUE;
    }
}

static void resetGame(void) {
    gameState.playerTurn = CROSS_TURN;
    gameState.gameResult = PLAYING;
    resetTiles();
}



static void checkWin(TileValue tile_value)
{
    const int NUM_OF_ROW = NUM_OF_SQUARES / MAX_PER_ROW;
    // 0 1 2
    // 3 4 5
    // 6 7 8

    bool threeInARowVertical = false;
    bool threeInARowHorizontal = false;

    for (int i = 0; i < NUM_OF_ROW; i++)
    {
        threeInARowHorizontal =
            ((tiles[i * MAX_PER_ROW + 0].value == tiles[i * MAX_PER_ROW + 1].value)
            && (tiles[i * MAX_PER_ROW + 1].value == tiles[i * MAX_PER_ROW + 2].value)
            && (tiles[i * MAX_PER_ROW + 2].value == tile_value));

        if (threeInARowHorizontal) break;

    }

    for (int i = 0; i < NUM_OF_ROW; i++)
    {
        threeInARowVertical =
         ((tiles[0 + i].value == tiles[3 + i].value)
         && (tiles[3 + i].value == tiles[6 + i].value)
         && (tiles[6 + i].value == tile_value));

        if (threeInARowVertical) break;
    }

    const bool firstDiagonal =
        ((tiles[0].value == tiles[4].value)
        && (tiles[4].value == tiles[8].value)
        && (tiles[8].value == tile_value));

    const bool secondDiagonal =
        ((tiles[2].value == tiles[4].value)
        && (tiles[4].value == tiles[6].value)
        && (tiles[6].value == tile_value));

    printf("%d\n", threeInARowHorizontal);

    if (firstDiagonal || secondDiagonal || threeInARowVertical || threeInARowHorizontal)
    {
        switch (tile_value)
        {
            case CIRCLE:
                gameState.gameResult = WIN_CIRCLE;
                break;
            case CROSS:
                gameState.gameResult = WIN_CROSS;
                break;
            default:
                break;
        }
    }
}


static void checkDraw(void) {
    bool hasDefault = false;
    for (int i = 0; i < NUM_OF_SQUARES; i++)
    {
        if (tiles[i].value == DEFAULT_VALUE)
        {
            hasDefault = true;
            break;
        }
    }

    if (!hasDefault)
    {
        gameState.gameResult = DRAW;
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    initTiles();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const Vector2 mousePosition = GetMousePosition();

        const bool endResult = (gameState.gameResult == DRAW || (gameState.gameResult == WIN_CIRCLE || gameState.gameResult == WIN_CROSS));

        if (!endResult)
        {
            checkWin(CIRCLE);
            checkWin(CROSS);

            if (gameState.gameResult != WIN_CIRCLE && gameState.gameResult != WIN_CROSS) {
                checkDraw();
            }
        }

        if (!endResult)
        {
            for (int i = 0; i < NUM_OF_SQUARES; i++)
            {
                if (CheckCollisionPointRec(mousePosition, tiles[i].tile) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    printf("%d\n", i);
                    if (tiles[i].value == DEFAULT_VALUE)
                    {
                        switch (gameState.playerTurn) {
                            case CIRCLE_TURN:
                                gameState.playerTurn = CROSS_TURN;
                                tiles[i].value = CIRCLE;
                                break;
                            case CROSS_TURN:
                                gameState.playerTurn = CIRCLE_TURN;
                                tiles[i].value = CROSS;
                                break;
                        }
                    }
                }
            }

        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (gameState.playerTurn)
        {
            case CROSS_TURN:
                DrawText(TextFormat("Player Turn: Cross"), 0, 0, 50, BLACK);
                break;
            case CIRCLE_TURN:
                DrawText(TextFormat("Player Turn: Circle"), 0, 0, 50, BLACK);
                break;
        }

        for (int i = 0; i < NUM_OF_SQUARES; i++)
        {
            switch (tiles[i].value)
            {
                case DEFAULT_VALUE:
                    DrawRectangleRec(tiles[i].tile, WHITE);
                    break;
                case CIRCLE:
                    DrawRectangleRec(tiles[i].tile, WHITE);
                    const Vector2 OSize = MeasureTextEx(GetFontDefault(), "O", 90, 1);
                    DrawText("O", (int) tiles[i].tile.x + (OSize.x / 2), (int) tiles[i].tile.y, 90, BLACK);
                    break;
                case CROSS:
                    DrawRectangleRec(tiles[i].tile, WHITE);
                    const Vector2 XSize = MeasureTextEx(GetFontDefault(), "X", 90, 1);
                    DrawText("X", tiles[i].tile.x + (XSize.x / 2), (int) tiles[i].tile.y, 90, BLACK);
                    break;
            }
        }

        if (endResult)
        {
            char titleText[12];

            switch (gameState.gameResult)
            {
                case WIN_CIRCLE:
                    strcpy(titleText, "Circle win!");
                    break;
                case WIN_CROSS:
                    strcpy(titleText, "Cross win!");
                    break;
                case DRAW:
                    strcpy(titleText, "Draw");
                default:
                    break;
            }

            const Font font = GetFontDefault();
            const float fontSize = 50.0f;

            const Vector2 size = MeasureTextEx(font, titleText, fontSize, 0);

            const float textXCenter = ((float)GetScreenWidth() - size.x) / 2.0f;
            const float textYCenter = ((float)GetScreenHeight() - size.y) / 2.0f;

            DrawTextEx(font, titleText, (Vector2){textXCenter, textYCenter - 175}, fontSize, 1, BLACK);

            if (GuiButton((Rectangle) {0,100, 100, 100}, "Reset Game"))
            {
               resetGame();
            }
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
