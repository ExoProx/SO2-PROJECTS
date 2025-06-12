
# PROJEKT NR.1 - PROBLEM JEDZĄCYCH FILOZOFÓW

**Problem jedzących filozfów** to problem sformułowany w 1965r. przez  Edsgera Dijkstrę. Jego wyzwaniem jest rozwiązanie problemów z synchronizacją. W problemie posiadamy X filozofów, i X widelców. Każdy z filozofów do jedzenia potrzebuje 2 widelce, jeden z lewej strony drugi z prawej. Możemy więc zauważyć, że 2 filozofów siedzących obok siebie nigdy nie będzie w stanie jeść w tym samym momencie. Przy implementacji sprzętowej tego problemu, wykorzystujemy wielowątkowość do obsługi filozofów, co kreuje problem synchronizacji. Nie możemy pozwolić dwóm wątkom na dostęp do jednego zasobu w tym samym momencie.

W celu rozwiązania tego problemu, wykorzystujemy **mutexy**. Jest to klasa pozwalająca na łatwą synchronizację wątków. W momencie zablokowania danego mutexu przez wątek, żaden inny wątek nie może z niego skorzystać. Pozwala to na zapobieganie wykorzystywaniu jednego widelca przez dwóch filozofów w jednym momencie. Klasa lock_guard pozwala nam na bezpieczne blokowanie mutexów. Działa ona w następujący sposób - podajemy mutex, który chcemy zablokować, a następnie dzieje się jedna z dwóch rzeczy:

1. Dany mutex jest zablokowany - funkcja lock() zostaje zablokowana do momentu, aż mutex nie zostanie odblokowany przez inny wątek.
2. Dany mutex jest wolny - zostaje wywołana funkcja lock() przez obiekt lock_guard, zablokowując mutex.

Po opuszczeniu zakresu, w który stworzyliśmy obiekt lock_guard, obiekt ten zostaje usunięty a sam mutex zwolniony. Dzięki temu nie dojdzie do momentu w którym zapomnimy o zwolnieniu mutexa, dzieje się to automatycznie po skończeniu danego przejścia pętli. 

Równolegle, mamy jeszcze jeden problem do rozwiązania. Takie rozwiązanie nie zapewnia nam, że któryś z filozofów nie zostanie zagłodzony - niedopuszczony do zasobów przez inne wątki. Aby to rozwiązać implementuję kolejke FIFO. W momencie w którym filozof myśli, zapisuje się do kolejki, a w trakcie myślenia sprawdza, czy jest to jego kolej. Jeżeli znajduje się na szczycie kolejki - przechodzi do próby zablokowania mutexów. W przypadku gdy żaden z mutexów nie jest zablokowany przez inny wątek - zaczyna jeść, w innym wypadku oczekuje, aż jego widelce zostaną odblokowane.

### Instrukcja uruchomienia projektu
Aby uruchomić projekt, wystarczy wykorzystać plik Makefile załączony w repozytorium, a następnie wywołać go komendą ./philosophers. W tym celu musimy mieć pobrane program make, oraz g++, które możemy zainstalować komendą 

`sudo apt install make`

oraz 

`sudo apt install g++`

### Sekcje krytyczne

1. Dodawanie filozofa do kolejki - przy diodaniu filozofa do kolejki aplikacja musi zabezpieczyć, aby wiele wątków nie mogło dodawać filozofów w tym samym czasie - osiągamy to blokując mutex blokujący dostęp do listy, i po dodaniu filozofa - ususwany.
2. Sprawdzenie i ewentualny pop() filozofa z kolejki - tak jak powyżej, przy zdejmowaniu/sprawdzaniu filozowa musimy zabezpieczyć, żeby 2 wątki na raz nie sprawdzały stanu kolejki, bo mogą doprowadzić do niechcianej sytuacji. Rozwiązujemy to w ten sam sposób - blokujemy dostęp do listy przy rozpoczęciu sprawdzania, a pod koniec cyklu blokada jest usuwana.
3. Wykorzystanie widelców przez filozofa - do każdego widelca dostęp ma tylko 1 filozof, więc aby nie doprowadzić do niedozwolonego stanu wątki blokują mutex danego widelca - jeśli jest już zablokowany to go pomijają. Jeśli nie jest zablokowany, to po losowym czasie jedzenia - zwalniają widelec i zmieniają stan


### Wykorzystanie wątków

W moim projekcie wątkami są filozofowie, sekcje krytyczne obejmują obiekty, do których dostęp może mieć wielu filozofów w jednym momencie. Każdy wątek ma dostęp do kilku funkcji:
1. Dodanie filozofa do kolejki oczekującej.
2. Sprawdzenie, czy nastąpiła kolej danego filozofa
3. Zablokowanie widelca

Wątki są tworzone funkcją emplace_back, która tworzy obiekty bezpośrednio w pętli w wektorze. Powoduje ona zaosczędzenie zasobów przy tworzeniu obiektów.
