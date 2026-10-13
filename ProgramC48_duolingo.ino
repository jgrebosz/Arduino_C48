//////////////////////////////////////////////////////////////////////////                   WERSJA 17.09.2025
#include <Adafruit_NeoPixel.h>
#define PIN 13         // pin sterowania taśmy jonowodu
#define PIND 6         // pin sterowania taśmy jonowodu
#define butt_en 8    // pin przycisku jezyka ang
#define butt_pl 9      // pin przycisku jezyka pl
#define NUMPIXELS 649  // Zakładamy, że mamy 649 LEDów
#define NUMPIXELSD 76  // Zakładamy ilosc ledow w duantach
#define NUM_LEDS NUMPIXELS
#define NUM_LEDSD NUMPIXELSD
// Tablica przechowująca stany LEDów: true = zapalony, false = zgaszony
bool ledStates[NUM_LEDS];
bool ledStatesD[NUM_LEDSD];

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);     // tasma led jonowowdu
Adafruit_NeoPixel pixelsD(NUMPIXELSD, PIND, NEO_GRB + NEO_KHZ800);  //Taśma led duantow

const int outputPins[8] = { 15, 16, 17, 18, 19, 20, 21, 22 };  // Piny wyjściowe sterowania modułem Audio
//******************************************************************************************************
void setup() {
  Serial.begin(9600);               // Inicjalizacja portu szeregowego na 9600 baudów
  pinMode(butt_en, INPUT_PULLUP);  // pin wejsciowy dla button en
  bool lang_pl = true;              // flaga dla wyboru jezyka
  pinMode(butt_pl, INPUT_PULLUP);   // pin wejsciowy dla buttona pl

  pinMode(14, INPUT);  // pin wejsciowy dla busy modułu audio

  pinMode(0, OUTPUT);  /// wyjście kanału sterowania 1
  pinMode(1, OUTPUT);  /// wyjście kanału sterowania 2
  pinMode(2, OUTPUT);  /// wyjście kanału sterowania 3
  pinMode(3, OUTPUT);  /// wyjście kanału sterowania 4
  pinMode(4, OUTPUT);  /// wyjście kanału sterowania 5
  pinMode(5, OUTPUT);  /// wyjście kanału sterowania 6
  for (int i = 0; i < 6; i++) {
    digitalWrite(i, HIGH);
    delay(100);
  }  // stan wysoki wyjsc sterowania kanałami 1-6
  for (int i = 0; i < 6; i++) {
    digitalWrite(i, LOW);
    delay(100);
  }  // zerowanie wyjsc sterowania kanałami 1-6


  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);
  }  // defiiowanie wyjsc sterowania modul audio

  pixels.begin();   // INITIALIZE tasma jonowodu
  pixelsD.begin();  // INITIALIZE tasma duantow
  for (int i = 0; i < NUM_LEDS; i++) {
    ledStates[i] = false;
  }
  updateLEDs();
  computeParamValues();
  blink_Duanty();  ///mrugnij duantami
  recznie_ustaw_polarity_dunatow(0);
}
//******************************************************************************************************
// Definicje trybów animacji
#define MODE_SINGLE 0
#define MODE_GROUP 1
#define MODE_MULTI 2

