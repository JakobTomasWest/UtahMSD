using System;
using System.Collections.Generic;
using System.Diagnostics.Tracing;
using System.IO;
using System.Text.RegularExpressions;

public static class PgnReader
{
    // read the pgn and and turn the files lines into a list of chess game objects
    public static List<ChessGame> ReadGames(string filePath)
    {

        var games = new List<ChessGame>();
        var lines = File.ReadAllLines(filePath);
        var tagData = new Dictionary<string, string>();
        var movesBuilder = new List<string>();
        //for each line that we are reading from the Pgn; Lines, lets first see if the line is whitespace, if there is an empty line lets make sure there are tags or moves after it

        // and parse them to create our list of games
        foreach (var line in lines)
        {
            if (string.IsNullOrWhiteSpace(line))
            {
                if (tagData.Count > 0 && movesBuilder.Count > 0)
                {
                    try
                    {
                        games.Add(ParseGame(tagData, movesBuilder));
                        Console.WriteLine($"Parsed game: {CleanTag(tagData, "White")} vs {CleanTag(tagData, "Black")}");

                    }
                    catch (Exception exception)
                    {
                        Console.WriteLine("Skipping the invalid ChessGame Entry " + exception.Message);

                    }
                    tagData.Clear();
                    movesBuilder.Clear();
                }
                continue;
            }
            //deconstruct line and parse ------- (.*) captures everything inside the quotes for the tag value 
            if (line.StartsWith("["))
            //^ we got a valid entry for tag 
            {
                var match = Regex.Match(line, @"\[(\w+)\s+""(.*)""\]");
                if (match.Success)
                {
                    tagData[match.Groups[1].Value] = match.Groups[2].Value;
                }
            }
            else
            {
                movesBuilder.Add(line);
            }

        }
        if (tagData.Count > 0 && movesBuilder.Count > 0)
        {
            try
            {
                games.Add(ParseGame(tagData, movesBuilder));
                Console.WriteLine($"Parsed game: {CleanTag(tagData, "White")} vs {CleanTag(tagData, "Black")}");

            }
            catch (Exception ex)
            {
                Console.WriteLine("Skipping invalid game: " + ex.Message);
            }
        }
        return games;
    }


    // for each line in car lines we are going to 
    private static ChessGame ParseGame(Dictionary<string, string> tags, List<string> moves)
    {
        return new ChessGame
        {
            Event = CleanTag(tags, "Event"),
            Site = CleanTag(tags, "Site"),
            Round = CleanTag(tags, "Round"),
            
            White = CleanTag(tags, "White"),
            Black = CleanTag(tags, "Black"),
            WhiteElo = ParseInt(CleanTag(tags, "WhiteElo")),
            BlackElo = ParseInt(CleanTag(tags, "BlackElo")),
            EventDate = ParseDate(CleanTag(tags, "EventDate")),
            
            Result = ConvertResult(CleanTag(tags, "Result")),
            Moves = string.Join(" ", moves).Trim() 
        };
    }

    private static string CleanTag(Dictionary<string, string> tags, string key)
    {
        if (tags.TryGetValue(key, out string value))
            return value;
        else
            return "";
    }

    private static int ParseInt(string value)
    {
        //parse string, if it's an int then return it as the result, if not return 0 for things like null or ???
        return int.TryParse(value, out int result) ? result : 0;
    }

    private static string ConvertResult(string result)
    {
        return result switch
        {
            "1-0" => "W",
            "0-1" => "B",
            "1/2-1/2" => "D",
            _ => "?"
        };
    }

    private static DateTime ParseDate(string rawDate)
    {
        // Handles dates like "2007.??.??"
        try
        {
            var cleaned = rawDate.Replace("??", "01").Replace('.', '-');
            return DateTime.Parse(cleaned);
        }
        catch
        {
            return DateTime.MinValue;
        }
    }
}
