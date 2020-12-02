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
    string name;
    person.FIO = "";
    for (int i = 0;i < 2; ++i) {
        stream >> name;
        person.FIO += name + ' ';
    }
    stream >> name;
    person.FIO += name;
    stream >> person.citizenAdd.street;
    stream >> person.citizenAdd.house;
    stream >> person.citizenAdd.flat;
    stream >> person.gender;
    stream >> person.age;
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

void CountPeople(const vector <citizen>& HMO){
    string street = HMO[0].citizenAdd.street;
    int counter = 0;
    for (int i = 0; i<HMO.size(); ++i){
        auto person = HMO[i];
        if (person.citizenAdd.street == street){
            if (person.age >=18 && person.age <= 60 && person.gender == "male") ++counter;
        } else {
            cout << counter << " men(man) aged 18 to 60 live on the street " << street;
            if (counter != 0) {
                cout << ":" << endl;
                for(int j = i-counter; j<i; ++j){
                    cout << HMO[j].FIO << endl;
                }
            } else cout << endl;
            if(person.age >= 18 && person.age<=60 && person.gender == "male") {
                counter = 1;
            }
            else counter = 0;
            street = person.citizenAdd.street;
        }
    }
    cout << counter << " men(man) aged 18 to 60 live on the street " << street;
    if (counter != 0) {
        cout << ":" << endl;
        for(auto j = HMO.size()-1-counter; j<HMO.size()-1; ++j){
            cout << HMO[j].FIO << endl;
        }
    } else cout << endl;
}

int main() {
    std::ifstream input;
    input.open ("input.txt");
    vector <citizen> HMO;
    string people;
    while (getline(input, people)){
        std::stringstream ss;
        ss << people;
        citizen person;
        ss >> person;
        HMO.push_back(person);
    }

    std::sort(HMO.begin(), HMO.end(), comparePersonStreet);
    CountPeople(HMO);
    std::sort(HMO.begin(), HMO.end(), comparePersonFIO);
    std::ofstream out;
    out.open ("output.txt");
    for (auto& person: HMO){
        out << person;
    }
    out.close();

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
        cout << person;
    }
    binin.close(); //Закрываем файл
    return 0;
}
