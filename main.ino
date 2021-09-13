#include <LiquidCrystal.h>
LiquidCrystal lcd(2, A5, A4, A3, A2, A1);

const int whiteButton = 8;
const int yellowButton = 9;
const int greenButton = 10;
const int redButton = 11;
const int blueButton = 12;
const int soundPin = 13;
const int restartButton = A0;

const int whiteLED = 3;
const int yellowLED = 4;
const int greenLED = 5;
const int redLED = 6;
const int blueLED = 7;

int level = 0;
int highestScore = 0;
int currentMode = 4;

bool playing = false;
bool lost = false;
int *target = 0;
int notes[] = {261, 293, 329, 349, 391};

const int difficulty = 1;

void dealWithUserInput();
void generateTarget();
int getUserInput();
void flash();
void turnAllLEDsOn();
void turnAllLEDsOff();

void setup()
{
    lcd.begin(16, 2);
    displayOnLCD(0);

    for (int i = 0; i < 5; ++i)
    {
        pinMode(whiteButton + i, INPUT);
        pinMode(whiteLED + i, OUTPUT);
    }

    pinMode(restartButton, INPUT);
    randomSeed(analogRead(0));

    turnAllLEDsOn();
}

void loop()
{
    if (lost)
    {
        playing = false;
        displayOnLCD(1);
        turnAllLEDsOn();
    }
    else if (playing)
    {
        generateTarget();
        dealWithUserInput();
    }

    if (!playing && digitalRead(restartButton))
    {
        level = 0;
        displayOnLCD(2);
        turnAllLEDsOff();
        playing = true;
        lost = false;
        delay(500);
    }
}

void generateTarget()
{
    if (target)
    {
        delete[] target;
        target = 0;
    }
    target = new int[level + difficulty];

    for (int i = 0; i < level + difficulty; ++i)
    {
        target[i] = random(5);
        digitalWrite(target[i] + 3, HIGH);
        tone(soundPin, notes[target[i]]);
        delay(500);
        digitalWrite(target[i] + 3, LOW);
        noTone(soundPin);
        delay(500);
    }
    displayOnLCD(3);
}

void dealWithUserInput()
{
    int clickedIndex;
    for (int i = 0; i < level + difficulty; ++i)
    {
        clickedIndex = getUserInput();
        if (target[i] != clickedIndex)
        {
            lost = true;
            break;
        }
    }
    if (!lost)
    {
        ++level;
        if (highestScore < level)
        {
            highestScore = level;
        }
        flash();
        displayOnLCD(2);
        flash();
        delay(500);
    }
}

int getUserInput()
{
    int clicked = 5;
    int released = 5;

    while (true)
    {
        if (digitalRead(whiteButton))
        {
            digitalWrite(whiteLED, HIGH);
            tone(soundPin, notes[0]);
            clicked = 0;
        }
        else if (clicked == 0)
        {
            digitalWrite(whiteLED, LOW);
            released = 0;
        }

        if (digitalRead(yellowButton))
        {
            digitalWrite(yellowLED, HIGH);
            tone(soundPin, notes[1]);
            clicked = 1;
        }
        else if (clicked == 1)
        {
            digitalWrite(yellowLED, LOW);
            released = 1;
        }

        if (digitalRead(greenButton))
        {
            digitalWrite(greenLED, HIGH);
            tone(soundPin, notes[2]);
            clicked = 2;
        }
        else if (clicked == 2)
        {
            digitalWrite(greenLED, LOW);
            released = 2;
        }

        if (digitalRead(redButton))
        {
            digitalWrite(redLED, HIGH);
            tone(soundPin, notes[3]);
            clicked = 3;
        }
        else if (clicked == 3)
        {
            digitalWrite(redLED, LOW);
            released = 3;
        }

        if (digitalRead(blueButton))
        {
            digitalWrite(blueLED, HIGH);
            tone(soundPin, notes[4]);
            clicked = 4;
        }
        else if (clicked == 4)
        {
            digitalWrite(blueLED, LOW);
            released = 4;
        }
        if (clicked != 5 && clicked == released)
        {
            noTone(soundPin);
            break;
        }
    }
    return clicked;
}

void flash()
{
    turnAllLEDsOn();
    delay(500);
    turnAllLEDsOff();
    delay(500);
}

void displayOnLCD(int mode)
{
    if (mode != currentMode)
    {
        lcd.clear();
        if (mode == 0)
        {
            lcd.print("Welcome to my");
            lcd.setCursor(0, 1);
            lcd.print("memory game");
        }
        else if (mode == 1)
        {
            lcd.print("You lost");
            lcd.setCursor(0, 1);
            lcd.print("S: ");
            lcd.print(level);
            lcd.print("   HS: ");
            lcd.print(highestScore);
        }
        else if (mode == 2)
        {
            lcd.print("Watch & listen");
            lcd.setCursor(0, 1);
            lcd.print("S: ");
            lcd.print(level);
            lcd.print("   HS: ");
            lcd.print(highestScore);
        }
        else if (mode == 3)
        {
            lcd.print("Your turn!");
            lcd.setCursor(0, 1);
            lcd.print("S: ");
            lcd.print(level);
            lcd.print("   HS: ");
            lcd.print(highestScore);
        }
        currentMode = mode;
    }
}

void turnAllLEDsOn()
{
    digitalWrite(whiteLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, HIGH);
}

void turnAllLEDsOff()
{
    digitalWrite(whiteLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
}
