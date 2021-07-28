#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
//void updatePageTwo(int number, sf::Font, vector<sf::Text> &vec);
bool checkNum(int start, int end, string number);
void StoreImage(string name, sf::Texture& text, map <string, sf::Texture>& imageStorage);
int main()
{
    //flags
    int page = 0;
    int visible1 = 0;
    int visibleMonth = 0;
    int visibleDay = 0;
    int visibleYear = 0;
    bool contained = false;
    bool containedMonth = false;
    bool containedDay = false;
    bool containedYear = false;
    bool deathBox = false;
    bool caseBox = false;

    string numState = "";
    string numMonth = "";
    string numYear = "";
    string numDay = "";


    vector<sf::Text> textVec;
    vector<sf::Text> textVec2;
    vector<sf::Text> textVec3;
    vector<sf::Text> textVec4;


    //start up window
    int length = 1920;
    int width = 1000;
    sf::RenderWindow window(sf::VideoMode(length, width), "Covid Tracker", sf::Style::Titlebar | sf::Style::Close);
    // tgui::Gui gui(window);
     //load main text font
    sf::Font font;
    font.loadFromFile("Arvo-Regular.ttf");
    sf::Font bold;
    bold.loadFromFile("Arvo-Bold.ttf");

    map<string, sf::Texture> imageStorage;
    sf::Vector2u windowSize = window.getSize();

    //set up all the textures 
    sf::Texture backText;
    backText.loadFromFile("Background.jpg");
    StoreImage("Background", backText, imageStorage);


    //sf::Texture white;
    //white.loadFromFile("images/white.jpg");
    //StoreImage("White", white, imageStorage);

    sf::RectangleShape top(sf::Vector2f(1920, 100));
    top.setPosition(0, 0);
    top.setFillColor(sf::Color(144, 238, 144, 180));

    sf::Sprite background(imageStorage["Background"]);

    sf::Texture answerBox;
    answerBox.loadFromFile("images/RectangleWithBlackOutline.jpg");
    StoreImage("AnswerBox", answerBox, imageStorage);

    //fill the entire window will a background image
    sf::Vector2u textureBackSize = backText.getSize();
    //found scaling in a sfml tutorial that scales images to the size of the screen on stack overflow
    float scaleBackX = (float)windowSize.x / textureBackSize.x;
    float scaleBackY = (float)windowSize.y / textureBackSize.y;
    background.setScale(scaleBackX, scaleBackY);
    //set the top rectange of the window
      //  sf::Sprite top(imageStorage["White"]);
      //  sf::Vector2u textureWhiteSize = white.getSize();
      //  float scaleWhiteX = (float)windowSize.x / textureWhiteSize.x;
        //make it transparent
        //top.setColor(sf::Color(255, 255, 255, 200));
       // top.setScale(scaleWhiteX, .15);
        //add title
    sf::Text title;
    title.setString("Covid Tracker");
    title.setFont(bold);
    title.setCharacterSize(35);
    title.setFillColor(sf::Color::Blue);
    title.setPosition(750, 30);


    //make the first answering space
    sf::RectangleShape answerNumStates(sf::Vector2f(170, 40));
    answerNumStates.setPosition(1100, 260);
    answerNumStates.setOutlineColor(sf::Color::Black);
    answerNumStates.setOutlineThickness(0);


    sf::Text questionOne;
    questionOne.setString("Please enter number of states to display:");
    questionOne.setFont(font);
    questionOne.setCharacterSize(40);
    questionOne.setFillColor(sf::Color::Black);
    questionOne.setPosition(280, 250);

    sf::Text constraint1;
    constraint1.setString("Enter 1-50 only");
    constraint1.setFont(font);
    constraint1.setCharacterSize(20);
    constraint1.setFillColor(sf::Color(169, 169, 169, 150));
    constraint1.setPosition(1110, 265);

    sf::Text answerTextOne;
    answerTextOne.setString("");
    answerTextOne.setFont(font);
    answerTextOne.setCharacterSize(27);
    answerTextOne.setFillColor(sf::Color::Black);
    answerTextOne.setPosition(1230, 265);

    sf::Text error1;
    int visibleError = false;
    error1.setString("Invalid Input");
    error1.setFont(font);
    error1.setCharacterSize(27);
    error1.setFillColor(sf::Color::Red);
    error1.setPosition(1100, 300);



    //second answering space//
            //month
    sf::RectangleShape answerDate1(sf::Vector2f(70, 40));
    answerDate1.setPosition(1250, 500);
    answerDate1.setOutlineColor(sf::Color::Black);
    answerDate1.setOutlineThickness(0);

    sf::Text monthText;
    monthText.setString("Mth");
    monthText.setFont(font);
    monthText.setCharacterSize(20);
    monthText.setFillColor(sf::Color(169, 169, 169, 150));
    monthText.setPosition(1260, 510);

    sf::Text answerTextMonth;
    answerTextMonth.setString("");
    answerTextMonth.setFont(font);
    answerTextMonth.setCharacterSize(27);
    answerTextMonth.setFillColor(sf::Color::Black);
    answerTextMonth.setPosition(1260, 510);

    sf::Text errorMonth;
    int visibleErrorMonth = false;
    errorMonth.setString("Invalid Month");
    errorMonth.setFont(font);
    errorMonth.setCharacterSize(25);
    errorMonth.setFillColor(sf::Color::Red);
    errorMonth.setPosition(1240, 550);

    //day
    sf::RectangleShape answerDate2(sf::Vector2f(70, 40));
    answerDate2.setPosition(1350, 500);
    answerDate2.setOutlineColor(sf::Color::Black);
    answerDate2.setOutlineThickness(0);

    sf::Text dayText;
    dayText.setString("Day");
    dayText.setFont(font);
    dayText.setCharacterSize(20);
    dayText.setFillColor(sf::Color(169, 169, 169, 150));
    dayText.setPosition(1360, 510);

    sf::Text answerTextDay;
    answerTextDay.setString("");
    answerTextDay.setFont(font);
    answerTextDay.setCharacterSize(27);
    answerTextDay.setFillColor(sf::Color::Black);
    answerTextDay.setPosition(1360, 510);

    sf::Text errorDay;
    int visibleErrorDay = false;
    errorDay.setString("Invalid Day");
    errorDay.setFont(font);
    errorDay.setCharacterSize(25);
    errorDay.setFillColor(sf::Color::Red);
    errorDay.setPosition(1240, 570);
    //year
    sf::RectangleShape answerDate3(sf::Vector2f(70, 40));
    answerDate3.setPosition(1450, 500);
    answerDate3.setOutlineColor(sf::Color::Black);
    answerDate3.setOutlineThickness(0);

    sf::Text yearText;
    yearText.setString("Year");
    yearText.setFont(font);
    yearText.setCharacterSize(20);
    yearText.setFillColor(sf::Color(169, 169, 169, 150));
    yearText.setPosition(1455, 510);

    sf::Text answerTextYear;
    answerTextYear.setString("");
    answerTextYear.setFont(font);
    answerTextYear.setCharacterSize(27);
    answerTextYear.setFillColor(sf::Color::Black);
    answerTextYear.setPosition(1455, 510);

    sf::Text errorYear;
    int visibleErrorYear = false;
    errorYear.setString("Invalid Year");
    errorYear.setFont(font);
    errorYear.setCharacterSize(25);
    errorYear.setFillColor(sf::Color::Red);
    errorYear.setPosition(1240, 590);

    sf::Text questionTwo;
    questionTwo.setString("Please enter effective date desired for cases:");
    questionTwo.setFont(font);
    questionTwo.setCharacterSize(40);
    questionTwo.setFillColor(sf::Color::Black);
    questionTwo.setPosition(280, 500);


    //make the bottom team promo
    sf::RectangleShape bottonPromo(sf::Vector2f(240, 90));
    bottonPromo.setPosition(1690, 950);
    bottonPromo.setOutlineColor(sf::Color::Black);
    bottonPromo.setFillColor(sf::Color(50, 50, 50, 240));

    sf::Text promoText;

    promoText.setString("By team FlowSoft");
    promoText.setFont(font);
    promoText.setCharacterSize(24);
    promoText.setFillColor(sf::Color::White);
    promoText.setPosition(1700, 960);





    //make buttons

    sf::RectangleShape searchButton(sf::Vector2f(190, 70));
    searchButton.setPosition(330, 790);
    searchButton.setOutlineColor(sf::Color::Black);
    searchButton.setOutlineThickness(1);
    searchButton.setFillColor(sf::Color::Yellow);

    sf::Text searchText;
    searchText.setString("Search");
    searchText.setFont(font);
    searchText.setCharacterSize(34);
    searchText.setFillColor(sf::Color::Black);
    searchText.setPosition(370, 800);


    sf::RectangleShape resetButton(sf::Vector2f(160, 70));
    resetButton.setPosition(1700, 720);
    resetButton.setOutlineColor(sf::Color::Black);
    resetButton.setOutlineThickness(1);
    resetButton.setFillColor(sf::Color::Yellow);

    sf::Text resetText;
    resetText.setString("Reset");
    resetText.setFont(font);
    resetText.setCharacterSize(34);
    resetText.setFillColor(sf::Color::Black);
    resetText.setPosition(1740, 730);

    sf::RectangleShape quitButton(sf::Vector2f(160, 70));
    quitButton.setPosition(1700, 790);
    quitButton.setOutlineColor(sf::Color::Black);
    quitButton.setOutlineThickness(1);
    quitButton.setFillColor(sf::Color::Yellow);

    sf::Text quitText;
    quitText.setString("Quit");
    quitText.setFont(font);
    quitText.setCharacterSize(34);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(1740, 800);


    //choose cases or deaths option
    sf::RectangleShape caseButton(sf::Vector2f(160, 70));
    caseButton.setPosition(750, 630);
    caseButton.setOutlineColor(sf::Color::Black);
    //caseButton.setOutlineThickness(1);
    caseButton.setFillColor(sf::Color(135, 206, 250));

    sf::Text caseText;
    caseText.setString("Cases");
    caseText.setFont(font);
    caseText.setCharacterSize(34);
    caseText.setFillColor(sf::Color::Black);
    caseText.setPosition(775, 640);

    sf::RectangleShape deathButton(sf::Vector2f(160, 70));
    deathButton.setPosition(950, 630);
    deathButton.setOutlineColor(sf::Color::Black);
    //deathButton.setOutlineThickness(1);
    deathButton.setFillColor(sf::Color(135, 206, 250));

    sf::Text deathText;
    deathText.setString("Deaths");
    deathText.setFont(font);
    deathText.setCharacterSize(34);
    deathText.setFillColor(sf::Color::Black);
    deathText.setPosition(970, 640);

    sf::Text searchByTitle;
    searchByTitle.setString("Search By: ");
    searchByTitle.setFont(font);
    searchByTitle.setCharacterSize(34);
    searchByTitle.setFillColor(sf::Color::Black);
    searchByTitle.setPosition(860, 580);

    sf::Text searchByError;
    bool visibleSearch = false;
    searchByError.setString("Choose One");
    searchByError.setFont(font);
    searchByError.setCharacterSize(25);
    searchByError.setFillColor(sf::Color::Red);
    searchByError.setPosition(860, 710);







 

    //make the second page background rectangles
    sf::RectangleShape whiteSpace1(sf::Vector2f(740, 800));
    whiteSpace1.setPosition(50, 170);
    whiteSpace1.setFillColor(sf::Color(255, 255, 255, 170));

    sf::Text bTreeTitle;
    bTreeTitle.setString("BTree");
    bTreeTitle.setPosition(350, 170);
    bTreeTitle.setFont(bold);
    bTreeTitle.setCharacterSize(30);
    bTreeTitle.setFillColor(sf::Color::Black);


    sf::RectangleShape whiteSpace2(sf::Vector2f(740, 800));
    whiteSpace2.setPosition(900, 170);
    whiteSpace2.setFillColor(sf::Color(255, 255, 255, 170));

    sf::Text hashTableTitle;
    hashTableTitle.setString("HashTable");
    hashTableTitle.setPosition(1150, 170);
    hashTableTitle.setFont(bold);
    hashTableTitle.setCharacterSize(30);
    hashTableTitle.setFillColor(sf::Color::Black);

    sf::Text time1;

    time1.setString("BTree Time: ");
    time1.setPosition(50, 140);
    time1.setFont(font);
    time1.setCharacterSize(20);
    time1.setFillColor(sf::Color::Red);

    sf::Text time2;
    time2.setString("HashTable: ");
    time2.setPosition(900, 140);
    time2.setFont(font);
    time2.setCharacterSize(20);
    time2.setFillColor(sf::Color::Red);






    for (int x = 1; x <= 25; x++) {
        sf::Text tempText;
        ///create the text for the 2nd page
        tempText.setString("");
        tempText.setPosition(60, 180 + 30 * x);
        tempText.setFont(font);
        tempText.setCharacterSize(20);
        tempText.setFillColor(sf::Color::Black);
        textVec.push_back(tempText);
    
        sf::Text tempText1;
        tempText1.setString("");
        tempText1.setPosition(400, 180 + 30 * x);
        tempText1.setFont(font);
        tempText1.setCharacterSize(20);
        tempText1.setFillColor(sf::Color::Black);
        textVec2.push_back(tempText1);
    

        sf::Text tempText2;
        tempText2.setString("");
        tempText2.setPosition(930, 180 + 30 * x);
        tempText2.setFont(font);
        tempText2.setCharacterSize(20);
        tempText2.setFillColor(sf::Color::Black);
        textVec3.push_back(tempText2);
    
        sf::Text tempText3;
        tempText3.setString("");
        tempText3.setPosition(1280, 180 + 30 * x);
        tempText3.setFont(font);
        tempText3.setCharacterSize(20);
        tempText3.setFillColor(sf::Color::Black);
        textVec4.push_back(tempText3);
    }



    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {


            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {

                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);


                    sf::Vector2f mousePosFloat(mousePosition.x, mousePosition.y);
                    sf::FloatRect quitRectangle = quitButton.getGlobalBounds();
                    if (quitRectangle.contains(mousePosFloat)) {

                        window.close();
                    }
                    sf::FloatRect resetRectangle = resetButton.getGlobalBounds();
                    if (resetRectangle.contains(mousePosFloat)) {
                        visibleError = false;
                        visibleErrorDay = false;
                        visibleErrorMonth = false;
                        visibleErrorYear = false;
                        visibleSearch = false;
                        deathBox = false;
                        caseBox = false;

                        numState = "";
                        numYear = "";
                        numDay = "";
                        numMonth = "";

      
                        visible1 = 0;
                        visibleDay = 0;
                        visibleMonth = 0;
                        visibleYear = 0;

                        contained = false;
                        containedDay = false;
                        containedMonth = false;
                        containedYear = false;
                        deathButton.setOutlineThickness(0);
                        caseButton.setOutlineThickness(0);

                        answerTextOne.setString("");
                        answerTextDay.setString("");
                        answerTextMonth.setString("");
                        answerTextYear.setString("");
                        page = 0;
                        for (int x = 0; x < textVec.size(); x++) {
                            textVec[x].setString("");
                        }
                        for (int x = 0; x < textVec2.size(); x++) {
                            textVec2[x].setString("");
                        }
                        for (int x = 0; x < textVec3.size(); x++) {
                            textVec3[x].setString("");
                        }
                        for (int x = 0; x < textVec4.size(); x++) {
                            textVec4[x].setString("");
                        }



                    }
                    if (page == 0) {
                        sf::FloatRect caseRectangle = caseButton.getGlobalBounds();
                        if (caseRectangle.contains(mousePosFloat)) {
                            caseButton.setOutlineThickness(1);
                            deathButton.setOutlineThickness(0);
                            deathBox = false;
                            caseBox = true;
                            visibleSearch = false;



                        }
                        sf::FloatRect deathRectangle = deathButton.getGlobalBounds();
                        if (deathRectangle.contains(mousePosFloat)) {
                            deathBox = true;
                            caseBox = false;
                            caseButton.setOutlineThickness(0);
                            deathButton.setOutlineThickness(1);
                            visibleSearch = false;
                        }



                        //if click on search
                        sf::FloatRect searchRectangle = searchButton.getGlobalBounds();
                        if (searchRectangle.contains(mousePosFloat)) {

                            //test if the number is a number and is less than 50 and over 0
                            bool flag = checkNum(1, 50, numState);
                            bool flagMonth = checkNum(1, 12, numMonth);
                            bool flagDay = checkNum(1, 31, numDay);
                            bool flagYear = checkNum(19, 21, numYear);

                            // cout << numState << endl;

                            if (flag && flagMonth && flagDay && flagYear && (deathBox == true || caseBox == true)) {
                                /*
                                    THIS IS THE PART WHERE THE DATA IMPLEMENTATION SHOULD BE PLACED



                                */

                                int number = stoi(numState);
                                //was going to put this into a function but it appears that sf might not have supported this
                                page = 1;
                                int value;
                                if (number <= 25) {
                                    value = number;
                                }
                                else {
                                    value = 25;
                                }

                                for (int x = 1; x <= value; x++) {
                                   
                                    ///create the text for the 2nd page
                                    textVec[x-1].setString(to_string(x) + ". ");
                                }
                                if (number > 25) {
                                    int value2 = number - 25;
                                    for (int x = 1; x <= value2; x++) {
                                        textVec2[x-1].setString((to_string(x + 25)) + ". ");
                                    }

                                }
                                for (int x = 1; x <= value; x++) {
                                    textVec3[x-1].setString(to_string(x) + ". ");
                                }
                                if (number > 25) {
                                    int value2 = number - 25;
                                    for (int x = 1; x <= value2; x++) {
                                        textVec4[x-1].setString((to_string(x + 25)) + ". ");
                                    }

                                }


                            }
                            else {
                                if (flag == false)
                                    visibleError = true;
                                if (flagDay == false)
                                    visibleErrorDay = true;
                                if (flagMonth == false)
                                    visibleErrorMonth = true;
                                if (flagYear == false)
                                    visibleErrorYear = true;
                                if (caseBox == false && deathBox == false) {
                                    visibleSearch = true;
                                    //cout << "triggered" << endl;
                                }
                            }

                        }
                       
                        sf::FloatRect answerOneRect = answerNumStates.getGlobalBounds();
                        if (answerOneRect.contains(mousePosFloat)) {
                            answerNumStates.setOutlineThickness(1);
                            visibleError = false;
                            contained = true;
                            visible1 = 1;

                        }
                        if (!answerOneRect.contains(mousePosFloat)) {
                            contained = false;
                            answerNumStates.setOutlineThickness(0);
                            if (numState == "") {
                                visible1 = 0;
                            }
                        }
                        sf::FloatRect answerMonthRect = answerDate1.getGlobalBounds();
                        if (answerMonthRect.contains(mousePosFloat)) {
                            answerDate1.setOutlineThickness(1);
                            visibleErrorMonth = false;
                            containedMonth = true;
                            visibleMonth = 1;

                        }
                        if (!answerMonthRect.contains(mousePosFloat)) {
                            containedMonth = false;
                            answerDate1.setOutlineThickness(0);
                            if (numMonth == "") {
                                visibleMonth = 0;
                            }
                        }
                        sf::FloatRect answerDayRect = answerDate2.getGlobalBounds();
                        if (answerDayRect.contains(mousePosFloat)) {
                            answerDate2.setOutlineThickness(1);
                            visibleErrorDay = false;
                            containedDay = true;
                            visibleDay = 1;

                        }
                        if (!answerDayRect.contains(mousePosFloat)) {
                            containedDay = false;
                            answerDate2.setOutlineThickness(0);
                            if (numDay == "") {
                                visibleDay = 0;
                            }
                        }

                        sf::FloatRect answerYearRect = answerDate3.getGlobalBounds();
                        if (answerYearRect.contains(mousePosFloat)) {
                            answerDate3.setOutlineThickness(1);
                            visibleErrorYear = false;
                            containedYear = true;
                            visibleYear = 1;

                        }
                        if (!answerYearRect.contains(mousePosFloat)) {
                            containedYear = false;
                            answerDate3.setOutlineThickness(0);
                            if (numYear == "") {
                                visibleYear = 0;
                            }
                        }










                    }

                }

            }


            if (contained == true) {
                if (event.type == sf::Event::TextEntered) {
                    //if delete was hit, i found the number from a youtube video on making buttons with a class
                    //cout << event.text.unicode << endl;
                    if (event.text.unicode == 8) {
                        if (numState.size() > 0) {
                            string x = "";
                            for (int c = 0; c < numState.size() - 1; c++) {
                                x += numState[c];

                            }
                            numState = x;
                            answerTextOne.setString(numState);
                        }

                    }
                    else if (event.text.unicode == 32) {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27) {
                        contained = false;
                        answerNumStates.setOutlineThickness(0);
                        if (numState == "") {
                            visible1 = 0;
                        }

                    }
                    else if (event.text.unicode < 128 && numState.size() < 2) {
                        char key = (char)event.text.unicode;
                        numState = numState + key;

                        answerTextOne.setString(numState);
                    }
                }
            }
            //text change for month
            if (containedMonth == true) {
                if (event.type == sf::Event::TextEntered) {

                    if (event.text.unicode == 8) {
                        if (numMonth.size() > 0) {
                            string x = "";
                            for (int c = 0; c < numMonth.size() - 1; c++) {
                                x += numMonth[c];

                            }
                            numMonth = x;
                            answerTextMonth.setString(numMonth);
                        }

                    }
                    else if (event.text.unicode == 32) {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27) {
                        containedMonth = false;
                        answerTextMonth.setOutlineThickness(0);
                        if (numMonth == "") {
                            visibleMonth = 0;
                        }

                    }
                    else if (event.text.unicode < 128 && numMonth.size() < 2) {
                        char key = (char)event.text.unicode;
                        numMonth = numMonth + key;

                        answerTextMonth.setString(numMonth);
                    }
                }
            }

            //Day change text
            if (containedDay == true) {
                if (event.type == sf::Event::TextEntered) {

                    if (event.text.unicode == 8) {
                        if (numDay.size() > 0) {
                            string x = "";
                            for (int c = 0; c < numDay.size() - 1; c++) {
                                x += numDay[c];

                            }
                            numDay = x;
                            answerTextDay.setString(numDay);
                        }


                    }
                    else if (event.text.unicode == 32) {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27) {
                        containedDay = false;
                        answerTextDay.setOutlineThickness(0);
                        if (numDay == "") {
                            visibleDay = 0;
                        }

                    }
                    else if (event.text.unicode < 128 && numDay.size() < 2) {
                        char key = (char)event.text.unicode;
                        numDay = numDay + key;

                        answerTextDay.setString(numDay);
                    }
                }
            }
            //change year
            if (containedYear == true) {
                if (event.type == sf::Event::TextEntered) {

                    if (event.text.unicode == 8) {
                        if (numYear.size() > 0) {
                            string x = "";
                            for (int c = 0; c < numYear.size() - 1; c++) {
                                x += numYear[c];

                            }
                            numYear = x;
                            answerTextYear.setString(numYear);
                        }

                    }
                    else if (event.text.unicode == 32) {
                        //to stop spaces
                    }
                    //if esc or enter is pressed
                    else if (event.text.unicode == 13 || event.text.unicode == 27) {
                        containedYear = false;
                        answerTextYear.setOutlineThickness(0);
                        if (numYear == "") {
                            visibleYear = 0;
                        }

                    }
                    else if (event.text.unicode < 128 && numYear.size() < 2) {
                        char key = (char)event.text.unicode;
                        numYear = numYear + key;

                        answerTextYear.setString(numYear);
                    }
                }
            }



        }
        window.clear();
        //all pages
        window.draw(background);
        window.draw(top);
        window.draw(bottonPromo);
        window.draw(title);
        window.draw(promoText);
        window.draw(quitButton);
        window.draw(quitText);
        window.draw(resetButton);
        window.draw(resetText);

        //first page
        if (page == 0) {
            window.draw(answerNumStates);
            window.draw(answerDate1);
            window.draw(answerDate2);
            window.draw(answerDate3);
            window.draw(questionOne);
            

            window.draw(deathButton);

            window.draw(caseButton);
            window.draw(deathText);
            window.draw(caseText);


            window.draw(searchByTitle);


            if (visible1 == 0) {
                window.draw(constraint1);
            }
            else {
                window.draw(answerTextOne);
            }
            if (visibleMonth == 0) {
                window.draw(monthText);
            }
            else {
                window.draw(answerTextMonth);
            }
            if (visibleDay == 0) {
                window.draw(dayText);
            }
            else {
                window.draw(answerTextDay);
            }
            if (visibleYear == 0) {
                window.draw(yearText);
            }
            else {
                window.draw(answerTextYear);
            }

            window.draw(questionTwo);

            window.draw(searchButton);
          
            if (visibleError == true) {
                window.draw(error1);
            }
            if (visibleErrorDay == true) {
                window.draw(errorDay);
            }
            if (visibleErrorMonth == true) {
                window.draw(errorMonth);
            }
            if (visibleErrorYear == true) {
                window.draw(errorYear);
            }
            if (visibleSearch == true) {
                window.draw(searchByError);
            }

            window.draw(searchText);
            

        }
        if (page == 1) {
            window.draw(whiteSpace1);
            window.draw(whiteSpace2);
            window.draw(time1);
            window.draw(time2);



            window.draw(bTreeTitle);
            window.draw(hashTableTitle);
            for (int x = 0; x < textVec.size(); x++) {
                window.draw(textVec[x]);
            }
            for (int x = 0; x < textVec2.size(); x++) {
                window.draw(textVec2[x]);
            }
            for (int x = 0; x < textVec3.size(); x++) {
                window.draw(textVec3[x]);
            }
            for (int x = 0; x < textVec4.size(); x++) {
                window.draw(textVec4[x]);
            }

        }





        window.display();
    }
}


void StoreImage(string name, sf::Texture& text, map <string, sf::Texture>& imageStorage) {
    imageStorage.emplace(name, text);
}
bool checkNum(int start, int end, string number) {
    bool flag = true;
    if (number == "" || number == " " || number == "  ") {

        flag = false;
    }
    for (int c = 0; c < number.size(); c++) {

        if (isdigit(number[c]) == false) {
            flag = false;
        }
    }
    try {
        int value = stoi(number);
        if (value > end || value < start) {
            flag = false;
        }
    }
    catch (...) {
        flag = false;
    }



    return flag;
}



