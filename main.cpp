#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>


using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct address{
    string street = "Dostoevskogo";
    string house = "5a";
    int flat = 101;
};

struct citizen{
    string FIO = "Ivanov Ivan Ivanovich";
    address citizenAdd;
    string gender = "male";
    int age = 28;
};

void BinWriteString(string &str, std::ostream& binout){
    uint8_t a = str.length();
    binout.write(reinterpret_cast<const char *>(&a), 1);
    binout.write(str.data(), str.length());
}

bool comparePersonFIO (const citizen& a, const citizen& b){
    return a.FIO < b.FIO;
}

bool comparePersonStreet (const citizen& a, const citizen& b){
    return a.citizenAdd.street < b.citizenAdd.street;
}

std::istream& operator>> (std::istream& stream, citizen& person){
    string people;
    std::getline(stream, people);
    person.FIO = people.substr(0, people.find(','));
    people.erase(0, people.find(',')+2);
    person.citizenAdd.street = people.substr(0, people.find(','));
    people.erase(0, people.find(',')+2);
    person.citizenAdd.house = people.substr(0, people.find(','));
    people.erase(0, people.find(',')+2);
    person.citizenAdd.flat = stoi(people.substr(0, people.find(',')));
    people.erase(0, people.find(',')+2);
    person.gender = people.substr(0, people.find(','));
    people.erase(0, people.find(',')+2);
    person.age = stoi(people.substr(0, people.find(',')));
    people.erase(0, people.find(',')+2);
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const citizen& person){
    stream << "ФИО: " << person.FIO << ' ' ;
    stream << "Адрес: " << person.citizenAdd.street;
    stream << ", " << person.citizenAdd.house;
    stream << ", "  << person.citizenAdd.flat << ' ';
    stream << "Пол: "<< person.gender << ' ';
    stream << "Возраст: "<< person.age << endl;
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const vector<citizen>& HMO){
    for (auto& person: HMO){
        stream << person;
    }
    return stream;
}

void CountPeople(vector <citizen>& HMO){
    std::sort(HMO.begin(), HMO.end(), comparePersonStreet);
    string street = HMO[0].citizenAdd.street;
    int counter = 0; //переменная counter отвечает за количество подходящих людей
    int CountOfPeopleOnStreet = 0; //а эта переменная отвечает за то, сколько всего людей живёт на определённой улице
    for (int i = 0; i<HMO.size(); ++i){
        auto person = HMO[i];
        if (person.citizenAdd.street == street){
            if (person.age >=18 && person.age <= 60 && person.gender == "male"){
                ++counter;
            }
            CountOfPeopleOnStreet++;
        } else {
            cout << counter << " men(man) aged 18 to 60 live on the street " << street;
            if (counter != 0) {
                cout << ":" << endl;
                for(int j = i-CountOfPeopleOnStreet; j<i; ++j){
                    if (HMO[j].age >=18 && HMO[j].age <= 60 && HMO[j].gender == "male") {
                        cout << HMO[j].FIO << endl;
                    }
                }
            } else cout << endl;
            if(person.age >= 18 && person.age<=60 && person.gender == "male") {
                counter = 1; //счётчик по улицам уже переключился на следующую и я проверяю, подходит ли этот житель
            }
            else counter = 0;
            street = person.citizenAdd.street;
            CountOfPeopleOnStreet = 1;
        }
    }
    cout << counter << " men(man) aged 18 to 60 live on the street " << street;
    if (counter != 0) {
        cout << ":" << endl;
        for(auto j = HMO.size()-CountOfPeopleOnStreet; j<HMO.size(); ++j){
            if (HMO[j].age >=18 && HMO[j].age <= 60 && HMO[j].gender == "male") {
                cout << HMO[j].FIO << endl;
            }
        }
    } else cout << endl;
}

void WriteToTxt(vector <citizen>& HMO){
    std::ofstream out;
    out.open ("output.txt");
    out << HMO;
    out.close();
}

void ReadFromTxt(vector <citizen>& HMO){
    std::ifstream input;
    input.open ("input.txt");
    string people;
    while (getline(input, people)){
        std::stringstream ss;
        ss << people;
        citizen person;
        ss >> person;
        HMO.push_back(person);
    }
}

void WriteToBin(vector <citizen>& HMO){
    std::ofstream binout("outputBin.txt", std::ios::binary);
    for (auto& person: HMO){
        BinWriteString(person.FIO, binout);
        std::stringstream ss;
        ss << person.citizenAdd.street << ' ' << person.citizenAdd.house << ' ' << person.citizenAdd.flat;
        string adress;
        getline(ss, adress);
        BinWriteString(adress, binout);
        BinWriteString(person.gender, binout);
        uint8_t a = person.age;
        binout.write(reinterpret_cast<const char *>(&a), sizeof(a));
    }
    binout.close();
}

void ReadFromBin(vector <citizen>& HMO){
    std::ifstream binin("outputBin.txt",std::ios::binary|std::ios::in); //Открываем файл в двоичном режиме только для чтения
    uint8_t a;
    while (binin.read(reinterpret_cast<char *>(&a), sizeof(uint8_t))){
        citizen person;
        person.FIO.resize(a);
        binin.read(person.FIO.data(), a);
        binin.read(reinterpret_cast<char *>(&a), sizeof(uint8_t));
        string adress;
        adress.resize(a);
        binin.read(adress.data(), a);
        std::stringstream ss;
        ss << adress;
        ss >> person.citizenAdd.street >> person.citizenAdd.house >> person.citizenAdd.flat;
        binin.read(reinterpret_cast<char *>(&a), sizeof(uint8_t));
        person.gender.resize(a);
        binin.read(person.gender.data(), a);
        binin.read(reinterpret_cast<char *>(&person.age), sizeof(uint8_t));
        HMO.push_back(person);
    }
    binin.close(); //Закрываем файл
}

int main() {
    vector <citizen> HMO;
    ReadFromTxt(HMO);
    CountPeople(HMO);
    std::sort(HMO.begin(), HMO.end(), comparePersonFIO);
    WriteToTxt(HMO);
    WriteToBin(HMO);
    ReadFromBin(HMO);
    return 0;
}
