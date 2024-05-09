#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <queue>
#include <string>

class NFA {
public:
    void readInput(std::ifstream &stream);
    void toDFA();

    bool isAccepted(const std::string &word);
private:
    std::unordered_map<std::string, std::unordered_map<char, std::set<int>>> m_deltaNFA{}; // functia de tranzitie delta[state1][symbol] = state2
    std::unordered_map<std::string, std::unordered_map<char, std::string>> m_deltaDFA;

    std::set<std::string> m_finalStates{};
};

void NFA::toDFA() {
    std::queue<std::string> q; // <multime de stari, simbol>

    for(const auto &delta: m_deltaNFA["0"]) {
        std::string newState;
        for(const int &state: delta.second) {
            newState += std::to_string(state);
        }
        m_deltaDFA["0"][delta.first] = newState;
        q.emplace(newState); // push la starea initiala
    }


    while(!q.empty()) {
        const std::string currentState = q.front();
        q.pop();

        std::string newState{};

        for(const char &state: currentState) {
            for(const auto &symbol: m_deltaNFA[std::to_string(state - '0')]) {
                for(const int &nfaState: m_deltaNFA[std::to_string(state - '0')][symbol.first]) {
                    if(newState.find(std::to_string(nfaState)) == -1) {
                        newState += std::to_string(nfaState);
                    }
                }

                m_deltaDFA[currentState][symbol.first] = newState;

                if(m_deltaDFA[newState][symbol.first].empty()) {
                    q.emplace(newState);
                }
            }
        }
    }

    std::set<std::string> newFinalStates{};

    for(const auto &delta: m_deltaDFA) {
        for(const auto &finalState: m_finalStates) {
            if(delta.first.find(finalState) != -1) {
                newFinalStates.insert(delta.first);
            }
        }
    }

    m_finalStates = newFinalStates;

    m_deltaNFA.erase(m_deltaNFA.begin(), m_deltaNFA.end());
}

void NFA::readInput(std::ifstream &stream) {
    int numarStari{}, numarTranzitii{};

    stream >> numarStari;
    stream >> numarTranzitii;

    for(int tranzitie = 0; tranzitie < numarTranzitii; tranzitie++) {
        int state1{}, state2{};
        char symbol{};

        stream >> state1 >> state2 >> symbol;
        m_deltaNFA[std::to_string(state1)][symbol].insert(state2);
    }

    int numarStariFinale{};

    stream >> numarStariFinale;

    for(int i = 0; i < numarStariFinale; i++) {
        std::string stareFinala{};
        stream >> stareFinala;
        m_finalStates.insert(stareFinala);
    }
}

bool NFA::isAccepted(const std::string &word) {
    std::queue<std::string> nodes;
    int pos = 0;
    nodes.push("0");
    while(!nodes.empty()) {
        const std::string crtNode = nodes.front();
        nodes.pop();

        if(pos == word.length() && m_finalStates.find(crtNode) != m_finalStates.end()) {
            return true;
        }

        if(!m_deltaDFA[crtNode][word[pos]].empty()) {
            nodes.push(m_deltaDFA[crtNode][word[pos]]);
            ++pos;
        }
    }

    return false;
}

int main() {
    std::ifstream fin("../test.in");
    NFA nfa{};

    nfa.readInput(fin);
    nfa.toDFA();

    int numarCuvinte{};

    fin>>numarCuvinte;

    for(int i = 0; i < numarCuvinte; i++) {
        std::string word{};

        fin>>word;

        if(nfa.isAccepted(word)) {
            std::cout << word << " acceptat\n";
        }
        else {
            std::cout << word << " respins\n";
        }
    }

    fin.close();

    return 0;
}