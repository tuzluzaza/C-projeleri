/****************************************************************************
**					SAKARYA �N�VERS�TES�
**			B�LG�SAYAR VE B�L���M B�L�MLER� FAK�LTES�
**				    B�LG�SAYAR M�HEND�SL��� B�L�M�
**				PROGRAMLAMAYA G�R��� DERS�
**
**				�DEV NUMARASI: 2
**				��RENC� ADI: ABDULLAH ANIL �OBANER
**				��RENC� NUMARASI: B231210097
**				DERS GRUBU: A
****************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

#pragma warning (disable:4996)   //zaman ile ilgili kulland���m i�lemlerin hata vermesini engelleyen kod (ctime,localtime,sscanf)

using namespace std;

//hesaplarda kullanaca��m�z de�i�kenler i�lemler fonksiyonlar�n bulundu�u hesap s�n�f�
class Hesap {
protected:
	int hesapNo;               //hesap numaras�
	double miktar;             //kripto para miktar�
	double tl;                 //TL miktar�
	double kurdakiDegisim;     //kripto paran�n g�nl�k de�i�im orani
	time_t acilisTarihi;       //hesap a��l�� tarihi
	time_t islemTarihi;        //en son i�lem yap�lan tarih

public:
	//kurucu fonksiyon
	Hesap(int hNo, double m) : hesapNo(hNo), miktar(m), tl(m), kurdakiDegisim(0) { //hNo hesap numaras�n� m miktar� temsil etmektedir
		acilisTarihi = time(0);
		islemTarihi = acilisTarihi;
	}
	//sanal bir fonksiyon kripto paranin g�nl�k de�i�imini ayarlayabilmek i�in kulland�m
	virtual void gunlukDegisim() {

	}
	//kripto para alma fonksiyonu
	void al(double m) {
		miktar += m;
		tl -= m * (1 + kurdakiDegisim);
		islemTarihi = time(0);
	}
	//kripto para satma fonksiyonu satilan fonksiyon TL ye �evrilir
	void sat(double m) {
		miktar -= m;
		tl += m * (1 + kurdakiDegisim);
		islemTarihi = time(0);
	}
	//hesaptaki bakiyeyi g�steren fonksiyon
	void bakiye() {
		cout << "Hesap No: " << hesapNo << endl;
		cout << "Kripto Para Miktari: " << miktar << endl;
		cout << "TL miktari: " << tl << endl;
		cout << "Kurdaki Y�zde De�i�im: %" << kurdakiDegisim * 100 << endl;
		cout << "Hasab�n A��ld��� Tarih: " << ctime(&acilisTarihi);
		cout << "En Son ��lem Yap�lan Tarih: " << ctime(&islemTarihi);
	}
	//kripto paranin borsada kur de�i�imi ile kar/zarar ili�kisini g�steren fonksiyon
	double kar(time_t tarih) {

		return miktar * kurdakiDegisim;
	}
};
//hesap s�n�f�n�n �zelliklerinin genel s�n�f�na kal�t�ld���n� g�sterir
class Genel : public Hesap {
private:
	static const double artisOrani; //genel hesaptaki arti� orani
	static const double dususOrani; //genel hesaptaki d���� orani
	int dususSayaci; // her 15 g�nde bir d���� yapabilmesi i�in atanan de�i�ken

public:
	Genel() : Hesap(0, 0), dususSayaci(0) {}
	Genel(int hNo, double m) : Hesap(hNo, 0), dususSayaci(0) {
		tl = m;
	}
	//bu fonksiyon genel hesab�n �zelli�i olan g�nde y�zde 1 kar etmesini ve 15 g�nde bir y�zde 5 d��mesini sa�lar
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

//Genel s�n�f� i�inde de�i�tirilemeyecek olan artisOrani ve dususOrani de�i�kenlerinin atanmas�
const double Genel::artisOrani = 0.01;
const double Genel::dususOrani = 0.05;

//hesap s�n�f�n�n alt s�n�f� denebilir hesap s�n�f�n�n �zel s�n�fa �zelliklerini kal�tt���n� g�sterir
class Ozel : public Hesap {
private:
	//�zel s�n�f�nda de�i�tirilemeyecek artisOrani ve dususOran� de�i�kenlerini atar
	static const double artisOrani;     //�zel hesaptaki art�� oran�
	static const double dususOrani;     //�zel hesaptaki d���� oran�
	bool dususGunu[30];

public:
	Ozel() : Hesap(0, 0) {
		//30 g�nl�k bir periyotta g�nl�k de�i�im olup olmad���n� kontrol eder 
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
	//�zel hesab�n �zelli�i olan her ay random 3 g�n y�zde 10 d���� olacak rastgele 3 g�n belirlenmesi fonksiyonu
	void rasgeleDususGunuBelirle() {
		srand(time(0)); //her seferinde farkl� g�n belirlemek i�in kullan�l�r
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
		int gun = t->tm_mday - 1;  //bu sat�r 1-31 g�n aras�nda bir de�er belirleyen tm_mday den 1 �ikartarak 0-30 aras�nda bir de�ere sahip olmas�n� sa�lar ki bir ayl�k bir periyotta i�lem yapabilelim
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

//kullan�c�n�n kar��s�na ��kacak ana men� se�im k�sm�
void anamenu() {
	cout << "-----------------------------------------------------------------------------------------";
	cout << endl;
	cout << "              $$ BORSA KR�PTO" << endl << endl << endl;
	cout << "  ��lemler: " << endl;
	cout << "---------------------------------------" << endl;
	cout << "1.)* Genel Hesap A�" << endl;
	cout << "2.)** �zel Hesap A�" << endl;
	cout << "---------------------------------------" << endl;
	cout << "3.)*** Kripto Para Al" << endl;
	cout << "4.)**** Kripto Para Sat" << endl;
	cout << "---------------------------------------" << endl;
	cout << "5.)***** Tarih Aral��� Se�iniz" << endl;
	cout << "6.)****** Hesaplar� Listele" << endl;
	cout << "---------------------------------------" << endl;
	cout << "7.)******* ��k�� Yap" << endl;
	cout << "-----------------------------------------------------------------------------------------";
	cout << endl;
}
//genel ve �zel hesap i�in 100 ile s�n�rland�r�lm�� diziler a��lan hesaplar kay�t edilecek
Genel genelHesaplar[100];
Ozel ozelHesaplar[100];

//kullan�c� taraf�ndan se�ilen bir men� i�lemine g�re hesap i�lemlerini ger�ekle�tiren ana i�levi yerine getirir
void menuIslemleri(Hesap* hesaplar[], Genel genelHesaplar[], Ozel ozelHesaplar[], time_t& sistemTarihi, int secim) {
	int hNo;
	double m;

	//ana men� kullan�c�n�n yapt��� se�ime g�re kar��s�na a��lacak pencereler ve i�lemlerin oldu�u switch case yap�s�
	switch (secim) {

	case 1: //genel hesap a�ma b�l�m�
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "*GENEL HESAP A�MA ��LEM�*" << endl;
		cout << "Hesap Numaran�z� Olu�turun (1-100 arasi numaralar se�ilebilir): ";
		cin >> hNo;
		cout << "Tl Miktar�n�z� Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] == nullptr && hNo > 0 && hNo < 101) {
			genelHesaplar[hNo] = Genel(hNo, m);
			hesaplar[hNo] = &genelHesaplar[hNo];
			cout << "Genel Hesab�n�z Ba�ar�yla A��ld�!" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Bu Hesap Numaras�n� Alamazs�n�z" << endl;
		}
		break;
	case 2: //�zel hesap a�ma b�l�m�
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "**�ZEL HESAP A�MA ��LEM�**" << endl;
		cout << "Hesap Numaran�z� Olu�turun (101-199 arasi numaralar se�ilebilir): ";
		cin >> hNo;
		cout << "Tl Miktar�n�z� Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] == nullptr && hNo > 100 && hNo < 201) {
			ozelHesaplar[hNo - 100] = Ozel(hNo, m);
			hesaplar[hNo] = &ozelHesaplar[hNo - 100];
			cout << "�zel Hesab�n�z Ba�ar�yla A��ld�!" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Bu Hesap Numaras�n� Alamazs�n�z" << endl;
		}
		break;
	case 3: //kripto para alma b�l�m�
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "***KR�PYO PARA ALMA ��LEM�***" << endl;
		cout << "Kripto Para Al�nacak Hesab� Se�iniz: ";
		cin >> hNo;
		cout << "Eklenecek Miktar� Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] != nullptr) {
			hesaplar[hNo]->al(m);
			cout << "Kripto Para Alma ��leminiz Ba�ar�yla Ger�ekle�ti" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA B�yle Bir Hesap Bulunamad�" << endl;
		}
		break;
	case 4:  //kripto para sat�m b�l�m� 
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "****KR�PTO PARA SATMA ��LEM�****" << endl;
		cout << "Sat�� Yap�lacak Hesab� Se�iniz: ";
		cin >> hNo;
		cout << "Sat�lacak Miktar� Giriniz: ";
		cin >> m;
		if (hesaplar[hNo] != nullptr) {
			hesaplar[hNo]->sat(m);
			cout << "Sat�� ��lemi Ba�ar�yla Ger�ekle�ti" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Boyle Bir Hesap Bulunamad�" << endl;
		}
		break;
	case 5: //kullan�c�n�n hangi tarihdeki borsa hareketini g�rmek istiyorsa oraya gitmesini sa�lar
	{
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "*****TAR�H ARALIK SE��M ��LEM�*****" << endl;
		cout << "Bug�n�n Tarihi: " << ctime(&sistemTarihi);

		cout << "Gitmek �stedi�iniz Tarihi Giriniz (gg/aa/yyyy): ";
		string yeniTarih;
		cin >> yeniTarih;
		tm* t = new tm; // Dinamik olarak bellekte bir tm yap�s� olu�turuldu
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
			cout << "Se�ilen Tarihe Gelindi" << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------" << endl;
		}
		else {
			cout << "HATA Ge�mi� Bir Tarih Se�tiniz" << endl;
		}

		// Belle�i serbest b�rak
		delete t;
	}
	break;


	case 6:  //kullan�c�n�n a�m�� oldu�u hesaplar� g�r�nt�leme b�l�m�
		cout << "-----------------------------------------------------------------------------------------" << endl;
		cout << "******HESAPLARI L�STELEME EKRANI******" << endl;
		cout << "A��lan Hesaplar�n Bilgileri: " << endl;
		cout << endl;
		for (int i = 0; i < 200; i++) {
			if (hesaplar[i] != nullptr) //nullptr asl�nda bir sa�lama yap�yor hesab�n bo� olmad���n� belirtiyor
			{
				hesaplar[i]->bakiye();
				cout << "Kar: " << hesaplar[i]->kar(sistemTarihi) << endl;
				cout << endl;
				cout << "-----------------------------------------------------------------------------------------" << endl;
			}
		}
		cout << endl;
		break;
	case 7:  //uygulamay� sonland�rma b�l�m�
		cout << "*******BORSA KR�PTODAN �IKI� YAPILIYOR*******" << endl;
		break;
	default:
		cout << "HATA Ge�ersiz Bir Se�im Yapt�n�z" << endl;
		cout << endl;
		break;
	}
}

int main() {

	//T�rk�e karakter kullan�lmas�na olanak sa�lar
	setlocale(LC_ALL, "Turkish");

	Hesap* hesaplar[200] = { nullptr };
	//genel ve �zel hesaplar i�in 100 ile s�n�rland�r�lm�� diziler
	Genel genelHesaplar[100];
	Ozel ozelHesaplar[100];
	//sistem tarihini saklar
	time_t sistemTarihi = time(0);
	//kullan�c�n�n ana men�den se�im yapmas�n� sa�layan de�i�ten
	int secim;

	//kullan�c�n�n g�rece�i de�erin kontrol�n� sa�layan do while d�ng�s�
	do {
		anamenu();
		cout << "Yap�lacak ��lemi Se�iniz: ";
		cin >> secim;
		cout << endl;
		menuIslemleri(hesaplar, genelHesaplar, ozelHesaplar, sistemTarihi, secim);
	} while (secim != 7);

	

	return 0;
}