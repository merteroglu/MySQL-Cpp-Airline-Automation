// Prolab4.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "mysql_connection.h"
#include <cppconn\driver.h>
#include <cppconn\exception.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\prepared_statement.h>

using namespace std;
using namespace sql;
sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;


bool connectToDB() {

	try {

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "1470");
	con->setSchema("test");
	stmt = con->createStatement();
	cout << "Baglanti kuruldu !\n";
	}
	catch (sql::SQLException &e) {
	cout << "Db baglanti hatasi !\n ";
	return false;
	}
	

	try {
		stmt->execute("CREATE TABLE IF NOT EXISTS ucaklar(UKN INT NOT NULL,Model VARCHAR(20) NOT NULL,Kapasite INT NOT NULL,PRIMARY KEY (UKN))");
		
	}
	catch (sql::SQLException &e) {
		cout << "Ucaklar Tablosu olusturulamadi !\n";
		return false;
	}
	
	try {
		stmt->execute("CREATE TABLE IF NOT EXISTS yolcular(TCKN INT NOT NULL,Isim VARCHAR(20) NOT NULL,Sehir VARCHAR(20),Adres VARCHAR(100),Eposta VARCHAR(20),PRIMARY KEY (TCKN))");
	}
	catch (sql::SQLException &e) {
		cout << "Yolcular Tablosu olusturulamadi !\n";
		return false;
	}

	try {
		stmt->execute("CREATE TABLE IF NOT EXISTS seferler(ucustarihi VARCHAR(20),UKN INT NOT NULL,BN INT NOT NULL,BIN INT NOT NULL,saat VARCHAR(10),PRIMARY KEY (ucustarihi,UKN),FOREIGN KEY(UKN) REFERENCES ucaklar(UKN))");
	}
	catch (sql::SQLException &e) {
		cout << "Seferler Tablosu olusturulamadi !\n";
		return false;
	}

	try {
		stmt->execute("CREATE TABLE IF NOT EXISTS yolcu(ucustarihi VARCHAR(20),UKN INT NOT NULL,TCKN INT NOT NULL,BN INT NOT NULL, BIN INT NOT NULL, CONSTRAINT fk_yolcu_sefer_tarih FOREIGN KEY(ucustarihi) REFERENCES seferler(ucustarihi) ON DELETE CASCADE,CONSTRAINT fk_yolcu_sefer_ukn FOREIGN KEY(UKN) REFERENCES ucaklar(UKN) ON DELETE CASCADE ,CONSTRAINT fk_yolcu_yolcular FOREIGN KEY(TCKN) REFERENCES yolcular(TCKN) ON DELETE CASCADE )");
	}
	catch (sql::SQLException &e) {
		cout << "Yolcu Tablosu olusturulamadi !" << e.what() << endl;
		return false;
	}


	return true;
}

// -------------------- Add To Database ---------------------------//
void addPassenger(int TCKN, string Name, string City, string Address, string email) {
	sql::PreparedStatement *prpStmt;
	try {
		prpStmt = con->prepareStatement("INSERT INTO yolcular(TCKN,Isim,Sehir,Adres,Eposta) VALUES(?,?,?,?,?)");
		prpStmt->setInt(1, TCKN);
		prpStmt->setString(2, Name);
		prpStmt->setString(3, City);
		prpStmt->setString(4, Address);
		prpStmt->setString(5, email);
		prpStmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		cout << "Yolcu eklenemedi ! : " << e.what() << endl;
	}


}

void getPassengerInfo() {
	int TCKN;
	string Name, City, Address, email;
	string tmp;
	cin.clear();
	cin.ignore();
	cout << "\n\n";
	cout << "Yolcu TC kimlik numarinizi giriniz:" << flush;
	cin >> TCKN;
	cin.clear();
	cin.ignore();
	cout << "Yolcu adini giriniz:" << flush;
	//cin >> Name;
	getline(cin, Name);
	cin.clear();
	cin.ignore();
	cout << "Yolcu'nun sehrini giriniz:" << flush;
	cin >> City;
	cin.clear();
	cin.ignore();
	cout << "Yolcunun acik adresini giriniz:" << flush;
	getline(cin, Address);
	cin.clear();
	cin.ignore();
	cout << "Yolcu emailini giriniz:" << flush;
	cin >> email;
	cin.clear();
	cin.ignore();
	addPassenger(TCKN, Name, City, Address, email);
}