int currentMode = MODE_SINGLE;
int nr_funkcji = 0;
int demo_punkt = 133;
//******************************************************************************************************
void STOP(int linia) {

  Serial.print("STOP w linii ");
  Serial.println(linia);
  while (1)
    ;
}
//******************************************************************************************************
void print_punkt(int linia) {

  Serial.print("w linii ");
  Serial.print(linia);
  Serial.print("  demo_punkt = ");
  Serial.println(demo_punkt);
}
//******************************************************************************************************
void zaczekaj_na_zakonczenie_nagrania()
{
   while (digitalRead(14) == LOW)
            ;
}
//******************************************************************************************************
// byte number = 1;  // Uruchomienie pierwszego pliku Audio o takim numerze. np. 00001.mp3
//******************************************************************************************************
void loop() {
poczatek:


  if (digitalRead(butt_pl) == LOW) {  // po wciśnieciu przycisku wykonanie pierwszej akcji i animacji
                                      //goto skok;

    zgas_wszystkie_LEDy();
    recznie_ustaw_polarity_dunatow(+0);

    //animacja spirali dwa razy w tym momencie

    play_track_number(1);  // (1);  //    // "Coto jest" //////////////////////////////////////////////////////////////
    cykl_animacji(1, MODE_SINGLE);
    //cykl_animacji(1, MODE_GROUP);
    cykl_animacji(1, MODE_MULTI);  // animacja przelotu grupy jonow powtarza się kilka razy aby nie było sytuacji w której animacja trwa po zakończeniu Audio i blokuje przycisk.
    while (digitalRead(14) == LOW) {
      delay(50);
    }
    delay(1000);


    play_track_number(2);  // plik dzwiekowy z pytaniem o przycisniecie powtornie
    zaczekaj_na_zakonczenie_nagrania()
      ;

    Serial.println("Oczekuje na kolejne przyciśniecie");
    // Teraz czekamy na drugie nacisniecie przycisku
    Serial.println("Nacisnij przycisk");
    for (int msek = 0;; ++msek) {
      delay(1);
      if (digitalRead(butt_pl) == LOW) {
        break;
      } else if (msek > 20000) {
        play_track_number(18);
        goto poczatek;
      }
    }

    // JAK TO DZIAŁA
    while (true) {


      recznie_ustaw_polarity_dunatow(0);
      play_track_number(33);  // (3);  // number);  // "Jak to dziala / zasada..." ---------------------------------------------------------
      zaczekaj_na_zakonczenie_nagrania();

      play_track_number(34);
      fioletowe_pole_magnetyczne_spirali();
      zaczekaj_na_zakonczenie_nagrania();

      play_track_number(35);

      blink_Duanty();  ///mrugnij duantami
      zaczekaj_na_zakonczenie_nagrania();


      demo_punkt = 133;                                // miganie lEDEm nr 133
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, 0);  // blinkLEDs(int startLED, int endLED, double frequency, double duration, int polarityMode)
      zapal_led(demo_punkt);                           // Stałe świecenie
      updateLEDs();
      // recznie_ustaw_polarity_dunatow(0);

      play_track_number(4);  //" Najczesciej jest to ..."    //////////////////////////////// Animacja
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(5);  // "Na duanty podajemy..."   //////////////////////////////// Animacja

      recznie_ustaw_polarity_dunatow(+1);
      zaczekaj_na_zakonczenie_nagrania()
        ;


      play_track_number(6);  //" W szczeline "      //////////////////////////////// Animacja

      zaczekaj_na_zakonczenie_nagrania()
        ;
      // print_punkt(__LINE__);
      animuj_punkty(demo_punkt, 2, 1, +1);  // animacja od 134 do +5, animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      // print_punkt(__LINE__);
      // zgas_wszystkie_LEDy();
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 2.0, +1);  /// miganie jonem
      // print_punkt(__LINE__);


      zapal_led(demo_punkt);
      updateLEDs();
      // print_punkt(__LINE__);


      play_track_number(7);  //"Zatrzymajmy sie "     //////////////////////////////// Animacja

      zaczekaj_na_zakonczenie_nagrania()
        ;

      // blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, +1);  /// miganie jonem

      //zgas_wszystkie_LEDy();

      play_track_number(8);  // "Wewnatrz duantu..."    //////////////////////////////// Animacja 6

      //recznie_ustaw_polarity_dunatow(+1);
      animuj_punkty(demo_punkt, 18, 16, +1);

      zaczekaj_na_zakonczenie_nagrania()
        ;
      delay(1000);


      play_track_number(9);  //" Zakrecajac..."  //////////////////////////////// Animacja 7

      zaczekaj_na_zakonczenie_nagrania()
        ;
      recznie_ustaw_polarity_dunatow(-1);

      play_track_number(10);  //" To znaczy zamieniamy ....."  //////////////////////////////// Animacja 8
      zaczekaj_na_zakonczenie_nagrania()
        ;


      play_track_number(11);  //"Dzięki temu jon..."    //////////////////////////////// Animacja
      zaczekaj_na_zakonczenie_nagrania()
        ;
      animuj_punkty(demo_punkt, 3, 2, -1);              //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, -1);  /// miganie jonem

      // zgas_wszystkie_LEDy();


      play_track_number(12);  //"Gdy jon przeleci..."  //////////////////////////////// Animacja


      animuj_punkty(demo_punkt, 14, 10, -1);  //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)

      zgas_wszystkie_LEDy();
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(13);  //"I tak dalej.."   //////////////////////////////// Animacja 10
      demo_punkt += 2;
      blinkLEDs(demo_punkt, demo_punkt + 2, 2.0, 2.0, -1);  /// miganie jonem
      zaczekaj_na_zakonczenie_nagrania()
        ;


      zgas_wszystkie_LEDy();


      cykl_animacji(1, MODE_SINGLE);


      zaczekaj_na_zakonczenie_nagrania()
        ;
      //  animuj_punkty(demo_punkt + 12, 20, 4, -1);  //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      //  zgas_wszystkie_LEDy();


      //////////////////////////////// Animacja 11

      play_track_number(14);  //"Rozpędzony jon..."

      zaczekaj_na_zakonczenie_nagrania()
        ;
      cykl_animacji(1, MODE_GROUP);


      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(15);  //" Jak widać pole ele..."

      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(16);  //" Skoro czestotliwosc ..."

      zaczekaj_na_zakonczenie_nagrania()
        ;
      cykl_animacji(1, MODE_MULTI);

      play_track_number(17);  //" Zakonczenie " koniec

      cykl_animacji(1, MODE_MULTI);
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(18);  //" Muzyka koncowa
      zaczekaj_na_zakonczenie_nagrania()
        ;


      Serial.println(F("Koniec."));
      goto poczatek;
    }
  }

  delay(100);
