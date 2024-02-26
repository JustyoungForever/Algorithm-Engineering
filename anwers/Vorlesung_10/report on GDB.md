## Bericht über die Verwendung von GDB und Breakpoints

GDB (GNU Debugger) ist ein mächtiges Werkzeug für das Debugging von Programmen unter Unix-Systemen, das Entwicklern die Möglichkeit bietet, die Ausführung ihrer Programme zu überprüfen und zu steuern. Zu den Vorteilen und Stärken der Verwendung von GDB gehören:

- **Schrittweises Debugging**: Die Fähigkeit, Code zeilenweise auszuführen und dabei die Zustandsänderungen nach jeder Zeile zu beobachten.
- **Anzeigen und Ändern von Variablenwerten**: Während der Ausführung können Werte von Variablen überprüft und modifiziert werden, was für das Testen verschiedener Ausführungspfade nützlich ist.
- **Setzen von Breakpoints**: Überall im Code können Breakpoints gesetzt werden, was beim Auffinden von Fehlern und dem Verständnis des Ausführungsflusses sehr hilfreich ist.
- **Bedingte Breakpoints**: Breakpoints können mit Bedingungen versehen werden, so dass die Ausführung nur dann anhält, wenn diese Bedingungen erfüllt sind.
- **Überprüfung des Aufrufstacks**: Ermöglicht die Sichtung des Funktionsaufrufstacks und das Verständnis der Aufrufreihenfolge und -hierarchie.
- **Automatisierte Tests**: Durch das Schreiben von Skripten können Debugging-Befehle automatisiert ausgeführt werden.

### Weitere Beispiel und Verwendungsmethode:

Betrachten wir den folgenden Codeabschnitt, den wir debuggen möchten:

```c
#include <stdio.h>

int main() {
    int array[5] = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += array[i];
    }
    printf("Summe ist: %d\n", sum);
    return 0;
}
```
* Wir könnten im Schleifenkörper einen Breakpoint setzen, um die Werte von i und sum zu überwachen. In GDB können wir mit dem break Befehl einen Breakpoint an der 5. Zeile setzen:

```bash
(gdb) break 5

##Wenn das Programm an der 5. Zeile ankommt, wird es pausieren und wir können mit dem print Befehl die Werte von i und sum überprüfen:
(gdb) print i
(gdb) print sum

##Falls wir feststellen, dass der Wert von sum nicht korrekt ist, können wir ihn ändern, ohne das Programm zu beenden:
(gdb) set var sum = 10

##Anschließend können wir das Programm mit dem continue Befehl fortsetzen:
(gdb) continue

##Auf diese Weise können wir das Programm dynamisch überwachen und steuern, ohne den Quellcode zu ändern, um Fehler effizienter zu finden und zu beheben.
```