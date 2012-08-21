wwwsh
=====

english
-------
wwwsh - a remote shell over http

This is a proof of concept for a remote shell that works although the target PC
has a installed and active personal firewall.

It was tested with the newest versions of all existing personal firewalls and
none could block this attack.

This software is shared under the terms of the GPL. It is meant as a proof of
concept to show the weakness of personal firewalls. Do not use it to do any
harm.

The shell consists of two parts: the server part running remotely on a computer
under your control and the client part which is dropped as a trojan horse on
the target PC.

Before the client part is started the server has to be up.

The server is a slimed webserver and has nothing special. Actually it formerly 
was an apache webserver using some cgi scripts.

The client uses Windows message system to start a browser and command it. By
surfing webpages of the server's domain information is transported out of the
target PC. To get information back into the target system the server always
replies with a page having a meta refresh tag set so that the browser surfes to
a new location. This location is shown on the browser's window from where the
client can read it. For both inbound and outbound communication information is
base64 encoded and part of the used urls.

The browser window is visible. This is no bug but wanted for demonstartion
purposes. But it is no problem to make the window either not visible for the
user or even completely invisible.

german
------

wwwsh ist ein Programm, dass es einem Angreifer erlaubt, trotz installierter Personal Firewall beliebige Kommandos mit den Rechten des Benutzers auf einem PC auszuführen. Die Remote Shell verwendet einen Browser als Wirt zur Kommunikation mit dem Internet. Zum Starten und zur Fernsteuerung des Browsers wird dem System Benutzerinteraktion mittels Windows-Fensternachrichten vorgetäuscht. Der Kommunikationsendpunkt ist ein Webserver mit ein paar CGI Skripten.

wwwsh startet zu erst einen Browser. Als Beispielanwendung wurde der Internet Explorer ausgewählt, da er der Standardbrowser auf Windows-Systemen ist. Da manche Personal Firewalls das Starten von Prozessen mittels Bibliotheksfunktionen unterbinden, simuliert wwwsh folgende Benutzereingaben:

    * Drücken des Hotkeys <Win>-<R> zum Öffnen des "Ausführen"-Dialogs
    * Eingabe von "%PROGRAMFILES%\Internet Explorer\IEXPLORE.EXE"
    * Drücken der <Enter>-Taste

Diese Vorgehensweise wurde von Volker Birk in der Usenetgruppe de.comp.security.misc bereits vorgestellt.

Mit Hilfe der Windows-Systembibliotheken holt sich wwwsh eine Referenz auf die URL Zeile des Internet Explorers, so dass diese mittels Fensternachrichten manipuliert werden kann. Zum Ansurfen einer URL schreibt wwwsh die URL mittels der Fensternachricht WM_SETTEXT in die URL Zeile und sendet danach eine WM_KEYDOWN mit dem Parameter VK_RETURN, was dem Drücken der Entertaste entspricht. Als Antwort kommt vom Webserver immer eine HTML Seite mit einem Meta-Refresh, was bewirkt, dass der Browser automatisch eine andere URL ansurft. Diese URL erscheint in der URL Zeile, und kann von dort mit einer WM_GETTEXT Fensternachricht ausgelesen werden. Die Informationen sind für beide Kommunikationsrichtungen base64-kodiert in der URL enthalten. Durch diese Kodierung ist sichergestellt, dass keine unzulässigen Zeichen in der URL auftreten können.

wwwsh fragt regelmäßig beim Webserver nach, ob es ein Kommando zum Ausführen gibt. Dazu lässt das Programm den Browser das CGI Skript "/getCommand.cgi" ansurfen. Als Antwort kommt ein Meta-Refresh auf die URL "/Response/". Falls es ein Kommando zum Ausführen gibt, erscheint dieses base64 kodiert als einziger CGI Parameter. Die maximal erlaubte Länge einer URL beschränkt damit die maximale Länge des Kommandos, was aber für übliche Kommandos kein Problem darstellt. Beim Aufrufen der Response-URL wird vom Webserver eine Seite geschickt, die nur dazu dient, dass der Browser seine Antwort erhält und sonst keine weitere Bedeutung hat.

Das empfangene Kommando wird in einer Shell ausgeführt und das Ergebnis base64 kodiert. Zum Übermitteln der Antwort wird zuerst das CGI Skript "/sendResul.cgi" mit dem Parameter "begin" angesurft. Danach folgen beliebig viele Aufrufe des selben Skriptes mit dem Parameter "data" und anschließendem base64 kodiertem Datenteil. Die Länge der Antwort ist damit quasi unbeschränkt. Ist die Antwort komplett übertragen folgt ein letzter Aufruf des Skriptes mit dem Parameter "end". Bei jedem Aufruf sendet der Webserver einen Meta-Refresh auf die Response-URL. Das dient zur Synchronisation der beiden Seiten.

wwwsh ist lediglich eine Machbarkeitsstudie ("Proof of Concept"), daher wurde kein Wert darauf gelegt, das Browserfenster zu verstecken. Das kann bei Bedarf auf unterschiedliche Arten erreicht werden: Erstens könnte man das Fenster aus dem sichtbaren Bereich des Desktops verschieben. Dann bleibt allerdings der Eintrag in der Startleiste. Zweitens könnte man eine Applikation starten, die wie z.B. ein Spiel im Fullscreen Modus läuft, so dass der komplette Desktop nicht einzusehen ist, und danach erst den Browser starten. Drittens könnte man die Fensternachrichten, die zum Anzeigen des Browserfensters führen (WM_SHOWWINDOW), gezielt abfangen. Das ist ein weiteres Feature der Windows-Systembibliotheken.