/////////////////////////////////////////////////////////////// WERSJA ANGIELSKA ////////////////////////////////////////////////
if (digitalRead(butt_en) == LOW) {  // po wciśnieciu przycisku wykonanie pierwszej akcji i animacji
                                      //goto skok;

    zgas_wszystkie_LEDy();
    recznie_ustaw_polarity_dunatow(+0);

    //animacja spirali dwa razy w tym momencie

    play_track_number(101);  // (1);  //    // "Coto jest" //////////////////////////////////////////////////////////////
    cykl_animacji(1, MODE_SINGLE);
    //cykl_animacji(1, MODE_GROUP);
    cykl_animacji(1, MODE_MULTI);  // animacja przelotu grupy jonow powtarza się kilka razy aby nie było sytuacji w której animacja trwa po zakończeniu Audio i blokuje przycisk.
    while (digitalRead(14) == LOW) {
      delay(50);
    }
    delay(1000);


    play_track_number(102);  // plik dzwiekowy z pytaniem o przycisniecie powtornie
    zaczekaj_na_zakonczenie_nagrania()
      ;

    Serial.println("Oczekuje na kolejne przyciśniecie");
    // Teraz czekamy na drugie nacisniecie przycisku
    Serial.println("Nacisnij przycisk");
    for (int msek = 0;; ++msek) {
      delay(1);
      if (digitalRead(butt_en) == LOW) {
        break;
      } else if (msek > 20000) {
        play_track_number(118);
        goto poczatek;
      }
    }

    // JAK TO DZIAŁA
    while (true) {


      recznie_ustaw_polarity_dunatow(0);
      play_track_number(133);  // (3);  // number);  // "Jak to dziala / zasada..." ---------------------------------------------------------
      zaczekaj_na_zakonczenie_nagrania();

      play_track_number(134);
      fioletowe_pole_magnetyczne_spirali();
      zaczekaj_na_zakonczenie_nagrania();

      play_track_number(135);

      blink_Duanty();  ///mrugnij duantami
      zaczekaj_na_zakonczenie_nagrania();


      demo_punkt = 133;                                // miganie lEDEm nr 133
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, 0);  // blinkLEDs(int startLED, int endLED, double frequency, double duration, int polarityMode)
      zapal_led(demo_punkt);                           // Stałe świecenie
      updateLEDs();
      // recznie_ustaw_polarity_dunatow(0);

      play_track_number(104);  //" Najczesciej jest to ..."    //////////////////////////////// Animacja
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(105);  // "Na duanty podajemy..."   //////////////////////////////// Animacja

      recznie_ustaw_polarity_dunatow(+1);
      zaczekaj_na_zakonczenie_nagrania()
        ;


      play_track_number(106);  //" W szczeline "      //////////////////////////////// Animacja

      zaczekaj_na_zakonczenie_nagrania()
        ;
      // print_punkt(__LINE__);
      animuj_punkty(demo_punkt, 2, 1, +1);  // animacja od 134 do +5, animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      // print_punkt(__LINE__);
      // zgas_wszystkie_LEDy();
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 2.0, +1);  /// miganie jonem
      // print_punkt(__LINE__);


      zapal_led(demo_punkt);
      updateLEDs();
      // print_punkt(__LINE__);


      play_track_number(107);  //"Zatrzymajmy sie "     //////////////////////////////// Animacja

      zaczekaj_na_zakonczenie_nagrania()
        ;

      // blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, +1);  /// miganie jonem

      //zgas_wszystkie_LEDy();

      play_track_number(108);  // "Wewnatrz duantu..."    //////////////////////////////// Animacja 6

      //recznie_ustaw_polarity_dunatow(+1);
      animuj_punkty(demo_punkt, 18, 16, +1);

      zaczekaj_na_zakonczenie_nagrania()
        ;
      delay(1000);


      play_track_number(109);  //" Zakrecajac..."  //////////////////////////////// Animacja 7

      zaczekaj_na_zakonczenie_nagrania()
        ;
      recznie_ustaw_polarity_dunatow(-1);

      play_track_number(110);  //" To znaczy zamieniamy ....."  //////////////////////////////// Animacja 8
      zaczekaj_na_zakonczenie_nagrania()
        ;


      play_track_number(111);  //"Dzięki temu jon..."    //////////////////////////////// Animacja
      zaczekaj_na_zakonczenie_nagrania()
        ;
      animuj_punkty(demo_punkt, 3, 2, -1);              //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      blinkLEDs(demo_punkt, demo_punkt, 2.0, 3.0, -1);  /// miganie jonem

      // zgas_wszystkie_LEDy();


      play_track_number(112);  //"Gdy jon przeleci..."  //////////////////////////////// Animacja


      animuj_punkty(demo_punkt, 14, 10, -1);  //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)

      zgas_wszystkie_LEDy();
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(113);  //"I tak dalej.."   //////////////////////////////// Animacja 10
      demo_punkt += 2;
      blinkLEDs(demo_punkt, demo_punkt + 2, 2.0, 2.0, -1);  /// miganie jonem
      zaczekaj_na_zakonczenie_nagrania()
        ;


      zgas_wszystkie_LEDy();


      cykl_animacji(1, MODE_SINGLE);


      zaczekaj_na_zakonczenie_nagrania()
        ;
      //  animuj_punkty(demo_punkt + 12, 20, 4, -1);  //// animacja po przejsciu przez szczeline// animuj_punkty(int selectedLED, int deltaLED, unsigned long duration, int polarity)
      //  zgas_wszystkie_LEDy();


      //////////////////////////////// Animacja 11

      play_track_number(114);  //"Rozpędzony jon..."

      zaczekaj_na_zakonczenie_nagrania()
        ;
      cykl_animacji(1, MODE_GROUP);


      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(115);  //" Jak widać pole ele..."

      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(116);  //" Skoro czestotliwosc ..."

      zaczekaj_na_zakonczenie_nagrania()
        ;
      cykl_animacji(1, MODE_MULTI);

      play_track_number(117);  //" Zakonczenie " koniec

      cykl_animacji(1, MODE_MULTI);
      zaczekaj_na_zakonczenie_nagrania()
        ;

      play_track_number(118);  //" Muzyka koncowa
      zaczekaj_na_zakonczenie_nagrania()
        ;


      Serial.println(F("Koniec."));
      goto poczatek;
    }
  }

  delay(100);
}
//**********************************************************************************
void play_track_number(byte number) {
  Serial.print("Odtwarzamy plik ");
  Serial.println(number);
  for (int i = 0; i < 8; i++) {
    digitalWrite(outputPins[i], ((number >> i) & 1) ? LOW : HIGH);  //
  }
  Serial.println();


  delay(500);
  number = 0;
  for (int i = 0; i < 8; i++) {
    digitalWrite(outputPins[i], ((number >> i) & 1) ? LOW : HIGH);  //
  }
}
//***********************************************************************************
void recznie_ustaw_polarity_dunatow(int state) {
  // tu instrukcje zapalajace duanty

  // state możliwe wartości: -1, 0, +1
  // +1 – np. prawy duant świeci (np. niebiesko), -1 – lewy duant (np. czerwono), 0 – duanty wyłączone
  if (state == -1) {
    for (int ledd = 0; ledd <= 36; ++ledd) {
      pixelsD.setPixelColor(ledd, pixelsD.Color(255, 0, 0));
    }
    for (int ledd = 37; ledd <= 75; ++ledd) {
      pixelsD.setPixelColor(ledd, pixelsD.Color(0, 0, 255));
    }
    pixelsD.show();
  } else if (state == 0) {
    pixelsD.clear();
    pixelsD.show();
  } else if (state == 1) {
    for (int ledd = 0; ledd <= 36; ++ledd) {
      pixelsD.setPixelColor(ledd, pixelsD.Color(0, 0, 255));
    }
    for (int ledd = 37; ledd <= 75; ++ledd) {
      pixelsD.setPixelColor(ledd, pixelsD.Color(255, 0, 0));
    }
    pixelsD.show();
  }
}
//*****************************************************************************
void zgas_wszystkie_LEDy() {
  pixels.clear();
  pixels.show();
  for (int led = 0; led < NUM_LEDS; ++led) {
    ledStates[led] = false;
  }
}
//***********************************************************************************
// void funkcja_spirala() {
//   pixels.clear();
//   for (int i = 0; i < NUMPIXELS; i++) {
//     pixels.setPixelColor(i - 1, pixels.Color(0, 0, 0));
//     pixels.setPixelColor(i, pixels.Color(250 - i, 150, 0));
//     pixels.show();  // Send the updated pixel colors to the hardware.
//     delay(5);       // DELAYVAL); // Pause before next pass through loop
//   }
// }
//************************************************************************************
void cykl_animacji(int ile, int mode) {
  currentMode = mode;
  for (int i = 0; i < ile; ++i) {
    while (updateAnimation()) {
      delay(1);
    }
  }
  recznie_ustaw_polarity_dunatow(0);
}
//******************************************************************************
// Funkcja zapalająca dany LED (możesz rozbudować ją o wysyłanie konkretnego koloru, itp.)
void zapal_led(int led) {
  if (led >= 0 && led < NUM_LEDS) {
    ledStates[led] = true;
  }
}
//***************************************************************************
// Funkcja gasząca dany LED
void zgasil_led(int led) {
  if (led >= 0 && led < NUM_LEDS) {
    ledStates[led] = false;
  }
}
//***************************************************************************
// Funkcja updateLEDs() – wysyła stan tablicy do taśmy LED.
// W przypadku FastLED możesz tu wywołać FastLED.show(); lub analogicznie dla NeoPixel.
void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (ledStates[i]) pixels.setPixelColor(i, pixels.Color(254, 254, 254));
    else pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}
