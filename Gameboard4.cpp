#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;
using namespace sf;

VideoMode screenSize = VideoMode::getDesktopMode();
RenderWindow window(screenSize, "Game Board");

class Button {
public:
    Button(std::string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor, float offset = 1)
        : offset(offset) {
        text.setString(t);
        text.setFillColor(textColor);
        text.setCharacterSize(charSize);
        button.setSize(size);
        button.setFillColor(bgColor);
    }
    void setFont(sf::Font& font) {
        text.setFont(font);
    }
    void setBackColor(sf::Color color) {
        button.setFillColor(color);
    }
    void setTextColor(sf::Color color) {
        text.setFillColor(color);
    }
    void setPosition(sf::Vector2f pos) {
        button.setPosition(pos);
        float xPos = pos.x + (button.getLocalBounds().width - text.getLocalBounds().width) / 2;
        float yPos = pos.y + (button.getLocalBounds().height - text.getLocalBounds().height) / 2;
        // Adjusts the vertical position by subtracting a specific offset (e.g., 5.0f)
        yPos -= offset; // You can adjust this offset as needed
        text.setPosition(xPos, yPos);
    }
    Vector2f getPosition() const {
        return button.getPosition();
    }
    Vector2f getSize() const {
        return button.getSize();
    }
    void drawTo(sf::RenderWindow& window) {
        window.draw(button);
        window.draw(text);
    }
    bool buttonClicked(sf::RenderWindow& window) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (button.getPosition().x <= mousePos.x && mousePos.x <= button.getPosition().x + button.getSize().x && button.getPosition().y <= mousePos.y && mousePos.y <= button.getPosition().y + button.getSize().y) {
            return 1;
        }
        else
            return 0;
    }
private:
    sf::RectangleShape button;
    sf::Text text;
    float offset;
};
void highScore(const std::string& filename, const std::string& namedscoretxt, const std::string& name, int highscore) {
    std::fstream highscoreFile(filename, std::ios::in | std::ios::out);

    if (!highscoreFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line[5];
    int lineNumber = 1;

    while (lineNumber <= 5 && std::getline(highscoreFile, line[0])) {
        if (std::stoi(line[0]) == highscore)
            return;

        if (std::stoi(line[0]) < highscore)
            break;

        lineNumber++;
    }

    highscoreFile.seekg(0);

    for (int i = 0; i < 5; i++) {
        std::getline(highscoreFile, line[i]);
    }

    lineNumber--;

    if (lineNumber >= 1 && lineNumber <= 5)
        line[lineNumber] = std::to_string(highscore);

    highscoreFile.close();

    highscoreFile.open(filename, std::ios::out | std::ios::trunc);

    for (int i = 0; i < 5; i++) {
        highscoreFile << line[i] << std::endl;
    }
    highscoreFile.close();

    std::fstream namedHighscoreFile(namedscoretxt, std::ios::in); // Use std:: before ios

    if (!namedHighscoreFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    namedHighscoreFile.seekp(0);
    for (int i = 0; i < 5; i++) {
        line[i].clear();  // Clear the contents of the string
    }

    for (int i = 0; i < 5; i++) {
        getline(namedHighscoreFile, line[i]);
    }

    namedHighscoreFile.close();

    namedHighscoreFile.open(namedscoretxt, std::ios::out | std::ios::trunc);

    namedHighscoreFile.seekp(0);
    namedHighscoreFile << std::fixed << std::setprecision(2);  // Set formatting for floating-point numbers

    for (int i = 0; i < 5; i++) {
        if (lineNumber != i)
            namedHighscoreFile << line[i] << std::endl;
        else
            namedHighscoreFile << std::left << std::setw(20) << name << std::right << std::setw(10) << highscore << std::endl;
    }

    namedHighscoreFile.close();
}
int scorefunc(int base, int merged) {
    int power = 0;

    while (base * (pow(2, power)) <= merged) {
        if (base * (pow(2, power)) == merged)
            return 2 * (pow(2, power));
        power++;
    }

    // Assuming the condition is always true
    return 0;
}

bool isGameOver(int(*arr[]), int n) {
    // Check if the board is entirely filled
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (arr[i][j] == 0) {
                return false; // If any empty cell found, the game is not over
            }
        }
    }

    // Check if any adjacent elements in the same row/column are the same
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            // Check row-wise
            if (arr[i][j] == arr[i][j + 1]) {
                return false; // If any adjacent elements are the same, the game is not over
            }
            // Check column-wise
            if (arr[j][i] == arr[j + 1][i]) {
                return false; // If any adjacent elements are the same, the game is not over
            }
        }
    }

    // If no empty cells and no adjacent elements found, the game is over
    return true;
}

