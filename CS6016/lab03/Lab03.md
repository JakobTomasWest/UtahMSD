# Part 1 - Joins
Consider the following relations:
*T1*
| A  | Q  | R |
|----|----|---|
| 20 | a  | 5 |
| 25 | b  | 8 |
| 35 | a  | 6 |
*T2*
| A  | B  | C |
|----|----|---|
| 20 | b  | 6 |
| 45 | c  | 3 |
| 20 | b  | 5 |
Provide the relations that result from the following queries.
Your relations should be in the form of a table and should include the schema.

## Schemas

T1(A, Q, R)
T2(A, B, C)


1. $T1 \bowtie_{T1.A = T2.A} T2$    
- conditional join (theta join)
   - **join T1 and T2 where their A values match**
      - matching rows: A = 20
      - Schema: (A, Q, R, A, B, C)
**Answer:**

Result Table: 
| A  | Q  | R | A  | B  | C  |
|---:|---:|--:|---:|---:|---:|
| 20 | a  | 5 | 20 | b  | 6  |
| 20 | a  | 5 | 20 | b  | 5  |


2. $T1 \bowtie_{T1.Q = T2.B} T2$
- conditional join (theta join)
   - **join T1 and T2 where T1's Q values match T2's B values match**
      - matching column row vlaues: Q = b & B = B
      - Schema: (A, Q, R, A, B, C)
**Answer:**

Result Table:
| A  | Q | R | A | B | C |
|---:|--:|--:|--:|--:|--:|
| 25 | b | 8 | 20| b | 6 |
| 25 | b | 8 | 20| b | 5 |

(nabil said that is shouldnt be T1.A & T2.A but just A and A )

3. $T1 \bowtie T2$ (Natural Join)
- Natural join
    - **join T1 and T2 where their are no duplicates**
        - matching rows: A = 20
        - Schema: (A, Q, R, B, C)
**Anser:**

Result Table:
| A  | Q  | R | B  | C |
|----:|----:|---:|----:|----:|
| 20 | a  | 5 | b  | 6 |
| 20 | a  | 5 | b  | 5 |

4. $T1 \bowtie_{T1.A = T2.A \land T1.R = T2.C} T2$
- conditional join (theta join)
    - **join T1 and T2 where T1A = T12 and T1R = T2C**
        -matching rows T1A & T12 and T1R & T2C
        - Schema: (A, Q, R, B, C)

Result Table:
| A | Q | R | B | C |
|--:|--:|--:|--:|--:|
| 20| a | 5 | b | 5 |



# Part 2 - Chess Queries
Consider the schemas below for a Chess database, similar to the one designed in Lab 2.
## Schema
### *Events*
| Name                | Year | eID |
|---------------------|------|-----|
| World Championship  | 1987 | 1   |
| Moscow Open         | 2018 | 2   |
| World Championship  | 2018 | 3   |
### *Players*
| Name             | Elo  | pID |
|------------------|------|-----|
| Magnus Carlsen   | 2882 | 1   |
| Judit Polgar     | 2735 | 2   |
| Fabiano Caruana  | 2844 | 3   |
| Gary Kasparov    | 2851 | 4   |
| Anatoly Karpov   | 2780 | 5   |
### *Games*
| gID | eID | Result   | wpID | bpID |
|-----|-----|----------|------|------|
| 1   | 3   | 1/2-1/2  | 1    | 3    |
| 2   | 3   | 1/2-1/2  | 3    | 1    |
| 3   | 2   | 1-0      | 2    | 1    |
| 4   | 1   | 1/2-1/2  | 4    | 5    |
| 5   | 3   | 0-1      | 3    | 1    |
---
### Relational Algebra Queries (Write below each prompt) 
Key:
Selection σ
Projection π
Renaming ρ
Binary Operators
Union ∪
Intersection ∩
Difference 
Cartesian product ×
Join
Logic Symbols
Logical AND ∧
Logical OR ∨

1. *Find the names of any player with an Elo rating of 2850 or higher.*\
Answer: $π_{Name}(σ_{Elo \geq 2850 (Players)})$