void addAirPlane(int UKN, string Model, int Capacity) {
	sql::PreparedStatement *prpStmt;

	try {
		prpStmt = con->prepareStatement("INSERT INTO ucaklar(UKN,Model,Kapasite) VALUES (?,?,?)");
		prpStmt->setInt(1,UKN);
		prpStmt->setString(2, Model);
		prpStmt->setInt(3, Capacity);
		prpStmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		cout << "Ucak Eklenemedi ! Hata : " << e.what() << endl;
	}
	
}

void getAirPlaneInfo() {
	int ukn, capacity;
	string model;
	cin.clear();
	cout << "\n\n";
	cout << "Ucak kuyruk numarasini giriniz:";
	cin >> ukn;
	cin.clear();
	cout << "Ucak Kapasitesini giriniz:";
	cin >> capacity;
	cin.clear();
	cin.ignore();
	cout << "Ucak Modelini giriniz:";
	getline(cin, model);
	cin.clear();
	addAirPlane(ukn, model, capacity);
	cout << "\n";
}

void addVoyage(int UKN, string DATE, int BN, int BIN, string TIME) {
	sql::PreparedStatement *prpStmt;
	try {
		prpStmt = con->prepareStatement("INSERT INTO seferler(ucustarihi,UKN,BN,BIN,saat) VALUES (?,?,?,?,?)");
		prpStmt->setString(1, DATE);
		prpStmt->setInt(2, UKN);
		prpStmt->setInt(3, BN);
		prpStmt->setInt(4, BIN);
		prpStmt->setString(5, TIME);
		prpStmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		cout << "Sefer Eklenemedi ! Hata : " << e.what() << endl;
	}

}

void getVoyageInfo() {
	cout << "\n\n";
	int ukn, bn, bin;
	string date, time;
	cin.clear();
	cout << "Seferi yapacak ucak kuyruk numarasini giriniz:";
	cin >> ukn;
	cin.clear();
	cin.ignore();
	cout << "Seferin baslangic sehrinin kodunu giriniz:";
	cin >> bn;
	cin.clear();
	cin.ignore();
	cout << "Seferin bitis sehrinin kodunu giriniz:";
	cin >> bin;
	cin.clear();
	cin.ignore();
	cout << "Sefer tarihini giriniz (GGAAYY):";
	cin >> date;
	cin.clear();
	cin.ignore();
	cout << "Sefer saatini giriniz (HHMM):";
	cin >> time;
	cin.clear();
	cin.ignore();
	
	addVoyage(ukn, date, bn, bin, time);
}

// -------------------- Add To Database ---------------------------//


// ---------------- Lists Funcs ------------------------- //
void listsAirPlanes() {
	sql::PreparedStatement *prpStmt;
	cout << "\n\n";
	cout << setiosflags(ios::left);
	cout  << setw(15) << "Kuyruk No" << setw(15) << "Model" << setw(15) << "Kapasite" << endl;
	try {
		prpStmt = con->prepareStatement("Select UKN,Model,Kapasite from ucaklar");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << setiosflags(ios::left);
			cout << setw(15) << res->getInt("UKN") << setw(15) << res->getString("Model") << setw(15) << res->getString("Kapasite") << endl;
		}

	}
	catch (sql::SQLException &e) {
		cout << "Ucaklar listenemedi ! Hata: " << e.what() << endl;
	}
	cout << "\n\n";
}

