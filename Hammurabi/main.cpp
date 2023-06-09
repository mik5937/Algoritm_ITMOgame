#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>

const float kMinRatCoef = 0.0f;
const float kMaxRatCoef = 0.07f;
const int kMinRipeWheatBushelsPerAcre = 1;
const int kMaxRipeWheatBushelsPerAcre = 6;
const int kMinAcrePrice = 17;
const int kMaxAcrePrice = 26;
const int kMaxAcresPerCitizen = 10;
const int kWheatBushelsPerCitizen = 20;
const float kWheatBushelsToPlantPerAcre = 0.5f;
const float kDeathPercentToLose = 0.45f;
const float kPlagueChance = 0.15f;
const int kReignLenth = 10;

struct City {
    int citizens;
    int wheatBushels;
    int landAcres;
};

struct Gameplay {
    int roundNumber;
    int citizensDied;
    int newCitizens;
    bool plague;
    int harvestedWheatBushels;
    int wheatBushelsPerAcre;
    int eatenWheatBushelsByRats;
    int acrePrice;
    bool loss;
    float diedPercent;
    float averageDiedPercent;
};

struct PlayerDecision {
    int acresToBuy;
    int acresToSell;
    int wheatToEat;
    int acresToSeed;
};

void CalculateResourcesAndEvents(City&, Gameplay&, PlayerDecision&);
void AdviserReport(City&, Gameplay&);
template <typename T> void InputVar(T&);
void InputPlayerDecision(City&, Gameplay&, PlayerDecision&);
void SummarizeReign(City&, Gameplay&);
void SaveGame(City&, Gameplay&);
void LoadGame(City&, Gameplay&);
std::string SplitStringRightPart(std::string, const char);

std::string savePath = "GameSave.txt";
std::default_random_engine randGenerator;

int main() {
    setlocale(LC_ALL, "RU");
    setlocale(LC_NUMERIC, "C");
    randGenerator.seed(time(NULL));

    bool loadGame = false;
    std::uniform_int_distribution<int> acrePriceDistribution(kMinAcrePrice, kMaxAcrePrice);
    City city{ 100, 2800, 1000 };
    Gameplay gameplay{ 0, 0, 0, false, 0, 0, 0, acrePriceDistribution(randGenerator), false, 0.0f, 0.0f };
    PlayerDecision playerDecision{};

    std::cout << "����������, ������� ���������� ���������� ����? (1 - ��, 0 - ���): ";
    InputVar(loadGame);

    if (loadGame)
        LoadGame(city, gameplay);

    if (gameplay.roundNumber == 0) {
        std::cout <<
            "��� ����������, ��� ��������� ����������" << std::endl <<
            "��������� ������ ���������� " << city.citizens << " �������;" << std::endl <<
            "� ������� ����� " << city.wheatBushels << " ������� �������;" << std::endl <<
            "����� �������� " << city.landAcres << " �����;" << std::endl <<
            "1 ��� ����� ����� ������ " << gameplay.acrePrice << " ������." << std::endl;

        InputPlayerDecision(city, gameplay, playerDecision);
        CalculateResourcesAndEvents(city, gameplay, playerDecision);
    }

    while (gameplay.roundNumber != kReignLenth && !gameplay.loss) {
        AdviserReport(city, gameplay);
        InputPlayerDecision(city, gameplay, playerDecision);
        CalculateResourcesAndEvents(city, gameplay, playerDecision);
    }

    SummarizeReign(city, gameplay);

    return 0;
}

