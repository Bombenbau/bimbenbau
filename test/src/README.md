# Leistungserfassung

## Beschreibung

Zu aller Erst verbindet sich der ESP32 mit dem WLAN-Netzwerk. Mithilfe eines ACS712 werden die Stromstärken der Solarzellen gemessen und an einen Deno Webserver übertragen. Über den Webserver können diese Daten dann ausgelesen und angezeigt werden. Zudem visualisiert der ESP die Stromstärke an zwei LEDs

## Programmablaufplan

```mermaid
graph TD
      A[WLAN-Netzwerk verbinden] --> B[LED's initialisieren]
      B --> C["Ausgangsspannungen der ACS712 (Amperometer) messen"]
      C --> D[Spannungen in Stromstärken umrechnen]
      D --> E[Spannungen der Solarzellen mit der Stromstärke zu Leistung umrechnen]
      E --> F[Leistung an Deno Webserver senden]
      F --> G[Leistung an LEDs visualisieren]
      G --> C
```