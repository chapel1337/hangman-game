#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>

// sleep function
#ifdef __linux__ || __unix__
#include <unistd.h>

#elif _WIN32
#include <windows.h>

void sleep(auto time)
{
    Sleep(time * 1000);
}
#endif

using std::cout; using std::cin; using std::string; using std::getline; using std::ws; using std::to_string; using std::vector; using std::ifstream; using std::ofstream;

// ------- \\

// written by chapel1337
// made on 10/27/2022
// started making this at school and actually got the word matching to work
// probably my second largest project(?) so far
// added save and load feature : )
// added some linux compability
// not sure what else to add here, this is just a simple hangman game
// 820 lines!!

// ------- \\

ofstream output{};
ifstream input{};

string chosenWord{};
string hiddenWord{};

bool finished{ false };
int tries{ 5 };

vector<string> presetWords
{
    "happy",
    "chimpanzees",
    "rabbit",
    "france",
    "true",
    "crazy",
    "dodgeball",
    "school",
    "awesome",
    "homework",
    "person",
    "website",
};
vector<string> halloweenWords
{
    "spooky",
    "halloween",
    "pumpkin",
    "ghost",
    "spirit",
    "killer",
    "consternation",
    "fright",
    "terror",
    "scary",
};
vector<string> loadedWords{};

vector<string> possibleWords { presetWords };

// ------- \\

void returnTo(string returnFunction);

void clear()
{
    #ifdef __linux__
      system("clear");
    #elif _WIN32
      system("cls");
    #endif
}

int getRandom(int minimum, int maximum)
{
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution random{ minimum, maximum };

    return random(mt);
}

void title(string title)
{
    SetConsoleTitleA(("hangman game - " + title).c_str());
}

void title()
{
    SetConsoleTitleA("hangman game");
}

void title(string title, int i)
{
    SetConsoleTitleA(("hangman game - " + title + to_string(i)).c_str());
}

void beep()
{
    Beep(200, 325); // accidently made a cool sfx, too bad beep() freezes the program while it's playing
}

void pause()
{
    cout << "press any key to continue . . .\n";

    cin.get();
    cin.ignore();
}

void invalidInput(string message, string returnFunction)
{
    clear();

    cout << "invalid input: " << message << '\n';

    sleep(2);
    returnTo(returnFunction);
}

// ------- \\

void youWin()
{
    clear();

    string response{};

    cout << "you win!\n";
    cout << "the word was: " << chosenWord << "\n\n";

    cout << "would you like to play again?\n\n";

    cout << "> ";
    getline(cin >> ws, response);

    if (response == "yes" || response == "y")
    {
        returnTo("start");
    }
    else if (response == "no" || response == "n")
    {
        returnTo("menu");
    }
    else
    {
        invalidInput("please specify yes or no", "youWin");
    }
}

void youLose()
{
    clear();

    string response{};

    cout << "you lose!\n";
    cout << "the word was: " << chosenWord << "\n\n";

    cout << "would you like to play again?\n\n";

    cout << "> ";
    getline(cin >> ws, response);

    if (response == "yes" || response == "y")
    {
        returnTo("start");
    }
    else if (response == "no" || response == "n")
    {
        returnTo("menu");
    }
    else
    {
        invalidInput("please specify yes or no", "youLose");
    }
}

void guessWord(string& input)
{
    // finally got it working!!

    bool correctGuess{ false };

    for (int i{ 0 }; i < chosenWord.length(); ++i)
    {
        if (chosenWord[i] == hiddenWord[i]) // to prevent the user from inputting a character that is already revealed
        {
            continue;
        }

        for (int o{ 0 }; o < input.length(); ++o)
        {
            if (chosenWord[i] == input[o] || toupper(chosenWord[i]) == toupper(input[o])) // case sensitivity is now not an issue
            {
                hiddenWord[i] = chosenWord[i];

                correctGuess = true;
            }
        }
    }

    if (!correctGuess)
    {
       --tries;
    }
}

void makeHiddenWord()
{
    for (int i{ 0 }; i < chosenWord.length(); ++i)
    {
        if (chosenWord[i] == ' ')
        {
            hiddenWord += ' ';
        }
        else
        {
            hiddenWord += '_';
        }
    }
}

string previousChosenWord{};

void setChosenWord()
{
    string randomWord{ possibleWords.at(getRandom(0, possibleWords.size() - 1)) }; // silly

    if (randomWord == previousChosenWord)
    {
        setChosenWord();
    }
    else
    {
        chosenWord = randomWord;
        previousChosenWord = chosenWord;
    }
}

// ------- \\

