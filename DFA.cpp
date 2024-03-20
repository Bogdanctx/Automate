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

    bool isAccepted(const std::string &word, int startNode, int wordIndex = 0);

private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> m_graf;
    std::unordered_set<int> m_stariFinale;
    int m_stareInitiala{};
};

bool DFA::isAccepted(const std::string &word, const int startNode, const int wordIndex) {
    if(wordIndex == word.length()) {
        return m_stariFinale.find(startNode) != m_stariFinale.end();
    }
    else {
        for(const auto &it: m_graf[startNode]) {
            if(it.second == word[wordIndex]) {
                return isAccepted(word, it.first, wordIndex + 1);
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

        bool wasAccepted = dfa.isAccepted(cuvant, dfa.getStareInitiala());

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