### Part 1 Lab 5

1. Find the name of the patron who has checked out the most books.
```sql
SELECT P.Name, COUNT(*) AS NumberOfBooksCheckedOut
FROM CheckedOut C
JOIN Patrons P ON C.CardNum = P.CardNum
GROUP BY P.Name
HAVING COUNT(*) = (
    SELECT MAX(bco)
    FROM (
        SELECT COUNT(*) AS bco
        FROM CheckedOut
        GROUP BY CardNum
    ) AS booksalias
);


```
2. Find the Titles of all books that were written by an author whose name starts with 'K'. You can assume author names always start with an uppercase letter
```sql
Select Titles.Title 
from Titles 
Where Titles.Author like 'k%';
```
3. Find the Authors who have written more than one book. Assume that two Authors with the same name are the same Author for this query.
```sql
SELECT Author
FROM Titles
GROUP BY Author
HAVING COUNT(DISTINCT ISBN) > 1;
```
4. Find the Authors for which the library has more than one book in inventory (this includes multiple copies of the same book). Assume that two Authors with the same name are the same Author for this query.
```sql

SELECT Titles.Author
FROM Inventory
JOIN Titles ON Inventory.ISBN = Titles.ISBN
GROUP BY Titles.Author
HAVING COUNT(*) > 1;

```

5.The library wants to implement a customer loyalty program based on how many books each patron has checked out. Provide an SQL query that returns the names, number of books they have checked out, and loyalty level of each Patron. The loyalty level should be the string "Platinum" if they have checked out > 2 books, "Gold" if they have 2 books, "Silver" if they have 1 book, and "Bronze" if they have no books. Hint: remember that NULL represents an unknown in SQL (it does not represent 0).

```sql
SELECT 
  Patrons.Name,
  COUNT(CheckedOut.Serial) AS NumberOfBooksCheckedOut,
  CASE 
    WHEN COUNT(CheckedOut.Serial) > 2 THEN 'Platinum'
    WHEN COUNT(CheckedOut.Serial) = 2 THEN 'Gold'
    WHEN COUNT(CheckedOut.Serial) = 1 THEN 'Silver'
    ELSE 'Bronze'
  END AS LoyaltyLevel
FROM Patrons
LEFT JOIN CheckedOut ON CheckedOut.CardNum = Patrons.CardNum
GROUP BY Patrons.Name;
```