void start()
{
    // placeholders
    chosenWord = "";
    hiddenWord = "";

    // why the user would start despite having no words is beyond me; but just to make sure, i've made this precaution
    if (possibleWords.size() == 0)
    {
        clear();

        cout << "there are no current words!\n\n";
        cout << "> ";

        sleep(2);
        returnTo("menu");
    }

    setChosenWord();
    makeHiddenWord();

    while (!finished)
    {
        clear();

        if (chosenWord == hiddenWord)
        {
            tries = 5;
            youWin();
        }
        else if (tries <= 0)
        {
            tries = 5;
            youLose();
        }
        else
        {
            string input{};

            cout << "tries left: " << tries << "\n\n";
            cout << hiddenWord << "\n\n";

            cout << "input a letter or word: \n\n";

            cout << "> ";
            getline(cin >> ws, input);

            guessWord(input);
        }
    }
}

// ------- \\

void saveWords()
{
    clear();
    title("save words");

    string response{};

    cout << "are you sure you want to save your words?\n";
    cout << "note: this will wipe your current save\n\n";

    cout << "> ";
    getline(cin >> ws, response);

    if (response == "yes" || response == "y")
    {
        clear();

        output.open("save.txt");

        for (string word : possibleWords)
        {
            output << '!' << word << '\n';
        }

        output.close();

        cout << "successfully saved words to save.txt\n\n";
        cout << "> ";

        sleep(2);
        returnTo("saveLoadMenu");
    }
    else if (response == "no" || response == "n")
    {
        returnTo("saveLoadMenu");
    }
    else
    {
        invalidInput("please specify yes or no", "saveWords");
    }
}

void loadSave()
{
    clear();
    title("load save");

    string response{};

    cout << "are you sure you want to load a save?\n";
    cout << "note: this will unload your current loaded save\n\n";

    cout << "> ";
    getline(cin >> ws, response);

    if (response == "yes" || response == "y")
    {
        clear();

        string line{};

        loadedWords.clear();

        input.open("save.txt");

        while (getline(input, line))
        {
            if (line[0] == '!') // line must have ! to be recognized
            {
                line = line.substr(1, line.length()); // erases the !

                loadedWords.push_back(line);
            }
        }

        output.close();

        possibleWords.clear();
        possibleWords = loadedWords;

        cout << "successfully loaded words from save.txt\n\n";
        cout << "> ";

        sleep(2);
        returnTo("saveLoadMenu");
    }
    else if (response == "no" || response == "n")
    {
        returnTo("saveLoadMenu");
    }
    else
    {
        invalidInput("please specify yes or no", "loadSave");
    }
}

void help()
{
    clear();
    title("help");

    cout << "to save a file, select the save words option\n";
    cout << "it saves all of the words you currently have in a text file named \"save.txt\"\n\n";
    cout << "each word in the text file has an exclamation point (!) in front of it\n";
    cout << "if a line in the text file does not have an exclamation point, it is ignored by the program when loaded\n\n";

    cout << "to load a file, select the load save option\n";
    cout << "it loads the current text file named \"save.txt\" in the directory that the executable is located in\n";
    cout << "to add your own words in the text file, simply add a line and put an exclamation point(!) in front of it\n";
    cout << "remember: only one save file can be loaded at a time\n\n";

    cout << "if confused, use your brain\n";
    cout << "if still confused, contact me (my contacts are at https://chapel1337.github.io/contacts) \n\n";

    pause();
    returnTo("saveLoadMenu");
}

// ------- \\

void saveLoadMenu()
{
    clear();
    title("save/load menu");

    char response{};

    cout << "save/load menu\n\n";

    cout << "1. save words\n";
    cout << "2. load save\n";
    cout << "3. help\n";
    cout << "4. back\n\n";

    cout << "> ";
    cin >> response;

    switch (response)
    {
    case '1':
        saveWords();
        break;

    case '2':
        loadSave();
        break;

    case '3':
        help();
        break;

    case '4':
        returnTo("settings");
        break;

    default:
        invalidInput("please specify 1, 2, 3 or 4", "saveLoadMenu");
        break;
    }
}

// ------- \\

void createWord()
{
    clear();
    title("createWord");

    string response{};

    cout << "input a word: \n\n";

    cout << "> ";
    getline(cin >> ws, response);

    possibleWords.push_back(response);

    clear();

    cout << "word: " << response << '\n';

    cout << "successfully added word\n\n";
    cout << "> ";

    sleep(2);
    returnTo("wordsMenu");
}

void deleteWord()
{
    clear();
    title("delete word");

    int response{};

    for (int i{ 0 }; i < possibleWords.size(); ++i)
    {
        cout << i << ". " << possibleWords[i] << '\n';
    }

    cout << "\ninput a word: \n\n";

    cout << "> ";
    cin >> response;

    possibleWords.erase(possibleWords.begin() + response - 1);

    clear();

    cout << "word: " << possibleWords[response - 1] << '\n';

    cout << "successfully deleted word\n\n";
    cout << "> ";

    sleep(2);
    returnTo("wordsMenu");
}

