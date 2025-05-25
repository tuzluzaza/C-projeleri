/****************************************************************************
**					SAKARYA ÜNÝVERSÝTESÝ
**			BÝLGÝSAYAR VE BÝLÝÞÝM BÝLÝMLERÝ FAKÜLTESÝ
**				    BÝLGÝSAYAR MÜHENDÝSLÝÐÝ BÖLÜMÜ
**				PROGRAMLAMAYA GÝRÝÞÝ DERSÝ
**
**				ÖDEV NUMARASI: 2
**				ÖÐRENCÝ ADI: ABDULLAH ANIL ÇOBANER
**				ÖÐRENCÝ NUMARASI: B231210097
**				DERS GRUBU: A
****************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

#pragma warning (disable:4996)   //zaman ile ilgili kullandýðým iþlemlerin hata vermesini engelleyen kod (ctime,localtime,sscanf)

using namespace std;

//hesaplarda kullanacaðýmýz deðiþkenler iþlemler fonksiyonlarýn bulunduðu hesap sýnýfý
class Hesap {
protected:
	int hesapNo;               //hesap numarasý
	double miktar;             //kripto para miktarý
	double tl;                 //TL miktarý
	double kurdakiDegisim;     //kripto paranýn günlük deðiþim orani
	time_t acilisTarihi;       //hesap açýlýþ tarihi
	time_t islemTarihi;        //en son iþlem yapýlan tarih

public:
	//kurucu fonksiyon
	Hesap(int hNo, double m) : hesapNo(hNo), miktar(m), tl(m), kurdakiDegisim(0) { //hNo hesap numarasýný m miktarý temsil etmektedir
		acilisTarihi = time(0);
		islemTarihi = acilisTarihi;
	}
	//sanal bir fonksiyon kripto paranin günlük deðiþimini ayarlayabilmek için kullandým
	virtual void gunlukDegisim() {

	}
	//kripto para alma fonksiyonu
	void al(double m) {
		miktar += m;
		tl -= m * (1 + kurdakiDegisim);
		islemTarihi = time(0);
	}
	//kripto para satma fonksiyonu satilan fonksiyon TL ye çevrilir
	void sat(double m) {
		miktar -= m;
		tl += m * (1 + kurdakiDegisim);
		islemTarihi = time(0);
	}
	//hesaptaki bakiyeyi gösteren fonksiyon
	void bakiye() {
		cout << "Hesap No: " << hesapNo << endl;
		cout << "Kripto Para Miktari: " << miktar << endl;
		cout << "TL miktari: " << tl << endl;
		cout << "Kurdaki Yüzde Deðiþim: %" << kurdakiDegisim * 100 << endl;
		cout << "Hasabýn Açýldýðý Tarih: " << ctime(&acilisTarihi);
		cout << "En Son ÝÞlem Yapýlan Tarih: " << ctime(&islemTarihi);
	}
	//kripto paranin borsada kur deðiþimi ile kar/zarar iliþkisini gösteren fonksiyon
	double kar(time_t tarih) {

		return miktar * kurdakiDegisim;
	}
};
//hesap sýnýfýnýn özelliklerinin genel sýnýfýna kalýtýldýðýný gösterir
class Genel : public Hesap {
private:
	static const double artisOrani; //genel hesaptaki artiþ orani
	static const double dususOrani; //genel hesaptaki düþüþ orani
	int dususSayaci; // her 15 günde bir düþüþ yapabilmesi için atanan deðiþken

public:
	Genel() : Hesap(0, 0), dususSayaci(0) {}
	Genel(int hNo, double m) : Hesap(hNo, 0), dususSayaci(0) {
		tl = m;
	}
	//bu fonksiyon genel hesabýn özelliði olan günde yüzde 1 kar etmesini ve 15 günde bir yüzde 5 düþmesini saðlar
	void gunlukDegisim() override {
		dususSayaci++;
		if (dususSayaci == 15) {
			kurdakiDegisim -= dususOrani;
			dususSayaci = 0;
		}
		else {
			kurdakiDegisim += artisOrani;
		}
	}
};

//Genel sýnýfý içinde deðiþtirilemeyecek olan artisOrani ve dususOrani deðiþkenlerinin atanmasý
const double Genel::artisOrani = 0.01;
const double Genel::dususOrani = 0.05;

//hesap sýnýfýnýn alt sýnýfý denebilir hesap sýnýfýnýn özel sýnýfa özelliklerini kalýttýðýný gösterir
class Ozel : public Hesap {
private:
	//özel sýnýfýnda deðiþtirilemeyecek artisOrani ve dususOraný deðiþkenlerini atar
	static const double artisOrani;     //özel hesaptaki artýþ oraný
	static const double dususOrani;     //özel hesaptaki düþüþ oraný
	bool dususGunu[30];

public:
	Ozel() : Hesap(0, 0) {
		//30 günlük bir periyotta günlük deðiþim olup olmadýðýný kontrol eder 
		for (int i = 0; i < 30; i++) {
			dususGunu[i] = false;
		}
		rasgeleDususGunuBelirle();
	}

	Ozel(int hNo, double m) : Hesap(hNo, 0) {
		tl = m;
		for (int i = 0; i < 30; i++) {
			dususGunu[i] = false;
		}
		rasgeleDususGunuBelirle();
	}
	//özel hesabýn özelliði olan her ay random 3 gün yüzde 10 düþüþ olacak rastgele 3 gün belirlenmesi fonksiyonu
	void rasgeleDususGunuBelirle() {
		srand(time(0)); //her seferinde farklý gün belirlemek için kullanýlýr
		int sayac = 0;
		while (sayac < 3) {
			int gun = rand() % 30;
			if (!dususGunu[gun]) {
				dususGunu[gun] = true;
				sayac++;
			}
		}
	}

	void gunlukDegisim() override {
		time_t bugun = time(0);
		tm* t = localtime(&bugun);
		int gun = t->tm_mday - 1;  //bu satýr 1-31 gün arasýnda bir deðer belirleyen tm_mday den 1 çikartarak 0-30 arasýnda bir deðere sahip olmasýný saðlar ki bir aylýk bir periyotta iþlem yapabilelim
		if (dususGunu[gun]) {
			kurdakiDegisim -= dususOrani;
		}
		else {
			kurdakiDegisim += artisOrani;
		}
		if (gun == 29) {
			rasgeleDususGunuBelirle();
		}
	}
};

const double Ozel::artisOrani = 0.05;
const double Ozel::dususOrani = 0.10;

//kullanýcýnýn karþýsýna çýkacak ana menü seçim kýsmý
void anamenu() {
	cout << "-----------------------------------------------------------------------------------------";
	cout << endl;
	cout << "              $$ BORSA KRÝPTO" << endl << endl << endl;
	cout << "  Ýþlemler: " << endl;
	cout << "---------------------------------------" << endl;
	cout << "1.)* Genel Hesap Aç" << endl;
	cout << "2.)** Özel Hesap Aç" << endl;
	cout << "---------------------------------------" << endl;
	cout << "3.)*** Kripto Para Al" << endl;
	cout << "4.)**** Kripto Para Sat" << endl;
	cout << "---------------------------------------" << endl;
	cout << "5.)***** Tarih Aralýðý Seçiniz" << endl;
	cout << "6.)****** Hesaplarý Listele" << endl;
	cout << "---------------------------------------" << endl;
	cout << "7.)******* Çýkýþ Yap" << endl;
	cout << "-----------------------------------------------------------------------------------------";
	cout << endl;
}
//genel ve özel hesap için 100 ile sýnýrlandýrýlmýþ diziler açýlan hesaplar kayýt edilecek
Genel genelHesaplar[100];
Ozel ozelHesaplar[100];

//kullanýcý tarafýndan seçilen bir menü iþlemine göre hesap iþlemlerini gerçekleþtiren ana iþlevi yerine getirir
void menuIslemleri(Hesap* hesaplar[], Genel genelHesaplar[], Ozel ozelHesaplar[], time_t& sistemTarihi, int secim) {
	int hNo;
	double m;

	//ana menü kullanýcýnýn yaptýðý seçime göre karþýsýna açýlacak pencereler ve iþlemlerin olduðu switch case yapýsý
	switch (secim) {

	case 1: //genel hesap açma bölümü
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "*GENEL HESAP AÇMA ÝÞLEMÝ*" << endl;
		cout << "Hesap Numaranýzý Oluþturun (1-100 arasi numaralar seçilebilir): ";
		cin >> hNo;
		cout << "Tl Miktarýnýzý Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] == nullptr && hNo > 0 && hNo < 101) {
			genelHesaplar[hNo] = Genel(hNo, m);
			hesaplar[hNo] = &genelHesaplar[hNo];
			cout << "Genel Hesabýnýz Baþarýyla Açýldý!" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Bu Hesap Numarasýný Alamazsýnýz" << endl;
		}
		break;
	case 2: //özel hesap açma bölümü
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "**ÖZEL HESAP AÇMA ÝÞLEMÝ**" << endl;
		cout << "Hesap Numaranýzý Oluþturun (101-199 arasi numaralar seçilebilir): ";
		cin >> hNo;
		cout << "Tl Miktarýnýzý Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] == nullptr && hNo > 100 && hNo < 201) {
			ozelHesaplar[hNo - 100] = Ozel(hNo, m);
			hesaplar[hNo] = &ozelHesaplar[hNo - 100];
			cout << "Özel Hesabýnýz Baþarýyla Açýldý!" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Bu Hesap Numarasýný Alamazsýnýz" << endl;
		}
		break;
	case 3: //kripto para alma bölümü
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "***KRÝPYO PARA ALMA ÝÞLEMÝ***" << endl;
		cout << "Kripto Para Alýnacak Hesabý Seçiniz: ";
		cin >> hNo;
		cout << "Eklenecek Miktarý Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] != nullptr) {
			hesaplar[hNo]->al(m);
			cout << "Kripto Para Alma Ýþleminiz Baþarýyla Gerçekleþti" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Böyle Bir Hesap Bulunamadý" << endl;
		}
		break;
	case 4:  //kripto para satým bölümü 
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "****KRÝPTO PARA SATMA ÝÞLEMÝ****" << endl;
		cout << "Satýþ Yapýlacak Hesabý Seçiniz: ";
		cin >> hNo;
		cout << "Satýlacak Miktarý Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] != nullptr) {
			hesaplar[hNo]->sat(m);
			cout << "Satýþ Ýþlemi Baþarýyla Gerçekleþti" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Boyle Bir Hesap Bulunamadý" << endl;
		}
		break;
	case 5: //kullanýcýnýn hangi tarihdeki borsa hareketini görmek istiyorsa oraya gitmesini saðlar
	{
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "*****TARÝH ARALIK SEÇÝM ÝÞLEMÝ*****" << endl;
		cout << "Bugünün Tarihi: " << ctime(&sistemTarihi);

		cout << "Gitmek Ýstediðiniz Tarihi Giriniz (gg/aa/yyyy): ";
		string yeniTarih;
		cin >> yeniTarih;
		tm* t = new tm; // Dinamik olarak bellekte bir tm yapýsý oluþturuldu
		sscanf(yeniTarih.c_str(), "%d/%d/%d", &t->tm_mday, &t->tm_mon, &t->tm_year);
		t->tm_mon -= 1;
		t->tm_year -= 1900;
		t->tm_hour = 0;
		t->tm_min = 0;
		t->tm_sec = 0;

		time_t yeniSistemTarihi = mktime(t);
		if (yeniSistemTarihi > sistemTarihi) {
			double fark = difftime(yeniSistemTarihi, sistemTarihi);
			double gun = fark / (24 * 60 * 60);
			for (int i = 0; i < 200; i++) {
				if (hesaplar[i] != nullptr) {
					for (int j = 0; j < gun; j++) {
						hesaplar[i]->gunlukDegisim();
					}
				}
			}
			sistemTarihi = yeniSistemTarihi;
			cout << "Seçilen Tarihe Gelindi" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Geçmiþ Bir Tarih Seçtiniz" << endl;
		}

		// Belleði serbest býrak
		delete t;
	}
	break;


	case 6:  //kullanýcýnýn açmýþ olduðu hesaplarý görüntüleme bölümü
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "******HESAPLARI LÝSTELEME EKRANI******" << endl;
		cout << "Açýlan Hesaplarýn Bilgileri: " << endl;
		cout << endl;
		for (int i = 0; i < 200; i++) {
			if (hesaplar[i] != nullptr) //nullptr aslýnda bir saðlama yapýyor hesabýn boþ olmadýðýný belirtiyor
			{
				hesaplar[i]->bakiye();
				cout << "Kar: " << hesaplar[i]->kar(sistemTarihi) << endl;
				cout << endl;
				cout << "-----------------------------------------------------------------------------------------" << endl;
			}
		}
		cout << endl;
		break;
	case 7:  //uygulamayý sonlandýrma bölümü
		cout << "*******BORSA KRÝPTODAN ÇIKIÞ YAPILIYOR*******" << endl;
		break;
	default:
		cout << "HATA Geçersiz Bir Seçim Yaptýnýz" << endl;
		cout << endl;
		break;
	}
}

int main() {

	//Türkçe karakter kullanýlmasýna olanak saðlar
	setlocale(LC_ALL, "Turkish");

	Hesap* hesaplar[200] = { nullptr };
	//genel ve özel hesaplar için 100 ile sýnýrlandýrýlmýþ diziler
	Genel genelHesaplar[100];
	Ozel ozelHesaplar[100];
	//sistem tarihini saklar
	time_t sistemTarihi = time(0);
	//kullanýcýnýn ana menüden seçim yapmasýný saðlayan deðiþten
	int secim;

	//kullanýcýnýn gýreceði deðerin kontrolünü saðlayan do while döngüsü
	do {
		anamenu();
		cout << "Yapýlacak Ýþlemi Seçiniz: ";
		cin >> secim;
		cout << endl;
		menuIslemleri(hesaplar, genelHesaplar, ozelHesaplar, sistemTarihi, secim);
	} while (secim != 7);

	

	return 0;
}