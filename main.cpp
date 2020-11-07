#include <iostream>
#include <conio.h>      //get char
#include <time.h>       // czas
#include <windows.h>
#include <fstream>      //pliki komunikacja
#include <cstdlib>      //dzialanie exit
#include <string>       //operacje na stringach


using namespace std;

int licznik=1;
string duns, generacja, part_number, hardware, raw, wo;

void load_st    (string &duns, string &generacja, string &hardware, string &part_number);
void save_hs    (string raw_file);
void raw_check  (string raw_c);
string wo_save  (string w_o);

int main()
{
    load_st (duns, generacja, hardware, part_number);

    system("Color 1A");
    cout << endl;
    cout << "Wprowadz numer WO i zatwierdz enterem: ";
    getline(cin,wo);
    cout << endl;
    cout << "Numer Zlecenia: "<< wo << endl;

    wo_save(wo);

    cout << "Skanuj barcode: ";
    getline(cin,raw);
    int raw_lnght = raw.length();
    system("CLS");

    if (raw_lnght < 47)
    {
        cout << endl;
        cout << "\t\t  Zeskanowano niewlasciwy barcode !" << endl;
        return 0;
    }

    cout << endl << endl;
    cout << "\t\t\t                             ilosc czesci :" << licznik << endl << endl;

    raw_check (raw);

    save_hs (raw);

    return 0;
}



    // input a work order number, do the destination of SPC file
    string wo_save  (string w_o)
    {
        wo.insert(0,"C:\\SPC\\S_lineFR_RH\\");
        wo += ".txt";
        return wo;
    }

// Loading part settings from settings file
    void load_st (string &duns, string &generacja, string &hardware, string &part_number)
{
    string linia;
    int nr_linii=1;

    fstream ustawienia;
    ustawienia.open("ustawienia.ini", ios::in);

    if(ustawienia.good()==false) cout<<"Nie mozna otworzyc pliku!"; //sprawdza czy plik istnieje

    while (getline(ustawienia, linia))
     {
        switch (nr_linii)
        {
            case 2: duns=linia;            break;
            case 3: generacja=linia;       break;
            case 4: hardware=linia;        break;
            case 5: part_number=linia;     break;

        }
        nr_linii++;
    }
    ustawienia.close();
}
    // get out components from scanned string and compare with correct ones loaded from a settings file.
    void raw_check (string raw_c)
{
    string  pn = raw.substr (1,10);
    string  pd = raw.substr (20,9);
    string  gn = raw.substr (37,3);
    string  hw = raw.substr (41,4);

    if((pn==part_number)&&(pd==duns)&&(gn==generacja)&&(hw==hardware))
    {
        system("Color A1");
        cout << "\t\t\t     CZESC ZGODNA !" << endl << endl;
        cout << endl;
        cout << "\t\t\t   A8 S-line Front RH" << endl << endl;
        licznik++;
    }
    else if (pn!=part_number)
    {
        system("Color E5");
        cout << "\t\t\t     Czesc NIEZGODNA !" << endl << endl;
        cout << "   ZLY PART NUMBER - Sprawdz czy zostal wybrany wlasciwy wariant do produkcji?";
        cout << "    czy zostala zeskanowana wlasciwa czesc ? " << endl << endl;
    }
    else
    {
        system("Color 4D");
        cout << "\t\t\t     Czesc NIEZGODNA !" << endl<< endl;
        cout << "   Sprawdz czy numery: DUNS, GENERACJI lub Hardware sa poprawne ? " << endl << endl;
    }
}

    // Save results to the SPC file
    void save_hs (string raw_file)
{
    time_t _tm = time(NULL);
    struct tm * curtime = localtime ( &_tm );

    fstream historia;
    historia.open(wo.c_str(),ios::out | ios::app);
    historia<<asctime(curtime);
    historia<<raw_file<<endl;
    historia<<"    ";
    historia.close();
    system("PAUSE");
}