void listsPassengers() {
	sql::PreparedStatement *prpStmt;

	cout << "\n\n";
	cout << setiosflags(ios::left);
	cout << setw(20) << "TCKN" << setw(20) << "Isim" << setw(20) << "Sehir" << setw(30) << "Adres" << setw(25) << "Eposta" << endl;
	try {
		prpStmt = con->prepareStatement("Select * from yolcular");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout <<setw(20) << res->getInt("TCKN")  << setw(20) << res->getString("Isim") << setw(20) << res->getString("Sehir") << setw(30) << res->getString("Adres")  << setw(25) << res->getString("Eposta") << endl;
		}

	}
	catch (sql::SQLException &e) {
		cout << "Yolcular listenemedi ! Hata: " << e.what() << endl;
	}
	cout << "\n\n";
}

void listsVoyages() {
	sql::PreparedStatement *prpStmt;
	cout << "\n\n";
	cout << setiosflags(ios::left);
	cout << setw(22) << "Ucus Tarihi " << setw(22) << "Ucak Kuyruk No" << setw(22) << "Baslangic Sehir kodu" << setw(22) << "Bitis Sehir Kodu" << setw(22) << "Saat" <<endl;
	try {
		prpStmt = con->prepareStatement("Select * from seferler");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << setw(22) << res->getString("ucustarihi") << setw(22) << res->getInt("UKN") << setw(22) << res->getInt("BN") << setw(22) << res->getInt("BIN") << setw(22) << res->getString("saat") << endl;
		}

	}
	catch (sql::SQLException &e) {
		cout << "Seferler listenemedi ! Hata: " << e.what() << endl;
	}
	cout << "\n\n";
}

// ---------------- Lists Funcs End ------------------------- //

void passengerVoyageQuery() {
	int yTCKN;
	sql::PreparedStatement *prpStmt;
	cin.clear();
	cin.ignore();
	cout << "\n\n";
	cout << "Yolcu TCKN giriniz :";
	cin >> yTCKN;
	cin.clear();
	cin.ignore();

	try {
		prpStmt = con->prepareStatement("Select * from yolcu where TCKN = " + to_string(yTCKN));
		res = prpStmt->executeQuery();
		if (res->rowsCount() == 0) {
			cout << "Yolcu'nun ucus kaydi bulunmamaktadir!\n";
		}
		else {
			cout << setiosflags(ios::left);
			cout << setw(20) << "Ucus Tarihi" << setw(20) << "UKN" << setw(20) << "BN" << setw(20) << "BIN" << endl;
			while (res->next()) {
				cout << setw(20) << res->getString("ucustarihi") << setw(20) << res->getInt("UKN") << setw(20) << res->getInt("BN") << setw(20) << res->getInt("BIN") << endl;
			}
		}
	}
	catch (sql::SQLException &e) {
		cout << "Yolcu seferleri listenemedi ! Hata : " << e.what() << endl;
	}
	
}

void voyageQuery() {
	sql::PreparedStatement *prpStmt;
	string qDATe; int qUKN;
	cin.clear();
	cin.ignore();
	cout << "\n\n";
	cout << "Ucak kuyruk numarasini giriniz:";
	cin >> qUKN;
	cin.clear();
	cin.ignore();
	cout << "Sefer tarihini giriniz (GGAAYY) :";
	cin >> qDATe;
	cin.clear();
	cin.ignore();
	string query = "SELECT y.TCKN , y.isim , y.sehir , y.adres , y.eposta FROM yolcu x, yolcular y where x.ucustarihi = " + qDATe + " AND x.UKN = " + to_string(qUKN) +
		" AND y.TCKN = x.TCKN";

	try {
		prpStmt = con->prepareStatement(query);
		res = prpStmt->executeQuery();
		if (res->rowsCount() == 0) {
			cout << "\nSeferde yolcu bulunmamaktadir ! \n";
			return;
		}
		cout << setiosflags(ios::left);
		cout << setw(20) << "TCKN" << setw(20) << "Isim" << setw(20) << "Sehir" << setw(20) << "Adres" << setw(20) << "Eposta" << endl;
		while (res->next()) {
			cout << setw(20) << res->getInt(1) << setw(20) << res->getString(2) << setw(20) << res->getString(3) << setw(20) << res->getString(4) << setw(20) << res->getString(5) << endl;
		}
	}
	catch (sql::SQLException &e) {
		cout << "Seferde ki yolcular listenemedi ! Hata : " << e.what() << endl;
	}
	

}

