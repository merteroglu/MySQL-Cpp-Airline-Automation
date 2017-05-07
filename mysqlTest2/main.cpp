// Prolab4.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <iostream>
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
		//stmt->execute("CREATE TABLE IF NOT EXISTS yolcu(ucustarihi VARCHAR(20),UKN INT NOT NULL,TCKN INT NOT NULL,BN INT NOT NULL,BIN INT NOT NULL,PRIMARY KEY (ucustarihi,UKN,TCKN,BN,BIN),FOREIGN KEY(UKN) REFERENCES ucaklar(UKN),FOREIGN KEY(ucustarihi,BN,BIN) REFERENCES seferler(ucustarihi,BN,BIN),FOREIGN KEY(TCKN) REFERENCES yolcular(TCKN))");
		stmt->execute("CREATE TABLE IF NOT EXISTS yolcu(ucustarihi VARCHAR(20) references seferler(ucustarihi) , UKN INT references ucaklar(UKN), TCKN INT references yolcular(TCKN),BN INT references seferler(BN),BIN INT references seferler(BIN), PRIMARY KEY(ucustarihi,UKN,TCKN,BN,BIN) )");

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
	cout << "Yolcu adini giriniz:" << flush;
	cin >> Name;
	cin.clear();
	cout << "Yolcu'nun sehrini giriniz:" << flush;
	cin >> City;
	cin.clear();
	cin.ignore();
	cout << "Yolcunun acik adresini giriniz:" << flush;
	getline(cin, Address);
	cin.clear();
	cout << "Yolcu emailini giriniz:" << flush;
	cin >> email;
	cin.clear();
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
	cout << "Kuyruk No  --  Model  --  Kapasite\n";
	try {
		prpStmt = con->prepareStatement("Select UKN,Model,Kapasite from ucaklar");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << res->getInt("UKN") << "   --   " << res->getString("Model") << "   --   " << res->getString("Kapasite") << endl;
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
	cout << "TCKN   --   Isim   --   Sehir    --   Adres   --   Eposta\n";
	try {
		prpStmt = con->prepareStatement("Select * from yolcular");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << res->getInt("TCKN") << "    --    " << res->getString("Isim") << "    --    " << res->getString("Sehir") << "    --    " << res->getString("Adres") << "     --     " << res->getString("Eposta") << endl;
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
	cout << "Ucus Tarihi --  Ucak Kuyruk No -- Baslangic Sehir kodu -- Bitis Sehir Kodu -- Saat\n";
	try {
		prpStmt = con->prepareStatement("Select * from seferler");
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << res->getString("ucustarihi") << "   --   " << res->getInt("UKN") << "   --   " << res->getInt("BN") << "  --  " << res->getInt("BIN") << "   --  " << res->getString("saat") << endl;
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
		prpStmt = con->prepareStatement("Select * from yolcu where TCKN =" + yTCKN);
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << res->getString("ucustarihi") << " --- " << res->getInt("UKN") << " --- " << res->getInt("BN") << "  ---  " << res->getInt("BIN") << endl;
		}
	}
	catch (sql::SQLException &e) {
		cout << "Yolcu seferleri listenemedi ! Hata : " << e.what() << endl;
	}
	
}

void voyageQuery() {
	sql::PreparedStatement *prpStmt;
	string qDATe; int qUKN;
	string query = "SELECT y.TCKN,y.isim,y.sehir,y.adres,y.eposta FROM yolcu as x,yolcular as y where x.ucustarihi =" + qDATe + "AND x.UKN =" + to_string(qUKN) +
		"AND y.TCKN = x.TCKN";
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

	try {
		prpStmt = con->prepareStatement(query);
		res = prpStmt->executeQuery();
		while (res->next()) {
			cout << res->getString("ucustarihi") << " --- " << res->getInt("UKN") << " --- " << res->getInt("BN") << "  ---  " << res->getInt("BIN") << endl;
		}
	}
	catch (sql::SQLException &e) {
		cout << "Yolcu seferleri listenemedi ! Hata : " << e.what() << endl;
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
		prpStmt = con->prepareStatement("DELETE FROM yolcu where tarih =" + flydate + "AND UKN = " + to_string(ukn) + "AND TCKN = " + to_string(tckn));
		res = prpStmt->executeQuery();
	}
	catch (sql::SQLException &e) {
		cout << "Bilet silme islemi basarisiz ! Hata : " << e.what() << endl;
	}

}

void showMenu() {
	cout << "Islemler Listesi:\n";
	cout << "1. Yeni sefer tanimla\n";
	cout << "2. Bilet Sat\n";
	cout << "3. Yolcu Sorgula\n";
	cout << "4. Ucus Iptali\n";
	cout << "5. Bilet Iptali\n";
	cout << "6. Sefer Sorgula\n";
	cout << "7. Doluluk Oranlarini Listele\n";
	cout << "8. Yolcu Ekle\n";
	cout << "9. Ucak Ekle\n";
	cout << "10.  \n";
	cout << "11. Ucaklari listele\n";
	cout << "12. Yolculari listele\n";
	cout << "13. Seferleri listele\n";
	cout << "15. Cikis\n";
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
			break;
		case 3:
			// Yolcu sorgula
			break;
		case 4:
			// Ucus iptali
			break;
		case 5:
			//Bilet iptali
			break;
		case 6:
			//Sefer Sorgula
			break;
		case 7:
			//Doluluk oranlarini listele
			break;
		case 8:
			getPassengerInfo();
			break;
		case 9:
			//Ucak Ekle
			getAirPlaneInfo();
			break;
		case 10:
			
			
			break;
		case 11:
			listsAirPlanes();
			break;
		case 12:
			listsPassengers();
			break;
		case 13:
			listsVoyages();
			break;
		case 15:
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

