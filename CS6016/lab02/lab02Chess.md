# Lab 02 - Chess
**Jakob West u0675702**

## Part 1 - ER Diagram for Chess Database

You can find this as Chess.drawio.html in my Lab02 file.

## Part 2 - SQL Tables


```sql
CREATE TABLE Player (
    PlayerID Integer,
    Name Text NOT NULL,
    EloRating Integer NOT NULL
    PRIMARY KEY PlayerID(Integer)
);

Create Table Event (
    EventID Integer,
    Name Text NOT NULL,
    Date Date NOT NULL,
    Site Text NOT NULL,
    PRIMARY KEY EventID (Integer)
    UNIQUE (Name, Date)
);

Create Game Event (
    GameID Integer NOT NULL,
    RoundNumber Integer NOT NULL,
    Result Text NOT NULL Check (Result IN ('1-0', '0-1', '1/2-1/2')),
    Moves Text Check (Length(Moves) <= 1000)
    WhitePlayerID Integer NOT NULL,
    BlackPlayerID Integer NOT NULL,
    EventID Integer NOT NULL,
    PRIMARY KEY (GameID)
    FOREIGN KEY (WhitePlayerID) REFERENCES Player(PlayerID)
    FOREIGN KEY (BlackPlayerID) REFERENCES Player(PlayerID)
    FOREIGN KEY (EventIDFK) REFERENCES Event(EventID)

    Unique (EventID, WhitePlayerID,BlackPlayerID, RoundNumber)
    
);