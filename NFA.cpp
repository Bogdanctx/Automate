#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class DFA {
public:
    DFA() = default;
    ~DFA() = default;

    void citesteStari(std::ifstream &fin);
    void citesteTranzitii(std::ifstream &fin);
    void citesteStareInitiala(std::ifstream &fin);
    void citesteStariFinale(std::ifstream &fin);

    int getStareInitiala() const { return m_stareInitiala; };

    void isAccepted(bool &result, const std::string &word, const int startNode, const int wordIndex);

private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> m_graf;
    std::unordered_set<int> m_stariFinale;
    int m_stareInitiala{};
    std::unordered_map<int, std::unordered_map<int, bool>> visitedNodes;
};

void DFA::isAccepted(bool &result, const std::string &word, const int startNode, const int wordIndex = 0) {
    if(startNode == m_stareInitiala) { // daca a inceput verificarea resetez nodurile vizitate
        visitedNodes = std::unordered_map<int, std::unordered_map<int, bool>>();
    }

    if(wordIndex == word.length()) {
        result = m_stariFinale.find(startNode) != m_stariFinale.end();
    }
    else {
        for(const auto &it: m_graf[startNode]) {
            if(it.second == word[wordIndex] && visitedNodes[it.first][wordIndex] == 0) {
                visitedNodes[it.first][wordIndex] = 1;
                isAccepted(result, word, it.first, wordIndex + 1);
            }
        }
    }
}

void DFA::citesteStareInitiala(std::ifstream &fin) {
    fin>>m_stareInitiala;
}

void DFA::citesteStariFinale(std::ifstream &fin) {
    int numarStariFinale;
    fin>>numarStariFinale;
    for(int i = 0; i < numarStariFinale; i++) {
        int stareFinala;
        fin>>stareFinala;
        m_stariFinale.insert(stareFinala);
    }
}

void DFA::citesteStari(std::ifstream &fin) {
    int numarStari;
    fin>>numarStari;
    for(int i = 0; i < numarStari; i++) {
        int stare;
        fin>>stare;
        m_graf[stare] = {};
    }
}

void DFA::citesteTranzitii(std::ifstream &fin) {
    int numarTranzitii;
    fin>>numarTranzitii;
    for(int i = 0; i < numarTranzitii; i++) {
        int x, y;
        char l;
        fin >> x >> y >> l;
        m_graf[x].emplace_back(y, l);
    }
}

int main() {
    std::ifstream fin("../input.txt");
    DFA dfa;

    dfa.citesteStari(fin);
    dfa.citesteTranzitii(fin);
    dfa.citesteStareInitiala(fin);
    dfa.citesteStariFinale(fin);

    int nrCuvinte;

    fin>>nrCuvinte;

    for(int i = 0; i < nrCuvinte; i++) {
        std::string cuvant;
        fin>>cuvant;

        bool wasAccepted = false;
        dfa.isAccepted(wasAccepted, cuvant, dfa.getStareInitiala());

        if(wasAccepted) {
            std::cout<<"DA\n";
        }
        else {
            std::cout<<"NU\n";
        }
    }

    fin.close();

    return 0;
}