void cancelTicket() {
	sql::PreparedStatement *prpStmt;
	int tckn, ukn;
	string flydate;
	cout << "\n\n";
	cin.clear();
	cin.ignore();
	cout << "TC kimlik no giriniz:";
	cin >> tckn;
	cin.clear();
	cin.ignore();
	cout << "Ucak Kuyruk no giriniz:";
	cin >> ukn;
	cin.clear();
	cin.ignore();
	cout << "Ucus tarihini giriniz:";
	cin >> flydate;
	cin.clear();
	cin.ignore();
	
	try {
		prpStmt = con->prepareStatement("DELETE FROM yolcu where ucustarihi = " + flydate + " AND UKN = " + to_string(ukn) + " AND TCKN = " + to_string(tckn));
		res = prpStmt->executeQuery();
	}
	catch (sql::SQLException &e) {
		cout << "Bilet silme islemi basarisiz ! Hata : " << e.what() << endl;
	}

}

bool isPlaneEmpty(int pUKN, string flyDate, int passengerNumber);

void sellaTicket() {
	sql::PreparedStatement *prpStmt;
	int yBN, yBIN, ticketsNumber = 1;
	string flydate;
	int i = 1,selectedFlyNo;
	cout << "\n\n";
	cin.clear();
	cin.ignore();
	cout << "Baslangic sehrinin kodunu giriniz:";
	cin >> yBN;
	cin.clear();
	cin.ignore();
	cout << "Gidilecek sehrin kodunu giriniz:";
	cin >> yBIN;
	cin.clear();
	cin.ignore();
	cout << "Ucus tarihini giriniz:";
	cin >> flydate;
	cin.clear();
	cin.ignore();
	cout << "Bilet sayisini giriniz:";
	cin >> ticketsNumber;
	cin.clear();
	cin.ignore();
	int aktarma = 0;

	try {
		prpStmt = con->prepareStatement("SELECT * FROM seferler WHERE ucustarihi =" + flydate + " AND BN = " + to_string(yBN) + " AND BIN = " + to_string(yBIN));
		res = prpStmt->executeQuery();
		
		if (res->rowsCount() == 0) { // Dogrudan ucus yoksa 1 aktarmali ucuslari listele

			prpStmt = con->prepareStatement("SELECT * FROM seferler as s1, seferler as s2 where s1.BN = " + to_string(yBN) + " AND s1.BIN = s2.BN AND s2.BIN =" + to_string(yBIN) + " AND s1.ucustarihi = " + flydate + " AND str_to_date(s2.ucustarihi,'%d%m%Y' ) >= str_to_date( s1.ucustarihi, '%d%m%Y')");
			res = prpStmt->executeQuery();
			
			if (res->rowsCount() == 0) { // 1 Aktarmali yoksa 2 aktarmali bak
				prpStmt = con->prepareStatement("SELECT * from seferler as s1 , seferler as s2 , seferler as s3 where s1.BN = " + to_string(yBN) + " AND s1.BIN = s2.BN AND s2.BIN = s3.BN AND s3.BIN = " + to_string(yBIN) + " AND s1.ucustarihi = " + flydate + " AND str_to_date(s3.ucustarihi,'%d%m%Y') >= str_to_date(s2.ucustarihi,'%d%m%Y') AND str_to_date(s2.ucustarihi,'%d%m%Y') > str_to_date(s1.ucustarihi,'%d%m%Y')");
				res = prpStmt->executeQuery();
				
				if (res->rowsCount() == 0) {
					aktarma = -1;
				}
				else {
					aktarma = 2;
				}
			}
			else {
				aktarma = 1;
			}
			
		}
		
		
		cout << setiosflags(ios::left);

		if (aktarma == 0) {
			cout << setw(15) << "Ucus No" << setw(15) << "Tarih" << setw(15) << "Ucak No" << setw(15) << "Saat" << endl;
			while (res->next()) {
				cout << setw(15) << i << setw(15) << res->getString(1) << setw(15) << res->getInt(2) << setw(15) << res->getString(5) << endl;
			}
		}
		else if (aktarma == 1) {
			cout << setw(15) <<"Ucus No" << setw(15) << "Tarih 1" << setw(15) << "Kalkis" << setw(15) << "Aktarma" << setw(15) << "Kalkis Saati" << setw(15) << " Tarih 2" << setw(15) << "Inis" << setw(15) << " Kalkis Saati" << endl;
			while (res->next()) {
				cout << setw(15) << i << setw(15) << res->getString(1) << setw(15) << res->getInt(3) << setw(15) << res->getInt(4) << setw(15) << res->getString(5) << setw(15) << res->getString(6) << setw(15) << res->getInt(9) << setw(15) << res->getString(10) << endl;
			}
		}
		else if (aktarma == 2) {
			cout << setw(15) << "Ucus No" << setw(15) << "Tarih 1" << setw(15) << "Kalkis" << setw(15) << "Aktarma" << setw(15) << "Kalkis Saati" << setw(15) << " Tarih 2" << setw(15) << "Inis" << setw(15) << " Kalkis Saati" << setw(15) << "Tarih 3" << setw(15) << "Inis" << setw(15) << "Kalkis Saati" << endl;
			while (res->next()) {
				cout << setw(15) << i << setw(15) << res->getString(1) << setw(15) << res->getInt(3) << setw(15) << res->getInt(4) << setw(15) << res->getString(5) << setw(15) << res->getString(6) << setw(15) << res->getInt(9) << setw(15) << res->getString(10) << setw(15) << res->getString(11) << setw(15) << res->getInt(14) << setw(15) << res->getString(15) << endl;
			}
		}

		
		cout << "Sectiginiz ucus nosunu giriniz:";
		cin >> selectedFlyNo;
		cin.clear();
		cin.ignore();
		
	}
	catch (sql::SQLException &e) {
		cout << "Bilet Satisi 1 ! Hata : " << e.what() << endl;
	}
	
	int tckns[50];
	
	try {
		if (aktarma == 0) {
			prpStmt = con->prepareStatement("SELECT * FROM seferler WHERE ucustarihi = " + flydate + " AND BN = " + to_string(yBN) + " AND BIN = " + to_string(yBIN));
			res = prpStmt->executeQuery();
		}
		else if(aktarma == 1) {
			prpStmt = con->prepareStatement("SELECT * FROM seferler as s1, seferler as s2 where s1.BN = " + to_string(yBN) + " AND s1.BIN = s2.BN AND s2.BIN =" + to_string(yBIN) + " AND s1.ucustarihi = " + flydate  + " AND str_to_date(s2.ucustarihi,'%d%m%Y' ) >= str_to_date( s1.ucustarihi, '%d%m%Y')");
			res = prpStmt->executeQuery();
		}
		else if (aktarma == 2) {
			prpStmt = con->prepareStatement("SELECT * from seferler as s1 , seferler as s2 , seferler as s3 where s1.BN = " + to_string(yBN) + " AND s1.BIN = s2.BN AND s2.BIN = s3.BN AND s3.BIN = " + to_string(yBIN) + " AND s1.ucustarihi = " + flydate + " AND str_to_date(s3.ucustarihi,'%d%m%Y') >= str_to_date(s2.ucustarihi,'%d%m%Y') AND str_to_date(s2.ucustarihi,'%d%m%Y') > str_to_date(s1.ucustarihi,'%d%m%Y')");
			res = prpStmt->executeQuery();
		}

		
		for (int j = 0; j < i; j++) {
			res->next();
		}

		
		
		/*string sctFlyDate = res->getString(1);
		int sctUKN = res->getInt(2);
		int sctBN = res->getInt(3);
		int sctBIN = res->getInt(4);
		*/

		string sctFlyDate[3];
		int sctUKN[3], sctBN[3], sctBIN[3];

		if (aktarma >= 0) {
			sctFlyDate[0] = res->getString(1);
			sctUKN[0] = res->getInt(2);
			sctBN[0] = res->getInt(3);
			sctBIN[0] = res->getInt(4);
		}
		
		if (aktarma >= 1) {
			sctFlyDate[1] = res->getString(6);
			sctUKN[1] = res->getInt(7);
			sctBN[1] = res->getInt(8);
			sctBIN[1] = res->getInt(9);
		}
		
		if (aktarma == 2) {
			sctFlyDate[2] = res->getString(11);
			sctUKN[2] = res->getInt(12);
			sctBN[2] = res->getInt(13);
			sctBIN[2] = res->getInt(14);
		 }

		for (int h = 0; h <= aktarma; h++) {
			if (!isPlaneEmpty(sctUKN[h], sctFlyDate[h], ticketsNumber)) {
				cout << sctUKN[h] << " - Ucak Kapasitesi Dolu! Bilet satilamaz.\n";
				return;
			}
		}
		

		for (int j = 0; j < ticketsNumber; j++) {
			cout << j + 1 << ". Yolcunun TCKN giriniz:";
			cin >> tckns[j];
		}
		

		for (int j = 0; j < ticketsNumber; j++) {
			int tcno = tckns[j];
			prpStmt = con->prepareStatement("SELECT * FROM yolcular WHERE TCKN = " + to_string(tcno));
			res = prpStmt->executeQuery();
			if (res->rowsCount() == 0) {
				cout << j+1 <<".Yolcu kayitli degil! Yolcu bilgilerini giriniz ! \n";
				getPassengerInfo();
			}
			
		}

		for (int j = 0; j < ticketsNumber; j++) {
			for (int n = 0; n <= aktarma; n++) {
				prpStmt = con->prepareStatement("INSERT INTO yolcu(ucustarihi,UKN,TCKN,BN,BIN) VALUES(?,?,?,?,?)");
				prpStmt->setString(1, sctFlyDate[n]);
				prpStmt->setInt(2, sctUKN[n]);
				prpStmt->setInt(3, tckns[j]);
				prpStmt->setInt(4, sctBN[n]);
				prpStmt->setInt(5, sctBIN[n]);
				prpStmt->executeUpdate();
			}
		}

		

	}
	catch (sql::SQLException &e) {
		cout << "Bilet Satisi 2 ! Hata : " << e.what() << endl <<e.getSQLState() << endl;
	}

}

