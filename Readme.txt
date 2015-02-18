EGC - Tema 4
Frustum Culling
Dinu Marian-Alexandru 334 CC

1.Cerinta

Am avut de implementat o scena in care se gaseau multe obiecte (paralelipipede)
texturate si iluminate cu o lumina de tip spotlight. In scena se afla si o sfera,
care se misca printre obiecte, iar la un moment dat sunt vizibile doar obiectele
care apartin frustumului sau intersecteaza frustumul.

2. Utilizare

C = schimbare camera

A = deplasare stanga
D = deplasare dreapta
Z = deplasare sus
X = deplasare jos
W = deplasare inainte
S = deplasare inapoi
U = rotatie Oy
I = rotatie Oy
O = rotatie Ox
P = rotatie Ox
1 = iluminare in fata
2 = iluminare in jos
ESC = inchiderea ferestrei de joc

3. Implementare
IDE - VS 2013
Compilator - GCC 4.5.2
SO - Windows 8.1

5. Probleme aparute
Fixarea pozitie luminii a.i. sa se observe ca este o lumina de tip spot.

6. Continutul arhivei
Am pornit de la Framework-ul din laboratorul cu iluminare si am adaugat cod in 
main si in alte 3 clase: lab_camera, Frustum, Objects.
In lab_camera sunt implementate cele 2 camera impreuna cu modificarile care
intervin asupra acestora.
In object sunt desenate obiectele scenei.
In Frustum sunt create planele care formeaza frustumul, precum si functia care
decide daca un obiect se afla sau nu in frustum. 

7. Functionalitati
Tot ce este specificat in enunt, mai putin textura obiectelor.

Personajul principal este o sfera care se misca prin scena. Lumina porneste din 
ea, iar frustumul este construit relativ la pozitia ei si la punctul spre care 
se priveste. 

La constructia obiectelor am salvat in doi vectori inaltimea, respectiv centrul
fiecarui obiect. Acest lucru m-a ajutat sa determin daca un obiect se afla in frustum.
Functia isInFrustum realizeaza aceasta functionalitate astfel: inscrie fiecare obiect
intr-o sfera de raza = inaltimea / 2 si centru = centru_obiect. Apoi calculeaza distanta
dintre centru si fiecare dintre cele 6 plane ale frustumului. Daca distanta < -raza, 
atunci inseamna ca obiectul respectiv nu intersecteaza frustumul si mai mult decat
atat, este in afara lui. Altfel, obiectul se afla in frustum si va fi desenat.

Pentru a intelege mai bine conceptul de frustum culling si elementele de algebra liniara
care sunt in spatele lui, m-am documentat folosind tutorialele de la adresa [1].

[1] http://www.lighthouse3d.com/tutorials/