//***************************************************************************
// Funkcja animująca LEDy.
// selectedLED – tablica LEDów (np. {100}), które chcemy animować,
// numSelected – liczba elementów w tablicy,
// deltaLED – o ile pozycji mają się przesunąć (cała animacja składa się z takiej liczby kroków),
// duration – łączny czas animacji w milisekundach.
void animuj_punkty(int& selectedLED, int deltaLED, unsigned long duration, int polarity) {
  int totalSteps = deltaLED;
  if (totalSteps <= 0) return;
  int numSelected = 1;

  recznie_ustaw_polarity_dunatow(polarity);

  // Obliczamy interwał między krokami w milisekundach
  unsigned long interval = 1000.0 * duration / totalSteps;
  for (int currentStep = 0; currentStep <= totalSteps; currentStep++) {
    // Zapalamy LEDy z bieżącego kroku
    for (int i = 0; i < numSelected; i++) {
      int ledIndex = selectedLED + currentStep;
      if (ledIndex < NUM_LEDS) {
        zapal_led(ledIndex);
      }
    }
    // Gasimy LEDy z poprzedniego kroku (jeśli nie pierwszy krok)
    if (currentStep > 0) {
      for (int i = 0; i < numSelected; i++) {
        int ledIndex = selectedLED + currentStep - 1;
        if (ledIndex < NUM_LEDS) {
          zgasil_led(ledIndex);
        }
      }
    }
    updateLEDs();
    delay(interval);  // Czekamy określony interwał przed kolejnym krokiem
  }
  selectedLED += deltaLED;  // TO JEST REFERENCJA
}
//***************************************************************************
void funkcja_animacja_wybranych() {
  // Przykład: animacja jednego LEDa
  // Załóżmy, że chcemy animować LED o numerze 100 przesuwając się o 50 pozycji,
  // w czasie 5000 ms (5 sekund)
  int selectedLED = { 100 };
  int numSelected = 6;
  int deltaLED = 150;
  unsigned long duration = 100;  // w ms
  int polarity_arg = +1;
  animuj_punkty(selectedLED, deltaLED, duration, polarity_arg);
  // Po animacji można zrobić przerwę, np. 1 sekunda
  delay(1000);
}
// Integer  single, group, multi,  i z  polarity + miganie----------------------------------------------
// Wybierz aktualny tryb (zmień na MODE_GROUP lub MODE_MULTI, by przetestować)
// int currentMode = MODE_SINGLE;
// Globalna zmienna polarity: możliwe wartości: -1, 0, +1
// +1 – np. prawy duant świeci (np. niebiesko), -1 – lewy duant (np. czerwono), 0 – duanty wyłączone
int polarity = 1;
// ----- Dane geometryczne spirali -----
// Wektor z indeksami LEDów, przy których zaczyna się kolejny zwój spirali
int ledy_wg_nr[] = { 0, 8, 22, 41, 66, 97, 134, 177, 227, 281, 340, 405, 477 };
const int ledy_wg_nr_size = sizeof(ledy_wg_nr) / sizeof(ledy_wg_nr[0]);
// Tablica liczby LEDów w każdym zwoju (obliczymy ją poniżej)
int led_per_turn[ledy_wg_nr_size - 1];
// Tablica parametrów – dla każdego LEDa przypiszemy wartość (w stopniach)
// odpowiadającą jego położeniu na spirali (część spiralna) lub odcinku prostym.
int paramValues[NUM_LEDS];
// ----- Parametry animacji (dla trybów SINGLE i GROUP) -----
// Faza spiralna: LEDy od 0 do 476
// Odcinek prosty: LEDy od 477 do 648
bool onSpiral = true;
int currentTheta = 0;      // bieżący kąt w fazie spiralnej (w stopniach)
int theta_end = 360 * 12;  // kąt kończący fazę spiralną (np. 360°*12)
int L_progress = 0;        // postęp liniowy po fazie spiralnej
int omega = 10;            // przyrost kąta (w stopniach) przy każdej aktualizacji
int linearSpeed = 10;      // przyrost postępu liniowego (w "stopniach") po fazie spiralnej
int maxParam;              // maksymalna wartość parametru (dla ostatniego LEDa)
int activeIndex = 0;       // numer LEDa, który ma być aktywny (dla trybów SINGLE i GROUP)
// ----- Struktura i zmienne dla trybu MULTI -----
struct Group {
  int progress;  // postęp danej grupy (w tych samych jednostkach co parametry)
};
//******************************************************************************************************
#define MAX_GROUPS 10
Group groups[MAX_GROUPS];
int groupsCount = 0;
int multiGlobalProgress = 0;