bool isPlaneEmpty(int pUKN,string flyDate,int passengerNumber) {
	sql::PreparedStatement *prpStmt;
	int capacity;
	try {
		prpStmt = con->prepareStatement("SELECT Kapasite FROM ucaklar where UKN = " + to_string(pUKN));
		res = prpStmt->executeQuery();
		res->next();
		capacity = res->getInt(1);
		
		prpStmt = con->prepareStatement("SELECT COUNT(*) FROM yolcu WHERE UKN = " + to_string(pUKN) + " AND ucustarihi = " + flyDate + " GROUP BY UKN,ucustarihi");
		res = prpStmt->executeQuery();
		res->next();
		if (res->rowsCount() == 0) {
			return true;
		}

	//	string fullness = res->getString(1);
		//int fullnes = stoi(fullness);
		int fullnes = res->getInt(1);
	
		if ( (fullnes + passengerNumber) <= capacity) {
			return true;
		}
		else {
			return false;
		}


	}
	catch (sql::SQLException &e) {
		cout << "Ucak Kapasite Kontrolu Basarisiz ! Hata : " << e.what() << endl;
	}

}

void cancelaFly() {
	sql::PreparedStatement *prpStmt;
	int ukn;
	string flydate,flyhour;
	cout << "\n\n";
	cin.clear();
	cin.ignore();
	cout << "Ucak Kuyruk No giriniz:";
	cin >> ukn;
	cin.clear();
	cin.ignore();
	cout << "Ucus Tarihini Giriniz:";
	cin >> flydate;
	cin.clear();
	cin.ignore();
	cout << "Ucus saatini giriniz:";
	cin >> flyhour;
	cin.clear();
	cin.ignore();

	try {
		prpStmt = con->prepareStatement("DELETE FROM seferler where ucustarihi = " + flydate + " AND UKN = " + to_string(ukn) + " AND saat = " + flyhour);
		res = prpStmt->executeQuery();
	}
	catch (sql::SQLException &e) {
		cout << "Sefer iptal iþlemi basarisiz ! Hata : " << e.what() << endl;
	}
}

