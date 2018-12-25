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
        bool operator==(const kural& k) {
            return (this->nt == k.nt) && (this->ac == k.ac);
        }
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


class durum {
    private:
        std::vector<kural> parcalar;
        std::set<durum*> gecisler;
    public:
        durum(std::vector<kural> &p, std::set<durum*> &g);
        std::vector<kural> parcalari_ver();
        std::set<durum*> gecisleri_ver();
};

durum::durum(std::vector<kural> &p, std::set<durum*> &g) {
    parcalar = p;
    gecisler = g;
}

std::vector<kural> durum::parcalari_ver() {
    return parcalar;
}

std::set<durum*> durum::gecisleri_ver() {
    return gecisler;
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
        void kilif(std::vector<kural> &kilif_kumesi);
        //void nt_acilimi(const std::string &nt, std::vector<std::vector<std::string>> &ac);
        void parcalar_kumesi();
        std::vector<kural> kilif_acilimlar(std::string &nt);
        bool parca_eklenebilir(std::vector<kural> &kilif_kumesi, std::vector<kural> &bakildi);
        std::vector<kural> kurallar();
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
        satirlar.push_back(satir);
        kural_satirlari.erase(0, konum + 1);
    }
    konum2 = kural_satirlari.find("  ");
    kural_satirlari.erase(0, konum2 + 2);
    satirlar.push_back(kural_satirlari);
    std::string non_terminal;
    for (auto s : satirlar) {
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
        std::cout << kayit.non_terminal()  << " -> ";
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

bool gramer::parca_eklenebilir(std::vector<kural> &kilif_kumesi, std::vector<kural> &bakildi) {
    for (const auto &k : kilif_kumesi) {
        if (bakildi.end() != std::find(bakildi.begin(), bakildi.end(), k)) {
            bakildi.push_back(k);
            continue;
        }
        bakildi.push_back(k);
        std::vector<std::string> parca_acilimi = k.acilim();
        auto it = std::find(parca_acilimi.begin(), parca_acilimi.end(), ".");
        it++;
        if (non_terminal_mi(*it)) {
            return true;
        }
    }
    return false;
}


std::vector<kural> gramer::kilif_acilimlar(std::string &nt) {
    std::vector<kural> ac;
    for (const auto &k : kayit_tablosu) {
        if (k.non_terminal() == nt) {
            ac.push_back(k);
        }
    }
    return ac;
}

void gramer::ilk(std::vector<std::string> &katar, std::set<std::string> &ilk_kumesi, std::vector<std::string> bakildi) {
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
    if (nt == "S’") {
        izle_kumesi.insert("$");
        return izle_kumesi;
    }
    std::vector<std::string> acilim;
    for (auto const &kayit : kayit_tablosu) {
        acilim = kayit.acilim();
        if (std::find(acilim.begin(), acilim.end(), nt) != acilim.end()) {
            auto it = std::find(acilim.begin(), acilim.end(), nt);
            it++;
            if (it != acilim.end()) {
                if (!non_terminal_mi(*it)) {
                    izle_kumesi.insert(*it);
                }
                else {
                    std::vector<std::string> katar;
                    katar.push_back(*it);
                    std::set<std::string> nt_ilk = ilk(katar);
                    izle_kumesi.insert(nt_ilk.begin(), nt_ilk.end());
                }
            }
            else {
                izle_kumesi.insert("$");
            }
        }
    }
    return izle_kumesi;
}

void gramer::kilif(std::vector<kural> &kilif_kumesi) {
    //kilif_kumesi.push_back(baslangic);
    std::vector<kural> bakildi;
    while (parca_eklenebilir(kilif_kumesi, bakildi)) {
        std::vector<kural> eklenecekler;
        for (const auto &k : kilif_kumesi) {
            std::vector<std::string> parca_acilimi = k.acilim();
            auto it = std::find(parca_acilimi.begin(), parca_acilimi.end(), ".");
            it++;
            if (non_terminal_mi(*it)) {
                std::vector<kural> ac = kilif_acilimlar(*it);
                for (const auto &a : ac) {
                    std::vector<std::string> yeni_acilim = a.acilim();
                    yeni_acilim.insert(yeni_acilim.begin(), ".");
                    yeni_acilim.emplace_back(",");
                    auto yeni_it = it + 1;
                    if (*yeni_it == ",") {
                        yeni_acilim.emplace_back("$");
                    }
                    else {
                        std::vector<std::string> ilk_vek(yeni_it, parca_acilimi.end());
                        ilk_vek.erase(remove(ilk_vek.begin(), ilk_vek.end(), ","), ilk_vek.end());
                        std::set<std::string> il = ilk(ilk_vek);
                        for ( const auto &i : il) {
                            yeni_acilim.emplace_back(i);
                            yeni_acilim.emplace_back(",");
                        }
                        yeni_acilim.emplace_back("$");
                    }
                    std::string x = a.non_terminal();
                    kural yeni_kural = kural(x, yeni_acilim);
                    eklenecekler.emplace_back(yeni_kural);
                }
            }
        }
        for (const auto &e : eklenecekler) {
            kilif_kumesi.emplace_back(e);
        }
    }
    //std::set<kural> s(kilif_kumesi.begin(), kilif_kumesi.end());
    //kilif_kumesi.assign(s.begin(), s.end());
    std::vector<kural> tekrarsiz;
    for (const auto &k : kilif_kumesi) {
        auto it = std::find(tekrarsiz.begin(), tekrarsiz.end(), k);
        if (it == tekrarsiz.end()) {
            tekrarsiz.push_back(k);
        }
    }
    kilif_kumesi = tekrarsiz;
}


void gramer::parcalar_kumesi() {
    std::vector<std::string> ac = kayit_tablosu[0].acilim();
    std::string isim = kayit_tablosu[0].non_terminal();
    ac.insert(ac.begin(), ".");
    ac.push_back(",");
    ac.push_back("$");
    kural baslangic(isim, ac);
    std::vector<kural> kilif_kumesi;
    kilif(kilif_kumesi);

}

std::vector<kural> gramer::kurallar() {
    return kayit_tablosu;
}


int main() {
    std::ifstream giris("giriş.txt");
    if (!giris.is_open()){
        std::cerr << "Dosya açılamadı!" << std::endl;
        return EXIT_FAILURE;
    }
    std::stringstream ss;
    ss << giris.rdbuf();
    giris.close();
    gramer LR1(ss.str());
    std::cout << "Gramer:" << std::endl;
    LR1.yazdir();
    std::cout << std::endl;
    std::vector<std::string> b;
    std::string s = "S";
    std::vector<std::vector<std::string>> vvs;
    vvs = LR1.acilimlar(s, b);
    std::vector<std::string> v;
    v.emplace_back("S");
    v.emplace_back("B");
    v.emplace_back("a");
    v.emplace_back("A");
    std::set<std::string> ik = LR1.ilk(v);
    std::cout << "Örnek ilk kümesi: İLK(SBaA) = {";
    for (auto const &e : ik) {
        std::cout << e << ", ";
    }
    std::cout << "}" << std::endl;
    std::cout << std::endl;
    std::string nt = "B";
    std::set<std::string> izk = LR1.izle(nt);
    std::cout << "Örnek izle kümesi: İZLE(B) = {";
    for (auto const &e : izk) {
        std::cout << e << ", ";
    }
    std::cout << "}" << std::endl;

    std::vector<kural> kilif_kumesi;
    kural baslangic = LR1.kurallar()[0];
    std::vector<std::string> bas_ac = baslangic.acilim();
    std::string bas_nt = baslangic.non_terminal();
    bas_ac.insert(bas_ac.begin(), ".");
    bas_ac.emplace_back(",");
    bas_ac.emplace_back("$");
    baslangic = kural(bas_nt, bas_ac);
    kilif_kumesi.emplace_back(baslangic);
    LR1.kilif(kilif_kumesi);
    for (const auto &k : kilif_kumesi) {
        std::cout << k.non_terminal() << " -> ";
        for (const auto &a : k.acilim()) {
            std::cout << a << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}