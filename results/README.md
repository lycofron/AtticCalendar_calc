# Οδηγίες ανάγνωσης των πινάκων

## Μερικές γενικές πληροφορίες

### Ιουλιανή Ημέρα
Η Ιουλιανή ημέρα είναι ένας τρόπος αναπαράστασης ημερομηνιών. Είναι, πολύ απλά, ένας αριθμός ο οποίος μετράει πόσες ημέρες πέρασαν από την 1η Ιανουαρίου του 4713 π.α.χ.χ. Μονάδα είναι η ημέρα, ενώ η ώρα είναι το δεκαδικό κομμάτι. Αντίθετα από ό,τι έχουμε συνηθίσει, ξεκινάει το μεσημέρι. Έτσι, π.χ. η Ιουλιανή Ημέρα 2460116,5 είναι τα μεσάνυχτα της 21ης Ιουνίου του 2023 ενώ το 2460116 είναι 12 ώρες πριν, άρα μεσημέρι της 20ής Ιουνίου.

Η ημέρα αυτή παρατίθεται για έναν πολύ πρακτικό λόγο: βοηθάει στο να υπολογίσουμε άμεσα πόσες μέρες έχουν περάσει μεταξύ δύο ημερομηνιών χωρίς να χρειάζεται να κάνουμε μετατροπές στις ημερομηνίες.

### Ημερομηνίες π.α.χ.χ.
Ως γνωστόν οι Έλληνες δεν είχαν μηδέν. Κατά συνέπεια, δεν υπάρχει "έτος 0". Το έτος 1 μ.α.χ.χ. ακολουθεί το έτος 1 π.α.χ.χ. χωρίς να μεσολαβεί έτος μηδέν. Επειδή όμως αυτό προκαλεί σύγχυση στους αστρονομικούς υπολογισμούς, έχει καθιερωθεί στην αστρονομία να μεσολαβεί το έτος 0. Οι ημερομηνίες στα αρχεία αυτά ακολουθούν την αστρονομική σύμβαση. Γι' αυτό, από το έτος 0 και πίσω θα πρέπει να αφαιρείται ένα έτος για να βρεθεί η ιστορική χρονολογία. Το 776 π.α.χ.χ., στο αρχείο, φαίνεται ως -775.

### Οι στήλες
Η ημερομηνία κάθε αστρονομικού γεγονότος αναγράφεται με δύο μορφές, η μία μετά την άλλη. Πρώτα σε Ιουλιανή ημερομηνία και μετά σε αστρονομική ημέρα και ώρα, όπου χρειάζεται.

### Το νόημα της "αττικής ημέρας" στα αρχεία
Στο ημερολόγιο το οποίο χρησιμοποιούμε σήμερα, όταν μας πουν ότι κάτι συνέβη την 21η Ιουνίου στις 20:45 ξέρουμε ότι, αφαιρώντας την ώρα, μας μένει η καθαρή ημερομηνία του συμβάντος. Το ίδιο όμως δεν ισχύει στο Αττικό Ημερολόγιο, όπου η μέρα ξεκινά στη δύση του Ήλιου. Με αυτόν τον τρόπο, αν ο ήλιος έχει δύσει στις 20:45, τότε το γεγονός συνέβη την 1η Εκατομβαιώνα της νέας χρονιάς. Αν όμως ο Ήλιος δεν έχει προλάβει να δύσει, τότε το γεγονός συνέβη την 30ή (ή 29η) Θαργηλιώνος της παλιάς χρονιάς.

Όπως βλέπουμε, η Αττική ημέρα δεν συμπίπτει πλήρως με τη σύγχρονη. Αλλά για να αποφύγουμε τον πονοκέφαλο του να δίνουμε δύο ημερομηνίες, δίνονται τα (σύγχρονα) μεσάνυχτα της Αττικής ημέρας στην οποία ανήκει το γεγονός. Στο προηγούμενο παράδειγμά μας, αν ο ο Ήλιος έχει δύσει, άρα έχει ξεκινήσει η επόμενη ημέρα οπότε θα αναγράφονταν τα μεσάνυχτα της 22ας Ιουνίου. Αν όμως ο Ήλιος δεν έχει δύσει, αναγράφονται τα μεσάνυχτα της 21ης.

## Ηλιοστάσια

Το αρχείο `solstices_athens.csv` περιέχει υπολογισμούς για όλα τα θερινά ηλιοστάσια από το 3001 π.α.χ.χ. έως και το 2999 μ.α.χ.χ. Για κάθε ηλιοστάσιο έχουν υπολογιστεί τα παρακάτω γεγονότα:
* Αστρονομικό διεθνές θερινό ηλιοστάσιο, υπολογισμένο με βάση το κέντρο της Γης.
* Μεσάνυχτα αττικής ημέρας του διεθνούς θερινού ηλιοστασίου.
* Αστρονομικό Θερινό ηλιοστάσιο, υπολογισμένο για την ΑΘήνα (και πιο συγκεκριμένα για την Ακρόπολη)
* Μεσάνυχτα αττικής ημέρας του αθηναϊκού θερινού ηλιοστασίου.
* Φαινόμενο θερινό ηλιοστάσιο, υπολογισμένο για την Αθήνα
* Μεσάνυχτα αττικής ημέρας του φαινόμενου θερινού ηλιοστασίου.

### Τι είναι το "φαινόμενο" θερινό ηλιοστάσιο
Όσο πλησιάζει το θερινό ηλιοστάσιο, το σημείο στο οποίο ο Ήλιος δύει, μετατοπίζεται κάθε μέρα βορειότερα, μέχρι που φτάνει το απώτατο σημείο κι αρχίζει να μετατοπίζεται και πάλι προς το νότο. Η μέρα κατά την οποία φτάνει το απώτατο σημείο, είναι το θερινό Ηλιοστάσιο.

## Νέα Σελήνη και Νουμηνία.

Το αρχείο `new_moon_athens.csv` περιέχει υπολογισμούς για κάθε Νέα Σελήνη και Νουμηνία από το 3001 π.α.χ.χ. έως και το 2999 μ.α.χ.χ. Για κάθε Νέα Σελήνη έχουν υπολογιστεί τα παρακάτω γεγονότα:
* Αστρονομική διεθνής Νέα Σελήνη, υπολογισμένη με βάση το κέντρο της Γης.
* Μεσάνυχτα αττικής ημέρας της αστρονομικής, διεθνούς Νέας Σελήνης.
* Αστρονομική Νέα Σελήνη , υπολογισμένη για την ΑΘήνα (και πιο συγκεκριμένα για την Ακρόπολη)
* Μεσάνυχτα αττικής ημέρας της αθηναϊκής Νέας Σελήνης.
* Νουμηνία, δηλαδή εμφάνιση της Νέας Σελήνης στον νυχτερινό Ουρανό, υπολογισμένη για την Αθήνα (Ακρόπολη).
* Μεσάνυχτα αττικής ημέρας της Νουμηνίας