void listVoyagesFullness() {
	sql::PreparedStatement *prpStmt;
	cout << "\n\n";
	
	try {
		prpStmt = con->prepareStatement("Select distinct *,count(*) from yolcu group by UKN,ucustarihi order by count(*) desc");
		res = prpStmt->executeQuery();
		cout << setiosflags(ios::left);
		cout << setw(20) << "Ucus Tarihi" << setw(20) << "UKN" << setw(20) << "BN" << setw(20) << "BIN" << setw(20) << "Yolcu sayisi" << endl;
		while (res->next()) {
			cout << setw(20) << res->getString(1) << setw(20) << res->getInt(2) << setw(20) << res->getInt(4) << setw(20) << res->getInt(5) << setw(20) << res->getInt(6) << endl;
		}

	}
	catch (sql::SQLException &e) {
		cout << "Seferlerde ki doluluk listenemedi ! Hata: " << e.what() << endl;
	}
	cout << "\n\n";
}

void showMenu() {
	cout << "\n\nIslemler Listesi:\n";

	cout << "1. Yeni sefer tanimla\n"; // OKEY
	cout << "2. Bilet Sat\n";
	cout << "3. Yolcu Sorgula\n"; // OKEY
	cout << "4. Ucus Iptali\n";
	cout << "5. Bilet Iptali\n";  // OKEY
	cout << "6. Sefer Sorgula\n"; // OKEY
	cout << "7. Doluluk Oranlarini Listele\n";
	cout << "8. Yolcu Ekle\n"; // OKEY
	cout << "9. Ucak Ekle\n"; // OKEY
	cout << "10. Ucaklari listele\n"; // OKEY
	cout << "11. Yolculari listele\n"; // OKEY
	cout << "12. Seferleri listele\n"; // OKEY
	cout << "13. Cikis\n";
}

