#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>


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
        void yazdir() const;
        std::string acilim_bul(const std::string &nt) const;
        std::set<std::string> ilk(std::vector<std::string> &katar);
        void ilk(std::vector<std::string> &katar, std::set<std::string> &ilk_kumesi, std::vector<std::string> bakildi);
        std::set<std::string> izle(std::string &nt);
        bool non_terminal_mi(std::string &s);
        std::vector<std::vector<std::string>> acilimlar(std::string &nt, std::vector<std::string> &bakildi);
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

void gramer::yazdir() const {
    for (const auto &kayit : kayit_tablosu) {
        std::cout << kayit.non_terminal()  << " : ";
        for (const auto &simge : kayit.acilim()) {
            std::cout << simge << " ";
        }
        std::cout << std::endl;
    }
}

std::string gramer::acilim_bul(const std::string &nt) const {
    for (const auto &kayit : kayit_tablosu) {
        if (kayit.non_terminal() == nt) {
            if (islower(kayit.acilim()[0][0])) {
                acilim_bul(kayit.acilim()[0]);
                continue;
            }
        }
    }
    std::string acilim;
    return acilim;
}

std::set<std::string> gramer::ilk(std::vector<std::string> &katar) {
    std::set<std::string> i;
    std::vector<std::string> bakildi;
    ilk(katar, i, bakildi);
    return i;
}

bool gramer::non_terminal_mi(std::string &s) {
    for ( auto const &kayit : kayit_tablosu) {
        if (kayit.non_terminal() == s) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<std::string>> gramer::acilimlar(std::string &nt, std::vector<std::string> &bakildi) {
    std::vector<std::vector<std::string>> ac;
    for (auto const &kayit : kayit_tablosu) {
        if (kayit.non_terminal() == nt) {
            if (std::find(bakildi.begin(), bakildi.end(), kayit.non_terminal()) == bakildi.end()) {
                ac.push_back(kayit.acilim());
            }
        }
    }
    return ac;
}

void gramer::ilk(std::vector<std::string> &katar, std::set<std::string> &ilk_kumesi, std::vector<std::string> bakildi) {
    /*
    if (islower(katar[0][0])) {
        ilk_kumesi.insert(katar[0]);
        return;
    }
    for (const auto &kayit : kayit_tablosu) {
        if (kayit.non_terminal() == katar[0]) {
            if (islower(kayit.acilim()[0][0])) {
                ilk_kumesi.insert(kayit.acilim()[0]);
            }
        }
    }
    for (const auto &kayit : kayit_tablosu) {
        if (kayit.non_terminal() == katar[0]) {
            if (islower(kayit.acilim()[0][0])) {
                ilk_kumesi.insert(kayit.acilim()[0]);
            }
        }
    }*/
    if (!non_terminal_mi(katar[0])) {
        ilk_kumesi.insert(katar[0]);
        return;
    }
    std::vector<std::vector<std::string>> ac = acilimlar(katar[0], bakildi);
    for (auto &a : ac) {
        bakildi.push_back(katar[0]);
        ilk(a, ilk_kumesi, bakildi);
    }
}

std::set<std::string> gramer::izle(std::string &nt) {
    std::set<std::string> izle_kumesi;
    return izle_kumesi;
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
    std::cout << std::endl;
    std::vector<std::string> v;
   // v.emplace_back("a");
    v.emplace_back("S’");
    v.emplace_back("b");
    v.emplace_back("a");
    v.emplace_back("A");
    std::set<std::string> ik = LR1.ilk(v);
    for (auto const &e : ik) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
    return 0;
}