// Function to copy the current board to prevArr
void copyBoard(int(*prevArr[]), int(*arr[]), int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            prevArr[i][j] = arr[i][j];
        }
    }
}
// Function to check if the board has changed after movement
bool boardChanged(int(*prevArr[]), int(*arr[]), int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (prevArr[i][j] != arr[i][j]) {
                return true; // Board has changed
            }
        }
    }
    return false; // Board remains the same
}
void random1(int(*arr[]), int n, int multi) {
    while (true) {
        int r = rand() % n;
        int c = rand() % n;
        if (arr[r][c] == 0) {
            arr[r][c] = multi;
            break;
        }
    }
}
void mergeTilesud(int(*arr[]), int n, int multi, int& scoreValue) {
    for (int c = 0; c < n; ++c) {
        for (int r = 0; r < n - 1; ++r) {
            if (arr[r][c] != 0 && arr[r][c] == arr[r + 1][c]) {
                arr[r][c] *= 2;
                scoreValue += scorefunc(multi, arr[r][c]);
                arr[r + 1][c] = 0;
            }
        }
    }
}
void mergeTilesrl(int(*arr[]), int n, int multi, int& scoreValue) {
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n - 1; ++c) {
            if (arr[r][c] != 0 && arr[r][c] == arr[r][c + 1]) {
                arr[r][c] *= 2;
                scoreValue += scorefunc(multi, arr[r][c]);
                arr[r][c + 1] = 0;
            }
        }
    }
}
void moveTilesRight(int(*arr[]), int n) {
    for (int r = 0; r < n; r++) {
        int column = n - 1;
        for (int c = n - 1; c >= 0; c--) {
            if (arr[r][c] != 0) {
                arr[r][column] = arr[r][c];
                if (column != c) {
                    arr[r][c] = 0;
                }
                column--;
            }
        }
    }
}
void moveTilesLeft(int(*arr[]), int n) {
    for (int r = 0; r < n; ++r) {
        int column = 0;
        for (int c = 0; c < n; c++) {
            if (arr[r][c] != 0) {
                arr[r][column] = arr[r][c];
                if (column != c) {
                    arr[r][c] = 0;
                }
                column++;
            }
        }
    }
}
void moveTilesUp(int(*arr[]), int n) {
    for (int c = 0; c < n; c++) {
        int row = 0;
        for (int r = 0; r < n; r++) {
            if (arr[r][c] != 0) {
                arr[row][c] = arr[r][c];
                if (row != r) {
                    arr[r][c] = 0;
                };
                row++;
            }
        }
    }
}
void moveTilesDown(int(*arr[]), int n) {
    for (int c = 0; c < n; c++) {
        int row = n - 1;
        for (int r = n - 1; r >= 0; r--) {
            if (arr[r][c] != 0) {
                arr[row][c] = arr[r][c];
                if (row != r) {
                    arr[r][c] = 0;
                }
                row--;
            }
        }
    }
}
void upArrow(int(*prevArr[]), int(*arr[]), int n, int multi, int& scoreValue) {
    copyBoard(prevArr, arr, n);
    moveTilesUp(arr, n);
    mergeTilesud(arr, n, multi, scoreValue);
    moveTilesUp(arr, n);
    if (boardChanged(prevArr, arr, n)) {
        random1(arr, n, multi);
    }
}
void leftArrow(int(*prevArr[]), int(*arr[]), int n, int multi, int& scoreValue) {
    copyBoard(prevArr, arr, n);
    moveTilesLeft(arr, n);
    mergeTilesrl(arr, n, multi, scoreValue);
    moveTilesLeft(arr, n);
    if (boardChanged(prevArr, arr, n)) {
        random1(arr, n, multi);
    }
}
void rightArrow(int(*prevArr[]), int(*arr[]), int n, int multi, int& scoreValue) {
    copyBoard(prevArr, arr, n);
    moveTilesRight(arr, n);
    mergeTilesrl(arr, n, multi, scoreValue);
    moveTilesRight(arr, n);
    if (boardChanged(prevArr, arr, n)) {
        random1(arr, n, multi);
    }
}
void downArrow(int(*prevArr[]), int(*arr[]), int n, int multi, int& scoreValue) {
    copyBoard(prevArr, arr, n);
    moveTilesDown(arr, n);
    mergeTilesud(arr, n, multi, scoreValue);
    moveTilesDown(arr, n);
    if (boardChanged(prevArr, arr, n)) {
        random1(arr, n, multi);
    }
}
Color tileColor4x4(int(*arr[]), int r, int c, int multi) {
    if (arr[r][c] == multi * pow(2, 0))
        return Color(158, 174, 190);
    else if (arr[r][c] == multi * pow(2, 1))
        return Color(136, 160, 176);
    else if (arr[r][c] == multi * pow(2, 2))
        return Color(100, 124, 148);
    else if (arr[r][c] == multi * pow(2, 3))
        return Color(84, 108, 132);
    else if (arr[r][c] == multi * pow(2, 4))
        return Color(68, 100, 124);
    else if (arr[r][c] == multi * pow(2, 5))
        return Color(60, 84, 108);
    else if (arr[r][c] == multi * pow(2, 6))
        return Color(36, 68, 100);
    else if (arr[r][c] == multi * pow(2, 7))
        return Color(28, 60, 84);
    else if (arr[r][c] == multi * pow(2, 8))
        return Color(20, 44, 68);
    else if (arr[r][c] == multi * pow(2, 9))
        return Color(4, 36, 68);
    else if (arr[r][c] == multi * pow(2, 10))
        return Color(16, 20, 52);
    else
        return Color(212, 220, 220);
}
int calculateFontSize(int value) {
    // Define a base font size
    int baseFontSize = 50;
    // Calculate a dynamic font size based on the value
    int fontSize = baseFontSize - log2(value + 1) * 2; // Adjust the multiplication factor for your preference
    // Ensure the font size doesn't become too small
    if (fontSize < 15) {
        fontSize = 15; // Set a minimum font size to maintain readability
    }
    return fontSize;
}

