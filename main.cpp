#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>


using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

/*
Ivanov Ivan Ivanovich
Mosfilm 5a 101 male 28
 */

struct address{
    string street;
    string house;
    string flat;
};

struct citizen{
    string FIO;
    address citizenAdd;
    string gender;
    string age;
};

bool comparePersonFIO (citizen& a, citizen& b){
    return a.FIO<b.FIO;
}

bool comparePersonStreet (citizen& a, citizen& b){
    return a.citizenAdd.street <b.citizenAdd.street;
}

int main() {
    std::ifstream input;
    input.open ("C:\\Users\\baykalov\\CLionProjects\\lab6\\input.txt");
    vector <citizen> HMO;
    for(int i = 0; i<10; ++i){
        citizen person;
        string information;
        getline (input, information);
        person.age = information.substr(information.rfind(' ')+1);
        information.erase(information.rfind(' '));
        person.gender = information.substr(information.rfind(' ')+1);
        information.erase(information.rfind(' '));
        person.citizenAdd.flat = information.substr(information.rfind(' ')+1);
        information.erase(information.rfind(' '));
        person.citizenAdd.house = information.substr(information.rfind(' ')+1);
        information.erase(information.rfind(' '));
        person.citizenAdd.street = information.substr(information.rfind(' ')+1);
        information.erase(information.rfind(' '));
        person.FIO = information;
        HMO.push_back(person);
    }

    std::sort(HMO.begin(), HMO.end(), comparePersonStreet);
    string street = HMO[0].citizenAdd.street;
    int counter = 0;
    for (int i = 0; i<HMO.size(); ++i){
        auto person = HMO[i];
        if (person.citizenAdd.street == street){
            if(atoi(person.age.c_str())>=18 && atoi(person.age.c_str())<=60) ++counter;
        } else {
            cout << counter << " people aged 18 to 60 live on the street " << street;
            if (counter != 0) {
                cout << ":" << endl;
                for(int j = i-counter; j<i; ++j){
                    cout << HMO[j].FIO << endl;
                }
            } else cout << endl;
            if(atoi(person.age.c_str())>=18 && atoi(person.age.c_str())<=60) {
                counter = 1;
            }
            else counter = 0;
            street = person.citizenAdd.street;
        }
    }
    cout << counter << " people aged 18 to 60 live on the street " << street;
    if (counter != 0) {
        cout << ":" << endl;
        for(int j = HMO.size()-1-counter; j<HMO.size()-1; ++j){
            cout << HMO[j].FIO << endl;
        }
    } else cout << endl;

    std::sort(HMO.begin(), HMO.end(), comparePersonFIO);
    std::ofstream out;
    out.open ("C:\\Users\\baykalov\\CLionProjects\\lab6\\output.txt");
    for (auto& people: HMO){
        out << "ФИО: " << people.FIO << ' ' ;
        out << "Адрес: " << people.citizenAdd.street;
        out << ", " << people.citizenAdd.house;
        out << ", "  << people.citizenAdd.flat << ' ';
        out << "Пол: "<< people.gender << ' ';
        out << "Возраст: "<< people.age << endl;
    }
    out.close();

    std::ofstream binout("C:\\Users\\baykalov\\CLionProjects\\lab6\\outputBin.txt", std::ios::binary);
    for (auto& people: HMO){
        string str = people.FIO + ' ' + people.citizenAdd.street +  ' ' + people.citizenAdd.house + ' ';
        str += people.citizenAdd.flat +  ' ' + people.gender + ' ' + people.age + '\n';
        binout.write(reinterpret_cast<char*>(&str), size(str));
    }
    binout.close();
    return 0;
}