/****************************************************************************
**					SAKARYA ÜNÝVERSÝTESÝ
**			BÝLGÝSAYAR VE BÝLÝÞÝM BÝLÝMLERÝ FAKÜLTESÝ
**				    BÝLGÝSAYAR MÜHENDÝSLÝÐÝ BÖLÜMÜ
**				PROGRAMLAMAYA GÝRÝÞÝ DERSÝ
**
**				ÖDEV NUMARASI:PROJE ODEVI
**				ÖÐRENCÝ ADI:ABDULLAH ANIL ÇOBANER
**				ÖÐRENCÝ NUMARASI:B231210097
**				DERS GRUBU:A
****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Futbolcu {
public:
    string LisansNo;
    string adi;
    string soyadi;
    string oyundakiKonumu;
    string ucret;
    string dogumTarihi;
    int takimNO;
    //futbolculari listelerken futbolcu dosyasi okunduktan sonra eger oyle bir futbolcu varsa bilgilerinin yazdirilacagi fonksiyon
    void ekranaYaz() const {
        cout << "Lisans NO: " << LisansNo
            << "\nAdý: " << adi
            << "\nSoyadý: " << soyadi
            << "\nKonumu: " << oyundakiKonumu
            << "\nÜcret: " << ucret
            << "\nDoðum Tarihi: " << dogumTarihi
            << "\nTakým NO: " << takimNO << "\n";
    }
};

class Takim {
public:
    int takimNO;
    string adi;
    string adresi;
    string telefon;
    int oyuncuSayisi;
    string yoneticiIsmi;
};
//ligde oynanan maclar skorlar ve puanlarin sifirlanip yeni bir lig baslatmayi saglayan fonksiyon
void ligsifirlama() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    remove("ligmac.txt");
    cout << "Lig mac ve puan kayitlari sifirlandi yeni lig baslatabilirsiniz" << endl;
}
//program icerigiyle ilgili bilgilerin bulundugu fonksiyon
void bilgilendirme() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "**Super Lig'e Hosgeldiniz**" << endl;
    cout << "*Program 6 takimdan olusan bir lig sistemiyle kurgulanmistir" << endl;
    cout << "*5 tane takim onceden dosyaya kaydedilmis olup 6. takimi kullanici kendi takimi olarak olusturur (istege gore takimlar silinip eklenebilir) ve kendi futbolcularini kendi istengine gore olusturarak ligi baslatir" << endl;
    cout << "*Takima eklenen futbolcular goruntulenebilir ve istege gore yeni futbolcu eklenip silinebilir" << endl;
    cout << "*Her takimda en az 11 futbolcu olma zorunlulugu vardir eger takima kayitli 11 futbolcu yok ise o takim ligde oynayamaz" << endl;
    cout << "*Her takim diger 5 takim ile 1 er kez mac yapar ve skorlar kaydedilir skorlara gore her lig sonu takimlarin puanlari yazdirilir" << endl;
    cout << "*Lig oynandiktan sonra skorlari ve puanlari goruntuleyebilirsiniz " << endl;
    cout << "*Yeni bir lig baslatmak isterseniz onceki puanlamanin ustune ekleyebilir yada onu sifirlayarak yeni bir lig baslatabilirsiniz" << endl;
}
//takimlarin mac yapabilmesi icin takimda en az 11 futbolcu olmasi gerekir bu kontrolu saglayan fonksiyon
bool takimOnbirOyuncuVarMi(int takimNO) {
    ifstream futbolcuOku("futbolcu.txt");
    Futbolcu futbolcu;
    int oyuncuSayisi = 0;

    while (futbolcuOku >> futbolcu.LisansNo
        >> futbolcu.adi
        >> futbolcu.soyadi
        >> futbolcu.oyundakiKonumu
        >> futbolcu.ucret
        >> futbolcu.dogumTarihi
        >> futbolcu.takimNO) {

        if (futbolcu.takimNO == takimNO) {
            oyuncuSayisi++;
        }
    }

    futbolcuOku.close();

    return oyuncuSayisi >= 11;
}

void puandurumunugoster();
//futbolcu eklenecegi zaman futbolcunun girilen bilgilerini dosyaya  yazmak icin cagrilan fonksiyon
void futbolcuDosyayaYaz(const Futbolcu& futbolcu) {
    ofstream dosya("futbolcu.txt", ios::app);
    dosya << futbolcu.LisansNo
        << " " << futbolcu.adi
        << " " << futbolcu.soyadi
        << " " << futbolcu.oyundakiKonumu
        << " " << futbolcu.ucret
        << " " << futbolcu.dogumTarihi
        << " " << futbolcu.takimNO << endl;

    dosya.close();
}
//takim eklenecegi zaman takimin girilen bilgilerini dosyaya  yazmak icin cagrilan fonksiyon
void takimDosyayaYaz(const Takim& takim) {
    ofstream dosya("takim.txt", ios::app);
    dosya << takim.takimNO
        << " " << takim.adi
        << " " << takim.adresi
        << " " << takim.telefon
        << " " << takim.oyuncuSayisi
        << " " << takim.yoneticiIsmi << endl;
    dosya.close();
}
//secilen takimin futbolcularini listeleme fonksiyonu
void futbolculariListele() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    int listelenecekTakimNO;
    cout << "Listelenecek takýmýn NO'sunu girin: ";
    cin >> listelenecekTakimNO;

    ifstream dosya("futbolcu.txt");

    if (!dosya.is_open()) {
        cout << "Futbolcu dosyasi acilamadi." << endl;
        return;
    }

    Futbolcu futbolcu;
    bool futbolcuVar = false;

    while (dosya >> futbolcu.LisansNo
        >> futbolcu.adi
        >> futbolcu.soyadi
        >> futbolcu.oyundakiKonumu
        >> futbolcu.ucret
        >> futbolcu.dogumTarihi
        >> futbolcu.takimNO) {
        if (futbolcu.takimNO == listelenecekTakimNO) {
            futbolcu.ekranaYaz();
            cout << "------------------------\n";
            futbolcuVar = true;
        }
    }

    dosya.close();

    if (!futbolcuVar) {
        cout << "Belirtilen takým numarasýna ait futbolcu bulunamadý." << endl;
    }
}

//secilen takimin varligini dosyadan okuyarak kontrol eden fonksiyon
bool takimVarMi(int takimNO) {
    ifstream dosya("takim.txt");
    Takim takim;

    while (dosya >> takim.takimNO
        >> takim.adi
        >> takim.adresi
        >> takim.telefon
        >> takim.oyuncuSayisi
        >> takim.yoneticiIsmi) {
        if (takim.takimNO == takimNO) {
            dosya.close();
            return true;
        }
    }

    dosya.close();
    return false;
}
//secilen takima yeni futbolcu ekleme fonksiyonu
void futbolcuEkle() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    Futbolcu yeniFutbolcu;
    cout << "Futbolcu LisansNo: ";
    cin >> yeniFutbolcu.LisansNo;
    cout << "Futbolcu Adi: ";
    cin >> yeniFutbolcu.adi;
    cout << "Futbolcu Soyadi: ";
    cin >> yeniFutbolcu.soyadi;
    cout << "Oyundaki Konumu: ";
    cin >> yeniFutbolcu.oyundakiKonumu;
    cout << "Futbolcu Ucreti: ";
    cin >> yeniFutbolcu.ucret;
    cout << "Dogum Tarihi: ";
    cin >> yeniFutbolcu.dogumTarihi;

    int takimNO;
    cout << "Hangi takýma eklemek istiyorsunuz? Takým NO: ";
    cin >> takimNO;

    ifstream takimDosya("takim.txt");
    Takim takim;

    bool takimVar = false;
    while (takimDosya >> takim.takimNO
        >> takim.adi
        >> takim.adresi
        >> takim.telefon
        >> takim.oyuncuSayisi
        >> takim.yoneticiIsmi) {
        if (takim.takimNO == takimNO) {
            takimVar = true;
            break;
        }
    }
    takimDosya.close();

    if (!takimVar) {
        cout << "Belirtilen takým numarasýna ait takým bulunamadý. Futbolcu eklenemedi." << endl;
        return;
    }

    yeniFutbolcu.takimNO = takimNO;


    ofstream futbolcuDosya("futbolcu.txt", ios::app);
    futbolcuDosyayaYaz(yeniFutbolcu);
    futbolcuDosya.close();

    cout << "Futbolcu takýmýna basarýyla eklendi." << endl;
}
//secilen takimdan secilen futbolcuyu silme fonksiyonu
void futbolcuSil() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "Hangi takýmýn futbolcusunu silmek istiyorsunuz? Takým NO: ";
    int silinecekFutbolcuTakimNO;
    cin >> silinecekFutbolcuTakimNO;


    if (!takimVarMi(silinecekFutbolcuTakimNO)) {
        cout << "Belirtilen takým numarasýna ait takým bulunamadý" << endl;
        return;
    }

    ifstream oku("futbolcu.txt");
    ofstream yaz("gecici.txt");

    Futbolcu futbolcu;

    string silinecekLisansNo;
    bool futbolcuVar = false;

    do {
        cout << "Silinecek futbolcunun LisansNo'sunu girin: ";
        cin >> silinecekLisansNo;

        while (oku >> futbolcu.LisansNo
            >> futbolcu.adi
            >> futbolcu.soyadi
            >> futbolcu.oyundakiKonumu
            >> futbolcu.ucret
            >> futbolcu.dogumTarihi
            >> futbolcu.takimNO) {
            if (futbolcu.LisansNo == silinecekLisansNo && futbolcu.takimNO == silinecekFutbolcuTakimNO) {
                futbolcuVar = true;
                break;
            }
        }

        if (!futbolcuVar) {
            cout << "Belirtilen takým ve lisans numarasýna ait futbolcu bulunamadý. Lütfen tekrar deneyin" << endl;
        }

        oku.clear();
        oku.seekg(0, ios::beg);

    } while (!futbolcuVar);


    while (oku >> futbolcu.LisansNo
        >> futbolcu.adi
        >> futbolcu.soyadi
        >> futbolcu.oyundakiKonumu
        >> futbolcu.ucret
        >> futbolcu.dogumTarihi
        >> futbolcu.takimNO) {
        if (futbolcu.LisansNo != silinecekLisansNo || futbolcu.takimNO != silinecekFutbolcuTakimNO) {
            yaz << futbolcu.LisansNo << " "
                << futbolcu.adi << " "
                << futbolcu.soyadi << " "
                << futbolcu.oyundakiKonumu << " "
                << futbolcu.ucret << " "
                << futbolcu.dogumTarihi << " "
                << futbolcu.takimNO << endl;
        }
    }

    oku.close();
    yaz.close();


    remove("futbolcu.txt");


    rename("gecici.txt", "futbolcu.txt");

    cout << "Futbolcu baþarýyla silindi" << endl;
}


//yeni bir takim ekleme fonksiyonu
void takimEkle() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    Takim yeniTakim;
    cout << "Takim NO: ";
    cin >> yeniTakim.takimNO;

    if (yeniTakim.takimNO > 6 || yeniTakim.takimNO < 0) {
        cout << "HATA lutfen takim numarasini 1 den 6 ya kadar bir sayi seciniz" << endl;
        return;
    }

    if (takimVarMi(yeniTakim.takimNO)) {
        cout << "Bu numaraya sahip bir takim mevcut. Lutfen baska bir numara girin." << endl;
        return;
    }

    cout << "Takim Adi: ";
    cin >> yeniTakim.adi;
    cout << "Takim Adresi: ";
    cin >> yeniTakim.adresi;
    cout << "Takim Telefon: ";
    cin >> yeniTakim.telefon;
    cout << "Oyuncu Sayisi: ";
    cin >> yeniTakim.oyuncuSayisi;
    cout << "Yonetici Ismi: ";
    cin >> yeniTakim.yoneticiIsmi;

    takimDosyayaYaz(yeniTakim);
    cout << "Takim eklendi" << endl;
}
//eklenen takimi silme fonksiyonu
void takimSil() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    int silinecekTakimNO;
    cout << "Silinecek takýmýn NO'sunu girin: ";
    cin >> silinecekTakimNO;

    ifstream oku("takim.txt");
    ofstream yaz("gecici.txt");

    Takim takim;

    while (oku >> takim.takimNO
        >> takim.adi
        >> takim.adresi
        >> takim.telefon
        >> takim.oyuncuSayisi
        >> takim.yoneticiIsmi) {
        if (takim.takimNO != silinecekTakimNO) {
            yaz << takim.takimNO << " "
                << takim.adi << " "
                << takim.adresi << " "
                << takim.telefon << " "
                << takim.oyuncuSayisi << " "
                << takim.yoneticiIsmi << endl;
        }
    }

    oku.close();
    yaz.close();

    remove("takim.txt");

    rename("gecici.txt", "takim.txt");


    ifstream futbolcuOku("futbolcu.txt");
    ofstream futbolcuYaz("geciciFutbolcu.txt");

    Futbolcu futbolcu;

    while (futbolcuOku >> futbolcu.LisansNo
        >> futbolcu.adi
        >> futbolcu.soyadi
        >> futbolcu.oyundakiKonumu
        >> futbolcu.ucret
        >> futbolcu.dogumTarihi
        >> futbolcu.takimNO) {

        if (futbolcu.takimNO != silinecekTakimNO) {

            futbolcuYaz << futbolcu.LisansNo << " "
                << futbolcu.adi << " "
                << futbolcu.soyadi << " "
                << futbolcu.oyundakiKonumu << " "
                << futbolcu.ucret << " "
                << futbolcu.dogumTarihi << " "
                << futbolcu.takimNO << endl;

        }
    }

    futbolcuOku.close();
    futbolcuYaz.close();

    remove("futbolcu.txt");

    rename("geciciFutbolcu.txt", "futbolcu.txt");

    cout << "Takým silindi" << endl;
}
//ligde mac yapacak iki takimi golleri ve isimleri icin degiskenler
int gol1, gol2;
int takim1, takim2;
//girilen takim numarasini alarak takimin adini yazdirma fonksiyonu
string takimadial(int takimNO) {
    ifstream dosya("takim.txt");
    Takim takim;

    while (dosya >> takim.takimNO
        >> takim.adi
        >> takim.adresi
        >> takim.telefon
        >> takim.oyuncuSayisi
        >> takim.yoneticiIsmi) {
        if (takim.takimNO == takimNO) {
            dosya.close();
            return takim.adi;
        }
    }

    dosya.close();
    return "";
}
//6 takimdan olusan ligde takimlarin skorlarinin alindigi puanlarinin hesaplandigi maclarin kaydedildigi ve gerekli kontrolleri saglayan fonksiyon
void ligmackaydi() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    ifstream takimDosya("takim.txt");
    int kayitlitakimlar = 0;
    Takim takim;

    while (takimDosya >> takim.takimNO
        >> takim.adi
        >> takim.adresi
        >> takim.telefon
        >> takim.oyuncuSayisi
        >> takim.yoneticiIsmi) {
        kayitlitakimlar++;
    }

    takimDosya.close();

    if (kayitlitakimlar < 6) {
        cout << "Lig maclari icin kayitli 6 takim olmalidir. Mevcut kayitli takim sayisi: " << kayitlitakimlar << endl; //kullaniciya dosyada kac tane takim kayitli oldugunu gosterir
        return;
    }

    int takimSayisi = 6; //ligi 6 takim ile sinirlar
    int puanlar[6] = { 0 };

    for (int i = 1; i <= takimSayisi; ++i) {

        if (!takimOnbirOyuncuVarMi(i)) {
            cout << "Hata: " << takimadial(i) << " takýmýnda en az 11 oyuncu olmalý" << endl;
            return;
        }
    }

    ofstream ligmacDosya("ligmac.txt", ios::app);

    if (!ligmacDosya.is_open()) {
        cout << "Lig maç dosyasi acilamadi." << endl;
        return;
    }

    cout << "Mac Sonuclarýný Girin:" << endl;
    //karsilikli mac skorlarinin alindigi for dongusu
    for (int i = 1; i <= takimSayisi; ++i) {
        for (int j = i + 1; j <= takimSayisi; ++j) {

            cout << "Mac Sonucu " << takimadial(i) << " vs " << takimadial(j) << endl;
            cout << "Ev sahibi takim skoru: ";
            cin >> gol1;

            while (gol1 < 0) {
                cout << "HATA gol sayisi negatif olamaz gecerli skor giriniz" << endl;
                cin >> gol1;
            }
            cout << "Deplasman takim skoru: ";
            cin >> gol2;

            while (gol2 < 0) {
                cout << "HATA gol sayisi negatif olamaz gecerli skor giriniz" << endl;
                cin >> gol2;
            }

            ligmacDosya << i << " " << j << " " << gol1 << " " << gol2 << endl;
            //gol sayilarina gore puanlarin hesaplanmasi ve uan dizisine kaydedilmesi
            if (gol1 == gol2) {
                puanlar[i - 1] += 1;
                puanlar[j - 1] += 1;
            }
            else if (gol1 > gol2) {
                puanlar[i - 1] += 3;
            }
            else {
                puanlar[j - 1] += 3;
            }
        }
    }

    ligmacDosya.close();

    cout << "Maclar,skorlar ve puanlar kaydedildi." << endl;

}
//puanlar dizisine kaydedilen puanlari yazdirma fonksiyonu   (dizi de atanan puanlar sistem her baslatildiginda sifirlanmaz cunku puanlar sistem her baslatildiginda dosyaya kayitli olan skorlari degerlendirilerek hesaplanir bu sekilde puanlar sifirlanmamis olur)
void puandurumunugoster() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    int takimSayisi = 6;
    int puanlar[6] = { 0 };

    ifstream ligmacDosya("ligmac.txt");

    if (!ligmacDosya.is_open()) {
        cout << "ligmac dosyasi acilamadi" << endl;
        return;
    }

    string takimIsimleri[6];

    for (int i = 1; i <= takimSayisi; ++i) {
        takimIsimleri[i - 1] = takimadial(i);
    }

    while (ligmacDosya >> takim1 >> takim2 >> gol1 >> gol2) {
        if (gol1 == gol2) {
            puanlar[takim1 - 1] += 1;
            puanlar[takim2 - 1] += 1;
        }
        else if (gol1 > gol2) {
            puanlar[takim1 - 1] += 3;
        }
        else {
            puanlar[takim2 - 1] += 3;
        }
    }

    ligmacDosya.close();

    cout << "Puan Durumu\n";
    for (int i = 0; i < takimSayisi; ++i) {
        cout << takimIsimleri[i] << ": " << puanlar[i] << " puan\n";
    }
}
//kaydedilen mac skorlarini dosyadan okuyarak goruntuleme fonksiyonu
void ligMacSkorlari() {
    cout << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    ifstream ligmacDosya("ligmac.txt");

    if (!ligmacDosya.is_open()) {
        cout << "ligmac dosyasi acilamadi" << endl;
        return;
    }

    cout << "Lig Maclari ve Skorlari:" << endl;

    while (ligmacDosya >> takim1 >> takim2 >> gol1 >> gol2) {
        cout << takimadial(takim1) << " " << gol1 << " - " << gol2 << " " << takimadial(takim2) << endl;
    }

    ligmacDosya.close();
}

//ana fonksiyonumuz
int main() {

    setlocale(LC_ALL, "Turkish");

    //yapilacak islemin secilebilecegi do while icinde switch case dongusu ile yapilan bir menu
    int secim;

    do {
        cout << "----------- MENU -----------" << endl;
        cout << "1- Bilgilendirme " << endl;
        cout << "2- Futbolcu ekle" << endl;
        cout << "3- Takim ekle" << endl;
        cout << "4- Futbolculari listele" << endl;
        cout << "5- Takim sil" << endl;
        cout << "6- Futbolcu sil" << endl;
        cout << "7- Super ligi baslat" << endl;
        cout << "8- Puan Durumu" << endl;
        cout << "9- Maclarin Skorlarini Goster" << endl;
        cout << "10- Lig Sifirla" << endl;
        cout << "0- Cikis" << endl;
        cout << endl;
        cout << "***Seciminizi yapin: ";
        cin >> secim;

        switch (secim) {
        case 1:bilgilendirme();
            break;
        case 2:
            futbolcuEkle();
            break;
        case 3:
            takimEkle();
            break;
        case 4:
            futbolculariListele();
            break;
        case 5:
            takimSil();
            break;
        case 6:
            futbolcuSil();
            break;
        case 7:
            ligmackaydi();
            break;
        case 8:
            puandurumunugoster();
            break;
        case 9:
            ligMacSkorlari();
            break;
        case 10:
            ligsifirlama();
            break;
        case 0:
            cout << "Super Lig programi kapatiliyor..." << endl;
            break;
        default:
            cout << "Lutfen gecerli bir secim yapin" << endl;
        }

    } while (secim != 0);

    return 0;
}