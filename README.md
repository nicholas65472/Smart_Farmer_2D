# Smart Farmer 2D — C++ / SFML

Smart Farmer 2D este o adaptare digitală a jocului logic Smart Farmer, realizată în C++ cu SFML. Jucătorul trebuie să așeze gardurile pe tabla de joc astfel încât animalele să fie separate corect în țarcuri, respectând cerințele fiecărui nivel.

## Obiectiv

Scopul jocului este rezolvarea fiecărui nivel prin plasarea corectă a gardurilor pe grilă.

Regulile principale sunt:

- animalele de același tip trebuie să fie în același țarc;
- animalele de tipuri diferite nu trebuie să împartă același țarc;
- fiecare țarc trebuie să aibă acces la cel puțin o sursă de apă;
- nivelul trebuie rezolvat înainte ca timpul să expire.

## Tehnologii folosite

- C++17
- SFML Graphics
- SFML Audio
- SFML System
- CMake
- fișiere `.txt` pentru încărcarea nivelurilor și a soluțiilor
- OOP

## Funcționalități

- meniu principal cu opțiunile Start, How to Play și Exit;
- ecran separat pentru selectarea nivelurilor;
- sistem cu 60 de niveluri;
- niveluri încărcate din fișiere text;
- soluții încărcate din fișiere text;
- timer diferit în funcție de dificultatea nivelului;
- plasare și eliminare garduri pe tabla de joc;
- validare automată a soluției;
- buton Solve pentru rezolvarea automată a nivelului;
- afișarea vizuală a gardurilor înainte de ecranul de câștig;
- ecran Win cu opțiuni pentru nivelul următor sau revenire la meniul de niveluri;
- ecran Lose cu opțiuni pentru retry sau revenire la meniul de niveluri;
- sunet on/off;
- muzică de fundal și efecte sonore.
