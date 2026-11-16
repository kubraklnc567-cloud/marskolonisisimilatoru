
//
//  main.c
//  MyTool
//
//  Created by KÜBRA on 13.11.2026.
//
//  Mars Kolonisi Yasam Destek Simulatoru
//

// Visual Studio'da scanf icin verilen C4996 uyarisini kapatir (diger derleyicilerde etkisizdir)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Degerleri 0-100 arasinda tutma fonksiyonu
int sinirla(int deger) {
    if (deger > 100) return 100;
    if (deger < 0) return 0;
    return deger;
}

// Okunan girdiden sonra satirin kalanini atar.
// Boylece "dur" gibi cok karakterli bir giris birden fazla komut olarak islenmez.
void satiriTemizle(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int main(void) {
    srand((unsigned int) time(NULL));

    int saglik = 100, enerji = 100, oksijen = 100, modulHasari = 0;
    int su = 0, yedekParca = 0, kilitMuhurlu = 0;
    int gun = 1, tur = 0;
    char zorluk, komut;
    int firtinaTuru = 0;   // Firtinanin kac tur surecegi (zorluga gore)
    int firtinaEsik = 0;   // Firtinada hasar alma esigi (zorluga gore)
    int oyunDevam = 1;

    // Zorluk Seviyesi Secimi
    int gecerliZorluk = 0;
    do {
        printf("Zorluk seviyesi seciniz (K: Kolay, N: Normal, Z: Zor): ");
        if (scanf(" %c", &zorluk) != 1) {
            printf("\nGirdi okunamadi, program sonlandiriliyor.\n");
            return 1;
        }
        satiriTemizle();
        zorluk = (char) toupper((unsigned char) zorluk);

        switch (zorluk) {
            case 'K':
                su = 15; yedekParca = 5; firtinaTuru = 3; firtinaEsik = 45;
                gecerliZorluk = 1;
                break;
            case 'N':
                su = 10; yedekParca = 3; firtinaTuru = 5; firtinaEsik = 60; modulHasari = 20;
                gecerliZorluk = 1;
                break;
            case 'Z':
                su = 5; yedekParca = 1; firtinaTuru = 7; firtinaEsik = 75; modulHasari = 40;
                gecerliZorluk = 1;
                break;
            default:
                printf("Gecersiz seviye! Lutfen tekrar deneyin.\n");
                break;
        }
    } while (!gecerliZorluk);

    // Ana Oyun Dongusu
    do {
        printf("\n--- GUN: %d | TUR: %d ---\n", gun, tur);
        printf("Komut giriniz (G, O, T, D, U, K, H, Q): ");
        if (scanf(" %c", &komut) != 1) {
            // Girdi kapandiysa (EOF) sonsuz donguye girmemek icin cikis komutu gibi davran
            printf("\nGirdi sonlandi.\n");
            komut = 'Q';
        } else {
            satiriTemizle();
        }

        int gecerliTur = 0; // Turu artirip artirmayacagimizi kontrol eder

        // Komut Sistemi
        switch (komut) {
            case 'g':
            case 'G':
                if (enerji < 15) {
                    printf("Uyari: Kesif icin yeterli enerji yok! (Gereken: 15, Mevcut: %d)\n", enerji);
                } else {
                    enerji -= 15;
                    oksijen -= 10;

                    int esik = (enerji > 40 && oksijen > 30) ? 65 : 35;
                    int zar = rand() % 100;
                    printf("Kesif Zari: %d / Esik: %d -> %s\n", zar, esik, (zar < esik) ? "BASARILI" : "BASARISIZ");

                    if (zar < esik) {
                        if (zar % 2 == 0) {
                            su += 2;
                            printf("Sonuc: 2 birim su buldunuz!\n");
                        } else {
                            yedekParca += 1;
                            printf("Sonuc: 1 adet yedek parca buldunuz!\n");
                        }
                    } else {
                        printf("Sonuc: Eliniz bos dondunuz.\n");
                    }

                    int hasarZari = rand() % 100;
                    int yaralandi = (hasarZari < 10 || modulHasari > 70);
                    printf("Yaralanma Zari: %d / Esik: 10 -> %s\n", hasarZari, yaralandi ? "YARALANDI" : "GUVENLI");
                    if (yaralandi) {
                        saglik -= 15;
                        if (hasarZari >= 10) {
                            printf("(Modul hasari 70'in uzerinde oldugu icin zar ne olursa olsun yaralanilir.)\n");
                        }
                        printf("DIKKAT: Kesif sirasinda yaralandiniz! Saglik -15\n");
                    }
                    gecerliTur = 1;
                }
                break;

            case 'o':
            case 'O': {
                int girilenSu = 0;
                printf("Oksijen uretmek icin kullanilacak su miktarini girin (Mevcut: %d): ", su);
                int okunan = scanf("%d", &girilenSu);
                satiriTemizle();

                if (okunan != 1) {
                    printf("Hata: Gecerli bir sayi girmelisiniz!\n");
                } else if (girilenSu < 2) {
                    printf("Hata: Oksijen uretimi icin en az 2 birim su girilmelidir!\n");
                } else if (girilenSu > su) {
                    printf("Hata: Yeterli suyunuz yok!\n");
                } else if (enerji < 5) {
                    printf("Hata: Oksijen uretimi icin en az 5 enerji gerekli!\n");
                } else {
                    enerji -= 5;
                    int islenecekSu = girilenSu;
                    if (girilenSu % 2 != 0) {
                        islenecekSu = girilenSu - 1;
                        printf("Bilgi: %d birim su tek oldugu icin 1 birimi depoda kaldi.\n", girilenSu);
                    }

                    int uretilenOksijen = (islenecekSu / 2) * 3;
                    su -= islenecekSu;
                    oksijen += uretilenOksijen;
                    printf("Sonuc: %d birim su harcanarak %d birim oksijen uretildi.\n", islenecekSu, uretilenOksijen);
                    if (oksijen > 100) {
                        printf("Bilgi: Oksijen deposu doldu, fazla %d birim oksijen kullanilamadi.\n", oksijen - 100);
                        oksijen = 100;
                    }
                    gecerliTur = 1;
                }
                break;
            }

            case 't':
            case 'T':
                if (yedekParca <= 0) {
                    printf("Hata: Hic yedek parcaniz yok!\n");
                } else if (modulHasari >= 71) {
                    if (yedekParca >= 2) {
                        yedekParca -= 2; modulHasari -= 40;
                        printf("Agir hasar onarildi. 2 parca harcandi, Hasar -40.\n");
                        gecerliTur = 1;
                    } else {
                        printf("Agir hasari onarmak icin 2 parca lazim! (Mevcut: %d)\n", yedekParca);
                    }
                } else if (modulHasari >= 31) {
                    yedekParca -= 1; modulHasari -= 25;
                    printf("Orta hasar onarildi. 1 parca harcandi, Hasar -25.\n");
                    gecerliTur = 1;
                } else if (modulHasari > 0) {
                    yedekParca -= 1; modulHasari = 0;
                    printf("Hafif hasar onarildi. 1 parca harcandi, Hasar sifirlandi.\n");
                    gecerliTur = 1;
                } else {
                    // Gecerli bir komut oldugu icin tur harcar (sadece D ve tanimsiz komutlar harcamaz)
                    printf("Modulde hasar bulunmuyor.\n");
                    gecerliTur = 1;
                }
                break;

            case 'd':
            case 'D':
                printf("\n--- DURUM RAPORU ---\n");
                printf("Saglik: %d | Enerji: %d | Oksijen: %d | Hasar: %d\n", saglik, enerji, oksijen, modulHasari);
                printf("Su: %d | Yedek Parca: %d | Hava Kilidi: %s\n", su, yedekParca, kilitMuhurlu ? "Muhurlu" : "Acik");

                if (oksijen < 25) printf("KRITIK UYARI: Oksijen seviyesi cok dusuk!\n");
                if (saglik < 30) printf("KRITIK UYARI: Saglik seviyesi kritik!\n");
                if (modulHasari > 70) printf("KRITIK UYARI: Modul hasari tehlikeli boyutta!\n");

                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70) {
                    printf("TUM SISTEMLER NORMAL\n");
                }
                break;

            case 'u':
            case 'U':
                enerji += 20;
                if (oksijen < 20) {
                    saglik -= 10;
                    printf("Uyari: Oksijen dusukken uyumak sagliginiza zarar verdi! Saglik -10\n");
                } else {
                    // Ic ice IF: oksijen yeterliyse saglik artar, ama zaten tamsa artmaz
                    if (saglik < 100) {
                        saglik += 10;
                        printf("Iyi bir uyku cektiniz. Saglik +10\n");
                    } else {
                        printf("Sagliginiz zaten tam.\n");
                    }
                }
                oksijen -= 5;
                printf("Dinlendiniz. Enerji +20, Oksijen -5\n");
                gecerliTur = 1;
                break;

            case 'k':
            case 'K': {
                printf("\nKUM FIRTINASI BASLIYOR! (%d Tur Surecek)\n", firtinaTuru);
                int hasarAlinanTur = 0;
                int toplamSaglikKaybi = 0;
                int toplamOksijenKaybi = 0;

                for (int i = 1; i <= firtinaTuru; i++) {
                    int zar = rand() % 100;
                    int carpan = (i % 2 == 0) ? 2 : 1; // Cift turlarda hasar iki katina cikar
                    // Esik zorluga gore belirlenir; agir hasarli modul daha az korur
                    int esik = (modulHasari > 70) ? firtinaEsik + 15 : firtinaEsik;

                    printf("- Firtina Turu %d | Zar: %d / Esik: %d -> %s | ", i, zar, esik, (zar < esik) ? "HASAR" : "KORUNDU");

                    if (zar >= esik) {
                        printf("Zararsiz atlatildi.\n");
                    } else if (zar % 3 == 0) {
                        int kayip = 10 * carpan;
                        if (kayip > saglik) kayip = saglik; // Saglik 0'in altina dusmez
                        saglik -= kayip;
                        toplamSaglikKaybi += kayip;
                        hasarAlinanTur++;
                        printf("Enkaz carpti! Saglik -%d\n", kayip);
                    } else if (zar % 3 == 1) {
                        if (kilitMuhurlu) {
                            printf("Hava kilidi muhurlu oldugu icin oksijen sizintisi onlendi.\n");
                            continue;
                        }
                        int kayip = 15 * carpan;
                        if (kayip > oksijen) kayip = oksijen; // Oksijen 0'in altina dusmez
                        oksijen -= kayip;
                        toplamOksijenKaybi += kayip;
                        hasarAlinanTur++;
                        printf("Sizinti! Oksijen -%d\n", kayip);
                    } else {
                        int hasarArtisi = 10 * carpan;
                        modulHasari += hasarArtisi;
                        hasarAlinanTur++;
                        printf("Siddetli ruzgar! Modul Hasari +%d\n", hasarArtisi);
                    }

                    if (saglik <= 0 || oksijen <= 0) {
                        printf("Firtina sirasinda yasam fonksiyonlari durdu!\n");
                        break;
                    }
                }
                printf("Firtina Ozeti: %d turda hasar alindi. Kaybedilen Saglik: %d, Oksijen: %d\n", hasarAlinanTur, toplamSaglikKaybi, toplamOksijenKaybi);
                gecerliTur = 1;
                break;
            }

            case 'h':
            case 'H': {
                if (kilitMuhurlu) {
                    printf("Hava kilidi zaten muhurlu.\n");
                    gecerliTur = 1;
                    break;
                }

                if (enerji < 5) {
                    printf("Hata: Sifre denemesi icin en az 5 enerji gerekli!\n");
                    break;
                }

                char sifre = (char) ('A' + (gun % 26));
                char tahmin = ' ';
                int deneme = 0;
                int dogru = 0;

                printf("Hava Kilidi Sifre Ekrani (Ipucu: Sifre, alfabede A'dan bugunun gun sayisi kadar ileridedir)\n");

                do {
                    printf("%d. Deneme (Harf giriniz): ", deneme + 1);
                    if (scanf(" %c", &tahmin) == 1) {
                        satiriTemizle();
                    } else {
                        tahmin = ' '; // Girdi okunamadiysa yanlis deneme sayilir
                    }
                    tahmin = (char) toupper((unsigned char) tahmin); // Kucuk harf girilirse buyuk harfe cevirir

                    if (tahmin == sifre) {
                        dogru = 1;
                        kilitMuhurlu = 1;
                        printf("Sifre dogru! Hava kilidi muhurlendi.\n");
                    } else {
                        enerji -= 5;
                        deneme++;
                        if (deneme < 3) {
                            printf("Yanlis! Enerji -5. Ipucu: Sifreye gore %s git.\n", (tahmin < sifre) ? "ileri" : "geri");
                        } else {
                            printf("Yanlis! Enerji -5.\n");
                        }
                    }
                } while (dogru == 0 && deneme < 3);

                if (!dogru) {
                    modulHasari += 20;
                    printf("3 basarisiz deneme! Sistem kilitlendi ve Modul Hasari 20 artti.\n");
                }
                gecerliTur = 1;
                break;
            }

            case 'q':
            case 'Q':
                printf("Simulasyondan cikiliyor...\n");
                oyunDevam = 0;
                break;

            default:
                printf("Gecersiz komut! Lutfen menudeki komutlardan birini giriniz.\n");
                break;
        }

        // Zaman Yonetimi
        if (gecerliTur && oyunDevam) {
            tur++;
            // Karakter bu komut sirasinda olduyse yeni gun baslamaz
            if (tur % 3 == 0 && saglik > 0 && oksijen > 0) {
                gun++;
                oksijen -= 8;
                printf("\n>>> YENI GUN (Gun %d). Oksijen -8 azaldi. <<<\n", gun);
                if (modulHasari > 70) {
                    saglik -= 5;
                    printf(">>> Modul hasari 70'in uzerinde: Saglik -5 <<<\n");
                }
            }
        }

        // Sinirlandirma kontrolleri (0'in altina inmesini veya 100'u asmasini onler).
        // Gun gecisindeki kayiplardan da sonra calismasi icin zaman yonetiminin altindadir.
        saglik = sinirla(saglik);
        enerji = sinirla(enerji);
        oksijen = sinirla(oksijen);
        modulHasari = sinirla(modulHasari);

        // Oyun Sonu Kontrolleri
        if (saglik <= 0) {
            printf("\nSAGLIK SIFIRA DUSTU. OLDUNUZ.\n");
            oyunDevam = 0;
        } else if (oksijen <= 0) {
            printf("\nOKSIJEN SIFIRA DUSTU. BOGULDUNUZ.\n");
            oyunDevam = 0;
        } else if (gun >= 16) {
            printf("\n15 GUN HAYATTA KALDINIZ! KURTARMA EKIBI ULASTI! KAZANDINIZ!\n");
            oyunDevam = 0;
        }

    } while (oyunDevam);

    // Sonuc Raporu ve Puan Hesaplama
    // 15. gun tamamlaninca sayac 16 olur; rapor ve puan icin 15 ile sinirlanir.
    int hayattaKalinanGun = (gun > 15) ? 15 : gun;
    int puan = (hayattaKalinanGun * 100) + saglik + oksijen - modulHasari;
    printf("\n==== OYUN SONU RAPORU ====\n");
    printf("Hayatta Kalinan Gun: %d\n", hayattaKalinanGun);
    printf("Toplam Oynanan Tur: %d\n", tur);
    printf("Son Durum -> Saglik: %d | Enerji: %d | Oksijen: %d | Hasar: %d\n", saglik, enerji, oksijen, modulHasari);
    printf("Su: %d | Yedek Parca: %d | Hava Kilidi: %s\n", su, yedekParca, kilitMuhurlu ? "Muhurlu" : "Acik");
    printf("TOPLAM PUAN: %d\n", puan);
    printf("==========================\n");

    return 0;
}
