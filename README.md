# Progetto: Visual Object Tracking via Feature Extraction

## 1. Architettura Modulare
Il lavoro è suddiviso in tre moduli indipendenti che comunicano tramite interfacce standard (NumPy arrays).

| Modulo | Responsabile | Input | Output |
| :--- | :--- | :--- | :--- |
| **Dataloader** | Collega A | Cartella Frame | Immagine (Matrice RGB/Gray) |
| **Feature Detection** | **TU** | Immagine | Matrice Keypoints + Descrittori |
| **Feature Matching** | Collega B | Descrittori (Frame t, t+1) | Omografia / Nuova BBox |

---

## 2. Dettaglio Modulo: Feature Detection (Tuo Task)

Il tuo obiettivo è implementare una funzione che trasformi i pixel in "impronte digitali" matematiche.

### Step Operativi:
1. **Normalizzazione**: Ricezione frame dal Dataloader e conversione in `grayscale`.
2. **Detection**: Applicazione algoritmo (es. SIFT) per trovare i punti salienti.
3. **Filtering**: Rimozione dei punti con basso contrasto o vicini ai bordi.
4. **Description**: Calcolo del vettore descrittore (es. 128-float per ogni punto).
5. **Consegna**: Restituzione dei dati al modulo di Matching.

### Interfaccia Dati (Il "Contratto"):
Dovrai fornire al collega del Matching un dizionario o un oggetto contenente:
- `keypoints`: Coordinate `(x, y)` dei punti trovati.
- `descriptors`: Matrice `N x 128` (se usi SIFT).

---

## 3. Protocollo di Integrazione

Per garantire che i moduli funzionino insieme, seguire questo workflow di esecuzione per ogni frame:

1. **Inizializzazione (Frame 0)**:
   - Il Dataloader carica il primo frame.
   - Tu estrai le feature **solo all'interno** della Bounding Box iniziale.
   - Il collega del Matching salva questo come "Template Gold".

2. **Loop di Tracking (Frame t > 0)**:
   - Il Dataloader fornisce il frame corrente.
   - **Tu estrai le feature su tutto il frame** (o su una regione di ricerca).
   - Il collega del Matching confronta i tuoi nuovi descrittori con quelli del frame precedente.
   - Viene calcolato lo spostamento e aggiornata la Bounding Box.

---