void CalculateResourcesAndEvents(City& city, Gameplay& gameplay, PlayerDecision& decision) {
    // Harvest wheat
    std::uniform_int_distribution<int> bushelsPerAcreDistribution(kMinRipeWheatBushelsPerAcre, kMaxRipeWheatBushelsPerAcre);
    gameplay.wheatBushelsPerAcre = bushelsPerAcreDistribution(randGenerator);
    gameplay.harvestedWheatBushels = gameplay.wheatBushelsPerAcre * decision.acresToSeed;
    city.wheatBushels += gameplay.harvestedWheatBushels;

    // Rats eat wheat
    std::uniform_real_distribution<float> ratCoefDistribution(kMinRatCoef, kMaxRatCoef);
    gameplay.eatenWheatBushelsByRats = ratCoefDistribution(randGenerator) * city.wheatBushels;
    city.wheatBushels -= gameplay.eatenWheatBushelsByRats;

    // Citizens die from starving
    gameplay.citizensDied = city.citizens - std::ceil(static_cast<float>(decision.wheatToEat) / kWheatBushelsPerCitizen);
    gameplay.diedPercent = static_cast<float>(gameplay.citizensDied) / city.citizens;
    gameplay.averageDiedPercent += gameplay.diedPercent;
    city.citizens -= gameplay.citizensDied;

    if (gameplay.diedPercent > kDeathPercentToLose) {
        gameplay.loss = true;
        return;
    }

    // New citizens
    gameplay.newCitizens = gameplay.citizensDied / 2 + (5 - gameplay.wheatBushelsPerAcre) * city.wheatBushels / 600 + 1;

    if (gameplay.newCitizens < 0)
        gameplay.newCitizens = 0;
    else if (gameplay.newCitizens > 50)
        gameplay.newCitizens = 50;

    city.citizens += gameplay.newCitizens;

    // Plague
    std::uniform_real_distribution<float> plagueDistribution(0.0f, 1.0f);
    gameplay.plague = plagueDistribution(randGenerator) <= kPlagueChance;
    if (gameplay.plague)
        city.citizens = std::floor(static_cast<float>(city.citizens) / 2);

    // Random acre price
    std::uniform_int_distribution<int> acrePriceDistribution(kMinAcrePrice, kMaxAcrePrice);
    gameplay.acrePrice = acrePriceDistribution(randGenerator);

    // Next round
    ++gameplay.roundNumber;
}

void AdviserReport(City& city, Gameplay& gameplay) {
    std::cout << std::endl <<
        "��� ����������, �������� �������� ����" << std::endl <<
        "� " << gameplay.roundNumber << " ����  ������ ����������� ���������" << std::endl <<
        gameplay.citizensDied << " ������� ������ � ������, � " << gameplay.newCitizens << " ������� ������� � ��� ������� �����;" << std::endl <<
        (gameplay.plague ? "���� ���������� �������� ���������;\n" : "") <<
        "��������� ������ ������ ���������� " << city.citizens << " �������;" << std::endl <<
        "�� ������� " << gameplay.harvestedWheatBushels << " ������� �������, �� " << gameplay.wheatBushelsPerAcre << " ������ � ����;" << std::endl <<
        "����� ��������� " << gameplay.eatenWheatBushelsByRats << " ������� �������, ������� " << city.wheatBushels << " ������ � �������;" << std::endl <<
        "����� ������ �������� " << city.landAcres << " �����;" << std::endl <<
        "1 ��� ����� ����� ������ " << gameplay.acrePrice << " ������." << std::endl;
}

template <typename T>
void InputVar(T& input) {
    while (true) {
        std::cin >> input;

        if (std::cin.fail() || input < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "� ����������, ��� �� ��������. ���������� �������: ";
        }
        else
            break;
    }
}

void InputPlayerDecision(City& city, Gameplay& gameplay, PlayerDecision& decision) {
    bool enoughResources = false;
    bool exitGame = false;
    int tempWheat;

    std::cout << std::endl << "��� ���������, ����������?" << std::endl;

    while (!enoughResources) {
        if (gameplay.roundNumber > 0) {
            std::cout << "������� �� �� ����� �������� ����? (1 - ��, 0 - ���) ";
            InputVar(exitGame);

            if (exitGame) {
                SaveGame(city, gameplay);
                exit(0);
            }
        }

        tempWheat = city.wheatBushels;

        std::cout << "������� ����� ����� ����������� ������? ";
        InputVar(decision.acresToBuy);
        tempWheat -= decision.acresToBuy * gameplay.acrePrice;

        std::cout << "������� ����� ����� ����������� �������? ";
        InputVar(decision.acresToSell);
        tempWheat += decision.acresToSell * gameplay.acrePrice;

        std::cout
            << "������� ������� ������� ����������� ������? (��� ��������� ������ ���������� 20 �������; ����� " <<
            city.citizens * kWheatBushelsPerCitizen << ") ";
        InputVar(decision.wheatToEat);
        tempWheat -= decision.wheatToEat;

        std::cout << "������� ����� ����� ����������� �������? (�� ���� ��� ����������� 0.5 ������ �������) ";
        InputVar(decision.acresToSeed);
        tempWheat -= kWheatBushelsToPlantPerAcre * decision.acresToSeed;

        if (tempWheat >= 0 && decision.acresToSeed / kMaxAcresPerCitizen <= city.citizens) {
            enoughResources = true;
            city.landAcres += decision.acresToBuy;
            city.landAcres -= decision.acresToSell;
            city.wheatBushels = tempWheat;
        }
        else
            std::cout << "�, ����������, ������ ���! � ��� ������ " << city.citizens << " �������, " <<
            city.wheatBushels << " ������� ������� � " << city.landAcres << " ����� �����!" << std::endl;
    }
}

