#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

/*
class terminal {
    private:
        std::string katar = "";
    public:
        terminal(std::string k);
        void set_terminal(std::string k);
        std::string get_terminal();
};

terminal::terminal(std::string k) {
    set_terminal(k);
}

void terminal::set_terminal(std::string k) {
    katar = k;
}

std::string terminal::get_terminal() {
    return katar;
}
*/

/*
class simge {
    protected:
        std::string isim;
    public:
        simge() = default;
        explicit simge(std::string k);
        std::string simgeyi_al();
};


//simge::simge() {}

simge::simge(std::string k) {
    isim = k;
}

std::string simge::simgeyi_al() {
    return isim;
}


class terminal: public simge {
    private:
        bool terminal = true;
    public:
        bool terminal_mi();
};

bool terminal::terminal_mi() {
    return terminal;
}


class nt: public simge {
    private:
        bool terminal = false;
        std::vector<std::vector<simge*>*> acilimlar;
    public:
        nt(std::string k, std::vector<std::string> simgeler);
        bool terminal_mi();
};

nt::nt(std::string k, std::vector<std::string> simgeler) {
    isim = k;

}

bool nt::terminal_mi() {
    return terminal;
}
*/

class kural {
    private:
        std::string nt;
        std::vector<std::string> ac;
    public:
        kural(std::string &isim, std::vector<std::string> &a);
        std::string non_terminal() const;
        std::vector<std::string> acilim() const;
};

kural::kural(std::string &isim, std::vector<std::string> &a) {
    nt = isim;
    ac = a;
}

std::string kural::non_terminal() const {
    return nt;
}

std::vector<std::string> kural::acilim() const {
    return ac;
}


class gramer {
    private:
        std::vector<kural> kayit_tablosu;
    public:
        explicit gramer(std::string kural_satirlari);
        void yazdir();
};

gramer::gramer(std::string kural_satirlari) {
    std::vector<std::string> satirlar;
    std::string satir;
    size_t konum = 0;
    size_t konum2 = 0;
    while ((konum = kural_satirlari.find('\n')) != std::string::npos) {
        satir = kural_satirlari.substr(0, konum);
        konum2 = satir.find("  ");
        satir.erase(0, konum2 + 2);
        // std::cout << satir << std::endl;
        satirlar.push_back(satir);
        kural_satirlari.erase(0, konum + 1);
    }
    konum2 = kural_satirlari.find("  ");
    kural_satirlari.erase(0, konum2 + 2);
    satirlar.push_back(kural_satirlari);
    // std::cout << kural_satirlari << std::endl;
    std::string non_terminal;
    for (auto s : satirlar) {
        // std::cout << s << std::endl;
        konum = s.find(" -> ");
        non_terminal = s.substr(0, konum);
        s.erase(0, konum + 4);
        std::vector<std::string> simgeler;
        size_t konum_bosluk;
        while ((konum_bosluk = s.find(' ')) != std::string::npos) {
            simgeler.push_back(s.substr(0, konum_bosluk));
            s.erase(0, konum + 1);
        }
        simgeler.push_back(s);
        kural k(non_terminal, simgeler);
        kayit_tablosu.push_back(k);
    }
}

void gramer::yazdir() {
    for (const auto &kayit : kayit_tablosu) {
        std::cout << kayit.non_terminal()  << " : ";
        for (const auto &simge : kayit.acilim()) {
            std::cout << simge << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    std::ifstream giris("giriş.txt");
    if (!giris.is_open()){
        std::cout << "Dosya açılamadı!" << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << giris.rdbuf();
    giris.close();
    gramer LR1(ss.str());
    LR1.yazdir();
    return 0;
}