void clearCurrentWords()
{
    clear();
    title("clear current words");

    string response{};

    cout << "are you sure you want to clear the current words?\n\n";

    cout << "> ";
    getline(cin >> ws, response);

    if (response == "yes" || response == "y")
    {
        clear();

        possibleWords.clear();

        cout << "successfully cleared all the current words\n\n";
        cout << "> ";

        sleep(2);
        returnTo("wordsMenu");
    }
    else if (response == "no" || response == "n")
    {
        returnTo("wordsMenu");
    }
    else
    {
        invalidInput("please specify yes or no", "wordsMenu");
    }
}

void listCurrentWords()
{
    clear();
    title("list of current words");

    for (int i{ 0 }; i < possibleWords.size(); ++i)
    {
        cout << i + 1 << ". " << possibleWords[i] << '\n';
    }
    cout << '\n';

    pause();
    returnTo("wordsMenu");
}

// ------- \\

void wordsMenu()
{
    clear();
    title("words menu");

    char response{};

    cout << "words menu\n\n";

    cout << "1. create word\n";
    cout << "2. delete word\n";
    cout << "3. clear current words\n";
    cout << "4. list current words\n";
    cout << "5. back\n\n";

    cout << "> ";
    cin >> response;

    switch (response)
    {
    case '1':
        createWord();
        break;

    case '2':
        deleteWord();
        break;

    case '3':
        clearCurrentWords();
        break;

    case '4':
        listCurrentWords();
        break;

    case '5':
        returnTo("settings");
        break;

    default:
        invalidInput("please specify 1, 2, 3, 4, or 5", "wordsMenu");
        break;
    }
}

// ------- \\

bool halloweenWordsEnabled{ false };

void toggleHalloweenWords()
{
    clear();
    title("halloween words");

    if (halloweenWordsEnabled)
    {
        possibleWords.clear();

        if (loadedWords.size() > 0)
        {
            possibleWords = loadedWords;
        }
        else
        {
            possibleWords = presetWords;
        }

        cout << "disabled halloween words\n\n";
        cout << "> ";

        halloweenWordsEnabled = false;
    }
    else
    {
        possibleWords.clear();
        possibleWords = halloweenWords;

        cout << "enabled halloween words\n\n";
        cout << "> ";

        halloweenWordsEnabled = true;
    }

    sleep(2);
    returnTo("settings");
}

// ------- \\

void settings()
{
    clear();
    title("settings");

    char response{};

    cout << "settings\n\n";

    cout << "1. save/load menu\n";
    cout << "2. words menu\n";
    cout << "3. toggle halloween words\n";
    cout << "4. back\n\n";

    cout << "> ";
    cin >> response;

    switch (response)
    {
    case '1':
        saveLoadMenu();
        break;

    case '2':
        wordsMenu();
        break;

    case '3':
        toggleHalloweenWords();
        break;

    case '4':
        returnTo("menu");
        break;

    default:
        invalidInput("please specify 1, 2, 3, or 4", "settings");
        break;
    }
}

void quit()
{
    clear();

    for (int i{ 3 }; i > 0; --i)
    {
        title("exiting in ", i);

        cout << "okay, exiting in " << i;

        sleep(1);
        clear();
    }

    exit(1);
}

// ------- \\

void menu()
{
    clear();
    title("menu");

    char response{};

    cout << "hangman game\n\n";

    cout << "1. start\n";
    cout << "2. settings\n";
    cout << "3. quit\n\n";

    cout << "> ";
    cin >> response;

    switch (response)
    {
    case '1':
        start();
        break;

    case '2':
        settings();
        break;

    case '3':
        quit();
        break;

    default:
        invalidInput("please specify 1, 2, or 3", "menu");
    }
}

void returnTo(string returnFunction)
{
    if (returnFunction == "menu")
    {
        menu();
    }
    else if (returnFunction == "settings")
    {
        settings();
    }
    else if (returnFunction == "saveLoadMenu")
    {
        saveLoadMenu();
    }
    else if (returnFunction == "wordsMenu")
    {
        wordsMenu();
    }

    else if (returnFunction == "start")
    {
        start();
    }
    else if (returnFunction == "youWin")
    {
        youWin();
    }
    else if (returnFunction == "youLose")
    {
        youLose();
    }

    else if (returnFunction == "saveWords")
    {
        saveWords();
    }
    else if (returnFunction == "loadSave")
    {
        loadSave();
    }
}

// ------- \\

int main()
{
    cout << "written by chapel1337\n";
    cout << "made on 10/27/2022\n";

    sleep(2);

    beep(); // to subtly notify the user that the program has started
    menu();
}

// ------- \\