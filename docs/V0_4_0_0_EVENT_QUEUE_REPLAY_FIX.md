# v0.4.0.0 – EventQueue Replay-CPU-Schutz

Ein sofortiges bereits bestätigtes identisches HTTP-200-EventQueue-Batch wird nicht erneut zugestellt und führt nun zu 150ms kontrollierter Wartezeit mit Stop-/Lease-Abbruchprüfung, statt ungebremster CPU-HTTP-Busy-Loop. Der native RegionEventQueue-Test prüft vier sofortige Batch-Duplikate, ACK -1/17/17/17, eine zugestellte Nachricht und mindestens 300ms Pause.

Kein authentifizierter Live-Grid-Test. v0.4.0.0-T02 bleibt IN_ARBEIT, A01-A03 offen. Weiterhin keine Releasefreigabe.