void SummarizeReign(City& city, Gameplay& gameplay) {
    if (gameplay.loss) {
        std::cout << std::endl <<
            "���� ������� ������� � ������ " << gameplay.diedPercent * 100 <<
            "% ������� �� ���� ��� ���������!" << std::endl <<
            "����� ������� ����, � ������ ��� �� ������. ������ �� ��������� ������� ������ ������������� � ��������" << std::endl;
    }
    else {
        gameplay.averageDiedPercent /= kReignLenth;
        int acresPerCitizen = std::ceil(static_cast<float>(city.landAcres) / city.citizens);

        //std::cout << std::endl << gameplay.averageDiedPercent << ' ' << acresPerCitizen << std::endl;
        if (gameplay.averageDiedPercent > 0.33f && acresPerCitizen < 7) {
            std::cout <<
                "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������." << std::endl <<
                "������ �� ��������� ������� ������ ������������� � ��������..." << std::endl;
        }
        else if (gameplay.averageDiedPercent > 0.1f && acresPerCitizen < 9) {
            std::cout <<
                "�� ������� �������� �����, ������� ������ � ����� ��������." << std::endl <<
                "����� �������� � �����������, � ����� ������ �� ������ ������ ��� ����������." << std::endl;
        }
        else if (gameplay.averageDiedPercent > 0.03f && acresPerCitizen < 10) {
            std::cout <<
                "�� ���������� ������ �������, � ���, �������, ���� ���������������," << std::endl <<
                "�� ������ ������ �� ������� ��� �� ����� ������ �����." << std::endl;
        }
        else
            std::cout << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����!" << std::endl;
    }
}

void SaveGame(City& city, Gameplay& gameplay) {
    std::ofstream os(savePath, std::ios::out | std::ios::trunc);

    if (os.is_open()) {
        os << "Citizens: " << city.citizens << std::endl;
        os << "Wheat bushels: " << city.wheatBushels << std::endl;
        os << "Land acres: " << city.landAcres << std::endl;

        os << "Round number: " << gameplay.roundNumber << std::endl;
        os << "Citizens died: " << gameplay.citizensDied << std::endl;
        os << "New citizens: " << gameplay.newCitizens << std::endl;
        os << "Plague: " << gameplay.plague << std::endl;
        os << "Harvested wheat bushels: " << gameplay.harvestedWheatBushels << std::endl;
        os << "Wheat bushels per acre: " << gameplay.wheatBushelsPerAcre << std::endl;
        os << "Acre price: " << gameplay.acrePrice << std::endl;
        os << "DiedPercent: " << gameplay.diedPercent << std::endl;
        os << "Average died percent: " << gameplay.averageDiedPercent << std::endl;
    }

    os.close();
}

void LoadGame(City& city, Gameplay& gameplay) {
    std::string str;
    std::ifstream is(savePath, std::ios::in);

    if (is.is_open()) {
        std::getline(is, str);
        city.citizens = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        city.wheatBushels = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        city.landAcres = std::stoi(SplitStringRightPart(str, ':'));

        std::getline(is, str);
        gameplay.roundNumber = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.citizensDied = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.newCitizens = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.plague = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.harvestedWheatBushels = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.wheatBushelsPerAcre = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.acrePrice = std::stoi(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.diedPercent = std::stof(SplitStringRightPart(str, ':'));
        std::getline(is, str);
        gameplay.averageDiedPercent = std::stof(SplitStringRightPart(str, ':'));
    }

    is.close();
}

std::string SplitStringRightPart(std::string strToSplit, const char delimeter) {
    std::string result;
    std::stringstream ss(strToSplit);

    std::getline(ss, result, delimeter);
    std::getline(ss, result, delimeter);

    return result.erase(0, 1);
}
