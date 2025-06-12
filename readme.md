
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

1. Dodawanie filozofa do kolejki.
2. Sprawdzenie i ewentualny pop() filozofa z kolejki
3. Wykorzystanie widelców przez filozofa

Wszystkie sekcje krytyczne zostały przeze mnie rozwiązane poprzez wprowadzenie mutexów i wykorzystanie klasy lock_guard, tak jak to zostało nadmienione w poprzednich paragrafach

### Wykorzystanie wątków

W moim projekcie wątkami są filozofowie, sekcje krytyczne obejmują obiekty, do których dostęp może mieć wielu filozofów w jednym momencie.
