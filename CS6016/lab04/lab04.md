Jakob West u0675702 lab04


### Part 3 Intermediate Retrieval Queries
Connect to the Library database, and create queries to find the specified information below. Some of these are quite simple, and we have already seen them. Note that the Library has some additional information in it to make the queries more interesting. The slides shown in class do not contain the full instances.

```sql
1. Get the ISBNs of all books by <Author> \
Answer: \
SELECT ISBN \
FROM Titles \
WHERE Author = '<Author>'; 


2. Get Serial numbers (descending order) of all books by <ISBN> \
Answer: \
SELECT Serial \
FROM Inventory \
WHERE ISBN = '<ISBN>' \
ORDER BY Serial DESC; 


3. Get the Serial numbers AND ISBNs of all books checked out by <Patron’s name> \
Answer: \
SELECT I.Serial, I.ISBN \
FROM CheckedOut C \
JOIN Patrons P ON C.CardNum = P.CardNum \
JOIN Inventory I ON C.Serial = I.Serial \
WHERE P.Name = '<Patron’s name>';


4. Get phone number(s) AND Name of anyone with <ISBN> checked out \
Answer: \
SELECT DISTINCT P.Name, Ph.Phone \
FROM CheckedOut C \
JOIN Inventory I ON C.Serial = I.Serial \
JOIN Patrons P ON C.CardNum = P.CardNum \
JOIN Phones Ph ON P.CardNum = Ph.CardNum \
WHERE I.ISBN = '<ISBN>';

```

### Part 4 Intermediate Retrieval Queries


1. Find the Authors of the library's oldest <N> books. Assume the lowest serial number is the oldest book.\
```sql
Answer: \
SELECT DISTINCT Titles.Author \
FROM Inventory \
JOIN Titles ON Inventory.ISBN = Titles.ISBN \
ORDER BY Inventory.Serial ASC \
LIMIT <N>;


2. Find the name and phone number of the person who has checked out the most recent book. Assume higher serial numbers are newer. Note that this query is not concerned with the absolute highest serial number, it is concerned with the highest one that has been checked out. \
Answer: \
SELECT Patrons.Name, Phones.Phone \
FROM CheckedOut \
JOIN Patrons ON CheckedOut.CardNum = Patrons.CardNum \
JOIN Phones ON Patrons.CardNum = Phones.CardNum \
WHERE CheckedOut.Serial = (
    SELECT MAX(Serial) FROM CheckedOut
);

3. Find the phone number(s) and name of anyone who has checked out any book. \
Answer: \
SELECT DISTINCT Patrons.Name, Phones.Phone \ 
FROM CheckedOut \
JOIN Patrons ON CheckedOut.CardNum = Patrons.CardNum \ 
JOIN Phones ON Patrons.CardNum = Phones.CardNum;

4. Find the Authors and Titles of the books who have NOT been checked out by anyone. The query should not return duplicates. \
Answer: \
SELECT DISTINCT Titles.Author, Titles.Title \
FROM Inventory \
JOIN Titles ON Inventory.ISBN = Titles.ISBN \
LEFT JOIN CheckedOut ON Inventory.Serial = CheckedOut.Serial \
WHERE CheckedOut.Serial IS NULL; \ 


```


### Part 5 Chess Queries
Provide SQL for the queries below:

```sql
1. Find the names and IDs of any player with the 10 highest Elo ratings. \
Answer: \
SELECT Name, Players.pID \
FROM Players \
ORDER BY Players.ELO DESC \
LIMIT 10;

2. Find the names and Elo ratings of any player who has ever played a game as black. \
Answer: \
SELECT Distinct Players.name, Players.elo
FROM Games
JOIN Players ON Games.blackID = Players.playerID;

3. Find the names of any player who has ever won a game as white. \
Answer: \
SELECT Players.Name \
FROM Games \
JOIN Players ON Games.WhitePlayer = Players.pID \
WHERE Games.Result = 'W'; 

4. Find the names of any player who played any games between 2014 and 2018 in Budapest HUN . \
Answer: \
SELECT Players.Name \
FROM Games \
JOIN Players ON Players.pID = Games.WhitePlayer OR  Players.pID = Games.BlackPlayer \
JOIN Events ON Events.eID = Games.eID WHERE Events.site = 'Budapest HUN' AND Events.date between '2014-01-01' AND '2018-12-31';

5. Find the Sites and dates of any event in which Garry Kasparov won a game. \
Answer: \
SELECT Events.Site, Events.Date \
FROM Games \
JOIN Events ON Games.eID = Events.eID \
JOIN Players ON Players.pID = Games.WhitePlayer OR Players.pID = Games.BlackPlayer \
WHERE Players.Name = 'Kasparov, Garry' \
    AND ((Games.Result = 'W' AND Games.WhitePlayer = Players.pID) 
    OR (Games.Result = 'B' AND Games.BlackPlayer = Players.pID))

6. Find the names of all opponents of Magnus Carlsen. An opponent is someone who he has played a game against. Hint: Both Magnus and his opponents could play as white or black. \
Answer: \
SELECT Opponents.Name \
FROM Players as Magnus \
JOIN Games on Magnus.pID = Games.WhitePlayer OR Magnus.pID = Games.BlackPlayer \
JOIN Players as Opponents \
ON (Magnus.pID = Games.WhitePlayer AND
Opponents.pID = Games.BlackPlayer)  OR (Magnus.pID = Games.BlackPlayer AND Opponents.pID = Games.WhitePlayer) \
WHERE Magnus.Name = "Carlsen, Magnus";

```