int main()
{
	bool isRunnig = true;
	int userSelect = 0;
	connectToDB();

	while (isRunnig) {
		showMenu();
		cin >> userSelect;

		switch (userSelect)
		{
		case 1:
			//Yeni Sefer Tanimla
			getVoyageInfo();
			break;
		case 2:
			//Bilet sat
			sellaTicket();
			break;
		case 3:
			// Yolcu sorgula
			passengerVoyageQuery();
			break;
		case 4:
			// Ucus iptali
			cancelaFly();
			break;
		case 5:
			//Bilet iptali
			cancelTicket();
			break;
		case 6:
			//Sefer Sorgula
			voyageQuery();
			break;
		case 7:
			//Doluluk oranlarini listele
			listVoyagesFullness();
			break;
		case 8:
			getPassengerInfo(); // yolcu ekle
			break;
		case 9:
			//Ucak Ekle
			getAirPlaneInfo();
			break;
		case 10:
			listsAirPlanes();
			break;
		case 11:
			listsPassengers();
			break;
		case 12:
			listsVoyages();
			break;
		case 13:
			isRunnig = false;
			break;
		default:
			cout << "Hatali giris !! Lutfen yapmak istediginiz islem kodunu giriniz:";
			showMenu();
			break;
		}


	}



	return 0;
}