//******************************************************************************************************
// ----- Funkcja mapująca parametr na numer LED -----
// Szukamy największego indeksu, dla którego paramValues[i] <= currentParam.
int getActiveIndex(int currentParam) {
  int index = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (paramValues[i] <= currentParam) {
      index = i;
    } else {
      break;
    }
  }
  return index;
}
//******************************************************************************************************
// ----- Obliczenie tablicy paramValues -----
// Dla LEDów w obrębie spirali (indeksy 0 do 476) przypisujemy wartości liniowo rosnące
// w obrębie każdego zwoju – od i*360 do (i+1)*360.
// Dla LEDów w odcinku prostym (indeksy od 477) przypisujemy wartości rosnące o stały krok.
void computeParamValues() {
  // Obliczamy liczbę LEDów w każdym zwoju (dla spirali)
  for (int i = 0; i < ledy_wg_nr_size - 1; i++) {
    led_per_turn[i] = ledy_wg_nr[i + 1] - ledy_wg_nr[i];
  }

  // Dla LEDów w spirali
  for (int i = 0; i < ledy_wg_nr_size - 1; i++) {
    int startLED = ledy_wg_nr[i];
    int endLED = ledy_wg_nr[i + 1];
    int count = led_per_turn[i];
    for (int j = startLED; j < endLED; j++) {
      // Parametr rośnie liniowo od i*360 do (i+1)*360
      paramValues[j] = i * 360 + ((j - startLED) * 360) / count;
    }
  }

  // Dla LEDów w odcinku prostym: od indeksu ledy_wg_nr[ledy_wg_nr_size - 1] do NUM_LEDS - 1
  int stepStraight = 5;  // stały przyrost parametru dla prostego odcinka
  paramValues[ledy_wg_nr[ledy_wg_nr_size - 1]] = (ledy_wg_nr_size - 1) * 360;
  for (int j = ledy_wg_nr[ledy_wg_nr_size - 1] + 1; j < NUM_LEDS; j++) {
    paramValues[j] = paramValues[j - 1] + stepStraight;
  }

  // Maksymalna wartość parametru to wartość przypisana ostatniemu LEDowi
  maxParam = paramValues[NUM_LEDS - 1];
}
//******************************************************************************************************
// ----- Aktualizacja polarity -----
// Dla trybów SINGLE i GROUP (oparta na currentTheta)
// oraz dla trybu MULTI (oparta na multiGlobalProgress).
void updatePolarity(int angleValue) {
  int tol = 2;                   // tolerancja w stopniach
  int angle = angleValue % 360;  // oblicz resztę z dzielenia przez 360
  // Przyjmujemy, że: gdy kąt jest blisko 0° lub 360° -> polarity = +1,
  // gdy kąt jest blisko 90° -> polarity = -1,
  // gdy kąt jest blisko 270° -> polarity = +1,
  // w przeciwnym przypadku duanty się nie świecą (0)

  // Serial.println(angle);
  if (angle < tol || angle > 360 - tol) {
    polarity = 1;
  } else if (abs(angle - 90) < tol) {
    polarity = -1;
  } else if (abs(angle - 270) < tol) {
    polarity = 1;
  } else {
    polarity = 0;
    // Serial.print("angle=");  Serial.println(angle);
    // Serial.println("Polarity ZERO?");
  }
  if (polarity) recznie_ustaw_polarity_dunatow(polarity);
}
//******************************************************************************************************
// ----- Funkcja blinkLEDs -----
// Funkcja migająca LEDami z przedziału [startLED, endLED].
// frequency - liczba mignięć na sekundę,
// duration - czas trwania migania w sekundach,
// polarityMode - tryb polarity, który zostanie ustawiony na czas migania.
//******************************************************************************************************
void blinkLEDs(int startLED, int endLED, double frequency, double duration, int polarityMode) {
  unsigned long period = (unsigned long)(1000.0 / frequency);  // okres w ms
  unsigned long halfPeriod = period / 2;
  unsigned long startTime = millis();
  int originalPolarity = polarity;  // zapamiętujemy oryginalną wartość
  polarity = polarityMode;          // ustawiamy żądany tryb
  recznie_ustaw_polarity_dunatow(polarity);

  while (millis() - startTime < (unsigned long)(duration * 1000)) {
    // Zapalamy LEDy z przedziału
    for (int i = startLED; i <= endLED; i++) {
      zapal_led(i);
    }
    updateLEDs();
    delay(halfPeriod);

    // Gasimy LEDy z przedziału
    for (int i = startLED; i <= endLED; i++) {
      zgasil_led(i);
    }
    updateLEDs();
    delay(halfPeriod);
  }

  // // Po zakończeniu migania przywracamy oryginalną wartość polarity
  // polarity = originalPolarity;
  // recznie_ustaw_polarity_dunatow(polarity);
}
//******************************************************************************************
// ----- Funkcja updateAnimation -------------------------------------------------
// Aktualizuje animację w zależności od trybu (SINGLE, GROUP, MULTI)
int updateAnimation() {
  if (currentMode == MODE_SINGLE || currentMode == MODE_GROUP) {
    // Aktualizacja animacji w trybach SINGLE i GROUP:
    if (onSpiral) {
      currentTheta += omega;
      if (currentTheta >= theta_end) {
        currentTheta = theta_end;
        onSpiral = false;
        L_progress = 0;
      }
    } else {
      currentTheta += omega;
      L_progress += linearSpeed;
    }
    int currentParam = onSpiral ? currentTheta : (theta_end + L_progress);
    if (currentParam >= maxParam) {
      // Reset animacji
      onSpiral = true;
      currentTheta = 0;
      L_progress = 0;
      currentParam = currentTheta;
      return 0;  // czyli stop
    }
    activeIndex = getActiveIndex(currentParam);

    // Aktualizacja polarity na podstawie currentTheta
    // Serial.print("current theta = ");
    // Serial.println(currentTheta);

    updatePolarity(currentTheta);

    // Ustawiamy stany LEDów:
    for (int i = 0; i < NUM_LEDS; i++) {
      if (currentMode == MODE_SINGLE) {
        // Tylko jeden LED zapalony
        ledStates[i] = (i == activeIndex);
      } else if (currentMode == MODE_GROUP) {
        // Zapalamy grupę LEDów (activeIndex oraz kolejne 3 LEDy)
        if (i >= activeIndex && i < activeIndex + 4)
          ledStates[i] = true;
        else
          ledStates[i] = false;
      }
    }

  } else if (currentMode == MODE_MULTI) {
    // Tryb MULTI – zarządzamy wieloma grupami.

    int kiedy_przestac = 8;
    static bool flaga_koncz = false;

    if (groupsCount == 0) {
      groups[groupsCount].progress = 0;
      groupsCount++;
      if (flaga_koncz) {
        flaga_koncz = false;
        return 0;
      }
    } else {

      if (groupsCount >= kiedy_przestac) {
        flaga_koncz = true;
      }
      if (flaga_koncz == false) {
        // Jeśli ostatnia grupa ukończyła drugi zwój (progress >= 720), dodajemy nową grupę.
        if (groups[groupsCount - 1].progress >= 720) {
          if (groupsCount < MAX_GROUPS) {
            groups[groupsCount].progress = 0;
            groupsCount++;
          } else {
          }
        }
      }
    }

    // Aktualizacja postępu każdej grupy
    for (int i = 0; i < groupsCount; i++) {
      if (groups[i].progress < theta_end)
        groups[i].progress += omega;
      else
        groups[i].progress += linearSpeed;
    }

    // Usuwamy grupy, które osiągnęły lub przekroczyły maxParam
    int shift = 0;
    for (int i = 0; i < groupsCount; i++) {
      if (groups[i].progress >= maxParam)
        shift++;
      else if (shift > 0)
        groups[i - shift] = groups[i];
    }
    groupsCount -= shift;
    if (groupsCount < 0)
      groupsCount = 0;

    // Aktualizacja polarity na podstawie globalnego postępu (multiGlobalProgress)
    updatePolarity(multiGlobalProgress);

    // Cache'owanie indeksów dla każdej grupy
    int groupIndices[MAX_GROUPS];
    for (int g = 0; g < groupsCount; g++) {
      groupIndices[g] = getActiveIndex(groups[g].progress);
    }

    // Ustawiamy stany LEDów: LED zapalony, gdy jego numer jest blisko którejś grupy (różnica <= 2)
    for (int i = 0; i < NUM_LEDS; i++) {
      bool lit = false;
      for (int g = 0; g < groupsCount; g++) {
        if (abs(i - groupIndices[g]) <= 2) {
          lit = true;
          break;
        }
      }
      ledStates[i] = lit;
    }

    // Aktualizujemy globalny postęp dla trybu MULTI
    if (multiGlobalProgress < theta_end)
      multiGlobalProgress += omega;
    else
      multiGlobalProgress += linearSpeed;
  }

  updateLEDs();
  return 1;
}
//*******************************************************************************************
//***************************************************************************
// Funkcja updateLEDs() – wysyła stan tablicy do taśmy LED.
// W przypadku FastLED możesz tu wywołać FastLED.show(); lub analogicznie dla NeoPixel.
void fioletowe_pole_magnetyczne_spirali() {
  pixels.setBrightness(1);
  int KONIEC_SPIRALI = 477;
  for (int i = 0; i < KONIEC_SPIRALI; ++i) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 255));
  }
  pixels.setBrightness(1);
  pixels.show();

  // rozjaśnienie
  for (int i = 1; i < 254; ++i) {
    pixels.setBrightness(i);
    pixels.show();
    delay(15);
  }
  //delay(4000);
  // wygaszenie
  for (int i = 254; i > 1; --i) {
    pixels.setBrightness(i);
    pixels.show();
  }
  zgas_wszystkie_LEDy();
  pixels.setBrightness(255);
}
//******************************************************************************************************
void blink_Duanty() {

  int duant[2][2]{
    {
      0,  // pocz ledow PRAWEGO duantu
      36  // kon ledow PRAWEGO duantu
    },
    { 37,   // pocz ledow LEWEGO duantu
      75 }  // kon ledow LEWEGO duantu

  };


  for (int nr = 0; nr < 2; ++nr) {
    for (int raz = 0; raz < 3; ++raz) {  // Trzy mrugnięcia

      // Zapalenie
      for (int ledd = duant[nr][0]; ledd < duant[nr][1]; ++ledd) {
        pixelsD.setPixelColor(ledd, pixelsD.Color(255, 255, 255));
      }

      pixelsD.show();
      delay(100);
      // zagaszenie
      pixelsD.clear();
      pixelsD.show();
      delay(50);
    }
    delay(1000);
  }

  recznie_ustaw_polarity_dunatow(0);
}

//******************************************************************************************