2. *Find the names of any player who has ever played a game as white.*\
Answer: $π_{Name}(Players ⨝_{Players.pID = Games.wpID} Games)$

3. *Find the names of any player who has ever won a game as white.*\
Answer: $π_{Name}(Players ⨝_{Players.pID = Games.wpID} (σ_{Result = '1-0' (Games)}))$

4. *Find the names of any player who played any games in 2018.*\
Answer: $π_{Name}(Players ⨝_{Players.pID = Games.wpID}(σ_{Year = 2018 (Games ⨝ Events)})) ∪  π_{Name}(Players ⨝_{Players.pID = Games.bpID}(σ_{Year = 2018 (Games ⨝ Events)}))$

5. *Find the names and dates of any event in which Magnus Carlsen lost a game.*\
Answer: $π_{Name},_{Year}(σ_{(Games.wpID =1 ∧ Games.Result = '0-1') ∨ (Games.bpID =1 ∧ Games.Result = '1-0')})$

6. *Find the names of all opponents of Magnus Carlsen.*\
Answer: $π_{Name}(Players ⨝_{Player.pID = Games.bID} (σ_{Games.wpID = 1 (Games)}))\cup π_{Name}(Players ⨝_{Players.pID = Games.wpID} (σ_{Games.bpID = 1 (Games)}))$


# Part 3 - LMS Queries
Consider this [basic student database](students.pdf) for a learning management system.
For these problems, assume that student names are unique.
---
## Part 3.1
A. Provide the relation that is the result of the following query.
Your relation should be in the form of a table, and should include the schema.
- $ \rho(C, \pi_{sID}(\sigma_{Grd=c}(\text{Enrolled}))) $
- $ \pi_{Name}((\pi_{sID}(\text{Enrolled}) - C) \bowtie \text{Students}) $


Databases
|Name     |
|--------:|
|Hermione |
|Harry    |

B. Provide a simple English description of what the query is searching for.\
Your description should be in general terms (remember that the original LMS instance data may change).

The Query is looking for all students that are enrolled in atleast one course don't have a 'C' grade in any course.

---
## Part 3.2
A. Provide the relation that is the result of the following query.
Your relation should be in the form of a table, and should include the schema.
- $ \rho(S1, \text{Students}) $
- $ \rho(S2, \text{Students}) $
- $ \pi_{S2.Name}(\sigma_{S1.Name == Ron \land S1.DOB == S2.DOB \land S2.Name != Ron}(S1 \times S2)) $

- Schema: Name

| Name      |
|-----------|
| Hermione  |

B. Provide a simple English description of what the query is searching for.
Your description should be in general terms (remember that the original LMS instance data may change).
The query says to make two copies of the S1 and S2. From there take the cross product of S1 and S2, but filter s1 for the rows with the name Ron, filter where s1 and s2 tables have equoivalent DOBs (keeping people with the same DOB as ron)and then take the Names in s2 that are not Ron. Then project just the name from the S2 rows that still have the same DOB that ron but who are not ron. Basically we should find the names of students who share the same DOB as Ron, but whose name is NOT Ron.

---
## Part 3.3
A. Provide the relation that is the result of the following query.
Your relation should be in the form of a table, and should include the schema.
- $ \pi_{cName} \left( \left( \pi_{cID, sID}(\text{Enroll}) / \pi_{sID}(\text{Students}) \right) \bowtie \text{Courses} \right) $

- Schema: Name

| Name      |

B. Provide a simple English description of what the query is searching for.
Your description should be in general terms (remember that the original LMS instance data may change).

Find the Course Names where all students in the enrollment list have enrolled into a course.

# Part 4
Provide a relational algebra query that uses the divide operator to find the names of all students who are taking all of the 3xxx-level classes.
Find the names of students who are taking all of the 3xxx-level classes

$$ \pi_{Name}(Students \bowtie_{Students.sID = Enroll.sID} \pi_{sID, cID} (Enroll) \div \pi_{cID} ( \sigma_{cID \geq 3000 \land cID < 4000} (Courses)))$$

| Name      |
|-----------|
| Hermione  |
| Ron       |