string boardValues(int gridValue) {

    if (gridValue != 0)
        return to_string(gridValue);
    else
        return "";
}

int main() {
    
    ifstream file("highscores/unnamedhighscore4x4.txt");
    string highscore;
    getline(file, highscore);
    file.close();

    string username = "Hi";
    int multi = 2;
    
    srand(time(0));

    int** arr = new int* [4];
    for (int i = 0; i < 4; ++i) {
        arr[i] = new int[4];
        for (int j = 0; j < 4; ++j) {
            arr[i][j] = 0;
        }
    }

    int** prevArr = new int* [4];
    for (int i = 0; i < 4; ++i) {
        prevArr[i] = new int[4];
        for (int j = 0; j < 4; ++j) {
            prevArr[i][j] = 0;
        }
    }

    int r = rand() % 4;
    int c = rand() % 4;
    
    bool isGameover = false;
    bool runHighscore = false;
    int scoreValue = 0;

    arr[r][c] = multi;
    window.setFramerateLimit(60);


    while (window.isOpen()) {
        Event event; // Making an object "event" of the Event class
        window.clear();
        while (window.pollEvent(event)) // Loop to manage when something changes in the console
        {

            Button gameOver("GAME OVER", Vector2f(200, 200), 24, Color::Black, Color::White);
            Button name("2048", Vector2f(150, 100), 50, Color::Transparent, Color::Black, 6);
            Button boardbackground(" ", Vector2f(395, 440), 90, Color(8, 24, 56), Color::Black);
            Button b1(boardValues(arr[0][0]), Vector2f(90, 100), calculateFontSize(arr[0][0]), tileColor4x4(arr, 0, 0, 2), Color::Black, 16.0f);
            Button b2(boardValues(arr[0][1]), Vector2f(90, 100), calculateFontSize(arr[0][1]), tileColor4x4(arr, 0, 1, 2), Color::Black, 16.0f);
            Button b3(boardValues(arr[0][2]), Vector2f(90, 100), calculateFontSize(arr[0][2]), tileColor4x4(arr, 0, 2, 2), Color::Black, 16.0f);
            Button b4(boardValues(arr[0][3]), Vector2f(90, 100), calculateFontSize(arr[0][3]), tileColor4x4(arr, 0, 3, 2), Color::Black, 16.0f);
            Button b5(boardValues(arr[1][0]), Vector2f(90, 100), calculateFontSize(arr[1][0]), tileColor4x4(arr, 1, 0, 2), Color::Black, 16.0f);
            Button b6(boardValues(arr[1][1]), Vector2f(90, 100), calculateFontSize(arr[1][1]), tileColor4x4(arr, 1, 1, 2), Color::Black, 16.0f);
            Button b7(boardValues(arr[1][2]), Vector2f(90, 100), calculateFontSize(arr[1][2]), tileColor4x4(arr, 1, 2, 2), Color::Black, 16.0f);
            Button b8(boardValues(arr[1][3]), Vector2f(90, 100), calculateFontSize(arr[1][3]), tileColor4x4(arr, 1, 3, 2), Color::Black, 16.0f);
            Button b9(boardValues(arr[2][0]), Vector2f(90, 100), calculateFontSize(arr[2][0]), tileColor4x4(arr, 2, 0, 2), Color::Black, 16.0f);
            Button b10(boardValues(arr[2][1]), Vector2f(90, 100), calculateFontSize(arr[2][1]), tileColor4x4(arr, 2, 1, 2), Color::Black, 16.0f);
            Button b11(boardValues(arr[2][2]), Vector2f(90, 100), calculateFontSize(arr[2][2]), tileColor4x4(arr, 2, 2, 2), Color::Black, 16.0f);
            Button b12(boardValues(arr[2][3]), Vector2f(90, 100), calculateFontSize(arr[2][3]), tileColor4x4(arr, 2, 3, 2), Color::Black, 16.0f);
            Button b13(boardValues(arr[3][0]), Vector2f(90, 100), calculateFontSize(arr[3][0]), tileColor4x4(arr, 3, 0, 2), Color::Black, 16.0f);
            Button b14(boardValues(arr[3][1]), Vector2f(90, 100), calculateFontSize(arr[3][1]), tileColor4x4(arr, 3, 1, 2), Color::Black, 16.0f);
            Button b15(boardValues(arr[3][2]), Vector2f(90, 100), calculateFontSize(arr[3][2]), tileColor4x4(arr, 3, 2, 2), Color::Black, 16.0f);
            Button b16(boardValues(arr[3][3]), Vector2f(90, 100), calculateFontSize(arr[3][3]), tileColor4x4(arr, 3, 3, 2), Color::Black, 16.0f);
            Button newgame("New Game", Vector2f(100, 45), 15, Color(160, 82, 45), Color::White);
            Button score(to_string(scoreValue), Vector2f(100, 55), 14, Color(160, 82, 45), Color::White);
            Button best(highscore, Vector2f(100, 55), 14, Color(160, 82, 45), Color::White);

            Font font;
            font.loadFromFile("LEMONMILK.otf");
            gameOver.setFont(font);
            name.setFont(font);
            boardbackground.setFont(font);
            b1.setFont(font);
            b2.setFont(font);
            b3.setFont(font);
            b4.setFont(font);
            b5.setFont(font);
            b6.setFont(font);
            b7.setFont(font);
            b8.setFont(font);
            b9.setFont(font);
            b10.setFont(font);
            b11.setFont(font);
            b12.setFont(font);
            b13.setFont(font);
            b14.setFont(font);
            b15.setFont(font);
            b16.setFont(font);
            newgame.setFont(font);
            score.setFont(font);
            best.setFont(font);

            // Assigning the positions...
            gameOver.setPosition(Vector2f(200, 200));
            boardbackground.setPosition(Vector2f(370, 200));
            name.setPosition(Vector2f(370, 70));
            b1.setPosition(Vector2f(378, 210));
            b2.setPosition(Vector2f(474, 210));
            b3.setPosition(Vector2f(570, 210));
            b4.setPosition(Vector2f(666, 210));
            b5.setPosition(Vector2f(378, 316));
            b6.setPosition(Vector2f(474, 316));
            b7.setPosition(Vector2f(570, 316));
            b8.setPosition(Vector2f(666, 316));
            b9.setPosition(Vector2f(378, 422));
            b10.setPosition(Vector2f(474, 422));
            b11.setPosition(Vector2f(570, 422));
            b12.setPosition(Vector2f(666, 422));
            b13.setPosition(Vector2f(378, 528));
            b14.setPosition(Vector2f(474, 528));
            b15.setPosition(Vector2f(570, 528));
            b16.setPosition(Vector2f(666, 528));
            newgame.setPosition(Vector2f(666, 150));
            score.setPosition(Vector2f(566, 50));
            best.setPosition(Vector2f(672, 50));
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed) // Checking if mouse was clicked
            {
                if (event.mouseButton.button == Mouse::Left) // Checking if the left mouse button was clicked
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    if (name.buttonClicked(window)) // Using self defined function to check if the button was clicked
                    {
                        window.close(); // Closes the window
                    }
                }
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                    upArrow(prevArr, arr, 8, multi, scoreValue);
                    if (isGameOver(arr, 8)) {
                        isGameover = true;
                        runHighscore = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                    downArrow(prevArr, arr, 8, multi, scoreValue);
                    Button gameOver("GAME OVER", Vector2f(200, 200), 24, Color::Black, Color::White);
                    if (isGameOver(arr, 8)) {
                        isGameover = true;
                        runHighscore = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                    leftArrow(prevArr, arr, 8, multi, scoreValue);
                    if (isGameOver(arr, 8)) {
                        isGameover = true;
                        runHighscore = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                    rightArrow(prevArr, arr, 8, multi, scoreValue);
                    if (isGameOver(arr, 8)) {
                        isGameover = true;
                        runHighscore = true;
                    }
                }
            }

            window.clear(Color::White);
            name.drawTo(window);
            boardbackground.drawTo(window);
            b1.drawTo(window);
            b2.drawTo(window);
            b3.drawTo(window);
            b4.drawTo(window);
            b5.drawTo(window);
            b6.drawTo(window);
            b7.drawTo(window);
            b8.drawTo(window);
            b9.drawTo(window);
            b10.drawTo(window);
            b11.drawTo(window);
            b12.drawTo(window);
            b13.drawTo(window);
            b14.drawTo(window);
            b15.drawTo(window);
            b16.drawTo(window);
            newgame.drawTo(window);
            score.drawTo(window);
            best.drawTo(window);

            if (isGameover) {
                gameOver.drawTo(window);
            }
            if (runHighscore) {
                highScore("highscores/unnamedhighscore4x4.txt", "highscores/highscore4x4.txt", username, scoreValue);
                runHighscore = false;
            }
            window.display();
        }
    }
}
